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
	{ 0x38052a75, "ieee80211_rts_duration" },
	{ 0xb961b0e9, "pci_bus_read_config_byte" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0x528c709d, "simple_read_from_buffer" },
	{ 0xb7633f86, "pci_release_region" },
	{ 0x2b200864, "_raw_spin_unlock" },
	{ 0x28bc421b, "debugfs_create_dir" },
	{ 0xfebf8737, "mem_map" },
	{ 0xd0d8621b, "strlen" },
	{ 0x387b0e17, "ieee80211_queue_work" },
	{ 0x3eabd939, "dev_set_drvdata" },
	{ 0x97c0e429, "led_classdev_register" },
	{ 0x9c64fbd, "ieee80211_frequency_to_channel" },
	{ 0x7ed61c6, "seq_open" },
	{ 0xb090b909, "dma_set_mask" },
	{ 0xef67c7b5, "pci_match_id" },
	{ 0xd65ae714, "pci_disable_device" },
	{ 0x47939e0d, "__tasklet_hi_schedule" },
	{ 0xad566c84, "ath_key_delete" },
	{ 0x87a45ee9, "_raw_spin_lock_bh" },
	{ 0x7ef39823, "ieee80211_hdrlen" },
	{ 0xcf5ae0f4, "ieee80211_beacon_get_tim" },
	{ 0x7b4b7a18, "seq_printf" },
	{ 0x87968b0f, "ath_regd_init" },
	{ 0xbd441ea9, "ieee80211_unregister_hw" },
	{ 0x7513e94e, "ieee80211_channel_to_frequency" },
	{ 0x4736dc7f, "x86_dma_fallback_dev" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0xb78c61e8, "param_ops_bool" },
	{ 0x9e1bdc28, "init_timer_key" },
	{ 0x6baae653, "cancel_delayed_work_sync" },
	{ 0xb34ec72e, "mutex_unlock" },
	{ 0x1cfc5229, "ieee80211_iterate_active_interfaces_atomic" },
	{ 0xc4ff0a40, "debugfs_create_file" },
	{ 0xd7dd8a35, "seq_read" },
	{ 0x15af7ef0, "sysfs_remove_group" },
	{ 0x7d11c268, "jiffies" },
	{ 0x2e10ca4, "ieee80211_stop_queues" },
	{ 0x6fb7331d, "ieee80211_stop_queue" },
	{ 0x7a7e37c6, "ieee80211_tx_status" },
	{ 0x9dad7635, "pci_set_master" },
	{ 0xe4b5a258, "default_llseek" },
	{ 0xba9938bc, "pci_iounmap" },
	{ 0x524da531, "dev_err" },
	{ 0x88941a06, "_raw_spin_unlock_irqrestore" },
	{ 0x99b6913c, "__mutex_init" },
	{ 0x50eedeb8, "printk" },
	{ 0x471e0c3e, "sysfs_create_group" },
	{ 0x5152e605, "memcmp" },
	{ 0x21f319d7, "ieee80211_wake_queues" },
	{ 0xfaef0ed, "__tasklet_schedule" },
	{ 0xb6ed1e53, "strncpy" },
	{ 0xb4390f9a, "mcount" },
	{ 0x6c2e3320, "strncmp" },
	{ 0x31219afb, "ath_is_world_regd" },
	{ 0x16305289, "warn_slowpath_null" },
	{ 0xcec4b513, "ieee80211_rx" },
	{ 0x316b566e, "skb_push" },
	{ 0x8223e551, "mutex_lock" },
	{ 0x71db534e, "noop_llseek" },
	{ 0x9545af6d, "tasklet_init" },
	{ 0xc5ff6f24, "dma_release_from_coherent" },
	{ 0x2072ee9b, "request_threaded_irq" },
	{ 0x2276db98, "kstrtoint" },
	{ 0x446a8703, "skb_pull" },
	{ 0xcf979da4, "debugfs_create_bool" },
	{ 0x581b5679, "ieee80211_queue_delayed_work" },
	{ 0x39e931d2, "dev_kfree_skb_any" },
	{ 0xba4394cc, "dma_alloc_from_coherent" },
	{ 0xe523ad75, "synchronize_irq" },
	{ 0xec73bd3c, "ath_reg_notifier_apply" },
	{ 0x7b1321f2, "wiphy_to_ieee80211_hw" },
	{ 0x82072614, "tasklet_kill" },
	{ 0xcc40df69, "_dev_info" },
	{ 0x7a27c184, "ewma_init" },
	{ 0x8ff4079b, "pv_irq_ops" },
	{ 0x63dd601, "pci_disable_link_state" },
	{ 0xe62e249e, "ath_hw_get_listen_time" },
	{ 0x7af02ee2, "ath_regd_get_band_ctl" },
	{ 0x7de5839b, "__ieee80211_get_rx_led_name" },
	{ 0x6223cafb, "_raw_spin_unlock_bh" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x60a79f89, "wiphy_rfkill_set_hw_state" },
	{ 0x3bd1b1f6, "msecs_to_jiffies" },
	{ 0x8949c526, "ath_hw_cycle_counters_update" },
	{ 0x6a4d51d6, "ieee80211_get_buffered_bc" },
	{ 0x41b55b04, "__ieee80211_get_tx_led_name" },
	{ 0xdf499ad6, "pci_unregister_driver" },
	{ 0x6443d74d, "_raw_spin_lock" },
	{ 0x319c6026, "ath_is_49ghz_allowed" },
	{ 0x1cb11e54, "ath_hw_setbssidmask" },
	{ 0x587c70d8, "_raw_spin_lock_irqsave" },
	{ 0x17090ed0, "ieee80211_wake_queue" },
	{ 0x4a0e1a91, "ieee80211_generic_frame_duration" },
	{ 0x1c4abfc5, "ieee80211_get_hdrlen_from_skb" },
	{ 0xf6ebc03b, "net_ratelimit" },
	{ 0xddad543d, "ieee80211_ctstoself_duration" },
	{ 0xea285186, "pci_bus_write_config_byte" },
	{ 0xed1bd76f, "ieee80211_register_hw" },
	{ 0xf48411aa, "led_classdev_unregister" },
	{ 0x8d66a3a, "warn_slowpath_fmt" },
	{ 0xf3b7bf5d, "seq_lseek" },
	{ 0x37a0cba, "kfree" },
	{ 0x1207534c, "regulatory_hint" },
	{ 0xb38b3ca8, "ieee80211_alloc_hw" },
	{ 0x930e43fe, "ath_rxbuf_alloc" },
	{ 0xb7787f0d, "__pci_register_driver" },
	{ 0x639fac8e, "ieee80211_free_hw" },
	{ 0x701d0ebd, "snprintf" },
	{ 0x85601526, "seq_release" },
	{ 0x8235805b, "memmove" },
	{ 0x872cfd72, "pci_iomap" },
	{ 0x80e69a0c, "consume_skb" },
	{ 0xf0184b9f, "ath_key_config" },
	{ 0x436c2179, "iowrite32" },
	{ 0xb2aba8a9, "skb_put" },
	{ 0x8171c286, "pci_enable_device" },
	{ 0x362ef408, "_copy_from_user" },
	{ 0xc3fe87c8, "param_ops_uint" },
	{ 0x1432b3c, "ath_hw_keyreset" },
	{ 0x850d1266, "dev_get_drvdata" },
	{ 0xf8229dab, "pci_request_region" },
	{ 0x9e7d6bd0, "__udelay" },
	{ 0xf8b6aa9e, "dma_ops" },
	{ 0xe484e35f, "ioread32" },
	{ 0xf20dabd8, "free_irq" },
	{ 0xe914e41e, "strcpy" },
	{ 0xa14f3d8c, "ewma_add" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=mac80211,cfg80211,ath";

MODULE_ALIAS("pci:v0000168Cd00000207sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000007sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000011sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000012sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000013sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000A727d00000013sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010B7d00000013sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00001014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000015sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000016sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000017sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000018sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd00000019sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd0000001Asv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd0000001Bsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd0000001Csv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v0000168Cd0000001Dsv*sd*bc*sc*i*");
