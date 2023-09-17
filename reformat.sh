#!/bin/bash
find . -regex '.*\.\(cpp\|hpp\|inc\|cc\|cu\|c\|h\)' -exec clang-format-17 -style=file -i {} \;
