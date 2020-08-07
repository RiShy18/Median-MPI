#!/bin/bash

for FILE in "$@"
  do
  convert $FILE out.ppm
  convert out.ppm -compress none out.ppm
  done