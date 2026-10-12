#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <TimerEvent.h>
#include <ArduinoJson.h>
#include "base.hpp"
#include "Logger.hpp"
#include "OneWire.h"
#include <DallasTemperature.h>
#include "Save.hpp"
#include "regulator.hpp"
#include "average.hpp"
#include "Mqtt.hpp"

#define REL_STATUS_FAILSAVE true

const char* ssid = "WMOSKITO";
const char* password = ".ubX54bVSt#vxW11m.";
const char* myhostname = "WasserbettSabrina";

const char* title = "Temperatursteuerung";
const unsigned int timerOnePeriod = 1000;
const unsigned int relOffBreakSeconds = 2; //2 minutes
unsigned int relOffBreakCounter = 0;
double lastTemp = 0.0;
int dallasErrorCnt = 0;
int relaisPin = 16; //D0 @ nodeMCU
regulator tempRegulator(0,0,0,0,0);
String fwVersion = "Version 2.4";
ESP8266WebServer server(80);
base indexPage(&server);
Logger* logger = Logger::instance();
TimerEvent timerOne;
bool relStatus = false;
bool allow = false;
SaveEEprom storage;
MqttBridge mqtt;

// IDs der MQTT-Werte (<topic>/<name>)
int mqttTemp, mqttRelay, mqttMode, mqttAllow;
int mqttTempMin, mqttTempMax, mqttTempMinForce, mqttTempMaxForce;
int mqttTreshold, mqttCalibrate;

enum e_mode {
  ON,
  OFF,
  AUTO
};

e_mode switchMode = OFF;

OneWire oneWire(4); //D2 @ nodeMCU
DallasTemperature sensors(&oneWire);
average avg(10, 0.2);

// Zahl aus Text lesen (Komma erlaubt), nur im Bereich lo..hi
bool parseNumber(String s, double* out, double lo, double hi)
{
  s.trim();
  s.replace(',', '.');
  if (s.length() == 0)
  {
    return false;
  }
  char* end = NULL;
  double v = strtod(s.c_str(), &end);
  if (end == s.c_str() || *end != 0 || isnan(v) || v < lo || v > hi)
  {
    return false;
  }
  *out = v;
  return true;
}

// "on", "1", "true" -> true; "off", "0", "false" -> false
bool parseOnOff(String s, bool* out)
{
  s.trim();
  s.toLowerCase();
  if (s == "on" || s == "1" || s == "true")
  {
    *out = true;
    return true;
  }
  if (s == "off" || s == "0" || s == "false")
  {
    *out = false;
    return true;
  }
  return false;
}

// Einstellung setzen: gemeinsamer Weg fuer Weboberflaeche und MQTT (<name>/set)
// true = angenommen
bool applySetting(const String& name, const String& value)
{
  double v;
  bool b;

  if (name == "tempMin")
  {
    if (!parseNumber(value, &v, -50, 120)) return false;
    storage.Set_tempMin(v);
    tempRegulator.setTempMin(v);
  }
  else if (name == "tempMax")
  {
    if (!parseNumber(value, &v, -50, 120)) return false;
    storage.Set_tempMax(v);
    tempRegulator.setTempMax(v);
  }
  else if (name == "tempMinForce")
  {
    if (!parseNumber(value, &v, -50, 120)) return false;
    storage.Set_tempMinForce(v);
    tempRegulator.setTempMinForce(v);
  }
  else if (name == "tempMaxForce")
  {
    if (!parseNumber(value, &v, -50, 120)) return false;
    storage.Set_tempMaxForce(v);
    tempRegulator.setTempMaxForce(v);
  }
  else if (name == "treshold")
  {
    if (!parseNumber(value, &v, 0, 20)) return false;
    storage.Set_treshold(v);
    tempRegulator.setTreshold(v);
  }
  else if (name == "calibrate")
  {
    if (!parseNumber(value, &v, -20, 20)) return false;
    storage.Set_calibrate(v);
  }
  else if (name == "allow")
  {
    if (!parseOnOff(value, &b)) return false;
    if (b != allow)
    {
      allow = b;
      indexPage.Set_allow(b ? "On" : "Off");
      if (!b)
      {
        relStatus = false;
      }
    }
  }
  else if (name == "mode")
  {
    String m = value;
    m.trim();
    m.toUpperCase();
    if (m == "OFF") switchMode = OFF;
    else if (m == "AUTO") switchMode = AUTO;
    else if (m == "ON") switchMode = ON;
    else return false;
    indexPage.Set_mode(m);
  }
  else
  {
    return false;
  }
  return true;
}

void onMqttCommand(const String& name, const String& value)
{
  applySetting(name, value);
}

// Weboberflaeche: POST /submit {"outSetTempMin":"28", "mode":"AUTO", ...}
// nur gesetzte Felder werden uebernommen
void handleSubmit()
{
  bool ok = true;
  String v;

  v = indexPage.Get_outSetTempMin();
  if (v.length()) { ok &= applySetting("tempMin", v); indexPage.Set_outSetTempMin(""); }
  v = indexPage.Get_outSetTempMax();
  if (v.length()) { ok &= applySetting("tempMax", v); indexPage.Set_outSetTempMax(""); }
  v = indexPage.Get_outSetTempMinForce();
  if (v.length()) { ok &= applySetting("tempMinForce", v); indexPage.Set_outSetTempMinForce(""); }
  v = indexPage.Get_outSetTempMaxForce();
  if (v.length()) { ok &= applySetting("tempMaxForce", v); indexPage.Set_outSetTempMaxForce(""); }
  v = indexPage.Get_outTreshold();
  if (v.length()) { ok &= applySetting("treshold", v); indexPage.Set_outTreshold(""); }
  v = indexPage.Get_outCalibrate();
  if (v.length()) { ok &= applySetting("calibrate", v); indexPage.Set_outCalibrate(""); }

  // mode und allow enthalten immer den aktuellen Zustand, bei Aenderung den neuen Wunsch
  if (!applySetting("mode", indexPage.Get_mode()))
  {
    ok = false;
    indexPage.Set_mode(OFF == switchMode ? "OFF" : (AUTO == switchMode ? "AUTO" : "ON"));
  }
  if (!applySetting("allow", indexPage.Get_allow()))
  {
    ok = false;
    indexPage.Set_allow(allow ? "On" : "Off");
  }

  server.send(ok ? 200 : 400, "text/plain", ok ? "ok" : "Bad value");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void Render()
{
  indexPage.Set_headline(String(title) + " - " + myhostname);
  indexPage.Render();
}

void Release()
{
  String cmd = server.arg("cmnd");
  Serial.println(cmd);
  if (String("Power on") == cmd)
  {
    applySetting("allow", "On");
  }
  else if (String("Power off") == cmd)
  {
    applySetting("allow", "Off");
    relStatus = false;
  }
  else
  {
    server.send(404, "text/plain", String("Bad Command: ") + cmd);
    return;
  }
  server.send(200, "text/plain", cmd);
}
void sendTemp()
{
  server.send(200, "text/plain", indexPage.Get_temp());
}

// GET /mqtt -> aktuelle MQTT-Konfiguration als JSON (ohne Passwort)
void sendMqttConfig()
{
  MqttConfig* c = storage.Mqtt();
  StaticJsonDocument < 512 > doc;
  doc["enabled"] = c->enabled;
  doc["host"] = c->host;
  doc["port"] = c->port;
  doc["user"] = c->user;
  doc["hasPass"] = (0 != c->pass[0]);
  doc["topic"] = c->topic;
  doc["state"] = mqtt.State();
  String out;
  serializeJson(doc, out);
  server.send(200, "application/json", out);
}

// POST /mqtt -> Konfiguration speichern und MQTT neu verbinden
void saveMqttConfig()
{
  MqttConfig* c = storage.Mqtt();
  String host = server.arg("host");
  String user = server.arg("user");
  String pass = server.arg("pass");
  String topic = server.arg("topic");
  int port = server.arg("port").toInt();

  host.trim();
  topic.trim();
  if (host.length() >= sizeof(c->host) || user.length() >= sizeof(c->user) ||
      pass.length() >= sizeof(c->pass) || topic.length() >= sizeof(c->topic))
  {
    server.send(400, "text/plain", "Eingabe zu lang");
    return;
  }
  if (topic.length() == 0 || topic.startsWith("/") || topic.endsWith("/") ||
      topic.indexOf('#') >= 0 || topic.indexOf('+') >= 0)
  {
    server.send(400, "text/plain", "Ungueltiges Topic");
    return;
  }
  if (port < 1 || port > 65535)
  {
    port = 1883;
  }

  c->enabled = (server.arg("enabled") == "1");
  strlcpy(c->host, host.c_str(), sizeof(c->host));
  c->port = port;
  strlcpy(c->user, user.c_str(), sizeof(c->user));
  if (pass.length() > 0)
  {
    strlcpy(c->pass, pass.c_str(), sizeof(c->pass)); // leer = unveraendert
  }
  if (0 == c->user[0])
  {
    c->pass[0] = 0; // ohne Benutzer kein Passwort
  }
  strlcpy(c->topic, topic.c_str(), sizeof(c->topic));

  storage.Save();
  mqtt.Apply();
  sendMqttConfig();
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(myhostname);
  WiFi.begin(ssid, password);
  sensors.begin();
  sensors.setWaitForConversion(false); // nicht 750 ms in loop() blockieren
  sensors.requestTemperatures();


  server.on("/", Render);
  server.on("/cm", Release);
  server.on("/temp", sendTemp);
  server.on("/mqtt", HTTP_GET, sendMqttConfig);
  server.on("/mqtt", HTTP_POST, saveMqttConfig);
  indexPage.SetCallback_submit(handleSubmit);
  indexPage.Set_mode("OFF");
  indexPage.Set_allow("Off");

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  timerOne.set(timerOnePeriod, timerOneFunc);
  indexPage.Set_headline(String(title) + " - " + myhostname);
  indexPage.Set_version(fwVersion);
  pinMode(relaisPin, OUTPUT);

  tempRegulator.setTempMin(storage.Get_tempMin());
  tempRegulator.setTempMax(storage.Get_tempMax());
  tempRegulator.setTempMinForce(storage.Get_tempMinForce());
  tempRegulator.setTempMaxForce(storage.Get_tempMaxForce());
  tempRegulator.setTreshold(storage.Get_treshold());

  mqttTemp = mqtt.Add("temp");
  mqttRelay = mqtt.Add("relay");
  mqttMode = mqtt.Add("mode");
  mqttAllow = mqtt.Add("allow");
  mqttTempMin = mqtt.Add("tempMin");
  mqttTempMax = mqtt.Add("tempMax");
  mqttTempMinForce = mqtt.Add("tempMinForce");
  mqttTempMaxForce = mqtt.Add("tempMaxForce");
  mqttTreshold = mqtt.Add("treshold");
  mqttCalibrate = mqtt.Add("calibrate");
  mqtt.SetCommandCallback(onMqttCommand);
  mqtt.Begin(storage.Mqtt(), String(myhostname) + "-" + String(ESP.getChipId(), HEX));
  indexPage.Set_mqtt(mqtt.State());
}

void loop(void) {
  server.handleClient();
  timerOne.update();
  mqtt.Loop();
}

void setRelais(bool relStatus)
{
  if (relStatus)
  {
    if (relOffBreakSeconds > relOffBreakCounter)
    {
      indexPage.Set_status(String("WAIT ") + (relOffBreakSeconds - relOffBreakCounter) + " sec");
      relOffBreakCounter++;
    }
    else
    {
      indexPage.Set_status("ON");
      digitalWrite(relaisPin, HIGH);
    }
  }
  else
  {
    relOffBreakCounter = 0;
    indexPage.Set_status("OFF");
    digitalWrite(relaisPin, LOW);
  }
}

void timerOneFunc()
{
  double temp = 0.0;
  double treshold = 0.0;
  double tempMin = 0.0;
  double tempMax = 0.0;
  double tempMinForce = 0.0;
  double tempMaxForce = 0.0;
  String tempString = "";
  temp = sensors.getTempCByIndex(0);

  // sensor anstossen fuer die naechste konvertierung
  sensors.requestTemperatures();

  if (temp != DEVICE_DISCONNECTED_C)
  {
    avg.setValue(temp + storage.Get_calibrate());
    lastTemp = temp = avg.getValue();
    dallasErrorCnt = 0;
  }
  else
  {
    temp = lastTemp;
    dallasErrorCnt++;
  }
  if (10 <= dallasErrorCnt)
  {
    lastTemp = -127;
    relStatus = REL_STATUS_FAILSAVE;
  }

  treshold = storage.Get_treshold();

  tempString = String(temp);
  indexPage.Set_temp(tempString);
  tempMax= storage.Get_tempMax();
  tempMin= storage.Get_tempMin();
  tempMaxForce= storage.Get_tempMaxForce();
  tempMinForce= storage.Get_tempMinForce();
  indexPage.Set_setTempMax(String(tempMax));
  indexPage.Set_setTempMin(String(tempMin));
  indexPage.Set_setTempMaxForce(String(tempMaxForce));
  indexPage.Set_setTempMinForce(String(tempMinForce));
  indexPage.Set_calibrate(String(storage.Get_calibrate()));
  indexPage.Set_treshold(String(storage.Get_treshold()));
  indexPage.Set_mqtt(mqtt.State());

  if (OFF == switchMode)
  {
    relStatus = false;
  }
  else if (ON == switchMode)
  {
    relStatus = true;
  }
  else if (temp != DEVICE_DISCONNECTED_C)
  {
    relStatus = tempRegulator.isOn(lastTemp, allow);
  }
  else
  {
    //FAIL SAFE nach 10 falschen Werten
  }

  Serial.println(String("tempMax:") + tempRegulator.getTempMax() + "°C; tempMin:" + tempRegulator.getTempMin() + "°C; tempMinForce:" + tempRegulator.getTempMinForce() + "; tempMaxForce:" + tempRegulator.getTempMaxForce() + "°C" + "; treshold:" + tempRegulator.getTreshold());

  setRelais(relStatus);

  // MQTT: Werte werden nur bei Aenderung gesendet
  if (temp > -100.0)
  {
    mqtt.Publish(mqttTemp, String(temp, 1));
  }
  mqtt.Publish(mqttRelay, indexPage.Get_status());
  mqtt.Publish(mqttMode, indexPage.Get_mode());
  mqtt.Publish(mqttAllow, allow ? "ON" : "OFF");
  mqtt.Publish(mqttTempMin, String(tempMin, 2));
  mqtt.Publish(mqttTempMax, String(tempMax, 2));
  mqtt.Publish(mqttTempMinForce, String(tempMinForce, 2));
  mqtt.Publish(mqttTempMaxForce, String(tempMaxForce, 2));
  mqtt.Publish(mqttTreshold, String(treshold, 2));
  mqtt.Publish(mqttCalibrate, String(storage.Get_calibrate(), 2));
}
