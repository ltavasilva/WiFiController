#ifndef WIFI_CONTROLLER_H
	#define WIFI_CONTROLLER_H
	
	#include <functional>
	#include <vector>
	
	#if defined(ESP8266)
		#include <ESP8266WiFi.h>
		#include <ESP8266WebServer.h>
		#include <ESP8266mDNS.h>
		#elif defined(ESP32)
		#include <WiFi.h>
		#include <WebServer.h>
		#include <ESPmDNS.h>
	#endif
	
	#include <DNSServer.h>
	
	class WiFiController {
		
		public:
		
		using Callback = std::function<void()>;
		
		WiFiController();
		
		void loop();
		
		void startMDNS();
		
		void beginSTA(const char* ssid,const char* pass=nullptr);
		
		void beginAP(const char* name,const char* pass=nullptr);
		
		void beginCaptivePortal();
		
		void startScanNetworks();
		String generateDeviceName();
		
		
		String getStatus();
		void setDeviceName(const char* name);
		void setAPName(const char* name);
		void setHostname(const char* name);
		void enableAPFallback(bool enable);
		
		void enableHybridMode(bool enable=true);
		
		void enableAutoReconnect(bool enable=true);
		
		void onStatusConnected(Callback cb);
		void onStatusDisconnected(Callback cb);
		
		void onStaConnected(Callback cb);
		void onStaDisconnected(Callback cb);
		void onStaGotIP(Callback cb);
		
		private:
		
		wl_status_t lastStatus;
		
		bool hybridMode=false;
		bool autoReconnect=true;
		String deviceName = "ESP-Device";
		String apName = "ESP-SETUP";
		String hostName = "esp-device";
		bool apFallback = true;
		bool captiveRunning = false;
		
		unsigned long reconnectTimer=0;
		
		IPAddress apIP = IPAddress(192,168,4,1);
		IPAddress netMsk = IPAddress(255,255,255,0);
		
		void handleScan();
		void handleScanResult();
		void handleRoot();
		void handleSave();
		void redirectToRoot();
		
		String generateNetworkList();
		
		void checkStatus();
		void autoReconnectTask();
		
		std::vector<Callback> cbConnected;
		std::vector<Callback> cbDisconnected;
		
		std::vector<Callback> cbStaConnected;
		std::vector<Callback> cbStaDisconnected;
		std::vector<Callback> cbStaGotIP;
		
		void runCallbacks(const std::vector<Callback>& cbs);
		
		static WiFiController* instance;
		
		#if defined(ESP8266)
			
			static ESP8266WebServer server;
			static DNSServer dnsServer;
			
			static void onWiFiEventStatic(WiFiEvent_t event);
			void onWiFiEvent(WiFiEvent_t event);
			
			#elif defined(ESP32)
			
			static WebServer server;
			static DNSServer dnsServer;
			
			static void onWiFiEventStatic(WiFiEvent_t event,WiFiEventInfo_t info);
			void onWiFiEvent(WiFiEvent_t event,WiFiEventInfo_t info);
			
		#endif
		
	};
	
#endif