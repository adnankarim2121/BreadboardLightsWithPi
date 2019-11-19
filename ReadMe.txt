To run program, just drag over the kernel8.img file to your rasberry pi sd card.
This program will run assuming the push buttons are hooked up correctly. That is, it is on gpio pins 23 and 22.  
Please note, all busy loops have been replaced with the system timer on the ARM side. That is, the 0.25 delay, 0.5 delay and interrupts occur using the system timer, not busy loops.