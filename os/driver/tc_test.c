#include<linux/kernel.h>
#include<linux/delay.h>
#include<linux/kthread.h>


#define DELAY1  (1000)
#define DELAY2  (872)
#define DELAY3  (1093)


static volatile int  v_s_wr = 0;

static  DEFINE_SPINLOCK(s_wr_lock);

static  struct task_struct *ts_task1,*ts_task2,*ts_task3;


static int ts_thread1(void *data)
{
	while(1)
	{
		printk("%s,add 1\n", __func__);

		spin_lock(&s_wr_lock);

		v_s_wr++;

		spin_unlock(&s_wr_lock);

		msleep(DELAY1);

	}

	return 0;
}


static int ts_thread2(void *data)
{
	while(1)
	{
		spin_lock(&s_wr_lock);

		printk("%s: print ---%d\n",__func__,v_s_wr);


		spin_unlock(&s_wr_lock);

		msleep(DELAY2);

	}

	return 0;
}


static int ts_thread3(void *data)
{
	while(1)
	{
		printk("%s: dec 1\n", __func__);

		spin_lock(&s_wr_lock);

		v_s_wr--;

		spin_unlock(&s_wr_lock);

		msleep(DELAY3);
	}

	return 0;
}

void ts_test_stop(void);

int ts_test_start(void)
{
	ts_task1 = kthread_create(ts_thread1,NULL,"ts_thread1");
	ts_task2 = kthread_create(ts_thread2, NULL, "ts_thread2");
	ts_task3 = kthread_create(ts_thread3, NULL, "ts_thread3");

	if(IS_ERR(ts_task1) || IS_ERR(ts_task2) || IS_ERR(ts_task3))
	{
		printk("unable to start kernel thread.\n");
		ts_test_stop();
		return -1;
	}


	wake_up_process(ts_task1);
	wake_up_process(ts_task2);
	wake_up_process(ts_task3);

	return 0;
}
void ts_test_stop(void)
{
	if(ts_task1) 
	{
		kthread_stop(ts_task1);
		ts_task1 = NULL;
	}

	if(ts_task2)
	{
		kthread_stop(ts_task2);
		ts_task2 = NULL;
	}
	
	if(ts_task3)
	{
		kthread_stop(ts_task3);
		ts_task3 = NULL;
	}

}
