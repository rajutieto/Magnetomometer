#include <string.h>
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "24cxx.h"
#include "spi.h"
#include "lcd.h"
#include "touch.h"
#include "flash.h"
#include "stmflash.h"
#include "rtc.h"
//#include "sdcard.h"
#include "mmc_sd.h"
#include "dma.h"
#include "vmalloc.h"
#include "ff.h"
#include "fattester.h"
#include "exfuns.h"
#include "enc28j60.h"
#include "timerx.h"
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "usb_app.h"
#include "labatts.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "modbus.h"
#include "../menu/menu.h"
#include "rm3100.h"
#include "tcp_demo.h"    
#define ABS(A, B)		((A > B) ? (A - B) : (B - A))

static void vLED0Task( void *pvParameters ); 
static void vKEYTask( void *pvParameters );
static void vUSBTask( void *pvParameters );
static void vUARTTask(void *pvParameters); 
static void vSDTask( void *pvParameters );
static void vNETTask( void *pvParameters );
void uip_polling(void);

#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	
	 
xQueueHandle qKey;

u8 Threshold = 5;
u8 opto_err = 60;

u8 get_metal = 0; 
static void debug_config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
}

int main(void)
{
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x8008000);
	debug_config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 	delay_init(72); 
	AT24CXX_Init(); 
	RTC_Init();
	mem_init(SRAMIN);
	 
//	TIM3_Int_Init(100, 7199);//20ms 
	delay_ms(1000);
	
	 
 	xTaskCreate( vKEYTask, ( signed portCHAR * ) "KEY", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL );
//	xTaskCreate( vUSBTask, ( signed portCHAR * ) "USB", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, NULL );
	xTaskCreate( vUARTTask, ( signed portCHAR * ) "UART", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 8, NULL );
	
	xTaskCreate( vSDTask, ( signed portCHAR * ) "SD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 7, NULL );
	xTaskCreate( vLED0Task, ( signed portCHAR * ) "LED0", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate( vNETTask, ( signed portCHAR * ) "NET", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	
 	vStartMenuTask(tskIDLE_PRIORITY + 2);
	vStartPNITask( tskIDLE_PRIORITY + 6);
	/* Start the scheduler. */ 
 	vTaskStartScheduler();
}



void vUARTTask(void *pvParameters)
{
	
	modbus_init();
	if(Modbus.baudrate)
		uart1_init(19200);
	else
		uart1_init(9600); 
	delay_ms(1000);
	for( ; ; )
	{ 
		if(uart_dealwithTag == VALID_PACKET)
        {
			u16 address; 
			if(checkData())
			{  
				 //given this is used in multiple places, decided to put it as an argument
				 address = ((u16)Uart_Rec_Buf[2] << 8) + Uart_Rec_Buf[3];
				 //MDF 6/25/04 changed the timing of responses to valid and invalid packets.
				 //Initialize tranmission
				 init_main_send_com();
				 //Initialize CRC
				 init_crc16();
				 //Store any data being written
				 internalDeal(address);
				 //Respond with any data requested
				 
				 responseData(address);
				 //main_serial_restart();     
			}
			else
				uart_serial_restart(); 
			uart_dealwithTag = INVALID_PACKET;
		}
		if(serial_receive_timeout_count > 0)
		{
			  serial_receive_timeout_count--;
			  if(serial_receive_timeout_count == 0)
			  {
					uart_serial_restart();
			  }
		}
		tick_ctr++;
		delay_ms(5);
	} 	
}

 
void vLED0Task( void *pvParameters )
{ 
	LED_Init();
	delay_ms(1000); 
	for( ;; )
	{   
		LED0 = ~LED0; 				
		if(calendar_ghost_enable == ENABLE)
		{
			calendar_ghost_enable = DISABLE;
			Time_Adjust();
		}
//		uip_gethostaddr(ip_address_ghost);
//		uip_getdraddr(gateway_address_ghost);
//		uip_getnetmask(subnet_mask_address_ghost);
		delay_ms(500); 
	}
}

extern u32 count_out;
extern u8 buffer_out[64];
void vUSBTask( void *pvParameters )
{ 

	for( ;; )
	{
		;		
	}
}

 
void vSDTask( void *pvParameters )
{
    u16  i; 
	SPI2_Init();
	exfuns_init();
	delay_ms(100); 
	
	mf_mount(0);
	mf_open((void *)"0:/labatts.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
	mf_lseek(mf_size());
	sprintf((char *)text, "sd card have created successfully\r\n");
	mf_puts(text);
	sprintf((char *)text, "0:/labatts_%d",calendar.sec);
	mf_puts(text);
	mf_close(); 
	for( ;; )
	{ 
		SD_CS_Output();
		delay_us(100);	
		
// 		sprintf((char *)text, "0:/%d-%d.txt",calendar.w_month,calendar.w_date);
//		mf_open(text, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
//		mf_lseek(mf_size());
//		sprintf((char *)text, "TIME = %d-%d\r\n",calendar.min,calendar.sec); 
//		mf_puts(text);
//		mf_close(); 
		
		
 		
		if(pni_para.record_over_flag==TRUE)
		{
 		    pni_para.record_over_flag=FALSE;
			tcp_buf_ready = TRUE;  
			uart_buf_len = pni_para.pni_record_number; 
 			memcpy(TCP_Buffer,pni_buffer,pni_para.pni_record_number*PNI_BUF_SIZE); 
			 
			sprintf((char *)text, "0:/%d-%d.txt",calendar.w_month,calendar.w_date);
			mf_open(text, FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
			
			mf_lseek(mf_size());  
 			sprintf((char *)text, "Total_Timer = %8d,Triggle_number = %8d\r\n",\
 			pni_para.pni_record_number,pni_para.opto_triggle_number); 
 			mf_puts(text);
         	for(i=0;i<pni_para.pni_record_number;i++) 
			{ 
					sprintf((char *)text, "timer = %8d,opto = %8d, X1 = %8d, Y1 = %8d, Z1 = %8d,X2 = %8d, Y2 = %8d, Z2 = %8d\r\n",\
					i+1,pni_buffer[i].Opto_Status,\
					pni_buffer[i].Sensor0[X_AXIS],pni_buffer[i].Sensor0[Y_AXIS], pni_buffer[i].Sensor0[Z_AXIS],\
					pni_buffer[i].Sensor1[X_AXIS],pni_buffer[i].Sensor1[Y_AXIS], pni_buffer[i].Sensor1[Z_AXIS]);
					mf_puts(text);
			}  		 
			mf_close();   	 		 		 
		} 
 		

		SD_CS_Input();
		delay_us(100);
		
		if(SD_CS_IN)				//insert the sd card
		{
 			if(SD_Card_Status < 3)
				SD_Card_Status++;
		    if(SD_Card_Status == 2)
				mf_mount(0);
			LED1 = 0;
			
		}
		else						//the sd card has been removed.
		{ 
			SD_Card_Status =0;
			LED1 = 1;
		} 
		
		delay_ms(100);			
	} 
}

//u8 myMAC[] = {0x00, 0x0e, 0x00, 0x25, 0x36, 0x64}; 
 

void vKEYTask( void *pvParameters )
{
	u16 key_temp;
	static u16 pre_key = KEY_NON;
	static U8_T long_press_key_start = 0;
	qKey = xQueueCreate(10, 2);
	KEY_Init();
	delay_ms(1000);
	
	for( ;; )
	{
/*
		key_temp = KEY_Scan();
		if(pre_key != key_temp)
		{
			if(key_temp != KEY_NON)
			{
				if(key_temp & KEY_1)
				{
					global_key = KEY_1; 
					 
				}
				else if(key_temp & KEY_2)
				{
					global_key = KEY_2;
					 
				}
				else if(key_temp & KEY_3)
				{
					global_key = KEY_3;
					 
				}
				else if(key_temp & KEY_4)
				{
					global_key = KEY_4;
					 
				}
			}
			
			pre_key = key_temp;
		}
		
*/		
		if((key_temp = KEY_Scan()) != pre_key)
		{
			xQueueSend(qKey, &key_temp, 0);
			pre_key = key_temp;
			long_press_key_start = 0;
		}
		else
		{
			if(key_temp != KEY_NON)
			{
				if(long_press_key_start >= LONG_PRESS_TIMER_SPEED_100)
					key_temp |= KEY_SPEED_100;
				else if(long_press_key_start >= LONG_PRESS_TIMER_SPEED_50)
					key_temp |= KEY_SPEED_50;
				else if(long_press_key_start >= LONG_PRESS_TIMER_SPEED_10)
					key_temp |= KEY_SPEED_10;
				else if(long_press_key_start >= LONG_PRESS_TIMER_SPEED_1)
					key_temp |= KEY_SPEED_1;

				if(long_press_key_start >= LONG_PRESS_TIMER_SPEED_1)
					xQueueSend(qKey, &key_temp, 0);

				if(long_press_key_start < LONG_PRESS_TIMER_SPEED_100)
					long_press_key_start++;
			}
		}
		
		delay_ms(20);
    }
}

//uip事件处理函数
//必须将该函数插入用户主循环,循环调用.
void uip_polling(void)
{
	u8 i; 
	static struct timer periodic_timer, arp_timer;
	static u8 timer_ok = 0;	 
	 
	if(timer_ok == 0)		//仅初始化一次
	{
		timer_ok = 1;
		timer_set(&periodic_timer, CLOCK_SECOND / 2); 	//创建1个0.5秒的定时器 
		timer_set(&arp_timer, CLOCK_SECOND * 10);	   	//创建1个10秒的定时器 
	}
	
	uip_len = tapdev_read();							//从网络设备读取一个IP包,得到数据长度.uip_len在uip.c中定义
	if(uip_len > 0)							 			//有数据
	{   
		//处理IP数据包(只有校验通过的IP包才会被接收) 
		if(BUF->type == htons(UIP_ETHTYPE_IP))			//是否是IP包? 
		{
			uip_arp_ipin();								//去除以太网头结构，更新ARP表
			uip_input();   								//IP包处理
			
			//当上面的函数执行后，如果需要发送数据，则全局变量 uip_len > 0
			//需要发送的数据在uip_buf, 长度是uip_len  (这是2个全局变量)		    
			if(uip_len > 0)								//需要回应数据
			{
				uip_arp_out();							//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();							//发送数据到以太网
			}
		}
		else if (BUF->type == htons(UIP_ETHTYPE_ARP))	//处理arp报文,是否是ARP请求包?
		{
			uip_arp_arpin();
			
 			//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len(这是2个全局变量)
 			if(uip_len > 0)
				tapdev_send();							//需要发送数据,则通过tapdev_send发送	 
		}
	}
	else if(timer_expired(&periodic_timer))				//0.5秒定时器超时
	{
		timer_reset(&periodic_timer);					//复位0.5秒定时器 
		
		//轮流处理每个TCP连接, UIP_CONNS缺省是40个  
		for(i = 0; i < UIP_CONNS; i++)
		{
			uip_periodic(i);							//处理TCP通信事件
			
	 		//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
	 		if(uip_len > 0)
			{
				uip_arp_out();							//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();							//发送数据到以太网
			}
		}
		
#if UIP_UDP	//UIP_UDP 
		//轮流处理每个UDP连接, UIP_UDP_CONNS缺省是10个
		for(i = 0; i < UIP_UDP_CONNS; i++)
		{
			uip_udp_periodic(i);						//处理UDP通信事件
			
	 		//当上面的函数执行后，如果需要发送数据，则全局变量uip_len>0
			//需要发送的数据在uip_buf, 长度是uip_len (这是2个全局变量)
			if(uip_len > 0)
			{
				uip_arp_out();							//加以太网头结构，在主动连接时可能要构造ARP请求
				tapdev_send();							//发送数据到以太网
			}
		}
#endif 
		//每隔10秒调用1次ARP定时器函数 用于定期ARP处理,ARP表10秒更新一次，旧的条目会被抛弃
		if(timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}


//u8 const myMAC[] = {0x00, 0x0e, 0x00, 0x25, 0x36, 0x64}; 
void vNETTask( void *pvParameters )
{
	static u16 run_timer;
	static u8 get_ip_flag = 0;
//	uip_ipaddr_t ipaddr;
//	u16 temp;
//	u8 tcp_server_tsta = 0XFF;
//	u8 tcp_client_tsta = 0XFF;
	tcp_server_initial();
	
 	SPI3_Init(); 
// 	printf("Temco ARM Demo\r\n");
	tapdev_init() ;
	
	delay_ms(500);  
	
	for( ;; )
	{ 
		run_timer++;
		if(run_timer%100 == 0) LED3 = ~LED3; 
		
		if(get_ip_flag ==0)
		{
			uip_gethostaddr(ip_address_ghost);
			uip_getdraddr(gateway_address_ghost);
			uip_getnetmask(subnet_mask_address_ghost);
			if((ip_address_ghost[0] != ip_address_ghost[1])||(ip_address_ghost[0] != ip_address_ghost[2])||\
			   (ip_address_ghost[0] != ip_address_ghost[3])||(ip_address_ghost[0] != 0))
			{
				get_ip_flag = 1;				
				uip_listen(HTONS(((u16)listen_port_at_tcp_server_mode[1]<<8)+listen_port_at_tcp_server_mode[0]));
			}
			else
			{ 
				if(ip_mode == 0)
				{
					ip_address_ghost[0] = 192;
					ip_address_ghost[1] = 168;
					ip_address_ghost[2] = 0;
					ip_address_ghost[3] = 158; 
					enable_ghost = ENABLE;
				} 
			}
		}
		if(enable_ghost == ENABLE)
		{
 			tcp_server_config();
			tapdev_init();	  
			enable_ghost = DISABLE;
			get_ip_flag = 0;
		}
		
 		uip_polling();	//处理uip事件，必须插入到用户程序的循环体中  		 
		delay_ms(5);
    }
}
 
