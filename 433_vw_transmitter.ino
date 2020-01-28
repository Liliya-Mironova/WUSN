#include <VirtualWire.h>

#define INPUT_PIN A0

int data_in[2];
int data_out[2];

// К 12 выводу передающей платы подключаем вывод DATA передатчика
void setup(void)
{
  vw_set_ptt_inverted(true); // Необходимо для DR3100
  vw_setup(2000); // Устанавливаем скорость передачи (бит/с)
}

void loop(void)
{
  //data_out[0] = analogRead(INPUT_PIN);
  data_out[0] = 2;
  //data_out[1] = 8;
  String strMsg = "z ";
  strMsg += data_out[0];
  //strMsg += " ";
  //strMsg += data_out[1];

  char msg[255];
  strMsg.toCharArray(msg, 255);

  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Ждем пока передача будет окончена
  delay(200);
}

// https://voltiq.ru/arduino-and-433-mhz-radio-module/
