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

// макроопределения кнопок
#define А0_PRESS (GPIOА->IDR & GPIO_IDR_IDR0)
#define А1_PRESS (!(GPIOА->IDR & GPIO_IDR_IDR1))
#define А2_PRESS (!(GPIOА->IDR & GPIO_IDR_IDR2))
#define А3_PRESS (GPIOA->IDR & GPIO_IDR_IDR3)

// макроопределения режимов работы СД
#define DO_LED_ON GPIOC->IDR & GPIO_IDR_IDR13
#define LED_ON GPIOC->BSRR = GPIO_BSRR_BR13
#define LED_OFF GPIOC->BSRR = GPIO_BSRR_BS13

//макроопределения времени паузы и коммутации
#define TIME_LED_STANDARD 	(uint16_t)1000
#define TIME_COMMUT_А0 	(uint16_t)2000
#define TIME_COMMUT_А2 	(uint16_t)3000
#define TIME_COMMUT_А3 	(uint16_t)4000

	volatile uint16_t high  = TIME_LED_STANDARD; //тактов коммутации
	volatile uint16_t low = TIME_LED_STANDARD; //тактов паузы
	uint8_t what_btn_prs = 0;



int main(void)
{


RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//Тактирование порта А

// Кнопка 1   A0
//Сброс сотояния  пина
GPIOA->CRL &= ~(GPIO_CRL_CNF0_1 | GPIO_CRL_CNF0_0 |
				GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);

//Настройка пина
GPIOA->CRL |= GPIO_CRL_CNF0_1;  // порт на вход
GPIOA->ODR &= ~GPIO_ODR_ODR0;  // подтяжка к -

//Кнопка 2    A1
//Сброс сотояния  пина
GPIOA->CRL &= ~(GPIO_CRL_CNF1_1 | GPIO_CRL_CNF1_0 |
		      GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);

//Настройка пина
GPIOA->CRL |= GPIO_CRL_CNF1_1;  // порт на вход
GPIOA->ODR &= ~GPIO_ODR_ODR1;  // подтяжка к -

//Кнопка 3  A2
//Сброс сотояния  пина
GPIOA->CRL &= ~(GPIO_CRL_CNF2_1 | GPIO_CRL_CNF2_0 |
				GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0);

//Настройка пина
GPIOA->CRL |= GPIO_CRL_CNF2_1;  // порт на вход
GPIOA->ODR &= ~GPIO_ODR_ODR2;  // подтяжка к -


//Кнопка 4   A3
//Сброс сотояния  пина
GPIOA->CRL &= ~(GPIO_CRL_CNF3_1 | GPIO_CRL_CNF3_0 |
				GPIO_CRL_MODE3_1 | GPIO_CRL_MODE3_0);

//Настройка пина
GPIOA->CRL |= GPIO_CRL_CNF3_1;  // порт на вход
GPIOA->ODR &= ~GPIO_ODR_ODR3;  // подтяжка к -



//Diod     C13
RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;
//Сброс сотояния  пина
GPIOC->CRH &= ~(GPIO_CRH_CNF13_1 |GPIO_CRH_CNF13_0 |
	  GPIO_CRH_MODE13_1 |GPIO_CRH_MODE13_0);
//Настройка пина
GPIOC->CRH|= GPIO_CRH_MODE13_1;



// TIM 3
RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;//Включение тактирования таймера

TIM3->PSC=36000-1;// Зпуск таймеран а тактовой частоте 1000kHz

TIM3->ARR=TIME_LED_STANDARD-1;// Период работы таймера  1000 тактов => 1000/1000 = 1Hz (1с)

TIM3->DIER|=TIM_DIER_UIE;//Разрешение прерывания по переполнению

NVIC_EnableIRQ(TIM3_IRQn);  //Включение прерывания

TIM3->CR1 |= TIM_CR1_CEN;

	for(;;)
	{
		if (GPIOA->IDR &GPIO_IDR_IDR0) //Если кнопка  нажата
		{


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
