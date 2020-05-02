# ATtiny85 OLED Thermometer
## Summary
This example shows the implementation for an OLED display, DHT22 thermometer and the usage of sleep for low power consumption.
The parameters shown are temperature, humidity and heat index.

The libraries required are:
- [DHT sensor library] from Adafruit (https://github.com/adafruit/DHT-sensor-library/)
- [Tiny4kOLED] from datacute (https://github.com/datacute/Tiny4kOLED/)

## How it works
![flow chart](./img/flow_chart.png)

As described above, the program basically runs on a loop but most of the time it is under the sleep state. As soons as an interrupt is triggered the program reads the DHT sensor and holds the information on screen for 5 seconds. Then it resumes the sleep state until the next interrupt.

## Circuit

Two main circuits are necessary. One circuit to program the ATtiny and another one for the thermometer itself.

![Programmer circuit](./img/sketch_aa.png)

The capacitor on the Arduino's reset pin is only necessary while programming the ATtiny. It must be taken off when loading the Arduino with the Arduino as ISP sketch.

![Thermometer circuit(./img/sketch_bb.png)

DHT sensor is connected on pin 4 of the ATtiny (pinout provided below). The SDA and SCL pins use pin 0 and 2 of the ATtiny respectively.

![ATtiny85 pinout](./img/attiny_pinout.jpg)
ATtiny85 pinout (source https://github.com/SpenceKonde/ATTinyCore)

- The maximum size of the data string that can be sended in one stream by the RadioHead library configuration is 60 chars, for the actual driver selected. It is said in documentation that it can be tuned for a bigger size, but it is not recommended.
- The AES library uses blocks of 4x4 to encrypt the data. This limits the size of the sending array in multiples of 16. Taking into consideration the information provided above, it is not possible to send more than 48 bytes in one stream with vanilla libraries. They could be tuned to expand this capacity.
- Both devices, master and slave, print debug in Serial Console to allow data check and comparison. Those prints should be deleted for production.

## Screenshots
### master's output
![master_aes output](./master_test.png)

### slave's output
![slave_aes output](./slave_test.png)
