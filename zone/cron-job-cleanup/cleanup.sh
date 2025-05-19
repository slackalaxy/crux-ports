#!/bin/bash

# Most of this is shamelessly stolen from SlackBuilds.org:
# https://slackbuilds.org/templates/doinst.sh

# Updates the system desktop database
if [ -x /usr/bin/update-desktop-database ]; then
  /usr/bin/update-desktop-database -q /usr/share/applications >/dev/null 2>&1
fi

# Updates the system mime database
if [ -x /usr/bin/update-mime-database ]; then
  /usr/bin/update-mime-database /usr/share/mime >/dev/null 2>&1
fi

# Updates the GTK icon cache
if [ -e /usr/share/icons/hicolor/icon-theme.cache ]; then
  if [ -x /usr/bin/gtk-update-icon-cache ]; then
    /usr/bin/gtk-update-icon-cache -f /usr/share/icons/hicolor >/dev/null 2>&1
  fi
fi

# GSettings (glib2) schema compiler
if [ -e /usr/share/glib-2.0/schemas ]; then
  if [ -x /usr/bin/glib-compile-schemas ]; then
    /usr/bin/glib-compile-schemas /usr/share/glib-2.0/schemas >/dev/null 2>&1
  fi
fi

# Update fonts cache
if [ -x /usr/bin/fc-cache ]; then
  /usr/bin/fc-cache -f
fi

# https://manpages.ubuntu.com/manpages/kinetic/man1/gdk-pixbuf-query-loaders.1.html
if [ -x /usr/bin/gdk-pixbuf-query-loaders ]; then
  /usr/bin/gdk-pixbuf-query-loaders --update-cache >/dev/null 2>&1
fi
