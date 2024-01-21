/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/04/06
 * Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 *GPIO routine:
 *PA0 push-pull output.
 *
 ***Only PA0--PA15 and PC16--PC17 support input pull-down.
 */

#include "debug.h"

/* Global define */

/* Global Variable */
void print(const char *buf)
{
    while (*buf) {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {}
        USART_SendData(USART2, *buf++);
    }
}

/*********************************************************************
 * @fn      Pins_Init
 *
 * @brief   Initializes GPIOA
 *
 * @return  none
 */
void Pins_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    u8 i = 0;

    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    Pins_Init();

    while (1) {
        print("Hello\n");
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (i == 0));
        GPIO_WriteBit(GPIOA, GPIO_Pin_1, (i == 1));
        i ^= 1;
        Delay_Ms(500);
    }
}
