//Yer, better make a timer... this can contol the call's to update info too --- min and sec will be defined on output of html



function countUp() {

if( login ) {
	sesspage++;
}

if( ( ( login == 0 ) && ( sesspage > 0 ) ) ) { // || ( sesstime < sesspage )
	try {
		document.location = "expired_bitch3";
	} catch(err) {
		return false;
	}
} else {
	SU=window.setTimeout("countUp();", 1000);
}

}

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
		return false;
	}
} else {
	min = min;
}

if( (sec == 0) && (min == 0) ) {
	time = "Now...";
} else {
	time = ( ( min > 0 ) ? min + " minute"+( (min > 1) ? "s" : "" )+( ( sec > 0 ) ? " and " : "" ) : "" );
	time += ( ( sec > 0 ) ? sec + " second"+( (sec > 1) ? "s" : "" ) : "" );
}

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
	return window.document.getElementById(el);
}

function changeimage(img, a) {

try {
	_(img).src=a;
} catch(err) {
	return;
}

}

function changebackground(img, a) {

try {
	_(img).style.backgroundImage="url("+a+")";;
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


addLoadEvent( SD=window.setTimeout("countDown();", 1000) );
//addLoadEvent( SU=window.setTimeout("countUp();", 1000) );


function toggle_form(id) { for(i=0;i<_(id).length;i++) if(_(id).elements[i].type == "checkbox") _(id).elements[i].click(); }

function pad ( val ) { return val > 9 ? val : "0" + val; }

function changeslider(newValue,field) {
	window.document.getElementById(field).value=newValue;
}

function bytesToSize(bytes) {
    var sizes = ['Bytes', 'KB', 'MB', 'GB', 'TB'];
    if (bytes == 0) return 'n/a';
    var i = parseInt(Math.floor(Math.log(bytes) / Math.log(1024)));
    return Math.round(bytes / Math.pow(1024, i), 2) + ' ' + sizes[[i]];
};

function uploadFile(){
	var file = _("fileupload").files[0];
	var destination = _("destination").value;
	if( ( file ) && ( destination ) ){
		var prompt = confirm("You are about to upload\n"+file.name+" | "+bytesToSize(file.size)+" | "+file.type);
		if ( prompt == true ) {
			sendFile(file, destination);
		} else {
  			updatehtml("status","Upload aborted!");
		}
	}
}
function sendFile(file, destination){
	toggle();
	var formdata = new FormData();
	formdata.append("uploadfile", file);
	var ajax = new XMLHttpRequest();
	ajax.upload.addEventListener("progress", progressHandler, false);
	ajax.addEventListener("load", completeHandler, false);
	ajax.addEventListener("error", errorHandler, false);
	ajax.addEventListener("abort", abortHandler, false);
	ajax.open("POST", destination);
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
	var mydiv = window.document.getElementById('progblock');
	if ( ( mydiv.style.display === 'block' ) || ( mydiv.style.display === '' ) ) {
		mydiv.style.display = 'none';
	} else {
		mydiv.style.display = 'block';
	}
}
  
parseQueryString = function() {

    var str = window.location.search;
    var objURL = {};

    str.replace(
    	new RegExp( "([^?=&]+)(=([^&]*))?", "g" ),
    	function( $0, $1, $2, $3 ){
    		objURL[ $1 ] = $3;
    	}
    );
    return objURL;
};


var ddajaxsidepanel = {
	defaults: {ajaxloadgif:'files?type=image&name=squareloading.gif', fx:{dur:'normal', easing:'swing'}, openamount:'80%', openamount_minthreshold:'400px'},

	$panelref: null,
	$contentarea: null,
	setting: null,
	docdimensions: null,
	paneldimensions: null,
	$ajaxloadgif: null,
	delaytimer: null,

	loadcontent: function(url, type){
		var $ = jQuery
		if (url == null){
			this.$contentarea.empty()
			this.$panelref.data('state', 'closed')
		}
		else{
			this.$panelref.data('state', 'open')		
			this.$contentarea.html(this.$ajaxloadgif)
			if (type){
				loadtype = type
			}
			else if (url.indexOf('http') != -1){
				var url_rootdomain = url.match(/^http[^:]*:\/\/((?:www\.){0,1}([^\/]+))/i) // match domain name portion of http link
				if (url_rootdomain && location.hostname.indexOf(RegExp.$2)!=-1){ //if URL's root domain (without www) matches current doc's hostname, meaning it's a internal URL (ie: http://mysite.com/page1.htm)
				document.write = document.writeLn = function(){} // overwrite default document.write() function, as it causes major problems if present inside Ajax fetched document
					url = url.replace(RegExp.$1, location.hostname)
					loadtype = "ajax"
				}		
				else{
					loadtype = "iframe"
				}		
			}
			else{
				loadtype = "ajax"
			}
			if (loadtype == "iframe"){
				ddajaxsidepanel.$contentarea.html('<iframe src="javascript:false" style="border:0; margin:0; padding:0; width:100%; height:100%"></iframe>')
				ddajaxsidepanel.$contentarea.find('iframe:eq(0)').attr('src', url)
			}
			else{
				ddajaxsidepanel.$contentarea.load(url)
			}
		}
	},

	showhidepanel:function(url, action, type){
		var $ = jQuery, setting = this.setting
		var panelstate =  this.$panelref.data('state')
		var winwidth = $(window).width(), panelwidth = this.$panelref.outerWidth()
		if (panelwidth < parseInt(setting.openamount_minthreshold))
			return true
		if (setting.openamount_maxwidth && setting.openamount_maxwidth > setting.openamount_minthreshold)
			panelwidth = Math.min(panelwidth, parseInt(setting.openamount_maxwidth))
		if (action =="show" && panelstate == "open")
			this.$panelref.animate({left: '+=50'}, function(){
				ddajaxsidepanel.loadcontent(null)
			})
		var finalcss = (action=="show")? {left: winwidth-panelwidth, opacity: 1} : {left: winwidth, opacity: 0}
		this.$panelref.animate(finalcss, setting.fx.dur, (this.$panelref.data('state')=='open')? 'easeOutBack' : setting.fx.easing, function(){
			ddajaxsidepanel.loadcontent(url, type)
		})
		return false
	},

	

	init: function(setting){
		var $ = jQuery
		this.setting = $.extend({}, this.defaults, setting)
		if (setting.targetselector){
			var $targetlinks = $(setting.targetselector).each(function(){ // seek out targeted selectors on the page
				var $el = $(this)
				$el.on('click', function(){
					return ddajaxsidepanel.showhidepanel(this.href, "show", this.getAttribute('data-loadtype'))
				})
			})
		}
		if (this.$panelref){ // if ajax content panel already defined, just exit
			return
		}
		this.$ajaxloadgif = $('<table width="100%" height="100%" align="center" valign="center" style="text-align:center"><tr><td><img src="' + this.setting.ajaxloadgif + '"/></td></tr></table>')
		this.$panelref = $('<div class="ddajaxsidepanel"><div class="panelhandle"></div><div class="contentarea"></div></div>').appendTo(document.body)
		this.$panelref
			.css({width:this.setting.openamount, height:'100%', left:'100%', visibility:'visible', opacity:0})
			.data('state', 'closed')
		this.$contentarea = this.$panelref.find('div.contentarea:eq(0)')
			.click(function(e){
				e.stopPropagation()
			})
		this.$panelref.find('div.panelhandle:eq(0)')
			.attr('title', 'Close Content Panel')
			.on('click', function(){
				ddajaxsidepanel.showhidepanel(null, "hide")
			})
		$(document).on('click', function(e){
			if (e.which == 1) // if left click on mouse
				ddajaxsidepanel.showhidepanel(null, "hide")
		})
		this.paneldimensions = {w: this.$panelref.outerWidth(), h: this.$panelref.outerHeight()}
	}

}

jQuery.extend(jQuery.easing, {  //see http://gsgd.co.uk/sandbox/jquery/easing/
	easeOutBack:function(x, t, b, c, d, s){
		if (s == undefined) s = 1.70158;
		return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
	},
	easeInQuad: function (x, t, b, c, d) {
		return c*(t/=d)*t + b;
	}
})


// Initialize Ajax Side Panel script:

jQuery(function(){
	ddajaxsidepanel.init({
		targetselector: 'a[rel="ajaxpanel"],area[rel="ajaxpanel"]',
		ajaxloadgif: 'files?type=image&name=squareloading.gif', //full path to "loading" gif relative to document. When in doubt use absolute URL to image.
		fx: {dur:500, easing: 'easeInQuad'}, // dur: duration of slide effect (milliseconds), easing: 'ease_in_type_string'
		openamount:'80%', // Width of panel when fully opened (Percentage value relative to page, or pixel value
		openamount_minthreshold:'400px' //Minimum required width of panel (when fully opened)  before panel is shown. This prevents panel from being shown on small screens or devices.
	})
});

var WindowObjectReference;
function openRequestedPopup(url, nme) {
	WindowObjectReference = window.open(url, nme,"menubar=no,height=550px,width=1250px,location=no,resizable=yes,scrollbars=yes,status=yes");
	return false;
}


