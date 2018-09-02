//Tools -> serialMonitor tam leci serial print 
//kable : 
//czerwony 5 V
//czarny masa
//zolty clock 

#include <Wire.h>


// idth11 **************************************
#include <idDHT11.h>
int idDHT11pin = 2; //Pin polaczenia z DHT11
int idDHT11intNumber = 1; //Numer przerwania dla DHT11 potrzebny do konstruktora

// Lib instantiate
void dht11_wrapper(); // must be declared before the lib initialization
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

// BH1750 **************************************
#include <BH1750.h>
BH1750 lightMeter;
//function declarations


//*******************************************
//***************SETUP***********************
//*******************************************
void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  delay(2000);
  
  initial_prints();
  Serial.println("11111 ");
  dht11_initialization(); //uwaga to musi byc inicjalozowane przed bh1750!! nie wiedziec czemu w odwrotnej kolejnosci dht11 nie moze przejsc linijki   while (DHT11.acquiring())
    ;
  bh1750_initialization();
}

void initial_prints()
{
    //dht11 ********************************************************
    Serial.println("idDHT11 ");
    Serial.print("LIB version: ");
    Serial.println(IDDHT11LIB_VERSION);
    Serial.println("---------------");  
    
}

void dht11_wrapper() {
  DHT11.isrCallback();
}

void bh1750_initialization(){
  //BH1750 light sensor*******************************************
    Wire.begin();
    // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  
    if (lightMeter.begin()) {
      Serial.println("BH1750 initialised");
    }
    else {
      Serial.println("Error initialising BH1750");
  }
  
}
void dht11_initialization()
{
  Serial.print("11112 ");
  DHT11.acquire();
Serial.print("11113 ");
  while (DHT11.acquiring())
    ;
  int result = DHT11.getStatus();
Serial.print("11114 ");
  switch (result)
  {
    case IDDHTLIB_OK:
      Serial.println("OK");
      break;
    case IDDHTLIB_ERROR_CHECKSUM:
      Serial.println("Error\n\r\tChecksum error");
      break;
    case IDDHTLIB_ERROR_ISR_TIMEOUT:
      Serial.println("Error\n\r\tISR Time out error");
      break;
    case IDDHTLIB_ERROR_RESPONSE_TIMEOUT:
      Serial.println("Error\n\r\tResponse time out error");
      break;
    case IDDHTLIB_ERROR_DATA_TIMEOUT:
      Serial.println("Error\n\r\tData time out error");
      break;
    case IDDHTLIB_ERROR_ACQUIRING:
      Serial.println("Error\n\r\tAcquiring");
      break;
    case IDDHTLIB_ERROR_DELTA:
      Serial.println("Error\n\r\tDelta time to small");
      break;
    case IDDHTLIB_ERROR_NOTSTARTED:
      Serial.println("Error\n\r\tNot started");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }

}

//*******************************************
//***************LOOP************************
//*******************************************

void loop() {

  Serial.print("Humidity (%): ");
  Serial.println(DHT11.getHumidity(), 2);

  Serial.print("Temperature (oC): ");
  Serial.println(DHT11.getCelsius(), 2);

  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);

  delay(2000);

}
