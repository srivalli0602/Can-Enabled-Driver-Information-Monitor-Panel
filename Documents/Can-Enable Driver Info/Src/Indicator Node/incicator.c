```c
#include "can.h"              // CAN driver functions and CAN frame structure
#include "lcd.h"              // LCD driver functions (currently not used)
#include "delay.h"            // Delay functions such as delay_ms()
#include <lpc21xx.h>          // LPC21xx microcontroller register definitions

#define LED 0                  // Starting bit position of LEDs at P0.0

int main()
{
    struct CAN_Frame CANRF;    // Structure to store the received CAN frame
    int i;                     // Loop counter for LED sequence
    int ls, rs;                // Variables for left and right indicator status

    Init_CAN1();               // Initialize CAN1 peripheral

    IODIR0 |= 255 << LED;      // Configure P0.0-P0.7 as output for 8 LEDs

    IOPIN0 = 255 << LED;       // Turn OFF all LEDs initially
                               // LEDs are active-low, so writing 1 turns them OFF

    while(1)                   // Continuously monitor CAN messages
    {
        if(rx_check())         // Check whether a CAN message has been received
        {
            CAN1_Rx(&CANRF);   // Receive the CAN frame into CANRF

            ls = CANRF.Data1;  // Get left indicator status from Data1
            rs = CANRF.Data2;  // Get right indicator status from Data2

            if(CANRF.ID == 1)  // Check whether the received message is an indicator command
            {
                // Left indicator command
                // Left indicator LEDs glow from right to left
                if((ls == 1) && (rs == 0))
                {
                    for(i = 0; i <= 7; i++)   // Move through LEDs from P0.0 to P0.7
                    {
                        if(ls == 0)             // Stop sequence if left indicator is disabled
                            break;

                        /*
                         * LEDs are active-low.
                         * ~(1 << i) makes the selected LED LOW
                         * and all other LED bits HIGH.
                         *
                         * & 255 limits the result to 8 bits.
                         */
                        IOPIN0 = ((~(1 << i)) & 255) << LED;

                        delay_ms(50);           // Delay between LED transitions
                    }
                }

                // Right indicator command
                // Right indicator LEDs glow from left to right
                else if((rs == 1) && (ls == 0))
                {
                    for(i = 7; i >= 0; i--)    // Move through LEDs from P0.7 to P0.0
                    {
                        if(rs == 0)             // Stop sequence if right indicator is disabled
                            break;

                        /*
                         * Select one LED at a time by making its output LOW.
                         * Other LEDs remain HIGH (OFF).
                         */
                        IOPIN0 = ((~(1 << i)) & 255) << LED;

                        delay_ms(50);           // Delay between LED transitions
                    }
                }

                // Both indicators OFF or both indicators ON
                else if((ls == 0 && rs == 0) || (ls == 1 && rs == 1))
                {
                    IOPIN0 = 255 << LED;        // Turn OFF all LEDs
                }
            }
        }

        // U32LCD(CANRF.Data1);                 // Previously used to display CAN data on LCD
    }
}
```
