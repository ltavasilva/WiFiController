/*
	WiFiController Library Example
	Automatic AP Fallback
	
	Autor: Leandro Tavares da Silva
	GitHub: https://github.com/ltavasilva
	
	Descrição:
	O dispositivo tenta conectar ao WiFi.
	Caso não consiga, abre automaticamente
	o portal de configuração.
*/

#include <WiFiController.h>

WiFiController wifi;

const char* ssid="SUA_REDE";
const char* pass="SENHA_WIFI";

void setup(){
	
	Serial.begin(115200);
	
	wifi.setDeviceName("ESP-Fallback");
	
	wifi.enableAPFallback(true);
	
	wifi.beginSTA(ssid,pass);
	
}

void loop(){
	
	wifi.loop();
	
}