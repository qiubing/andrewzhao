#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timex.h>
#include <linux/delay.h>

#define STUDY "jiffies and HZ"

MODULE_AUTHOR("ZHAO Yao <soarsky@foxmail.com>");
MODULE_DESCRIPTION(STUDY);
MODULE_LICENSE("Dual BSD/GPL");

int main(void)
{
	cycles_t time;
	unsigned int low, high;
	time = get_cycles();
	low = (unsigned int) time;
	high = (unsigned int)(time >> 32);
	printk(KERN_ALERT "HZ = %d\n", HZ);
	printk(KERN_ALERT "low = %u, high = %u\n", low, high);
	
	printk(KERN_ALERT "jiffies=%d, after 100ms, ", jiffies);
	mdelay(100);
	printk(KERN_ALERT "jiffies=%d\n", jiffies);

	printk(KERN_ALERT "I wanna sleep\n");
	set_current_state(TASK_UNINTERRUPTIBLE);
	schedule_timeout(HZ * 10);
	printk(KERN_ALERT "I am alive again...\n");

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
