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
updatehtml('headerTime',"<b>Next Tick: " + time + "</b>");
updatehtml('hqTime',time);
updatehtml('sstatsTime',time);

SD=window.setTimeout("countDown();", 1000);

if (min == '00' && sec == '00') { 
	getInfo("tick");
	if( login ) {
		getInfo("basic");
	}
	updatehtml('headerTime',"<b>" + tickmsg + "</b>");
	updatehtml('hqTime',tickmsg);
	updatehtml('sstatsTime',tickmsg);
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
			var week;
			var year;
			//Fetch basic tick data, and check if user is loged in... this controls weither we will call more data.
			if( getnodevar(xmlhttp.responseXML,"pass") > 0 ) { 
				if(login == false) { 
					login = true; 
					getInfo("basic"); 
				} 
			} else { login = false; }
			sec=getnodevar(xmlhttp.responseXML,"next");
			SD=window.setTimeout("countDown();", 1000);
			week = getnodevar(xmlhttp.responseXML,"week");
			year = getnodevar(xmlhttp.responseXML,"year");
			updatehtml("hqweeks",week);
			updatehtml("hqyears",year);
			updatehtml("sstatweeks",week);
			updatehtml("sstatyears",year);

		}
		if(str == "basic") {
			//Okie, Update basic user infos...
			var population;
			var networth; var planets;
			var energy; var mineral; var crystal; var ectrolium;
			var fready; var pready; var aready;

			population = getnodevar(xmlhttp.responseXML,"population");
			networth = getnodevar(xmlhttp.responseXML,"networth");
			planets = getnodevar(xmlhttp.responseXML,"planets");

			energy = getnodevar(xmlhttp.responseXML,"energy");
			mineral = getnodevar(xmlhttp.responseXML,"mineral");
			crystal = getnodevar(xmlhttp.responseXML,"crystal");
			ectrolium = getnodevar(xmlhttp.responseXML,"ectrolium");

			fready = getnodevar(xmlhttp.responseXML,"fleetready");
			pready = getnodevar(xmlhttp.responseXML,"psychicsready");
			aready = getnodevar(xmlhttp.responseXML,"agentsready");

			updatehtml("headerpopulation",population);
			updatehtml("hqpopulation",population);
			updatehtml("headernetworth",networth);
			updatehtml("hqnetworth",networth);
			updatehtml("hqplanets",planets);


			updatehtml("headerenergy",energy);
			updatehtml("headermineral",mineral);
			updatehtml("headercrystal",crystal);
			updatehtml("headerectrolium",ectrolium);

			updatehtml("hqfleetready",fready);
			updatehtml("hqpsychready",pready);
			updatehtml("hqagentready",aready);
			
			// Here we set images for mail, build etc... we shouldn't need the double check here since these images are unique enough
			if( getnodevar(xmlhttp.responseXML,"mail") == 1 ) { changeimage("headermail", "images/i09a.jpg"); }
			if( getnodevar(xmlhttp.responseXML,"build") == 1 ) { changeimage("headerbuild", "images/i10a.jpg"); }
			if( getnodevar(xmlhttp.responseXML,"aid") == 1 ) { changeimage("headeraid", "images/i11a.jpg"); }
			if( getnodevar(xmlhttp.responseXML,"fleet") == 1 ) { changeimage("headerfleet", "images/i12a.jpg"); }
			else if( getnodevar(xmlhttp.responseXML,"fleet") == 2 ) { changeimage("headerfleet", "images/i12b.jpg"); }
		}

	} else {
		if(str == "tick")
		window.clearTimeout(SD);
	}
}
xmlhttp.open("GET","ajax?typ="+str,true);
xmlhttp.send();
}

function getnodevar(xmlin,tagvar) {
	var node;
	var input;

node = xmlin.documentElement.getElementsByTagName(tagvar)[0].childNodes[0];

if( ( node ) && ( node.nodeValue != "undefined" ) ) { input = node.nodeValue; } else { input = ""; }

return input;
}


function changeimage(img, a) { if( document.getElementById(img) ) { document.getElementById(img).src=a; } else { return; } }

function updatehtml(id, i) { if((i == "")||(i == "undefined")){ return; } if( document.getElementById(id) ) { document.getElementById(id).innerHTML=i; } else { return; } }

function togglemb(num) { for(i=0;i<document.forms[num].length;i++) if(document.forms[num].elements[i].type == "checkbox") document.forms[num].elements[i].click(); }



