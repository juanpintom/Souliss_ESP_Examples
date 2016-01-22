///
//  HTML PAGE
//

const char PAGE_AdminGeneralSettings[] PROGMEM =  R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;
<strong>Souliss Slot Settings</strong>
<hr>

<a href="config.html" align="center" style="width:300px" class="btn btn--m btn--blue" >Configure Network First!</a><br>
<hr>

<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" >
  <tr><td align="left" style="width:190px">Node Name</td>
    <td align="right"><input type="text" id="devicename" name="devicename" value="" maxlength="10"></td>
    </tr>
    </table>
  
  <hr>
  <div align="center" style="width:380px"><strong>Lights Configuration:(L1-L2-L3)</strong></div>
  
  <table style="width:500px">
  <tr><td>L1 Output Mode:</td><td>
    <select  id="L1" name="L1" onchange="getComboL1(this)" style="width:180px">
      <option value="0">None</option>
      <option value="1">ON-OFF Mode</option>
      <option value="2">Dimmable Mode</option>
      <option value="3">RGB Mode (RED)</option>
      <option value="4">PIR Mode</option>
      <option value="5">Pulse Mode</option>
    </select>
    </td></tr>
  <tr><td>L2 Output Mode:</td><td>
    <select  id="L2" name="L2" onchange="getComboL2(this)" style="width:180px">
      <option value="0">None</option>
      <option value="1">ON-OFF Mode</option>
      <option value="2">Dimmable Mode</option>
      <option value="3">RGB Mode (GREEN)</option>
      <option value="4">PIR Mode</option>
      <option value="5">Pulse Mode</option>
    </select>
    </td></tr>
  <tr><td>L3 Output Mode:</td><td>
    <select  id="L3" name="L3" onchange="getComboL3(this)"style="width:180px">
      <option value="0">None</option>
      <option value="1">ON-OFF Mode</option>
      <option value="2">Dimmable Mode</option>
      <option value="3">RGB Mode (BLUE)</option>
      <option value="4">PIR Mode</option>
      <option value="5">Pulse Mode</option>
    </select>
    </td></tr></table>

    
    <table style="width:500px">
    <tr id="ALARM_TD"><td align="left">Alarm MODE?:</td><td>
    <select  id="ALARM_MODE" name="ALARM_MODE" style="width:180px">
      <option value="0">No</option>
      <option value="1">Yes</option>
    </select>
    </td></tr>
    
    <tr id="PIR1"><td>L1 PIR Sensor:</td><td>
    <select  id="L1PIR" name="L1PIR" style="width:180px">
      <option value="0">PIR Sensor 1</option>
      <option value="1">PIR Sensor 2</option>
    </select>
    </td></tr>
    
    <tr id="PIR2"><td>L2 PIR Sensor:</td><td>
    <select  id="L2PIR" name="L2PIR" style="width:180px">
      <option value="0">PIR Sensor 1</option>
      <option value="1">PIR Sensor 2</option>
    </select>
    </td></tr>
    
    <tr id="PIR3"><td>L3 PIR Sensor:</td><td>
    <select  id="L3PIR" name="L3PIR" style="width:180px">
      <option value="0">PIR Sensor 1</option>
      <option value="1">PIR Sensor 2</option>
    </select>
    </td></tr>    
  </table>
  
  <hr>  
  <div align="center" style="width:380px"><strong>Sensors Configuration:(S1-S2-S3)</strong></div>
  
  <table>
  <tr><td>(S1) GPIO14 Configuration: </td><td>
    <select  id="S1" name="S1" onchange="getComboS1(this)" style="width:180px">
      <option value="0">None</option>
      <option value="1">Dallas (Temperature)</option>
      <option value="2">More comming...</option>
    </select></td>
    <td align="left" id="qty">Qty: <input type="text" id="dallas_qty" name="dallas_qty" size="2" value="1"></td>
    </tr>
    
  <tr><td>(S2) GPIO16 Configuration: </td><td>
    <select  id="S2" name="S2" onchange="getComboS2(this)"style="width:180px">
      <option value="0">None</option>
      <option value="1">DHT (Temp - Humidity)</option>
      <option value="2">Rain Sensor</option>
      <option value="3">More comming...</option>
    </select></td>
    <td align="left" id="type" name="type"> Type:</td><td>
     <select  id="dht_type" name="dht_type">
        <option value="0">DHT11</option>
        <option value="1">DHT22</option>
        </select></td>
    </tr>
  
  <tr><td>(S3) Analog Configuration: </td><td>
    <select  id="S3" name="S3" onchange="getComboS3(this)"style="width:180px">
      <option value="0">None</option>
      <option value="1">LDR (Luminosity)</option>
      <option value="2">Soil Sensor</option>
      <option value="3">More comming...</option>
    </select>
    </td></tr>
    <tr id="Emoncms"><td align="left">Send to EmonCMS?:</td><td>
      <select  id="Send_Emon" name="Send_Emon" style="width:180px" onclick="getEmon(this)">
        <option value="0">No</option>
        <option value="1">Yes</option>
      </select></td>
      <td id="API_text"><input type="text" id="API" name="API" value="" maxlength="32"> API Key</td>
      </tr>
  </table>
  
  <hr>
  <table>
  <tr><td>(S4-1) Configuration: </td><td>
    <select  id="S41" name="S41" onchange="getComboS41(this)"style="width:220px">
      <option value="0">None</option>
      <option value="1">Capacitive</option>
      <option value="3">Buttons_PullUp</option>
      <option value="4">Alarm EndStop</option>
      <option value="6">Garage EndStop (UP)</option>
    </select> TX
    </td></tr>
  <tr><td>(S4-2) Configuration: </td><td>
    <select  id="S42" name="S42" onchange="getComboS42(this)"style="width:220px">
      <option value="0">None</option>
      <option value="1">Capacitive</option>
      <option value="2">Buttons</option>
      <option value="3">Buttons_PullUp</option>
      <option value="4">Alarm EndStop</option>
      <option value="5">Buttons 2 State</option>
      <option value="6">Garage EndStop (DOWN)</option>     
    </select> RX
    </td></tr> 
  </table>
  
  <hr>
  <table>
  <tr><td>(S5-1) Configuration: </td><td>
    <select  id="S51" name="S51" onchange="getComboS51(this)"style="width:220px">
      <option value="0">None</option>
      <option value="1">Capacitive</option>
      <option value="2">Buttons</option>
      <option value="3">Buttons_PullUp</option>
      <option value="4">Alarm EndStop</option>
      <option value="5">Buttons 2 State</option>      
      <option value="6">Relay</option>
      <option value="7">Pulse Output</option>
      <option value="8">PIR Sensor 1</option>
      <option value="9">Pressure Sensor</option>
      <option value="10">Garage Door (UP)</option>
      <option value="11">Window Curtain (OPEN) </option>
      <option value="12">220V_Opto</option>
    </select>
    </td></tr>
  <tr><td>(S5-2) Configuration: </td><td>
    <select  id="S52" name="S52" onchange="getComboS52(this)"style="width:220px">
      <option value="0">None</option>
      <option value="1">Capacitive</option>
      <option value="2">Buttons</option>
      <option value="3">Buttons_PullUp</option>
      <option value="4">Alarm EndStop</option>
      <option value="5">Buttons 2 State</option>      
      <option value="6">Relay</option>
      <option value="7">Pulse Output</option>
      <option value="8">PIR Sensor 2</option>
      <option value="9">Pressure Sensor</option>
      <option value="10">Garage Door (DOWN)</option>
      <option value="11">Window Curtain (CLOSE) </option>
      <option value="12">220V_Relay</option>
    </select>
    </td></tr>

    <tr id="Altitude">
    <td align="left" id="Altitude_line"> Altitude:</td>
    <td><input type="text" id="Altitude_id" name="Altitude_id" size="2" value="20"></td></tr>
    </table>
    <table id="Thresold">
      <tr><td align="left" id="cap_thresold_line"> Capacitive Thresold: </td>
      <td><input type="text" id="cap_thresold" name="cap_thresold" size="2" value="5"></td></tr> 
      <tr><td align="left">Capacitive /w Debug:</td><td>
        <select  id="cap_debug" name="cap_debug" style="width:180px">
          <option value="0">No</option>
          <option value="1">Yes</option>
          </select></td></tr>
      </table>

  <hr>    
  <table>
    <tr><td align="left" style="width:230px">Receive IR on this Node? (S6):</td><td>
      <select  id="IR_ENABLE" name="IR_ENABLE" style="width:180px">
        <option value="0">No</option>
        <option value="1">Yes</option>
      </select></td></tr></table>
    
  <hr>
  <table>
    <tr id="usart"><td align="left" style="width:230px"> Node is USART Bridge?: (S4)</td><td>
      <select  id="usartbridge" name="usartbridge" style="width:180px" onclick="getUSART(this)">
        <option value="0">No</option>
        <option value="1">Yes</option>
      </select></td></tr></table>
  
  <hr>
  <table>  
  <tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
  </table>
  
  <strong>Remember Get Souliss Nodes again from the app after Save Settings</strong>
</form>

<script>
 
window.onload = function ()
{
 load("style.css","css", function() 
 {
  load("microajax.js","js", function() 
  {       
        setValues("/admin/generalvalues");    
  });
 });
}
function load(e,t,n){
  //getConfig();
  if("js"==t){
    var a=document.createElement("script");
    a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)
  }else if("css"==t){
    var a=document.createElement("link");
    a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)
  }
}
function getConfig(){
    var L1 = document.getElementById("L1");
    var L2 = document.getElementById("L2");
    var L3 = document.getElementById("L3");
    var S1 = document.getElementById("S1");
    var S2 = document.getElementById("S2");
    var S3 = document.getElementById("S3");
    var S41 = document.getElementById("S41");
    var S42 = document.getElementById("S42");
    var S51 = document.getElementById("S51");
    var S52 = document.getElementById("S52");
    
    document.getElementById("usart").style.display = "none";
  //  ALARM
    if(L1.value == 4 || L2.value == 4 || L3.value == 4 || S41.value == 4 || S42.value == 4 || S51.value == 4 || S52.value == 4){
      document.getElementById("ALARM_TD").style.display = "block";  //none;  
    }else{
      document.getElementById("ALARM_TD").style.display = "none";  //none;  
      document.getElementById("ALARM_MODE").value = 0;  
    }
    if(S41.value == 4 || S42.value == 4 || S51.value == 4 || S52.value == 4){
      document.getElementById("ALARM_MODE").value = 1;    
    }
  // PIR SENSORS
    if(L1.value == 4){
      document.getElementById("PIR1").style.display = "block";  //none;  
    }else{
      document.getElementById("PIR1").style.display = "none";  //none;  
    }
    if(L2.value == 4){
      document.getElementById("PIR2").style.display = "block";  //none;  
    }else{
      document.getElementById("PIR2").style.display = "none";  //none;  
    }
    if(L3.value == 4){
      document.getElementById("PIR3").style.display = "block";  //none;  
    }else{
      document.getElementById("PIR3").style.display = "none";  //none;  
    }  
  
  //  DALLAS  
    if(S1.value == 1){
      document.getElementById("qty").style.display = "block";  //none;  
    }else{
      document.getElementById("qty").style.display = "none";  //none;  
    }
  //  DHT  
    if(S2.value == 1){
      document.getElementById("type").style.display = "block";  //none;  
    }else{
      document.getElementById("type").style.display = "none";  //none;  
    }
  // EMONCMS CHECK  
    if(document.getElementById("Send_Emon").value == 1){
      document.getElementById("API_text").style.display = "block";  //none;  
    } else { 
      document.getElementById("API_text").style.display = "none"; 
    } 
    
  //  CAPACITIVE    
    if(S41.value == 1 || S42.value == 1 || S51.value == 1 || S52.value == 1) {
       document.getElementById("Thresold").style.display = "block";  //none;  
    } else { 
       document.getElementById("Thresold").style.display = "none"; 
    }
   
  //  BMP180
    if(S51.value == 9 || S52.value == 9) {
      document.getElementById("Altitude").style.display = "block";  //none;  
    } else {
      document.getElementById("Altitude").style.display = "none";  
    }
  
}
function getComboL1(sel) {
  var value = sel.value;
  if(value == 3){
     document.getElementById("L2").value = 3;
     document.getElementById("L3").value = 3;
  } else {
    if(document.getElementById("L2").value == 3){
       document.getElementById("L2").value = 0;
    }
    if(document.getElementById("L3").value == 3){
       document.getElementById("L3").value = 0;
    } 
  } 
  getConfig();
}
  
function getComboL2(sel) {
  var value = sel.value;
  if(value == 3){
     document.getElementById("L1").value = 3;
     document.getElementById("L3").value = 3;
  } else {
    if(document.getElementById("L1").value == 3){
       document.getElementById("L1").value = 0;
    }
    if(document.getElementById("L3").value == 3){
       document.getElementById("L3").value = 0;
    } 
  } 
  getConfig();
}
function getComboL3(sel) {
  var value = sel.value;
  if(value == 3){
     document.getElementById("L1").value = 3;
     document.getElementById("L2").value = 3;
  } else {
    if(document.getElementById("L1").value == 3){
       document.getElementById("L1").value = 0;
    }
    if(document.getElementById("L2").value == 3){
       document.getElementById("L2").value = 0;
    } 
  }
  getConfig();
}
  
  
function getComboS1(sel) {
    getConfig();
}
  
function getComboS2(sel) {
    getConfig();
}
 
  
function getComboS3(sel) {
    getConfig();
}
  
function getEmon(sel) {
    getConfig();
}
  
function getComboS41(sel) {
  if(sel.value > 0){
    document.getElementById("usartbridge").value = 0;
  }
  if(sel.value == 6){    //GARAGE_ENDSTOP
    document.getElementById("S42").value = sel.value;
    document.getElementById("S51").value = 10;
    document.getElementById("S52").value = 10;
  }else if(document.getElementById("S42").value == 6){
    document.getElementById("S42").value = 0;
    document.getElementById("S51").value = 0;
    document.getElementById("S52").value = 0;
  }
  getConfig();
}
function getComboS42(sel) {
  if(sel.value > 0){
    document.getElementById("usartbridge").value = 0;
  }
  if(sel.value == 6){    //GARAGE_ENDSTOP
    document.getElementById("S41").value = sel.value;
    document.getElementById("S51").value = 10;
    document.getElementById("S52").value = 10;
  }else if(document.getElementById("S41").value == 6){
    document.getElementById("S41").value = 0; 
    document.getElementById("S51").value = 0;
    document.getElementById("S52").value = 0;
  }
  getConfig();
}  
  
function getComboS51(sel) {
  var value = sel.value;
  if(value > 8 && value < 13) {
      document.getElementById("S52").value = value;
  } else {
    if(document.getElementById("S52").value > 8 && document.getElementById("S52").value < 13){
      document.getElementById("S52").value = 0;
    }
  }
  if(value == 10) {  //GARAGE_DOOR
    document.getElementById("S41").value = 6;
    document.getElementById("S42").value = 6;
  } else if(document.getElementById("S41").value == 6){
    document.getElementById("S41").value = 0;
    document.getElementById("S42").value = 0;      
  } 
  getConfig();
}
  
function getComboS52(sel) {
  var value = sel.value;  
  if(value > 8 && value < 13) {
      document.getElementById("S51").value = value;
  } else {
    if(document.getElementById("S51").value > 8 && document.getElementById("S51").value < 13){  
      document.getElementById("S51").value = 0;
    } 
  }
  if(value == 10) {
    document.getElementById("S41").value = 6;
    document.getElementById("S42").value = 6;
  } else if(document.getElementById("S41").value == 6){
    document.getElementById("S41").value = 0;
    document.getElementById("S42").value = 0;      
  } 
  getConfig();
}
  
function getUSART(sel){
  if(sel.value == 1){
    document.getElementById("S41").value = 0;   
    document.getElementById("S42").value = 0;
    if(document.getElementById("S51").value == 10){  //GARAGE_DOOR
      document.getElementById("S51").value = 0;
      document.getElementById("S52").value = 0;        
    }
  }
  getConfig();
}
  

</script>
)=====";

//void send_general_html()
//{
//	
//	if (server.args() > 0 )  // Save Settings
//	{
//		//byte0 = 0;
//		//byte1 = 0;
//		//byte2 = 0;
//		String temp = "";
//		usartbridge = false;
//		for ( uint8_t i = 0; i < server.args(); i++ ) {
//		    if (server.argName(i) == "L1") L1 = server.arg(i).toInt(); 
//		    if (server.argName(i) == "L2") L2 = server.arg(i).toInt();
//		    if (server.argName(i) == "L3") L3 = server.arg(i).toInt();
//        if (server.argName(i) == "S1") S1 = server.arg(i).toInt();
//        if (server.argName(i) == "S2") S2 = server.arg(i).toInt();
//        if (server.argName(i) == "S3") S3 = server.arg(i).toInt();
//        if (server.argName(i) == "S41") S41 = server.arg(i).toInt();
//        if (server.argName(i) == "S51") S51 = server.arg(i).toInt();
//        if (server.argName(i) == "S42") S42 = server.arg(i).toInt();
//        if (server.argName(i) == "S52") S52 = server.arg(i).toInt();
//        if (server.argName(i) == "S6") S6 = server.arg(i).toInt();
//        if (server.argName(i) == "ALARM_MODE") ALARM_MODE = true;         
//        if (server.argName(i) == "L1PIR") L1PIR = server.arg(i).toInt();
//        if (server.argName(i) == "L2PIR") L2PIR = server.arg(i).toInt();
//        if (server.argName(i) == "L3PIR") L3PIR = server.arg(i).toInt();
//        if (server.argName(i) == "cap_debug") cap_debug = true;
//     		if (server.argName(i) == "cap_thresold") cap_thresold = server.arg(i).toInt();
//      	if (server.argName(i) == "Altitude_id") ALTITUDE = server.arg(i).toInt();
//      	if (server.argName(i) == "usartbridge") usartbridge = true;
//      	if (server.argName(i) == "devicename") DeviceName = urldecode(server.arg(i)); 
//      	if (server.argName(i) == "API") API = urldecode(server.arg(i));
//      	if (server.argName(i) == "Send_Emon") Send_Emon = true; 
//      	if (server.argName(i) == "dht_type") dht_type = server.arg(i).toInt(); 
//      	if (server.argName(i) == "dallas_qty") dallas_qty = server.arg(i).toInt(); 
//      	if (server.argName(i) == "IR_ENABLE") IR_ENABLE = true;
//        if (server.argName(i) == "DEBUG_LOG") DEBUG_LOG = true;
//        if (server.argName(i) == "DEBUG_CAPSENSE") DEBUG_CAPSENSE = true;
//        if (server.argName(i) == "DEBUG_CAPSENSE_ALL") DEBUG_CAPSENSE_ALL = true;
//        if (server.argName(i) == "DEBUG_DHT") DEBUG_DHT = true;
//        if (server.argName(i) == "DEBUG_PRESSURE") DEBUG_PRESSURE = true;
//        if (server.argName(i) == "DEBUG_GETLUX") DEBUG_GETLUX = true;
//        if (server.argName(i) == "DEBUG_DALLAS") DEBUG_DALLAS = true;
//        if (server.argName(i) == "DEBUG_EMONCMS") DEBUG_EMONCMS = true;
//        if (server.argName(i) == "DEBUG_IR") DEBUG_IR = true;
//        if (server.argName(i) == "DEBUG_IR_FULL") DEBUG_IR_FULL = true; 
//        if (server.argName(i) == "DEBUG_PLC") DEBUG_PLC = true;
//		}
//		WriteConfig_Slots();
//		//firstStart = true;
//		ESP.restart();
//	}
//	server.send ( 200, "text/html", PAGE_AdminGeneralSettings ); 
//	LOG.println(__FUNCTION__); 
//	
//	
//}
//
//void send_general_configuration_values_html()
//{
//	String values ="";
//	values += "L1|" +  (String) L1 + "|input\n";
//	values += "L2|" +  (String) L2 + "|input\n";
//	values += "L3|" +  (String) L3 + "|input\n";
//  values += "S1|" +  (String) S1 + "|input\n";
//  values += "S2|" +  (String) S2 + "|input\n";
//  values += "S3|" +  (String) S3 + "|input\n";
//  values += "S41|" +  (String) S41 + "|input\n";
//  values += "S51|" +  (String) S51 + "|input\n";
//  values += "S42|" +  (String) S42 + "|input\n";
//  values += "S52|" +  (String) S52 + "|input\n";
//  values += "S6|" +  (String) S6 + "|input\n";
//  values += "ALARM_MODE|" +  (String) (ALARM_MODE ? "checked" : "") + "|chk\n";
//  values += "L1PIR|" +  (String) L1PIR + "|input\n";
//  values += "L2PIR|" +  (String) L2PIR + "|input\n";
//  values += "L3PIR|" +  (String) L3PIR + "|input\n";
//  values += "cap_debug|" +  (String) (cap_debug ? "checked" : "") + "|chk\n";
//	values += "cap_thresold|" +  (String) cap_thresold + "|input\n";
//	values += "Altitude_id|" +  (String) ALTITUDE + "|input\n";
//	values += "usartbridge|" +  (String) (usartbridge ? "checked" : "") + "|chk\n";
//	values += "devicename|" +  (String)  DeviceName +  "|input\n";
//	values += "API|" +  (String)  API +  "|input\n";
//	values += "Send_Emon|" +  (String) (Send_Emon ? "checked" : "") + "|chk\n";
//	values += "dht_type|" +  (String)  dht_type +  "|input\n";
//	values += "dallas_qty|" +  (String)  dallas_qty +  "|input\n";
//	values += "IR_ENABLE|" +  (String) (IR_ENABLE ? "checked" : "") + "|chk\n";
//  values += "DEBUG_LOG|" +  (String) (DEBUG_LOG ? "checked" : "") + "|chk\n";
//  values += "DEBUG_CAPSENSE|" +  (String) (DEBUG_CAPSENSE ? "checked" : "") + "|chk\n";
//  values += "DEBUG_CAPSENSE_ALL|" +  (String) (DEBUG_CAPSENSE_ALL ? "checked" : "") + "|chk\n";
//  values += "DEBUG_DHT|" +  (String) (DEBUG_DHT ? "checked" : "") + "|chk\n";
//  values += "DEBUG_PRESSURE|" +  (String) (DEBUG_PRESSURE ? "checked" : "") + "|chk\n";
//  values += "DEBUG_GETLUX|" +  (String) (DEBUG_GETLUX ? "checked" : "") + "|chk\n";
//  values += "DEBUG_DALLAS|" +  (String) (DEBUG_DALLAS ? "checked" : "") + "|chk\n";
//  values += "DEBUG_EMONCMS|" +  (String) (DEBUG_EMONCMS ? "checked" : "") + "|chk\n";
//  values += "DEBUG_IR|" +  (String) (DEBUG_IR ? "checked" : "") + "|chk\n";
//  values += "DEBUG_IR_FULL|" +  (String) (DEBUG_IR_FULL ? "checked" : "") + "|chk\n";
//  values += "DEBUG_PLC|" +  (String) (DEBUG_PLC ? "checked" : "") + "|chk\n";
//
//	server.send ( 200, "text/plain", values);
//	LOG.println(__FUNCTION__); 
//  LOG.println(values);
//}

void send_general_html()
{
  
  if (server.args() > 0 )  // Save Settings
  {
    //byte0 = 0;
    //byte1 = 0;
    //byte2 = 0;
    String temp = "";
    usartbridge = false;
    for ( uint8_t i = 0; i < server.args(); i++ ) {
        if (server.argName(i) == "L1") L1 = server.arg(i).toInt(); 
        if (server.argName(i) == "L2") L2 = server.arg(i).toInt();
        if (server.argName(i) == "L3") L3 = server.arg(i).toInt();
        if (server.argName(i) == "S1") S1 = server.arg(i).toInt();
        if (server.argName(i) == "S2") S2 = server.arg(i).toInt();
        if (server.argName(i) == "S3") S3 = server.arg(i).toInt();
        if (server.argName(i) == "S41") S41 = server.arg(i).toInt();
        if (server.argName(i) == "S51") S51 = server.arg(i).toInt();
        if (server.argName(i) == "S42") S42 = server.arg(i).toInt();
        if (server.argName(i) == "S52") S52 = server.arg(i).toInt();
        //if (server.argName(i) == "S6") S6 = server.arg(i).toInt();        
        if (server.argName(i) == "L1PIR") L1PIR = server.arg(i).toInt();
        if (server.argName(i) == "L2PIR") L2PIR = server.arg(i).toInt();
        if (server.argName(i) == "L3PIR") L3PIR = server.arg(i).toInt();
        if (server.argName(i) == "cap_thresold") cap_thresold = server.arg(i).toInt();
        if (server.argName(i) == "Altitude_id") ALTITUDE = server.arg(i).toInt(); 
        if (server.argName(i) == "IR_ENABLE") IR_ENABLE = server.arg(i).toInt();
        if (server.argName(i) == "ALARM_MODE") ALARM_MODE = server.arg(i).toInt();
        if (server.argName(i) == "cap_debug") cap_debug = server.arg(i).toInt();
        if (server.argName(i) == "usartbridge") usartbridge = server.arg(i).toInt();
        if (server.argName(i) == "Send_Emon") Send_Emon = server.arg(i).toInt();
        if (server.argName(i) == "devicename") DeviceName = urldecode(server.arg(i)); 
        if (server.argName(i) == "API") API = urldecode(server.arg(i));
        if (server.argName(i) == "dht_type") dht_type = server.arg(i).toInt(); 
        if (server.argName(i) == "dallas_qty") dallas_qty = server.arg(i).toInt();   
            

    }
    WriteConfig_Slots();
    //firstStart = true;
    ESP.restart();
  }
  server.send ( 200, "text/html", PAGE_AdminGeneralSettings ); 
  LOG.println(__FUNCTION__); 
  
  
}

void send_general_configuration_values_html()
{
  String values ="";
  values += "cap_thresold|" +  (String) cap_thresold + "|input\n";
  values += "Altitude_id|" +  (String) ALTITUDE + "|input\n";
  values += "IR_ENABLE|" +  (String)  IR_ENABLE +  "|input\n";
  values += "ALARM_MODE|" +  (String)  ALARM_MODE +  "|input\n";
  values += "cap_debug|" +  (String)  cap_debug +  "|input\n";
  values += "usartbridge|" +  (String)  usartbridge +  "|input\n";
  values += "Send_Emon|" +  (String)  Send_Emon +  "|input\n";
  values += "devicename|" +  (String)  DeviceName +  "|input\n";
  values += "API|" +  (String)  API +  "|input\n";
  values += "dht_type|" +  (String)  dht_type +  "|input\n";
  values += "dallas_qty|" +  (String)  dallas_qty +  "|input\n";
  
//  server.send ( 200, "text/plain", values);
//  LOG.println(__FUNCTION__); 
//  LOG.println(values);
//  delay(500);
//  values ="";
  values += "L1|" +  (String) L1 + "|input\n";
  values += "L2|" +  (String) L2 + "|input\n";
  values += "L3|" +  (String) L3 + "|input\n";
  values += "S1|" +  (String) S1 + "|input\n";
  values += "S2|" +  (String) S2 + "|input\n";
  values += "S3|" +  (String) S3 + "|input\n";
  values += "S41|" +  (String) S41 + "|input\n";
  values += "S51|" +  (String) S51 + "|input\n";
  values += "S42|" +  (String) S42 + "|input\n";
  values += "S52|" +  (String) S52 + "|input\n";
  //values += "S6|" +  (String) S6 + "|input\n";
  values += "L1PIR|" +  (String) L1PIR + "|input\n";
  values += "L2PIR|" +  (String) L2PIR + "|input\n";
  values += "L3PIR|" +  (String) L3PIR + "|input\n";

  server.send ( 200, "text/plain", values);
  LOG.println(values);
}
