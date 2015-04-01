#include "stm32f10x.h"
#include "usart.h"	 		   
#include "uip.h"	    
#include "enc28j60.h"
#include "httpd.h"
#include "tcp_demo.h"
#include "modbus.h"
//TCPӦ�ýӿں���(UIP_APPCALL)
//���TCP����(����server��client)��HTTP����
void tcp_demo_appcall(void)
{	
	u16_t temp;
//	switch(uip_conn->lport)		//���ؼ����˿�80��1200 
//	{
//		case HTONS(80):
//			httpd_appcall(); 
//			break;
//		case HTONS(1200): 
//		     tcp_server_demo_appcall(); 
//			break;
//		default:						  
//		    break;
//	}
	if(uip_conn->lport == HTONS(((u16)listen_port_at_tcp_server_mode[1]<<8)+listen_port_at_tcp_server_mode[0]))		//���ؼ����˿�
	{
		  
		     tcp_server_demo_appcall();  
	}
	
	switch(uip_conn->rport)		//Զ������1400�˿�
	{
	    case HTONS(1400):
			tcp_client_demo_appcall();
	       break;
	    default: 
	       break;
	}   
}

//��ӡ��־��
void uip_log(char *m)
{			    
//	init_main_send_com();
//	printf("uIP log:%s\r\n", m);
//	uart_serial_restart(); 
}
