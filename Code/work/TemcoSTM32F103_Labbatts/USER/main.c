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
//u8 opto_refresh = 1;
u8 test_finished_flag = FALSE; 
u16 OVER_REC_TIMER = START_REC_TIMER;
u16 opto_distrigger_timer = 0;

#define ABS(A, B)		((A > B) ? (A - B) : (B - A))

static void vLED0Task( void *pvParameters );
static void vCOMMTask( void *pvParameters );
static void vKEYTask( void *pvParameters );
static void vUSBTask( void *pvParameters );
static void vUARTTask(void *pvParameters);
static void vUART2Task(void *pvParameters);
static void vSDTask( void *pvParameters );
static void vNETTask( void *pvParameters );
void uip_polling(void);
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])	
	 


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
	debug_config();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 	delay_init(72);
	LED_Init();
	uart1_init(19200);
	uart2_init(19200);
	KEY_Init();
	AT24CXX_Init();
//	Lcd_Initial();
	SPI1_Init();
	SPI2_Init();
	mem_init(SRAMIN);
	exfuns_init();
	modbus_init();
	TIM3_Int_Init(100, 7199);//20ms
	 
	
	delay_ms(2000);
	
	xTaskCreate( vCOMMTask, ( signed portCHAR * ) "COMM", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 6, NULL );
	xTaskCreate( vUART2Task, ( signed portCHAR * ) "UART2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 8, NULL );

 	xTaskCreate( vKEYTask, ( signed portCHAR * ) "KEY", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, NULL );
//	xTaskCreate( vUSBTask, ( signed portCHAR * ) "USB", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, NULL );
	xTaskCreate( vUARTTask, ( signed portCHAR * ) "UART", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 8, NULL );
	
	xTaskCreate( vSDTask, ( signed portCHAR * ) "SD", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 7, NULL );
	xTaskCreate( vLED0Task, ( signed portCHAR * ) "LED0", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate( vNETTask, ( signed portCHAR * ) "NET", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	
 	vStartMenuTask(tskIDLE_PRIORITY + 2);
	/* Start the scheduler. */
	
 	vTaskStartScheduler();
}



void vUARTTask(void *pvParameters)
{
	delay_ms(1000);
	for( ; ; )
	{ 
		if(uart_dealwithTag[UART1] == VALID_PACKET)
         { 
			   if(checkData(&Uart_Rec_Buf[UART1][0]))
               { 
                     uart_internalDeal(UART1);
//                     internalDeal();     
               }
               uart_serial_restart(UART1);           
               uart_dealwithTag[UART1] = INVALID_PACKET;
			   USART_Cmd(USART1, ENABLE);
         } 
		delay_ms(1);
	}
}

void vUART2Task(void *pvParameters)
{
	delay_ms(1000);
	for( ; ; )
	{
 
		if(uart_dealwithTag[UART2] == VALID_PACKET)
         { 
 			   if(checkData(&Uart_Rec_Buf[UART2][0]))
               { 
                    
                     uart_internalDeal(UART2);     
               }
               uart_serial_restart(UART2);          
               uart_dealwithTag[UART2] = INVALID_PACKET;
			   USART_Cmd(USART2, ENABLE);
         } 
 
		delay_ms(1);
	}
}
void vLED0Task( void *pvParameters )
{
//	u16 temp,i;
	delay_ms(1000); 
	for( ;; )
	{  
		if((rm3100[0].result)||(rm3100[1].result)) 
			LED0 = ~LED0; 				
			  
		delay_ms(300); 
	}
}

extern u32 count_out;
extern u8 buffer_out[64];
void vUSBTask( void *pvParameters )
{
//	u8 i;
//	USB_Lowlevel_Init();

	for( ;; )
	{
		;		
	}
}

void test()
{
	print_buf[0].RM3100_0[X_AXIS] = 1;
	print_buf[0].RM3100_0[Y_AXIS] = 2;
	print_buf[0].RM3100_0[Z_AXIS] = 3;
	print_buf[0].RM3100_1[X_AXIS] = 4;
	print_buf[0].RM3100_1[Y_AXIS] = 5;
	print_buf[0].RM3100_1[Z_AXIS] = 6;
	print_buf[0].opto_status = 13;
	print_buf[1].RM3100_0[X_AXIS] = 7;
	print_buf[1].RM3100_0[Y_AXIS] = 8;
	print_buf[1].RM3100_0[Z_AXIS] = 9;
	print_buf[1].RM3100_1[X_AXIS] = 10;
	print_buf[1].RM3100_1[Y_AXIS] = 11;
	print_buf[1].RM3100_1[Z_AXIS] = 12; 
	print_buf[1].opto_status = 14;
	print_buf[40].RM3100_0[Y_AXIS] = 22;
	print_buf[40].RM3100_0[Z_AXIS] = 23;
	print_buf[40].RM3100_1[X_AXIS] = 24;
	print_buf[40].RM3100_1[Y_AXIS] = 25;
	print_buf[40].RM3100_1[Z_AXIS] = 26;
	print_buf[40].opto_status = 13;
	print_buf[41].RM3100_0[X_AXIS] = 27;
	print_buf[41].RM3100_0[Y_AXIS] = 28;
	print_buf[41].RM3100_0[Z_AXIS] = 29;
	print_buf[41].RM3100_1[X_AXIS] = 30;
	print_buf[41].RM3100_1[Y_AXIS] = 31;
	print_buf[41].RM3100_1[Z_AXIS] = 32; 
	print_buf[41].opto_status = 14; 	
	uart_buf_len = 41;
}
void vSDTask( void *pvParameters )
{
     u16  i;
//	u8  led_record = 0;
	delay_ms(1000);

	mf_mount(0);
 
	for( ;; )
	{
//		if(led_record>= 80)
//		{
//			LED0 = ~LED0;
//			led_record = 0;
//		}
//		else
//			led_record++;	 
		
		if(test_finished_flag==TRUE)
		{
 		   // memcpy(tcp_buf,print_buf,rec_timer*PRINT_BUF_SIZE); 
			tcp_buf_ready = 1;
//			rec_timer = 100;
			uart_buf_len = rec_timer;
//			test();
 			memcpy(tcp_buf,print_buf,rec_timer*PRINT_BUF_SIZE); 
			
 			rm3100_detect(); 
			
			mf_open("0:/labatts.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);
			mf_lseek(mf_size()); 
			sprintf((char *)text, "    filter1 = %8d,       filter2 = %8d,  sensitivity1 = %8d,  sensitivity2 = %8d\r\n",\
			rm3100[0].filter,rm3100[1].filter,rm3100[0].sensitivity,rm3100[1].sensitivity); 
 			mf_puts(text);
 			sprintf((char *)text, "Total_Timer = %8d,Triggle_number = %8d,sensor1_result = %8d,sensor2_result = %8d\r\n",\
 			rm3100[0].data_number,rm3100[0].triggle_number,rm3100[0].result,rm3100[1].result); 
 			mf_puts(text);
         	for(i=0;i<rec_timer;i++)
//  			for(i=0;i<100;i++)
			{
					
					sprintf((char *)text, "timer = %8d,opto = %8d, X1 = %8d, Y1 = %8d, Z1 = %8d,X2 = %8d, Y2 = %8d, Z2 = %8d\r\n",\
					i,print_buf[i].opto_status,\
					print_buf[i].RM3100_0[X_AXIS],print_buf[i].RM3100_0[Y_AXIS], print_buf[i].RM3100_0[Z_AXIS],\
					print_buf[i].RM3100_1[X_AXIS],print_buf[i].RM3100_1[Y_AXIS], print_buf[i].RM3100_1[Z_AXIS]);
					//n = strlen(text)
					mf_puts(text);
			}  			
			
			
			mf_close();
 			 
			test_finished_flag=FALSE;
				
			rec_timer = 0; 	 		 		 
		}
		delay_ms(5);			
	} 
}

//u8 myMAC[] = {0x00, 0x0e, 0x00, 0x25, 0x36, 0x64}; 
void vCOMMTask( void *pvParameters )
{
 
	 
	u16 i,temp,j; 
	s32 temp_buf[2][3];
	BL_ON(); 
	
	
//	mf_mount(0);
	
    		    //�������ӵ�TCP Server��,����TCP Client
	rm3100_initial();
	FGM3_Init(); 
	delay_ms(500);
    for( ;; )
	{ 
		//opto triggle
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == 0) 
		{ 
			if(opto_capture != TRUE) 
			{
				opto_capture = TRUE;
				rm3100[0].triggle_number++;
			}
 		}
		else
		{
			opto_capture = FALSE; 
		}
		//set offset 
		for(i=0;i<2;i++)
		{
			for(j=0;j<TOTAL_AXIS;j++)
			{    
				temp_buf[i][j] = rm3100[i].input[j] - rm3100[i].offset[j];	
			} 
		} 
        //for display
		display.x1 = temp_buf[0][X_AXIS];
		display.x2 = temp_buf[1][X_AXIS];
		display.y1 = temp_buf[0][Y_AXIS];
		display.y2 = temp_buf[1][Y_AXIS];
		display.z1 = temp_buf[0][Z_AXIS];
		display.z2 = temp_buf[1][Z_AXIS];  
		display.left_result  = rm3100[0].result;
		display.right_result = rm3100[1].result;
		display.triggle_number = rm3100[0].triggle_number;
			
		//print buffer	
		if((fgm3.status == 1)&&(test_finished_flag == FALSE))
		{ 
				fgm3.status = 0;	
				print_buf[rec_timer].RM3100_0[X_AXIS] = temp_buf[0][X_AXIS];
				print_buf[rec_timer].RM3100_0[Y_AXIS] = temp_buf[0][Y_AXIS];
				print_buf[rec_timer].RM3100_0[Z_AXIS] = temp_buf[0][Z_AXIS];
			    print_buf[rec_timer].RM3100_1[X_AXIS] = temp_buf[1][X_AXIS];
				print_buf[rec_timer].RM3100_1[Y_AXIS] = temp_buf[1][Y_AXIS];
				print_buf[rec_timer].RM3100_1[Z_AXIS] = temp_buf[1][Z_AXIS];
//				print_buf[rec_timer].FGM_1 = fgm3.ch1 ;
//				print_buf[rec_timer].FGM_2 = fgm3.ch2 ;
				
				FGM3_enable_read();
		
				if(opto_capture == FALSE)
				{
					print_buf[rec_timer].opto_status  = 0;
					if (rec_timer == START_REC_TIMER)
					{
						for(i=1;i<=START_REC_TIMER;i++)//<<1
						{
							temp = i-1;
							print_buf[temp].RM3100_0[X_AXIS] = print_buf[i].RM3100_0[X_AXIS];
							print_buf[temp].RM3100_0[Y_AXIS] = print_buf[i].RM3100_0[Y_AXIS];
							print_buf[temp].RM3100_0[Z_AXIS] = print_buf[i].RM3100_0[Z_AXIS];
							print_buf[temp].RM3100_1[X_AXIS] = print_buf[i].RM3100_1[X_AXIS];
							print_buf[temp].RM3100_1[Y_AXIS] = print_buf[i].RM3100_1[Y_AXIS];
							print_buf[temp].RM3100_1[Z_AXIS] = print_buf[i].RM3100_1[Z_AXIS]; 
//							print_buf[temp].FGM_1 = print_buf[i].FGM_1;
//							print_buf[temp].FGM_2 = print_buf[i].FGM_2;
						}
					}
					else if(rec_timer<START_REC_TIMER) 
						rec_timer++;
					else if((rec_timer>START_REC_TIMER)&&(rec_timer<OVER_REC_TIMER))
						rec_timer++;
					else
					{
						test_finished_flag = TRUE;
						rm3100[0].data_number = opto_distrigger_timer-START_REC_TIMER;
						rm3100[1].data_number = rm3100[0].data_number;
						
					}
				}
				else
				{
					print_buf[rec_timer].opto_status  = 100;
					if(rec_timer<REC_TIMER_TOTAL)
							rec_timer++;
					opto_distrigger_timer = rec_timer;
					OVER_REC_TIMER = rec_timer+END_REC_TIMER;
					if(OVER_REC_TIMER >REC_TIMER_TOTAL) OVER_REC_TIMER =REC_TIMER_TOTAL;	
				}
		}
 		 
		delay_ms(1);
    }
}

void vKEYTask( void *pvParameters )
{
	u8 key_temp;
	static u8 pre_key = KEY_NON;
	
	delay_ms(1000);
	
	for( ;; )
	{
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

		delay_ms(100);
    }
}

//uip�¼�������
//���뽫�ú��������û���ѭ��,ѭ������.
void uip_polling(void)
{
	u8 i; 
	static struct timer periodic_timer, arp_timer;
	static u8 timer_ok = 0;	 
	 
	if(timer_ok == 0)		//����ʼ��һ��
	{
		timer_ok = 1;
		timer_set(&periodic_timer, CLOCK_SECOND / 2); 	//����1��0.5��Ķ�ʱ�� 
		timer_set(&arp_timer, CLOCK_SECOND * 10);	   	//����1��10��Ķ�ʱ�� 
	}
	
	uip_len = tapdev_read();							//�������豸��ȡһ��IP��,�õ����ݳ���.uip_len��uip.c�ж���
	if(uip_len > 0)							 			//������
	{   
		//����IP���ݰ�(ֻ��У��ͨ����IP���Żᱻ����) 
		if(BUF->type == htons(UIP_ETHTYPE_IP))			//�Ƿ���IP��? 
		{
			uip_arp_ipin();								//ȥ����̫��ͷ�ṹ������ARP��
			uip_input();   								//IP������
			
			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ��� uip_len > 0
			//��Ҫ���͵�������uip_buf, ������uip_len  (����2��ȫ�ֱ���)		    
			if(uip_len > 0)								//��Ҫ��Ӧ����
			{
				uip_arp_out();							//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();							//�������ݵ���̫��
			}
		}
		else if (BUF->type == htons(UIP_ETHTYPE_ARP))	//����arp����,�Ƿ���ARP�����?
		{
			uip_arp_arpin();
			
 			//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len(����2��ȫ�ֱ���)
 			if(uip_len > 0)
				tapdev_send();							//��Ҫ��������,��ͨ��tapdev_send����	 
		}
	}
	else if(timer_expired(&periodic_timer))				//0.5�붨ʱ����ʱ
	{
		timer_reset(&periodic_timer);					//��λ0.5�붨ʱ�� 
		
		//��������ÿ��TCP����, UIP_CONNSȱʡ��40��  
		for(i = 0; i < UIP_CONNS; i++)
		{
			uip_periodic(i);							//����TCPͨ���¼�
			
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
	 		if(uip_len > 0)
			{
				uip_arp_out();							//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();							//�������ݵ���̫��
			}
		}
		
#if UIP_UDP	//UIP_UDP 
		//��������ÿ��UDP����, UIP_UDP_CONNSȱʡ��10��
		for(i = 0; i < UIP_UDP_CONNS; i++)
		{
			uip_udp_periodic(i);						//����UDPͨ���¼�
			
	 		//������ĺ���ִ�к������Ҫ�������ݣ���ȫ�ֱ���uip_len>0
			//��Ҫ���͵�������uip_buf, ������uip_len (����2��ȫ�ֱ���)
			if(uip_len > 0)
			{
				uip_arp_out();							//����̫��ͷ�ṹ������������ʱ����Ҫ����ARP����
				tapdev_send();							//�������ݵ���̫��
			}
		}
#endif 
		//ÿ��10�����1��ARP��ʱ������ ���ڶ���ARP����,ARP��10�����һ�Σ��ɵ���Ŀ�ᱻ����
		if(timer_expired(&arp_timer))
		{
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}


u8 const myMAC[] = {0x00, 0x0e, 0x00, 0x25, 0x36, 0x64}; 
void vNETTask( void *pvParameters )
{
//	uip_ipaddr_t ipaddr;
//	u16 temp;
//	u8 tcp_server_tsta = 0XFF;
//	u8 tcp_client_tsta = 0XFF;
	 
	BL_OFF();
// 	printf("Temco ARM Demo\r\n");
	delay_ms(500);
	
	while(tapdev_init())	//��ʼ��ENC28J60����
	{								   
// 		printf("ENC28J60 Init Error!\r\n");
		delay_ms(200);
	};		
//	printf("ENC28J60 Init Successful!\r\n");
 
    for( ;; )
	{ 
		uip_polling();	//����uip�¼���������뵽�û������ѭ������  		 
		delay_ms(5);
    }
}
 
