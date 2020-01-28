#include <VirtualWire.h>

int data_in[2];
int data_out[2];

// к 11 выводу принимающей платы – любой из двух выводов DATA приёмника.
void setup()
{
  Serial.begin(9600);
  vw_set_ptt_inverted(true); // Необходимо для DR3100
  vw_setup(2000); // Задаем скорость приема
  vw_rx_start(); // Начинаем мониторинг эфира
}
 
void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN]; // Буфер для сообщения
  uint8_t buflen = VW_MAX_MESSAGE_LEN; // Длина буфера
 
  if (vw_get_message(buf, &buflen)) // Если принято сообщение
  {
    // Начинаем разбор
    int i;
    // Если сообщение адресовано не нам, выходим
    if (buf[0] != 'z')
    {
      return; 
    }
    //char command = buf[2]; // Команда находится на индексе 2
    // Числовой параметр начинается с индекса 4
    i = 2; 
    data_in[0] = 0;
    // Поскольку передача идет посимвольно, то нужно преобразовать набор символов в число
    while (buf[i] != ' ')
    {
      data_in[0] *= 10;
      data_in[0] += buf[i] - '0';
      i++;
    }
    //Serial.print(command);
    //Serial.print(" ");
    Serial.println(data_in[0]);
  }
}
