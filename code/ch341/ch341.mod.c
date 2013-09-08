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
	{ 0x86942e2b, "usb_serial_suspend" },
	{ 0xb626b1d6, "usb_serial_disconnect" },
	{ 0xa3a34db7, "usb_serial_probe" },
	{ 0xb78c61e8, "param_ops_bool" },
	{ 0x2e5e70e1, "usb_deregister" },
	{ 0xb12514f7, "usb_serial_deregister" },
	{ 0xae2226d5, "usb_register_driver" },
	{ 0x12793fe4, "usb_serial_register" },
	{ 0xef7ea6eb, "dev_set_drvdata" },
	{ 0x48eb0c0d, "__init_waitqueue_head" },
	{ 0xb5e7a846, "usb_serial_generic_open" },
	{ 0xae228111, "usb_serial_resume" },
	{ 0x37a0cba, "kfree" },
	{ 0x258479b7, "kmem_cache_alloc_trace" },
	{ 0x7177f6f6, "kmalloc_caches" },
	{ 0x62cb4cee, "interruptible_sleep_on" },
	{ 0xa15a6cab, "current_task" },
	{ 0x7cc17639, "tty_get_baud_rate" },
	{ 0xc363bd82, "usb_kill_urb" },
	{ 0xf45c30d1, "usb_serial_generic_close" },
	{ 0x1d49a044, "usb_control_msg" },
	{ 0xbaf410e7, "dev_printk" },
	{ 0x3010e5fc, "tty_kref_put" },
	{ 0xe5f6f7f1, "usb_serial_handle_dcd_change" },
	{ 0x8a1a2cca, "tty_port_tty_get" },
	{ 0xe45f60d8, "__wake_up" },
	{ 0x887f3790, "dev_err" },
	{ 0xb4102ac3, "usb_submit_urb" },
	{ 0x50eedeb8, "printk" },
	{ 0xf97456ea, "_raw_spin_unlock_irqrestore" },
	{ 0x21fb443e, "_raw_spin_lock_irqsave" },
	{ 0x1f72dc4, "dev_get_drvdata" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=usbserial,usbcore";

MODULE_ALIAS("usb:v4348p5523d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1A86p7523d*dc*dsc*dp*ic*isc*ip*");
MODULE_ALIAS("usb:v1A86p5523d*dc*dsc*dp*ic*isc*ip*");
