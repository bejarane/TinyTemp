#include <DHT.h>
#include <Tiny4kOLED.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


#define I_PINB PCINT3
#define I_PIN PB3
#define I_VECTOR PCINT0_vect

#define adc_disable() (ADCSRA &= ~(1<<ADEN)) //disable ADC converter


#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN,DHTTYPE);

const uint8_t width = 128;
const uint8_t height = 64;

volatile bool turned = false;


/*
*
* Interrupt handler
*
*/
ISR(PCINT_VECTOR){ //interrupt always recovers from sleep
  if( digitalRead(I_PIN) == HIGH ) {
    turned = true; //set the flag to redraw screen again
  }
}


void setup() {
  cli(); //turn off interrupts

  PCMSK |= (1 << I_PINB);
  GIMSK |= (1 << PCIE);
  pinMode(I_PIN, INPUT);

  sei(); //turn on interrupts

  adc_disable(); //save power turning off the converter
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); //set the type of sleep to deep sleep

  
  oled.begin(width, height, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br); //start oled object
  oled.setFont(FONT8X16); //this font looks better than the other one
  drawScreen();
  oled.on();
  
  delay(2000);
  dht.begin();
  
  turned = true; //flag for drawing screen
}

void sleep(){
  sleep_enable();
  sleep_cpu();
}

void loop() {
  if (turned){
    cli(); //turn off interrupts
    oled.on();

    float h = dht.readHumidity();//false read to reset dht cache
    delay(2200);//wait dht to load
    h = dht.readHumidity();
    float t = dht.readTemperature();
    float i = dht.computeHeatIndex(t,h,false);
  
  
    oled.setCursor(55, 1);
    oled.print(t);
    oled.print("~C");
    oled.setCursor(55, 3);
    oled.print(h,0);
    oled.print("%");
    oled.setCursor(55,5);
    oled.print(i);
    oled.print("~C"); //the ~ symbol was changed on the library to look like a ° degree symbol
    delay(5000); //show for 5 seconds
    oled.off();
    
    turned = false;
    sei();//attach interrupts again
  }

  sleep(); //always go to sleep at the end
  
  
}

void drawScreen() {
  //draws the frame an the main text
  
  for (uint8_t y = 0; y < 8; y++) {
    oled.setCursor(0, y);
    oled.startData();
    for (uint8_t x=0; x<128; x += 2) {
      oled.sendData(0b10101010);
      oled.sendData(0b01010101);
    }
    oled.endData();
  }

  oled.setCursor(0, 0);
  oled.startData();
  oled.sendData(0b11111111);
  oled.repeatData(0b00000101, width - 2);
  oled.sendData(0b11111111);
  oled.endData();

  for (uint8_t y = 1; y < (height - 8) / 8; y++) {
    oled.setCursor(0, y);
    oled.startData();
    oled.sendData(0b11111111);
    oled.repeatData(0b00000000, width - 2);
    oled.sendData(0b11111111);
    oled.endData();
  }

  oled.setCursor(0, (height - 8) / 8);
  oled.startData();
  oled.sendData(0b11111111);
  oled.repeatData(0b10100000, width - 2);
  oled.sendData(0b11111111);
  oled.endData();

  oled.setCursor(4, 1);
  oled.print("Temp:");
  oled.setCursor(4, 3);
  oled.print("Hum:");
  oled.setCursor(4,5);
  oled.print("Sens:");
}
