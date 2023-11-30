#!/bin/sh
# Default acpi script that takes an entry for all actions

IFS=${IFS}/
set $@

case "$1" in
  button)
    case "$2" in
      power) /sbin/init 0
         ;;
      *) logger "ACPI action $2 is not defined"
         ;;
    esac
    ;;
  *)
    logger "ACPI group $1 / action $2 is not defined"
    ;;
esac
