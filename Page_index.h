

//
//  HTML PAGE
//

const char PAGE_index[] PROGMEM = R"=====(
<html>
<head>
<title>TexTime</title>
<meta name="viewport" content="width=device-width, initial-scale=1" />
</head>
<body>
<strong>TexTime</strong>
<hr>
<a href="/info.html" style="width:250px" class="btn btn--m btn--blue" >System Information</a><br>
<a href="/general.html" style="width:250px" class="btn btn--m btn--blue" >General Configuration</a><br>
<a href="/network.html" style="width:250px" class="btn btn--m btn--blue" >Network Configuration</a><br>
<a href="/ntp.html" style="width:250px" class="btn btn--m btn--blue" >NTP Configuration</a><br>
<a href="/update" style="width:250px" class="btn btn--m btn--blue" >Update Firmware</a><br>
<hr>
<span><a href="http://www.psykokwak.com/" style="text-decoration:none" >By Psykokwak</a></span><br>


<script>
window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				// Do something after load...
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>

</body>

)=====";


