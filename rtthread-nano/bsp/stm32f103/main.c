#include "rtthread.h"
#include "led.h"

static rt_thread_t led_thrd = RT_NULL;

static void led_thread_entry(void *param) {
    Led_Blink();
}

int main() {
     led_thrd =                          /* 线程控制块指针 */
    rt_thread_create( "led1",              /* 线程名字 */
                    led_thread_entry,   /* 线程入口函数 */
                    RT_NULL,             /* 线程入口函数参数 */
                    512,                 /* 线程栈大小 */
                    3,                   /* 线程的优先级 */
                    20);                 /* 线程时间片 */

    /* 启动线程，开启调度 */
    if (led_thrd != RT_NULL)
        rt_thread_startup(led_thrd);
    else
        return -1;
}