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
	{ 0x8cd3e66b, "mutex_destroy" },
	{ 0x9e6f57b4, "led_trigger_event" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0xcff870a6, "no_llseek" },
	{ 0xa0e8ee74, "kobject_uevent" },
	{ 0xe9627f41, "input_unregister_handle" },
	{ 0xe72bdbc2, "led_trigger_register" },
	{ 0x8949858b, "schedule_work" },
	{ 0x4205ad24, "cancel_work_sync" },
	{ 0x9e1bdc28, "init_timer_key" },
	{ 0x6baae653, "cancel_delayed_work_sync" },
	{ 0xb34ec72e, "mutex_unlock" },
	{ 0xd72df9cf, "input_register_handler" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0x9013c46c, "nonseekable_open" },
	{ 0x7d11c268, "jiffies" },
	{ 0xe174aa7, "__init_waitqueue_head" },
	{ 0x52b3c4a4, "misc_register" },
	{ 0xe1bc7ede, "del_timer_sync" },
	{ 0x294bd30, "input_unregister_handler" },
	{ 0x322412a4, "device_del" },
	{ 0x88941a06, "_raw_spin_unlock_irqrestore" },
	{ 0xf1b5393e, "input_close_device" },
	{ 0x3eabc96e, "current_task" },
	{ 0x99b6913c, "__mutex_init" },
	{ 0x50eedeb8, "printk" },
	{ 0x6df6a009, "led_trigger_unregister" },
	{ 0xad950f90, "class_unregister" },
	{ 0x2f287f0d, "copy_to_user" },
	{ 0xb4390f9a, "mcount" },
	{ 0x6f5427, "_raw_spin_unlock_irq" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0x8223e551, "mutex_lock" },
	{ 0xa497b62, "input_open_device" },
	{ 0x8ecb6e1d, "device_add" },
	{ 0xc37ff33d, "__class_register" },
	{ 0x7ce94405, "boot_tvec_bases" },
	{ 0xc6cbbc89, "capable" },
	{ 0x99cf2da7, "put_device" },
	{ 0x77edf722, "schedule_delayed_work" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x4292364c, "schedule" },
	{ 0xf333a2fb, "_raw_spin_lock_irq" },
	{ 0x54a9db5f, "_kstrtoul" },
	{ 0xb85d9f9b, "kmem_cache_alloc_trace" },
	{ 0x587c70d8, "_raw_spin_lock_irqsave" },
	{ 0x3a2381a8, "input_register_handle" },
	{ 0xf09c7f68, "__wake_up" },
	{ 0x8d66a3a, "warn_slowpath_fmt" },
	{ 0x1eb9516e, "round_jiffies_relative" },
	{ 0x37a0cba, "kfree" },
	{ 0xe75663a, "prepare_to_wait" },
	{ 0x4b99f99c, "device_initialize" },
	{ 0xb00ccc33, "finish_wait" },
	{ 0x1ad94b34, "dev_set_name" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xc3fe87c8, "param_ops_uint" },
	{ 0x655b7c0c, "misc_deregister" },
	{ 0xb9eb3aa9, "add_uevent_var" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

