/*

  Dectiption:
    monitors gsm communication 
    
  LED/LED_BUILTIN= D8
*/
#include <SoftwareSerial.h>
//#include <String.h>

#define D1 5
#define D2 4
#define D7 13
#define D8 15
#define LED D8

#define OK 1
#define NOTOK 2
#define TIMEOUT 3
#define RST D7

#define RX D2  //Yellow from  gsm TX
#define TX D1  //GREEN from GSM RX
#define BAUD_RATE 9600 //115200
#define A6baud 9600

unsigned long previousTime=0;
int interval=100;

SoftwareSerial A6board(RX,TX);

void setup() {  
  // put your setup code here, to run once:
  pinMode(LED,OUTPUT);   
  Serial.begin(9600);
  A6board.begin(9600);
  delay(1000);
  
  //A6command("", "OK", "yy", 1000,1);
  A6command("ATE0", "OK", "yy", 1000,1);
  A6command("AT+CLIP=1", "OK", "yy", 1000,1);             //Caller line display enabled
  A6command("AT+CNMI=1,2,0,0,0", "OK", "yy", 1000,1);     //SMS notification settings
  Serial.println("System ready");    
  
  flash();
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis=millis();
   if( (currentMillis - previousTime) > interval){
        previousTime=currentMillis;
        updateSerial();
   }
}
/***********************************************************************************************************/
void updateSerial(){           
  String sline;  
 
  while(Serial.available()){    
      handleSerial();
  }
  
  while(A6board.available()){      
     //Serial.println("updateSerial called");
     //Serial.println("updateSerial called");
    handleA6() ;
  }  
}

/***********************************************************************************************************/
void handleA6(){
  String sline;
    sline="";
    while (A6board.available()){     
      sline+=A6board.readString();  
       //Serial.println(sline);   
      //sline+=sline;
    }
    
     
     Serial.print("Serial RCV from GSM module:");
     Serial.println(sline);             
 
    
    if(sline.indexOf("+CMT:")>-1){   handleSMS(sline);  } //New SMS
}

/***********************************************************************************************************/
void handleSMS(String sline){
  int count;
  String lines[4];
  String smslines[4];
  String commandLine;
  String number;
  String message;
  int i;
  
  count=splitWord(sline,lines,'\n');
  if(count>0){
    //Serial.print("count=");   Serial.println(count);
    commandLine=lines[1];
    message=lines[2];

    
    //Serial.println("message=" + message);    
    //split commandLine
    if(splitWord(commandLine,smslines,',')>0){ //separates everyting by comma
      if(splitWord(smslines[0],smslines,' ')>0){ //separates "+CMT: " and rest of op codes
        //index 0 =+CMT:  and index 1 sent number
        number=smslines[1];
        //Serial.println("Number=" + number);        
      }         
    }


    //digitalWrite(LED_BUILTIN,LOW);    
    if(message.startsWith("flash")){ 
      Serial.println("cmnd=flash");
      //digitalWrite(LED_BUILTIN,HIGH);
      for(i=0; i<2; i++) { flash(); } 
    }
    
    Serial.println("commandLine=(" + commandLine + ")");
    Serial.println("Message=(" + message + ")" );
    Serial.println("Number=(" + number + ")");
    //Loops through raw SMS notification packet
    /*
    for(i=0;i<count;i++){
      Serial.print(i,DEC); Serial.print(":");  Serial.println(lines[i]);
    }
    */
  }
}

/***********************************************************************************************************/
void flash(){
  for(int i=0;i<4;i++){
    digitalWrite(LED,HIGH); 
    delay(500);
    digitalWrite(LED,LOW); 
    delay(500);
  }
}

/***********************************************************************************************************/
void handleSerial(){   
  String sline;    
  //String lines[10];
  //int count;
  sline=Serial.readStringUntil('\n');  
  
  if(sline.indexOf('#')>-1 ) {
    Serial.println("#custom command recieved");
     
     if( sline.indexOf("#flash")>-1)    {    flash();      }
     if( sline.indexOf("#reset_gsm")>-1){    ResetGSM();      }
     if( sline.indexOf("#restart")>-1)  {    Restart();      }

    //execute_command(sline); 
  }  else{      
    //Serial.print("forwarding to A6board.. ");
    //Serial.println(sline);    
    A6board.println(sline);
  }  
}

/***************************************************************************************************************/
void Restart(){
  ESP.restart();
}

/***************************************************************************************************************/
void ResetGSM(){
  digitalWrite(RST,HIGH);
  delay(2000);
  digitalWrite(RST,LOW);
}
/***************************************************************************************************************/
byte splitWord(String inputWord,String lines[],char delimeter) {   
  int len;  //length of string
  String sword;   //stores sequence of char  
  int counter=0; //counts number of commas  
  char ch;
 
  len=inputWord.length()+1;
  byte bytes[len];
  inputWord.getBytes(bytes,len);
  sword=""; 
  for(int i=0;i<len;i++){       
    ch=(char)bytes[i];
    //Serial.println(String(i,DEC) + " ch=" + String(ch) );
    
    if( ((char)bytes[i])==delimeter ){//comma code is 44      
        lines[counter]=sword;     
        counter++;
        sword="";     
    }///       
    else if(bytes[i]==34){ //Found speech marks. extract following string until next apostrophe is found then set i to x index      
      for(int x=i+1;x<len;x++){        
        if(bytes[x] !=34){          
          sword+=(char)bytes[x]; //if bytes index value is not speech marks then add to sword string
        }else{//found end speech marks. end of word;                             
          i=x;        
          break;       
        }         
      }/// end x for loop
      
    }else{             
          sword+=ch;        
    }
  }/// ends for i statement
  lines[counter]=sword;      
  
  return (byte)counter;
}

