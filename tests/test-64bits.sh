# _GREEN=`tput setaf 2`
# _YELLOW=`tput setaf 3`
# _END=`tput sgr0`

# # echo filename
# echo "\n\n\n\n${_YELLOW}$0 :${_END}\n\n"


# # Test with two simples binary
# gcc -no-pie ./tests/test1.c -o test1_no_pie
# echo "\n${_GREEN}Creating and Executing famine with no_pie binary...${_END}\n"
# ./famine test1_no_pie
# ./famine
# rm famine
# rm test1_no_pie

# gcc ./tests/test1.c -o test1
# echo "\n${_GREEN}Creating and Executing famine with normal binary...${_END}\n"
# ./famine test1
# ./famine
# rm famine
# rm test1

# # Test with bin/ls binary
# echo "\n${_GREEN}Creating and Executing famine with /bin/ls binary...${_END}\n"
# ./famine /bin/ls
# ./famine
# rm famine

# # Test with bin/echo binary
# echo "\n${_GREEN}Creating and Executing famine with /bin/echo binary...${_END}\n"
# ./famine /bin/echo
# ./famine "This is echo binaryyy"
# rm famine

# # Test with file close to page size
# echo "\n${_GREEN}Creating and Executing famine with binary close to page_size...${_END}\n"
# gcc -no-pie ./tests/test_close_to_page_size.c -o test_close_to_page_size
# ./famine test_close_to_page_size
# ./famine
# rm famine
# rm test_close_to_page_size
