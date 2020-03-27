#include <VirtualWire.h>
#include <EasyTransferVirtualWire.h>
#include <EEPROM.h> // эта библиотека нужна для работы с энерго-независимой памятью

#define DEVICE_ID 0
#define DESTINATION_ID DEVICE_ID+1
#define RECEIVE_PIN 11
#define TRANSMIT_PIN 12
#define LED_PIN 13

unsigned int count = 1;

EasyTransferVirtualWire ET; 

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
  
    ET.begin(details(mydata));
    vw_set_tx_pin(TRANSMIT_PIN); //установка пина, к которому подключен data-вход передатчика
    vw_setup(2000);        //скорость передачи
    Serial.begin(9600);
  
    Serial.print("Getting Device ID... "); 
    Serial.println(DEVICE_ID);
    EEPROMWriteInt(0, DEVICE_ID);
}

void loop() {
    mydata.device_id = DEVICE_ID;
    mydata.destination_id = DESTINATION_ID;
    mydata.packet_id = random(65535);
    //mydata.command = 0;
    mydata.data = count;
  
    digitalWrite(LED_PIN, HIGH); // включаем светодиод для отображения процесса передачи
    Serial.print("Transmitting packet ");   
    Serial.print(mydata.packet_id); 
    Serial.print(" device id ");   
    Serial.print(mydata.device_id);   
    Serial.print(" data: "); 
    Serial.print(mydata.data);
    Serial.print(" ... "); 
    ET.sendData(); // отправка данных
  
    digitalWrite(LED_PIN, LOW);
    Serial.println("DONE");
    delay(1000);
    count++;
}
