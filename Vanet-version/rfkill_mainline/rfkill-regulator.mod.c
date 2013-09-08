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
	{ 0xc886c5bc, "platform_driver_unregister" },
	{ 0x1fdf2bd9, "platform_driver_register" },
	{ 0xcc40df69, "_dev_info" },
	{ 0x3eabd939, "dev_set_drvdata" },
	{ 0x3047aa7e, "rfkill_register" },
	{ 0x11493a7, "rfkill_alloc" },
	{ 0xb85d9f9b, "kmem_cache_alloc_trace" },
	{ 0xa438e74c, "kmalloc_caches" },
	{ 0x6f85c033, "regulator_get_exclusive" },
	{ 0x524da531, "dev_err" },
	{ 0x37a0cba, "kfree" },
	{ 0x6c5e5da, "regulator_put" },
	{ 0x65382f09, "rfkill_destroy" },
	{ 0x5d73920f, "rfkill_unregister" },
	{ 0x850d1266, "dev_get_drvdata" },
	{ 0x5e241a45, "regulator_disable" },
	{ 0x187554d4, "regulator_enable" },
	{ 0x4ef8a7e1, "regulator_is_enabled" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=rfkill";

