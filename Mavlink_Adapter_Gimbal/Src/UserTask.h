
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
	void app_run(void);   /* ���������ʼ�� */	
	static void init_system(void);
	
	/* ������ */
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
//		unsigned char fly_count;       /* ���д��� */
//		unsigned char pre_fly_count;   /* �ϴη��д���*/
//		unsigned char init_loader: 1; /* ����������ؽ��ȣ����û����ȷ���ӣ���һֱ���ڼ��ؽ��� 1-loader 0-jump */
//		unsigned char armed: 1;          /* 1-armed 0-disarm */
//		unsigned char beep_enable:1; 		/* ������ʹ��   0-�ر�    1-���� */
//		unsigned char rgb1_enable:1;
//		unsigned char rgb2_enable:1;
//		unsigned char	oled_enable:1;
//		unsigned char mavlink_exist:1;  /* mavlink���ݣ��������� */
//		unsigned char mavlink_valid:1;  /* mavlink�е�״̬������Ч */
//		unsigned char drone_complete:1; /* 1-������� */
//		unsigned char drone_send_compass_cal; 
//		unsigned char test_mask_bit;        /*  */
//		unsigned char oled_page; 			/* OLED��ʾҳ 0-����ҳ��   1-������Ϣҳ�� */
//		unsigned char compass_select; /* ������ѡ�� */
//		
//	}SystemFlag;
	

#endif
