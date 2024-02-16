#include "rpi_ws281x/ws2811.h"
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdarg.h>
#include <getopt.h>


#include "rpi_ws281x/clk.h"
#include "rpi_ws281x/gpio.h"
#include "rpi_ws281x/dma.h"
#include "rpi_ws281x/pwm.h"

#define TARGET_FREQ 800000
#define DMA 10
#define GPIO_PIN 18
#define LED_COUNT 90
#define STRIP_TYPE 0x00081000 

ws2811_t ledstring =
{
    .freq = TARGET_FREQ,
    .dmanum = DMA,
    .channel =
    {
        [0] =
        {
            .gpionum = GPIO_PIN,
            .invert = 0,
            .count = LED_COUNT,
            .strip_type = STRIP_TYPE,
            .brightness = 255,
        },
        [1] =
        {
            .gpionum = 0,
            .invert = 0,
            .count = 0,
            .brightness = 0,
        },
    },
};

//gcc ledtest.c -o ledtest rpi_ws281x/ws2811.c rpi_ws281x/mailbox.c rpi_ws281x/pwm.c rpi_ws281x/pcm.c rpi_ws281x/dma.c rpi_ws281x/rpihw.c -lm

int main(int argc, char** argv)
{

    if(argc < 2){
        printf("Pass arguments\n");
        return -1;
    }

    ws2811_init(&ledstring);

    unsigned int color;
    for (int i = 1; i < argc; i++) {
        
        sscanf(argv[i], "%x", &color);
        ledstring.channel[0].leds[i-1] = color; // 0xFFfc9a44;

        //printf("Hexadecimal value %d: 0x%X\n", i, color);
    }
    ws2811_render(&ledstring);
    ws2811_fini(&ledstring);
    return 0;
}