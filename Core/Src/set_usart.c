//incldues
#include "main.h"
#include <stdio.h>
#include"set_usart.h"
#include "math.h"
#include"OLED.h"
#include "string.h"

extern volatile uint8_t ischange;
extern uint8_t status;
static UART_HandleTypeDef *s_huart = NULL; 
static uint8_t receive_data=0;


const float periods[3] = {1.0f, 0.5f, 1.5f};
const float maxs[3] = {2.0f, 1.0f, 0.5f};
const char *wave_names[3] = {"Sine", "Sawtooth", "Square"};


//启动函数

void receive_strat(UART_HandleTypeDef *huart)
{
    s_huart=huart;
    HAL_UART_Receive_IT(s_huart,&receive_data,1);
}

//发送波形
void send_wave(uint8_t now_status,float tik_time)//第一个参数是收到的123，第二个是运行的时间
{
    float period=periods[now_status-1];
    float max=maxs[now_status-1];
    float x=fmod(tik_time,period);
    float send_val=0.0f;

    switch (now_status)
    {
    case 1:
    send_val= max*sinf(2.0f*M_PI*x/period);//神秘正弦波
    break;
    
    case 2:
    send_val= -max+2.0f*max*x/period;
    break;

    case 3:
    send_val= (x<0.5f*period)?max:-max;
    break;
   
    default:
    send_val= 0.0f;
    break;
    }


    uint8_t to_vofa[8];
    memcpy(to_vofa,&send_val,4);


    to_vofa[4]=0x00;
    to_vofa[5]=0x00;
    to_vofa[6]=0x80;
    to_vofa[7]=0x7f;


    HAL_UART_Transmit(s_huart,to_vofa,8,10);

}



//切换led显示
void change_show_led(uint8_t now_status)
{
    OLED_operate_gram(PEN_CLEAR);
    OLED_show_string(0, 0, (uint8_t*)"wave:");
    OLED_show_string(2, 0, (uint8_t*)wave_names[now_status-1]);
    OLED_refresh_gram();
}


//回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance==USART1)
    {
      if(receive_data=='1'||receive_data=='2'||receive_data=='3')
    {
        status=receive_data-'0';
        ischange=1;
    }
    }

    HAL_UART_Receive_IT(s_huart, &receive_data, 1);
}

