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
	{ 0x79dcbcbd, "lib80211_unregister_crypto_ops" },
	{ 0x5224a65b, "lib80211_register_crypto_ops" },
	{ 0xb2aba8a9, "skb_put" },
	{ 0x316b566e, "skb_push" },
	{ 0xe7b6456d, "crypto_alloc_base" },
	{ 0xb85d9f9b, "kmem_cache_alloc_trace" },
	{ 0xa438e74c, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x5a28c35c, "crypto_destroy_tfm" },
	{ 0x50eedeb8, "printk" },
	{ 0xf6ebc03b, "net_ratelimit" },
	{ 0xa99a6c04, "skb_trim" },
	{ 0x446a8703, "skb_pull" },
	{ 0x8235805b, "memmove" },
	{ 0x5152e605, "memcmp" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x3c2c5af5, "sprintf" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=lib80211";

