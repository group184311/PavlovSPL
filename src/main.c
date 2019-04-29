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

// макроопределения кнопок (ПРОВЕРКА СОСТОЯНИЯ)
#define A0_PRESS (GPIOA->IDR & GPIO_IDR_IDR0)
#define A1_PRESS (GPIOA->IDR & GPIO_IDR_IDR1)
#define A2_PRESS (GPIOA->IDR & GPIO_IDR_IDR2)
#define A3_PRESS (GPIOA->IDR & GPIO_IDR_IDR3)

// макроопределения режимов работы Светодиода
#define DO_LED_ON GPIOC->IDR & GPIO_IDR_IDR13   //Проверка состояния диода
#define LED_ON GPIOC->BSRR = GPIO_BSRR_BR13    //1 на выход
#define LED_OFF GPIOC->BSRR = GPIO_BSRR_BS13  //0 на выход

//макроопределения времени паузы и коммутации
#define TIME_LED_STANDARD 	(uint16_t)1000   // 1с
#define TIME_COMMUT_A1 	(uint16_t)2000      // 2с
#define TIME_COMMUT_A2 	(uint16_t)3000     //3с
#define TIME_COMMUT_A3 	(uint16_t)4000    //4с

	volatile uint16_t high  = TIME_LED_STANDARD; //тактов коммутации
	volatile uint16_t low = TIME_LED_STANDARD;  //тактов паузы
	uint8_t what_btn_prs = 0;  //Указатель нажатой кнопки


int main(void)
{


//RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;//Тактирование порта А
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
// Кнопка 1   A0
//Сброс сотояния  пина
//GPIOA->CRL &= ~(GPIO_CRL_CNF0_1 | GPIO_CRL_CNF0_0 |
			//GPIO_CRL_MODE0_1 | GPIO_CRL_MODE0_0);

	//Настройка пина
	//GPIOA->CRL |= GPIO_CRL_CNF0_1;  // порт на вход
	//GPIOA->ODR &= ~GPIO_ODR_ODR0;  // подтяжка к -

	GPIO_InitTypeDef PinA0;
		PinA0.GPIO_Pin = GPIO_Pin_0;
		PinA0.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(GPIOA, &PinA0);


//Кнопка 2    A1
//Сброс сотояния  пина
/*GPIOA->CRL &= ~(GPIO_CRL_CNF1_1 | GPIO_CRL_CNF1_0 |
		      GPIO_CRL_MODE1_1 | GPIO_CRL_MODE1_0);

//Настройка пина
GPIOA->CRL |= GPIO_CRL_CNF1_1;  // порт на вход
GPIOA->ODR &= ~GPIO_ODR_ODR1;  // подтяжка к -
*/
		GPIO_InitTypeDef PinA1;
				PinA1.GPIO_Pin = GPIO_Pin_1;
				PinA1.GPIO_Mode = GPIO_Mode_IPD;
				GPIO_Init(GPIOA, &PinA1);



//Кнопка 3  A2
//Сброс сотояния  пина
/*GPIOA->CRL &= ~(GPIO_CRL_CNF2_1 | GPIO_CRL_CNF2_0 |
				GPIO_CRL_MODE2_1 | GPIO_CRL_MODE2_0);

//Настройка пина
GPIOA->CRL |= GPIO_CRL_CNF2_1;  // порт на вход
GPIOA->ODR &= ~GPIO_ODR_ODR2;  // подтяжка к -*/

	GPIO_InitTypeDef PinA2;
			PinA2.GPIO_Pin = GPIO_Pin_2;
			PinA2.GPIO_Mode = GPIO_Mode_IPD;
			GPIO_Init(GPIOA, &PinA2);


//Кнопка 4   A3
//Сброс сотояния  пина
/*GPIOA->CRL &= ~(GPIO_CRL_CNF3_1 | GPIO_CRL_CNF3_0 |
				GPIO_CRL_MODE3_1 | GPIO_CRL_MODE3_0);

//Настройка пина
GPIOA->CRL |= GPIO_CRL_CNF3_1;  // порт на вход
GPIOA->ODR &= ~GPIO_ODR_ODR3;  // подтяжка к -*/

		GPIO_InitTypeDef PinA4;
				PinA4.GPIO_Pin = GPIO_Pin_4;
				PinA4.GPIO_Mode = GPIO_Mode_IPD;
				GPIO_Init(GPIOA, &PinA4);



//Diod     C13
//RCC->APB2ENR |=RCC_APB2ENR_IOPCEN;
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//Сброс сотояния  пина
/*GPIOC->CRH &= ~(GPIO_CRH_CNF13_1 |GPIO_CRH_CNF13_0 |
	  GPIO_CRH_MODE13_1 |GPIO_CRH_MODE13_0);
//Настройка пина
      GPIOC->CRH|= GPIO_CRH_MODE13_1;*/

         GPIO_InitTypeDef PinC13;
			PinC13.GPIO_Pin = GPIO_Pin_13;
			PinC13.GPIO_Speed  = GPIO_Speed_2MHz;
		    PinC13.GPIO_Mode = GPIO_Mode_Out_PP;
			GPIO_Init(GPIOC, &PinC13);


// TIM 3
/*RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;//Включение тактирования таймера

TIM3->PSC=36000-1;// Зпуск таймеран а тактовой частоте 1000kHz

TIM3->ARR=TIME_LED_STANDARD-1;// Период работы таймера  1000 тактов => 1000/1000 = 1Hz (1с)

TIM3->DIER|=TIM_DIER_UIE;//Разрешение прерывания по переполнению

NVIC_EnableIRQ(TIM3_IRQn);  //Включение прерывания

TIM3->CR1 |= TIM_CR1_CEN;*/

RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

		TIM_TimeBaseInitTypeDef TIM3_CLOCK;
			TIM3_CLOCK.TIM_Prescaler = 36000 - 1;
			TIM3_CLOCK.TIM_Period = TIME_LED_STANDARD;
			TIM3_CLOCK.TIM_CounterMode = TIM_CounterMode_Up;
			TIM3_CLOCK.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseInit(TIM3, &TIM3_CLOCK);

			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			NVIC_EnableIRQ(TIM3_IRQn);
			TIM_Cmd(TIM3, ENABLE);



	for(;;)
	{
		if (A0_PRESS && what_btn_prs != 1)
		{
			        //Обнуление счетчика
					TIM3->CNT = 0x0;
					//максимальное значение периода
					TIM3->ARR = 0xFFFF;
					//отключается разрешение прерывания по переполнению
					TIM3->DIER &= ~TIM_DIER_UIE;
                    //Комутация 1с
					high = TIME_LED_STANDARD;
					what_btn_prs = 1;
		}

	      else if (A1_PRESS && what_btn_prs == 0)
				{
					what_btn_prs = 2;
					//Комутация 2с
					high = TIME_COMMUT_A1;
				}

	      else if (A2_PRESS && what_btn_prs == 0)
	      {
					what_btn_prs = 3;
					//Комутация 3с
					high = TIME_COMMUT_A2;
				}

				else if (A3_PRESS && what_btn_prs == 0)
				{
					what_btn_prs = 4;
					//Комутация 4с
					high = TIME_COMMUT_A3;
				}

				else if (!A0_PRESS && what_btn_prs == 1)
				{
					uint16_t now_pause = TIM3->CNT;
					if (now_pause > 300)
					{
						low = now_pause;//фильтр на дребезг и присвоение значение счетчика в паузу
					}
					//включается разрешение прерывания по переполнению
					TIM3->DIER |= TIM_DIER_UIE;
					what_btn_prs = 0;
					//принудительная генерация события (иначе придется ждать, пока таймер
					//не дотикает до макс значения, а это противоречит заданию)
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
			//Сброс флага переполнения таймера
			TIM3->SR &= ~TIM_SR_UIF;
			//Введение понятий комутация/пауза
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
