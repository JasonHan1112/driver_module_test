#include <linux/module.h>//与module相关的信息
 
#include <linux/kernel.h>
#include <linux/init.h>      //与init相关的函数

#include <linux/fs.h> //file_operation
#include <linux/device.h>
#include <linux/slab.h>//kmalloc
#include <linux/uaccess.h>
#include <linux/io.h>// memmap memunmap
#include <linux/mm.h> //remap_pfn_range

#define dg_printf printk
#define nd_printf printk
#define MEM_MAP_MAJOR 0


static int mem_map_open(struct inode *inode, struct file *filp)
{
    dg_printf("in function:%s\n", __FUNCTION__);
    return 0;
}

static inline int page_is_allowed(unsigned long pfn)//any page is allowed
{
	return 1;
}
static inline unsigned long size_inside_page(unsigned long start,
					     unsigned long size)
{
	unsigned long sz;

	sz = PAGE_SIZE - (start & (PAGE_SIZE - 1));

	return min(sz, size);
}

static void *my_xlate_dev_mem_ptr(phys_addr_t phys)
{
	unsigned long start  = phys &  PAGE_MASK;
	unsigned long offset = phys & ~PAGE_MASK;
	void *vaddr;

	/* memremap() maps if RAM, otherwise falls back to ioremap() */
	vaddr = memremap(start, PAGE_SIZE, MEMREMAP_WB);

	/* Only add the offset on success and return NULL if memremap() failed */
	if (vaddr)
		vaddr += offset;

	return vaddr;
}

static void my_unxlate_dev_mem_ptr(phys_addr_t phys, void *addr)
{
	memunmap((void *)((unsigned long)addr & PAGE_MASK));
}



static ssize_t mem_map_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{ 
    //dg_printf("in function:%s\n", __FUNCTION__);

/////////////////////////////////////////modified
    phys_addr_t p = *ppos;//offset to start of the file
	ssize_t read, sz;
	void *ptr;
	char *bounce;
	int err;

	if (p != *ppos)//check ppos size if overflow
		return 0;
    dg_printf("loff_t ppos 0x%llx", p);

    //need check addr_range
	read = 0;

	bounce = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!bounce)
		return -ENOMEM;

	while (count > 0) {
		unsigned long remaining;
		int allowed, probe;

		sz = size_inside_page(p, count);

		err = -EPERM;
		allowed = page_is_allowed(p >> PAGE_SHIFT);//any page is allowed

		err = -EFAULT;
		ptr = my_xlate_dev_mem_ptr(p);
		if (!ptr)
			goto failed;

		probe = probe_kernel_read(bounce, ptr, sz);
		my_unxlate_dev_mem_ptr(p, ptr);
		if (probe)
			goto failed;

		remaining = copy_to_user(buf, bounce, sz);

		if (remaining)
			goto failed;

		buf += sz;
		p += sz;
		count -= sz;
		read += sz;
	}
	kfree(bounce);

	*ppos += read;
	return read;

failed:
	kfree(bounce);
	return err;
/////////////////////////////////////////modified

}

static ssize_t mem_map_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
    //dg_printf("in function:%s\n", __FUNCTION__);

/////////////////////////////////////////modified
    phys_addr_t p = *ppos;
	ssize_t written, sz;
	unsigned long copied;
	void *ptr;

	if (p != *ppos)
		return -EFBIG;
    dg_printf("loff_t ppos 0x%llx", p);

	written = 0;
	while (count > 0) {
		int allowed;

		sz = size_inside_page(p, count);

		allowed = page_is_allowed(p >> PAGE_SHIFT);
		if (!allowed)
			return -EPERM;

		ptr = my_xlate_dev_mem_ptr(p);
		if (!ptr) {
			if (written)
				break;
			return -EFAULT;
		}

		copied = copy_from_user(ptr, buf, sz);
		my_unxlate_dev_mem_ptr(p, ptr);
		if (copied) {
			written += sz - copied;
			if (written)
				break;
			return -EFAULT;
		}

		buf += sz;
		p += sz;
		count -= sz;
		written += sz;
	}

	*ppos += written;
	return written;
/////////////////////////////////////////modified

}

static int uncached_access(struct file *file, phys_addr_t addr)
{
	/*
	 * Accessing memory above the top the kernel knows about or through a
	 * file pointer
	 * that was marked O_DSYNC will be done non-cached.
	 */
	if (file->f_flags & O_DSYNC)
		return 1;
	return addr >= __pa(high_memory);
}

static pgprot_t my_phys_mem_access_prot(struct file *file, unsigned long pfn,
				     unsigned long size, pgprot_t vma_prot)
{
	phys_addr_t offset = pfn << PAGE_SHIFT;

	if (uncached_access(file, offset))
		return pgprot_noncached(vma_prot);
	return vma_prot;
}

static int mem_map_mmap(struct file *file, struct vm_area_struct *vma)
{
    //dg_printf("in function:%s\n", __FUNCTION__);
/////////////////////////////////////////modified
    size_t size = vma->vm_end - vma->vm_start;
	phys_addr_t offset = (phys_addr_t)vma->vm_pgoff << PAGE_SHIFT;

	/* Does it even fit in phys_addr_t? */
	if (offset >> PAGE_SHIFT != vma->vm_pgoff)
		return -EINVAL;

	/* It's illegal to wrap around the end of the physical address space. */
	if (offset + (phys_addr_t)size - 1 < offset)
		return -EINVAL;


	vma->vm_page_prot = my_phys_mem_access_prot(file, vma->vm_pgoff,
						 size,
						 vma->vm_page_prot);

	/* Remap-pfn-range will mark the range VM_IO */
	if (remap_pfn_range(vma,
			    vma->vm_start,
			    vma->vm_pgoff,
			    size,
			    vma->vm_page_prot)) {
		return -EAGAIN;
	}
	return 0;
/////////////////////////////////////////modified
}

static loff_t mem_map_lseek(struct file *file, loff_t offset, int orig)
{
    //dg_printf("in function:%s\n", __FUNCTION__);
    loff_t ret;

	inode_lock(file_inode(file));
	switch (orig) {
	case SEEK_CUR:
		offset += file->f_pos;
		/* fall through */
	case SEEK_SET:
		/* to avoid userland mistaking f_pos=-9 as -EBADF=-9 */
		if ((unsigned long long)offset >= -MAX_ERRNO) {
			ret = -EOVERFLOW;
			break;
		}
		file->f_pos = offset;
		ret = file->f_pos;
		//force_successful_syscall_return();
		break;
	default:
		ret = -EINVAL;
	}
	inode_unlock(file_inode(file));
	return ret;

    return 0;
}

static const struct file_operations mem_map_fops = {
    .open = mem_map_open,
    .read = mem_map_read,
    .write = mem_map_write,
    .mmap = mem_map_mmap,
    .llseek = mem_map_lseek,
};
typedef struct mem_map_dev_t {
    const char* name;
    struct device *dev;
    const struct file_operations *fops;
    int major;
    int minor;
    struct class *mem_map_dev_class;
}MEM_MAP_DEV_T;
static MEM_MAP_DEV_T mem_map_dev = {
    
    .name = "mem_map",
    .fops = &mem_map_fops,

};

static int chr_dev_init(void)
{
    int minor = 1;//just create a dev only
    int major = 0;//dynamic major id
    struct device *dev = NULL;
    struct class *mem_map_class = NULL;
    
    
    major = register_chrdev(MEM_MAP_MAJOR, mem_map_dev.name, 
        mem_map_dev.fops);//dynamic major id
    dg_printf("mem_map dev major: %d\n", major);
    if (major < 0) 
    {
        nd_printf("unable to get major %d for mem_map devs\n", MEM_MAP_MAJOR);
        return -1;
    }
    
    mem_map_class = class_create(THIS_MODULE, "mem_map");
	if (IS_ERR(mem_map_class))
    {

        nd_printf("create mem_map_class error, 0x%p\n", dev);
		return PTR_ERR(mem_map_class);
    }
    
    dev = device_create(mem_map_class, NULL, MKDEV(major, minor),
			      NULL, mem_map_dev.name);
    if(IS_ERR(dev))
    {
        nd_printf("create dev error, 0x%p\n", dev);
        return -1;
    }
    mem_map_dev.mem_map_dev_class = mem_map_class;
    mem_map_dev.dev = dev;
    mem_map_dev.major = major;
    mem_map_dev.minor = minor;

    return 0;
}
static int __init mem_map_init(void)
{
        printk(KERN_INFO "hello mem_map!\n");
        chr_dev_init();
        return 0;
}

static void release_mem_map_resource(void)
{
    if(mem_map_dev.dev)
    {
        device_destroy(mem_map_dev.mem_map_dev_class, 
            MKDEV(mem_map_dev.major, mem_map_dev.minor));
    }
    if(mem_map_dev.mem_map_dev_class)
    {
        class_destroy(mem_map_dev.mem_map_dev_class);
    }
    if(mem_map_dev.major)
    {
        unregister_chrdev(mem_map_dev.major, mem_map_dev.name);
    }
    return;
}
static void __exit mem_map_exit(void)
{
    release_mem_map_resource();
    printk(KERN_INFO "bye mem_map!\n");
}
 
 
module_init(mem_map_init);
module_exit(mem_map_exit);
 
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xueqing");
