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

uint16_t delay=500 ;


int main(void)
{

	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

// ������ 1   A0

GPIOA->CRL &= ~(GPIO_CRL_CNF0_1 | GPIO_CRL_CNF0_0 |
				GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);


GPIOA->CRL |= GPIO_CRL_CNF0_1;  // ���� �� ����
GPIOA->ODR &= ~GPIO_ODR_ODR0;  // �������� � -

//������ 2    A1
GPIOA->CRL &= ~(GPIO_CRL_CNF1_1 | GPIO_CRL_CNF1_0 |
		      GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);


GPIOA->CRL |= GPIO_CRL_CNF1_1;  // ���� �� ����
GPIOA->ODR &= ~GPIO_ODR_ODR1;  // �������� � -

//������ 3  A2
GPIOA->CRL &= ~(GPIO_CRL_CNF2_1 | GPIO_CRL_CNF2_0 |
				GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0);


GPIOA->CRL |= GPIO_CRL_CNF2_1;  // ���� �� ����
GPIOA->ODR &= ~GPIO_ODR_ODR2;  // �������� � -


//������ 4   A3
GPIOA->CRL &= ~(GPIO_CRL_CNF3_1 | GPIO_CRL_CNF3_0 |
				GPIO_CRL_MODE3_1 | GPIO_CRL_MODE3_0);


GPIOA->CRL |= GPIO_CRL_CNF3_1;  // ���� �� ����
GPIOA->ODR &= ~GPIO_ODR_ODR3;  // �������� � -



//Diod     C13
RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;

GPIOC->CRH &= ~(GPIO_CRH_CNF13_1 |GPIO_CRH_CNF13_0 |
	  GPIO_CRH_MODE13_1 |GPIO_CRH_MODE13_0);

GPIOC->CRH|= GPIO_CRH_MODE13_1;



// TIM 3
RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;

TIM3->PSC=36000-1;

TIM3->ARR=delay-1;

TIM3->DIER|=TIM_DIER_UIE;

NVIC_EnableIRQ(TIM3_IRQn);  //��������� ����������


	for(;;){
		if (GPIOA->IDR &GPIO_IDR_IDR0) //�������� ���������
		{
		TIM3->CR1 |= TIM_CR1_CEN;
		}
		else
		{
		TIM3->CR1 &= ~TIM_CR1_CEN;
		}
		else if (GPIOA->IDR &GPIO_IDR_IDR1)
		{
        delay= 1000;
		}


	};

}

void TIM3_IRQHandler(void)
{
TIM3->SR &= ~TIM_SR_UIF;

if (GPIOC->ODR & GPIO_ODR_ODR13)

	GPIOC->BSRR = GPIO_BSRR_BR13;
	else

	GPIOC->BSRR = GPIO_BSRR_BS13;

}
