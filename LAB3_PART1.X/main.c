// ******************************************************************************************* //
//
// File:         lab1p2.c
// Date:         Feb 16, 2016
// Authors:      Andres D. Rebeil
//
// Description: 
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "switch.h"
#include "leds.h"
#include "config.h"
#include "interrupt.h"
#include "adc.h"
#include "pwm.h"

#define pressed 0
#define released 1
#define SW1 PORTAbits.RA7 


typedef enum stateTypeEnum{
    INITIAL, CONTROL
} stateType;

//Volatile variable declarations
volatile stateType state = INITIAL;
volatile unsigned int SW1_pressed = 0, SW1_toggle = 0;

volatile unsigned int ADC_Value = 0;

// ******************************************************************************************* //

int main(void)
{
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    
    //INITIALIZATION PROCESS
    initLEDs();
    initTimer1();
    initTimer2();
    initLCD();
    delayMs(1);
    //initSW();
    initADC();
    initMotorControl();
    initPWM();
    int TEMP = 0, i = 0;
    turnOnLED(0);
    for(i = 0; i < 20; i++) delayMs(50);
    turnOnLED(-1);
    for(i = 0; i < 20; i++) delayMs(50);
    turnOnLED(0);
    for(i = 0; i < 20; i++) delayMs(50);
    turnOnLED(-1);
    //INITIALIZATION COMPLETE
    while(1)
    {   
        if(IFS0bits.AD1IF == 1) 
        {   
            ADC_Value = ADC1BUF0;
            displayVoltage(0,ADC_Value);
            IFS0bits.AD1IF = 0;
        }
        if(TEMP != ADC_Value){
            //delayMs(5);
            if(ADC_Value >= 768) {
                turnOnLED(1);
                ENABLE_L = 1; ENABLE_R = 1;
                MCG_L1 = 1; MCG_R1 = 1;
                MCG_L2 = 0; MCG_R2 = 0;
                updatePWM(0.80,0,FORWARD);
            }else if(ADC_Value <= 256){
                turnOnLED(2);
                ENABLE_L = 1; ENABLE_R = 1;
                MCG_L1 = 0; MCG_R1 = 0;
                MCG_L2 = 1; MCG_R2 = 1;
                updatePWM(0.80 ,0,REVERSE);
            }else{
                turnOnLED(-1);
                ENABLE_L = 1; ENABLE_R = 1;
                MCG_L1 = 0; MCG_R1 = 0;
                MCG_L2 = 0; MCG_R2 = 0;
                updatePWM(0.60,0,IDLE);
            }
        }
         TEMP = ADC_Value;
        moveCursorLCD(1,1);
    }
            
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void){
    //----------------------------------------
    int j;
    PORTA;
    j = SW1; //RA7

    //CHECK SW2 TOGGLE
    if(SW1 == pressed){
        SW1_pressed = 1;
    }else if((SW1 == released) && (SW1_pressed == 1)){
        SW1_pressed = 0;
        SW1_toggle = 1;
    }
    //RESET FLAGS
    IFS1bits.CNAIF = 0; //FLAG DOWN
}

//void __ISR(_ADC_VECTOR, IPL7AUTO) _ADCInterrupt(void){
//    
//    IFS0bits.AD1IF = 0;
//    
//    ADC_Value = ADC1BUF0;
//}