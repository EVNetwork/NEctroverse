//Yer, better make a timer... this can contol the call's to update info too --- min and sec will be defined on output of html

function countDown() {

sec--;

if (sec < 0 ) {
	if ( min > 0 ) {
		sec = 59;
		min--;
	} else {
		sec = 00;
		getInfo("ticker");
		SD=window.setTimeout("countDown();", 5000);
		login = false;
		return;
	}
} else {
	min = min;
}

time = ( ( min > 0 ) ? min + " minute"+( (min > 1) ? "s" : "" )+" and " : "" ) + sec + " second"+( (sec > 1) ? "s" : "" );
updatehtml('headerTime',time);
updatehtml('hqTime',time);
updatehtml('sstatsTime',time);

SD=window.setTimeout("countDown();", 1000);

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


function _(el){
	return document.getElementById(el);
}

function changeimage(img, a) {

try {
	_(img).src=a;
} catch(err) {
	return;
}

}

function updatehtml(id, i) {

if(i==""){ return; }
else if(i=="undefined"){ return; }

try {
	_(id).innerHTML=i;
} catch(err) {
	return;
}


}

function updatevalue(id, i) {

if(i==""){ return; }
else if(i=="undefined"){ return; }

try {
	_(id).value=i;
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

function bytesToSize(bytes) {
    var sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
    if (bytes == 0) return 'n/a';
    var i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)));
    return Math.round(bytes / Math.pow(1024, i), 2) + ' ' + sizes[[i]];
};

function uploadFile(){
	var file = _("fileupload").files[0];
	var prompt = confirm("You are about to upload\n"+file.name+" | "+bytesToSize(file.size)+" | "+file.type);
	if ( prompt == true ) {
		sendFile(file);
	} else {
  		updatehtml("status","Upload aborted!");
	}
}
function sendFile(file){
	toggle();
	var formdata = new FormData();
	formdata.append("uploadfile", file);
	var ajax = new XMLHttpRequest();
	ajax.upload.addEventListener("progress", progressHandler, false);
	ajax.addEventListener("load", completeHandler, false);
	ajax.addEventListener("error", errorHandler, false);
	ajax.addEventListener("abort", abortHandler, false);
	ajax.open("POST", "files");
	ajax.send(formdata);
}
function progressHandler(event){
	updatehtml("loaded_n_total","Uploaded "+bytesToSize(event.loaded)+" of "+bytesToSize(event.total) );
	var percent = ( event.loaded / event.total ) * 100;
	updatevalue("progressBar",Math.round(percent) );
	updatehtml("status",Math.round(percent)+"% uploaded... please wait");
}
function completeHandler(event){
	updatehtml("status","Upload Completed");
	updatevalue("progressBar",0);
	window.setTimeout("toggle();", 15000);
}
function errorHandler(event){
	updatehtml("status","Upload Failed");
}
function abortHandler(event){
	updatehtml("status","Upload Aborted");
}

  var toggle = function() {
  var mydiv = document.getElementById('progblock');
  if (mydiv.style.display === 'block' || mydiv.style.display === '')
    mydiv.style.display = 'none';
  else
    mydiv.style.display = 'block'
  }
  


