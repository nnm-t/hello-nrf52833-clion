#include "cppmain.h"
#include "nrf_delay.h"
#include "boards.h"

#ifdef __cplusplus
extern "C" {
#endif

void setup(void)
{
    bsp_board_init(LEDS_NUMBER);
}

void loop(void)
{
    // LED0をON/OFF切替
    bsp_board_led_invert(0);
    // 50ms待機
    nrf_delay_ms(500);
}

#ifdef __cplusplus
}
#endif
