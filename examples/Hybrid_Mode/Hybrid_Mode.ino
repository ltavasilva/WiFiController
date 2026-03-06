/*
	WiFiController Library Example
	Hybrid Mode (AP + STA)
	
	Autor: Leandro Tavares da Silva
	GitHub: https://github.com/ltavasilva
	
	Descrição:
	Permite que o dispositivo opere simultaneamente
	conectado a uma rede WiFi e oferecendo um AP.
*/

#include <WiFiController.h>

WiFiController wifi;

const char* ssid="SUA_REDE";
const char* pass="SENHA_WIFI";

void setup(){
	
	Serial.begin(115200);
	
	wifi.setDeviceName("ESP-Hybrid");
	
	wifi.enableHybridMode(true);
	
	wifi.beginSTA(ssid,pass);
	
}

void loop(){
	
	wifi.loop();
	
}