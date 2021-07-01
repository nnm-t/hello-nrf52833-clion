#include <stdbool.h>
#include <stdint.h>

#include "nrf_delay.h"
#include "boards.h"

int main(void)
{
    bsp_board_init(LEDS_NUMBER);

    while (true) {
        // LED0をON/OFF切替
        bsp_board_led_invert(0);
        // 50ms待機
        nrf_delay_ms(50);
    }
}
