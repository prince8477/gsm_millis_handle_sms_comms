
/***********************************************************************************************************/
bool A6begin() {
  A6board.println("AT+CREG?");
  byte hi = A6waitFor("1,", "5,", 1500);  // 1: registered, home network ; 5: registered, roaming
  while ( hi != OK) {
    A6board.println("AT+CREG?");
    hi = A6waitFor("1,", "5,", 1500);
  }

  if (A6command("AT&F0", "OK", "yy", 5000, 2) == OK) {   // Reset to factory settings
    if (A6command("ATE0", "OK", "yy", 5000, 2) == OK) {  // disable Echo
      if (A6command("AT+CMEE=2", "OK", "yy", 5000, 2) == OK) return OK;  // enable better error messages
      else return NOTOK;
    }
  }
  return OK;
}

/***********************************************************************************************************/
String A6read() {
  String reply = "";
  if (A6board.available())  {
    reply = A6board.readString();
  }
  //  Serial.print("Reply: ");
  //  Serial.println(reply);
  return reply;
}

/***********************************************************************************************************/
byte A6waitFor(String response1, String response2, int timeOut) {
  unsigned long entry = millis();
  int count = 0;
  String reply = A6read();
  byte retVal = 99;
  do {
    reply = A6read();
    if (reply != "") {
      Serial.print((millis() - entry));
      Serial.print(" ms ");
      Serial.println(reply);
    }
  } while ((reply.indexOf(response1) + reply.indexOf(response2) == -2) && millis() - entry < timeOut );
  if ((millis() - entry) >= timeOut) {
    retVal = TIMEOUT;
  } else {
    if (reply.indexOf(response1) + reply.indexOf(response2) > -2) retVal = OK;
    else retVal = NOTOK;
  }
  //  Serial.print("retVal = ");
  //  Serial.println(retVal);
  return retVal;
}

/***********************************************************************************************************/
byte A6command(String command, String response1, String response2, int timeOut, int repetitions) {
  byte returnValue = NOTOK;
  byte count = 0;
  while (count < repetitions && returnValue != OK) {
    A6board.println(command);
    Serial.print("Command: ");
    Serial.println(command);
    if (A6waitFor(response1, response2, timeOut) == OK) {
      //     Serial.println("OK");
      returnValue = OK;
    } else returnValue = NOTOK;
    count++;
  }
  return returnValue;
}