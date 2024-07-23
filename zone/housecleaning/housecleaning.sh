#!/bin/bash

# collects information about loadable modules for gdk-pixbuf and writes
# it to the default cache file location
# https://manpages.ubuntu.com/manpages/kinetic/man1/gdk-pixbuf-query-loaders.1.html
#echo "=======> Running gdk-pixbuf-query-loaders"
gdk-pixbuf-query-loaders --update-cache

# build a cache database of the MIME types handled by desktop files
# https://manpages.ubuntu.com/manpages/kinetic/en/man1/update-desktop-database.1.html
#echo "=======> Running update-desktop-database"
update-desktop-database

# creates mmapable cache files for icon themes
# https://manpages.ubuntu.com/manpages/kinetic/en/man1/gtk-update-icon-cache.1.html
#echo "=======> Rinning gtk-update-icon-cache"
for i in /usr/share/icons/* ; do
  gtk-update-icon-cache -q -t -f $i
done

# compiles all the GSettings XML schema files in DIRECTORY into a binary
# file with the name gschemas.compiled that can be used by GSettings
# https://manpages.ubuntu.com/manpages/kinetic/man1/glib-compile-schemas.1.html
#echo "=======> Running glib-compile-schemas"
glib-compile-schemas /usr/share/glib-2.0/schemas/

# update mime database
update-mime-database /usr/share/mime

# Update fonts cache
fc-cache -f -v
