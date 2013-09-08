#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <linux/in6.h>
#include <linux/random.h>

#define STUDY "mcast_single"

MODULE_AUTHOR("ZHAO Yao <soarsky@foxmail.com>");
MODULE_DESCRIPTION(STUDY);
MODULE_LICENSE("Dual BSD/GPL");

#define PACKET_BITMAP_LEN 4 //unsinged long
#define MSTA_HASH_TABLE_LEN 101
#define MSTA_HASH(a) ((a.s6_addr16[4] ^ a.s6_addr16[5] ^ a.s6_addr16[6] ^ a.s6_addr16[7]) % MSTA_HASH_TABLE_LEN)

struct mcast_station {
	struct in6_addr src_ip6addr;
	rwlock_t sta_rwlock;
	struct mcast_station *prev;
	struct mcast_station *next;
	unsigned long lvt;
	unsigned long flags;
#define MSTA_F_RELEASE 0x00000001

	unsigned long packet_id[PACKET_BITMAP_LEN];
};

struct msta_htable_entry {
	struct mcast_station *first_msta;
	int count;
	rwlock_t hte_rwlock;
};

static struct kmem_cache *mcast_station_cache __read_mostly;
static struct msta_htable_entry msta_hash_table[MSTA_HASH_TABLE_LEN] __read_mostly;

void gen_random(struct in6_addr *addr)
{
	get_random_bytes(addr->s6_addr, sizeof(struct in6_addr));
}

void *new_msta(void)
{
	return kmem_cache_alloc(mcast_station_cache, GFP_KERNEL);
}

int add_msta(struct mcast_station *msta)
{
	return 0;
}

int del_msta(struct mcast_station *msta)
{


	kmem_cache_free(mcast_station_cache, msta);
	return 0;
}

int main(void)
{
	struct mcast_station *p;
	struct msta_htable_entry *q;
	struct in6_addr addr;
	int i, max=0, j=0;

	mcast_station_cache = kmem_cache_create("mcast_station_cache",
			sizeof(struct mcast_station), 0, 0, NULL);
	memset(msta_hash_table, 0, sizeof(msta_hash_table));

	for(i=0; i<2020; i++) {
		gen_random(&addr);
		q = &msta_hash_table[MSTA_HASH(addr)];
		q->count++;
	}

	for(i=0; i<MSTA_HASH_TABLE_LEN; i++) {
		printk("index[%d]: count=%d\n", i, msta_hash_table[i].count);
		if (max < msta_hash_table[i].count) {
			max = msta_hash_table[i].count;
			j = i;
		}
	}
	printk("Max=%d, index=%d\n", max, j);

	kmem_cache_destroy(mcast_station_cache);

	return 0;
}

static int template_init(void)
{
	printk(KERN_ALERT "%s study modules\n", STUDY);
	main();
	return 0;
}

static void template_exit(void)
{
	printk(KERN_ALERT "%s study exit\n", STUDY);
}

module_init(template_init);
module_exit(template_exit);
