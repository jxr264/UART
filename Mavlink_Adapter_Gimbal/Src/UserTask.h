
#ifndef USER_TASK_H
#define USER_TASK_H
	#include "HardwareConfig.h"
	#include "stm32f1xx_hal.h"
	#include "cmsis_os.h"
	#include "oled.h"
  #include "oled_display.h"
	
	static xSemaphoreHandle mutex_usart1_tx;
	static xSemaphoreHandle mutex_usart2_tx;
	static xSemaphoreHandle mutex_usart3_tx;
  static xQueueHandle button_event_queue;
  static xSemaphoreHandle button_event_take;
	
	//static SystemFlag system_flag ;
  //static FailStatus fail_status;
	void app_run(void);   /* 所有任务初始化 */	
	static void init_system(void);
	
	/* 任务函数 */
	static void initUsartIT(UART_HandleTypeDef *huart);
	static void usart1_send_task(void const* arg);
	static void usart1_receive_task(void const* arg);
  static void usart2_send_task(void const* arg);
	static void usart2_receive_task(void const* arg);
	static void usart3_receive_task(void const* arg);
	static void usart3_send_task(void const* arg);
	static void update_oled_task(void const* arg);
	static void button_event_task(void const* arg);
	
	
	static void task_50ms(void const* arg);
	
	static void restart_usart(UART_HandleTypeDef *huart);
	
	static void run_led_fun(void);
	static void feed_dog_fun(void);
	static void beep_fun(void);
	static void func_key(void);
	//static void update_oled_task(void const* argument);
//	static void button_event_task(void const* argument);
	
	extern IWDG_HandleTypeDef hiwdg;
	extern UART_HandleTypeDef huart1;
	extern UART_HandleTypeDef huart2;
	extern UART_HandleTypeDef huart3;
	extern TIM_HandleTypeDef htim2;
	
//	
//		typedef struct sys_flag
//	{
//		unsigned int oled_page_bit_mask;
//		unsigned char fly_count;       /* 飞行次数 */
//		unsigned char pre_fly_count;   /* 上次飞行次数*/
//		unsigned char init_loader: 1; /* 启动画面加载进度，如果没有正确连接，则一直处于加载界面 1-loader 0-jump */
//		unsigned char armed: 1;          /* 1-armed 0-disarm */
//		unsigned char beep_enable:1; 		/* 蜂鸣器使能   0-关闭    1-开启 */
//		unsigned char rgb1_enable:1;
//		unsigned char rgb2_enable:1;
//		unsigned char	oled_enable:1;
//		unsigned char mavlink_exist:1;  /* mavlink数据，常亮蓝灯 */
//		unsigned char mavlink_valid:1;  /* mavlink中的状态数据有效 */
//		unsigned char drone_complete:1; /* 1-飞行完成 */
//		unsigned char drone_send_compass_cal; 
//		unsigned char test_mask_bit;        /*  */
//		unsigned char oled_page; 			/* OLED显示页 0-加载页面   1-基本信息页面 */
//		unsigned char compass_select; /* 磁罗盘选择 */
//		
//	}SystemFlag;
	

#endif
