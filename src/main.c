
#include "stm32f10x.h"

void led_init(void);
void buttons_init(void);
void tim_init(void);

// ���������������� ������
#define A0_PRESS (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0))
#define A1_PRESS (!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
#define A2_PRESS (!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2))
#define A3_PRESS (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4))

// ���������������� ������� ������ ��
#define DOLEDON GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)
#define LEDON GPIO_SetBits(GPIOC, GPIO_Pin_13)
#define LEDOFF GPIO_ResetBits(GPIOC, GPIO_Pin_13)

//���������������� ������� ����� � ����������
#define TIME_LED_STANDARD 	(uint16_t)1000   // 1�
#define TIME_COMMUT_A1 	(uint16_t)2000      // 2�
#define TIME_COMMUT_A2 	(uint16_t)3000     //3�
#define TIME_COMMUT_A3 	(uint16_t)4000    //4�

 uint16_t *pcommutation;  //������ ����������
 uint16_t *ppause;    //������ �����

int main(void)
{
	 volatile uint16_t commutation = TIME_LED_STANDARD; //������ ����������,�������� ���������� ����� �������� �� ���
	 volatile uint16_t pause = TIME_LED_STANDARD; //������ �����,�������� ���������� ����� �������� �� ���
	 pcommutation = &commutation;
	 ppause = &pause;

	 uint8_t what_btn_prs = 0;                  //��������� ������� ������

	 led_init();
	 buttons_init();
	 tim_init();

	 for(;;) {
	 		if (A0_PRESS && what_btn_prs != 1) {
	 			TIM_SetCounter(TIM3, 0x00);
	 			//������������ �������� �������
	 			TIM_SetAutoreload(TIM3, 0xFFFF);
	 			//����������� ���������� ���������� �� ������������
	 			TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);
	 			commutation = TIME_LED_STANDARD;
	 			what_btn_prs = 1;
	 		}
	 		else if (A1_PRESS && what_btn_prs == 0) {
	 			commutation = TIME_COMMUT_A1;
	 			what_btn_prs = 2;
	 		}
	 		else if (A2_PRESS && what_btn_prs == 0) {
	 			commutation = TIME_COMMUT_A2;
	 			what_btn_prs = 3;
	 		}
	 		else if (A3_PRESS && what_btn_prs == 0) {
	 			commutation = TIME_COMMUT_A3;
	 			what_btn_prs = 4;
	 		}
	 		else if (!A0_PRESS && what_btn_prs == 1) {
	 			uint16_t now_pause = TIM_GetCounter(TIM3);
	 			if (now_pause > 300) {
	 				pause = now_pause;
	 			}
	 			//���������� ���������� ���������� �� ������������
	 			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	 			what_btn_prs = 0;
	 			//�������������� ��������� ������� (����� �������� �����, ���� ������
	 			//�� �������� �� ���� ��������, � ��� ������������ �������)
	 			TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
	 		}
	 		else if (!A1_PRESS && what_btn_prs == 2) {
	 			commutation = TIME_LED_STANDARD;
	 			what_btn_prs = 0;
	 		}
	 		else if (!A2_PRESS && what_btn_prs == 3) {
	 			commutation = TIME_LED_STANDARD;
	 			what_btn_prs = 0;
	 		}
	 		else if (!A3_PRESS && what_btn_prs == 4) {
	 			commutation = TIME_LED_STANDARD;
	 			what_btn_prs = 0;
	 		}
	 	}
	 }

	 void TIM3_IRQHandler(void){
	 	//����� ����� ������������ �������
	 	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	 	//�������� ������� ���������/�����
	 	if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)){
	 		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	 		TIM_SetAutoreload(TIM3, *ppause);
	 	}
	 	else {
	 		GPIO_SetBits(GPIOC, GPIO_Pin_13);
	 		TIM_SetAutoreload(TIM3, *pcommutation);
	 	}
	 }

void led_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	//Diod     C13
	GPIO_InitTypeDef PinC13;
        PinC13.GPIO_Pin = GPIO_Pin_13;
	    PinC13.GPIO_Speed  = GPIO_Speed_2MHz;
	    PinC13.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &PinC13);
}


void buttons_init (void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	// ������ 1   A0
	GPIO_InitTypeDef PinA0;
		PinA0.GPIO_Pin = GPIO_Pin_0;
		PinA0.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &PinA0);

	//������ 2    A1
	GPIO_InitTypeDef PinA1;
		PinA1.GPIO_Pin = GPIO_Pin_1;
		PinA1.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &PinA1);

    //������ 3  A2
	GPIO_InitTypeDef PinA2;
	    PinA2.GPIO_Pin = GPIO_Pin_2;
	    PinA2.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &PinA2);

    //������ 4   A3
	 GPIO_InitTypeDef PinA4;
	    PinA4.GPIO_Pin = GPIO_Pin_4;
	    PinA4.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &PinA4);
}
void tim_init(void)
{

	/*RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;//��������� ������������ �������

	TIM3->PSC=36000-1;// ����� �������� � �������� ������� 1000kHz

	TIM3->ARR=TIME_LED_STANDARD-1;// ������ ������ �������  1000 ������ => 1000/1000 = 1Hz (1�)

	TIM3->DIER|=TIM_DIER_UIE;//���������� ���������� �� ������������

	NVIC_EnableIRQ(TIM3_IRQn);  //��������� ����������

	TIM3->CR1 |= TIM_CR1_CEN;*/

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	// TIM 3
		TIM_TimeBaseInitTypeDef TIM3_CLOCK;
			TIM3_CLOCK.TIM_Prescaler = 36000 - 1;
			TIM3_CLOCK.TIM_Period = TIME_LED_STANDARD;
			TIM3_CLOCK.TIM_CounterMode = TIM_CounterMode_Up;
			TIM3_CLOCK.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseInit(TIM3, &TIM3_CLOCK);

		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //���������� ���������� ������� �� ������������

		NVIC_EnableIRQ(TIM3_IRQn);//��� ���������� ������� 3
		TIM_Cmd(TIM3, ENABLE);
}
