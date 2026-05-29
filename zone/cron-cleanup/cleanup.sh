#!/bin/bash
#
# weekly-cache-update: Refresh desktop, mime, icon, font and glib caches
# Best run from /etc/cron.weekly/
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
    /usr/bin/fc-cache -f >/dev/null 2>&1 &
fi

# Update gdk-pixbuf loaders
if [ -x /usr/bin/gdk-pixbuf-query-loaders ]; then
    /usr/bin/gdk-pixbuf-query-loaders --update-cache >/dev/null 2>&1
fi

# Compile GSettings schemas (important for GTK apps)
if [ -d /usr/share/glib-2.0/schemas ] && [ -x /usr/bin/glib-compile-schemas ]; then
    /usr/bin/glib-compile-schemas /usr/share/glib-2.0/schemas >/dev/null 2>&1
fi
