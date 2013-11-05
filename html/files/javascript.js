//Yer, better make a timer... this can contol the call's to update info too --- min and sec will be defined on output of html

function countDown() {

sec--;

if (sec == -01) {
	if ( min > 0 ) {
		sec = 59;
		min = min - 1;
	} else {
		sec = 00;
		getInfo("ticker");
		SD=window.setTimeout("countDown();", 15000);
		login = false;
		return;
	}
} else {
	min = min;
}

time = (min < 0 ? min + " minute and " : "" ) + sec + " seconds";
updatehtml('headerTime',time);
updatehtml('hqTime',time);
updatehtml('sstatsTime',time);

SD=window.setTimeout("countDown();", 1000);

if (min == '00' && sec == '00') { getInfo("ticker"); } 

}
setInterval( function(){
var stime;

gsec++;
if(gsec == '60') {
gsec='00';
gmin++;
}
if(gmin == '60') {
gmin='00';
ghour++;
}
if(ghour == '60') {
ghour='00';
gday++;
}

stime =  (gday > 0 ? gday + " day" : "" ) + (gday > 1 ? "s " : ( gday > 0 ? " " : "" ));

stime += (ghour > 0 ? ghour + " hour" : "" ) + (ghour > 1 ? "s " : ( ghour > 0 ? " " : "" ) );

stime += (gmin > 0 ? gmin + " minute" : "" ) + (gmin > 1 ? "s " : ( gmin > 0 ? " " : "" ) );

stime += gsec + " second"+ (gsec > 1 ? "s" : ( gsec > 0 ? " " : "" ) );


	updatehtml("gameuptime",stime);

}, 1000);


setInterval( function hosttimer(){
var htime;

hsec++;
if(hsec == '60') {
hsec='00';
hmin++;
}
if(hmin == '60') {
hmin='00';
hhour++;
}
if(hhour == '60') {
hhour='00';
hday++;
}

htime =  (hday > 0 ? hday + " day" : "" ) + (hday > 1 ? "s " : ( hday > 0 ? " " : "" ) );

htime += (hhour > 0 ? hhour + " hour" : "" ) + (hhour > 1 ? "s " : ( hhour > 0 ? " " : "" ) );

htime += (hmin > 0 ? hmin + " minute" : "" ) + (hmin > 1 ? "s " : ( hmin > 0 ? " " : "" ) );

htime += hsec + " second"+ (hsec > 1 ? "s" : ( hsec > 0 ? " " : "" ) );


	updatehtml("hostuptime",htime);
}, 1000);

function getnodevar(xmlin,tagvar) {
	var node;
	var input;

try {
	node = xmlin.documentElement.getElementsByTagName(tagvar)[0].childNodes[0];
	if( node.nodeValue != "undefined" ) { input = node.nodeValue; }
} catch(err) {
	input = "";
}

return input;
}


function changeimage(img, a) { 

try {
	document.getElementById(img).src=a; 
} catch(err) {
	return;
}

}

function updatehtml(id, i) { 

if(i==""){ return; }
else if(i=="undefined"){ return; }

try {
	document.getElementById(id).innerHTML=i;
} catch(err) {
	return;
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

function togglemb(num) { for(i=0;i<document.forms[num].length;i++) if(document.forms[num].elements[i].type == "checkbox") document.forms[num].elements[i].click(); }

function pad ( val ) { return val > 9 ? val : "0" + val; }

function changeslider(newValue,field) {
	document.getElementById(field).value=newValue;
}

