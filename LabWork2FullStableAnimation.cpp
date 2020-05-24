#include "hal.h"

int i = 0;
int pattern_sw;
//int delay;
int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
                  GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9,
                  GPIO_PIN_11, GPIO_PIN_12};
int sw_num[] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};

void TIM6_IRQ_Haldler(){
    if(i != 7) {
        HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, leds_num[i + 1], GPIO_PIN_SET);
    }
    else{
        HAL_GPIO_WritePin(GPIOD, leds_num[7], GPIO_PIN_SET);
        HAL_GPIO_WritePin(GPIOD, leds_num[0], GPIO_PIN_SET);
    }
}


void TIM7_IRQ_Haldler(){
    if(i != 7) {
        HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, leds_num[i + 1], GPIO_PIN_RESET);
    }
    else{
        HAL_GPIO_WritePin(GPIOD, leds_num[7], GPIO_PIN_RESET);
        HAL_GPIO_WritePin(GPIOD, leds_num[0], GPIO_PIN_RESET);
    }
    i++;
}


int umain(){
    i = 0;


    //tim6 - set/reset
    //tim7 - switchers interruptions
    //???
    registerTIM6_IRQHandler(TIM6_IRQ_Haldler);
    registerTIM7_IRQHandler(TIM7_IRQ_Haldler);

    __enable_irq();

    while(1){

        pattern_sw = 0;
        for (int k = 0; k < 4; k++) {
            if (HAL_GPIO_ReadPin(GPIOE, sw_num[k]) == GPIO_PIN_SET) {
                pattern_sw |= 1 << k;
            }
        }


        WRITE_REG(TIM6_ARR, 500 + pattern_sw * 150);
        WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
        WRITE_REG(TIM6_PSC, 0);

        WRITE_REG(TIM7_ARR, 500);
        WRITE_REG(TIM7_DIER, TIM_DIER_UIE);
        WRITE_REG(TIM7_PSC, 1);

        WRITE_REG(TIM6_CR1, TIM_CR1_CEN);
        WRITE_REG(TIM7_CR1, TIM_CR1_CEN);



        HAL_Delay(10);
        if (i == 8){
            i = 0;
        }

    }

    return 0;
}
