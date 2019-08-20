#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "libmem.h"

#include <unistd.h>
#include <errno.h>
#include <string.h>

static int get_fd(char *file_name)
{
    int fd = 0;
    fd = open(file_name, O_RDWR | O_SYNC);
    if(fd == -1)
    {
        nd_print("open %s failed, error code: %d, \n",
            file_name, errno, strerror(errno));
        return -1;
    }
    else
    {
        return fd;
    }
}

static int get_mem_fd(void)
{
    return get_fd(DEV_MEM);
}
uint16_t read_dram_map_raw(uint64_t dram_addr, uint32_t *value, uint32_t size)
{
    int ret = 0;
    uint8_t *tmp = NULL;
    int fd = get_mem_fd();
    long page_size = sysconf(_SC_PAGESIZE);
    uint64_t dram_addr_page_aligned = (dram_addr / page_size)* page_size;
    uint64_t in_page_ofst = dram_addr % page_size;

    dg_print("page_size = %d, dram_addr_page_aligned = 0x%x, in_page_ofst = 0x%x\n", 
        page_size, dram_addr_page_aligned, in_page_ofst);

    tmp = mmap(NULL, (size_t)page_size, PROT_READ, MAP_SHARED, fd, dram_addr_page_aligned);
    if(tmp == NULL)
    {
        nd_print("mmap file error\n");
        return 1;
    }

    if(size == 4)
        *value = *(uint32_t*)(tmp + in_page_ofst);
    else if(size == 2)
        *value = *(uint16_t*)(tmp + in_page_ofst);
    else if(size == 1)
        *value = *(uint8_t*)(tmp + in_page_ofst);
    else
    {
        nd_print("read size error\n");
        return 1;
    }
    munmap(tmp, (size_t)page_size);    

    return 0;
}

uint16_t read_dram_raw(uint64_t dram_addr, uint32_t *value, uint32_t size)
{
    int ret = 0;
    int fd = get_mem_fd();
    uint32_t tmp = 0;
    lseek(fd, dram_addr, SEEK_SET);
    
    ret = read(fd, &tmp, (size_t)size);
    if(size == 4)
        *value = *(uint32_t*)(&tmp);
    else if(size == 2)
        *value = *(uint16_t*)(&tmp);
    else if(size == 1)
        *value = *(uint8_t*)(&tmp);
    else
    {
        nd_print("read size error\n");
        return 1;
    }


    if(ret != size)
    {
        nd_print("read file error\n");
        return 1;
    }
    close(fd);

    return 0;
}


uint16_t write_dram_map_raw(uint64_t dram_addr, uint32_t value, uint32_t size)
{
    int ret = 0;
    uint8_t *tmp = NULL;
    int fd = get_mem_fd();
    long page_size = sysconf(_SC_PAGESIZE);
    uint64_t dram_addr_page_aligned = (dram_addr / page_size)* page_size;
    uint64_t in_page_ofst = dram_addr % page_size;

    /*dg_print("page_size = %d, dram_addr_page_aligned = 0x%x, in_page_ofst = 0x%x\n", 
        page_size, dram_addr_page_aligned, in_page_ofst);*/

    tmp = mmap(NULL, (size_t)page_size, PROT_READ, MAP_SHARED, fd, dram_addr_page_aligned);
    if(tmp == NULL)
    {
        nd_print("mmap file error\n");
        return 1;
    }

    if(size == 4)
        *(uint32_t*)(tmp + in_page_ofst) = (uint32_t)value;
    else if(size == 2)
        *(uint16_t*)(tmp + in_page_ofst) = (uint16_t)value;
    else if(size == 1)
        *(uint8_t*)(tmp + in_page_ofst) = (uint8_t)value;
    else
    {
        nd_print("write size error\n");
        return 1;
    }
    munmap(tmp, (size_t)page_size);    
 
    return 0;
}
uint16_t write_dram_raw(uint64_t dram_addr, uint32_t value, uint32_t size)
{
    int ret = 0;
    int fd = get_mem_fd();
    uint32_t tmp = 0;
    lseek(fd, dram_addr, SEEK_SET);
    
    if(size == 4)
        *(uint32_t*)(&tmp) = (uint32_t)value;
    else if(size == 2)
        *(uint16_t*)(&tmp) = (uint16_t)value;
    else if(size == 1)
        *(uint8_t*)(&tmp) = (uint8_t)value;
    else
    {
        nd_print("read size error\n");
        return 1;
    }


    ret = write(fd, &tmp, (size_t)size);
    

    if(ret != size)
    {
        nd_print("read file error\n");
        return 1;
    }
    close(fd);

    return 0;
}
int main(void)
{
#if 1//test open read
    int ret = 0;
    uint32_t val = 0;
    size_t size = 4;
    uint64_t dram_addr = 0xf000d018;
    read_dram_raw(dram_addr, &val, size);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);
    size = 2;
    read_dram_raw(dram_addr, &val, size);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);
    size = 1;
    read_dram_raw(dram_addr, &val, size);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);

#endif
#if 0//test mmap
    int ret = 0;
    uint32_t val = 0;
    size_t size = 4;
    read_dram_map_raw(0xf000d018, &val, size);
    dg_print("map val = 0x%x\n", val);
    size = 2;
    read_dram_map_raw(0xf000d018, &val, size);
    dg_print("map val = 0x%x\n", val);
    size = 1;
    read_dram_map_raw(0xf000d018, &val, size);
    dg_print("map val = 0x%x\n", val);
    //munmap(val, size);


#endif
#if 0//test open write

    

    int ret = 0;
    int fd = get_mem_fd();
    uint32_t val = 0;
    size_t size = 4;
    uint64_t dram_addr = 0xf000d018;
    read_dram_raw(dram_addr, &val, 4);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);
    dg_print("==============================\n");

    write_dram_raw(dram_addr, 0xffffffff, 4);
    read_dram_raw(dram_addr, &val, 4);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);
    dg_print("==============================\n");

    size = 1;
    val = 0xffffff00;
    write_dram_raw(dram_addr, val, size);
    dg_print("write ret = %d, val = 0x%x\n", ret, val);
    read_dram_raw(dram_addr, &val, 4);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);
    dg_print("==============================\n");
    
    write_dram_raw(dram_addr, 0xffffffff, 4);
    read_dram_raw(dram_addr, &val, 4);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);
    dg_print("==============================\n");

    size = 2;
    val = 0xffff0000;
    write_dram_raw(dram_addr, val, size);
    dg_print("write ret = %d, val = 0x%x\n", ret, val);
    read_dram_raw(dram_addr, &val, 4);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);
    dg_print("==============================\n");

    write_dram_raw(dram_addr, 0xffffffff, 4);
    read_dram_raw(dram_addr, &val, 4);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);
    dg_print("==============================\n");

    size = 4;
    val = 0x0;
    write_dram_raw(dram_addr, val, size);
    dg_print("write ret = %d, val = 0x%x\n", ret, val);
    read_dram_raw(dram_addr, &val, 4);
    dg_print("read ret = %d, val = 0x%x\n", ret, val);
    dg_print("==============================\n");

#endif


    return 0;
}

