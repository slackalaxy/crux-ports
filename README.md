# crux-ports (3.6.1)

This is my personal collection of CRUX ports. Many of them were written
by me, others were adapted from existing ports. There are also some that
were shamelessly stolen and put here, so I have a single place for all.
It just makes it more simple to have a unified place of "cherry picked"
ports, than a bunch of repos with overlapping entries.

I did all this in a hurry and it would be too complicated to track back
the original authors, in order to credit them properly. So, if someone
recognised their port and wants to be credited, let me know!

Ports are divided (for now) in the following categories:
* **bumped/**: ports (*in any*) that need to be bumped to a newer version (until maintainer catches up)
* **custom/**: main collection of programs and libraries
* **locked/**: ports that need to be locked at an older version
* **p5/**: perl modules, generated mainly by [cpan2crux](https://crux.nu/portdb/index.php?q=cpan2crux&a=search)
* **scientific/**: academic programs for biologists and chemists
* **xfce_4.12/**: I like to have a GTK2-only desktop, so here it is

There are 3 unversioned ports, that simply repackage the latest archive
provided by upstream:

* [custom/google-chrome](custom/google-chrome)
* [custom/viber](custom/viber)
* [custom/zoom-linux](custom/zoom-linux)

Therefore `.signature` and `.footprint` may mismatch. I can extract the
version number from the archive, but it's just simpler to do it that
way. This can change of course.
