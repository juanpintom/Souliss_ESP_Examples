///
//  HTML PAGE
//

const char PAGE_AdminGeneralSettings[] PROGMEM =  R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Souliss Slot Settings</strong>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" >
	
	<tr><td>Sensors Configuration:</td><td>
	<select  id="byte0" name="byte0">
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
	<tr><td>Lights Output Mode:</td><td>
	<select  id="byte1" name="byte1">
		<option value="0">None</option>
		<option value="1">PWM MODE</option>
		<option value="2">PIR_MODE</option>
		<option value="3">RGB_MODE</option>
                <option value="4">PIR_ALARM_MODE</option>
                <option value="5">THERMOSTAT_MODE</option>
	</select>
	</td></tr>
	
	<tr><td>Others: </td><td>
	<select  id="byte2" name="byte2">
		<option value="0">None</option>
		<option value="1">CAPACITIVE</option>
		<option value="2">RELAY</option>
		<option value="3">PRESSURE SENSOR</option>
		<option value="4">CAPACITIVE_DEBUG</option>
	</select>
	</td></tr>
        <tr>
        <td align="left" id="cap_thresold_line"> Capacitive Thresold:</td>
        <td><input type="text" id="cap_thresold" name="cap_thresold" size="2" value="5"></td>
	</tr>
        <tr>
        <td align="left" id="Altitude_line"> Altitude:</td>
        <td><input type="text" id="Altitude_id" name="Altitude_id" size="2" value="20"></td>
	</tr>

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
  if("js"==t){
    var a=document.createElement("script");
    a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)
  }else if("css"==t){
    var a=document.createElement("link");
    a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}
  }
  if(byte == 3) {
      document.getElementById("Altitude_line").style.display = "none";  //none;  
      document.getElementById("Altitude_id").style.display = "none";  //none;
  }
  else {
      document.getElementById("Altitude_line").style.display = "block"; 
      document.getElementById("Altitude_id").style.display = "block";    
  }
  
</script>
)=====";

void send_general_html()
{
	
	if (server.args() > 0 )  // Save Settings
	{
		byte0 = 0;
		byte1 = 0;
		byte2 = 0;
		String temp = "";
		for ( uint8_t i = 0; i < server.args(); i++ ) {
		    if (server.argName(i) == "byte0") byte0 = server.arg(i).toInt(); 
		    if (server.argName(i) == "byte1") byte1 = server.arg(i).toInt(); 
		    if (server.argName(i) == "byte2") byte2 = server.arg(i).toInt(); 
       		    if (server.argName(i) == "cap_thresold") cap_thresold = server.arg(i).toInt();
                    if (server.argName(i) == "Altitude_id") ALTITUDE = server.arg(i).toInt();
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
	values += "byte0|" +  (String) byte0 + "|input\n";
	values += "byte1|" +  (String) byte1 + "|input\n";
	values += "byte2|" +  (String) byte2 + "|input\n";
	values += "cap_thresold|" +  (String) cap_thresold + "|input\n";
        values += "Altitude_id|" +  (String) ALTITUDE + "|input\n";
        server.send ( 200, "text/plain", values);
	LOG.println(__FUNCTION__); 
}
