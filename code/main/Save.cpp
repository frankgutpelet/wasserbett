#include <cstring>
#include "Save.hpp"
#include <ESP8266WiFi.h>

static const uint32_t SAVE_MAGIC = 0x57423231; // aendern, wenn der MQTT-Teil von dataStruct sich aendert

SaveEEprom::SaveEEprom()
{
  this->Read();
}

void SaveEEprom::MqttDefaults()
{
  memset(&this->mqtt, 0, sizeof(this->mqtt));
  this->mqtt.port = 1883;
  strncpy(this->mqtt.topic, "wasserbett", sizeof(this->mqtt.topic) - 1);
}

void SaveEEprom::Read()
{
  dataStruct storage;
  
  EEPROM.begin(sizeof(storage)); 
  EEPROM.get(0, storage);
  EEPROM.end();
  this->tempMin = storage.tempMin;
  this->tempMax = storage.tempMax;
  this->tempMinForce = storage.tempMinForce;
  this->tempMaxForce = storage.tempMaxForce;
  this->treshold = storage.treshold;
  this->calibrate = storage.calibrate;
  if (storage.magic == SAVE_MAGIC)
  {
    this->mqtt = storage.mqtt;
    // Strings sicher terminieren
    this->mqtt.host[sizeof(this->mqtt.host) - 1] = 0;
    this->mqtt.user[sizeof(this->mqtt.user) - 1] = 0;
    this->mqtt.pass[sizeof(this->mqtt.pass) - 1] = 0;
    this->mqtt.topic[sizeof(this->mqtt.topic) - 1] = 0;
    if (this->mqtt.port == 0)
    {
      this->mqtt.port = 1883;
    }
  }
  else
  {
    // altes Layout (nur Schwellwerte) oder leer
    this->MqttDefaults();
  }
  Serial.println(String("EEPROM Read 0:") + this->tempMin + " 8:" + this->tempMax);
}
void SaveEEprom::Save()
{
  dataStruct storage;
    
  Serial.println(String("EEPROM Save 0:") + this->tempMin + " 8:" + this->tempMax);
  storage.tempMin = this->tempMin;
  storage.tempMax = this->tempMax;
  storage.tempMinForce = this->tempMinForce;
  storage.tempMaxForce = this->tempMaxForce;
  storage.treshold = this->treshold;
  storage.calibrate = this->calibrate;
  storage.magic = SAVE_MAGIC;
  storage.mqtt = this->mqtt;
	EEPROM.begin(sizeof(storage)); 
	EEPROM.put(0, storage); 
	EEPROM.commit();
	EEPROM.end();
  this->Read();
}
void SaveEEprom::Set_tempMin(double tempMin)
{
  if (this->tempMin != tempMin)
  {
	  this->tempMin = tempMin;
    this->Save();
  }
}
void SaveEEprom::Set_tempMax(double tempMax)
{
	if (this->tempMax != tempMax)
  {
    this->tempMax = tempMax;
    this->Save();
  }
}
void SaveEEprom::Set_tempMinForce(double tempMinForce)
{
  if (this->tempMinForce != tempMinForce)
  {
    this->tempMinForce = tempMinForce;
    this->Save();
  }
}
void SaveEEprom::Set_tempMaxForce(double tempMaxForce)
{
  if (this->tempMaxForce != tempMaxForce)
  {
    this->tempMaxForce = tempMaxForce;
    this->Save();
  }
}
void SaveEEprom::Set_treshold(double treshold)
{
  if (this->treshold != treshold)
  {
    this->treshold = treshold;
    this->Save();
  }
}
void SaveEEprom::Set_calibrate(double calibrate)
{
  if (this->calibrate != calibrate)
  {
    this->calibrate = calibrate;
    this->Save();
  }
}
double SaveEEprom::Get_tempMin()
{
	return this->tempMin;
}
double SaveEEprom::Get_tempMax()
{
	return this->tempMax;
}
double SaveEEprom::Get_tempMinForce()
{
  return this->tempMinForce;
}
double SaveEEprom::Get_tempMaxForce()
{
  return this->tempMaxForce;
}
double SaveEEprom::Get_treshold()
{
  return this->treshold;
}
double SaveEEprom::Get_calibrate()
{
  return this->calibrate;
}
MqttConfig* SaveEEprom::Mqtt()
{
  return &this->mqtt;
}
