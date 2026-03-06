![ESP8266](https://img.shields.io/badge/ESP8266-supported-blue)
![ESP32](https://img.shields.io/badge/ESP32-supported-green)
![License](https://img.shields.io/badge/license-MIT-orange)
![Version](https://img.shields.io/badge/version-1.0-brightgreen)
# WiFiController

Biblioteca para gerenciamento simplificado de WiFi em **ESP8266** e **ESP32**.

Permite criar aplicações IoT com:

- Conexão WiFi STA
- Access Point
- Captive Portal de configuração
- mDNS automático
- Fallback automático para AP
- Callbacks de eventos WiFi
- Scan inteligente de redes

Projeto criado para facilitar o desenvolvimento de dispositivos IoT robustos.

---

## ✨ Funcionalidades

✔ Conexão automática a redes WiFi  
✔ Captive Portal para configuração  
✔ Access Point integrado  
✔ mDNS automático (`device.local`)  
✔ Reconexão automática  
✔ Fallback para portal se WiFi falhar  
✔ Callbacks de eventos WiFi  
✔ Compatível com ESP8266 e ESP32  

---

## 📦 Instalação

### Arduino IDE

1. Baixe o repositório
2. Extraia para: "Documents/Arduino/libraries"
3. Reinicie a Arduino IDE

---

## 🚀 Exemplo rápido

```cpp
#include <WiFiController.h>

WiFiController wifi;

void setup(){

Serial.begin(115200);

wifi.setDeviceName("MeuESP");

wifi.beginCaptivePortal();

}

void loop(){
wifi.loop();
}```

Após iniciar:

Conecte no WiFi

MeuESP

O portal de configuração abrirá automaticamente.

🌐 Acesso via mDNS

Após conectar na rede:

http://meuesp.local


🔔 Callbacks disponíveis
wifi.onStatusConnected(callback);
wifi.onStatusDisconnected(callback);

wifi.onStaConnected(callback);
wifi.onStaDisconnected(callback);
wifi.onStaGotIP(callback);

📂 Exemplos incluídos
Exemplo	Descrição
Basic_STA_Connection	Conexão simples WiFi
Access_Point	Modo Access Point
Captive_Portal_Setup	Portal de configuração
Hybrid_Mode	AP + STA simultâneo
AP_Fallback_Automatico	Portal automático se falhar
Callbacks_e_Eventos	Monitoramento de eventos

👨‍💻 Autor: Leandro Tavares da Silva
GitHub: https://github.com/ltavasilva