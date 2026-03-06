/*
	WiFiController Library Example
	Callbacks e Eventos WiFi
	
	Autor: Leandro Tavares da Silva
	GitHub: https://github.com/ltavasilva
	
	Descrição:
	Demonstra o uso dos callbacks para monitorar
	eventos de conexão WiFi.
*/

#include <WiFiController.h>

WiFiController wifi;

void setup(){
	
	Serial.begin(115200);
	
	wifi.setDeviceName("ESP-Callbacks");
	
	wifi.beginCaptivePortal();
	
	wifi.onStaConnected([](){
		
		Serial.println("STA conectada ao AP");
		
	});
	
	wifi.onStaDisconnected([](){
		
		Serial.println("STA desconectada");
		
	});
	
	wifi.onStaGotIP([](){
		
		Serial.print("IP obtido: ");
		Serial.println(WiFi.localIP());
		
	});
	
	wifi.onStatusConnected([](){
		
		Serial.println("Status geral: conectado");
		
	});
	
	wifi.onStatusDisconnected([](){
		
		Serial.println("Status geral: desconectado");
		
	});
	
}

void loop(){
	
	wifi.loop();
	
}