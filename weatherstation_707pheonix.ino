#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#include "FastLED.h"
#include <ESP8266WiFi.h>
#include <time.h>
#include <ArduinoJson.h>
#define NUM_LEDS 256
// Data pin that led data will be written out over
#define DATA_PIN 5

uint8_t S1 = D1;
uint8_t S2 = D2;
uint8_t S3 = D3;

CRGB leds[NUM_LEDS];

//WIFI Credentials
const char* ssid = "JADE16C";
const char* password = "Mclaren@98";

int timezone = 19080;//9 * 3600;
int dst = 1800;

////weather varaibles
WiFiClient client;
char servername[]="api.openweathermap.org";              // remote weather server we will connect to
String result;
String APIKEY = "a08836891c5685f7795009e8965d9ef0";                                 
String CityID = "1264527";                        
int  fetchweatherflag = 0; 
String weatherDescription ="";
String weatherLocation = "";
String Country;
float Temperature=0.00;
float Humidity;
float Pressure;

//Matrix Mapping for 8X32 matrix
int Matrix[8][32]={{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31},
                  {63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32},
                  {64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95},
                  {127,126,125,124,123,122,121,120,119,118,117,116,115,114,113,112,111,110,109,108,107,106,105,104,103,102,101,100,99,98,97,96},
                  {128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159},
                  {191,190,189,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,170,169,168,167,166,165,164,163,162,161,160},
                  {192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223},
                  {255,254,253,252,251,250,249,248,247,246,245,244,243,242,241,240,239,238,237,236,235,234,233,232,231,230,229,228,227,226,225,224},
                  };
//Number mapping in 8X3 matrix
int N1 [8][3]={{0,0,0},{0,1,0},{1,1,0},{0,1,0},{0,1,0},{1,1,1},{0,0,0},{0,0,0}};        
int N2 [8][3]={{0,0,0},{1,1,1},{0,0,1},{1,1,1},{1,0,0},{1,1,1},{0,0,0},{0,0,0}};        
int N3 [8][3]={{0,0,0},{1,1,1},{0,0,1},{1,1,1},{0,0,1},{1,1,1},{0,0,0},{0,0,0}};        
int N4 [8][3]={{0,0,0},{1,0,1},{1,0,1},{1,1,1},{0,0,1},{0,0,1},{0,0,0},{0,0,0}};        
int N5 [8][3]={{0,0,0},{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1},{0,0,0},{0,0,0}};        
int N6 [8][3]={{0,0,0},{1,1,1},{1,0,0},{1,1,1},{1,0,1},{1,1,1},{0,0,0},{0,0,0}};  
int N7 [8][3]={{0,0,0},{1,1,1},{0,0,1},{0,0,1},{0,0,1},{0,0,1},{0,0,0},{0,0,0}};        
int N8 [8][3]={{0,0,0},{1,1,1},{1,0,1},{1,1,1},{1,0,1},{1,1,1},{0,0,0},{0,0,0}};        
int N9 [8][3]={{0,0,0},{1,1,1},{1,0,1},{1,1,1},{0,0,1},{1,1,1},{0,0,0},{0,0,0}};        
int N0 [8][3]={{0,0,0},{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1},{0,0,0},{0,0,0}};        
int Nd [8][3]={{0,0,0},{1,1,0},{1,1,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};        
int Nc [8][3]={{0,0,0},{1,1,1},{1,0,0},{1,0,0},{1,0,0},{1,1,1},{0,0,0},{0,0,0}};  
int A  [8][3]={{0,0,0},{1,1,0},{1,0,1},{1,1,1},{1,0,1},{1,0,1},{0,0,0},{0,0,0}};
int P  [8][3]={{0,0,0},{1,1,1},{1,0,1},{1,1,0},{1,0,0},{1,0,0},{0,0,0},{0,0,0}};
int M1 [8][3]={{0,0,0},{1,0,0},{1,1,0},{1,0,1},{1,0,0},{1,0,0},{0,0,0},{0,0,0}};
int M2 [8][3]={{0,0,0},{0,1,0},{1,1,0},{0,1,0},{0,1,0},{0,1,0},{0,0,0},{0,0,0}};

// Weather mapping for first 8X8 matrix

int ClearDay[]  = {2,5,64,66,67,68,69,71,125,124,123,122,130,131,132,133,191,189,188,187,186,184,253,250};
int sunsmall[]  = {3,6,58,59,68,69,121};
int Cloud1[]    = {65,127,126,125,128,129,130,131,191,190,189};//,192,193};
int Cloud2[]    = {67,127,125,124,123,128,129,130,131,132,133,191,190,189,188,187};//,192,193,194,195};
int Cloud3[]    = {69,125,123,122,121,129,130,131,132,133,134,135,190,189,188,187,186,185};//,194,195,196,197};
int Cloud4[]    = {127,122,120,128,129,132,133,134,135,191,187,186,185,184};//,197,198,199};
int windy11[][8]={{0,0,64,0,0,191,0,0},{0,62,0,0,0,190,0,0},{0,0,66,0,130,0,0,0},{0,0,0,124,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
int windy12[][8]={{0,0,64,0,0,191,0,0},{0,0,0,126,0,190,0,0},{0,0,66,0,0,189,0,0},{0,60,0,0,0,188,0,0},{0,0,68,0,132,0,0,0},{0,0,0,122,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0}};
int windy13[][8]={{0,0,0,0,0,0,0,0},{0,0,0,126,129,0,0,0},{0,0,66,0,0,189,0,0},{0,0,0,124,0,188,0,0},{0,0,68,0,0,187,0,0},{0,58,0,0,0,186,0,0},{0,0,70,0,134,0,0,0},{0,0,0,120,0,0,0,0}};
int windy14[][8]={{0,0,64,0,128,0,0,0},{0,0,0,126,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,123,132,0,0,0},{0,0,69,0,0,186,0,0},{0,0,0,121,0,185,0,0},{0,0,71,0,0,184,0,0}};
int StaticCloud[] = {5,61,59,58,57,65,66,67,68,69,70,71,126,125,124,123,122,121,130,131,132,133};
int Raindrop1[] = {190,186,195,199,254,250};
int Raindrop2[] = {188,184,193,197,252,248};
int Raindrop3[] = {191,187,194,198,255,251};
int Raindrop4[] = {189,185,192,196,253,249};
int thunder[]   = {59,67,125,130,131,132,187,195,253};
int mist1[]     = {63,60,59,58,56,65,66,70,128,131,132,133,135,190,189,185,255,252,251,250,248};
int mist2[]     = {62,61,57,64,67,68,69,71,129,130,134,191,188,187,186,184,254,253,249};

// alphabets in 8X3 matrix format
int a  [8][3]={{0,0,0},{1,1,0},{1,0,1},{1,1,1},{1,0,1},{1,0,1},{0,0,0},{0,0,0}};
int b  [8][3]={{0,0,0},{1,1,0},{1,0,1},{1,1,0},{1,0,1},{1,1,0},{0,0,0},{0,0,0}};
int c  [8][3]={{0,0,0},{1,1,1},{1,0,0},{1,0,0},{1,0,0},{1,1,1},{0,0,0},{0,0,0}};
int d  [8][3]={{0,0,0},{1,1,0},{1,0,1},{1,0,1},{1,0,1},{1,1,0},{0,0,0},{0,0,0}};
int e  [8][3]={{0,0,0},{1,1,1},{1,0,0},{1,1,1},{1,0,0},{1,1,1},{0,0,0},{0,0,0}};
int f  [8][3]={{0,0,0},{1,1,1},{1,0,0},{1,1,0},{1,0,0},{1,0,0},{0,0,0},{0,0,0}};
int g  [8][3]={{0,0,0},{1,1,0},{1,0,0},{1,0,0},{1,0,1},{1,1,0},{0,0,0},{0,0,0}};
int h  [8][3]={{0,0,0},{1,0,1},{1,0,1},{1,1,1},{1,0,1},{1,0,1},{0,0,0},{0,0,0}};
int i  [8][3]={{0,0,0},{1,1,1},{0,1,0},{0,1,0},{0,1,0},{1,1,1},{0,0,0},{0,0,0}};
int j  [8][3]={{0,0,0},{1,1,1},{0,1,0},{0,0,0},{0,1,0},{1,0,0},{0,0,0},{0,0,0}};
int k  [8][3]={{0,0,0},{1,0,1},{1,0,1},{1,1,0},{1,0,1},{1,0,0},{0,0,0},{0,0,0}};
int l  [8][3]={{0,0,0},{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,1,1},{0,0,0},{0,0,0}};
int mm1[8][3]={{0,0,0},{1,0,0},{1,1,0},{1,0,1},{1,0,0},{1,0,0},{0,0,0},{0,0,0}};
int mm2[8][3]={{0,0,0},{0,1,0},{1,1,0},{0,1,0},{0,1,0},{0,1,0},{0,0,0},{0,0,0}};
int nn1[8][3]={{0,0,0},{1,0,0},{1,1,0},{1,0,1},{1,0,0},{1,0,0},{0,0,0},{0,0,0}};
int nn2[8][3]={{0,0,0},{1,0,0},{1,0,0},{1,0,0},{1,0,0},{1,0,0},{0,0,0},{0,0,0}};
int o  [8][3]={{0,0,0},{1,1,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1},{0,0,0},{0,0,0}};
int p  [8][3]={{0,0,0},{1,1,1},{1,0,1},{1,1,0},{1,0,0},{1,0,0},{0,0,0},{0,0,0}};
int Q  [8][3]={{0,0,0},{1,1,1},{1,0,1},{0,1,1},{0,0,1},{0,0,1},{0,0,0},{0,0,0}};
int r  [8][3]={{0,0,0},{1,1,1},{1,0,1},{1,1,0},{1,0,1},{1,0,1},{0,0,0},{0,0,0}};
int s  [8][3]={{0,0,0},{1,1,1},{1,0,0},{1,1,1},{0,0,1},{1,1,1},{0,0,0},{0,0,0}};
int t  [8][3]={{0,0,0},{1,1,1},{0,1,0},{0,1,0},{0,1,0},{0,1,0},{0,0,0},{0,0,0}};
int u  [8][3]={{0,0,0},{1,0,1},{1,0,1},{1,0,1},{1,0,1},{1,1,1},{0,0,0},{0,0,0}};
int v  [8][3]={{0,0,0},{1,0,1},{1,0,1},{1,0,1},{1,0,1},{0,1,0},{0,0,0},{0,0,0}};
int w1 [8][3]={{0,0,0},{1,0,0},{1,0,0},{1,0,1},{1,1,0},{1,0,0},{0,0,0},{0,0,0}};
int w2 [8][3]={{0,0,0},{0,1,0},{0,1,0},{0,1,0},{1,1,0},{0,1,0},{0,0,0},{0,0,0}};
int x  [8][3]={{0,0,0},{1,0,1},{1,0,1},{0,1,0},{1,0,1},{1,0,0},{0,0,0},{0,0,0}};
int y  [8][3]={{0,0,0},{1,0,1},{1,0,1},{0,1,0},{0,1,0},{0,0,0},{0,0,0},{0,0,0}};
int z  [8][3]={{0,0,0},{1,1,1},{0,0,1},{0,1,0},{1,0,0},{1,1,1},{0,0,0},{0,0,0}};

// varaible to store time value
int h1=0;
int h2=0;
int m1=0;
int m2=0;
int s1=0;
int s2=0;
int dat=0;
int mon=0;
int yr=0;
// varaible to store last time value 
int temph1=0;
int temph2=0;
int tempm1=0;
int tempm2=0;
int temps1=0;
int temps2=0;
int tempdat=0;
int tempmon=0;
int tempyr=0;
//other control varaible
int offset =3;             // to control reset of clock digits
float tempTEMP=0.00;       // last vale of temp to prevent unwanted number refresh
int secondreference=0;     // to use second value to control Animations
int minutereference=0;     // to use minute value to control Animations
int hourreference=0;     // to use hour value to control Animations
int stringoffset=9; // offset for controlling text with animation
int displayflag=0;
int screenid=1;

void setup() {
  // sanity check delay - allows reprogramming if accidently blowing power w/leds
  delay(2000);
  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(8);
  FastLED.clear();
  displaytext(offset,N7,Matrix);
  delay(20000);
      // TIME setup
  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );
  WiFi.begin(ssid,password);
  Serial.println();
  Serial.print("Connecting");
  
  

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");        
  }

  Serial.println();
  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );
  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
  Serial.println("\nWaiting for Internet time");

  while(!time(nullptr)){
     Serial.print("*");
     delay(1000);
  }
  Serial.println("\nTime response....OK");   
  getWeatherData();
  FastLED.clear();
  pinMode( S1,INPUT_PULLUP);
  pinMode( S2,INPUT_PULLUP);
  pinMode( S3,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(S1), S1Handler, RISING);
  attachInterrupt(digitalPinToInterrupt(S2), S2Handler, RISING);
  attachInterrupt(digitalPinToInterrupt(S1), S1Handler, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
    time_t now = time(nullptr);
    struct tm* p_tm = localtime(&now);
    //Serial.println(p_tm->tm_hour);
    h1=p_tm->tm_hour/10;  //1st digit of hour
    h2=p_tm->tm_hour%10;  //2st digit of hour
    m1=p_tm->tm_min/10;   //1st digit of minute
    m2=p_tm->tm_min%10;   //2st digit of minute
    s1=p_tm->tm_sec/10;   //1st digit of sec
    s2=p_tm->tm_sec%10;   //2st digit of sec
    dat=p_tm->tm_mday;    //Day of the month
    mon=p_tm->tm_mon + 1; // month of the year
    yr=p_tm->tm_year + 1900; //year
    secondreference= p_tm->tm_sec;                     // to be used as update animation based on second value
    hourreference=p_tm->tm_hour;
    minutereference=p_tm->tm_min;
    if(p_tm->tm_min%10==0 && fetchweatherflag ==0)    //Get new data every 10 minutes
    {
     fetchweatherflag = 1;
      getWeatherData();
    }
    else if (p_tm->tm_min%10!=0)
    {
     fetchweatherflag = 0;
    }

//timeformat1();// display time in HH:MM:SS format
//timeformat2();// display time in HH:MM PM/AM format
//showtemperature(Temperature);// display temperate with thermometer animation 
//showweather(weatherDescription);
//cleardigit(0,Matrix);
//cleardigit(4,Matrix);
//ClearDayAnime(ClearDay,(sizeof(ClearDay) / sizeof(int)));
//ClearDayAnime(sunsmall,(sizeof(sunsmall) / sizeof(int)));
//CloudyAnime();
//RainAnime();
//StormAnime();
//MistAnime();
 //delay(120);

mode1();

FastLED.show();
delay(200);

///void loop ends
}

void digit(int offset, int digpoint [8][3], int matrix [8][32] )
{ cleardigit(offset,matrix);
  for (int i=0 ; i<8; i++)
  { for (int k=0 ; k<=i; k++)
    { for(int j=0;j<3; j++)
      { if (digpoint[k][j]==0)
        leds[matrix [k][j+offset]] = CRGB(0,0,0);
        else
        leds[matrix [k][j+offset]] = CRGB(200,200,255);
        }
        delay(5);
        FastLED.show(); 
    }
    delay(30);
   }
 }

  void displaytext(int offset, int digpoint [8][3], int matrix [8][32] )
{ cleardigit(offset,matrix);
  for (int i=0 ; i<8; i++)
  { for (int k=0 ; k<=i; k++)
    { for(int j=0;j<3; j++)
      { if (digpoint[k][j]==0)
        leds[matrix [k][j+offset]] = CRGB(0,0,0);
        else
        leds[matrix [k][j+offset]] = CRGB(200,200,255);
        }
        //delay(10);
        }
    //delay(20);
   }
   FastLED.show(); 
  }

void cleardigit(int offset,int matrix [8][32])
{ for (int k=0 ; k<8; k++)
    { for(int j=0;j<4; j++)
      { leds[matrix [k][j+offset]] = CRGB(0,0,0);
} }}


void timeformat1 ()
{   
     if (temps2 != s2)
    { offset=27;
      temps2=s2;
      updatedigit(offset,s2);
     // if(s2==8) delay(500);
      }  
      if (temps1 != s1)
    { offset=23;
      temps1=s1;
      updatedigit(offset,s1);
      }
     if (tempm2 != m2)
    { offset=17;
      tempm2=m2;
      updatedigit(offset,m2);
      } 
     if (tempm1 != m1)
    { offset=13;
      tempm1=m1;
      updatedigit(offset,m1);      
      }
     if (temph2 != h2)
    { offset=7;
      temph2=h2;
      updatedigit(offset,h2);
      }
      if (temph1 != h1)
    { offset=3;
      temph1=h1;
      updatedigit(offset,h1);
      }
      
      if((s1*10+s2)%2==0)
      {leds[75] = CRGB(0,200,255);;
      leds[139] = CRGB(0,200,255);
      leds[85] = CRGB(0,200,255);
      leds[149]= CRGB(0,200,255);
      }
      else
      {leds[75] = CRGB(200,200,255);;
      leds[139] = CRGB(200,200,255);
      leds[85] = CRGB(200,200,255);
      leds[149]= CRGB(200,200,255);
      }
  }


void timeformat2 ()
{    //FastLED.clear();
  if((h1*10+h2)>12)
  {
    int temp = h1*10+h2-12;
    
    h1=temp/10;
    h2=temp%10;
    updatedigit(21,12);
    updatedigit(25,13);
    updatedigit(28,14);
    }
  else
  {
    updatedigit(21,11);
    updatedigit(25,13);
    updatedigit(28,14);
    }
    
  
  if (temph1 != h1  )
    { offset=3;
      temph1=h1;
      updatedigit(offset,h1);
      }
   if (temph2 != h2  )
    { offset=7;
      temph2=h2;
      updatedigit(offset,h2);
      }
   if (tempm1 != m1 ) 
    { offset=13;
      tempm1=m1;
      updatedigit(offset,m1);      
      }
   if (tempm2 != m2 )
    { offset=17;
      tempm2=m2;
      updatedigit(offset,m2);
      } 

      if((s1*10+s2)%2==0)
      {leds[75] = CRGB(0,200,255);;
      leds[139] = CRGB(0,200,255);
      }
      else
      {leds[75] = CRGB(200,200,255);;
      leds[139] = CRGB(200,200,255);
      }
  }


void updatedigit (int offset, int num)
{
  switch (num) {
  case 1: digit(offset,N1,Matrix);  break;
  case 2:
    digit(offset,N2,Matrix);
    break;
  case 3:
    digit(offset,N3,Matrix);
    break;
  case 4:
   digit(offset,N4,Matrix);
    break;
  case 5:
    digit(offset,N5,Matrix);
    break;
  case 6:
    digit(offset,N6,Matrix);
    break;
  case 7:
    digit(offset,N7,Matrix);
    break;
  case 8:
    digit(offset,N8,Matrix);
    break;
   case 9:
    digit(offset,N9,Matrix);
    break;
  case 0:
    digit(offset,N0,Matrix);
    break;
     case 11:
    displaytext(offset,A,Matrix);
    break;
     case 12:
    displaytext(offset,P,Matrix);
    break;
     case 13:
    displaytext(offset,M1,Matrix);
    break;
     case 14:
    displaytext(offset,M2,Matrix);
    break;
  default:
    // if nothing else matches, do the default
    // default is optional
    break;
}
  
  }


  void getWeatherData()     //client function to send/receive GET request data.
{
  if (client.connect(servername, 80))   
          {                         //starts client connection, checks for connection
          client.println("GET /data/2.5/weather?id="+CityID+"&units=metric&APPID="+APIKEY);
          client.println("Host: api.openweathermap.org");
          client.println("User-Agent: ArduinoWiFi/1.1");
          client.println("Connection: close");
          client.println();
          } 
  else {
         Serial.println("connection failed");        //error message if no client connect
          Serial.println();
       }

  while(client.connected() && !client.available()) 
  delay(1);                                          //waits for data
  while (client.connected() || client.available())    
       {                                             //connected or data available
         char c = client.read();                     //gets byte from ethernet buffer
         result = result+c;
       }

client.stop();                                      //stop client
result.replace('[', ' ');
result.replace(']', ' ');
Serial.println(result);
char jsonArray [result.length()+1];
result.toCharArray(jsonArray,sizeof(jsonArray));
jsonArray[result.length() + 1] = '\0';
StaticJsonBuffer<1024> json_buf;
JsonObject &root = json_buf.parseObject(jsonArray);

if (!root.success())
  {
    Serial.println("parseObject() failed");
  }

String location = root["name"];
String country = root["sys"]["country"];
float temperature = root["main"]["temp"];
float humidity = root["main"]["humidity"];
String weather = root["weather"]["main"];
String description = root["weather"]["description"];
float pressure = root["main"]["pressure"];
//weatherDescription = description;
weatherDescription = weather;
weatherLocation = location;
Country = country;
Temperature = temperature;
Humidity = humidity;
Pressure = pressure;

Serial.println(weather);
Serial.println(temperature);

}


void showtemperature(float temp)
{
  AnimeThermo (temp,secondreference);
  if(tempTEMP != temp)
  {
  tempTEMP = temp;
  int bd= (int) temp; 
  float adtemp = temp - bd;
  adtemp=adtemp*10;
  int ad= (int) adtemp;
  updatedigit(10,bd/10);
  updatedigit(14,bd%10);
  updatedigit(20,ad);
  Serial.println(bd);
  Serial.println(bd%10);
  Serial.println(adtemp);

 //to display degeree and celcius symbol
  leds[173]   = CRGB(200,200,235);
  leds[38 ]   = CRGB(200,200,235);
  leds[39 ]   = CRGB(200,200,235);
  leds[88 ]   = CRGB(200,200,235);
  leds[89 ]   = CRGB(200,200,235);
  leds[36 ]   = CRGB(200,200,235);
  leds[35 ]   = CRGB(200,200,235);
  leds[34 ]   = CRGB(200,200,255);
  leds[91 ]   = CRGB(200,200,255);
  leds[100]   = CRGB(200,200,255);
  leds[155]   = CRGB(200,200,255);
  leds[164]   = CRGB(200,200,255);
  leds[163]   = CRGB(200,200,255);
  leds[162]   = CRGB(200,200,255);
 
  }
  //Function End
  }

  void AnimeThermo(float temp, int sec)
{
  leds[2  ]   = CRGB(200,200,235);
  leds[3  ]   = CRGB(200,200,235);
  leds[4  ]   = CRGB(200,200,235);
  leds[61 ]   = CRGB(200,200,235);
  leds[59 ]   = CRGB(200,200,235);
  leds[66 ]   = CRGB(200,200,235);
  leds[68 ]   = CRGB(200,200,235);
  leds[125]   = CRGB(200,200,235);
  leds[123]   = CRGB(200,200,235);
  leds[130]   = CRGB(200,200,235);
  leds[132]   = CRGB(200,200,235);
  leds[190]   = CRGB(200,200,235);
  leds[186]   = CRGB(200,200,235);
  leds[193]   = CRGB(200,200,235);
  leds[197]   = CRGB(200,200,255);
  leds[251]   = CRGB(200,200,255);
  leds[252]   = CRGB(200,200,255);
  leds[253]   = CRGB(200,200,255);
  leds[55 ]   = CRGB(200,200,255);
  leds[56 ]   = CRGB(200,200,255);
  leds[120]   = CRGB(200,200,255);
  leds[119]   = CRGB(200,200,255);
  leds[183]   = CRGB(200,200,255);
 // leds[254]   = CRGB(200,200,255);
//  leds[250]   = CRGB(200,200,255);

if (temp<10)
{ leds[60 ]   = CRGB(0,0,0);
  leds[67 ]   = CRGB(0,0,0);
  leds[124]   = CRGB(0,0,0);
  leds[131]   = CRGB(0,0,0);
  leds[187]   = CRGB(0,0,0);
  leds[188]   = CRGB(0,0,0);
  leds[189]   = CRGB(0,0,0);
  leds[194]   = CRGB(0,0,240);
  leds[195]   = CRGB(0,0,240);
  leds[196]   = CRGB(0,0,240);
  }
else if (temp>10 && temp <20)
{ leds[60 ]   = CRGB(0,0,0);
  leds[67 ]   = CRGB(0,0,0);
  leds[124]   = CRGB(0,0,0);
  leds[131]   = CRGB(0,0,0);
  leds[187]   = CRGB(0,240,100);
  leds[188]   = CRGB(0,240,100);
  leds[189]   = CRGB(0,240,100);
  leds[194]   = CRGB(0,0,240);
  leds[195]   = CRGB(0,0,240);
  leds[196]   = CRGB(0,0,240);
  }

else if (temp>20 && temp <30)
{ leds[60 ]   = CRGB(0,0,0);
  leds[67 ]   = CRGB(0,0,0);
  leds[124]   = CRGB(0,0,0);
  leds[131]   = CRGB(255,165,0);
  leds[188]   = CRGB(255,164,0);
  leds[187]   = CRGB(0,240,100);
  leds[189]   = CRGB(0,240,100);
  leds[194]   = CRGB(0,240,100);
  leds[195]   = CRGB(0,240,100);
  leds[196]   = CRGB(0,240,100);
  }
else if (temp>30 && temp <40)
{ leds[60 ]   = CRGB(0,0,0);
  leds[67 ]   = CRGB(0,0,0);
  leds[124]   = CRGB(0,0,0);
  leds[131]   = CRGB(255,0,0);
  leds[188]   = CRGB(255,0,0);
  leds[187]   = CRGB(255,165,0);
  leds[189]   = CRGB(255,165,0);
  leds[194]   = CRGB(255,165,0);
  leds[195]   = CRGB(255,165,0);
  leds[196]   = CRGB(255,165,0);
  }
else if (temp>40 )
{ leds[60 ]   = CRGB(0,0,0);
  leds[67 ]   = CRGB(255,0,0);
  leds[124]   = CRGB(255,0,0);
  leds[131]   = CRGB(255,0,0);
  leds[187]   = CRGB(255,0,0);
  leds[188]   = CRGB(255,0,0);
  leds[189]   = CRGB(255,0,0);
  leds[194]   = CRGB(255,0,0);
  leds[195]   = CRGB(255,0,0);
  leds[196]   = CRGB(255,0,0);
  }
// 
if (sec%2 == 0)
{ leds[184]   = CRGB(200,200,255);
  leds[121]   = CRGB(200,200,255);
  leds[57 ]   = CRGB(0,0,0);
} 
else
{ leds[184]   = CRGB(0,0,0);
  leds[121]   = CRGB(0,0,0);
  leds[57 ]   = CRGB(200,200,255);;
} 
 FastLED.show(); 
  }

//iterator = (sizeof(Raindrop1) / sizeof(int))
void ClearDayAnime(int *Num,int iterator)
{ for (int i=0; i< iterator;i++)
  { leds[*(Num+i)] = CRGB(255,int (random(140, 255)),0);
    }
  }

void AnimeXX (int *Num,int iterator, int r , int g, int b)
{ for (int i=0; i< iterator;i++)
  { leds[*(Num+i)] = CRGB(r,g,b);
    }
  }

//int StaticCloud[] = {5,61,59,58,57,65,66,67,68,69,70,71,126,125,124,123,122,121,130,131,132,133};
//int Raindrop1[] = {190,186,195,199,254,250};

void RainAnime()
{   
  AnimeXX(StaticCloud,sizeof(StaticCloud) / sizeof(int),220,220,240);
  if(secondreference%4==0)
  AnimeXX(Raindrop1,sizeof(Raindrop1) / sizeof(int),0,0,220);
    if(secondreference%4==1)
  AnimeXX(Raindrop2,sizeof(Raindrop2) / sizeof(int),0,0,220);
    if(secondreference%4==2)
  AnimeXX(Raindrop3,sizeof(Raindrop3) / sizeof(int),0,0,220);
    if(secondreference%4==3)
  AnimeXX(Raindrop4,sizeof(Raindrop4) / sizeof(int),0,0,220);
}

void StormAnime()
{   
  AnimeXX(StaticCloud,sizeof(StaticCloud) / sizeof(int),220,220,240);
  if(secondreference%2==1)
  AnimeXX(thunder,sizeof(thunder) / sizeof(int),255,140,0);
 }

void MistAnime()
{   
  //AnimeXX(StaticCloud,sizeof(StaticCloud) / sizeof(int),220,220,240);
  if(secondreference%2==0)
  AnimeXX(mist1,sizeof(mist1) / sizeof(int),135,206,250);
  if(secondreference%2==1)
  AnimeXX(mist2,sizeof(mist2) / sizeof(int),135,206,250);
 }

void CloudyAnime()
{   if(secondreference%4==0)
  windy1();
    if(secondreference%4==1)
  windy2();
    if(secondreference%4==2)
  windy3();
    if(secondreference%4==3)
  windy4();
}


void windy1()
{  for (int i=0;i<8;i++)
  { for (int j=0;j<8;j++)
        { if (windy11[i][j]!=0)
          { leds[windy11[i][j]] = CRGB(135,206,235);
            }
            }  }}

void windy2()
{ for (int i=0;i<8;i++)
  { for (int j=0;j<8;j++)
        {  if (windy12[i][j]!=0)
          {  leds[windy12[i][j]] = CRGB(135,206,235);
            }
             }   }  }

void windy3()
{ leds[69] = CRGB(0,0,0);
  leds[121] = CRGB(0,0,0);
  for (int i=0;i<8;i++)
  {for (int j=0;j<8;j++)
        { if (windy13[i][j]!=0)
          { leds[windy13[i][j]] = CRGB(135,206,235);
            }
            }  } }

void windy4( )
{  for (int i=0;i<8;i++)
  { for (int j=0;j<8;j++)
        {  if (windy14[i][j]!=0)
          {  leds[windy14[i][j]] = CRGB(135,206,235);
            }
         }   }  }

void ScrollingText(String disp)
{
  int offsetval[]   = {9,13,17,21,25,29};
  int tempcheck[]  =  {11,12,13,14,11,12};
  int strLen= disp.length();
  if (strLen>0 && strLen<7)
  {
    //function goes here
    for (int i=0 ; i<strLen; i++)
    {
      updatedigit (offsetval[i], tempcheck[i]);
      }
   
    // function ends here
    }
  }

void showweather (String wDe)
{
  //Serial.println(wDe);
  cleardigit(0,Matrix);
  cleardigit(4,Matrix);
  if (wDe.equalsIgnoreCase("Thunderstorm"))
  {
    StormAnime();
    displaytext(9 ,s ,Matrix);
    displaytext(13,t ,Matrix);
    displaytext(17,o ,Matrix);
    displaytext(21,r ,Matrix);
    displaytext(25,mm1 ,Matrix);
    displaytext(28,mm2 ,Matrix);
    }

    if (wDe.equalsIgnoreCase("Drizzle"))
  {
      if (minutereference%2==0)
      RainAnime();
      else
      CloudyAnime();
   
    displaytext(9 ,d ,Matrix);
    displaytext(13,r ,Matrix);
    displaytext(17,i ,Matrix);
    displaytext(21,z ,Matrix);
    displaytext(25,l ,Matrix);
    displaytext(29,e ,Matrix);
    }
  if (wDe.equalsIgnoreCase("Rain"))
    {
      RainAnime();
      //displaytext(9 ,s ,Matrix);
      displaytext(12,r ,Matrix);
      displaytext(16,a ,Matrix);
      displaytext(20,i ,Matrix);
      displaytext(24,nn1 ,Matrix);
      displaytext(27,nn2 ,Matrix);
      }
    if (wDe.equalsIgnoreCase("Snow"))
    {
      RainAnime();
      //displaytext(9 ,s ,Matrix);
      displaytext(10 ,s ,Matrix);
      displaytext(14,nn1 ,Matrix);
      displaytext(17,nn2 ,Matrix);
      displaytext(19,o ,Matrix);
      displaytext(23,w1 ,Matrix);
      displaytext(26,w2 ,Matrix);
      }
    if (wDe.equalsIgnoreCase("Clear"))
    {
      if(hourreference >5 &&  hourreference<18)
      ClearDayAnime(ClearDay,(sizeof(ClearDay) / sizeof(int)));
      //else
      // add animation for clear night sky with moon
      //displaytext(9 ,s ,Matrix);
      displaytext(10,c ,Matrix);
      displaytext(14,l ,Matrix);
      displaytext(18,e ,Matrix);
      displaytext(22,a ,Matrix);
      displaytext(26,r ,Matrix);
      }
    if (wDe.equalsIgnoreCase("Clouds"))
    {
      if(hourreference >5 &&  hourreference<18)
      {
      ClearDayAnime(sunsmall,(sizeof(sunsmall) / sizeof(int)));
      CloudyAnime();
      }
      else
      CloudyAnime();
      //Serial.println("reached clouds");
      displaytext(9 ,c ,Matrix);
      displaytext(13,l ,Matrix);
      displaytext(17,o ,Matrix);
      displaytext(21,u ,Matrix);
      displaytext(25,d ,Matrix);
      displaytext(29,s ,Matrix);
      }
      if (wDe.equalsIgnoreCase("Mist"))
    {
      MistAnime();
      //displaytext(9 ,s ,Matrix);
      displaytext(12,mm1 ,Matrix);
      displaytext(15,mm2 ,Matrix);
      displaytext(19,i ,Matrix);
      displaytext(23,s ,Matrix);
      displaytext(27,t ,Matrix);
      //leds[14] = CRGB(200,200,255);
      //FastLED.show(); 
      }

      if (wDe.equalsIgnoreCase("Fog"))
    {
      MistAnime();
      //displaytext(9 ,s ,Matrix);
      //displaytext(9 ,c ,Matrix);
      displaytext(13,f ,Matrix);
      displaytext(17,o ,Matrix);
      displaytext(21,g ,Matrix);
      //displaytext(25,d ,Matrix);
      //displaytext(29,s ,Matrix);
      leds[103] = CRGB(200,200,255);
      leds[104] = CRGB(200,200,255);
      FastLED.show(); 
      }
    //function ends
  }

void resetTimeDisplay()
{
    FastLED.clear();
    temph1=0;    temph2=0;    tempm1=0;    tempm2=0;    temps1=0;    temps2=0;    tempdat=0;    tempmon=0;    tempyr=0;
    displaytext(3,N0,Matrix);
    displaytext(7,N0,Matrix);
    displaytext(13,N0,Matrix);
    displaytext(17,N0,Matrix);
    displaytext(23,N0,Matrix);
    displaytext(27,N0,Matrix);
  
  } 

////////////////////////////////////////////Interrupt Handler Starts Here\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void S1Handler ()
  {
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 300) 
 {   
    screenid=screenid+1;
        if(screenid>3)
        {      screenid=1;     }
 }
 last_interrupt_time = interrupt_time;
 Serial.println("Interrupt Triggered : Button - S1"); 
    }
    

void S2Handler ()
  {static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 300) 
 {
    screenid=screenid-1;
        if(screenid<1)
        {      screenid=3;     }

 }
 last_interrupt_time = interrupt_time;
 Serial.println("Interrupt Triggered : Button - S2");       
    }

    void S3Handler ()
  {  Serial.println("Interrupt Triggered : Button - S3");
    }

////////////////////////////////////////////Display Mode starts Here \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

void mode1()
{
  if (screenid==1)
{  if (displayflag!=1)
  {    displayflag=1;
        FastLED.clear();
        resetTimeDisplay(); }
     timeformat2();}
if (screenid==2)
{  if (displayflag!=2)
  {    displayflag=2;
    FastLED.clear();
    tempTEMP=0.0;  }
    showtemperature(Temperature);}
if (screenid==3)
{ if (displayflag!=3)
  {    displayflag=3;
    FastLED.clear();  }
showweather(weatherDescription);}
  }
