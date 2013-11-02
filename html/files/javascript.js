// Yer, better make a timer... this can contol the call's to update info too --- min and sec will be defined on output of html
var login = false;
var tickmsg = "Game is ticking!";

function countDown() {

sec--;

if (sec == -01) {
	sec = 59;
	min = min - 1;
} else {
	min = min;
}

time = (min < 0 ? min + " minute and " : "" ) + sec + " seconds";

if( document.getElementById('headerTime') ) { document.getElementById('headerTime').innerHTML = "<b>Next Tick: " + time + "</b>"; }
if( document.getElementById('hqTime') ) { document.getElementById('hqTime').innerHTML = time; }
if( document.getElementById('sstatsTime') ) { document.getElementById('sstatsTime').innerHTML = time; }

SD=window.setTimeout("countDown();", 1000);

if (min == '00' && sec == '00') { 
	getInfo("tick");
	if( login ) {
		getInfo("basic");
	}
	if( document.getElementById('headerTime') ) { document.getElementById('headerTime').innerHTML = "<b>" + tickmsg + "</b>"; }
	if( document.getElementById('hqTime') ) { document.getElementById('hqTime').innerHTML = tickmsg; }
	if( document.getElementById('sstatsTime') ) { document.getElementById('sstatsTime').innerHTML = tickmsg; }
}

}

function addLoadEvent(func) {
	var oldonload = window.onload;
	if (typeof window.onload != 'function') {
		window.onload = func;
	} else {
		window.onload = function() {
			if (oldonload) {
				oldonload();
			}
			func();
		}
	}
}

addLoadEvent(function(){countDown();});

// OK, Time for some AJAX! =/ ... 
// It's been a while, forgive me if its messy lol! =P
function getInfo(str) {
	var xmlhttp;
if (str=="") {
	return;
}
if (window.XMLHttpRequest) { 
	xmlhttp=new XMLHttpRequest(); 
} else { 
	xmlhttp=new ActiveXObject("Microsoft.XMLHTTP"); 
}

xmlhttp.onreadystatechange=function() {
	if (xmlhttp.readyState==4 && xmlhttp.status==200) {
		if(str == "tick") {
			//Fetch basic tick data, and check if user is loged in... this controls weither we will call more data.
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("pass")[0].childNodes[0].nodeValue > 0 ) { 
				if(login == false) { 
					login = true; 
					getInfo("basic"); 
				} 
			} else { login = false; }
			sec=xmlhttp.responseXML.documentElement.getElementsByTagName("next")[0].childNodes[0].nodeValue;
			SD=window.setTimeout("countDown();", 1000);
			if( document.getElementById('hqweeks') ) { 
				document.getElementById("hqweeks").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("week")[0].childNodes[0].nodeValue;
			}
			if( document.getElementById('hqyears') ) { 
				document.getElementById("hqyears").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("year")[0].childNodes[0].nodeValue;
			}
			if( document.getElementById('sstatweeks') ) { 
				document.getElementById("sstatweeks").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("week")[0].childNodes[0].nodeValue;
			}
			if( document.getElementById('sstatyears') ) { 
				document.getElementById("sstatyears").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("year")[0].childNodes[0].nodeValue;
			}
		}
		if(str == "basic") {
			//Okie, Update basic user infos...
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("population")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('headerpopulation') ) { 
				document.getElementById("headerpopulation").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("population")[0].childNodes[0].nodeValue;
			}
			if( document.getElementById('hqpopulation') ) { 
				document.getElementById("hqpopulation").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("population")[0].childNodes[0].nodeValue;
			}
			}
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("networth")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('headernetworth') ) { 
				document.getElementById("headernetworth").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("networth")[0].childNodes[0].nodeValue;
			}
			if( document.getElementById('hqnetworth') ) { 
				document.getElementById("hqnetworth").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("networth")[0].childNodes[0].nodeValue;
			}
			}
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("energy")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('headerenergy') ) { 
				document.getElementById("headerenergy").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("energy")[0].childNodes[0].nodeValue;
			}
			}
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("mineral")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('headermineral') ) { 
				document.getElementById("headermineral").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("mineral")[0].childNodes[0].nodeValue;
			}
			}
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("crystal")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('headercrystal') ) { 
				document.getElementById("headercrystal").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("crystal")[0].childNodes[0].nodeValue;
			}
			}
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("ectrolium")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('headerectrolium') ) { 
				document.getElementById("headerectrolium").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("ectrolium")[0].childNodes[0].nodeValue;
			}
			}

			if( xmlhttp.responseXML.documentElement.getElementsByTagName("planets")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('hqplanets') ) { 
				document.getElementById("hqplanets").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("planets")[0].childNodes[0].nodeValue;
			}
			}
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("fleetready")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('hqfleetready') ) { 
				document.getElementById("hqfleetready").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("fleetready")[0].childNodes[0].nodeValue;
			}
			}
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("psychicsready")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('hqpsychready') ) { 
				document.getElementById("hqpsychready").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("psychicsready")[0].childNodes[0].nodeValue;
			}
			}
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("agentsready")[0].childNodes[0].nodeValue > 1 ) { 
			if( document.getElementById('hqagentready') ) { 
				document.getElementById("hqagentready").innerHTML=xmlhttp.responseXML.documentElement.getElementsByTagName("agentsready")[0].childNodes[0].nodeValue;
			}
			}

			// Here we set images for mail, build etc... we shouldn't need the double check here since these images are unique enough
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("mail")[0].childNodes[0].nodeValue == 1 ) { changeImage("headermail", "images/i09a.jpg"); }
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("build")[0].childNodes[0].nodeValue == 1 ) { changeImage("headerbuild", "images/i10a.jpg"); }
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("aid")[0].childNodes[0].nodeValue == 1 ) { changeImage("headeraid", "images/i11a.jpg"); }
			if( xmlhttp.responseXML.documentElement.getElementsByTagName("fleet")[0].childNodes[0].nodeValue == 1 ) { changeImage("headerfleet", "images/i12a.jpg"); }
			else if( xmlhttp.responseXML.documentElement.getElementsByTagName("fleet")[0].childNodes[0].nodeValue == 2 ) { changeImage("headerfleet", "images/i12b.jpg"); }
		}

	} else {
		if(str == "tick")
		window.clearTimeout(SD);
	}
}
xmlhttp.open("GET","ajax?typ="+str,true);
xmlhttp.send();
}


function changeImage(img, a) {
    document.getElementById(img).src=a;
}


function togglemb(num) { for(i=0;i<document.forms[num].length;i++) if(document.forms[num].elements[i].type == "checkbox") document.forms[num].elements[i].click(); }



