// ******************************************************************************************* //
//
// File:         LAB3_PART1
// Date:         March 16, 2016
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
    initADC();
    initPWM();
    int TEMP = 0, i = 0;
    turnOnLED(0);
    for(i = 0; i < 20; i++) delayMs(50);
    turnOnLED(-1);
    for(i = 0; i < 20; i++) delayMs(50);
    turnOnLED(0);
    for(i = 0; i < 20; i++) delayMs(50);
    turnOnLED(-1);
    moveCursorLCD(1,1);
    displayVoltage(0,ADC_Value);
    //INITIALIZATION COMPLETE
    
    while(1)
    {   
        if(IFS0bits.AD1IF == 1) 
        {   
            ADC_Value = ADC1BUF0;
            IFS0bits.AD1IF = 0;
        }
        switch(state){
            case INITIAL:
                turnOnLED(1);
                for(i = 0; i < 20; i++) delayMs(50);
                state = CONTROL;
            break;
            case CONTROL:
                turnOnLED(2);
                if((abs(TEMP - ADC_Value)>= 5)){
                    delayMs(10);
                    ENABLE_L = 1; ENABLE_R = 1;
                    MCG_L1 = DISABLE_ODC; MCG_R1 = DISABLE_ODC;
                    MCG_L2 = ENABLE_ODC;  MCG_R2 = ENABLE_ODC;
                    updatePWM(TEMP/1023.0, 0, REVERSE);
                    clearLCD();
                    moveCursorLCD(1,1);
                    displayVoltage(0, ADC_Value);
                }
            break; 
        }
        if(abs(TEMP - ADC_Value)>= 5) TEMP = ADC_Value;   
    }      
    return 0;
}
