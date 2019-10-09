    #include "Filament_Sensor.h"
    Filament_Sensor::Filament_Sensor(byte PIN){
    	_pin = PIN;
    }
    void Filament_Sensor::Init(){
    ds = new OneWire(_pin);
    readout_to_mean = READOUT_MEAN_N;
    integration_time = INT_TIME;
    measurement_time = M_TIME;
    slope_coef = SLOPE;
    intercept = INTERCEPT;
    ADDR = OW_ADDR;
    upload_config();
    }
    void Filament_Sensor::set_measurement_time(int time){
    	measurement_time = time;
    }
    void Filament_Sensor::set_integration_time(int time){
    	integration_time = time;	
    }
    void Filament_Sensor::set_readout_to_mean(byte n){
    	readout_to_mean = n;
    }
    void Filament_Sensor::set_addr(byte addr){
    	ADDR = addr;
    }
    void Filament_Sensor::set_calibration_parameters(float _slope,byte _intercept){
      slope_coef = _slope;
      intercept = _intercept;
    }
	   int Filament_Sensor::readSpeed_X(bool raw){
     if(data[8] != 1){
      int16_t raw_1 = (data[2] << 8) | data[0];
      if(raw) raw_1 = (data[4] << 8) | data[5];
      //if(raw_1 > 1000) raw_1 = -1*(4095-raw_1);
      return(slope_coef*raw_1*60+intercept);
       }else{
        #ifdef SERIAL_FILAMENT_DEBUG
        Serial.println("Comm error");
        #endif
        return(-999);
      }
	 }
    int Filament_Sensor::readSpeed_Y(bool raw){
      if(data[8] != 1){
      int16_t raw_1 = (data[3] << 8) | data[1];
      if(raw) raw_1 = (data[6] << 8) | data[7];
      //if(raw_1 > 1000) raw_1 = -1*(4095-raw_1);
      return(slope_coef*raw_1*60+intercept);
      }else{
        #ifdef SERIAL_FILAMENT_DEBUG
        Serial.println("Comm error");
        #endif
        return(-999);
      }
    }

    bool Filament_Sensor::readData(){
    	if ( !ds->search(addr)) {
    		ds->reset_search();
    		delay(50);
        #ifdef SERIAL_FILAMENT_DEBUG
        Serial.println("No sensor detected!");
    		#endif
        return(false);
  		}

  		if (OneWire::crc8(addr, 7) != addr[7]) {
    	  	#ifdef SERIAL_FILAMENT_DEBUG
          Serial.println("CRC is not valid!");
    		#endif
        ds->reset_search();
    		delay(50);
      	  return(false);
      	}

      	if(addr[0] != ADDR){
		    #ifdef SERIAL_FILAMENT_DEBUG
        Serial.println("Comm error no sensor detected!");
		    #endif
        ds->reset_search();
    		delay(50);
		    return(false);
      	}
        delay(50);
  		  ds->reset();
  		  ds->select(addr);    
  		  ds->write(0xBE);           // Read Scratchpad
		  for (byte i = 0; i < 9; i++) {  // we need 9 bytes
		    data[i] = ds->read();
		  }
		  ds->reset_search();
		  return(true);
    }

    bool Filament_Sensor::upload_config(){
    	if ( !ds->search(addr)) {
    		ds->reset_search();
        #ifdef SERIAL_FILAMENT_DEBUG
        Serial.println("Config uplaod fail");
    		#endif
        return(false);
  		}
  		if(addr[0] == ADDR){
  		ds->reset();
  		ds->select(addr);
  		ds->write(0x4E);
  		ds->write(uint8_t(measurement_time>>8));
  		ds->write(uint8_t(measurement_time));
  		ds->write(uint8_t(integration_time>>8));
  		ds->write(uint8_t(integration_time));
  		ds->write(readout_to_mean);
  		ds->reset_search();
      #ifdef SERIAL_FILAMENT_DEBUG
      Serial.println("Config uploaded!");
  		#endif
      return(true);
  		}else{
		  	#ifdef SERIAL_FILAMENT_DEBUG
		    	Serial.println("Comm error no sensor detected!");
		    #endif
		    return(false);
  		}
    }

    bool Filament_Sensor::set_resolution(byte X_res,byte Y_res){
      if ( !ds->search(addr)) {
        ds->reset_search();
        #ifdef SERIAL_FILAMENT_DEBUG
        Serial.println("Resolution uplaod fail");
        #endif
        return(false);
      }
      if(addr[0] == ADDR){
      ds->reset();
      ds->select(addr);
      ds->write(0xAA);
      ds->write(X_res);
      ds->write(Y_res);
      ds->reset_search();
      #ifdef SERIAL_FILAMENT_DEBUG
      Serial.println("Resolution Set!");
      #endif
      return(true);
    }else{
      #ifdef DEBUG_FIL_SENSOR
          #ifdef SERIAL_FILAMENT_DEBUG
          Serial.println("Comm error no sensor detected!");
        #endif
          #endif
        return(false);
    }
    }




