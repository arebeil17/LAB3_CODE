/* 
 * File:   pwm.c
 * Author: Andres
 *
 * Created on March 9, 2016, 11:02 PM
 */

#include "pwm.h"

void initMotorControl(){
    
    ANSELG = 0x0000;
            
    TRIS_RG6 = OUTPUT; 
    TRIS_RG7 = OUTPUT; 
    ENABLE_L = 0; ENABLE_R = 0;
    TRIS_RB10 = OUTPUT;
    TRIS_RB11 = OUTPUT; 
    TRIS_RB12 = OUTPUT; 
    TRIS_RB13 = OUTPUT; 
    MCG_L1 = 0; MCG_L2 = 0; MCG_R1 = 0; MCG_R2 = 0;
    ODCBbits.ODCB10 = ENABLED; 
    ODCBbits.ODCB11 = ENABLED; 
    ODCBbits.ODCB12 = ENABLED;  
    ODCBbits.ODCB13 = ENABLED; 
}

void initPWM(){
    
    initMotorControl();
    ANSELD = 0x0000;
    ANSELF = 0x0000;
    //-------------------------------------
    //FOR MOTOR 1 LEFT
    //-------------------------------------
    //TRIS_RD0 = OUTPUT; TRIS_RD1 = OUTPUT;
    OC1CON = 0x0000; //Turn off OC1 while doing setup
//    RPD0Rbits.RPD0R = OC1; //MAP OC1 TO RD0
//    RPD1Rbits.RPD1R = OC1; //MAP OC1 TO RD1
    
    T2CONbits.TCKPS = 0;
    //PERIOD CONTROL
    PR2 = 1023;
    TMR2 = 0;
    //PULSE WIDTH CONTROL OCxRS / PR2 = duty cycle
    OC1RS = 255;    //REAL-TIME UPDATE
    OC1R = 0;       //UPDATED BY OCxRS EVERY CLOCK CYCLE
    
    T2CONbits.TON = 1; //ENABLE CN
    
    OC1CONbits.OCTSEL = 0;
    OC1CONbits.OCM = 6; //6
    OC1CONbits.ON = 1;
    OC1CONSET = 0x8000; //Enable OC1
    //-------------------------------------
    //FOR MOTOR 2 RIGHT
    //-------------------------------------
    //TRIS_RF1 = OUTPUT; TRIS_RD2 = OUTPUT;
    OC3CON = 0x0000; //Turn off OC3 while doing setup
//    RPF1Rbits.RPF1R = OC3; //MAP OC3 TO RF1
//    RPD2Rbits.RPD2R = OC3; //MAP OC3 TO RD2
    
    T3CONbits.TCKPS = 0;
    //PERIOD CONTROL
    PR3 = 1023;
    TMR3 = 0;
    //PULSE WIDTH CONTROL OCxRS / PR2 = duty cycle
    OC3RS = 255;    //REAL-TIME UPDATE
    OC3R = 0;       //UPDATED BY OCxRS EVERY CLOCK CYCLE
    
    T3CONbits.TON = 1; //ENABLE CN
    
    OC3CONbits.OCTSEL = 1; //selects Timer to be used
    OC3CONbits.OCM = 6; //6
    OC3CONbits.ON = 1;
    OC3CONSET = 0x8000; //Enable OC3
}

void updatePWM(float duty, int motor, int direction){
    
    TMR2 = 0;
    TMR3 = 0;
    if(motor == 1){
        OC1RS = floor(duty*1023);
    }else if(motor == 2){
        OC3RS = floor(duty*1023);
    }else if(motor == 0){
        OC1RS = floor(duty*1023);
        OC3RS = floor(duty*1023);
    }
    if(direction == FORWARD){
        MAP_L1_OC1 = 0;
        MAP_L2_OC1 = OC1;
        MAP_R1_OC3 = 0;
        MAP_R2_OC3 = OC3;
    }else if(direction == REVERSE){
        MAP_L1_OC1 = OC1;//fix
        MAP_L2_OC1 = 0;
        MAP_R1_OC3 = OC3;//fix
        MAP_R2_OC3 = 0;
    }else{
        MAP_L1_OC1 = 0;
        MAP_L2_OC1 = 0;
        MAP_R1_OC3 = 0;
        MAP_R2_OC3 = 0;
    }
}