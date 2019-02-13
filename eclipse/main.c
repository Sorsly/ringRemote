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
    if(currSt != *prevSt) {
        TMR_Enable(TMR0);       //enables timer on change 
        *tStart = TMR_GetCount(TMR0);
    } else {
        if((TMR_GetCount(TMR0) - *tStart) > THRESH) {
            *prevSt = currSt;
            currSt = !currSt;
            TMR_Disable(TMR0);  //disables timer after settling
        }
    }
        
    return currSt;
}
int main(void)
{
    /*
    gpio_cfg_t gpio_in;
    gpio_cfg_t gpio_out;
    gpio_cfg_t gpio_interrupt;
    gpio_cfg_t gpio_interrupt_status;
    */

    gpio_cfg_t pb1, pb2, pb3, pb4;
    bool fx1 = false;
    bool fx2 = false;
    bool fx3 = false;
    bool fx4 = false;

    pb1.port = PORT_0;
    pb1.mask = PB1_PIN;
    pb1.pad = GPIO_PAD_PULL_DOWN;
    pb1.func = GPIO_FUNC_IN;

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

    while(1) {
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
            LED_On(0);
        } else {
            LED_Off(0);
        }
    }
}
