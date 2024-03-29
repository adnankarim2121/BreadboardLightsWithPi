// The addresses of the GPIO registers.
//
// These are defined on page 90 - 91 of the Broadcom BCM2837 ARM Peripherals
// Manual. Note that we specify the ARM physical addresses of the
// peripherals, which have the address range 0x3F000000 to 0x3FFFFFFF.
// These addresses are mapped by the VideoCore Memory Management Unit (MMU)
// onto the bus addresses in the range 0x7E000000 to 0x7EFFFFFF.

#define MMIO_BASE       0x3F000000

#define GPFSEL0         ((volatile unsigned int *)(MMIO_BASE + 0x00200000))
#define GPFSEL1         ((volatile unsigned int *)(MMIO_BASE + 0x00200004))
#define GPFSEL2         ((volatile unsigned int *)(MMIO_BASE + 0x00200008))
#define GPFSEL3         ((volatile unsigned int *)(MMIO_BASE + 0x0020000C))
#define GPFSEL4         ((volatile unsigned int *)(MMIO_BASE + 0x00200010))
#define GPFSEL5         ((volatile unsigned int *)(MMIO_BASE + 0x00200014))
#define GPSET0          ((volatile unsigned int *)(MMIO_BASE + 0x0020001C))
#define GPSET1          ((volatile unsigned int *)(MMIO_BASE + 0x00200020))
#define GPCLR0          ((volatile unsigned int *)(MMIO_BASE + 0x00200028))
#define GPCLR1          ((volatile unsigned int *)(MMIO_BASE + 0x0020002C))
#define GPLEV0          ((volatile unsigned int *)(MMIO_BASE + 0x00200034))
#define GPLEV1          ((volatile unsigned int *)(MMIO_BASE + 0x00200038))
#define GPEDS0          ((volatile unsigned int *)(MMIO_BASE + 0x00200040))
#define GPEDS1          ((volatile unsigned int *)(MMIO_BASE + 0x00200044))
#define GPREN0          ((volatile unsigned int *)(MMIO_BASE + 0x0020004C))
#define GPREN1          ((volatile unsigned int *)(MMIO_BASE + 0x00200050))
#define GPFEN0          ((volatile unsigned int *)(MMIO_BASE + 0x00200058))
#define GPFEN1          ((volatile unsigned int *)(MMIO_BASE + 0x0020005C))
#define GPHEN0          ((volatile unsigned int *)(MMIO_BASE + 0x00200064))
#define GPHEN1          ((volatile unsigned int *)(MMIO_BASE + 0x00200068))
#define GPLEN0          ((volatile unsigned int *)(MMIO_BASE + 0x00200070))
#define GPLEN1          ((volatile unsigned int *)(MMIO_BASE + 0x00200074))
#define GPAREN0         ((volatile unsigned int *)(MMIO_BASE + 0x0020007C))
#define GPAREN1         ((volatile unsigned int *)(MMIO_BASE + 0x00200080))
#define GPAFEN0         ((volatile unsigned int *)(MMIO_BASE + 0x00200088))
#define GPAFEN1         ((volatile unsigned int *)(MMIO_BASE + 0x0020008C))
#define GPPUD           ((volatile unsigned int *)(MMIO_BASE + 0x00200094))
#define GPPUDCLK0       ((volatile unsigned int *)(MMIO_BASE + 0x00200098))
#define GPPUDCLK1       ((volatile unsigned int *)(MMIO_BASE + 0x0020009C))

/*
The next seven defines are from the lecture notes and are needed for the delays used 
in the program.
*/
#define CS             ((volatile unsigned int *)(MMIO_BASE + 0x00003000))
#define CLO             ((volatile unsigned int *)(MMIO_BASE + 0x00003004))
#define CHI             ((volatile unsigned int *)(MMIO_BASE + 0x00003008))
#define C0             ((volatile unsigned int *)(MMIO_BASE + 0x0000300c))
#define C1             ((volatile unsigned int *)(MMIO_BASE + 0x00003010))
#define C2             ((volatile unsigned int *)(MMIO_BASE + 0x00003014))
#define C3             ((volatile unsigned int *)(MMIO_BASE + 0x00003018))
