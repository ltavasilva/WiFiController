const char CAPTIVE_PORTAL_PAGE[] PROGMEM = R"rawliteral(
<!doctype html><html lang=pt-BR>
	<head>
		<meta charset=utf-8>
		<meta name=viewport content="width=device-width,initial-scale=1">
		<title>Registrar WiFi</title>
		<link rel=icon href=data:,>
		<style>
			:root{--b:#0b1220;--s:rgba(255,255,255,.06);--t:#fff;--m:rgba(255,255,255,.7);--r:16px}
			*{box-sizing:border-box}body{margin:0;font-family:system-ui;background:radial-gradient(800px 400px at 20% 0,#6ee7ff22,transparent),radial-gradient(800px 400px at 80% 0,#8b5cf622,transparent),var(--b);color:var(--t)}
			.p{min-height:100vh;display:grid;place-items:center;padding:16px}
			.c{width:min(520px,100%);background:var(--s);border-radius:var(--r);padding:18px;border:1px solid #ffffff22}
			h1{margin:0 0 6px;font-size:1.5rem}
			.sub{margin:0 0 12px;color:var(--m);font-size:.9rem}
			.f{margin:12px 0}
			label{font-weight:600;font-size:.9rem}
			select,input{width:100%;padding:10px;border-radius:12px;border:1px solid #ffffff22;background:#00000033;color:#fff}
			.row{display:flex;gap:8px}
			.row input{flex:1}
			button{padding:10px;border-radius:12px;border:1px solid #ffffff33;background:#ffffff22;color:#fff;font-weight:700;cursor:pointer}
			.primary{background:#6ee7ff44}
			.actions{display:flex;gap:10px;margin-top:14px}
			.status{margin-top:12px;padding:8px;border:1px dashed #ffffff33;border-radius:10px;font-size:.85rem;color:#ddd}
			small{color:var(--m);font-size:.8rem}
			@media(max-width:420px){.actions{flex-direction:column}}

			.wifi-section{
				margin-top:10px
			}

			.scan-btn{
				width:100%;
				padding:12px;
				border-radius:12px;
				border:1px solid #ffffff22;
				background:#4da3ff;
				color:#fff;
				font-weight:600;
				margin-bottom:10px;
				cursor:pointer
			}

			.wifi-list{
				max-height:215px;
				overflow-y:auto;
				border:1px solid #ffffff22;
				border-radius:12px;
				background:#00000022
			}

			.net{
				display:flex;
				justify-content:space-between;
				align-items:center;
				padding:12px;
				border-bottom:1px solid #ffffff11;
				cursor:pointer
			}

			.net:hover{background:#ffffff11}

			.net:first-child{
				background:#6ee7ff22;
				font-weight:600;
			}

			.ssid{
				font-size:14px;
				white-space:nowrap;
				overflow:hidden;
				text-overflow:ellipsis
			}

			.ssid::after{
				margin-left:6px;
				color:#6ee7ff;
				font-weight:700;
			}

			.sig{
				display:flex;
				gap:2px;
				height:12px;
				align-items:flex-end
			}

			.sig i{
				width:3px;
				background:#9ae6ff;
				display:block;
				border-radius:2px;
				animation:fade 1.6s infinite
			}

			.sig i:nth-child(1){height:3px}
			.sig i:nth-child(2){height:6px}
			.sig i:nth-child(3){height:9px}
			.sig i:nth-child(4){height:12px}

			@keyframes fade{
				0%,100%{opacity:.35}
				50%{opacity:1}
			}

			@media(max-width:480px){
				
				.title{font-size:1.4rem}
				
				.card{
					width:100%;
					margin:0;
					border-radius:14px
				}
				
				.current-net{
					margin:8px 0 14px;
					padding:8px 10px;
					border-radius:10px;
					background:#00000033;
					font-size:.9rem;
					color:#9ae6ff
				}
				
				.scan-loading{
					display:flex;
					align-items:center;
					gap:8px;
					padding:12px;
					justify-content:center;
					color:#ccc;
					font-size:14px
				}
				
				.loader{
					width:14px;
					height:14px;
					border-radius:50%;
					border:2px solid #ffffff33;
					border-top:2px solid #6ee7ff;
					animation:spin 0.8s linear infinite
				}
				
				@keyframes spin{
					to{transform:rotate(360deg)}
				}
				
				.icons{
					display:flex;
					align-items:center;
					gap:6px
				}
				
				.lock::before{
					content:"🔒";
					font-size:12px;
					opacity:.8
				}
				
				body{
					padding:10px
				}
				
			}
		</style>
	</head>

	<body>
	<main class=p>
	<section class=c>
	<h1>Registre a rede WiFi</h1>
	<p class=sub>Conecte o dispositivo à sua rede.</p>
	<div class="current-net" id="currentNet">
	Rede atual: carregando...
	</div>

	<form action=/save method=POST>
	<div class=f>
	<label for=ssid>SSID</label>
	<div class="wifi-section">

	<button type="button" class="scan-btn" onclick="scan()">Atualizar redes</button>

	<div id="wifiList" class="wifi-list">
	Toque em "Atualizar redes"
	</div>

	<input type="hidden" id="ssid" name="ssid">

	</div>
	<small>Lista de redes detectadas.</small>
	</div>

	<div class=f>
	<label for=password>Senha</label>
	<div class=row>
	<input id=password name=password type=password minlength=8 required placeholder="Digite a senha">
	<button type=button id=t>Mostrar</button>
	</div>
	<small>A senha será enviada ao dispositivo.</small>
	</div>

	<div class=actions>
	<button class="primary" type=submit>Salvar</button>
	<button type=reset>Limpar</button>
	</div>

	<div class=status id=s>Pronto para registrar.</div>

	<p><small><b>Nota:</b> redes corporativas podem não funcionar.</small></p>
	</form>
	</section>
	</main>

	<script>
	const p=document.getElementById("password"),t=document.getElementById("t"),s=document.getElementById("s"),f=document.querySelector("form");
	t.onclick=()=>{let h=p.type=="password";p.type=h?"text":"password";t.textContent=h?"Ocultar":"Mostrar"}
	f.onsubmit=()=>s.textContent="Enviando credenciais...";
	f.onreset=()=>s.textContent="Pronto para registrar.";

	function sel(name){
		document.getElementById("ssid").value=name;
		s.textContent="Rede selecionada: "+name;
	}

	async function scan(){
		loadCurrent();
		let list=document.getElementById("wifiList");
		let btn=document.querySelector(".scan-btn");
		
		btn.disabled=true;
		
		list.innerHTML='<div class="scan-loading"><div class="loader"></div>Escaneando redes...</div>';
		
		await fetch("/scan");
		
		let timer=setInterval(async ()=>{
			
			let r=await fetch("/scanResult");
			let t=await r.text();
			
			if(t=="RUNNING") return;
			
			clearInterval(timer);
			
			if(t=="NONE")
			list.innerHTML="Nenhuma rede encontrada";
			else
			list.innerHTML=t;
			
			btn.disabled=false;
			
		},500);
		
	}

	async function loadCurrent(){
		
		try{
			
			let r=await fetch("/current");
			let t=await r.text();
			
			document.getElementById("currentNet").textContent="Rede atual: "+t;
			
			}catch(e){
			
			document.getElementById("currentNet").textContent="Rede atual: desconhecida";
			
		}
		
	}

	window.onload = () => {
		let list=document.getElementById("wifiList");
		list.innerHTML='<div class="scan-loading"><div class="loader"></div>Escaneando redes...</div>';
		
		loadCurrent();
		scan();
	}
	</script>

	</body>
</html>
)rawliteral";