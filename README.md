# Ports for CRUX 3.7

This is my personal collection of CRUX ports. Many of them were written
by me, others were adapted from existing ports. There are also some that
were shamelessly stolen and put here, so I have a single place for all.
It just makes it more simple to have a unified place of "cherry picked"
ports, than a bunch of repos with overlapping entries.

It would be too complicated to track back the original authors, in order
to credit them properly. So, if someone recognised their port and wants
to be credited, let me know!

## ppetrov/
Main collection of programs and libraries. Most likely this is what you
are interested in. There are 4 unversioned ports, that simply repackage
the latest archive provided by upstream:
* google-chrome
* mplayer
* viber
* zoom-linux

Therefore `.signature` and `.footprint` may mismatch. I can extract the
version number from the archive, but it's simpler to do it that way.

The repo contains over 60 p5 ports, initially generated by the amazing
[cpan2crux](https://www.mizrahi.com.ve/crux/ports/cpan2crux/Pkgfile).
This is the only repo listed at [portdb](https://crux.nu/portdb/).

## r4-modules
R modules, mostly generated by my own, home-brewed tool called 
[cran2crux](https://github.com/slackalaxy/cran2crux). The repo is still
mostly experimental for now.

## xfce_4.12/
I like to have a GTK2-only desktop, so here it is.

Ports that are part of CRUX, but need to be locked at an older version,
because of Xfce 4.12 are:
* upower (0.99.13): the latest version with which `xfce4-power-manager`
battery status works
* xorg-libx11 (1.8): Xfce 4.12 themes settings do not get applied with
version >1.8
* libblockdev (2.28)

Make sure you install the locked ports first, then you can build the
whole desktop with:
```
prt-get depinst xfce
```
Check `/usr/etc/xdg/xfce4`, there are examples for `.bashrc_profile`,
`.bashrc` and Xfce's `.xinitrc`.

## zone/
Ports that package custom settings files, configs, modified programs,
scripts, workarounds (e.g. jdk symlink), as well as more unconventional
stuff all go in the zone.
