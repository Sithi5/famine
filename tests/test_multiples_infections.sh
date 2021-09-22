_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_END=`tput sgr0`

# echo filename
echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"


# Multiples infections should fail
gcc ./tests/test1.c -o test1
echo "\n${_GREEN}Creating and Executing woody with normal binary...${_END}\n"
./woody_woodpacker test1
./woody
echo "\n${_GREEN}Try infecting a second time...${_END}\n"
./woody_woodpacker woody

rm test1
rm woody