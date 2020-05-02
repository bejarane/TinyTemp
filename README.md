# ATtiny85 OLED Thermometer
## Summary
This example shows the implementation for an OLED display, DHT22 thermometer and the usage of sleep for low power consumption.
The parameters shown are temperature, humidity and heat index.

The libraries required are:
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library/)
- [Tiny4kOLED](https://github.com/datacute/Tiny4kOLED/)

## How it works
This example requires two Arduinos connected by RF modules or jumpers crossed in a D12<->D11 fashion. Also, it is recommended to join both Arduino's grounds to stabilize 0V levels.

The procedure is as follows:
- Master Arduino
  - 1. Wait for Serial Input and pluck the CR and LF from the incoming data.
  - 2. Check if data input size is sendable.
  - 3. Encrypt data and print decrypt for testing.
  - 4. Send data.
- Slave Arduino
  - 1. Wait for incoming stream.
  - 2. Decrypt incoming stream and print original stream for testing.
  - 3. Parse decrypted variable without using method provided in AES library.

## Remarkable stuff
- The maximum size of the data string that can be sended in one stream by the RadioHead library configuration is 60 chars, for the actual driver selected. It is said in documentation that it can be tuned for a bigger size, but it is not recommended.
- The AES library uses blocks of 4x4 to encrypt the data. This limits the size of the sending array in multiples of 16. Taking into consideration the information provided above, it is not possible to send more than 48 bytes in one stream with vanilla libraries. They could be tuned to expand this capacity.
- Both devices, master and slave, print debug in Serial Console to allow data check and comparison. Those prints should be deleted for production.

## Screenshots
### master's output
![master_aes output](./master_test.png)

### slave's output
![slave_aes output](./slave_test.png)
