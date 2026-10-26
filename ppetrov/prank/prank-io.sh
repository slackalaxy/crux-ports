#!/bin/sh
# Run PRANK with plain input/output file names: prank-io in.fa out.fa [prank options]
# PRANK always writes out.fa.best.fas; rename it to the requested name.
in="$1"
out="$2"
shift 2
prank -d="$in" -o="$out" "$@" && mv "$out.best.fas" "$out"
