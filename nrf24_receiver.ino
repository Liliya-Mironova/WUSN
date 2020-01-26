#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

#define INPUT_PIN A0

RF24 radio(9, 10);
int data[2];

void setup(){
    radio.begin();
    radio.setChannel(0);
    radio.setDataRate     (RF24_1MBPS);
    radio.setPALevel      (RF24_PA_HIGH);
    radio.openReadingPipe (1, 0x7878787878LL);
    radio.setRetries (1000, 2);
    radio.setAutoAck (true);
    radio.setCRCLength (1);
    
    Serial.begin(9600);
    printf_begin();
    radio.printDetails();

    radio.startListening  ();
    delay(20);
//  radio.stopListening   (); // Выключаем приёмник, если потребуется передать данные
}

void loop(){
    if (radio.available()){ // Если в буфере имеются принятые данные
        radio.read(&data, sizeof(data));
        Serial.println(data[0]);
        Serial.println(data[1]);      
    }
}
