#!/bin/bash
# call from the top to build!
CF=clang-format-10
$CF -h > /dev/null || { echo "$CF does not exist, will not run" ; exit 1 ; }
find . \( -iname '*.h' ! -iname "gxco_isa.h" \) \
    -o \( -iname '*.hpp' ! -iname "half.hpp" \) \
    -o -iname '*.cpp' \
    -o -iname '*.h.in' \
    -o -iname '*.hpp.in' \
    -o -iname '*.cpp.in' \
    -o -iname '*.cl' \
| grep -v -E '(build/)|(install/)' \
| xargs -n 1 -P $(nproc) -I{} -t $CF -style=file {} -i 2>/dev/null