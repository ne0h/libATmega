#include <util/delay.h>

#include <led.hpp>

int main() {
    RGBLED l = RGBLED(  LEDManager::getInstance()->get(LED3),
                        LEDManager::getInstance()->get(LED2),
                        LEDManager::getInstance()->get(LED0));

    l.set(1.f, 1.f, 0.f);

    while (true) {
        
    }

    return 0;
}
