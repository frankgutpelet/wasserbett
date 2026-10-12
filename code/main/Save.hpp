#pragma once
#include <Arduino.h>
#include <ESP_EEPROM.h>

typedef struct
{
  bool enabled;
  char host[64];
  uint16_t port;
  char user[32];
  char pass[64];
  char topic[48];
} MqttConfig;

class SaveEEprom
{
  // Reihenfolge nicht aendern: die ersten 6 Werte liegen bereits so im EEPROM
  typedef struct
  {
    double tempMin;
    double tempMax;
    double tempMinForce;
    double tempMaxForce;
    double treshold;
    double calibrate;
    uint32_t magic;
    MqttConfig mqtt;
  }dataStruct;

	public:
    SaveEEprom();
	  void Save();
    void Read();
    void Set_tempMin(double tempMin);
	  void Set_tempMax(double tempMax);
    void Set_tempMinForce(double tempMinForce);
    void Set_tempMaxForce(double tempMaxForce);
    void Set_treshold(double treshold);
    void Set_calibrate(double calibrate);
	  double Get_tempMin();
	  double Get_tempMax();
    double Get_tempMinForce();
    double Get_tempMaxForce();
    double Get_treshold();
    double Get_calibrate();
    MqttConfig* Mqtt();

	private:
    void MqttDefaults();
	  double tempMin;
	  double tempMax;
    double tempMinForce;
    double tempMaxForce;
    double treshold;
    double calibrate;
    MqttConfig mqtt;
};
