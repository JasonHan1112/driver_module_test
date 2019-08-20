#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xe15704bf, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x6bc3fbc0, __VMLINUX_SYMBOL_STR(__unregister_chrdev) },
	{ 0x8133c1b9, __VMLINUX_SYMBOL_STR(class_destroy) },
	{ 0x42deffa7, __VMLINUX_SYMBOL_STR(device_destroy) },
	{ 0x2c49175, __VMLINUX_SYMBOL_STR(device_create) },
	{ 0xa9613291, __VMLINUX_SYMBOL_STR(__class_create) },
	{ 0xf121a92b, __VMLINUX_SYMBOL_STR(__register_chrdev) },
	{ 0x4c9d28b0, __VMLINUX_SYMBOL_STR(phys_base) },
	{ 0x5944d015, __VMLINUX_SYMBOL_STR(__cachemode2pte_tbl) },
	{ 0xb9dcf25d, __VMLINUX_SYMBOL_STR(boot_cpu_data) },
	{ 0xd4091f0f, __VMLINUX_SYMBOL_STR(remap_pfn_range) },
	{ 0x7cd8d75e, __VMLINUX_SYMBOL_STR(page_offset_base) },
	{ 0x8a7d1c31, __VMLINUX_SYMBOL_STR(high_memory) },
	{ 0xe4aa8c83, __VMLINUX_SYMBOL_STR(up_write) },
	{ 0x16ca495e, __VMLINUX_SYMBOL_STR(down_write) },
	{ 0x37a0cba, __VMLINUX_SYMBOL_STR(kfree) },
	{ 0xc671e369, __VMLINUX_SYMBOL_STR(_copy_to_user) },
	{ 0x2e1da9fb, __VMLINUX_SYMBOL_STR(probe_kernel_read) },
	{ 0x1430a3de, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xafd29968, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x310f02ec, __VMLINUX_SYMBOL_STR(memremap) },
	{ 0x9e9fdd9d, __VMLINUX_SYMBOL_STR(memunmap) },
	{ 0xb5419b40, __VMLINUX_SYMBOL_STR(_copy_from_user) },
	{ 0x88db9f48, __VMLINUX_SYMBOL_STR(__check_object_size) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

