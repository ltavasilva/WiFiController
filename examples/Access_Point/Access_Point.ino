/*
	WiFiController Library Example
	Access Point Mode
	
	Autor: Leandro Tavares da Silva
	GitHub: https://github.com/ltavasilva
	
	Descrição:
	Inicia o dispositivo como ponto de acesso WiFi (AP).
	Ideal para configuração ou redes locais isoladas.
*/

#include <WiFiController.h>

WiFiController wifi;

void setup(){
	
	Serial.begin(115200);
	
	wifi.setAPName("ESP-AccessPoint");
	
	wifi.beginAP("ESP-AccessPoint","12345678");
	
	Serial.println("Access Point iniciado");
	
	Serial.print("IP do AP: ");
	Serial.println(WiFi.softAPIP());
	
}

void loop(){
	wifi.loop();
}