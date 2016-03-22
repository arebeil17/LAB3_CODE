/* 
 * File:   adc.c
 * Author: Andres
 *
 * Created on March 9, 2016, 10:19 PM
 */

#include "adc.h"


void initADC(){
    
    TRIS_RB15_POT = INPUT; //WIPER SET AS INPUT FOR PIN RB15
    
    TRIS_RA7_VRN = INPUT;
    TRIS_RA6_VRP = INPUT;
    
    ANSELA = 0x0060; //set ALL A to digital except for RA6 and RA7
    ANSELB = 0x8000; //set all B to digital except for RB15 ANALOG
    
    AD1CON1bits.FORM = 0; // 16 unsigned integer
    AD1CON1bits.SSRC = 7; // Auto-convert mode
    AD1CON1bits.ASAM = 1; // Auto-sampling
    AD1CON2bits.VCFG = 0b010; // Use internal VREF+ and external VREF- reference
    AD1CON2bits.CSCNA = 1; // enable scanning
    AD1CON2bits.SMPI = 15; // Take average of 16 samples
    AD1CON2bits.ALTS = 0; // Only Mux A
    AD1CON3bits.ADRC = 0; // Use PBCLK
    AD1CON3bits.SAMC = 31; // 2 Tad per sample
    AD1CON3bits.ADCS = 0xFF; // 4 times the PBCLK
    AD1CHSbits.CH0NA = 0; // Use Vref- as negative reference
    AD1CHSbits.CH0SA = 0; // Scan AN15 at least
    AD1CSSLbits.CSSL15 = 1; // Select AN15 for input SCAN
    IFS0bits.AD1IF = 0; // Put down ADC flag
    //IPC5bits.AD1IP = 7;
    //IEC0bits.AD1IE = 1;
    AD1CON1bits.ADON = 1; // turn on the ADC
}