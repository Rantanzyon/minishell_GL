#!/bin/sh -e

for attr in $(seq 0 1); do
  for fg in $(seq 30 37); do
    for bg in $(seq 40 47); do
      printf "\033[$attr;${bg};${fg}m$attr;$fg;$bg\033[m "
    done
    echo
  done
done