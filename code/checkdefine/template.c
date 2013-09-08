#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>

#define STUDY "Check Define"
#define HEHE 0

MODULE_AUTHOR("ZHAO Yao <soarsky@foxmail.com>");
MODULE_DESCRIPTION(STUDY);
MODULE_LICENSE("Dual BSD/GPL");

int main(void)
{
	printk(KERN_ALERT "I am in main()\n");
#ifdef HEHE
	printk(KERN_ALERT "HEHE is defined\n");
#else
	printk(KERN_ALERT "HEHE is not defined\n");
#endif
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
