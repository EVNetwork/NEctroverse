// Yer, better make a timer... this can contol the call's to update info too --- min and sec will be defined on output of html
var page;
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
	getInfo("ticker");
	updatehtml('headerTime',"<b>" + tickmsg + "</b>");
	if( page == "hq" ) {
		updatehtml('hqTime',tickmsg);
	} else if ( page == "status" ) {
		updatehtml('sstatsTime',tickmsg);
	}
}

}

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
		//Fetch uinfos tick data, and check if user is loged in... this controls weither we will call more data.
		if( getnodevar(xmlhttp.responseXML,"pass") > 0 ) { 
			if(login == false) { 
				login = true; 
			} 
		} else { login = false; }
		sec = getnodevar(xmlhttp.responseXML,"next");
		SD = window.setTimeout("countDown();", 1000);
		page = getnodevar(xmlhttp.responseXML,"page");
		var week = getnodevar(xmlhttp.responseXML,"week");
		var year = getnodevar(xmlhttp.responseXML,"year");
		if( page == "hq" ) {
			updatehtml("hqweeks",week);
			updatehtml("hqyears",year);
		} else if ( page == "status" ) {
			updatehtml("sstatweeks",week);
			updatehtml("sstatyears",year);
		}

		if( login ) {
			//Okie, Update uinfos user infos...
			var population = getnodevar(xmlhttp.responseXML,"population");
			var networth = getnodevar(xmlhttp.responseXML,"networth");
			var planets = getnodevar(xmlhttp.responseXML,"planets");

			var energy = getnodevar(xmlhttp.responseXML,"energy");
			var mineral = getnodevar(xmlhttp.responseXML,"mineral");
			var crystal = getnodevar(xmlhttp.responseXML,"crystal");
			var ectrolium = getnodevar(xmlhttp.responseXML,"ectrolium");

			updatehtml("headerenergy",energy);
			updatehtml("headermineral",mineral);
			updatehtml("headercrystal",crystal);
			updatehtml("headerectrolium",ectrolium);

			if( page == "hq" ) {
				var fready = getnodevar(xmlhttp.responseXML,"fleetready");
				var pready = getnodevar(xmlhttp.responseXML,"psychicsready");
				var aready = getnodevar(xmlhttp.responseXML,"agentsready");
				updatehtml("headerpopulation",population);
				updatehtml("hqpopulation",population);
				updatehtml("headernetworth",networth);
				updatehtml("hqnetworth",networth);
				updatehtml("hqplanets",planets);
				updatehtml("hqfleetready",fready);
				updatehtml("hqpsychready",pready);
				updatehtml("hqagentready",aready);
			} else if ( page == "council" ) {
				var energyincome = getnodevar(xmlhttp.responseXML,"energyincome");
				var mineralincome = getnodevar(xmlhttp.responseXML,"mineralincome");
				var crystalincome = getnodevar(xmlhttp.responseXML,"crystalincome");
				var ectroliumincome = getnodevar(xmlhttp.responseXML,"ectroliumincome");
				var energyproduction = getnodevar(xmlhttp.responseXML,"energyproduction");
				var energydecay = getnodevar(xmlhttp.responseXML,"energydecay");
				var buildingupkeep = getnodevar(xmlhttp.responseXML,"buildingupkeep");
				var populationreduction = getnodevar(xmlhttp.responseXML,"populationreduction");
				var portalsupkeep = getnodevar(xmlhttp.responseXML,"portalsupkeep");
				var unitupkeep = getnodevar(xmlhttp.responseXML,"unitupkeep");
				var mineralproduction = getnodevar(xmlhttp.responseXML,"mineralproduction");
				var crystalproduction = getnodevar(xmlhttp.responseXML,"crystalproduction");
				var crystaldecay = getnodevar(xmlhttp.responseXML,"crystaldecay");
				var ectroliumproduction = getnodevar(xmlhttp.responseXML,"ectroliumproduction");


				updatehtml("energyincome",( energyincome >= 0 ? "+" : "<span class=\"genred\">" ) + energyincome + ( energyincome >= 0 ? "" : "</span>" ));
				updatehtml("mineralincome",( mineralincome >= 0 ? "+" : "" ) + mineralincome);
				updatehtml("crystalincome",( crystalincome >= 0 ? "+" : "<span class=\"genred\">" ) + crystalincome + ( crystalincome >= 0 ? "" : "</span>" ));
				updatehtml("ectroliumincome",( ectroliumincome >= 0 ? "+" : "" ) + ectroliumincome);
				updatehtml("energyproduction","+" + energyproduction);
				updatehtml("energydecay","-" + energydecay);
				updatehtml("buildingupkeep","-" + buildingupkeep);
				updatehtml("populationreduction","+" + populationreduction);
				updatehtml("portalsupkeep","-" + portalsupkeep);
				updatehtml("unitupkeep","-" + unitupkeep);
				updatehtml("mineralproduction","+" + mineralproduction);
				updatehtml("crystalproduction","+" + crystalproduction);
				updatehtml("crystaldecay","-" + crystaldecay);
				updatehtml("ectroliumproduction","+" + ectroliumproduction);

				var empiretax = getnodevar(xmlhttp.responseXML,"taxlevel");
				var energytax = getnodevar(xmlhttp.responseXML,"energytax");
				var mineraltax = getnodevar(xmlhttp.responseXML,"mineraltax");
				var crystaltax = getnodevar(xmlhttp.responseXML,"crystaltax");
				var ectroliumtax = getnodevar(xmlhttp.responseXML,"ectroliumtax");

				updatehtml("counciltax",empiretax);
				updatehtml("energytax","-" + energytax);
				updatehtml("mineraltax","-" + mineraltax);
				updatehtml("crystaltax","-" + crystaltax);
				updatehtml("ectroliumtax","-" + ectroliumtax);


				var bld0 = getnodevar(xmlhttp.responseXML,"bld0");
				var bld1 = getnodevar(xmlhttp.responseXML,"bld1");
				var bld2 = getnodevar(xmlhttp.responseXML,"bld2");
				var bld3 = getnodevar(xmlhttp.responseXML,"bld3");
				var bld4 = getnodevar(xmlhttp.responseXML,"bld4");
				var bld5 = getnodevar(xmlhttp.responseXML,"bld5");
				var bld6 = getnodevar(xmlhttp.responseXML,"bld6");
				var bld7 = getnodevar(xmlhttp.responseXML,"bld7");
				var bld8 = getnodevar(xmlhttp.responseXML,"bld8");
				var bldnum = getnodevar(xmlhttp.responseXML,"bldnum");

				updatehtml("bld0",bld0);
				updatehtml("bld1",bld1);
				updatehtml("bld2",bld2);
				updatehtml("bld3",bld3);
				updatehtml("bld4",bld4);
				updatehtml("bld5",bld5);
				updatehtml("bld6",bld6);
				updatehtml("bld7",bld7);
				updatehtml("bld8",bld8);
				updatehtml("bldnum",bldnum);

			}

		
			// Here we set images for mail, build etc... we shouldn't need the double check here since these images are unique enough
			if( getnodevar(xmlhttp.responseXML,"mail") == 1 ) { changeimage("headermail", "images/i09a.jpg"); }
			if( getnodevar(xmlhttp.responseXML,"build") == 1 ) { changeimage("headerbuild", "images/i10a.jpg"); }
			if( getnodevar(xmlhttp.responseXML,"aid") == 1 ) { changeimage("headeraid", "images/i11a.jpg"); }
			if( getnodevar(xmlhttp.responseXML,"fleet") == 1 ) { changeimage("headerfleet", "images/i12a.jpg"); }
			else if( getnodevar(xmlhttp.responseXML,"fleet") == 2 ) { changeimage("headerfleet", "images/i12b.jpg"); }
		}

	} else {
		window.clearTimeout(SD);
	}
}
xmlhttp.open("GET","ajax?typ="+str,true);
xmlhttp.send();
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


