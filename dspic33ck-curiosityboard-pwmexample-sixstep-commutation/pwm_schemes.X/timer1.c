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

#ifdef __XC16__  
    #include <xc.h>
#endif 

#include <stdint.h>
#include <stdbool.h>
#include "timer1.h"

void TIMER1_Initialize(void)
{
    
    /** Initialize Timer1 Control Register
    bit 15:8    TON-U-SIDL-TMWDIS TMWIP-PRWIP-TECS<1:0> 
    bit 7:0     TGATE-U-TCKPS<1:0> U-TSYNC-TCS-U                          
    */
    T1CON = 0;
    
    /**1 = External Clock is from the T1CK pin
       0 = Internal Peripheral Bus Clock
     */
    
    T1CONbits.TCS = 0;
    /** Timer1 Input Clock Pre-scale Select bits
        11 = 1:256
        10 = 1:64
        01 = 1:8
        00 = 1:1 
    */    
    T1CONbits.TCKPS = 0b00;             
    
    /** Timer Period in Sec = ((Pre-scaler /FP) * PR1) 
        = ((1 / 200M) * 65535) =  327.67 uSec or  ~ 3.05 kHz */
    PR1 = 0xFFFF;
    
    _T1IF = 0;
    _T1IP = 5;
    _T1IE = 1;                        
    T1CONbits.TON = 1;     
}
