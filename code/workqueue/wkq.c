#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/workqueue.h>
#include <linux/sched.h>

MODULE_AUTHOR("ZHAO YAO");
MODULE_DESCRIPTION("workqueue study");
MODULE_LICENSE("Dual BSD/GPL");

int main(void)
{
	struct workqueue_struct *wkq;
	wkq = create_workqueue("wkthread");
	schedule();
	return 0;
}

static int m_init(void)
{
	printk("Workqueue study modules\n");
	main();
	return 0;
}

static void m_exit(void)
{
	printk("Workqueue study exit\n");
}

module_init(m_init);
module_exit(m_exit);
