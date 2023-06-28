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

static void PWM_Generator1_Initialize(void);
static void PWM_Generator2_Initialize(void);
static void PWM_Generator3_Initialize(void);

void PWMInitialization(void)
{
    PCLKCON      = 0x0000;
    /* PWM Clock Divider Selection bits
       0b11 = 1:16 ; 0b10 = 1:8 ;0b01 = 1:4 ; 0b00 = 1:2*/
    PCLKCONbits.DIVSEL = 0;
    /* PWM Master Clock Selection bits
       0b11 = AFPLLO ; 0b10 = FPLLO ; 0b01 = AFVCO/2 ; 0b00 = FOSC */
    PCLKCONbits.MCLKSEL = 0;
    /* Lock bit: 0 = Write-protected registers and bits are unlocked   */
    PCLKCONbits.LOCK = 0;
    
     /* Set PWM MASTER Period */
    MPER = 10000;
    /* Set Duty Cycle - 25% */
    MDC = 0;
    /* Set Phase shift - No phase shift */
    MPHASE = 0;
    
    /*  Initialize PWM Generators */
    PWM_Generator1_Initialize();
    PWM_Generator2_Initialize();
    PWM_Generator3_Initialize();
    
    _PWM1IE = 1;
    _PWM1IP = 7;
    _PWM1IF = 0;
   
    PG3CONLbits.ON = 0;
    PG2CONLbits.ON = 0;
    PG1CONLbits.ON = 0;
}


void PWM_Generator1_Initialize (void)
{
    /* Select PWM Generator duty cycle register as MDC */
    /* Select PWM Generator period register as MPER */
    /* PWM Generator broadcasts software set of UPDREQ */
    /* control bit and EOC signal to other PWM Generators. */
    /* PWM buffer update mode is at start of next PWM cycle if UPDREQ = 1 */
    /* PWM generator operates in single trigger mode */
    /* Start of cycle is local EOC */
    PG1CONH = 0xC800;
    /* PWM Generator is disabled */
    /* PWM Generator uses Master Clock selected by
    * the PCLKCONbits.MCLKSEL bits */
    /* PWM Generator operates in Independent Edge PWM mode*/
    PG1CONL = 0x0008;
    /* PWM Generator Output Mode is Independent Mode */
    /* PWM Generator controls the PWMxH output pin */
    /* PWM Generator controls the PWMxL output pin */
    PG1IOCONH = 0x001C;
    /* Override is enabled on PWMxH/L with OVRDAT = 0b00,
    * turning OFF PWM outputs */
    /* User output overrides are synchronized to next start of cycle */
    PG1IOCONL = 0x0000;
    /* User must set the UPDREQ bit (PGxSTAT[3]) manually */
    /* EOC event is the PWM Generator trigger*/
    PG1EVTL = 0x0000;
    
    /* Interrupts CPU at EOC  */
    PG1EVTHbits.IEVTSEL = 0b00;
    
    PG1DTH = 100;
    PG1DTL = 100;
}

void PWM_Generator2_Initialize (void)
{
    /* Select PWM Generator duty cycle register as MDC */
    /* Select PWM Generator period register as MPER */
    /* Trigger output selected by PG1 PGTRGSEL[2:0] bits (PGxEVTL[2:0])*/
    /* PWM buffer update mode is at start of next PWM cycle if UPDREQ = 1 */
    /* PWM generator operates in single trigger mode */
    PG2CONH = 0xC001;
    /* PWM Generator is disabled */
    /* PWM Generator uses Master Clock selected by
    * the PCLKCONbits.MCLKSEL bits */
    /* PWM Generator operates in Independent Edge PWM mode*/
    PG2CONL = 0x0008;
    /* PWM Generator Output Mode is Independent Mode */
    /* PWM Generator controls the PWMxH output pin */
    /* PWM Generator controls the PWMxL output pin */
    PG2IOCONH = 0x001C;
    /* Override is enabled on PWMxH/L with OVRDAT = 0b00,
    * turning OFF PWM outputs */
    /* User output overrides are synchronized to next start of cycle */
    PG2IOCONL = 0x0000;

    PG2DTH = 100;
    PG2DTL = 100;
}

void PWM_Generator3_Initialize (void)
{
    /* Select PWM Generator duty cycle register as MDC */
    /* Select PWM Generator period register as MPER */
    /* Trigger output selected by PG1 PGTRGSEL[2:0] bits (PGxEVTL[2:0])*/
    /* PWM buffer update mode is at start of next PWM cycle if UPDREQ = 1 */
    /* PWM generator operates in single trigger mode */
    PG3CONH = 0xC001;
    /* PWM Generator is disabled */
    /* PWM Generator uses Master Clock selected by
    * the PCLKCONbits.MCLKSEL bits */
    /* PWM Generator operates in Independent Edge PWM mode*/
    PG3CONL = 0x0008;
    /* PWM Generator Output Mode is Independent Mode */
    /* PWM Generator controls the PWMxH output pin */
    /* PWM Generator controls the PWMxL output pin */
    PG3IOCONH = 0x001C;
    /* Override is enabled on PWMxH/L with OVRDAT = 0b00,
    * turning OFF PWM outputs */
    /* User output overrides are synchronized to next start of cycle */
    PG3IOCONL = 0x0000;

    PG3DTH = 100;
    PG3DTL = 100;
}