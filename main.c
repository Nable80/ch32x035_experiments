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

int try_getch(char *buf)
{
    if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET) {
        return 0;
    }
    *buf = (char)USART_ReceiveData(USART2);
    return 1;
}

void board_init(void)
{
    // Enable clocking for GPIOA and USART2 peripherals:
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // Configure PA0 and PA1 as general-purpose push-pull output:
    GPIO_InitTypeDef gpio_info;
    gpio_info.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gpio_info.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio_info.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio_info);

    // Configure PA2 as USART2 TX...
    gpio_info.GPIO_Pin = GPIO_Pin_2;
    gpio_info.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_info.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_info);
    // ...and PA3 as input (USART2 RX):
    gpio_info.GPIO_Pin = GPIO_Pin_3;
    gpio_info.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio_info);

    // Actually configure and enable USART2:
    USART_InitTypeDef usart_info = {
        .USART_BaudRate = 115200,
        .USART_WordLength = USART_WordLength_8b,
        .USART_StopBits = USART_StopBits_1,
        .USART_Parity = USART_Parity_No,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
        .USART_Mode = USART_Mode_Tx | USART_Mode_Rx
    };
    USART_Init(USART2, &usart_info);
    USART_Cmd(USART2, ENABLE);
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
    char c;

    SystemCoreClockUpdate();
    Delay_Init();
    board_init();

    while (1) {
        // Jump to USB bootloader when user presses 'q'
        if (try_getch(&c) && c == 'q') {
            SystemReset_StartMode(Start_Mode_BOOT);
            NVIC_SystemReset();
        }

        print("Hello\n");
        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (i == 0));
        GPIO_WriteBit(GPIOA, GPIO_Pin_1, (i == 1));
        i ^= 1;
        Delay_Ms(500);
    }
}
