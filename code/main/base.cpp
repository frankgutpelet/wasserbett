#include "base.hpp"
#include <ArduinoJson.h>

StaticJsonDocument < 600 > base_doc;
const char base_text[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<html lang="de">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<meta name="theme-color" content="#0b1020">
<title>Embedded Template</title>
<style>
:root {
	--bg: #0b1020; --card: #141b2e; --card2: #1b243b; --line: #27324d;
	--text: #e8ecf6; --mute: #8b97b5;
	--accent: #ff8a3d; --accent2: #ffb36b;
	--ok: #34d399; --warn: #fbbf24; --bad: #f87171;
}
@media (prefers-color-scheme: light) {
	:root {
		--bg: #f3f5fa; --card: #ffffff; --card2: #eef1f8; --line: #dde3f0;
		--text: #161c2e; --mute: #667290;
		--accent: #f06a0f; --accent2: #ff9a4d;
		--ok: #059669; --warn: #b45309; --bad: #dc2626;
	}
}
* { box-sizing: border-box; }
body {
	margin: 0; min-height: 100vh; color: var(--text);
	font: 16px/1.4 system-ui, -apple-system, "Segoe UI", Roboto, sans-serif;
	background: radial-gradient(900px 500px at 15% -10%, rgba(255,138,61,.14), transparent 60%), var(--bg);
}
.wrap { max-width: 560px; margin: 0 auto; padding: 20px 16px 40px; }
header { display: flex; align-items: center; justify-content: space-between; gap: 12px; margin-bottom: 18px; }
h1 { margin: 0; font-size: 1.35rem; font-weight: 700; letter-spacing: .2px; }
.ver { color: var(--mute); font-size: .8rem; margin-top: 2px; }
.card { background: var(--card); border: 1px solid var(--line); border-radius: 20px; padding: 20px; margin-bottom: 16px; }
.card h2 { margin: 0 0 14px; font-size: .8rem; font-weight: 600; text-transform: uppercase; letter-spacing: .08em; color: var(--mute); display: flex; justify-content: space-between; align-items: center; }
.pill { display: inline-flex; align-items: center; gap: 6px; font-size: .8rem; font-weight: 600; padding: 4px 10px; border-radius: 99px; background: var(--card2); color: var(--mute); text-transform: none; letter-spacing: 0; }
.pill::before { content: ""; width: 8px; height: 8px; border-radius: 50%; background: currentColor; }
.pill.ok { color: var(--ok); } .pill.warn { color: var(--warn); } .pill.bad { color: var(--bad); }

.temp { font-size: 3.4rem; font-weight: 700; line-height: 1.1; } .temp small { font-size: 1.3rem; color: var(--mute); }
.hint { color: var(--mute); font-size: .85rem; margin-top: 4px; min-height: 1.2em; }
.top { display: flex; justify-content: space-between; align-items: flex-start; gap: 12px; }

.seg { display: flex; background: var(--card2); border: 1px solid var(--line); border-radius: 14px; padding: 4px; gap: 4px; margin-top: 18px; }
.seg button { flex: 1; border: none; background: transparent; padding: 10px 0; border-radius: 10px; color: var(--mute); }
.seg button.on { background: linear-gradient(135deg, var(--accent2), var(--accent)); color: #1a0e02; }

button { font: inherit; font-weight: 600; cursor: pointer; border: 1px solid var(--line); background: var(--card2); color: var(--text); border-radius: 12px; padding: 10px 16px; transition: transform .05s, filter .15s; }
button:hover { filter: brightness(1.12); } button:active { transform: scale(.97); }
button.primary { background: linear-gradient(135deg, var(--accent2), var(--accent)); border: none; color: #1a0e02; width: 100%; margin-top: 18px; padding: 12px 16px; }

.sub { font-size: .8rem; color: var(--mute); margin: 18px 0 2px; font-weight: 600; }
.sub:first-of-type { margin-top: 0; }
.rowf { display: flex; align-items: center; justify-content: space-between; gap: 12px; padding: 8px 0; }
.rowf + .rowf { border-top: 1px solid var(--line); }
.rowf .l { flex: 1; }
.rowf .u { color: var(--mute); font-size: .8rem; width: 1.8em; flex: none; }
.step { display: flex; align-items: center; gap: 6px; flex: none; }
.step button { width: 38px; height: 38px; padding: 0; font-size: 1.2rem; line-height: 1; }
.step input[type=text] { width: 78px; flex: none; text-align: center; }

label { display: block; font-size: .85rem; color: var(--mute); margin: 12px 0 5px; }
input[type=text], input[type=password], input[type=number] { width: 100%; font: inherit; color: var(--text); background: var(--card2); border: 1px solid var(--line); border-radius: 12px; padding: 9px 10px; outline: none; }
input:focus { border-color: var(--accent); }
.grid { display: grid; grid-template-columns: 1fr 110px; gap: 10px; }

.switch { display: flex; align-items: center; justify-content: space-between; margin: 0; font-size: 1rem; color: var(--text); }
.switch input { appearance: none; -webkit-appearance: none; width: 50px; height: 30px; border-radius: 99px; background: var(--card2); border: 1px solid var(--line); position: relative; cursor: pointer; transition: background .2s; flex: none; }
.switch input::after { content: ""; position: absolute; top: 3px; left: 3px; width: 22px; height: 22px; border-radius: 50%; background: #fff; transition: transform .2s; }
.switch input:checked { background: var(--accent); border-color: var(--accent); }
.switch input:checked::after { transform: translateX(20px); }

details { margin-top: 4px; } summary { cursor: pointer; color: var(--mute); font-size: .9rem; padding: 4px 0; }
pre { white-space: pre-wrap; margin: 10px 0 0; padding: 12px; background: var(--card2); border-radius: 12px; font-size: .8rem; color: var(--text); }

#toast { position: fixed; left: 50%; bottom: 24px; transform: translate(-50%, 80px); background: var(--text); color: var(--bg); padding: 10px 18px; border-radius: 99px; font-weight: 600; font-size: .9rem; opacity: 0; transition: all .25s; pointer-events: none; }
#toast.show { transform: translate(-50%, 0); opacity: 1; }
</style>
</head>
<body>
<div class="wrap">
	<header>
		<div>
			<h1 id="headline">Embedded Template</h1>
			<div class="ver" id="version"></div>
		</div>
		<span class="pill" id="conn">verbinde&hellip;</span>
	</header>

	<section class="card">
		<h2>Temperatur <span class="pill" id="relay">--</span></h2>
		<div class="temp"><span id="temp">--</span><small> &deg;C</small></div>
		<div class="hint" id="tempHint"></div>
		<div class="seg" id="modeSeg">
			<button data-mode="OFF" onclick="setMode('OFF')">Aus</button>
			<button data-mode="AUTO" onclick="setMode('AUTO')">Auto</button>
			<button data-mode="ON" onclick="setMode('ON')">Ein</button>
		</div>
		<label class="switch" style="margin-top:16px">Solarfreigabe <input type="checkbox" id="allowSw" onchange="setAllow(this.checked)"></label>
	</section>

	<section class="card">
		<h2>Schwellwerte</h2>
		<div class="sub">Mit Freigabe</div>
		<div class="rowf"><span class="l">Unterschreiten</span>
			<span class="step"><button onclick="nudge('inTempMin',-0.5)">&minus;</button><input type="text" inputmode="decimal" id="inTempMin" onchange="apply('inTempMin')"><button onclick="nudge('inTempMin',0.5)">+</button></span><span class="u">&deg;C</span></div>
		<div class="rowf"><span class="l">&Uuml;berschreiten</span>
			<span class="step"><button onclick="nudge('inTempMax',-0.5)">&minus;</button><input type="text" inputmode="decimal" id="inTempMax" onchange="apply('inTempMax')"><button onclick="nudge('inTempMax',0.5)">+</button></span><span class="u">&deg;C</span></div>
		<div class="sub">Ohne Freigabe</div>
		<div class="rowf"><span class="l">Unterschreiten</span>
			<span class="step"><button onclick="nudge('inTempMinForce',-0.5)">&minus;</button><input type="text" inputmode="decimal" id="inTempMinForce" onchange="apply('inTempMinForce')"><button onclick="nudge('inTempMinForce',0.5)">+</button></span><span class="u">&deg;C</span></div>
		<div class="rowf"><span class="l">&Uuml;berschreiten</span>
			<span class="step"><button onclick="nudge('inTempMaxForce',-0.5)">&minus;</button><input type="text" inputmode="decimal" id="inTempMaxForce" onchange="apply('inTempMaxForce')"><button onclick="nudge('inTempMaxForce',0.5)">+</button></span><span class="u">&deg;C</span></div>
		<div class="sub">Regler</div>
		<div class="rowf"><span class="l">Hysterese</span>
			<span class="step"><button onclick="nudge('inTreshold',-0.1)">&minus;</button><input type="text" inputmode="decimal" id="inTreshold" onchange="apply('inTreshold')"><button onclick="nudge('inTreshold',0.1)">+</button></span><span class="u">K</span></div>
		<div class="rowf"><span class="l">Kalibrierung</span>
			<span class="step"><button onclick="nudge('inCalibrate',-0.1)">&minus;</button><input type="text" inputmode="decimal" id="inCalibrate" onchange="apply('inCalibrate')"><button onclick="nudge('inCalibrate',0.1)">+</button></span><span class="u">K</span></div>
	</section>

	<section class="card">
		<h2>MQTT <span class="pill" id="mqttPill">aus</span></h2>
		<form id="mqttForm" onsubmit="return saveMqtt(event)">
			<label class="switch">MQTT aktivieren <input type="checkbox" id="mEnabled"></label>
			<label for="mHost">Broker</label>
			<div class="grid">
				<input type="text" id="mHost" placeholder="192.168.1.10" autocomplete="off" maxlength="63">
				<input type="number" id="mPort" placeholder="1883" min="1" max="65535">
			</div>
			<label for="mUser">Benutzer (optional)</label>
			<input type="text" id="mUser" autocomplete="off" maxlength="31">
			<label for="mPass">Passwort (leer = unver&auml;ndert)</label>
			<input type="password" id="mPass" autocomplete="new-password" maxlength="63">
			<label for="mTopic">Basis-Topic</label>
			<input type="text" id="mTopic" placeholder="wasserbett" autocomplete="off" maxlength="47" oninput="topics()">
			<button class="primary" type="submit">Speichern</button>
		</form>
		<details>
			<summary>Schnittstellen</summary>
			<pre id="api"></pre>
		</details>
	</section>
</div>
<div id="toast"></div>
</body>
</html>

<script>
	var values =
	{
		"setTempMin" : "dynamic",
		"setTempMax" : "dynamic",
		"setTempMinForce" : "dynamic",
		"setTempMaxForce" : "dynamic",
		"outSetTempMin" : "dynamic",
		"outSetTempMax" : "dynamic",
		"outSetTempMinForce" : "dynamic",
		"outSetTempMaxForce" : "dynamic",
		"outTreshold" : "dynamic",
		"outCalibrate" : "dynamic",
		"temp" : "dynamic",
		"status" : "dynamic",
		"allow" : "dynamic",
		"headline" : "dynamic",
		"version" : "dynamic",
		"treshold" : "dynamic",
		"calibrate" : "dynamic",
		"mode" : "dynamic",
		"mqtt" : "dynamic",
	}
	// Eingabefeld -> Name im Submit-JSON / Name in values
	var fields =
	{
		"inTempMin" : ["outSetTempMin", "setTempMin"],
		"inTempMax" : ["outSetTempMax", "setTempMax"],
		"inTempMinForce" : ["outSetTempMinForce", "setTempMinForce"],
		"inTempMaxForce" : ["outSetTempMaxForce", "setTempMaxForce"],
		"inTreshold" : ["outTreshold", "treshold"],
		"inCalibrate" : ["outCalibrate", "calibrate"]
	}
	var editUntil = 0
	var nudgeTimers = {}
	function $(id) { return document.getElementById(id) }

	function toast(msg)
	{
		var t = $("toast")
		t.textContent = msg
		t.classList.add("show")
		clearTimeout(t.timer)
		t.timer = setTimeout(function() { t.classList.remove("show") }, 2200)
	}

	function setPill(el, cls, text)
	{
		el.className = "pill " + cls
		el.textContent = text
	}

	function post(obj)
	{
		editUntil = Date.now() + 3000
		return fetch("submit", {
			method: "POST",
			headers: {"Content-Type": "application/x-www-form-urlencoded"},
			body: "action=" + encodeURIComponent(JSON.stringify(obj))
		}).then(function(r)
		{
			if (r.status == 400) throw new Error("Ungültiger Wert")
			if (!r.ok) throw new Error("Senden fehlgeschlagen")
			editUntil = 0
			updateValues()
		}).catch(function(e)
		{
			editUntil = 0
			toast(e.message || "Senden fehlgeschlagen")
			updateValues()
		})
	}

	function apply(id)
	{
		var v = $(id).value.trim().replace(",", ".")
		if (v === "") return
		var o = {}
		o[fields[id][0]] = v
		post(o)
	}

	function nudge(id, d)
	{
		var cur = parseFloat($(id).value.replace(",", "."))
		if (isNaN(cur)) cur = parseFloat(values[fields[id][1]]) || 0
		$(id).value = (Math.round((cur + d) * 100) / 100).toFixed(Math.abs(d) < 0.5 ? 1 : 2)
		editUntil = Date.now() + 3000
		clearTimeout(nudgeTimers[id])
		nudgeTimers[id] = setTimeout(function() { apply(id) }, 500)
	}

	function setMode(m) { post({"mode": m}) }
	function setAllow(on) { post({"allow": on ? "On" : "Off"}) }

	function show()
	{
		document.title = values.headline
		$("headline").textContent = values.headline
		$("version").textContent = values.version

		var t = parseFloat(values.temp)
		var bad = isNaN(t) || t < -100
		$("temp").textContent = bad ? "--" : t.toFixed(1)
		$("tempHint").textContent = bad ? "Kein Temperatursensor gefunden" : ""

		var s = String(values.status)
		if (s.indexOf("WAIT") == 0) setPill($("relay"), "warn", "Relais wartet" + s.substring(4))
		else if (s == "ON") setPill($("relay"), "ok", "Relais an")
		else setPill($("relay"), "", "Relais aus")

		var btns = $("modeSeg").children
		for (var i = 0; i < btns.length; i++)
		{
			btns[i].classList.toggle("on", btns[i].getAttribute("data-mode") == values.mode)
		}

		if (Date.now() > editUntil)
		{
			$("allowSw").checked = (values.allow == "On")
			for (var id in fields)
			{
				if (document.activeElement !== $(id) && values[fields[id][1]] !== undefined)
				{
					$(id).value = values[fields[id][1]]
				}
			}
		}

		var m = values.mqtt
		if (m == "verbunden") setPill($("mqttPill"), "ok", "verbunden")
		else if (m == "getrennt") setPill($("mqttPill"), "warn", "getrennt")
		else setPill($("mqttPill"), "", "aus")
	}

	function updateValues()
	{
		fetch("getValues", {cache: "no-store"}).then(function(r) { return r.text() }).then(function(text)
		{
			text.split(";").forEach(function(pair)
			{
				var i = pair.indexOf(":")
				if (i > 0) values[pair.slice(0, i)] = pair.slice(i + 1)
			})
			setPill($("conn"), "ok", "online")
			show()
		}).catch(function()
		{
			setPill($("conn"), "bad", "offline")
		})
	}

	function topics()
	{
		var t = $("mTopic").value || "wasserbett"
		$("api").textContent =
			"HTTP\n" +
			"  /cm?cmnd=Power on     Solarfreigabe ein\n" +
			"  /cm?cmnd=Power off    Solarfreigabe aus\n\n" +
			"MQTT lesen (retained)\n" +
			"  " + t + "/temp           Temperatur in °C\n" +
			"  " + t + "/relay          ON / OFF / WAIT n sec\n" +
			"  " + t + "/status         online / offline\n\n" +
			"MQTT lesen und setzen (<name>/set)\n" +
			"  " + t + "/tempMin        unterschreiten, mit Freigabe\n" +
			"  " + t + "/tempMax        überschreiten, mit Freigabe\n" +
			"  " + t + "/tempMinForce   unterschreiten, ohne Freigabe\n" +
			"  " + t + "/tempMaxForce   überschreiten, ohne Freigabe\n" +
			"  " + t + "/treshold       Hysterese in K\n" +
			"  " + t + "/calibrate      Kalibrierung in K\n" +
			"  " + t + "/allow          ON / OFF\n" +
			"  " + t + "/mode           OFF / AUTO / ON"
	}

	function fillMqtt(c)
	{
		$("mEnabled").checked = c.enabled
		$("mHost").value = c.host
		$("mPort").value = c.port
		$("mUser").value = c.user
		$("mPass").value = ""
		$("mPass").placeholder = c.hasPass ? "•••••• gespeichert" : ""
		$("mTopic").value = c.topic
		topics()
	}

	function loadMqtt()
	{
		fetch("mqtt", {cache: "no-store"}).then(function(r) { return r.json() }).then(fillMqtt).catch(function() { setTimeout(loadMqtt, 2000) })
	}

	function saveMqtt(ev)
	{
		ev.preventDefault()
		var p = new URLSearchParams()
		p.set("enabled", $("mEnabled").checked ? "1" : "0")
		p.set("host", $("mHost").value.trim())
		p.set("port", $("mPort").value)
		p.set("user", $("mUser").value)
		p.set("pass", $("mPass").value)
		p.set("topic", $("mTopic").value.trim())
		fetch("mqtt", {
			method: "POST",
			headers: {"Content-Type": "application/x-www-form-urlencoded"},
			body: p.toString()
		}).then(function(r)
		{
			if (!r.ok) return r.text().then(function(t) { throw new Error(t) })
			return r.json()
		}).then(function(c)
		{
			fillMqtt(c)
			toast("MQTT gespeichert")
		}).catch(function(e)
		{
			toast(e.message || "Speichern fehlgeschlagen")
		})
		return false
	}

	topics()
	loadMqtt()
	updateValues()
	setInterval(updateValues, 1000)
</script>
)rawliteral";

base::base(ESP8266WebServer* server)
{
	this->server = server;
	this->submit_UserCallback = (void(*)()) NULL;
	this->server->on("/submit", HTTP_POST, std::bind( & base::Submit_Callback, this));
	this->server->on("/getValues", std::bind( & base::GetAjaxValues, this));
}
void base::Submit_Callback(void)
{
	String jsonstring = this->server->arg("action");
	DeserializationError error = deserializeJson(base_doc, jsonstring);
	JsonObject obj = base_doc.as < JsonObject > ();

	if (error)
	{
	Serial.print(F("deserializeJson() failed: "));
	Serial.println(error.f_str());
	}
	else
	{
		if (obj.containsKey("setTempMin"))
		{
			this->setTempMin = obj["setTempMin"].as < String > ();
		}
		if (obj.containsKey("setTempMax"))
		{
			this->setTempMax = obj["setTempMax"].as < String > ();
		}
		if (obj.containsKey("setTempMinForce"))
		{
			this->setTempMinForce = obj["setTempMinForce"].as < String > ();
		}
		if (obj.containsKey("setTempMaxForce"))
		{
			this->setTempMaxForce = obj["setTempMaxForce"].as < String > ();
		}
		if (obj.containsKey("outSetTempMin"))
		{
			this->outSetTempMin = obj["outSetTempMin"].as < String > ();
		}
		if (obj.containsKey("outSetTempMax"))
		{
			this->outSetTempMax = obj["outSetTempMax"].as < String > ();
		}
		if (obj.containsKey("outSetTempMinForce"))
		{
			this->outSetTempMinForce = obj["outSetTempMinForce"].as < String > ();
		}
		if (obj.containsKey("outSetTempMaxForce"))
		{
			this->outSetTempMaxForce = obj["outSetTempMaxForce"].as < String > ();
		}
		if (obj.containsKey("outTreshold"))
		{
			this->outTreshold = obj["outTreshold"].as < String > ();
		}
		if (obj.containsKey("outCalibrate"))
		{
			this->outCalibrate = obj["outCalibrate"].as < String > ();
		}
		if (obj.containsKey("temp"))
		{
			this->temp = obj["temp"].as < String > ();
		}
		if (obj.containsKey("status"))
		{
			this->status = obj["status"].as < String > ();
		}
		if (obj.containsKey("allow"))
		{
			this->allow = obj["allow"].as < String > ();
		}
		if (obj.containsKey("headline"))
		{
			this->headline = obj["headline"].as < String > ();
		}
		if (obj.containsKey("version"))
		{
			this->version = obj["version"].as < String > ();
		}
		if (obj.containsKey("treshold"))
		{
			this->treshold = obj["treshold"].as < String > ();
		}
		if (obj.containsKey("calibrate"))
		{
			this->calibrate = obj["calibrate"].as < String > ();
		}
		if (obj.containsKey("mode"))
		{
			this->mode = obj["mode"].as < String > ();
		}
		if (obj.containsKey("mqtt"))
		{
			this->mqtt = obj["mqtt"].as < String > ();
		}

	}
	if (NULL != this->submit_UserCallback)
	{
		this->submit_UserCallback();
	}
}

void base::SetCallback_submit (void (*callback)(void))
{
	this->submit_UserCallback = callback;
}

void base::Set_setTempMin (String value)
{
	this->setTempMin = value;
}

String base::Get_setTempMin ( void )
{
	return this->setTempMin;
}
void base::Set_setTempMax (String value)
{
	this->setTempMax = value;
}

String base::Get_setTempMax ( void )
{
	return this->setTempMax;
}
void base::Set_setTempMinForce (String value)
{
	this->setTempMinForce = value;
}

String base::Get_setTempMinForce ( void )
{
	return this->setTempMinForce;
}
void base::Set_setTempMaxForce (String value)
{
	this->setTempMaxForce = value;
}

String base::Get_setTempMaxForce ( void )
{
	return this->setTempMaxForce;
}
void base::Set_outSetTempMin (String value)
{
	this->outSetTempMin = value;
}

String base::Get_outSetTempMin ( void )
{
	return this->outSetTempMin;
}
void base::Set_outSetTempMax (String value)
{
	this->outSetTempMax = value;
}

String base::Get_outSetTempMax ( void )
{
	return this->outSetTempMax;
}
void base::Set_outSetTempMinForce (String value)
{
	this->outSetTempMinForce = value;
}

String base::Get_outSetTempMinForce ( void )
{
	return this->outSetTempMinForce;
}
void base::Set_outSetTempMaxForce (String value)
{
	this->outSetTempMaxForce = value;
}

String base::Get_outSetTempMaxForce ( void )
{
	return this->outSetTempMaxForce;
}
void base::Set_outTreshold (String value)
{
	this->outTreshold = value;
}

String base::Get_outTreshold ( void )
{
	return this->outTreshold;
}
void base::Set_outCalibrate (String value)
{
	this->outCalibrate = value;
}

String base::Get_outCalibrate ( void )
{
	return this->outCalibrate;
}
void base::Set_temp (String value)
{
	this->temp = value;
}

String base::Get_temp ( void )
{
	return this->temp;
}
void base::Set_status (String value)
{
	this->status = value;
}

String base::Get_status ( void )
{
	return this->status;
}
void base::Set_allow (String value)
{
	this->allow = value;
}

String base::Get_allow ( void )
{
	return this->allow;
}
void base::Set_headline (String value)
{
	this->headline = value;
}

String base::Get_headline ( void )
{
	return this->headline;
}
void base::Set_version (String value)
{
	this->version = value;
}

String base::Get_version ( void )
{
	return this->version;
}
void base::Set_treshold (String value)
{
	this->treshold = value;
}

String base::Get_treshold ( void )
{
	return this->treshold;
}
void base::Set_calibrate (String value)
{
	this->calibrate = value;
}

String base::Get_calibrate ( void )
{
	return this->calibrate;
}
void base::Set_mode (String value)
{
	this->mode = value;
}

String base::Get_mode ( void )
{
	return this->mode;
}
void base::Set_mqtt (String value)
{
	this->mqtt = value;
}

String base::Get_mqtt ( void )
{
	return this->mqtt;
}
void base::Render( void )
{
	this->server->send_P( 200, "text/html; charset=utf-8", base_text);
}
void base::GetAjaxValues( void )
{
	String message = "setTempMin:" + this->setTempMin + ";setTempMax:" + this->setTempMax + ";setTempMinForce:" + this->setTempMinForce + ";setTempMaxForce:" + this->setTempMaxForce + ";outSetTempMin:" + this->outSetTempMin + ";outSetTempMax:" + this->outSetTempMax + ";outSetTempMinForce:" + this->outSetTempMinForce + ";outSetTempMaxForce:" + this->outSetTempMaxForce + ";outTreshold:" + this->outTreshold + ";outCalibrate:" + this->outCalibrate + ";temp:" + this->temp + ";status:" + this->status + ";allow:" + this->allow + ";headline:" + this->headline + ";version:" + this->version + ";treshold:" + this->treshold + ";calibrate:" + this->calibrate + ";mode:" + this->mode + ";mqtt:" + this->mqtt;
	this->server->send(200, "text/plain", message);
}
