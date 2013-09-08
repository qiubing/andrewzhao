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
	{ 0xd62c833f, "schedule_timeout" },
	{ 0x3eabc96e, "current_task" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x7d11c268, "jiffies" },
	{ 0x50eedeb8, "printk" },
	{ 0xc217b0dd, "pv_cpu_ops" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

