// This function returns an HTML formated page in the correct type for display
// It uses the Raw string macro 'R' to place commands in PROGMEM
const char root[] PROGMEM = R"=====(


<!DOCTYPE html>
<html lang="en">
<head>
<meta charset=utf-8>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>gsm_millis_handle_sms_comms </title>
  <style>
  body{
    background-color:#000000;
    color:#ffffff;
  }

  .div_container{
    display:block;
    width:500px;
    height:auto;
    margin:auto;
    border: 0px solid #ffffff; 
  }
   .displayobject{
       font-family: sans-serif;
       margin: auto;
       text-align: center;
       width: 380px;
       border: 3px solid #000000;
       padding: 10px;
       background: #9bc6fc; /**/
    }
 

    .buttons {
      display:block;
      width:99%;
      height:65px;
      border: solid 0px #cccccc;
      margin:auto;
      margin-bottom:4px;
      cursor:pointer;
      border-radius:10px;
      background-color:#363534;
      color:#ffffff;
    }

    .buttons:hover {
        background-color:#727171;
    }

.block {
  display:block;   
}

    .flex-container {
      display:flex;  
      flex-flow: row wrap;
      flex-direction: column;
      width: inherit;
    }


      .status_bar {
        display:block;
        width:380px;
        height:30px;
        color:#ffffff;
        font-size:18px;
        margin:auto;    
        margin-bottom:4px;       
      }

    .title {
      font-size:18px;
      color:#ffffff; 
    }

    .border{
        border: solid 1px #cccccc;
    } 

    .m-10{
        margin-bottom:10px;
    }

    .leds_bg{
      display:block;
      width:100px;
      height:20px;
      border: solid 0px #cccccc;
      margin:auto;    
      margin-bottom:4px;
    }

    .red {
      color:#ff0000;
    }
    .ta_websock {
      display:block;
      width:inherit;
      height:400px;
    }
    .ta_websock_tx{
      display:block;
      width:300px;
      height:40px;
    }
      </style>


   
  </head>
  <body>
      <div class="div_container">          
          <div class="status_bar" id="div_ws_value" style="margin:0"> div_ws_value</div>
      </div>           

    <div class="div_container m-10" >  
      <p> Serial Tx </p>  
    </div>
      <!-- display received serail response -->
      <div class="div_container"> 
          <input type='textbox'   
              id="ta_websocket_tx" 
              onkeyup="javascript:{  if (event.keyCode==13) {_post(); _clearTxTextbox();}}"              
              class='ta_websock_tx' />               
          <button  onclick="_post();"> _post()    </button>
      </div>


    <!-- websocket serial monitor -->
                                  
      <div class="div_container">
          <p class='title'>Incoming Serial Text </p>
      </div>

      <!-- display received serail response -->
      <div class="div_container"> 
          <textarea   
              id="ta_websocket" 
              rows="4"
              cols="50"
              class='ta_websock'></textarea>
      </div>
    

  
   




          
     <script>
       //setInterval(function() {getSensorData();}, 100); // Call the update function every half second  
       function getSensorData() {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("RSSIvalue").innerHTML = this.responseText;
          }
        };  

        xhttp.open("GET", "RSSIread", true);
        xhttp.send();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("dBmvalue").innerHTML = this.responseText;
          }
        };
        xhttp.open("GET", "dBmread", true);
        xhttp.send();
        //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      }

      function execFunction(action){
        var slocation;
          slocation=window.location;
          //slocation="http://192.168.1.113/"


        console.log('execFunction:action');
        //show busy
        document.getElementById("div_ws_value").innerHTML = "...";

        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            document.getElementById("div_ws_value").innerHTML = this.responseText;
          }
        };

        //slocation
        var s_action;        
          action +='?randm=' + Math.random();
          s_action=slocation  + action;
          
          console.log("s_action=" + s_action);

          xhttp.open("GET", s_action, true);
          xhttp.send();
      }


        function updateTime() {  
          /*
            var d = new Date();
            var t = "";
            t = d.toLocaleTimeString();
            document.getElementById('P_time').innerHTML = t;
          */
        }

            //var myVar2 = setInterval(updateTime, 1000);  
              socket = new WebSocket("ws:/" + "/" + location.host + ":81");

              //socket = new WebSocket("ws:/" + "/" + "192.168.1.113" + ":81");
              socket.onopen = function(e) {  console.log("[socket] socket.onopen "); };
              socket.onerror = function(e) {  console.log("[socket] socket.onerror "); };
              socket.onmessage = function(e) {  
              
              //console.log("[socket] " + e.data);
              //document.getElementById("div_websocket_status").innerHTML = e.data;
              
              if(e.data.length <=1) return;
              
              websocketResponse(e.data);
            };





   /***********************************************************************************************/
      function websocketResponse(e){
            console.log("websockResponse.e=" + e);
            
            try{
              var data="";
              var lines="";
              data = e;
              let time_stamp=Date.now();              
              var textarea=document.getElementById("ta_websocket");

              console.log('websocket response');
              console.log(e);      
              
              //Add timestamp thenscroll to bottom;

              lines=textarea.innerText + data;
              console.log(lines);
              
              textarea.innerText=lines;                       

            }catch(err){
              //console.log("websockResponse.err=" + err);
            }
      }



/***********************************************************************************************/
      function testWebsocket(){
            //var myVar2 = setInterval(updateTime, 1000);  
              socket = new WebSocket("ws:/" + "/" + location.host + ":81");
              socket.onopen = function(e) {  console.log("[socket] socket.onopen "); };
              socket.onerror = function(e) {  console.log("[socket] socket.onerror "); };
              socket.onmessage = function(e) {  
                  console.log("[socket] " + e.data);
                  document.getElementById("div_ws_value").innerHTML = e.data;
            };        
      }


   /***********************************************************************************************/
   function _post(){
    console.log('js function.post() called');
    
    let textbox=document.getElementById("ta_websocket_tx");
    let textValue;

      textValue=encodeURIComponent(textbox.value);
      //textValue="AT";
      //textValue=textbox.value;

    
    console.log("textbox value:"+ textValue );

      var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            console.log("fn:-post response :" + this.responseText );
            //document.getElementById("div_ws_value").innerHTML = this.responseText;
          }
        };

		    xhttp.open("GET", "/post?data=" + textValue , true);
        xhttp.send();

   }  
  
  /*********************************************************************************/
   function _clearTxTextbox(){
      let textbox=document.getElementById("ta_websocket_tx");
        textbox.value="";
        textbox.focus();
        
   }

     </script>
  </body>
   
   <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
  
   <script type="text/javascript">
        $(document).ready( function(){

        });
   </script>
  

</html>



)=====";
