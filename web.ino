
void init_webserver(){  
  
  server.begin();
  Serial.println("HTTP server started");     
    
  server.on("/",[]{
       //server.send(200,"text/plain","esp8266_power-monitor V2..:" + GetJsonString() );
      SendHTML();
        //server.send(200,"text/plain",returnHTML());
  });  
  
  server.on("/restart",[]{
    server.send(200,"text/plain","Restarting ");
    //restart();
    ESP.restart();
  });  

  server.on("/flash",[]{    
      //Flash();
      server.send(200,"text/plain","flash");
  }); 

  




  


 

}

String returnHTML(){
  String sbuff="";
  sbuff+="<html>";
  sbuff+="<body>";
  //sbuff+="  led state:" + String(ledState)  + "<br />";  
  //sbuff+="  utility_line_state:" + String(utility_line_state) + "<br />";
  sbuff+="</body>";
  sbuff+="</html>";    
  return sbuff;
}

 void SendHTML(){
      server.send_P(200, "text/html", root);
 } 
 
