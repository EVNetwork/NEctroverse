
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

if(gday == '7') {
gday='00';
gweek++;
}

stime =  (gweek > 0 ? gweek + " week" : "" ) + (gweek > 1 ? "s " : ( gweek > 0 ? " " : "" ));

stime +=  (gday > 0 ? gday + " day" : "" ) + (gday > 1 ? "s " : ( gday > 0 ? " " : "" ));

stime += (ghour > 0 ? ghour + " hour" : "" ) + (ghour > 1 ? "s " : ( ghour > 0 ? " " : "" ) );

stime += (gmin > 0 ? gmin + " minute" : "" ) + (gmin > 1 ? "s " : ( gmin > 0 ? " " : "" ) );

stime += (gsec > 0 ? gsec + " second" : "" ) + (gsec > 1 ? "s " : ( gsec > 0 ? " " : "" ) );


	updatehtml("gameuptime",stime);

}, 1000);


setInterval( function(){
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

if(hday == '7') {
hday='00';
hweek++;
}

htime =  (hweek > 0 ? hweek + " week" : "" ) + (hweek > 1 ? "s " : ( hweek > 0 ? " " : "" ));

htime +=  (hday > 0 ? hday + " day" : "" ) + (hday > 1 ? "s " : ( hday > 0 ? " " : "" ) );

htime += (hhour > 0 ? hhour + " hour" : "" ) + (hhour > 1 ? "s " : ( hhour > 0 ? " " : "" ) );

htime += (hmin > 0 ? hmin + " minute" : "" ) + (hmin > 1 ? "s " : ( hmin > 0 ? " " : "" ) );

htime += (hsec > 0 ? hsec + " second" : "" ) + (hsec > 1 ? "s " : ( hsec > 0 ? " " : "" ) );


	updatehtml("hostuptime",htime);
}, 1000);

