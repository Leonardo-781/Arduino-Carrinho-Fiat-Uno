#include <RF24.h>
#include <RF24_config.h>
#include <nRF24L01.h>
#include <printf.h>
#include <SPI.h>

int ledVerde = 10;
int ledVermelho = 9; 

RF24 radio(7, 8);  // CE, CSN pins
const byte address[6] = "00003";


struct JoystickData {
  int x;
  int y;
  bool button;
} data;

// Defina aqui os pinos para os motores. Estes são apenas exemplos.
int motorA1 = 2;  // Motor A forward
int motorA2 = 3;  // Motor A backward
int motorB1 = 4;  // Motor B forward
int motorB2 = 5;  // Motor B backward

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);

  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.startListening();


  // Configura os pinos dos motores como saída
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
}



void loop() {
  radio.read(&data, sizeof(JoystickData));

  if (radio.available()) {
    Serial.println("CONECTADO");

    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledVermelho, LOW);


    if (data.x > 150) {  // Mover para frente
      Serial.println("FRETE");
      digitalWrite(motorA1, LOW);
      digitalWrite(motorA2, HIGH);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
    } else if (data.x < 120) {  // Mover para trás
      Serial.println("TRAS");
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      digitalWrite(motorB1, LOW);
      digitalWrite(motorB2, HIGH);
    } else if (data.y > 200) {  // Virar à direita
      Serial.println("DIREITA");
      digitalWrite(motorA1, LOW);
      digitalWrite(motorA2, HIGH);
      digitalWrite(motorB1, LOW);
      digitalWrite(motorB2, HIGH);
    } else if (data.y < 120) {  // Virar à esquerda
      Serial.println("ESQUERDA");
      digitalWrite(motorA1, HIGH);
      digitalWrite(motorA2, LOW);
      digitalWrite(motorB1, HIGH);
      digitalWrite(motorB2, LOW);
    } else {  // Parar
      digitalWrite(motorA1, LOW);
      digitalWrite(motorA2, LOW);
      digitalWrite(motorB1, LOW);
      digitalWrite(motorB2, LOW);
    }
  } else {

    digitalWrite(ledVermelho, HIGH);
    digitalWrite(ledVerde, LOW);


    digitalWrite(motorA1, LOW);
    digitalWrite(motorA2, LOW);
    digitalWrite(motorB1, LOW);
    digitalWrite(motorB2, LOW);
    Serial.println("sem conexao");
  }


  // Controle os motores com os dados recebidos
  //controlMotors(data.x, data.y, data.button);

  // Imprime os dados recebidos no Monitor Serial para depuração

  Serial.print("X: ");
  Serial.print(data.x);
  Serial.print(" Y: ");
  Serial.print(data.y);
  Serial.print(" Button: ");
  Serial.println(data.button ? "Pressed" : "Released");


  delay(50);
}
