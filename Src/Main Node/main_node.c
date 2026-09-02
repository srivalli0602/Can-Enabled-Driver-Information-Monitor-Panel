```c
#include "lcd.h"          // Include LCD driver functions and definitions
#include "delay.h"        // Include delay functions such as delay_ms() and delay_s()
#include "eint.h"         // Include external interrupt functions
#include "can.h"          // Include CAN driver functions and CAN frame structure
#include "ds18b20.h"      // Include DS18B20 temperature sensor functions
#include <lpc21xx.h>      // Include LPC21xx microcontroller register definitions

extern volatile int flag;        // External flag for left indicator, modified inside EINT0 ISR
extern volatile int flag1;       // External flag for right indicator, modified inside EINT1 ISR

// Custom character pattern for left-pointing arrow
unsigned char left_arrow[] = {
    0x02, 0x06, 0x0e, 0x1e,
    0x0e, 0x06, 0x02, 0x00
};

// Custom character pattern for right-pointing arrow
unsigned char right_arrow[] = {
    0x08, 0x0c, 0x0e, 0x0f,
    0x0e, 0x0c, 0x08, 0x00
};

// Custom character pattern for degree symbol
unsigned char degree[] = {
    0x06, 0x09, 0x09, 0x06,
    0x00, 0x00, 0x00, 0x00
};

// String displayed during the title animation
char *p = "CAN ENABLED DRIVER INFO & MONITOR PANEL";

// Border displayed above the title
char *start = "*-*-*-*-*-*-*-*-*-*-";

// Border displayed below the title
char *end = "*-*-*-*-*-*-*-*-*-*-";

int o, j;                  // Variables used to control title positioning


// Function to display the project title on the LCD
void Title(void)
{
    CmdLCD(0x80);          // Move LCD cursor to first row, first position

    while(*start)          // Continue until the end of the start string
    {
        CharLCD(*start);   // Display one character of the border
        delay_ms(80);      // Delay to create scrolling/animation effect
        start++;            // Move pointer to the next character
    }

    CmdLCD(0xd4);          // Move cursor to fourth row, first position

    while(*p)              // Continue until the end of the title string
    {
        o++;                // Increment title position counter

        if(o < 20)          // If the title is within the first 20 positions
        {
            CmdLCD(0xc0 + o); // Move cursor on the second LCD row
        }
        else                 // If the title exceeds the second row
        {
            CmdLCD(0x94 + j); // Move cursor to the third LCD row
            j++;              // Increment third-row position
        }

        CharLCD(*p);        // Display the current title character
        delay_ms(80);       // Delay between characters
        p++;                 // Move pointer to the next title character
    }

    CmdLCD(0xd4);           // Move cursor to fourth row, first position

    while(*end)             // Continue until the end of the end string
    {
        CharLCD(*end);      // Display one character of the bottom border
        delay_ms(80);       // Delay to create animation effect
        end++;               // Move pointer to the next character
    }
}


// Function to display fuel percentage along with a graphical fuel bar
void DisplayFuelBar(unsigned int fuel)
{
    int i;                  // Loop counter for displaying fuel blocks
    int bars;               // Number of filled blocks in the fuel bar

    // Make sure fuel percentage does not exceed 100
    if(fuel > 100)
        fuel = 100;

    /*
       Convert fuel percentage into 8 graphical blocks.

       100% -> 8 blocks
        75% -> 6 blocks
        50% -> 4 blocks
        25% -> 2 blocks
         0% -> 0 blocks
    */
    bars = (fuel * 8) / 100; // Calculate number of filled blocks

    CmdLCD(0x94);             // Move cursor to third LCD row

    StrLCD("Fuel:");           // Display "Fuel:"

    U32LCD(fuel);              // Display fuel percentage value

    CharLCD('%');              // Display percentage symbol

    /*
       Move cursor to the position where the fuel bar starts.
       0x94 + 9 is the tenth position of the third LCD row.
    */
    CmdLCD(0x94 + 9);

    CharLCD('[');              // Display opening bracket of fuel bar

    for(i = 0; i < 8; i++)    // Loop through all 8 fuel bar positions
    {
        if(i < bars)           // Check whether this position should be filled
        {
            /*
               0xFF represents a full block character
               on a standard HD44780-compatible LCD.
            */
            CharLCD(0xFF);     // Display a filled block
        }
        else
        {
            CharLCD(' ');      // Display a blank space for an empty block
        }
    }

    CharLCD(']');              // Display closing bracket of fuel bar
}


// Main program
int main()
{
    struct CAN_Frame CANRF;    // CAN frame used to receive data
    struct CAN_Frame CANF;     // CAN frame used to transmit data

    int temp;                  // Variable to store temperature reading
    unsigned char tp;          // Variable to store integer part of temperature
    unsigned char tpd;         // Variable to store fractional temperature character

    Init_CAN1();               // Initialize CAN1 peripheral
    Init_LCD();                // Initialize LCD
    enable_eint0();            // Enable external interrupt EINT0 for left indicator
    enable_eint1();            // Enable external interrupt EINT1 for right indicator
    Init_LCD();                // Initialize LCD again

    Title();                   // Display animated project title
    delay_s(2);                // Wait for 2 seconds

    CmdLCD(0x80);              // Move cursor to first row
    StrLCD("                    "); // Clear first LCD row

    CmdLCD(0xc0);              // Move cursor to second row
    StrLCD("                    "); // Clear second LCD row

    CmdLCD(0x94);              // Move cursor to third row
    StrLCD("                    "); // Clear third LCD row

    CmdLCD(0xd4);              // Move cursor to fourth row
    StrLCD("                    "); // Clear fourth LCD row

    CANF.vbf.RTR = 0;          // Configure CAN frame as a data frame
    CANF.vbf.DLC = 8;           // Set CAN data length code to 8 bytes

    CmdLCD(0x80);              // Move cursor to first row
    StrLCD("VEHICLE MONITOR:"); // Display vehicle monitoring heading

    /*
    CmdLCD(0xC0);              // Move cursor to second row
    StrLCD("Temp =  ");         // Display temperature label
    CharLCD(tpd);               // Display temperature character
    StrLCD(" C  ");             // Display Celsius unit
    */

    CmdLCD(0x94);              // Move cursor to third row
    StrLCD("Fuel:");            // Display fuel label

    CmdLCD(0xD4);              // Move cursor to fourth row
    StrLCD("indicator:");       // Display indicator label

    BuildCGRAM(left_arrow, 8, 0x40); // Store left arrow pattern in CGRAM location 0
    CmdLCD(0xDE);                    // Move cursor to indicator position
    CharLCD(0);                      // Display custom character 0 (left arrow)

    BuildCGRAM(right_arrow, 8, 0x48); // Store right arrow pattern in CGRAM location 1
    CmdLCD(0xDF);                     // Move cursor to next indicator position
    CharLCD(1);                       // Display custom character 1 (right arrow)

    BuildCGRAM(degree, 8, 0x50);      // Store degree symbol pattern in CGRAM location 2

    // Infinite loop; continuously monitor temperature, CAN data, and indicators
    while(1)
    {
        /*
        if(ResetDS18b20())          // Check/reset DS18B20 sensor
        {
        */

            temp = ReadTemp();       // Read temperature from DS18B20 using 1-Wire protocol

            tp = temp >> 4;          // Extract integer part of temperature

            /*
               Extract fractional part of temperature.
               If bit 3 is set, display '5'; otherwise display '0'.
            */
            tpd = temp & 0x08 ? 0x35 : 0x30;

            CmdLCD(0xC0);            // Move cursor to second LCD row
            StrLCD("Temp =");        // Display temperature label

            U32LCD(tp);              // Display integer part of temperature

            CharLCD('.');            // Display decimal point

            CharLCD(tpd);            // Display fractional part

            CharLCD(2);              // Display custom degree symbol

            StrLCD(" C  ");          // Display Celsius unit

        /*
        }
        else
        {
            CmdLCD(0xC5);            // Move cursor to temperature value position
            StrLCD("N/A");            // Display N/A if sensor is unavailable
        }
        */

        // Check whether a CAN message has been received
        if(rx_check())
        {
            CAN1_Rx(&CANRF);         // Receive CAN frame into CANRF structure

            // Check whether received message is the fuel message
            if(CANRF.ID == 3)
            {
                CmdLCD(0x94);         // Move cursor to third LCD row

                StrLCD("Fuel:    ");  // Display fuel label and spaces

                CmdLCD(0x94 + 5);    // Move cursor to fuel percentage position

                U32LCD(CANRF.Data1); // Display received fuel percentage

                StrLCD("%");         // Display percentage symbol

                DisplayFuelBar(CANRF.Data1); // Display graphical fuel bar
            }
        }

        /*
        else
        {
            CmdLCD(0x94 + 5);        // Move cursor to fuel value position
            StrLCD("    ");          // Clear previous fuel value

            CmdLCD(0x94 + 5);        // Move cursor again
            StrLCD("N/A");            // Display N/A when no fuel data is received
        }
        */

        // Check whether the left indicator interrupt has been triggered
        if(flag == 1)
        {
            CANF.ID = 1;             // Set CAN ID for indicator command

            CANF.Data1 = flag;       // Store left indicator flag in Data1

            CANF.Data2 = flag1;      // Store right indicator flag in Data2

            CAN1_Tx(CANF);           // Transmit indicator status through CAN

            CmdLCD(0xDE);             // Move cursor to left indicator position
            CharLCD(0);               // Display left arrow

            delay_ms(200);            // Wait 200 ms

            CmdLCD(0xDE);             // Move cursor to left indicator position
            CharLCD(' ');             // Clear left arrow

            CmdLCD(0xDF);             // Move cursor to right indicator position
            CharLCD(1);               // Display right arrow

            delay_ms(200);            // Wait 200 ms
        }

        // Check whether the right indicator interrupt has been triggered
        if(flag1 == 1)
        {
            CANF.ID = 1;             // Set CAN ID for indicator command

            CANF.Data1 = flag;       // Store left indicator flag in Data1

            CANF.Data2 = flag1;      // Store right indicator flag in Data2

            CAN1_Tx(CANF);           // Transmit indicator status through CAN

            CmdLCD(0xDE);             // Move cursor to left indicator position
            CharLCD(0);               // Display left arrow

            CmdLCD(0xDF);             // Move cursor to right indicator position
            CharLCD(1);               // Display right arrow

            delay_ms(200);            // Wait 200 ms

            CmdLCD(0xDF);             // Move cursor to right indicator position
            CharLCD(' ');             // Clear right arrow

            delay_ms(200);            // Wait 200 ms
        }

        /*
           If both indicators are OFF or both flags are ON,
           restore/display both indicator arrows and transmit
           the current indicator status.
        */
        else if((flag == 0 && flag1 == 0) || (flag == 1 && flag1 == 1))
        {
            BuildCGRAM(left_arrow, 8, 0x40); // Reload left arrow into CGRAM

            CmdLCD(0xDE);                    // Move cursor to left indicator position
            CharLCD(0);                      // Display left arrow

            BuildCGRAM(right_arrow, 8, 0x48); // Reload right arrow into CGRAM

            CmdLCD(0xDF);                     // Move cursor to right indicator position
            CharLCD(1);                       // Display right arrow

            CANF.ID = 1;                      // Set CAN ID for indicator command

            CANF.Data1 = flag;               // Store left indicator flag

            CANF.Data2 = flag1;              // Store right indicator flag

            CAN1_Tx(CANF);                   // Transmit current indicator status
        }
    }
}
```
