//for Adafruit ESP32 Feather
#define RXD2 16
#define TXD2 17

#define MAIN_MENU             0
#define SENSOR_TEST_MENU      1


#define RED_LED               14
#define HPM_SENSOR_HEAD       0x42

int userCommand = 0;
char current_menu = MAIN_MENU;

byte readpart[] = {0x68, 0x01, 0x04, 0x93};  //Listed command to read particle data
byte readstart[] = {0x68, 0x01, 0x04, 0x96}; //Listed command to start particle measurement
int RxSensorData[32];
char Packet_Ok = 0;

void RedLed_test() {
  digitalWrite(RED_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(RED_LED, LOW);    // turn the LED off by making the voltage LOW
  delay(1000); 
}
void BuiltInLed_test() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}


void setup() {

  char i;
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  
  // put your setup code here, to run once:

  Serial.begin(115200);                                         //Opens the main serial port to communicate with the computer
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);                  //Opens the virtual serial port with a baud of 9600
  delay(2000);                                                  //Wait for serial to have started
  Serial.println("Sending particle start reading code");        //Update user that autosend code sent
  Serial2.write(0x68);  
  Serial2.write(0x01);
  Serial2.write(0x04);
  Serial2.write(0x96);
  delay(6000);                                                  //Allow 6 seconds for sensor response time
  Serial.println("Sensor started"); 

  // Initiate variable ----------------------
  for(i=0; i<32; i++){
    RxSensorData[i] =0;
  }
  //-----------------------------------------

  main_menu();

}
void main_menu(void){
  current_menu = MAIN_MENU;
  Serial.println("This is Test mode. Select test Function");
  Serial.println("1: cpu test (LED heart beat)");
  Serial.println("2: sensor test");
  Serial.println("h: display main menu");
  
}


void sensor_Test_menu(void){
  current_menu = SENSOR_TEST_MENU;
  Serial.println("This is sensor_Test_menu. Select test Function");
  Serial.println("1: temperature sensor test");
  Serial.println("L: Red LED test");
  Serial.println("2: return main");
  Serial.println("h: display main menu");
}




/*
 * @brief 먼지센서 데이터를 수신하기 위한 함수다.
 * @details HPM 먼지센서 데이터를 시리얼2번으로 수신하고, 수신된 데이터는 RxSensorData 전역변수에 저장한다.
 * @author oncellboy
 * @date 2020-07-23
 * @version 0.0.9
 *
 */
void ReadParticleSensor(void)
{
   int ReadData=0;
   char readCount;
   
   
   if (Serial2.available() > 0) {
      // read the incoming byte:
      ReadData = Serial2.read();
      

      if(ReadData == HPM_SENSOR_HEAD){

        readCount =0;
        RxSensorData[readCount] = ReadData;

        //receive 31 byte
        while(readCount != 31){

           // Wait until new data available.
           while(!(Serial2.available() > 0)){
            //not operation 
           }
           readCount = readCount+1;
           RxSensorData[readCount] = Serial2.read();
           
         
        }
        Packet_Ok = 1;
        
        
      }
   }


  
}



void loop() {

  Packet_Ok = 0;
  ReadParticleSensor();

  if(Packet_Ok == 1)
  {
    Serial.print("PM2.5 Data= ");
    Serial.println(RxSensorData[7], HEX);
    delay(1000);
  }
  else{
    Serial.println("data not received");
  }
 
  
/*
  if (Serial.available() > 0) {
    // read the incoming byte:
    userCommand = Serial.read();

    if(current_menu == MAIN_MENU){
        if(userCommand == '1')
        {
          Serial.println("Run cpu test ..... ");
          BuiltInLed_test();           
        }
        else if (userCommand == '2')
        {
          sensor_Test_menu();
        }
        else if (userCommand == 'h')
        {
           main_menu();
        }
    }else if(current_menu == SENSOR_TEST_MENU){
        if(userCommand == '1')
        {
          Serial.println("temperature sensor test ..... ");
        }
        else if (userCommand == '2')
        {
          Serial.println("Return to main ..... ");
          main_menu();
        }
        else if (userCommand == 'h')
        {
         sensor_Test_menu();
        }
        else if (userCommand == 'L')
        {
          Serial.println("run Red Led ..... ");
         RedLed_test();
        }
    }
        
    
    
   
  }
    
*/
  //led_test();
  //sendRequest(readpart);  
  
}
