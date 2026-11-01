
#ifndef __USART_H
#define __USART_H



#include"main.h"
#include "stm32f1xx_hal.h"



typedef enum {
    sinwave=1,
    sawtoothwave,
    squarewave
}current_wave;


void receive_strat(UART_HandleTypeDef *huart);
void send_wave(uint8_t now_status,float tik_time);
void change_show_led(uint8_t now_status);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);



#endif