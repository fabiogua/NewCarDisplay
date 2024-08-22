#include "mbed.h"
#include "HardwareDisplay.h"
#include "communication/CANService.h"

CANService canService(PB_8, PB_9);

// Displays
HardwareDisplay display(COMPONENT_DISPLAY_MAIN);


// Called once at bootup
int main()
{
    printf("Initializing CAN\n");
    canService.setSenderId(DEVICE_DISPLAY);
    canService.addComponent((ICommunication *)&display);
    canService.addComponentToSendLoop((ICommunication *) &display);

        wait_ns(1000);

    while (true)
    {
        canService.run();
        display.lv_task();
        wait_ns(1000);
    }
}