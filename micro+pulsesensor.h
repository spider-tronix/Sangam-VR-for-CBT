
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
 int sensor_pin = A0;    
 int analog_data; 
 int led_pin = 13;                   

 int heart_rate;          
             
 int time_between_beats = 600;            
boolean pulse_signal = false;    
 int beat[10];        
 int peak_value = 512;          
int trough_value = 512;        
int thresh = 525;              
 int amplitude = 100;                 
 boolean first_heartpulse = true;      
 boolean second_heartpulse = false;    
 long samplecounter = 0;
 long lastBeatTime = 0;
WiFiServer  server(80);
WiFiClient  client;
String rule;

void start(String ssid, String pass){
  WiFi.mode(WIFI_STA );
  WiFi.begin(ssid.c_str(),pass.c_str());

  Serial.println("");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (!MDNS.begin("esp8266")) {
    Serial.println ("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  server.begin();
  Serial.println("TCP server started");
// Add service to MDNS-SD
  MDNS.addService ("http", "tcp", 80);
}

bool isReqCame = false;

void CheckNewReq (){
  client = server.available ();
  if (!client) {
    return;
  }
  Serial.println("");
  Serial.println("NEW REQUEST");
  String req = client.readStringUntil('\r');
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
    Serial.print("Invalid request: ");
    Serial.println (req);
    return;
  }
  req = req.substring (addr_start + 1, addr_end);
  Serial.print("Requested Path: ");
  Serial.println(req);
  
  rule = req; 
  isReqCame = true;
  
  client.flush();
}

int interruptCall()

{ 

  cli();                                     
  analog_data = analogRead(sensor_pin);            
  samplecounter += 2;                        
  int N = samplecounter - lastBeatTime;      


  if(analog_data < thresh && N > (time_between_beats/5)*3)

    {     if (analog_data < trough_value)
      { trough_value = analog_data;

      }
}

  if(analog_data > thresh && analog_data > peak_value)

    { peak_value = analog_data;
      }                          



   if (N > 250)

  {   if ( (analog_data > thresh) && (pulse_signal == false) && (N > (time_between_beats/5)*3) )

      {       

        pulse_signal = true;         
        digitalWrite(led_pin,HIGH);
        time_between_beats = samplecounter - lastBeatTime;
        lastBeatTime = samplecounter;     



       if(second_heartpulse)

        { second_heartpulse = false;   
        for(int i=0; i<=9; i++)    

          {            

            beat[i] = time_between_beats;                     

          }

        }


        if(first_heartpulse)

        {                        

          first_heartpulse = false;
          second_heartpulse = true;
          return (0);            
                     
          }  


      word runningTotal = 0;  


      for(int i=0; i<=8; i++)

        { beat[i] = beat[i+1];
          runningTotal += beat[i];
}


      beat[9] = time_between_beats;             

      runningTotal += beat[9];   

      runningTotal /= 10;        

      heart_rate = 60000/runningTotal;

    }                      

  }




  if (analog_data < thresh && pulse_signal == true)

    {  

      digitalWrite(led_pin,LOW);
      pulse_signal = false;             
      amplitude = peak_value - trough_value;
      thresh = amplitude/2 + trough_value; 
      peak_value = thresh;        
      trough_value = thresh;

    }


  if (N > 2500)

    {                          

      thresh = 512;                     

      peak_value = 512;                 
      trough_value = 512;               
      lastBeatTime = samplecounter;     
      first_heartpulse = true;                 
      second_heartpulse = false;               
}


  
  return (heart_rate);                                

}

void waitUntilNewReq (){
  do {CheckNewReq();
        delay(100);
           Serial.print("pulse rate: ");

      //Serial.println(heart_rate);
      analog_data = analogRead(sensor_pin); 
      Serial.println(analog_data/10);
        
      //Serial.println (heart_rate );
      //heart_rate = interruptCall();
      } 
      while (!isReqCame);
  isReqCame = false;
}


String getPath(){
  return rule;
}
