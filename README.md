nullidentdMod
----------

Version 1.0
Copyright 1999 Brian Young <bayoung@acm.org>
Modified 2011 by dxtr

NullidentdMod 2014 by AcidHub <acidhub@craft.net.br>

What
----

NullidentdMod is intended to be a bare minimum identd server.

The program implements the auth protocol from [RFC 1413](http://www.rfc-base.org/rfc-1413.html).  This protocol is used to identify active TCP connections.  It depends on the trustworthiness of the server and as such is completely useless as a method of identification.  Unfortunately some applications still require that an identd server is available to query about incoming connections.  nullidentd implements the absolute minimum server to allow these applications to function. It returns a fake response for any request.

The auth port is TCP 113.


Goals
-----

NullidentdMod is a single source file less than 150 lines long.  It is intended to be so simple that it is probably bug free.  It can certainly be understood in its entirety with no difficulty.  It should also be somewhat immune to denial of service attacks by following strict timeouts on idle clients.


Installation
------------

NullidentdMod must be run from systemd.
This is archieved by enabling nullidentdmod.socket:

    # systemctl enable nullidentdmod.socket


As is, NullidentdMod will return always a random userid.

For assign a custom userid, copy nullidentdmod.service to /etc/systemd/system/:

    # cp /usr/lib/systemd/system/nullidentdmod@.service /etc/systemd/system/

Edit the unit:

    # vim /etc/systemd/system/nullidentdmod@.service

At line 6, write desired userid:

    ExecStart=/usr/bin/nullidentdmod <userid>

Obviously where <userid> you put your custom userid

