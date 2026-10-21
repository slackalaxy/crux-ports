#!/bin/sh
#
# A (weekly) cleanup: Refresh desktop, mime, icon, font and glib caches.
# Best run from /etc/cron/weekly/
#

# Update desktop database
if [ -x /usr/bin/update-desktop-database ]; then
    /usr/bin/update-desktop-database -q /usr/share/applications >/dev/null 2>&1
fi

# Update MIME database
if [ -x /usr/bin/update-mime-database ]; then
    /usr/bin/update-mime-database /usr/share/mime >/dev/null 2>&1
fi

# === Icon caches ===
if [ -x /usr/bin/gtk-update-icon-cache ]; then
    # Update hicolor explicitly (most important)
    if [ -d /usr/share/icons/hicolor ]; then
        /usr/bin/gtk-update-icon-cache -t -f -q /usr/share/icons/hicolor >/dev/null 2>&1
    fi

    # Update other icon themes
    find /usr/share/icons -maxdepth 2 -name icon-theme.cache 2>/dev/null | while read -r cache; do
        theme_dir=$(dirname "$cache")
        # Skip hicolor since we already did it
        if [ "$theme_dir" != "/usr/share/icons/hicolor" ]; then
            /usr/bin/gtk-update-icon-cache -t -f -q "$theme_dir" >/dev/null 2>&1
        fi
    done
fi

# Remove the giant combined cache if it exists
rm -f /usr/share/icons/icon-theme.cache 2>/dev/null

# Update font cache (can be slow)
if [ -x /usr/bin/fc-cache ]; then
    /usr/bin/fc-cache -f >/dev/null 2>&1
fi

# Update gdk-pixbuf loaders
if [ -x /usr/bin/gdk-pixbuf-query-loaders ]; then
    /usr/bin/gdk-pixbuf-query-loaders --update-cache >/dev/null 2>&1
fi

# Compile GSettings schemas (important for GTK apps)
if [ -d /usr/share/glib-2.0/schemas ] && [ -x /usr/bin/glib-compile-schemas ]; then
    /usr/bin/glib-compile-schemas /usr/share/glib-2.0/schemas >/dev/null 2>&1
fi

# Update the GIO modules cache. GIO modules are plugins for file access
# (e.g. network shares, trash, thumbnails in some apps); the cache tells
# GTK programs which plugins exist without scanning the folder each time.
if [ -x /usr/bin/gio-querymodules ] && [ -d /usr/lib/gio/modules ]; then
    /usr/bin/gio-querymodules /usr/lib/gio/modules >/dev/null 2>&1
fi

# Update the GTK3 input method cache. Input methods are plugins that
# change how keyboard input works (e.g. typing accented or Asian
# characters); the cache lists which ones are installed.
if [ -x /usr/bin/gtk-query-immodules-3.0 ]; then
    /usr/bin/gtk-query-immodules-3.0 --update-cache >/dev/null 2>&1
fi

# The same for GTK2 programs
if [ -x /usr/bin/gtk-query-immodules-2.0 ]; then
    /usr/bin/gtk-query-immodules-2.0 --update-cache >/dev/null 2>&1
fi
