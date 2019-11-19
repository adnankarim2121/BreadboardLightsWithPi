/*
The first pattern turns on lights 1,2,3 after clicking button on GPIO 23 every 0.5 seconds.
The second pattern turns on lights 3,2,1 after clicking button on GPIO 22 every 0.25 seconds
*/

// All Neccessary Include files
#include "uart.h"
#include "sysreg.h"
#include "gpio.h"
#include "irq.h"

// Function prototypes
void init_GPIO23_to_risingEdgeInterrupt();
void init_GPIO22_to_risingEdgeInterrupt();

void init_GPIO4_to_output();
void init_GPIO17_to_output();
void init_GPIO27_to_output();

void set_GPIO4();
void clear_GPIO4();

void set_GPIO17();
void clear_GPIO17();

void set_GPIO27();
void clear_GPIO27();

unsigned long getSystemTimerCounter();
void timedWait(int delay );

// Declare a global shared variable
unsigned int sharedValue;


/*
Main function. The function which runs the specified behviour. In C, the main function 
is needed to run any file. Hence, majority of the program execution occurs within here.
Return value is void.
*/

void main()
{
    //unsigned int r;

    // Set up the UART serial port
    uart_init();

    // Initialize the sharedValue global variable and
    // and set the local variable to be same value
    sharedValue = 0;


    /*
    Set up GPIO pin #23 and #22 to input and so that it triggers
    an interrupt when a rising edge is detected
    */
    init_GPIO23_to_risingEdgeInterrupt();
    init_GPIO22_to_risingEdgeInterrupt();

    /*
    Set up GPIO pin #4, #17 and #27 to output. These are leds 1, 2 and 3 respectively. 
    */
    init_GPIO4_to_output();
    init_GPIO17_to_output();
    init_GPIO27_to_output();

    // Enable IRQ Exceptions
    enableIRQ();

    // Print out a message to the console
    uart_puts("\nRising Edge IRQ program starting.\n");

    // Loop forever, waiting for interrupts to change the shared value
    while (1)
    {
        if (sharedValue == 0)
        {
            /*
            When the shared value is zero, clear the first
            pattern (1 -> 2 -> 3). This is triggered when hitting the
            push button in GPIO 23.
            */
            clear_GPIO4();
            clear_GPIO17();
            clear_GPIO27();
        }
        else if (sharedValue == 1)
        {
            /*
            When the shared value is one, the first
            pattern (1 -> 2 -> 3) is lit up every 0.5s between LED. 
            This is triggered when hitting the
            push button in GPIO 23.
            */

            // Turn on the First LED
            set_GPIO4();

            // Print a message to the console
            uart_puts("ON\n");

            //Wait 0.5s before turning first LED off
            timedWait(500000);

            // Turn the First LED off
            clear_GPIO4();

            // Print a message to the console
            uart_puts("OFF\n");

            //Wait 0.5s before turning on second LED. 
            timedWait(500000);

            // Turn on the 2nd LED
            set_GPIO17();

            // Print a message to the console
            uart_puts("ON\n");

            //Wait 0.5s before turning off second LED. 
            timedWait(500000);

            // Turn the LED off
            clear_GPIO17();

            // Print a message to the console
            uart_puts("OFF\n");

            //Wait 0.5s before turning on third LED. 
            timedWait(500000);

            // Turn on the 3rd LED
            set_GPIO27();

            // Print a message to the console
            uart_puts("ON\n");

            //Wait 0.5s before turning off third LED. 
            timedWait(500000);

            // Turn the LED off
            clear_GPIO27();

            // Print a message to the console
            uart_puts("OFF\n");

            //Wait 0.5s before turning on first LED. 
            timedWait(500000);

        }

        else if(sharedValue == 3)
        {
            /*
            When the shared value is three, clear the second
            pattern (3 -> 2 -> 1). This is triggered when hitting the
            push button in GPIO 22.
            */
            clear_GPIO27();
            clear_GPIO17();
            clear_GPIO4();
        }

        else
        {
            /*
            When the shared value is four, the second
            pattern (3 -> 2 -> 1) is lit up every 0.25s between LED. 
            This is triggered when hitting the
            push button in GPIO 22.
            */
            // Turn on the third LED
            set_GPIO27();

            // Print a message to the console
            uart_puts("ON\n");

            //Wait 0.25s before turning off third LED
            timedWait(250000);

            // Turn the third LED off
            clear_GPIO27();

            // Print a message to the console
            uart_puts("OFF\n");

            //Wait 0.25s before turning on second LED
            timedWait(250000);

            // Turn on the 2nd LED
            set_GPIO17();

            // Print a message to the console
            uart_puts("ON\n");

            //Wait 0.25s before turning off second LED
            timedWait(250000);

            // Turn the LED off
            clear_GPIO17();

            // Print a message to the console
            uart_puts("OFF\n");

            //Wait 0.25s before turning on first LED
            timedWait(250000);

            // Turn on the first LED
            set_GPIO4();

            // Print a message to the console
            uart_puts("ON\n");

            //Wait 0.25s before turning off first LED
            timedWait(250000);

            // Turn first LED off
            clear_GPIO4();

            // Print a message to the console
            uart_puts("OFF\n");

            //Wait 0.25s before turning on third LED
            timedWait(250000);
        }
    }
}




/*
This function sets GPIO pin 23 to an input pin without any internal pullup or 
pulldown resistor. It is used as an interrupt to turn on and off the first LED pattern
(1->2->3).

A void value is returned for this function.
*/

void init_GPIO23_to_risingEdgeInterrupt()
{
    register unsigned int r;


    // Get the current contents of the GPIO Function Select Register 2
    r = *GPFSEL2;

    // Clear bits 9 - 11. This is the field FSEL23, which maps to GPIO pin 23.
    // We clear the bits by ANDing with a 000 bit pattern in the field. This
    // sets the pin to be an input pin
    r &= ~(0x7 << 9);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 2
    *GPFSEL2 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 23. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. We
    // will pull down the pin using an external resistor connected to ground.

    // Disable internal pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 23 to
    // clock in the control signal for GPIO pin 23. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 |= (0x1 << 23);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;

    // Set pin 23 to so that it generates an interrupt on a rising edge.
    // We do so by setting bit 23 in the GPIO Rising Edge Detect Enable
    // Register 0 to a 1 value (p. 97 in the Broadcom manual).
    *GPREN0 |= (0x1 << 23);

    // Enable the GPIO IRQS for ALL the GPIO pins by setting IRQ 52
    // GPIO_int[3] in the Interrupt Enable Register 2 to a 1 value.
    //20 cause; each register has access to 32 bits; so for index 2, we have access to bits
    //32-63, and so 52 is the 20th spot for index 2
    // See p. 117 in the Broadcom Peripherals Manual.
    *IRQ_ENABLE_IRQS_2 = (0x1 << 20);
}

void init_GPIO22_to_risingEdgeInterrupt()
{
    register unsigned int r;


    // Get the current contents of the GPIO Function Select Register 2
    r = *GPFSEL2;

    // Clear bits 6 - 8. This is the field FSEL22, which maps to GPIO pin 22.
    // We clear the bits by ANDing with a 000 bit pattern in the field. This
    // sets the pin to be an input pin
    r &= ~(0x7 << 6);

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 2
    *GPFSEL2 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 22. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. We
    // will pull down the pin using an external resistor connected to ground.

    // Disable internal pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 22 to
    // clock in the control signal for GPIO pin 22. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 |= (0x1 << 22);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
        asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;

    // Set pin 22 to so that it generates an interrupt on a rising edge.
    // We do so by setting bit 17 in the GPIO Rising Edge Detect Enable
    // Register 0 to a 1 value (p. 97 in the Broadcom manual).
    *GPREN0 |= (0x1 << 22);

    // Enable the GPIO IRQS for ALL the GPIO pins by setting IRQ 52
    // GPIO_int[3] in the Interrupt Enable Register 2 to a 1 value.
    //20 cause; each register has access to 32 bits; so for index 2, we have access to bits
    //32-63, and so 52 is the 20th spot for index 2
    // See p. 117 in the Broadcom Peripherals Manual.
    *IRQ_ENABLE_IRQS_2 = (0x1 << 20);
}

/*
This function sets GPIO pin 4 to an output pin without any pullup or pulldown resistors.
Return is void.
*/

void init_GPIO4_to_output()
{
    register unsigned int r;


    // Get the current contents of the GPIO Function Select Register 0
    r = *GPFSEL0;

    // Clear bits 12 - 14. This is the field FSEL4, which maps to GPIO pin 4.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << (4 * 3));

    // Set the field FSEL4 to 001, which sets pin 4 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << (4 * 3));

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 0
    *GPFSEL0 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 4. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. The
    // internal pull-up and pull-down resistor isn't needed for an output pin.

    // Disable pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 4 to
    // clock in the control signal for GPIO pin 4. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 4);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
}

/*
This function sets GPIO pin 17 to an output pin without any pullup or pulldown resistors.
Return is void.
*/

void init_GPIO17_to_output()
{
    register unsigned int r;


    // Get the current contents of the GPIO Function Select Register 1
    //(access to gpio pins 10-19)
    r = *GPFSEL1;

    // Clear bits 21 - 23. This is the field FSEL17, which maps to GPIO pin 17.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << (7*3));

    // Set the field FSEL17 to 001, which sets pin 17 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << (7*3));

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 0
    *GPFSEL1 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 17. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. The
    // internal pull-up and pull-down resistor isn't needed for an output pin.

    // Disable pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 4 to
    // clock in the control signal for GPIO pin 17. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 17);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
}

/*
This function sets GPIO pin 27 to an output pin without any pullup or pulldown resistors.
Return is void.
*/

void init_GPIO27_to_output()
{
    register unsigned int r;


    // Get the current contents of the GPIO Function Select Register 2 (access to pins 20-29)
    r = *GPFSEL2;

    // Clear bits 21 - 23. This is the field FSEL27, which maps to GPIO pin 27.
    // We clear the bits by ANDing with a 000 bit pattern in the field.
    r &= ~(0x7 << (7*3));

    // Set the field FSEL27 to 001, which sets pin 27 to an output pin.
    // We do so by ORing the bit pattern 001 into the field.
    r |= (0x1 << (7*3));

    // Write the modified bit pattern back to the
    // GPIO Function Select Register 0
    *GPFSEL2 = r;

    // Disable the pull-up/pull-down control line for GPIO pin 27. We follow the
    // procedure outlined on page 101 of the BCM2837 ARM Peripherals manual. The
    // internal pull-up and pull-down resistor isn't needed for an output pin.

    // Disable pull-up/pull-down by setting bits 0:1
    // to 00 in the GPIO Pull-Up/Down Register
    *GPPUD = 0x0;

    // Wait 150 cycles to provide the required set-up time
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Write to the GPIO Pull-Up/Down Clock Register 0, using a 1 on bit 4 to
    // clock in the control signal for GPIO pin 17. Note that all other pins
    // will retain their previous state.
    *GPPUDCLK0 = (0x1 << 27);

    // Wait 150 cycles to provide the required hold time
    // for the control signal
    r = 150;
    while (r--) {
      asm volatile("nop");
    }

    // Clear all bits in the GPIO Pull-Up/Down Clock Register 0
    // in order to remove the clock
    *GPPUDCLK0 = 0;
}

/*
This function sets the GPIO output pin 4 to a 1 (high) level.
Return is void.
*/
void set_GPIO4()
{
	  register unsigned int r;

	  // Put a 1 into the SET4 field of the GPIO Pin Output Set Register 0
	  r = (0x1 << 4);
	  *GPSET0 = r;
}

/*
This function sets the GPIO output pin 17 to a 1 (high) level.
Return is void.
*/
void set_GPIO17()
{
      register unsigned int r;

      // Put a 1 into the SET17 field of the GPIO Pin Output Set Register 0
      r = (0x1 << 17);
      *GPSET0 = r;
}

/*
This function sets the GPIO output pin 27 to a 1 (high) level.
Return is void.
*/
void set_GPIO27()
{
      register unsigned int r;

      // Put a 1 into the SET27 field of the GPIO Pin Output Set Register 0
      r = (0x1 << 27);
      *GPSET0 = r;
}

/*
This function sets the GPIO output pin 4 to a 0 (low) level.
Return is void.
*/
void clear_GPIO4()
{
	  register unsigned int r;

	  // Put a 1 into the CLR4 field of the GPIO Pin Output Clear Register 0
	  r = (0x1 << 4);
	  *GPCLR0 = r;
}

/*
This function sets the GPIO output pin 17 to a 0 (low) level.
Return is void.
*/
void clear_GPIO17()
{
      register unsigned int r;

      // Put a 1 into the CLR17 field of the GPIO Pin Output Clear Register 0
      r = (0x1 << 17);
      *GPCLR0 = r;
}

/*
This function sets the GPIO output pin 27 to a 0 (low) level.
Return is void.
*/
void clear_GPIO27()
{
      register unsigned int r;

      // Put a 1 into the CLR27 field of the GPIO Pin Output Clear Register 0
      r = (0x1 << 27);
      *GPCLR0 = r;
}

/*
The next two functions are from our lecture notes. It handles how to system delays.
It is used for the 0.5s delay for pattern one and 0.25s delay for pattern two.
*/

/*
This function retrieves the system timer counter and
returns an unsigned long int.
*/
unsigned long getSystemTimerCounter() 
{
      unsigned int h=-1, l;
      // we must read MMIO area as two separate 32 bit reads
      h=*CHI;
      l=*CLO;
      // we have to repeat it if high word changed during read
      if(h!=*CHI) {
        h=*CHI;
        l=*CLO;
      }
      // compose long int value
      return ((unsigned long) h << 32) | l;
}

/*
The second function (timed wait) calculates our delay period and returns void.
*/
void timedWait(int delay ) 
{
        unsigned long endTime;
        endTime = getSystemTimerCounter() + delay;
        while ( endTime > getSystemTimerCounter() )
        asm volatile("nop");
}
