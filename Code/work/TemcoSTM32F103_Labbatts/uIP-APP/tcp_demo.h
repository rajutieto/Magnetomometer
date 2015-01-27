#ifndef __TCP_DEMO_H__
#define __TCP_DEMO_H__		 
/* Since this file will be included by uip.h, we cannot include uip.h
   here. But we might need to include uipopt.h if we need the u8_t and
   u16_t datatypes. */
#include "uipopt.h"
#include "psock.h"
#include "bitmap.h"

//ͨ�ų���״̬��(�û������Լ�����)  
enum
{
	STATE_CMD		= 0,	//�������״̬ 
	STATE_TX_TEST	= 1,	//�����������ݰ�״̬(�ٶȲ���)  
	STATE_RX_TEST	= 2		//�����������ݰ�״̬(�ٶȲ���)  
};	 
//���� uip_tcp_appstate_t �������ͣ��û��������Ӧ�ó�����Ҫ�õ�
//��Ա��������Ҫ���Ľṹ�����͵����֣���Ϊ����������ᱻuip���á�
//uip.h �ж���� 	struct uip_conn  �ṹ���������� uip_tcp_appstate_t		  
struct tcp_demo_appstate
{
	u8_t state;
	u8_t *textptr;
	int textlen;
};	 
typedef struct tcp_demo_appstate uip_tcp_appstate_t;

void tcp_demo_appcall(void);
void tcp_client_demo_appcall(void);
void tcp_server_demo_appcall(void);

//����Ӧ�ó���ص����� 
#ifndef UIP_APPCALL
#define UIP_APPCALL tcp_demo_appcall //����ص�����Ϊ tcp_demo_appcall 
#endif
/////////////////////////////////////TCP SERVER/////////////////////////////////////
extern u8 tcp_server_databuf[];   		//�������ݻ���	 
extern u8 tcp_server_sta;				//�����״̬   
//tcp server ����
void tcp_server_aborted(void);
void tcp_server_timedout(void);
void tcp_server_closed(void);
void tcp_server_connected(void);
void tcp_server_newdata(void);
void tcp_server_acked(void);
void tcp_server_senddata(void);
/////////////////////////////////////TCP CLIENT/////////////////////////////////////
extern u8 tcp_client_databuf[];   		//�������ݻ���	 
extern u8 tcp_client_sta;				//�ͻ���״̬   
void tcp_client_reconnect(void);
void tcp_client_connected(void);
void tcp_client_aborted(void);
void tcp_client_timedout(void);
void tcp_client_closed(void);
void tcp_client_acked(void);
void tcp_client_senddata(void);
//////////////////////////////////////////////////////////////////////////////////// 
#define BUF_LEN			40

extern u8 mac_address[6];//registers: 100 101 102 103 104 105
extern u8 ip_mode ;//register: 106, 0 = static, 1 = DHCP
extern u8 ip_address[4];//registers: 107 108 109 110
extern u8 subnet_mask_address[4];//registers: 111 112 113 114
extern u8 gateway_address[4];//registers: 115 116 117 118
extern u8 tcp_server_enable;//registers 119
extern u8 listen_port_at_tcp_server_mode[2];//register 120 121

extern u8 ip_mode_ghost;//register: 106, 0 = static, 1 = DHCP
extern u8 ip_address_ghost[4];//registers: 107 108 109 110
extern u8 subnet_mask_address_ghost[4];//registers: 111 112 113 114
extern u8 gateway_address_ghost[4];//registers: 115 116 117 118
extern u8 tcp_server_enable_ghost;//registers 119
extern u8 listen_port_at_tcp_server_mode_ghost[2];//register 120 121
extern u8 enable_ghost; 
extern u16 uart_buf_len;
extern u8 tcp_buf_ready;

#endif
























