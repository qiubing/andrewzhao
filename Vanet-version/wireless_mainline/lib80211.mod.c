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
	{ 0xccee108d, "module_layout" },
	{ 0xa438e74c, "kmalloc_caches" },
	{ 0x9e1bdc28, "init_timer_key" },
	{ 0x7d11c268, "jiffies" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xe1bc7ede, "del_timer_sync" },
	{ 0x11089ac7, "_ctype" },
	{ 0x88941a06, "_raw_spin_unlock_irqrestore" },
	{ 0x50eedeb8, "printk" },
	{ 0xb4390f9a, "mcount" },
	{ 0x71205378, "add_timer" },
	{ 0x2462159a, "module_put" },
	{ 0xb85d9f9b, "kmem_cache_alloc_trace" },
	{ 0x587c70d8, "_raw_spin_lock_irqsave" },
	{ 0x37a0cba, "kfree" },
	{ 0x701d0ebd, "snprintf" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

