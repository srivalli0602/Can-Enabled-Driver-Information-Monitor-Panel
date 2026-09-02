```c
#include "can.h"              // CAN driver functions and CAN frame structure
#include "delay.h"            // Delay functions
#include "adc_defines.h"      // ADC-related definitions and macros
#include "adc.h"              // ADC initialization and ADC reading functions

u32 val;                      // Variable to store calculated fuel percentage
u32 AdcDval;                 // Variable to store the raw ADC digital value
f32 eAR;                      // Variable to store the ADC equivalent analog voltage

#define min 0.45              // Minimum sensor voltage corresponding to 0% fuel
#define max 2.1               // Maximum sensor voltage corresponding to 100% fuel


int main()
{
    struct CAN_Frame CANF;    // CAN frame used to transmit fuel information

    Init_CAN1();              // Initialize CAN1 peripheral
    Init_ADC();               // Initialize the ADC peripheral

    CANF.ID = 3;              // Set CAN ID 3 for fuel-level messages
                              // CAN ID 3 identifies the Fuel Node data

    CANF.vbf.RTR = 0;         // Set RTR = 0 to configure a CAN data frame

    CANF.vbf.DLC = 4;         // Set the Data Length Code
                              // Check your CAN driver structure to confirm
                              // the intended number of data bytes


    while(1)
    {
        /*
         * Read the analog fuel-level input from ADC channel 1.
         *
         * AdcDval → raw ADC digital value
         * eAR     → equivalent analog voltage
         */
        Read_ADC(CH1, &AdcDval, &eAR);

        /*
         * Convert the sensor voltage into fuel percentage.
         *
         * min = voltage corresponding to 0% fuel
         * max = voltage corresponding to 100% fuel
         *
         * Formula:
         *
         * Fuel % = ((Voltage - Minimum Voltage) /
         *           (Maximum Voltage - Minimum Voltage)) × 100
         */
        val = ((eAR - min) / (max - min)) * 100;

        /*
         * Limit the calculated fuel percentage to 100%.
         * This prevents values above 100 if the input voltage
         * exceeds the configured maximum voltage.
         */
        if(val > 100)
        {
            val = 100;
        }

        CANF.Data1 = val;      // Store fuel percentage in CAN Data1

        CAN1_Tx(CANF);         // Transmit fuel percentage through CAN

        // delay_ms(500);       // Optional delay between CAN transmissions
                               // Currently disabled, so transmission
                               // occurs continuously
    }
}
```
