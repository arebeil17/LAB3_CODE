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
    INITIAL, DRIVE_1, IDLE_1, DRIVE_2, IDLE_2
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
    initSW();
    initADC();
    initMotorControl();
    initPWM();
    int TEMP = 0, i = 0, UPDATE = 1;
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
        if(abs(TEMP - ADC_Value)>= 5) UPDATE = 1;
        
        if(SW1_pressed) delayMs(10);
        
        switch(state){
            case INITIAL:
                turnOnLED(0);
                TEMP = readADC(TEMP, ADC_Value);
                updatePWM(TEMP/1023.0,SYNC,IDLE);
                if(SW1_toggle){ state = DRIVE_1; SW1_toggle = 0; delayMs(10); 
                                UPDATE = 1;}
                break;
            case DRIVE_1:
                turnOnLED(1);
                TEMP = readADC(TEMP, ADC_Value);
                if(UPDATE){
                    updatePWM(getDutyCycle(TEMP,LEFT),LEFT,FORWARD);
                    updatePWM(getDutyCycle(TEMP,RIGHT),RIGHT,FORWARD);
                    UPDATE = 0;
                }
                if(SW1_toggle){ state = IDLE_1; SW1_toggle = 0; delayMs(10);}
                break;
            case IDLE_1:
                turnOnLED(0);
                TEMP = readADC(TEMP, ADC_Value);
                updatePWM(TEMP/1023.0,0,IDLE);
                if(SW1_toggle){ state = DRIVE_2; SW1_toggle = 0; delayMs(10);
                                UPDATE = 1;}
                break;
            case DRIVE_2:
                turnOnLED(2);
                TEMP = readADC(TEMP, ADC_Value);
                if(UPDATE){
                    updatePWM(getDutyCycle(TEMP,LEFT),LEFT,REVERSE);
                    updatePWM(getDutyCycle(TEMP,RIGHT),RIGHT,REVERSE);
                    UPDATE = 0;
                }
                if(SW1_toggle){ state = IDLE_2; SW1_toggle = 0; delayMs(10);}
                break;
            case IDLE_2:
                turnOnLED(0);
                TEMP = readADC(TEMP, ADC_Value);
                updatePWM(TEMP/1023.0,SYNC,IDLE);
                if(SW1_toggle){ state = DRIVE_1; SW1_toggle = 0; delayMs(10);
                                UPDATE = 1;}
                break;
        }   
        
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