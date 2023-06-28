/*******************************************************************************
* Copyright (c) 2019 released Microchip Technology Inc.  All rights reserved.
*
* SOFTWARE LICENSE AGREEMENT:
* 
* Microchip Technology  ("Microchip") retains all ownership and
* intellectual property rights in the code aIncorporatedccompanying this message and in all
* derivatives hereto.  You may use this code, and any derivatives created by
* any person or entity by or on your behalf, exclusively with Microchip's
* proprietary products.  Your acceptance and/or use of this code constitutes
* agreement to the terms and conditions of this notice.
*
* CODE ACCOMPANYING THIS MESSAGE IS SUPPLIED BY MICROCHIP "AS IS".  NO
* WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
* TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP'S
* PRODUCTS, COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.
*
* YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE LIABLE,
* WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
* STATUTORY DUTY),STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE,
* FOR ANY INDIRECT, SPECIAL,PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL
* LOSS, DAMAGE, FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE,
* HOWSOEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR
* THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT ALLOWABLE BY LAW,
* MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS CODE,
* SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO MICROCHIP SPECIFICALLY TO
* HAVE THIS CODE DEVELOPED.
*
* You agree that you are solely responsible for testing the code and
* determining its suitability.  Microchip has no obligation to modify, test,
* certify, or support the code.
*
*******************************************************************************/
#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "pwm.h"
#include "port_config.h"
#include "board_service.h"

uint16_t state = 0;
uint16_t counter = 0; 
uint16_t schemePWM = 0,switchPress = 0;

uint16_t PWM1[3][6] = {{0x1000, 0x1000, 0x3000, 0x2000, 0x2000, 0x3000},
					   {0x0000, 0x3000, 0x3400, 0x3400, 0x3000, 0x0000},
					   {0x0000, 0x0000, 0x3000, 0x4000, 0x4000, 0x7000}};

uint16_t PWM2[3][6] = {{0x2000, 0x3000, 0x1000, 0x1000, 0x3000, 0x2000},
					   {0x3000, 0x0000, 0x0000, 0x3000, 0x3400, 0x3400},
					   {0x4000, 0x7000, 0x0000, 0x0000, 0x3000, 0x4000}};

uint16_t PWM3[3][6] = {{0x3000, 0x2000, 0x2000, 0x3000, 0x1000, 0x1000},
				       {0x3400, 0x3400, 0x3000, 0x0000, 0x0000, 0x3000},
					   {0x3000, 0x4000, 0x4000, 0x7000, 0x0000, 0x0000}};

void APP_UpdatePWMConfig_Scheme1(void);
void APP_UpdatePWMConfig_Scheme2(void);
void APP_UpdatePWMConfig_Scheme3(void);

// *****************************************************************************
/* Function:
   _PWM1Interrupt()

  Summary:
   _PWM1Interrupt() ISR routine

  Description:
  ISR executes at every 50us.Counter value increments upto 4 which results 4 PWM 
  cycles per sector(200us). PGxIOCONL register update happens at the end of the 
  sector.
  schemePWM is updated according to the push button status.
  State represents the sector.
  
  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */
void __attribute__((__interrupt__, no_auto_psv)) _PWM1Interrupt(void)
{
    if(counter < 4)
    {
        counter++;
    }
    else
    {
        PG1IOCONL = PWM1[schemePWM][state];
        PG2IOCONL = PWM2[schemePWM][state];
        PG3IOCONL = PWM3[schemePWM][state];
        if(state < 5)
        {
            state++;
        }
        else
        {
            state = 0;
        }
        counter = 0;
    }
    IFS4bits.PWM1IF = 0; //Clear PWM interrupt flag
}

// *****************************************************************************
/* Function:
   _T1Interrupt()

  Summary:
   _T1Interrupt() ISR routine

  Description:
  Executes at the rate of 3kHz.BoardServiceStepIsr() is executed and if the 
  push button is pressed, the PWM schemes are updated accordingly.

  Precondition:
    None.

  Parameters:
    None

  Returns:
    None.

  Remarks:
    None.
 */

void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void)
{
    BoardServiceStepIsr();
    if(IsPressed_Button1() == true)
    {
        switch(switchPress)
        {
            case 0: 
                RED = 1; 
                APP_UpdatePWMConfig_Scheme1();
                schemePWM = 0;
            break;
            case 1:
                RED = 0;
                PG3CONLbits.ON = 0;
                PG2CONLbits.ON = 0;
                PG1CONLbits.ON = 0;
            break;
            case 2: 
                GREEN = 1;
                APP_UpdatePWMConfig_Scheme2();
                schemePWM = 1;
            break;
            case 3: 
                GREEN = 0;
                PG3CONLbits.ON = 0;
                PG2CONLbits.ON = 0;
                PG1CONLbits.ON = 0;
            break;
            case 4: 
                BLUE = 1;  
                APP_UpdatePWMConfig_Scheme3();
                schemePWM = 2;
            break;
            case 5: 
                BLUE = 0;
                PG3CONLbits.ON = 0;
                PG2CONLbits.ON = 0;
                PG1CONLbits.ON = 0;
            break;
        }
        if(switchPress < 5)
        {
            switchPress++;
        }
        else
        {
            switchPress = 0;
        }
    }
    IFS0bits.T1IF = 0; //Clear TIMER1 interrupt flag
}

void APP_UpdatePWMConfig_Scheme1(void)
{ 
    /* PWM Generators operate in Independent Edge PWM mode*/
    /* PWM Generator Output Mode is Independent Mode */
    PG3CONLbits.MODSEL = 0;
    PG3IOCONHbits.PMOD = 1;
    PG2CONLbits.MODSEL = 0;
    PG2IOCONHbits.PMOD = 1;
    PG1CONLbits.MODSEL = 0;
    PG1IOCONHbits.PMOD = 1;
    
    MPER = 10000;
    MDC  = 5000;
    
    /* User software writes a ?1? to this bit location to request a PWM Data 
       register update.*/
    PG1STATbits.UPDREQ = 1;
    
    /* Enable PWM generators, starting all PWM generators together */
    PG3CONLbits.ON = 1;
    PG2CONLbits.ON = 1;
    PG1CONLbits.ON = 1;
}
void APP_UpdatePWMConfig_Scheme2(void)
{
    /* PWM Generators operate in Independent Edge PWM mode*/
    /* PWM Generator Output Mode is Complementary Mode */
    PG3CONLbits.MODSEL = 0;
    PG3IOCONHbits.PMOD = 0;
    PG2CONLbits.MODSEL = 0;
    PG2IOCONHbits.PMOD = 0;
    PG1CONLbits.MODSEL = 0;
    PG1IOCONHbits.PMOD = 0;
    
    MPER = 10000;
    MDC  = 5000;
    PG1DTH = 100;
    PG1DTL = 100;
    
    /* User software writes a ?1? to this bit location to request a PWM Data 
       register update.*/
    PG1STATbits.UPDREQ = 1;
    
    /* Enable PWM generators, starting all PWM generators together */
    PG3CONLbits.ON = 1;
    PG2CONLbits.ON = 1;
    PG1CONLbits.ON = 1;
    
}
void APP_UpdatePWMConfig_Scheme3(void)
{
    /* PWM Generators operate in Center-Aligned PWM mode*/
    /* PWM Generator Output Mode is Complementary Mode */
    PG3CONLbits.MODSEL = 4;
    PG3IOCONHbits.PMOD = 0;
    PG2CONLbits.MODSEL = 4;
    PG2IOCONHbits.PMOD = 0;
    PG1CONLbits.MODSEL = 4;
    PG1IOCONHbits.PMOD = 0;
    
    MPER = 5000;
    MDC  = 2500;
    PG1DTH = 200;
    PG1DTL = 200;
    
    /* User software writes a ?1? to this bit location to request a PWM Data 
       register update.*/
    PG1STATbits.UPDREQ = 1;
    
    /* Enable PWM generators, starting all PWM generators together */    
    PG3CONLbits.ON = 1;
    PG2CONLbits.ON = 1;
    PG1CONLbits.ON = 1;
}
