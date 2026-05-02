#!/bin/bash
GUID_HOME="${GUID_HOME:-/usr/lib/guidance/www/Guidance}"
BIOSEQUENCE="${GUID_HOME}/../bioSequence_scripts_and_constants"
SELECTON="${GUID_HOME}/../Selecton"

export PERL5LIB="${GUID_HOME}:${BIOSEQUENCE}:${SELECTON}:${PERL5LIB:-}"
export PATH="${GUID_HOME}:${GUID_HOME}/exec:${PATH}"

exec perl "${GUID_HOME}/guidance.pl" "$@"
