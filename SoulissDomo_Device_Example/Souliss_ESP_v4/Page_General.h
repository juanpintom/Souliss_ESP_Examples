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
  <tr>
    <td align="left" style="width:190px">Node Name</td>
    <td align="right"><input type="text" id="devicename" name="devicename" value="" maxlength="10"></td>
  </tr>
  </table>
  <td><hr></td>
  
  <div align="center" style="width:380px"><strong>Lights Configuration:(L1-L2-L3)</strong></div>
  
  <table style="width:500px">
  <tr><td>L1 Output Mode:</td><td>
    <select  id="L1" name="L1" onchange="getComboL1(this)" style="width:180px">
      <option value="0">None</option>
      <option value="1">ON-OFF MODE</option>
      <option value="2">PWM MODE</option>
      <option value="3">RGB MODE (RED)</option>
      <option value="4">PIR MODE</option>
      <option value="5">PIR ALARM MODE</option>
      <option value="6">PULSE MODE</option>
    </select>
    </td></tr>
  <tr><td>L2 Output Mode:</td><td>
    <select  id="L2" name="L2" onchange="getComboL2(this)" style="width:180px">
      <option value="0">None</option>
      <option value="1">ON-OFF MODE</option>
      <option value="2">PWM MODE</option>
      <option value="3">RGB MODE (GREEN)</option>
      <option value="4">PIR MODE</option>
      <option value="5">PIR ALARM MODE</option>
      <option value="6">PULSE MODE</option>
    </select>
    </td></tr>
  <tr><td>L3 Output Mode:</td><td>
    <select  id="L3" name="L3" onchange="getComboL3(this)"style="width:180px">
      <option value="0">None</option>
      <option value="1">ON-OFF MODE</option>
      <option value="2">PWM MODE</option>
      <option value="3">RGB MODE (BLUE)</option>
      <option value="4">PIR MODE</option>
      <option value="5">PIR ALARM MODE</option>
      <option value="6">PULSE MODE</option>
    </select>
    </td></tr>

  </table>
  
  <td><hr></td>  
  <div align="center" style="width:380px"><strong>Sensors Configuration:(S1-S2-S3)</strong></div>
  <table>
  <tr><td id="ComboA">Sensors Configuration:</td><td>
  <select  id="byte0" name="byte0" onchange="getComboA(this)">
    <option value="0">None</option>
    <option value="1">DHT |........|.......</option>
    <option value="2">.......| LDR |.......</option>
    <option value="3">.......|........| DALLAS</option>
    <option value="4">DHT | LDR |.......</option>
    <option value="5">DHT |........| DALLAS</option>
    <option value="6">.......| LDR | DALLAS</option>
    <option value="7">DHT | LDR | DALLAS</option>
  </select>
  </td></tr>
  
  <tr><td>(S1) GPIO14 Configuration: </td><td>
    <select  id="S1" name="S1" onchange="getComboS1(this)" style="width:180px">
      <option value="0">None</option>
      <option value="1">Dallas (Temperature)</option>
      <option value="2">Other</option>
    </select>
    <td align="left" id="qty">Qty: <input type="text" id="dallas_qty" name="dallas_qty" size="2" value="1"></td>
    </td></tr>
  <tr>
    
  <tr><td>(S2) GPIO16 Configuration: </td><td>
    <select  id="S2" name="S2" onchange="getComboS2(this)"style="width:180px">
      <option value="0">None</option>
      <option value="1">DHT (Temp - Humidity)</option>
      <option value="2">Rain Sensor</option>
    </select>
    <td align="left" id="type" name="type"> Type:
     <select  id="dht_type" name="dht_type">
        <option value="0">DHT11</option>
        <option value="1">DHT22</option>
        </select>
    </td>
    </td></tr>
  
  <tr><td>(S3) Analog Configuration: </td><td>
    <select  id="S3" name="S3" onchange="getComboS3(this)"style="width:180px">
      <option value="0">None</option>
      <option value="1">LDR (Luminosity)</option>
      <option value="2">Soil Sensor</option>
    </select>
    </td></tr>
  <tr id="Emoncms"><td><input type="checkbox" id="Send_Emon" name="Send_Emon" onclick="getEmon(this)"> Send to EmonCMS?: </td>
  <td id="API_text"><input type="text" id="API" name="API" value="" maxlength="32"> API Key</td></tr>
  </table>
  
  <td><hr></td>  
  
  <tr><td>(S4) Configuration: </td><td>
    <select  id="S4" name="S4" onchange="getComboS4(this)"style="width:180px">
      <option value="0">None</option>
      <option value="1">CAPACITIVE</option>
      <option value="2">CAPACITIVE /w DEBUG</option>
      <option value="3">BUTTONS</option>
      <option value="4">BUTTONS_PULLUP</option>
      <option value="5">ALARM_ENDSTOP</option>
      <option value="6">BUTTONS_2_STATE</option>
    </select>
    </td></tr>
    
  <tr><td>(S5) Configuration: </td><td>
    <select  id="S5" name="S5" onchange="getComboS5(this)"style="width:180px">
      <option value="0">None</option>
      <option value="1">RELAY</option>
      <option value="2">PULSE_OUTPUT</option>
      <option value="3">PRESSURE_SENSOR</option>
      <option value="4">GARAGE_DOOR</option>
      <option value="5">WINDOW_CURTAIN</option>
      <option value="6">220V_OPTO_AND_RELAY</option>
    </select>
    </td></tr>
  
  <div align="center" style="width:280px"><strong>Other Sensors: (S4-S5)</strong></div>
  <table style="width:500px">
  <tr><td style="width:170px">Others: (S4-S5) </td><td>
    <select id="byte2" name="byte2" onchange="getComboC(this)">
      <option value="0">None</option>
      <option value="1">(S5) CAPACITIVE</option>
      <option value="2">(S4) RELAY</option>
      <option value="3">(S5) PRESSURE SENSOR</option>
      <option value="4">(S5) CAPACITIVE /w DEBUG</option>
      <option value="5">(S5) BUTTONS</option>
      <option value="6">(S5) BUTTONS_PULLUP</option>
      <option value="7">(S5) ALARM_ENDSTOP</option>
      <option value="8">(S5) BUTTONS_2_STATE</option>
      <option value="9">(S4) PULSE_OUTPUT</option>
      <option value="10">(S4) GARAGE_DOOR</option>
      <option value="11">(S4) WINDOW_CURTAIN</option>
      <option value="12">(S4) 220V_OPTO_AND_RELAY</option>
    </select>
    </td></tr>
  
 <tr id="Thresold">
  <td align="left" id="cap_thresold_line"> Capacitive Thresold:</td>
  <td><input type="text" id="cap_thresold" name="cap_thresold" size="2" value="5"></td>
 </tr>
 <tr id="Altitude">
  <td align="left" id="Altitude_line"> Altitude:</td>
  <td><input type="text" id="Altitude_id" name="Altitude_id" size="2" value="20"></td>
 </tr>
 </table>

  <td><hr></td>    
    
  <table>
    <tr>
    <td align="left" style="width:230px">Receive IR on this Node? (S6):</td>
    <td><input type="checkbox" id="IR_ENABLE" name="IR_ENABLE" onclick="getChk(this)"></td>
    </tr></table>
    
  <td style="width:500px"><hr></td>
  <table>
    <tr>
    <td align="left" style="width:230px"> Node is USART Bridge?: (S4)</td>
    <td><input type="checkbox" id="usartbridge" name="usartbridge" onclick="getUSART(this)"></td>
    </tr></table>
  
  <td style="width:500px"><hr></td>
  <table>
    <tr><td align="left" style="width:230px"><strong>DEBUG Configuration:</strong></td>
    <td><input type="checkbox" id="DEBUG" name="DEBUG" onclick="getDebug(this)"> <-Check to Show</td></tr></table>
    
  <table id="LOG">
    <tr><td><input type="checkbox" id="ALL_LOG" name="ALL_LOG" onclick="getAllLog(this)"><strong>Enable All </strong></td></tr>
    <tr><td><input type="checkbox" id="DEBUG_LOG" name="DEBUG_LOG">Debug LOG</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_CAPSENSE" name="DEBUG_CAPSENSE">Debug CAPSENSE</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_CAPSENSE_ALL" name="DEBUG_CAPSENSE_ALL">Debug DEBUG_CAPSENSE_ALL</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_DHT" name="DEBUG_DHT">Debug DHT</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_PRESSURE" name="DEBUG_PRESSURE">Debug PRESSURE</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_GETLUX" name="DEBUG_GETLUX">Debug GETLUX</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_DALLAS" name="DEBUG_DALLAS">Debug DALLAS</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_EMONCMS" name="DEBUG_EMONCMS">Debug EMONCMS</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_IR" name="DEBUG_IR">Debug IR</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_IR_FULL" name="DEBUG_IR_FULL">Debug IR_FULL</td></tr>
    <tr><td><input type="checkbox" id="DEBUG_PLC" name="DEBUG_PLC">Debug PLC</td></tr>
  </table>
    <td style="width:500px"><hr></td>
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
  getConfig();
  if("js"==t){
    var a=document.createElement("script");
    a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)
  }else if("css"==t){
    var a=document.createElement("link");
    a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)
  }
}
function getConfig(){
    document.getElementById("ComboA").style.display = "none";  //QUITAR!!
    document.getElementById("byte0").style.display = "none";  //QUITAR!!
    
    if(document.getElementById("S2").value == 1){
      document.getElementById("type").style.display = "block";  //none;  
    }else{
      document.getElementById("type").style.display = "none";  //none;  
    }
    if(document.getElementById("S1").value == 1){
      document.getElementById("qty").style.display = "block";  //none;  
    }else{
      document.getElementById("qty").style.display = "none";  //none;  
    }
    
    if(document.getElementById("Send_Emon").checked){
      document.getElementById("API_text").style.display = "block";  //none;  
    } else { 
      document.getElementById("API_text").style.display = "none"; 
    } 
    
    value = document.getElementById("byte2");
    if(value == 1 || value == 4) {
       document.getElementById("Thresold").style.display = "block";  //none;  
    } else { 
       document.getElementById("Thresold").style.display = "none"; 
    }
   
    if(value == 3) {
      document.getElementById("Altitude").style.display = "block";  //none;  
    } else {
      document.getElementById("Altitude").style.display = "none";  
    }
    
    if(document.getElementById("DEBUG").checked){
      document.getElementById("LOG").style.display = "block";  //none;  
    }else{
      document.getElementById("LOG").style.display = "none";  //none;  
    }
}
   
function getComboS1(sel) {
    if(sel.value == 1){
      document.getElementById("qty").style.display = "block";  //none;  
    } else { 
      document.getElementById("qty").style.display = "none"; 
    } 
}
  
function getComboS2(sel) {
    if(sel.value == 1){
      document.getElementById("type").style.display = "block";  //none;  
    } else { 
      document.getElementById("type").style.display = "none"; 
    }
    if(sel.value == 2){
      //alert("RAIN")
    }  
}
 
  
function getComboS3(sel) {
    if(sel.value == 1){
      //alert("LDR")
    }
}
  
function getEmon(sel) {
  if(sel.checked){
    document.getElementById("API_text").style.display = "block";  //none;  
  } else { 
    document.getElementById("API_text").style.display = "none"; 
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
}
  
  
function getComboC(sel) {
  var value = sel.value;
  if(value == 1 || value == 4) {
       document.getElementById("Thresold").style.display = "block";  //none;  
  } else { 
       document.getElementById("Thresold").style.display = "none"; 
  }
   
  if(value == 3) {
      document.getElementById("Altitude").style.display = "block";  //none;  
  } else {
      document.getElementById("Altitude").style.display = "none";  
  }
  if(value == 2 || value == 9 || value == 10 || value == 11 || value == 12){
    document.getElementById("usartbridge").checked = false;  
  
  }
}
function getUSART(sel){
  if(sel.checked){
    document.getElementById("byte2").value = 0;   
  }
}
  
function getDebug(sel){
  if(sel.checked){
    document.getElementById("LOG").style.display = "block";  //none;  
  } else {
    document.getElementById("LOG").style.display = "none";  
  }
}
  
function getAllLog(sel){
  if(sel.checked){
    document.getElementById("DEBUG_LOG").checked = true;    
    document.getElementById("DEBUG_CAPSENSE").checked = true;  
    document.getElementById("DEBUG_CAPSENSE_ALL").checked = true;  
    document.getElementById("DEBUG_DHT").checked = true;  
    document.getElementById("DEBUG_PRESSURE").checked = true;  
    document.getElementById("DEBUG_GETLUX").checked = true;  
    document.getElementById("DEBUG_DALLAS").checked = true;  
    document.getElementById("DEBUG_EMONCMS").checked = true;  
    document.getElementById("DEBUG_IR").checked = true;
    document.getElementById("DEBUG_IR_FULL").checked = true;
    document.getElementById("DEBUG_PLC").checked = true;
  } else {
    document.getElementById("DEBUG_LOG").checked = false;    
    document.getElementById("DEBUG_CAPSENSE").checked = false;  
    document.getElementById("DEBUG_CAPSENSE_ALL").checked = false;  
    document.getElementById("DEBUG_DHT").checked = false;  
    document.getElementById("DEBUG_PRESSURE").checked = false;  
    document.getElementById("DEBUG_GETLUX").checked = false;  
    document.getElementById("DEBUG_DALLAS").checked = false;  
    document.getElementById("DEBUG_EMONCMS").checked = false;  
    document.getElementById("DEBUG_IR").checked = false;
    document.getElementById("DEBUG_IR_FULL").checked = false;
    document.getElementById("DEBUG_PLC").checked = false;
  }
}
</script>
)=====";

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
        if (server.argName(i) == "S4") S4 = server.arg(i).toInt();
        if (server.argName(i) == "S5") S5 = server.arg(i).toInt();
        if (server.argName(i) == "S6") S6 = server.arg(i).toInt(); 
     		if (server.argName(i) == "cap_thresold") cap_thresold = server.arg(i).toInt();
      	if (server.argName(i) == "Altitude_id") ALTITUDE = server.arg(i).toInt();
      	if (server.argName(i) == "usartbridge") usartbridge = true;
      	if (server.argName(i) == "devicename") DeviceName = urldecode(server.arg(i)); 
      	if (server.argName(i) == "API") API = urldecode(server.arg(i));
      	if (server.argName(i) == "Send_Emon") Send_Emon = true; 
      	if (server.argName(i) == "dht_type") dht_type = server.arg(i).toInt(); 
      	if (server.argName(i) == "dallas_qty") dallas_qty = server.arg(i).toInt(); 
      	if (server.argName(i) == "IR_ENABLE") IR_ENABLE = true;
        if (server.argName(i) == "DEBUG_LOG") DEBUG_LOG = true;
        if (server.argName(i) == "DEBUG_CAPSENSE") DEBUG_CAPSENSE = true;
        if (server.argName(i) == "DEBUG_CAPSENSE_ALL") DEBUG_CAPSENSE_ALL = true;
        if (server.argName(i) == "DEBUG_DHT") DEBUG_DHT = true;
        if (server.argName(i) == "DEBUG_PRESSURE") DEBUG_PRESSURE = true;
        if (server.argName(i) == "DEBUG_GETLUX") DEBUG_GETLUX = true;
        if (server.argName(i) == "DEBUG_DALLAS") DEBUG_DALLAS = true;
        if (server.argName(i) == "DEBUG_EMONCMS") DEBUG_EMONCMS = true;
        if (server.argName(i) == "DEBUG_IR") DEBUG_IR = true;
        if (server.argName(i) == "DEBUG_IR_FULL") DEBUG_IR_FULL = true; 
        if (server.argName(i) == "DEBUG_PLC") DEBUG_PLC = true;
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
	values += "L1|" +  (String) L1 + "|input\n";
	values += "L2|" +  (String) L2 + "|input\n";
	values += "L3|" +  (String) L3 + "|input\n";
  values += "S1|" +  (String) L1 + "|input\n";
  values += "S2|" +  (String) L2 + "|input\n";
  values += "S3|" +  (String) L3 + "|input\n";
  values += "S4|" +  (String) L1 + "|input\n";
  values += "S5|" +  (String) L2 + "|input\n";
  values += "S6|" +  (String) L3 + "|input\n";
	values += "cap_thresold|" +  (String) cap_thresold + "|input\n";
	values += "Altitude_id|" +  (String) ALTITUDE + "|input\n";
	values += "usartbridge|" +  (String) (usartbridge ? "checked" : "") + "|chk\n";
	values += "devicename|" +  (String)  DeviceName +  "|input\n";
	values += "API|" +  (String)  API +  "|input\n";
	values += "Send_Emon|" +  (String) (Send_Emon ? "checked" : "") + "|chk\n";
	values += "dht_type|" +  (String)  dht_type +  "|input\n";
	values += "dallas_qty|" +  (String)  dallas_qty +  "|input\n";
	values += "IR_ENABLE|" +  (String) (IR_ENABLE ? "checked" : "") + "|chk\n";
  values += "DEBUG_LOG|" +  (String) (DEBUG_LOG ? "checked" : "") + "|chk\n";
  values += "DEBUG_CAPSENSE|" +  (String) (DEBUG_CAPSENSE ? "checked" : "") + "|chk\n";
  values += "DEBUG_CAPSENSE_ALL|" +  (String) (DEBUG_CAPSENSE_ALL ? "checked" : "") + "|chk\n";
  values += "DEBUG_DHT|" +  (String) (DEBUG_DHT ? "checked" : "") + "|chk\n";
  values += "DEBUG_PRESSURE|" +  (String) (DEBUG_PRESSURE ? "checked" : "") + "|chk\n";
  values += "DEBUG_GETLUX|" +  (String) (DEBUG_GETLUX ? "checked" : "") + "|chk\n";
  values += "DEBUG_DALLAS|" +  (String) (DEBUG_DALLAS ? "checked" : "") + "|chk\n";
  values += "DEBUG_EMONCMS|" +  (String) (DEBUG_EMONCMS ? "checked" : "") + "|chk\n";
  values += "DEBUG_IR|" +  (String) (DEBUG_IR ? "checked" : "") + "|chk\n";
  values += "DEBUG_IR_FULL|" +  (String) (DEBUG_IR_FULL ? "checked" : "") + "|chk\n";
  values += "DEBUG_PLC|" +  (String) (DEBUG_PLC ? "checked" : "") + "|chk\n";
  
 
	server.send ( 200, "text/plain", values);
	LOG.println(__FUNCTION__); 
}
