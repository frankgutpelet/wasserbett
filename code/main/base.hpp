#include <ESP8266WebServer.h>
#include <Arduino.h>
#pragma once
class base
{
	public:
		base (ESP8266WebServer* server);

		void Render (void);
		void GetAjaxValues (void);
		void SetCallback_submit (void (*callback)(void));
		void Set_setTempMin (String value);
		String Get_setTempMin ();
		void Set_setTempMax (String value);
		String Get_setTempMax ();
		void Set_setTempMinForce (String value);
		String Get_setTempMinForce ();
		void Set_setTempMaxForce (String value);
		String Get_setTempMaxForce ();
		void Set_outSetTempMin (String value);
		String Get_outSetTempMin ();
		void Set_outSetTempMax (String value);
		String Get_outSetTempMax ();
		void Set_outSetTempMinForce (String value);
		String Get_outSetTempMinForce ();
		void Set_outSetTempMaxForce (String value);
		String Get_outSetTempMaxForce ();
		void Set_outTreshold (String value);
		String Get_outTreshold ();
		void Set_outCalibrate (String value);
		String Get_outCalibrate ();
		void Set_temp (String value);
		String Get_temp ();
		void Set_status (String value);
		String Get_status ();
		void Set_allow (String value);
		String Get_allow ();
		void Set_headline (String value);
		String Get_headline ();
		void Set_version (String value);
		String Get_version ();
		void Set_treshold (String value);
		String Get_treshold ();
		void Set_calibrate (String value);
		String Get_calibrate ();
		void Set_mode (String value);
		String Get_mode ();
		void Set_mqtt (String value);
		String Get_mqtt ();
	private:
		void Submit_Callback(void);
		void(*submit_UserCallback)(void);
		ESP8266WebServer* server;

		String setTempMin;
		String setTempMax;
		String setTempMinForce;
		String setTempMaxForce;
		String outSetTempMin;
		String outSetTempMax;
		String outSetTempMinForce;
		String outSetTempMaxForce;
		String outTreshold;
		String outCalibrate;
		String temp;
		String status;
		String allow;
		String headline;
		String version;
		String treshold;
		String calibrate;
		String mode;
		String mqtt;
};