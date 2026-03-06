/*
	WiFiController Library Example
	Basic STA Connection
	
	Autor: Leandro Tavares da Silva
	GitHub: https://github.com/ltavasilva
	
	Descrição:
	Conecta o dispositivo a uma rede WiFi existente
	utilizando modo STA (Station).
	
	Compatível com ESP8266 e ESP32.
*/

#include <WiFiController.h>

WiFiController wifi;

const char* ssid = "SUA_REDE";
const char* password = "SENHA_WIFI";

void setup(){
	
	Serial.begin(115200);
	
	wifi.setDeviceName("ESP-Basic");
	
	wifi.beginSTA(ssid,password);
	
	wifi.onStatusConnected([](){
		
		Serial.println("WiFi conectado!");
		Serial.print("IP: ");
		Serial.println(WiFi.localIP());
		
	});
	
	wifi.onStatusDisconnected([](){
		
		Serial.println("WiFi desconectado");
		
	});
	
}

void loop(){
	wifi.loop();
}