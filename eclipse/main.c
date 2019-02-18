/**
 * @file    main.c
 * @brief   Hello World!
 * @details This example uses the UART to print to a terminal and flashes an LED.
 */

/*******************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sub-license,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, mask work rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *
 * $Date: 2018-08-09 23:45:02 +0000 (Thursday, 09 August 2018) $
 * $Revision: 36818 $
 *
 ******************************************************************************/

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

//delay libraries
#include "mxc_config.h"
#include "mxc_delay.h"

#include "tmr.h"
#include "tmr_utils.h"

//led library
#include "led.h"

//required libraries
#include "board.h"  //this NEEDS to be before gpio.h apparently
#include "gpio.h"


/***** Definitions *****/
//reminder that all pins listed here are SOFTWARE, not HARDWARE
#define PB1_PIN     PIN_9
#define PB2_PIN     PIN_8
#define PB3_PIN     PIN_3
#define PB4_PIN     PIN_2
#define THRESH      10      //ms
#define TMR0        MXC_TMR0

/***** Globals *****/

/***** Functions *****/

bool debounce(bool *prevSt, int *tStart, bool currSt) {
    if(currSt != *prevSt) {     //change has occured
        TMR_Enable(TMR0);       //enables timer on change
        *tStart = TMR_GetCount(TMR0);
        *prevSt = currSt;
    } else {
        if((TMR_GetCount(TMR0) - *tStart) > THRESH) {
            *prevSt = currSt;
            currSt = !currSt;
            TMR_Disable(TMR0);  //disables timer after settling
        }
    }

    return currSt;
}

void debounce(bool press) {
    if(press == true) {
        on++;   //count time up
        off = 0;
        if(on > THRESH) {
            if(output == false) {
                //action
                output = true;
            }
        }
    } else {
        off++;
        on = 0;
        if(off > THRESH) {
            output = 0;
            off = 0;
        }
    }
}

//inits atbtlc device
void btInit() {
    //TODO

}

//tells atbtlc to send bytes
void btTx(int byte) {
    //TODO
    
}

//timer interrupt
void gpio_isr(void *cbdata)
{
    //cbdata.ten = 1;   //enable timer
    TMR_Enable(TMR0);   //enable timer
}


int main(void)
{
//declarations
    gpio_cfg_t pb1, pb2, pb3, pb4;
    bool fx1 = false;
    bool fx2 = false;
    bool fx3 = false;
    bool fx4 = false;

    //int tElapsed;

    tmr_cfg_t tmr;

    btInit();

//MXC_TMR0 init config
    /*
    TMR0_CN.ten = 0;    //disable timer
    TMR0_CN.tmode = 000b;    //one-shot mode
    //TMR0_CN.pres3:TMR0_CN.pres    //set prescaler //TODO
    TMR0_CMP = 10;
    //use "TMR0_CN.ten = 1;" to enable timer
    */

    unsigned clk_shift = 0;
    uint64_t max_us;

    /* Find the proper clock shift for timer */
    do {
        max_us = (uint64_t)((0xFFFFFFFFUL / ((uint64_t)PeripheralClock >> clk_shift++)) * 1000000UL);
    } while (us > max_us);
    clk_shift--;

    TMR_Disable(TMR0);
    TMR_Init(TMR0, 0, 0);   //prescaler is second arg
    
    //ms to ticks
    uint32_t ticks; //ticks temp var
    TMR_GetTicks(TMR0, THRESH, TMR_UNIT_MILLISEC, &ticks);  //get ticks from ms
    
    //define tmr
    tmr.mode = TMR_MODE_ONESHOT;
    tmr.cmp_cnt = ticks;    //timer mode?
    tmr.pol = 0;    //compare register value in timer ticks. not used
    
    TMR_Config(TMR0, &tmr); //configure tmr as TMR0

    //use "TMR_Enable(TMR0);" to enable timer


//interrupt pin init (turn into function)TODO
    pb1.port = PORT_0;
    pb1.mask = PB1_PIN;
    pb1.pad = GPIO_PAD_PULL_DOWN;
    pb1.func = GPIO_FUNC_IN;
    GPIO_Config(&pb1);
    
    //GPIO_RegisterCallback(&pb1, gpio_isr, &tElapsed);
    //GPIO_RegisterCallback(&pb1, gpio_isr, &TMR0_CN);
    GPIO_RegisterCallback(&pb1, gpio_isr);
    
    GPIO_IntConfig(&pb1, GPIO_INT_EDGE, GPIO_INT_BOTH); //reads on falling and rising
    
    GPIO_IntEnable(&pb1);
    NVIC_EnableIRQ((IRQn_Type)MXC_GPIO_GET_IRQ(GPIO_PORT_INTERRUPT_IN));

    pb2.port = PORT_0;
    pb2.mask = PB2_PIN;
    pb2.pad = GPIO_PAD_PULL_DOWN;
    pb2.func = GPIO_FUNC_IN;

    pb3.port = PORT_0;
    pb3.mask = PB3_PIN;
    pb3.pad = GPIO_PAD_PULL_DOWN;
    pb3.func = GPIO_FUNC_IN;

    pb4.port = PORT_0;
    pb4.mask = PB4_PIN;
    pb4.pad = GPIO_PAD_PULL_DOWN;
    pb4.func = GPIO_FUNC_IN;

//loop
    while(1) {
        if(TMR_IntStatus(TMR0)) {   //check if TMR0 finished the cnt
            //somehow pass which switch got pushed to the bt func
            if(GPIO_InGet(&pb1)) {
                btTx(1);
            } else if(GPIO_InGet(&pb2)) {
                btTx(2);
            } else if(GPIO_InGet(&pb3)) {
                btTx(3);
            } else if(GPIO_InGet(&pb4)) {
                btTx(4);
            }

            //TMR_IntClear(TMR0);     //unnecessary because interrupt clears auto?
        }
        

        //old code, not oriented for interrupts
        /***
        //read pb1
        if (GPIO_InGet(&pb1)) {
            fx1 = true;
        } else {
            fx1 = false;
        }

        //read pb2
        if (GPIO_InGet(&pb2)) {
            fx2 = true;
        } else {
            fx2 = false;
        }

        //read pb3
        if (GPIO_InGet(&pb3)) {
            fx3 = true;
        } else {
            fx3 = false;
        }

        //read pb4
        if (GPIO_InGet(&pb4)) {
            fx4 = true;
        } else {
            fx4 = false;
        }

        if (fx1 || fx2 || fx3 || fx4) {
            LED_On(0);  //just for the evalboard?
        } else {
            LED_Off(0);
        }
        ***/
    }
}
