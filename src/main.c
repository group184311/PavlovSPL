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
			

int main(void)
{

//GPIO
RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;

GPIOC->CRH &= ~(GPIO_CRH_CNF15_1 | GPIO_CRH_CNF15_0 |
		GPIO_CRH_MODE15_1 | GPIO_CRH_MODE15_0 |
		GPIO_CRH_CNF13_1 |GPIO_CRH_CNF13_0 |
	  GPIO_CRH_MODE13_1 |GPIO_CRH_MODE13_0);

GPIOC->CRH|= GPIO_CRH_MODE13_1|GPIO_CRH_CNF15_1;

GPIOC->ODR |= GPIO_ODR_ODR15;


//TIM
RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;

TIM3->PSC=36000-1;

TIM3->ARR=1000-1;

TIM3->DIER|=TIM_DIER_UIE;

NVIC_EnableIRQ(TIM3_IRQn);



	for(;;){
		if (GPIOC->IDR &GPIO_IDR_IDR15)
				{
		TIM3->CR1 |= TIM_CR1_CEN;
				}
		else
		{
			TIM3->CR1 &= ~TIM_CR1_CEN;
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
