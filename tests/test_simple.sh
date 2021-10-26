#!/bin/bash
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
gcc -m64 ./tests/tmp.c -o /tmp/test2/sample

/tmp/test/sample
/tmp/test2/sample

./Famine

/tmp/test/sample
/tmp/test2/sample
strings /tmp/test/sample | grep "mabouce"
strings /tmp/test2/sample | grep "mabouce"

rm ./tests/tmp.c
