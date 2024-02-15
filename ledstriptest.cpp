#include <pigpio.h>
#include <pigpiod_if2.h>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

// Define GPIO pin number connected to the LED strip
const int LED_PIN = 18; // Change this to match your GPIO pin

int main() {
    // Initialize pigpio library
    if (gpioInitialise() < 0) {
        cerr << "Failed to initialize pigpio library" << endl;
        return 1;
    }

    // Set the GPIO pin connected to the LED strip as output
    gpioSetMode(LED_PIN, PI_OUTPUT);

    // Loop to blink the LED strip
    while (true) {
        // Turn on the LED strip
        gpioWrite(LED_PIN, 1);
        cout << "LED ON" << endl;

        // Wait for 1 second
        this_thread::sleep_for(chrono::seconds(1));

        // Turn off the LED strip
        gpioWrite(LED_PIN, 0);
        cout << "LED OFF" << endl;

        // Wait for 1 second
        this_thread::sleep_for(chrono::seconds(1));
    }

    // Release pigpio resources
    gpioTerminate();

    return 0;
}
