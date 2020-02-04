#include "delay.h"
void delay_msec(uint32_t delay){
    uint32_t k;
    volatile uint32_t internal_fake;
    while(delay-- > 0){
        for (k=0;k<21000; k++)
        {
            internal_fake--;
        }
    }
}




void delay_usec(uint32_t delay){
    uint32_t k;
    volatile uint32_t internal_fake;
    while(delay-- > 0){
        for (k=0;k<21; k++)
        {
            internal_fake--;
        }
    }
}