#include <SPI.h>                           // библиотека для работы с шиной SPI
#include <nRF24L01.h>                      // файл настроек из библиотеки RF24
#include <RF24.h>                          // библиотека для работы с nRF24L01+
#include <printf.h>

#define INPUT_PIN A0

// CE, CSN
RF24 radio(9, 10);                         // Создаём объект radio для работы с библиотекой RF24, указывая номера выводов nRF24L01+ (CE, CSN)
int data[2];                               // Создаём массив для приёма данных

void setup(){
    radio.begin();                         // Инициируем работу nRF24L01+
    radio.setChannel(0);                   // канал передачи данных (от 0 до 127), 5 - значит передача данных осуществляется на частоте 2,405 ГГц (на одном канале может быть только 1 приёмник и до 6 передатчиков)
    radio.setDataRate(RF24_1MBPS);         // скорость передачи данных (RF24_250KBPS, RF24_1MBPS, RF24_2MBPS), RF24_1MBPS - 1Мбит/сек
    radio.setPALevel(RF24_PA_HIGH);        // мощность передатчика (RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_HIGH=-6dBm, RF24_PA_MAX=0dBm)
    radio.openWritingPipe(0x7878787878LL); // Открываем pipe с идентификатором 0x1234567890 для передачи данных (на одном канале может быть открыто до 6 разных труб, которые должны отличаться только последним байтом идентификатора)
    radio.setRetries(2, 3);
    radio.setAutoAck(true);
    radio.setCRCLength(1);
    
    Serial.begin(9600);
    printf_begin();
    radio.printDetails();

    pinMode(INPUT_PIN, INPUT);
}

void loop(){                             
    data[0] = AnalogRead(INPUT_PIN);
    data[1] = 83;
    radio.write(&data, sizeof(data)); // отправляем данные из массива data, указывая сколько байт массива мы хотим отправить. Отправить данные можно с проверкой их доставки: if( radio.write(&data, sizeof(data)) ){данные приняты приёмником;}else{данные не приняты приёмником;}
    delay(300);
}
