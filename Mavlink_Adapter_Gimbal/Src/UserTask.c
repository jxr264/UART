
#include "UserTask.h"
#define DEBUG 1  //调试代码 ，1编译  0不编译
unsigned char status_bled=0;//灯标志位
unsigned char button_enable=0;//灯标志位

 void app_run(void)
{
	init_system();                        //-3--+3
	/* 喂狗任务 */
	osThreadDef(Task50MSThread, task_50ms, osPriorityLow, 0, 128);   //priority: idle (lowest)    -3
	osThreadCreate(osThread(Task50MSThread), NULL);	
	/* 串口1接收数据处理任务 */
	osThreadDef(UART1RXTask, usart1_receive_task, osPriorityRealtime, 0, 512);    //priority: realtime (highest)   +3
	osThreadCreate(osThread(UART1RXTask), NULL);
	
	/* 串口2接收数据处理任务 */
	osThreadDef(UART2RXTask, usart2_receive_task, osPriorityLow, 0, 256);      //priority: low    -2
	osThreadCreate(osThread(UART2RXTask), NULL);
	
	/* 串口3接收数据处理任务 */
//	osThreadDef(UART3RXTask, usart3_receive_task, osPriorityHigh, 0, 512);   //priority: high   +2
	//osThreadCreate(osThread(UART3RXTask), NULL);  
	
	/* 串口1发送数据处理任务 *//* 与通信模组通信任务 */	
	osThreadDef(UART1TXTask, usart1_send_task, osPriorityBelowNormal, 0, 256);   //priority: below normal   -1
	osThreadCreate(osThread(UART1TXTask), NULL);
	
	/* 串口2发送数据处理任务 */
	osThreadDef(UART2TXTask, usart2_send_task, osPriorityAboveNormal, 0, 256);   // priority: above normal  +1
	osThreadCreate(osThread(UART2TXTask), NULL);
	
	/* 串口3发送数据处理任务 */
//	osThreadDef(UART3TXTask, usart3_send_task, osPriorityLow, 0, 256);     //priority: low  -2
 //	osThreadCreate(osThread(UART3TXTask), NULL);	
	/*OLED显示*/
  osThreadDef(OledTask, update_oled_task, osPriorityIdle, 0, 128);    //priority: idle (lowest)    -3
	osThreadCreate(osThread(OledTask), NULL);
	/* 按键队列处理任务 */
	osThreadDef(ButtonTask, button_event_task, osPriorityIdle, 0, 256);    //priority: below normal   +1
	osThreadCreate(osThread(ButtonTask), NULL);	
}

/* function code end */

/* 串口1接收处理任务 */
static void usart1_receive_task(void const* arg)
{
	unsigned int data_len = 0;
	char buff[128];
	unsigned int i = 0;
	unsigned char count = 0;
	while(1)
	{
		osDelay(20);
		restart_usart(&huart1);
		data_len = readBuffLen(USART1_ID); /* 读取串口1缓冲队列中的数据长度 */
		if(data_len>0)
		{
			if(data_len>128) data_len = 128;			
			read(USART1_ID, &buff[0], data_len);
			#if DEBUG//测试代码
				xSemaphoreTake( mutex_usart1_tx, portMAX_DELAY );
				write(USART1_ID, &buff[0], data_len);
				xSemaphoreGive(mutex_usart1_tx);
			#endif
			
			/* 解析数据 *///假如需要对接收到的数据进行判断，只需将判断代码假如解析的数据中
			for(i=0; i<data_len; ++i)
			{					
				if(buff[i]=='2')
			{
		   status_bled = 1;   //判断串口1输入2时点亮蓝灯
			}
			count = 0;
			}
		}
		else if(0==data_len)
		{	
			count++;    			
			if(count>=250)
			{
				count = 250;
			}		
		}
	}
}
/* function code end */

/* 串口1发送任务 */
static void usart1_send_task(void const* arg)
{
	uint16_t data_len = 0;
	LoopQueue* sendQueue;
	char send_buff[128];
	
	while(1)
	{
		osDelay(50);	
		
		xSemaphoreTake( mutex_usart1_tx, portMAX_DELAY );		
		sendQueue = getUsartSendLoopQueue(USART1_ID); /* get send queue */
		if(sendQueue!=NULL)
		{		
			data_len = writeBuffLen(USART1_ID); /* send queue data count */
			if(data_len>0)
			{
				unsigned int i = 0;
				if(data_len>128) data_len=128;
				for( i=0; i<data_len; ++i)
				{
					send_buff[i] = readCharLoopQueue(sendQueue);
				}
				HAL_UART_Transmit_DMA(&huart1, (uint8_t *)send_buff, (uint16_t)data_len); /* DMA send	*/
			}
		}		
		xSemaphoreGive(mutex_usart1_tx);
	}
}
/* function code end */

/* function code end */

/* 串口2接收处理任务 */
static void usart2_receive_task(void const* arg)
{
	unsigned int data_len = 0;
	char buff[128];
	unsigned int i = 0;
	unsigned char count = 0;
	
	while(1)
	{
		osDelay(50);
		restart_usart(&huart2);
		data_len = readBuffLen(USART2_ID); /* 读取串口2缓冲队列中的数据长度 */
		if(data_len>0)
		{
			if(data_len>128) data_len = 128;			
			read(USART2_ID, &buff[0], data_len);
			
			#if DEBUG
				xSemaphoreTake( mutex_usart2_tx, portMAX_DELAY );   //假如需要将串口2的数据在串口1中输出，只需将对应的编号改为1即可
				write(USART2_ID, &buff[0], data_len);
				xSemaphoreGive(mutex_usart2_tx);
			#endif
			
			/* 解析数据 */
			for(i=0; i<data_len; ++i)
			{                             
				count = 0;
			}
		}else if(0==data_len)    //假如需要对接收到的数据进行判断，只需将判断代码假如解析的数据中
		{
			count++;
			if(count>=250)
			{
				count = 250;
			}
		}
	}
}
/* function code end */

/* 串口2发送任务 */
static void usart2_send_task(void const* arg)
{
	uint16_t data_len = 0;
	LoopQueue* sendQueue;
	char send_buff[128];
	
	while(1)
	{
		osDelay(50);	
		
		xSemaphoreTake( mutex_usart2_tx, portMAX_DELAY );		
		sendQueue = getUsartSendLoopQueue(USART2_ID); /* get send queue */
		if(sendQueue!=NULL)
		{		
			data_len = writeBuffLen(USART2_ID); /* send queue data count */
			if(data_len>0)
			{
				unsigned int i = 0;
				if(data_len>128) data_len=128;
				for( i=0; i<data_len; ++i)
				{
					send_buff[i] = readCharLoopQueue(sendQueue);
				}
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)send_buff, (uint16_t)data_len); /* DMA send	*/
			}
		}		
		xSemaphoreGive(mutex_usart2_tx);
	}
}


/* 串口3接收处理任务 */
static void usart3_receive_task(void const* arg)
{
	unsigned int data_len = 0;
	char buff[128];
	unsigned int i = 0;
	unsigned char count = 0;
	
	while(1)
	{
		osDelay(20);
		restart_usart(&huart3);
		data_len = readBuffLen(USART3_ID); /* 读取串口1缓冲队列中的数据长度 */
		if(data_len>0)
		{
			if(data_len>128) data_len = 128;			
			read(USART3_ID, &buff[0], data_len);
			
			#if DEBUG
				xSemaphoreTake( mutex_usart3_tx, portMAX_DELAY );
				write(USART3_ID, &buff[0], data_len);
				xSemaphoreGive(mutex_usart3_tx);
			#endif
			
			/* 解析数据 */
			for(i=0; i<data_len; ++i)
			{
				count = 0;
			}
		}else if(0==data_len)
		{
			count++;
			if(count>=250)
			{
				count = 250;
			}
		}
	}
}
/* function code end */

/* 串口3发送任务 */
static void usart3_send_task(void const* arg)
{
	uint16_t data_len = 0;
	LoopQueue* sendQueue;
	char send_buff[128];
	
	while(1)
	{
		osDelay(50);	
		
		xSemaphoreTake( mutex_usart3_tx, portMAX_DELAY );		
		sendQueue = getUsartSendLoopQueue(USART3_ID); /* get send queue */
		if(sendQueue!=NULL)
		{		
			data_len = writeBuffLen(USART3_ID); /* send queue data count */
			if(data_len>0)
			{
				unsigned int i = 0;
				if(data_len>128) data_len=128;
				for( i=0; i<data_len; ++i)
				{
					send_buff[i] = readCharLoopQueue(sendQueue);
				}
				HAL_UART_Transmit_DMA(&huart3, (uint8_t *)send_buff, (uint16_t)data_len); /* DMA send	*/
			}
		}		
		xSemaphoreGive(mutex_usart3_tx);
	}
}
/* function code end */



/* 每50ms调用一次任务： LED、BEEP和喂狗操作 */
static void task_50ms(void const* arg)
{
	while(1)
	{
		osDelay(50);
		run_led_fun();
		beep_fun();
	  func_key();
		feed_dog_fun();
		
	}
}
/* end the function */

/* 初始化系统函数 */
static void init_system(void)
{
	initUsartBuff(USART2_ID);
	initUsartBuff(USART1_ID);
	initUsartBuff(USART3_ID);

	mutex_usart1_tx = xSemaphoreCreateMutex();
	mutex_usart2_tx = xSemaphoreCreateMutex();
	mutex_usart3_tx =  xSemaphoreCreateMutex();
	
	initUsartIT(&huart1);
	initUsartIT(&huart2);
	initUsartIT(&huart3);
	
	write(USART1_ID, "USART1 ENBALE\n", sizeof("USART1 ENBALE\n")/sizeof(char));
	write(USART2_ID, "USART2 ENBALE\n", sizeof("USART2 ENBALE\n")/sizeof(char));
	write(USART3_ID, "USART3 ENBALE\n", sizeof("USART3 ENBALE\n")/sizeof(char));
}
/* function code end */

static void restart_usart(UART_HandleTypeDef *huart)
{
	if(huart==0) return;
	
	if((huart->ErrorCode |= HAL_UART_ERROR_ORE)!=RESET)
	{
		huart->ErrorCode = HAL_UART_ERROR_NONE;
		huart->RxState = HAL_UART_STATE_BUSY_RX;
		
		//SET_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
		//SET_BIT(huart->Instance->CR3, USART_CR3_EIE);

		/* Process Unlocked */
		__HAL_UNLOCK(huart);

		/* Enable the UART Parity Error Interrupt */ //使能奇偶错误中断
		__HAL_UART_ENABLE_IT(huart, UART_IT_PE);

		/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) *///启用uart中断
		__HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

		/* Enable the UART Data Register not empty Interrupt *///启用uart寄存器不是空中断
		__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
		return;
	}
}

/* 初始化串口接收中断 */
static void initUsartIT(UART_HandleTypeDef *huart)
{
		huart->ErrorCode = HAL_UART_ERROR_NONE;
		huart->RxState = HAL_UART_STATE_BUSY_RX;
			/* Process Unlocked */
		__HAL_UNLOCK(huart);

		/* Enable the UART Parity Error Interrupt */
		__HAL_UART_ENABLE_IT(huart, UART_IT_PE);

		/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
		__HAL_UART_ENABLE_IT(huart, UART_IT_ERR);

		/* Enable the UART Data Register not empty Interrupt */
		__HAL_UART_ENABLE_IT(huart, UART_IT_RXNE);
}

/* led 控制函数 50ms任务 *///小led
static void run_led_fun(void)
{
	static unsigned int step = 0;
	
	++step;
	
	if(step<=20)
	{
		set_run_led();
	}else if(step<=40)
	{
		reset_run_led();
	}else
	{
		step = 0;
	}
	
}
/*喂狗函数*/
static void feed_dog_fun(void)
{
	static unsigned int step = 0;
	
	++step;
	
	if(2==step)
	{
		step = 0;
		HAL_IWDG_Refresh(&hiwdg);
	}
	
}
/*蜂鸣器*/
static void beep_fun(void)
{
	//static unsigned int step = 0;
}

static void func_key(void)//led的亮灭水时间
{ 
	static unsigned int n = 0;	
	static unsigned int step = 0;	
	++step;
	if(!status_bled)
	{
		if(step<=20)
		{
			RGB2_Red();
		}
		else if(step<=40)
		{
			RGB2_Green();
		}	
		else
		{
			step = 0;
		}	
		return;
	}else  //通过串口控制灯
	{
		++n;
		if(n<=40)//延时2s，40*50ms
		{
			RGB2_Blue();
		}
		else
		{
			n = 0;
			step = 0;
			status_bled = 0;
		}
	}	
}

/*OLED	显示程序*/
static void update_oled_task(void const* arg)
{	
	unsigned char old_page = 0; 
  unsigned char count = 0;
	char buff[10];	
	OLED_Init();
	OLED_Clear();
	display_title();
	display_logo();
	OLED_ShowString(30, 7, "2018.11.10", 18);
	while(1)
	{	
	
	}
}


/*按键运行程序*/
static void button_event_task(void const* arg)
{  static unsigned int t = 0;	
  /* USER CODE BEGIN 4 */
	//xSemaphoreTake( button_event_take, portMAX_DELAY );	
//			if(button_enable)
//			{
//				++t;
//				if(t<=40)//延时2s，40*50ms
//				{
//					RGB2_Blue();
//				}
//			}
//			else
//			{
//				t=0;
//				button_enable=0;
//			}	
		//	xSemaphoreGive(button_event_take);
	while(1)
	{	
    
	}	
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
	switch(GPIO_Pin)
	{
		case GPIO_PIN_0:RGB2_Blue();osDelay(50);break;
		case GPIO_PIN_1:RGB2_Green();osDelay(50);break;
		default:break;
	}	return;
//	   button_enable=1;

}
/* USER CODE END 4 */

















