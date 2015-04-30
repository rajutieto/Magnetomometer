#include "stm32f10x.h"
#include "usart1.h" 
#include "math.h"

/*	the header of test function  */
#include "rt_nonfinite.h"
#include "adjustRollerData.h"
#include "adjustRollerData_initialize.h"
#include "adjustRollerData_emxutil.h"
#include "rand.h"


/**
  * @brief  Main program.
  * @param  None
  * @retval : None
  */
u8 print_buffer[1000];
 
const real_T roller_resultant[205] = {  //define the input 
87.8179936004006,
87.6869431557515,
87.6869431557515,
88.4081444212014,
89.5712007288057,
89.5712007288057,
90.5317623820502,
90.5317623820502,
90.5317623820502,
92.2388204608017,
92.2388204608017,
92.2388204608017,
94.4563391202517,
94.4563391202517,
94.4563391202517,
94.3345111822815,
94.3345111822815,
94.3345111822815,
95.1577637400123,
95.1577637400123,
95.1577637400123,
96.4727940924280,
96.4727940924280,
96.4727940924280,
97.2882315596290,
97.2882315596290,
97.2882315596290,
98.5444062339411,
98.5444062339411,
98.5444062339411,
98.2344135219425,
98.2344135219425,
98.2344135219425,
98.6813052203912,
98.6813052203912,
98.6813052203912,
102.288806816777,
102.288806816777,
102.288806816777,
105.560409245133,
105.560409245133,
105.560409245133,
109.868102741424,
109.868102741424,
109.357212839392,
109.631199938704,
109.631199938704,
109.808924956034,
107.004672795163,
107.004672795163,
107.042047812997,
101.911726508778,
101.911726508778,
102.058806577385,
98.7572782127981,
98.7572782127981,
100.439036235918,
95.1787791474549,
95.1787791474549,
97.2728122344574,
96.7987603226405,
96.7987603226405,
99.5740930162058,
101.916632597432,
101.916632597432,
101.350875674559,
102.200782775867,
102.200782775867,
99.2370898404422,
96.3742704252541,
96.3742704252541,
95.7653381970742,
95.7653381970742,
92.8008620649615,
91.9836942071800,
91.9836942071800,
88.5268320906153,
90.8074886779719,
90.8074886779719,
92.0597631976098,
92.0597631976098,
92.0597631976098,
94.4033897696476,
94.4033897696476,
94.4033897696476,
94.2761899951414,
94.2761899951414,
94.2761899951414,
90.8515272298710,
90.8515272298710,
90.8515272298710,
89.3196506934504,
89.3196506934504,
89.3196506934504,
88.2043082847998,
88.2043082847998,
88.2043082847998,
88.7637313321156,
88.7637313321156,
88.7637313321156,
90.1110426085505,
90.1110426085505,
90.1110426085505,
88.7637313321156,
88.7637313321156,
88.7637313321156,
86.8043777697876,
86.8043777697876,
86.8043777697876,
85.1234397801217,
85.1234397801217,
85.1234397801217,
84.6049643933499,
84.6049643933499,
84.6049643933499,
88.1532756056177,
88.1532756056177,
87.6983466206747,
89.4986033410578,
89.4986033410578,
88.3459110542191,
88.1022133660670,
88.1022133660670,
87.2926113711808,
84.6167832052247,
84.6167832052247,
83.9940474081348,
79.7182538694871,
79.7182538694871,
80.9567785920364,
78.2496006379585,
78.2496006379585,
78.7654746700609,
79.2653770570733,
79.2653770570733,
79.0253124005214,
79.6931615635871,
79.6931615635871,
79.0379655608620,
78.0768851837726,
78.0768851837726,
77.6466354712167,
77.6466354712167,
76.2495901628330,
78.1280999384984,
78.1280999384984,
76.2692598626734,
79.4103267843673,
79.4103267843673,
77.4919350642375,
78.5811682275086,
78.5811682275086,
78.4028060722319,
78.4028060722319,
78.4028060722319,
74.7729897757205,
74.7729897757205,
74.7729897757205,
73.7088868454815,
73.7088868454815,
73.7088868454815,
74.2024258363566,
74.2024258363566,
74.2024258363566,
76.9740215917033,
76.9740215917033,
76.9740215917033,
80.8455317256310,
80.8455317256310,
80.8455317256310,
80.8455317256310,
80.8455317256310,
80.8455317256310,
80.8207894047070,
80.8207894047070,
80.8207894047070,
80.5853584716231,
80.5853584716231,
80.5853584716231,
80.4487414444751,
80.4487414444751,
80.4487414444751,
83.5583628370015,
83.5583628370015,
83.5583628370015,
84.2318229649578,
84.2318229649578,
81.4248119432891,
83.5942581760255,
83.5942581760255,
80.2807573457052,
82.2921624457639,
82.2921624457639,
80.6721761203948,
81.9084855189009,
81.9084855189009,
81.0678727980450,
81.6455755078008,
81.6455755078008,
78.9810103759125,
77.8203058333749,
77.8203058333749,
74.2226380560540,
71.8540186767588,
71.8540186767588
	
};

real_T roller_resultantMat_data[205]= { 0.00 };
int32_T roller_resultantMat_size[2] = { 205, 1 };

int main(void)
{	 
	
	
	/* USART1 config */	
	USART1_Config();
	printf( "\r\n Test1 is Starting...... \r\n" );
	//printf( "Testing print =%f",test_obtData[1]);
	
	/*matlab function test*/
	sprintf((char *)print_buffer, "Roller Resultant Mat = %f",roller_resultant[1]);
	printf("%s\r\n",print_buffer);
	
	adjustRollerData_initialize();
	adjustRollerData(roller_resultant,238,roller_resultantMat_data,roller_resultantMat_size);
		
	
while (1)
	{
		
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
