#include "WiFiController.h"
#include "captive_portal_page.h"

WiFiController* WiFiController::instance=nullptr;

#if defined(ESP8266)
	ESP8266WebServer WiFiController::server(80);
	DNSServer WiFiController::dnsServer;
	#elif defined(ESP32)
	WebServer WiFiController::server(80);
	DNSServer WiFiController::dnsServer;
#endif

WiFiController::WiFiController(){
	lastStatus=WL_IDLE_STATUS;
}

void WiFiController::setDeviceName(const char* name){
	
	deviceName = name;
	
	/* sincroniza tudo automaticamente */
	
	apName = deviceName;
	hostName = deviceName;
	
	/* limpa hostname anterior */
	
	#if defined(ESP8266)
		WiFi.hostname("");
		#elif defined(ESP32)
		WiFi.setHostname("");
	#endif
	
	/* aplica novo */
	
	#if defined(ESP8266)
		WiFi.hostname(hostName.c_str());
		#elif defined(ESP32)
		WiFi.setHostname(hostName.c_str());
	#endif
	
}

void WiFiController::setAPName(const char* name){
	apName = name;
}

void WiFiController::setHostname(const char* name){
	
	hostName = name;
	
	#if defined(ESP8266)
		WiFi.hostname("");
		WiFi.hostname(hostName.c_str());
		#elif defined(ESP32)
		WiFi.setHostname(hostName.c_str());
	#endif
	
}

void WiFiController::enableAPFallback(bool enable){
	apFallback = enable;
}

void WiFiController::startMDNS(){
	
	if(WiFi.status()!=WL_CONNECTED) return;
	
	#if defined(ESP8266)
		
		if(MDNS.begin(hostName.c_str())){
			MDNS.addService("http","tcp",80);
		}
		
		#elif defined(ESP32)
		
		if(MDNS.begin(hostName.c_str())){
			MDNS.addService("http","tcp",80);
		}
		
	#endif
	
}

void WiFiController::beginSTA(const char* ssid,const char* pass){
	
	if(hybridMode)
	WiFi.mode(WIFI_AP_STA);
	else
	WiFi.mode(WIFI_STA);
	
	#if defined(ESP8266)
		WiFi.hostname(hostName.c_str());
		#elif defined(ESP32)
		WiFi.setHostname(hostName.c_str());
	#endif
	
	WiFi.begin(ssid,pass);
	
	instance=this;
	
	WiFi.onEvent(onWiFiEventStatic);
}

void WiFiController::beginAP(const char* name,const char* pass){
	
	WiFi.mode(WIFI_AP);
	
	WiFi.softAP(name,pass);
	
	instance=this;
	
	WiFi.onEvent(onWiFiEventStatic);
}

void WiFiController::enableHybridMode(bool enable){
	hybridMode=enable;
}

void WiFiController::enableAutoReconnect(bool enable){
	autoReconnect=enable;
}

void WiFiController::beginCaptivePortal(){
	captiveRunning = true;
	
	WiFi.mode(WIFI_AP);
	
	WiFi.softAP(apName.c_str());
	WiFi.softAPConfig(apIP,apIP,netMsk);
	
	dnsServer.start(53,"*",apIP);
	
	startScanNetworks();   // <<< INICIA SCAN AQUI
	
	server.on("/",[this](){handleRoot();});
	server.on("/scan", [this](){handleScan();});
	server.on("/scanResult",[this](){handleScanResult();});
	server.on("/save",[this](){handleSave();});
	server.on("/current",[this](){
		
		if(WiFi.status()==WL_CONNECTED)
		server.send(200,"text/plain",WiFi.SSID());
		else
		server.send(200,"text/plain","não conectado");
		
	});
	
	server.onNotFound([this](){handleRoot();});
	
	server.begin();
}

void WiFiController::startScanNetworks(){
	WiFi.scanDelete();
	WiFi.scanNetworks(true, true); // async + show hidden
}

String WiFiController::generateNetworkList(){
	
	WiFi.scanDelete();
	int n = WiFi.scanNetworks();
	
	struct Net{
		String ssid;
		int rssi;
		bool enc;
	};
	
	std::vector<Net> nets;
	
	for(int i=0;i<n;i++){
		nets.push_back({
			WiFi.SSID(i),
			WiFi.RSSI(i),
			#ifdef ESP32
				WiFi.encryptionType(i)!=WIFI_AUTH_OPEN
				#else
				WiFi.encryptionType(i)!=AUTH_OPEN
			#endif
		});
	}
	
	std::sort(nets.begin(),nets.end(),
	[](Net&a,Net&b){return a.rssi>b.rssi;});
	
	String html;
	
	for(auto &net:nets){
		
		int level;
		
		if(net.rssi>-55)level=4;
		else if(net.rssi>-65)level=3;
		else if(net.rssi>-75)level=2;
		else if(net.rssi>-85)level=1;
		else level=0;
		
		html+="<div class='net' onclick=\"sel('";
		html+=net.ssid;
		html+="')\">";
		
		html+="<span class='ssid'>";
		html+=net.ssid;
		
		if(net.enc) html+=" 🔒";
		
		html+="</span>";
		
		html+="<span class='sig s";
		html+=level;
		html+="'></span>";
		
		html+="</div>";
	}
	
	return html;
}

void WiFiController::handleScan(){
	WiFi.scanDelete();
	WiFi.scanNetworks(true,true); // async, show_hidden
	
	server.send(200,"text/plain","OK");
	//server.send(200,"text/html",generateNetworkList());
}

void WiFiController::handleScanResult(){
	
	int n = WiFi.scanComplete();
	
	if(n == WIFI_SCAN_RUNNING){
		server.send(200,"text/plain","RUNNING");
		return;
	}
	
	if(n <= 0){
		server.send(200,"text/plain","NONE");
		return;
	}
	
	String current = WiFi.SSID();
	
	/* estrutura */
	
	struct Net{
		String ssid;
		int rssi;
		bool open;
		bool connected;
	};
	
	std::vector<Net> nets;
	
	for(int i=0;i<n;i++){
		
		Net net;
		
		net.ssid = WiFi.SSID(i);
		net.rssi = WiFi.RSSI(i);
		net.connected = (net.ssid == current);
		
		#if defined(ESP8266)
			net.open = WiFi.encryptionType(i) == ENC_TYPE_NONE;
			#else
			net.open = WiFi.encryptionType(i) == WIFI_AUTH_OPEN;
		#endif
		
		nets.push_back(net);
	}
	
	/* ordenar */
	
	std::sort(nets.begin(), nets.end(),
		[](const Net &a,const Net &b){
			
			/* conectada sempre primeiro */
			
			if(a.connected && !b.connected) return true;
			if(!a.connected && b.connected) return false;
			
			/* depois por sinal */
			
			return a.rssi > b.rssi;
		});
		
		int limit = min((int)nets.size(),5);
		
		String html="";
		
		for(int i=0;i<limit;i++){
			
			int bars;
			
			if(nets[i].rssi>-55) bars=4;
			else if(nets[i].rssi>-65) bars=3;
			else if(nets[i].rssi>-75) bars=2;
			else if(nets[i].rssi>-85) bars=1;
			else bars=0;
			
			html += "<div class='net' onclick=\"sel('";
			html += nets[i].ssid;
			html += "')\">";
			
			html += "<span class='ssid'>";
			html += nets[i].ssid;
			
			if(nets[i].connected)
			html += " ✓";
			
			html += "</span>";
			
			html += "<span class='icons'>";
			
			if(!nets[i].open)
			html += "<span class='lock'></span>";
			
			html += "<span class='sig'>";
			
			for(int b=0;b<bars;b++)
			html += "<i></i>";
			
			html += "</span></span></div>";
		}
		
		WiFi.scanDelete();
		
		server.send(200,"text/html",html);
		
}

void WiFiController::handleRoot(){
	
	String page;
	page.reserve(8000);   // evita fragmentação
	
	page = FPSTR(CAPTIVE_PORTAL_PAGE);
	
	String opt = generateNetworkList();
	page.replace("%NETWORK_LIST%", opt);
	
	server.send(200,"text/html",page);
	
}

void WiFiController::handleSave(){
	
	String ssid=server.arg("ssid");
	String pass=server.arg("password");
	
	WiFi.persistent(true);        // salva na flash
	WiFi.setAutoReconnect(true);  // reconecta automático
	
	WiFi.mode(WIFI_STA);
	
	WiFi.begin(ssid.c_str(),pass.c_str());
	
	server.send(200,"text/html","<h3>Conectando...</h3>");
}

String WiFiController::generateDeviceName(){
	
	String mac = WiFi.macAddress();
	mac.replace(":","");
	
	return "ESP-" + mac.substring(6);
}

void WiFiController::loop(){
	#if defined(ESP8266) || defined(ESP32)
		MDNS.update();
	#endif
	
	dnsServer.processNextRequest();
	
	server.handleClient();
	
	if(WiFi.scanComplete() == WIFI_SCAN_FAILED){
		startScanNetworks();
	}
	
	checkStatus();
	
	autoReconnectTask();
}

void WiFiController::checkStatus(){
	
	wl_status_t s=WiFi.status();
	
	if(s==lastStatus) return;
	
	lastStatus=s;
	
	switch(s){
		
		case WL_CONNECTED:
		startMDNS();
		runCallbacks(cbConnected);
		break;
		
		case WL_DISCONNECTED:
		runCallbacks(cbDisconnected);
		break;
		
		default:
		break;
		
	}
}

void WiFiController::autoReconnectTask(){
	
	if(!autoReconnect) return;
	
	if(WiFi.status()==WL_CONNECTED) return;
	
	if(millis()-reconnectTimer<10000) return;
	
	reconnectTimer=millis();
	
	WiFi.reconnect();
	
	/* fallback AP */
	
	if(apFallback && !captiveRunning){
		
		beginCaptivePortal();
		
		captiveRunning = true;
		
	}
}

void WiFiController::runCallbacks(const std::vector<Callback>& cbs){
	
	for(auto &cb:cbs)
	if(cb) cb();
}

void WiFiController::onStatusConnected(Callback cb){cbConnected.push_back(cb);}
void WiFiController::onStatusDisconnected(Callback cb){cbDisconnected.push_back(cb);}

void WiFiController::onStaConnected(Callback cb){cbStaConnected.push_back(cb);}
void WiFiController::onStaDisconnected(Callback cb){cbStaDisconnected.push_back(cb);}
void WiFiController::onStaGotIP(Callback cb){cbStaGotIP.push_back(cb);}

#if defined(ESP8266)
	
	void WiFiController::onWiFiEventStatic(WiFiEvent_t event){
		if(instance) instance->onWiFiEvent(event);
	}
	
	void WiFiController::onWiFiEvent(WiFiEvent_t event){
		
		switch(event){
			
			case WIFI_EVENT_STAMODE_CONNECTED:
			runCallbacks(cbStaConnected);
			break;
			
			case WIFI_EVENT_STAMODE_DISCONNECTED:
			runCallbacks(cbStaDisconnected);
			break;
			
			case WIFI_EVENT_STAMODE_GOT_IP:
			runCallbacks(cbStaGotIP);
			break;
			
			default: break;
		}
	}
	
	#elif defined(ESP32)
	
	void WiFiController::onWiFiEventStatic(WiFiEvent_t event,WiFiEventInfo_t info){
		if(instance) instance->onWiFiEvent(event,info);
	}
	
	void WiFiController::onWiFiEvent(WiFiEvent_t event,WiFiEventInfo_t info){
		
		switch(event){
			
			case ARDUINO_EVENT_WIFI_STA_CONNECTED:
			runCallbacks(cbStaConnected);
			break;
			
			case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
			runCallbacks(cbStaDisconnected);
			break;
			
			case ARDUINO_EVENT_WIFI_STA_GOT_IP:
			runCallbacks(cbStaGotIP);
			break;
			
			default: break;
		}
	}
	
#endif