/*
	WiFiController Library Example
	Captive Portal
	
	Autor: Leandro Tavares da Silva
	GitHub: https://github.com/ltavasilva
	
	Descrição:
	Cria um portal de configuração WiFi.
	O usuário conecta ao AP e escolhe a rede
	que deseja conectar o dispositivo.
*/

#include <WiFiController.h>

WiFiController wifi;

void setup(){
	
	Serial.begin(115200);
	
	wifi.setDeviceName("ESP-Setup");
	
	wifi.beginCaptivePortal();
	
	Serial.println("Portal de configuração iniciado");
	
	Serial.print("IP: ");
	Serial.println(WiFi.softAPIP());
	
}

void loop(){
	
	wifi.loop();
	
}