#include <stdint.h>
#include <stdbool.h>
#include <math.h>
 
#define RELAY1_PIN GPIO.F1          // GPIO1                       
#define RELAY2_PIN GPIO.F2          // GPIO2
 
#pragma config MCLRE = OFF          // MCLRE OFF on GPIO3
 
#define Series_Resistance 10000.00  // 10k ohm between AN0 on GPIO0 and GND 
#define Target_Resistance 4706.6    // NTC resistance at 24°C
 
unsigned int value;
float NTC_Resistance = 0.0;
 
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
            NTC_Resistance = Series_Resistance; // Avoid division by zero
        } else {
            NTC_Resistance = ((1023.0 / value) - 1) * Series_Resistance; // Calculate NTC resistance
        }
         
        //Control Relays    
 
        if (NTC_Resistance <= Target_Resistance) {
            RELAY1_PIN = 1;        // Turn on relay 1 (GP1)
        } else {
            RELAY1_PIN = 0;        // Turn off relay 1 (GP1)
        }
 
        Delay_ms(1000);            // Delay 1 second
    }
}
