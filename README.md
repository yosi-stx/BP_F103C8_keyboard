# project objectives:
- to create a converter from **UART** to **HID** keyboard
for sending chars with Android **USB FTDI terminal**
https://play.google.com/store/apps/details?id=de.kai_morich.serial_usb_terminal&hl=en
it takes the UART chars and convert them to HID Keyboard (named: "YG_STM32 Human interface") clicking.

### Connection Image:
![Alt text](https://github.com/yosi-stx/BP_F103C8_keyboard/blob/master/DOC/img_2025_03_15_011259_UART_to_HID_Keyboard.png)

2025_02_26__22_31 
note:

### need to use :
|                          description                                            |
|---------------------------------------------------------------------------------|
| FTDI cable to the bluePill uart inputs pins A3 and A2                           |
| FTDI cable usb side to smartPhone running serial_usb_terminal                   |
| connection of the native  bluePill micro-USB connector the the raspberryPi host |
