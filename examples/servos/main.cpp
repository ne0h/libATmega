#include <util/delay.h>
#include <servo.hpp>

int main() {
    Servo s1(SERVO1A);
    Servo s2(SERVO1B);

    while(1) {
        s1.setValue(0);
        s2.setValue(1000);
		_delay_ms(1000);

		s1.setValue(500);
        s2.setValue(500);
        _delay_ms(1000);

        s1.setValue(1000);
        s2.setValue(0);
		_delay_ms(1000);

        s1.setValue(500);
        s2.setValue(500);
        _delay_ms(1000);
	}
    return 0;
}
