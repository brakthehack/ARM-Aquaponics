TEMPLATEROOT = ..

# compilation flags for gdb

CFLAGS  = -O1 -g
ASFLAGS = -g 

# object files

OBJS=  $(STARTUP) main.o \
	   f3d_led.o \
	   f3d_uart.o \
	   f3d_button.o \
	   f3d_systick.o \
	   f3d_rtc.o \
	   newlib_stubs.o \
	   queue.o \
	   stm32f30x_rcc.o \
	   stm32f30x_gpio.o \
	   stm32f30x_usart.o \
	   stm32f30x_spi.o \
	   stm32f30x_tim.o \
	   stm32f30x_misc.o

# include common make file
include $(TEMPLATEROOT)/Makefile.common
