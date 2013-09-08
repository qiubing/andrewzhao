#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x5cf9d9c5, "module_layout" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x1675bc39, "kmem_cache_destroy" },
	{ 0x50eedeb8, "printk" },
	{ 0xfc2b8e7f, "kmem_cache_create" },
	{ 0xb433bf86, "kmem_cache_free" },
	{ 0xe2fcb866, "kmem_cache_alloc" },
	{ 0x79aa04a2, "get_random_bytes" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

