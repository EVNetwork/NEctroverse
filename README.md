NEctroverse Alpha in Testing!
========

NEctroverse is a recode of the game Ectroverse created by Maloeran.<br>
It natrually contains a lot of changes from the original code.

You can help test the current versin at http://www.nectroverse.com<br>
You can find the original source code at https://github.com/ectroverse/evsource


To install the game:<br>
Edit config/buildflags.h to ajust optinal addins.<br>
Edit config/evconfig.ini and change as required.<br>

Additional Librarys needed for components:
* build-essential -- needed, but you should already have that.
* libpng-dev -- needed to render the galaxy map.
--Below are Optional's
* libmysqlclient-dev -- needed for mySql addition.
* libgnutls-dev -- For HTTPs support.
* libcurl4-gnutls-dev -- For Facebook support.

Run:
* make 
* ./evserver

Yup, that's it...<br>
Map creation is embeded into the core.

If an invalid config is found, you will recive a shell notice.<br>
If no file exists, the game will create a blank template for your ajustment.

NEctroverse can run in forked daemon mode, which does not require a shell.<br>
This does not require any extra librarys, and will save logs via syslog feature.

KNOWN BUGS - Very annoying and need to go:
* Facebook linkage is unstable.
* Empire Positions can not be changed.
* ... Possibly more
* ... Join and help find them all!
* .... ?
