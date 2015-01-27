#include "tcp_demo.h"
#include "uip.h"
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "led.h"
#include "RM3100.h"

u8 send_flag = 0; 
u16 uart_buf_len;
u8 tcp_buf_ready;

u8   mac_address[6];//registers: 100 101 102 103 104 105
u8   ip_mode = 0;//register: 106, 0 = static, 1 = DHCP
u8   ip_address[4] = {192, 168, 0, 3};//registers: 107 108 109 110
u8   subnet_mask_address[4] = {255, 255, 255, 0};//registers: 111 112 113 114
u8   gateway_address[4] = {192, 168, 0, 4};//registers: 115 116 117 118
u8   tcp_server_enable = 0;//registers 119
u8   listen_port_at_tcp_server_mode[2] = {0x17, 0x71};//register 120 121

u8   ip_mode_ghost = 0;//register: 106, 0 = static, 1 = DHCP
u8   ip_address_ghost[4] = {0, 0, 0, 0};//registers: 107 108 109 110
u8   subnet_mask_address_ghost[4] = {0, 0, 0, 0};//registers: 111 112 113 114
u8   gateway_address_ghost[4] = {0, 0, 0, 0};//registers: 115 116 117 118
u8   tcp_server_enable_ghost = 0;//registers 119
u8   listen_port_at_tcp_server_mode_ghost[2] = {0, 0};//register 120 121
u8   enable_ghost = 0;

u8 tcp_server_databuf[1024];   	//�������ݻ���	  
u16 tcp_buf_len;
u8 tcp_server_sta;				//�����״̬

 	   
//����һ��TCP ������Ӧ�ûص�������
//�ú���ͨ��UIP_APPCALL(tcp_demo_appcall)����,ʵ��Web Server�Ĺ���.
//��uip�¼�����ʱ��UIP_APPCALL�����ᱻ����,���������˿�(1200),ȷ���Ƿ�ִ�иú�����
//���� : ��һ��TCP���ӱ�����ʱ�����µ����ݵ�������Ѿ���Ӧ��������Ҫ�ط����¼�
void tcp_server_demo_appcall(void)
{
 	u16 temp;
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	if(uip_aborted())tcp_server_aborted();		//������ֹ
 	if(uip_timedout())tcp_server_timedout();	//���ӳ�ʱ   
	if(uip_closed())tcp_server_closed();		//���ӹر�	   
 	if(uip_connected())tcp_server_connected();	//���ӳɹ�	    
	if(uip_acked())tcp_server_acked();			//���͵����ݳɹ��ʹ� 
	//���յ�һ���µ�TCP���ݰ� 
	if(uip_newdata())//�յ��ͻ��˷�����������
	{
//		if((tcp_server_sta & (1 << 6)) == 0)	//��δ�յ�����
//		{
//			if(uip_len > 499)
//			{		   
//				((u8*)uip_appdata)[499] = 0;
//			}
//		    
//	    	strcpy((char*)tcp_server_databuf, uip_appdata);				   	  		  
//			tcp_server_sta |= 1 << 6;			//��ʾ�յ��ͻ�������
//		}
		if(strcmp(uip_appdata,"send") == 0)
			{
				send_flag =1;
			} 
	}
//	else if(tcp_server_sta & (1 << 5))			//��������Ҫ����
//	{
//		s->textptr = tcp_server_databuf;
//		s->textlen = strlen((const char*)tcp_server_databuf);
//		tcp_server_sta &= ~(1 << 5);			//������
//	}
	
	//����Ҫ�ط��������ݵ�����ݰ��ʹ���ӽ���ʱ��֪ͨuip�������� 
	if(uip_rexmit() || uip_newdata() || uip_acked() || uip_connected() || uip_poll())
	{
		if(send_flag == 1)
		{
			if(tcp_buf_ready == 1)
			{	
				if(uart_buf_len > BUF_LEN)
				{	
					temp = BUF_LEN;
				}
				else
					temp = uart_buf_len;
				
				tcp_buf_len = temp*PRINT_BUF_SIZE; 
				memcpy(tcp_server_databuf,(u8*)tcp_buf,tcp_buf_len);	
				
				uart_buf_len -=temp;
				if(uart_buf_len == 0)
				{
					tcp_buf_ready =0;
					send_flag = 0;
				}
				memcpy(tcp_buf,&tcp_buf[temp],uart_buf_len*PRINT_BUF_SIZE);
				
				s->textptr = tcp_server_databuf;
				s->textlen =tcp_buf_len;//strlen((const char*)tcp_server_databuf);
			}
			else
			{
				//memcpy(tcp_server_databuf,"over",4);
				s->textptr = tcp_server_databuf;
				s->textlen =0;//strlen((const char*)tcp_server_databuf);
				send_flag = 0;
			}
		}
		tcp_server_senddata();
	}
}

//��ֹ����				    
void tcp_server_aborted(void)
{
	tcp_server_sta &= ~(1 << 7);				//��־û������
	uip_log("tcp_server aborted!\r\n");			//��ӡlog
}

//���ӳ�ʱ
void tcp_server_timedout(void)
{
	tcp_server_sta &= ~(1 << 7);				//��־û������
	uip_log("tcp_server timeout!\r\n");			//��ӡlog
}

//���ӹر�
void tcp_server_closed(void)
{
	tcp_server_sta &= ~(1 << 7);				//��־û������
	uip_log("tcp_server closed!\r\n");			//��ӡlog
}

//���ӽ���
void tcp_server_connected(void)
{								  
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	//uip_conn�ṹ����һ��"appstate"�ֶ�ָ��Ӧ�ó����Զ���Ľṹ�塣
	//����һ��sָ�룬��Ϊ�˱���ʹ�á�
 	//����Ҫ�ٵ���Ϊÿ��uip_conn�����ڴ棬����Ѿ���uip�з�����ˡ�
	//��uip.c �� ����ش������£�
	//		struct uip_conn *uip_conn;
	//		struct uip_conn uip_conns[UIP_CONNS]; //UIP_CONNSȱʡ=10
	//������1�����ӵ����飬֧��ͬʱ�����������ӡ�
	//uip_conn��һ��ȫ�ֵ�ָ�룬ָ��ǰ��tcp��udp���ӡ�
	tcp_server_sta |= 1 << 7;					//��־���ӳɹ�
  	uip_log("tcp_server connected!\r\n");		//��ӡlog
	s->state = STATE_CMD; 						//ָ��״̬
	s->textlen = 0;
	s->textptr = "Connect to STM32 Board Successfully!\r\n";
	s->textlen = strlen((char *)s->textptr);
}

//���͵����ݳɹ��ʹ�
void tcp_server_acked(void)
{						    	 
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	s->textlen = 0;								//��������
	uip_log("tcp_server acked!\r\n");			//��ʾ�ɹ�����		 
}

//�������ݸ��ͻ���
void tcp_server_senddata(void)
{
	struct tcp_demo_appstate *s = (struct tcp_demo_appstate *)&uip_conn->appstate;
	//s->textptr : ���͵����ݰ�������ָ��
	//s->textlen �����ݰ��Ĵ�С����λ�ֽڣ�		   
	if(s->textlen > 0)
		uip_send(s->textptr, s->textlen);//����TCP���ݰ�	 
}
