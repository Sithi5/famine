_GREEN=`tput setaf 2`
_YELLOW=`tput setaf 3`
_END=`tput sgr0`

# echo filename
echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"


# Test with two simples binary
gcc -no-pie -m32 ./tests/test1.c -o test1_no_pie
echo "\n${_GREEN}Creating and Executing famine with no_pie binary...${_END}\n"
./famine test1_no_pie
./famine
rm famine

gcc -m32 ./tests/test1.c -o test1
echo "\n${_GREEN}Creating and Executing famine with normal binary...${_END}\n"
./famine test1
./famine
rm famine

rm test1
rm test1_no_pie