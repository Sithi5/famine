SHELL				=	/bin/sh

################################################################################
#                                BINARIES NAME                                 #
################################################################################

NAME				=	Famine

ART_NAME			=	bird

################################################################################
#                                COMPILATION MODE                              #
################################################################################

AS					:=	nasm
LD					:=	ld #Linker
LDFLAGS				:= #Linker flags

################################################################################
#                                DEFINES                                       #
################################################################################

LONG_BITS := $(shell getconf LONG_BIT)
ifeq ($(LONG_BITS),32)
# Define for 32bits
CC				+= -D ARCH_32
ASM_FLAG		:= -f elf32
else
ASM_FLAG		:= -f elf64
endif

################################################################################
#                                     NAME                                     #
################################################################################

SRC_NAME		:=	famine.asm

INCLUDE_NAME		:=	define.inc

TESTS_SRC_NAME		:= 	./tests/test*.sh

################################################################################
#                                     PATH                                     #
################################################################################

SRC_PATH		:=	./asm/

OBJ_PATH		:= 	./obj_asm/

INCLUDE_PATH		:=	./include/

################################################################################
#                                 NAME + PATH                                  #
################################################################################

SRC					:= 	$(addprefix $(SRC_PATH), $(SRC_NAME))

OBJ					:=	$(patsubst $(SRC_PATH)%.asm, $(OBJ_PATH)%.o, $(SRC))

INCLUDE				:=	$(addprefix $(INCLUDE_PATH), $(INCLUDE_NAME))

################################################################################
#                                     RULES                                    #
################################################################################

all: $(ART_NAME) $(NAME)

$(NAME): $(OBJ)
	@echo "\n$(NAME)"
	@echo "\n$(_CYAN)====================================================$(_END)"
	@echo "$(_YELLOW)		COMPILING $(NAME)$(_END)"
	@echo "$(_CYAN)====================================================$(_END)"
	@$(LD) $(LDFLAG) -o $(NAME) $(OBJ)
	@echo "\n$(_WHITE)$(_BOLD)$@\t$(_END)$(_GREEN)[OK]\n$(_END)"
	@echo "\n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.asm $(INCLUDE)
	@mkdir -p $(OBJ_PATH)
	@$(AS) $(ASM_FLAG) -I $(INCLUDE_PATH) -I $(SRC_PATH) $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)\t"	\
		"COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"

tests: all
	@echo "\n$(_CYAN)====================================================$(_END)"
	@echo "$(_YELLOW)		LAUNCHING TESTS$(_END)"
	@echo "$(_CYAN)====================================================$(_END)"
	@for f in $(TESTS_SRC_NAME);  do sh $${f}; done;

clean:
	@rm -rf $(OBJ_PATH) 2> /dev/null || true
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(OBJ_PATH)"$(_END)"
	@rm -f $(ART_NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(ART_NAME)"$(_END)"


fclean: clean
	@rm -f $(NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(NAME)
	@echo "$(_END)"

re: fclean all

help:
	@echo "$(_YELLOW)Makefile for generating binary infectors."
	@echo "$(_YELLOW)Usage:                                                                    "
	@echo "$(_YELLOW)   make                                runs all                           "
	@echo "$(_YELLOW)   make all                            generates all                      "
	@echo "$(_YELLOW)   make clean                          remove the object files            "
	@echo "$(_YELLOW)   make fclean                         clean and remove binaries files    "
	@echo "$(_YELLOW)   make tests                          launch tests scripts               "
	@echo "$(_YELLOW)   make help                           prints this message                $(_END)"


$(ART_NAME):
	@touch $(ART_NAME)
	@echo "$(_CYAN)" > $(ART_NAME)
	@echo "       '-------------------.. " >> $(ART_NAME)
	@echo "         '\\_________         \`-.-----. " >> $(ART_NAME)
	@echo "             '\\_______          \`\\    \`.                   $(_GREEN)  __   ____ $(_CYAN)" >> $(ART_NAME)
	@echo "                  \\____           \`\\   :                    $(_GREEN)/_ | | .-.\\ $(_CYAN)" >> $(ART_NAME)
	@echo "    '._____         (____           \`\\.\`.             .--._ $(_GREEN)  \\\\' / $(_CYAN)" >> $(ART_NAME)
	@echo "     \\____  \"'..____.(_______          \`~-.________.-'  $(_RED)@$(_CYAN). \\___${_GREEN}\\.'${_CYAN}___ " >> $(ART_NAME)
	@echo "     )___     ___                                         /___________\\ " >> $(ART_NAME)
	@echo "      )___..''   '--.                    _.----------.____\`----$(_GREEN))'($(_CYAN)---' " >> $(ART_NAME)
	@echo "                   _:-'   ,          _.-'                $(_GREEN)     /( \\\\ $(_CYAN)" >> $(ART_NAME)
	@echo "               .-~~ __..-'~~~~~~~~~~'                    $(_GREEN)   .'    '\\ $(_CYAN)" >> $(ART_NAME)
	@echo "               \\ \\~~ \\ \\                             $(_GREEN)      / '    ) \\ $(_CYAN)" >> $(ART_NAME)
	@echo "                \\ \\   \\ \\_.-'                        $(_GREEN)     |      )   ' $(_CYAN)" >> $(ART_NAME)
	@echo "                 \\ \\_ _;-,,'                           $(_GREEN)   ( (      ) ) $(_CYAN)" >> $(ART_NAME)
	@echo "                 ;-,,'                                   $(_GREEN)  \"--------\" $(_CYAN)" >> $(ART_NAME)
	@echo "" >> $(ART_NAME)
	@echo "$(_END)" >> $(ART_NAME)
	@cat $(ART_NAME)

.PHONY: all clean fclean re check help tests clean_payload

################################################################################
#                                    FORMAT                                    #
################################################################################

# Text format
_END		=	\033[0m
_GRAS		=	\033[1m
_SOUL		=	\033[4m
_CLIG		=	\033[5m
_SURL		=	\033[7m

# Colors format
_BLACK		=	\033[30m
_RED		=	\033[31m
_GREEN		=	\033[32m
_YELLOW		=	\033[33m
_BLUE		=	\033[34m
_PURPLE		=	\033[35m
_CYAN		=	\033[36m
_GREY		=	\033[37m

# Background
_IBLACK		=	\033[40m
_IRED		=	\033[41m
_IGREEN		=	\033[42m
_IYELLOW	=	\033[43m
_IBLUE		=	\033[44m
_IPURPLE	=	\033[45m
_ICYAN		=	\033[46m
_IGREY		=	\033[47m
