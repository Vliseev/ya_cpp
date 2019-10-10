#!/bin/bash
FILES=$(find -name "*.cpp")
A="// This is an open source non-commercial project. Dear PVS-Studio, please check"
B="// it. PVS-studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com"
for el in $FILES; do
  if grep -q "Dear PVS-Studio" $el; then
    :
  else
    sed -i "1i $A" $el
    sed -i "2i $B" $el
  fi
done
