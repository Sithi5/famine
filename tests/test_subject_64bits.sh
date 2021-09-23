#!/bin/bash
set -x
_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_END=`tput sgr0`

# Echo filename
echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"

# Test subject
rm -rf /tmp/test/
rm -rf /tmp/test2/
mkdir /tmp/test/
mkdir /tmp/test2/

printf '%s\n%s\n%s\n\t%s\n\t%s\n%s\n' "#include <stdio.h>" "int main(void)" "{" "printf(\"Hello, World\n\");" "return 0;" "}" > ./tests/tmp.c
gcc -m64 ./tests/tmp.c -o /tmp/test/sample
ls -al /tmp/test
/tmp/test/sample
file /tmp/test/sample
strings /tmp/test/sample | grep "mabouce"

cp /bin/ls /tmp/test2/
ls -al /tmp/test2
file /tmp/test2/ls

./Famine

strings /tmp/test/sample | grep "mabouce"
/tmp/test/sample

strings /tmp/test2/ls | grep "mabouce"
/tmp/test2/ls -la /tmp/test2/

gcc -m64 ./tests/tmp.c -o /tmp/test/sample
ls -al /tmp/test
/tmp/test/sample
file /tmp/test/sample
strings /tmp/test/sample | grep "mabouce"
/tmp/test2/ls -la /tmp/test2/
strings /tmp/test/sample | grep "mabouce"

rm ./tests/tmp.c
