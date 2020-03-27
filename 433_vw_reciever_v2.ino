#include <VirtualWire.h>
#include <EasyTransferVirtualWire.h>
#include <EEPROM.h>

#define DEVICE_ID 1
#define DESTINATION_ID DEVICE_ID+1
#define RECEIVE_PIN 11
#define TRANSMIT_PIN 12
#define LED_PIN 13

//create object
EasyTransferVirtualWire ET; 
char buf[120];

struct SEND_DATA_STRUCTURE {
  // должна быть определена одинаково на приёмнике и передатчике
  // размер структуры не должен превышать 26 байт (ограничение VirtualWire)
  unsigned int device_id;
  unsigned int destination_id;    
  unsigned int packet_id;
  //byte command;
  int data;
};

SEND_DATA_STRUCTURE mydata;

// функции для записи данных типа unsigned int в EEPROM
void EEPROMWriteInt(int p_address, unsigned int p_value) {
    byte lowByte = ((p_value >> 0) & 0xFF);
    byte highByte = ((p_value >> 8) & 0xFF);
    EEPROM.write(p_address, lowByte);
    EEPROM.write(p_address + 1, highByte);
}

unsigned int EEPROMReadInt(int p_address) {
    byte lowByte = EEPROM.read(p_address);
    byte highByte = EEPROM.read(p_address + 1);
    return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(9600); // Debugging only

    ET.begin(details(mydata));
    // Initialise the IO and ISR
    vw_set_rx_pin(RECEIVE_PIN);
    vw_setup(2000);      // Скорость приёма
    vw_rx_start();       // Запуск режима приёма
    
    // Device ID
    Serial.print("Getting Device ID... ");
    Serial.println(DEVICE_ID);
    EEPROMWriteInt(0, DEVICE_ID);
}

void loop() {
    if(ET.receiveData()) { // получили пакет данных, обрабатываем
        digitalWrite(LED_PIN, HIGH);
        Serial.print("Got: ");
        Serial.print("Device ID: ");
        Serial.print(mydata.device_id);
        Serial.print(" Destination ID: ");
        Serial.print(mydata.destination_id);        
        Serial.print(" Packet ID: ");
        Serial.print(mydata.packet_id);
        //Serial.print(" Command: ");
        //Serial.print(mydata.command);
        Serial.print(" Data: ");
        Serial.print(mydata.data);
        Serial.println();
        digitalWrite(LED_PIN, LOW);
    }
}
