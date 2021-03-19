#!/bin/bash

COLOR=${1:-white}
shift

echo "Color: $COLOR"

for IMG in $@
do
   convert $IMG -transparent $COLOR $IMG
done
