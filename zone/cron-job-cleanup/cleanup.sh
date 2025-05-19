#!/bin/bash

# Most of this is shamelessly stolen from SlackBuilds.org and Slackware:
# https://slackbuilds.org/templates/doinst.sh
# https://slackware.uk/slackware/slackware64-current/source/a/sysvinit-scripts/scripts/rc.M

# Updates the system desktop database
if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q /usr/share/applications >/dev/null 2>&1
fi

# Updates the system mime database
if [ -x /usr/bin/update-mime-database ]; then
  /usr/bin/update-mime-database /usr/share/mime >/dev/null 2>&1
fi

# Update any existing icon cache files:
if find /usr/share/icons -maxdepth 2 2> /dev/null | grep -q icon-theme.cache ; then
  for theme_dir in /usr/share/icons/* ; do
    if [ -r ${theme_dir}/icon-theme.cache ]; then
      /usr/bin/gtk-update-icon-cache -t -f ${theme_dir} 1> /dev/null 2> /dev/null &
    fi
  done
  # This would be a large file and probably shouldn't be there.
  if [ -r /usr/share/icons/icon-theme.cache ]; then
    rm -f /usr/share/icons/icon-theme.cache
  fi
fi

# Update fonts cache
if [ -x /usr/bin/fc-cache ]; then
  /usr/bin/fc-cache -f &
fi

# https://manpages.ubuntu.com/manpages/kinetic/man1/gdk-pixbuf-query-loaders.1.html
if [ -x /usr/bin/gdk-pixbuf-query-loaders ]; then
  /usr/bin/gdk-pixbuf-query-loaders --update-cache >/dev/null 2>&1
fi

# These GTK+/pango files need to be kept up to date for
# proper input method, pixbuf loaders, and font support.
#if [ -x /usr/bin/update-gtk-immodules ]; then
#  /usr/bin/update-gtk-immodules > /dev/null 2>&1 &
#fi

#if [ -x /usr/bin/update-gdk-pixbuf-loaders ]; then
#  HOME=/root /usr/bin/update-gdk-pixbuf-loaders > /dev/null 2>&1 &
#fi
#if [ -x /usr/bin/update-pango-querymodules ]; then
#  /usr/bin/update-pango-querymodules > /dev/null 2>&1 &
#fi

# GSettings (glib2) schema compiler
if [ -e /usr/share/glib-2.0/schemas ]; then
  if [ -x /usr/bin/glib-compile-schemas ]; then
    /usr/bin/glib-compile-schemas /usr/share/glib-2.0/schemas >/dev/null 2>&1
  fi
fi

