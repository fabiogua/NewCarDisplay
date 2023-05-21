#include "mbed.h"
#include "HardwareDisplay.h"
#include "communication/CANService.h"
Serial pcSerial(USBTX, USBRX); // Connection to PC over Serial

CANService canService(PB_8, PB_9);

// Displays
HardwareDisplay display(COMPONENT_DISPLAY_MAIN);

// Called once at bootup
int main()
{
    pcSerial.printf("Initializing CAN\n");
    canService.setSenderId(DEVICE_DISPLAY);
    canService.addComponent((ICommunication *)&display);
    while (true)
    {
        canService.run();
        display.lv_task();
        wait_ms(10);
    }
}