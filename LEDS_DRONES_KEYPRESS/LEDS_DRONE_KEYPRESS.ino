/* 
 * Projeto: Acionamento de LEDS via Rádio Controle 2.4Ghz  
 * 
 * Desenvolvido: Guilherme Lirio Tomasi de Oliveira
 * 
 * Versão: 1.0
 * 
 * Rádio: Flysky FS-i6S 
 * Receptor: Flysky FS-IA10B (10 Canais)
 * Chave usada: KEY1 (Canal 8)
 * 
 */

#include <ESP8266WiFi.h>

//===============================================================
// PINOS
//===============================================================

int CH8; //Canal 8 do Rádio
int btn_radio = 4; //GPIO0 OU D2
int leds = 5; //GPIO2 OU D1
int estado_leds = LOW; //Inicia-se com os leds desligados.
int contaclique = 0; //Contagem dos pressionamentos do botão btn_radio
unsigned long intervalo = 0; //Intervalo para calcular o tempo entre o pressionar de botão.
unsigned long tempo_atual = 0;
unsigned long ultimo_tempo = 0;  
unsigned long intervalo_led = 500; //Tempo em ms do blink dos leds.
//String estado_atual = "";

//===============================================================
// SETUP
//===============================================================

void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_OFF); //Desliga o WiFi.
  WiFi.forceSleepBegin(); //Seta o modo de economia de energia.
  
  pinMode(btn_radio, INPUT); //Setando o pino do rádio como INPUT (entrada).
  pinMode(leds, OUTPUT); //Setando o pino dos leds como OUTPUT (saída).
  digitalWrite(leds, LOW); //Setando os leds como desligados.
}

//===============================================================
// LOOP
//===============================================================

void loop() {      

  CH8 = pulseIn(btn_radio, HIGH, 50000); //Lê pulso em nível alto do canal 8 e armazena na variável CH8.

/* Usado apenas para verificar no Serial.
  Serial.println("=============");
  Serial.print("Canal 8: "); 
  Serial.println(CH8);
  Serial.print("Cliques: ");
  Serial.println(contaclique);  
  Serial.print("Estado: ");
  Serial.println(estado_atual);
  Serial.println("=============");
  Serial.println("=============");
*/

  //Se o btn_radio for pressionado, ele conta +1 e ativa a tempo.
  if(CH8 > 1500){
    contaclique++;
    delay(250);      
    intervalo = millis();    
  }

  //Se passar mais que 1 segundos e menos que 2 segundos entre os pressionar do botão (btn_radio), ele volta a zero e precisa pressionar duas ou mais vezes novamente.     
  if((millis() > intervalo + 500) && (millis() < intervalo + 1000) && (contaclique != 2)){
    contaclique = 0;
  }

  //Se a variavel contaclique chegar a 1 (no intervalo inferior a 1 segundo), acende os leds do drone.
  if(contaclique == 1){
    digitalWrite(leds, HIGH);
    //estado_atual = "Leds acesos";  
  }
  
  //Se a variavel contaclique chegar a 2 (no intervalo inferior a 1 segundo), liga o blink (piscando).
  if(contaclique == 2){
    tempo_atual = millis();       
    if(tempo_atual - ultimo_tempo >= intervalo_led){
      ultimo_tempo = tempo_atual;
      //estado_atual = "BLINK";
      if(estado_leds == LOW){
        estado_leds = HIGH;
      } else {
        estado_leds = LOW;        
      }
      digitalWrite(leds, estado_leds);
    }  
  }

  //Se a variavel contaclique chegar a 3, (no intervalo inferior a 1 segundo), apaga os leds do drone.
  if(contaclique == 3){
    digitalWrite(leds, LOW);
    //estado_atual = "Leds apagados";  
  }

}
