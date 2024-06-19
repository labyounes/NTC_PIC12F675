#include <stdint.h>                 
#include <stdbool.h>
 
#define RELAY1_PIN GPIO.F1          // Define GPIO1 as RELAY1 Pin                      
#define RELAY2_PIN GPIO.F2          // Define GPIO1 as RELAY1 Pin 
 
#pragma config MCLRE = OFF          // MCLRE OFF on GPIO3
 
#define Series_Resistance 10000.00  // 10k ohm between AN0 on GPIO0 and GND
 
/*----------- Enter Resistor value and Time for phase 1 and phase 2 --------*/
#define Target_Resistance 4706.6    // NTC resistance at 24°C
#define Time_R1           60        // Phase 1 time
#define Time_R2           5         // Phase 2 time
/*--------------------------------------------------------------------------*/
 
unsigned long secondsCounter = 0;   // Time Counter 
unsigned int value;                 // ADC value
float NTC = 0.0;                    // Initiate NTC value at 0
bool firstPhase = true;
 
void main() 
{
    TRISIO = 1;                     // Set GP0 as input (AN0), GP1 and GP2 as output 0b00000001 
    ANSEL = 1;                      // Set AN0 as analog input 0b0001
    CMCON = 7;                      // Disable comparators 0b0111
    ADC_Init();                     // Initialize ADC
    GPIO = 0;                       // Initialize GPIO to 0
 
    while(1) 
     { 
        value = ADC_Read(0);        // Read ADC value from channel 0
        if (value == 0) {
            NTC = Series_Resistance; // Avoid division by zero
        } else {
            NTC = ((1023.0 / value) - 1) * Series_Resistance; // Calculate NTC resistance
        }
         
        /* ---------------------- Control Relays ---------------------- */    
 
     if (firstPhase) {
 
         if( NTC <= Target_Resistance )
            RELAY1_PIN = 1;        
         else
            RELAY1_PIN = 0;     
 
         if (secondsCounter >= Time_R1) {        // 60 seconds for first phase
               RELAY1_PIN = 0;              // Turn off relay 1 (GP1)
               firstPhase = false;
               secondsCounter = 0;          // Reset counter for next phase
         }
 
         } else { 
        
            RELAY2_PIN = 1;                 // Turn on relay 2 (GP2)
 
            if (secondsCounter >= Time_R2) {      // 5 seconds for second phase
                RELAY2_PIN = 0;             // Turn off relay 2 (GP2)
                firstPhase = true;
                secondsCounter = 0;         // Reset counter for next phase
            }
        }
 
        Delay_ms(1000);                    // Delay 1 second
        secondsCounter++;                  // Increment counter
    }
}
