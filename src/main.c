/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"

// ���������������� ������ (�������� ���������)
#define A0_PRESS (GPIOA->IDR & GPIO_IDR_IDR0)
#define A1_PRESS (GPIOA->IDR & GPIO_IDR_IDR1)
#define A2_PRESS (GPIOA->IDR & GPIO_IDR_IDR2)
#define A3_PRESS (GPIOA->IDR & GPIO_IDR_IDR3)

// ���������������� ������� ������ ����������
#define DO_LED_ON GPIOC->IDR & GPIO_IDR_IDR13   //�������� ��������� �����
#define LED_ON GPIOC->BSRR = GPIO_BSRR_BR13    //1 �� �����
#define LED_OFF GPIOC->BSRR = GPIO_BSRR_BS13  //0 �� �����

//���������������� ������� ����� � ����������
#define TIME_LED_STANDARD 	(uint16_t)1000   // 1�
#define TIME_COMMUT_A1 	(uint16_t)2000      // 2�
#define TIME_COMMUT_A2 	(uint16_t)3000     //3�
#define TIME_COMMUT_A3 	(uint16_t)4000    //4�

	volatile uint16_t high  = TIME_LED_STANDARD; //������ ����������
	volatile uint16_t low = TIME_LED_STANDARD;  //������ �����
	uint8_t what_btn_prs = 0;  //��������� ������� ������


int main(void)
{


RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//������������ ����� �

// ������ 1   A0
//����� ��������  ����
GPIOA->CRL &= ~(GPIO_CRL_CNF0_1 | GPIO_CRL_CNF0_0 |
				GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);

//��������� ����
GPIOA->CRL |= GPIO_CRL_CNF0_1;  // ���� �� ����
GPIOA->ODR &= ~GPIO_ODR_ODR0;  // �������� � -

//������ 2    A1
//����� ��������  ����
GPIOA->CRL &= ~(GPIO_CRL_CNF1_1 | GPIO_CRL_CNF1_0 |
		      GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);

//��������� ����
GPIOA->CRL |= GPIO_CRL_CNF1_1;  // ���� �� ����
GPIOA->ODR &= ~GPIO_ODR_ODR1;  // �������� � -

//������ 3  A2
//����� ��������  ����
GPIOA->CRL &= ~(GPIO_CRL_CNF2_1 | GPIO_CRL_CNF2_0 |
				GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0);

//��������� ����
GPIOA->CRL |= GPIO_CRL_CNF2_1;  // ���� �� ����
GPIOA->ODR &= ~GPIO_ODR_ODR2;  // �������� � -


//������ 4   A3
//����� ��������  ����
GPIOA->CRL &= ~(GPIO_CRL_CNF3_1 | GPIO_CRL_CNF3_0 |
				GPIO_CRL_MODE3_1 | GPIO_CRL_MODE3_0);

//��������� ����
GPIOA->CRL |= GPIO_CRL_CNF3_1;  // ���� �� ����
GPIOA->ODR &= ~GPIO_ODR_ODR3;  // �������� � -


//Diod     C13
RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;
//����� ��������  ����
GPIOC->CRH &= ~(GPIO_CRH_CNF13_1 |GPIO_CRH_CNF13_0 |
	  GPIO_CRH_MODE13_1 |GPIO_CRH_MODE13_0);
//��������� ����
GPIOC->CRH|= GPIO_CRH_MODE13_1;


// TIM 3
RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;//��������� ������������ �������

TIM3->PSC=36000-1;// ����� �������� � �������� ������� 1000kHz

TIM3->ARR=TIME_LED_STANDARD-1;// ������ ������ �������  1000 ������ => 1000/1000 = 1Hz (1�)

TIM3->DIER|=TIM_DIER_UIE;//���������� ���������� �� ������������

NVIC_EnableIRQ(TIM3_IRQn);  //��������� ����������

TIM3->CR1 |= TIM_CR1_CEN;

	for(;;)
	{
		if (A0_PRESS && what_btn_prs != 1)
		{
			        //��������� ��������
					TIM3->CNT = 0x0;
					//������������ �������� �������
					TIM3->ARR = 0xFFFF;
					//����������� ���������� ���������� �� ������������
					TIM3->DIER &= ~TIM_DIER_UIE;
                    //��������� 1�
					high = TIME_LED_STANDARD;
					what_btn_prs = 1;
		}

	      else if (A1_PRESS && what_btn_prs == 0)
				{
					what_btn_prs = 2;
					//��������� 2�
					high = TIME_COMMUT_A1;
				}

	      else if (A2_PRESS && what_btn_prs == 0)
	      {
					what_btn_prs = 3;
					//��������� 3�
					high = TIME_COMMUT_A2;
				}

				else if (A3_PRESS && what_btn_prs == 0)
				{
					what_btn_prs = 4;
					//��������� 4�
					high = TIME_COMMUT_A3;
				}

				else if (!A0_PRESS && what_btn_prs == 1)
				{
					uint16_t now_pause = TIM3->CNT;
					if (now_pause > 300)
					{
						low = now_pause;//������ �� ������� � ���������� �������� �������� � �����
					}
					//���������� ���������� ���������� �� ������������
					TIM3->DIER |= TIM_DIER_UIE;
					what_btn_prs = 0;
					//�������������� ��������� ������� (����� �������� �����, ���� ������
					//�� �������� �� ���� ��������, � ��� ������������ �������)
					TIM3->EGR = 0x0001;
				}
				else if (!A1_PRESS && what_btn_prs == 2)
				{
					what_btn_prs = 0;
					high = TIME_LED_STANDARD;
				}
				else if (!A2_PRESS && what_btn_prs == 3)
				{
					what_btn_prs = 0;
					high = TIME_LED_STANDARD;
				}
				else if (!A3_PRESS && what_btn_prs == 4)
				{
					what_btn_prs = 0;
					high = TIME_LED_STANDARD;
				}
			}
		}

		void TIM3_IRQHandler(void){
			//����� ����� ������������ �������
			TIM3->SR &= ~TIM_SR_UIF;
			//�������� ������� ���������/�����
			if (DO_LED_ON)
			{
				LED_ON;
				TIM3->ARR = high;
			}
			else {
				LED_OFF;
				TIM3->ARR = low;
			}
		}
