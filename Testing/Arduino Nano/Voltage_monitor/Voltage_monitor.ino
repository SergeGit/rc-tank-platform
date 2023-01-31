/*
DC Voltmeter 
An Arduino DVM based on voltage divider concept
*/

// pin assignments
const int analogInput = 5;  // Battery measurement on A5
const int relayOutput = 5;  // Battery relay output pin on D5
//bool batteryAvailable = false;
//float vbat = 0.0;     // Caclulated battery voltage
//float vin = 0.0;      // Arduino input voltage
float R1 = 100000.0;  // resistance of R1 (100K - 99.2) -see text
float R2 = 19500.0;   // resistance of R2 (10K - 19.46) - see text!
float vPow = 4.75;    // Maximum value for the analog pin (this can be measured between GND and +5V on arduino)


void setup(){
  BatteryReadSetup(analogInput);
  RelayCtrlSetup(relayOutput);
   
  Serial.begin(9600);
  Serial.println("--------------------");
  Serial.println("DC VOLTMETER");
  Serial.print("Maximum Voltage: ");
  Serial.print((int)(vPow / (R2 / (R1 + R2))));
  Serial.println("V");
  Serial.println("--------------------");
  Serial.println("");
   
  delay(2000);
}


void loop(){
  bool relaycmd = BatteryRead(analogInput);
  RelayCtrl(relaycmd);
} 



/* functions*/

// Relay Control
void RelayCtrlSetup(int digitalpin){
  pinMode(digitalpin, OUTPUT);
  digitalWrite(digitalpin, LOW);
}

void RelayCtrl(bool cmd){
  if(cmd){
    digitalWrite(relayOutput, HIGH);  // Close relay
  } else {  
    digitalWrite(relayOutput, LOW);   // Open relay  
  }
} 

// Battery Measurement
void BatteryReadSetup(int analogpin){
  pinMode(analogpin, INPUT);
}

bool BatteryRead(int analogpin){
  bool batteryResult;
  float vin = (analogRead(analogpin) * vPow) / 1024.0;  // analog pin input voltage
  //float vin = map(analogRead(analogpin), 0, 1023, 0, vPow);  //Does not work
  float vbat = vin / (R2 / (R1 + R2));                  // calculated battery voltage
  
  // battery not connected
  if (vin<0.09){
      vbat=0.0;   //statement to quash undesired reading
      Serial.println("No battery is connected");
      batteryResult = false;
    }
  
  // identify battery based on voltage and calculate the lower limit
  float vlimit = 3*3.3;   // identify battery and calculate lower limit
  
  if (vbat>vlimit){
    batteryResult = true;
  } 
  else if (vbat <= vlimit){
    batteryResult = false;
  }

  Serial.println(vbat);
  return batteryResult;
}



