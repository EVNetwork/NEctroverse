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

SD = window.setTimeout("countDown();", 1000);

if (min == '00' && sec == '00') { getInfo("ticker"); } 

}

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


