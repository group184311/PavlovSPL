#include "stm32f10x.h"

#define B5_PRESS ( GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_5))
#define C15_PRESS (!GPIO_ReadInputDataBit( GPIOC, GPIO_Pin_15))
#define B0_PRESS ( !GPIO_ReadInputDataBit( GPIOB, GPIO_Pin_0))
#define A11_PRESS ( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_11))

#define TIME_LED_DEF   (uint16_t)500
#define TIME_C15       (uint16_t)1000
#define TIME_B0        (uint16_t)1500
#define TIME_A11       (uint16_t)2000

volatile uint16_t pause_ON = TIME_LED_DEF;
volatile uint16_t pause_OFF = TIME_LED_DEF;

int main(void)
{

    uint8_t what_btn_prs = 0;

	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

		GPIO_InitTypeDef gpio_init; //экземпляр структуры для конфигурирования ГПИО

		// Конфигурирование периферии А
		gpio_init.GPIO_Pin = GPIO_Pin_11;
		gpio_init.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(GPIOA, &gpio_init);

		// Конфигурирование периферии Б
		gpio_init.GPIO_Pin = GPIO_Pin_0;
		gpio_init.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOB, &gpio_init);

		gpio_init.GPIO_Pin = GPIO_Pin_5;
		gpio_init.GPIO_Mode = GPIO_Mode_IPD;
		GPIO_Init(GPIOB, &gpio_init);

		// Конфигурирование периферии Ц
		gpio_init.GPIO_Pin = GPIO_Pin_13;
		gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
		gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(GPIOC, &gpio_init);

		gpio_init.GPIO_Pin = GPIO_Pin_15;
		gpio_init.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIOC, &gpio_init);

		//Тактирование таймера ТИМ3
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


		TIM_TimeBaseInitTypeDef tim_init; //экземпляр структуры для конфигурирования ТИМ3
		TIM_TimeBaseStructInit(&tim_init); //заполнение стуктуры дефолтом
		tim_init.TIM_Prescaler = 36000 - 1 ; //Частота работы таймера 1 KГц
		tim_init.TIM_Period = TIME_LED_DEF - 1; //Период таймера
		tim_init.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &tim_init);

		//Разрешение прерывания таймера по переполнению
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

		//Вкл прерывания таймера 3
		NVIC_EnableIRQ(TIM3_IRQn);
		TIM_Cmd(TIM3, ENABLE);

	for(;;)
	{

		if (B5_PRESS && what_btn_prs != 1)
		   {
			//Обнуление счетчика
			TIM_SetCounter (TIM3, 0x00);
			//максимальное значение периода
			TIM_SetAutoreload(TIM3, 0xFFFF);
		    //отключается разрешение прерывания по переполнению
			TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);

			pause_ON = TIME_LED_DEF;
			what_btn_prs = 1;
		   }
			else if (C15_PRESS && what_btn_prs == 0)
						{
						what_btn_prs = 2;
						pause_ON = TIME_C15;
						}

					else if (B0_PRESS && what_btn_prs == 0)
					    {
						what_btn_prs = 3;
						pause_ON = TIME_B0 ;
						}

				    else if (A11_PRESS && what_btn_prs == 0)
						{
						what_btn_prs = 4;
						pause_ON = TIME_A11;
						}

				 else if (!B5_PRESS && what_btn_prs == 1)
				     {
					 uint16_t now_pause = TIM_GetCounter(TIM3);
					    if (now_pause > 300)
							     {
							      pause_OFF = now_pause;
							     }
					 //включается разрешение прерывания по переполнению
					 TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
					 what_btn_prs = 0;
					 //принудительная генерация события (иначе придется ждать, пока таймер
					 //не дотикает до макс значения, а это противоречит заданию)
					 TIM_GenerateEvent(TIM3, TIM_EventSource_Update);
					 }
				 else if (!C15_PRESS && what_btn_prs == 2)
				 					 {
				 					 what_btn_prs = 0;
				 					 pause_ON = TIME_LED_DEF;
				 					 }
				 			    else if (!B0_PRESS && what_btn_prs == 3)
				 					 {
				 					 what_btn_prs = 0;
				 					 pause_ON = TIME_LED_DEF;
				 					 }
				 				else if (!A11_PRESS && what_btn_prs == 4)
				 					 {
				 					 what_btn_prs = 0;
				 					 pause_ON = TIME_LED_DEF;
				 					 }

			}

}
void TIM3_IRQHandler(void)
{

   TIM_ClearFlag(TIM3,TIM_FLAG_Update);

   if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
     {
      GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	  TIM_SetAutoreload(TIM3, pause_ON);
      }
    else
    {
      GPIO_SetBits(GPIOC, GPIO_Pin_13);
      TIM_SetAutoreload(TIM3, pause_OFF);
     }
}


