SHELL				=	/bin/sh

################################################################################
#                                BINARIES NAME                                 #
################################################################################

NAME				=	woody_woodpacker

PAYLOAD_NAME		=	payload

ART_NAME			=	bird

################################################################################
#                                COMPILATION MODE                              #
################################################################################

WALL				:=	yes
WEXTRA				:=	yes
WSHADOW				:=	yes
WERROR				:=	yes
FSANITIZE			:=	no
DEBUG				:=	no
O2					:=	no

CC					:=	gcc
AS					:= nasm
GEN					:=	Generation in mode

LONG_BITS := $(shell getconf LONG_BIT)
ifeq ($(LONG_BITS),32)
# Define for 32bits
CC				:= $(CC) -D ARCH_32
AS_FLAG				:= -f elf32
else
AS_FLAG				:= -f elf64
endif

ifeq ($(WALL), yes)
	CC				+=	-Wall
	GEN				+=	 all
endif

ifeq ($(WEXTRA), yes)
	CC				+=	-Wextra
	GEN				+=	extra
endif

ifeq ($(WSHADOW), yes)
	CC				+=	-Wshadow
	GEN				+=	shadow
endif

ifeq ($(WERROR), yes)
	CC				+=	-Werror
	GEN				+=	error
endif

ifeq ($(FSANITIZE), yes)
	CC				+=	-fsanitize=address
	GEN				+=	sanitize
endif

ifeq ($(DEBUG), yes)
	CC				+=	-g
	GEN				+=	debug
endif

ifeq ($(O2),yes)
	CC				+=	-O2
	GEN				+=	O2
endif

ifeq ($(GEN), "Generation in mode")
	GEN				+=	no flags
endif


################################################################################
#                                     NAME                                     #
################################################################################


ifeq ($(LONG_BITS),32)
# Define for 32bits
PAYLOAD_SRC_NAME	=	print_woody_payload_32.asm

ASM_SRC_NAME		:=	xor_cipher_32.asm					\

else
# Define for 64bits
PAYLOAD_SRC_NAME	=	rc4_payload_64.asm

ASM_SRC_NAME		:=	rc4_cipher_64.asm					\

endif

SRC_NAME			:=	main.c								\
						error.c								\
						utils.c								\
						utils_elf.c							\
						infection.c							\
						utils_payload.c						\
						overwrite_payload.c					\
						find_payload_offset_elf32.c			\
						find_payload_offset_elf64.c			\
						silvio_text_infection.c				\
						crypto.c							\
						key_generator.c						\



INCLUDE_NAME		:=	woody_woodpacker.h					\

TESTS_SRC_NAME		:= 	./tests/test*.sh					\

################################################################################
#                                     PATH                                     #
################################################################################

SRC_PATH			:=	./src/

ASM_SRC_PATH		:=	./asm/

PAYLOAD_SRC_PATH	:=	./payloads/

OBJ_PATH 			:=	./obj/

ASM_OBJ_PATH		:= 	./obj_asm/

PAYLOAD_OBJ_PATH	:= 	$(ASM_OBJ_PATH)

INCLUDE_PATH		:=	./include/


################################################################################
#                                 NAME + PATH                                  #
################################################################################

SRC					:=	$(addprefix $(SRC_PATH), $(SRC_NAME))

ASM_SRC				:= 	$(addprefix $(ASM_SRC_PATH), $(ASM_SRC_NAME))

PAYLOAD_SRC			:= 	$(addprefix $(PAYLOAD_SRC_PATH), $(PAYLOAD_SRC_NAME))

OBJ					:=	$(patsubst $(SRC_PATH)%.c, $(OBJ_PATH)%.o,	$(SRC))

ASM_OBJ				:=	$(patsubst $(ASM_SRC_PATH)%.asm, $(ASM_OBJ_PATH)%.o, $(ASM_SRC))

PAYLOAD_OBJ			:=	$(patsubst $(PAYLOAD_SRC_PATH)%.asm, $(PAYLOAD_OBJ_PATH)%.o, $(PAYLOAD_SRC))

INCLUDE				:=	$(addprefix $(INCLUDE_PATH), $(INCLUDE_NAME))


################################################################################
#                                     RULES                                    #
################################################################################

all: $(ART_NAME) $(PAYLOAD_NAME) $(NAME)

$(NAME): $(ASM_OBJ) $(OBJ)

	@echo "\n$(NAME) : $(GEN)"
	@echo "\n$(_CYAN)====================================================$(_END)"
	@echo "$(_YELLOW)		COMPILING $(NAME)$(_END)"
	@echo "$(_CYAN)====================================================$(_END)"
	@$(CC) -o $(NAME) $(OBJ) $(ASM_OBJ)
	@echo "\n$(_WHITE)$(_BOLD)$@\t$(_END)$(_GREEN)[OK]\n$(_END)"
	@echo "\n"

$(PAYLOAD_NAME): $(PAYLOAD_OBJ)
	@echo ""
	@echo "\n$(_CYAN)====================================================$(_END)"
	@echo "$(_YELLOW)		COMPILING $(PAYLOAD_NAME)$(_END)"
	@echo "$(_CYAN)====================================================$(_END)"
	@nasm -f bin -o $(PAYLOAD_NAME) $(PAYLOAD_SRC_PATH)$(PAYLOAD_SRC_NAME)
	@echo "\n$(_WHITE)$(_BOLD)$@\t$(_END)$(_GREEN)[OK]\n$(_END)"
	@echo "\n"

$(OBJ_PATH)%.o: $(SRC_PATH)%.c $(INCLUDE)
	@mkdir -p $(OBJ_PATH)
	@$(CC) -I $(INCLUDE_PATH) -I $(INCLUDE_PATH) -c $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)\t"	\
		"COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"

$(ASM_OBJ_PATH)%.o: $(ASM_SRC_PATH)%.asm
	@mkdir -p $(ASM_OBJ_PATH)
	@$(AS) $(AS_FLAG) $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)\t"	\
		"COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"

# Only use to know if there is any update in the file.
$(PAYLOAD_OBJ_PATH)%.o: $(PAYLOAD_SRC_PATH)%.asm
	@mkdir -p $(PAYLOAD_OBJ_PATH)
	@$(AS) $(AS_FLAG) $< -o $@
	@echo "$(_END)$(_GREEN)[OK]\t$(_UNDER)$(_YELLOW)\t"	\
		"COMPILE :$(_END)$(_BOLD)$(_WHITE)\t$<"

tests: all
	@echo "\n$(_CYAN)====================================================$(_END)"
	@echo "$(_YELLOW)		LAUNCHING TESTS$(_END)"
	@echo "$(_CYAN)====================================================$(_END)"
	@for f in $(TESTS_SRC_NAME);  do sh $${f}; done;

clean:
	@rm -rf $(OBJ_PATH) 2> /dev/null || true
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(OBJ_PATH)"$(_END)"
	@rm -rf $(ASM_OBJ_PATH) 2> /dev/null || true
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(ASM_OBJ_PATH)"$(_END)"
	@rm -f $(ART_NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(ART_NAME)"$(_END)"


clean_payload:
	@rm -f $(PAYLOAD_NAME) $(ART_NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(PAYLOAD_NAME)"$(_END)"

fclean: clean clean_payload
	@rm -f $(NAME) woody
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)$(NAME)
	@echo "$(_YELLOW)Remove :\t$(_RED)" $(LDFLAGS)woody
	@echo "$(_END)"

re: fclean all


help:
	@echo "$(_YELLOW)Makefile for generating binary infectors."
	@echo "$(_YELLOW)Usage:                                                                    "
	@echo "$(_YELLOW)   make                                runs all                           "
	@echo "$(_YELLOW)   make all                            generates all binaries             "
	@echo "$(_YELLOW)   make clean                          remove the generated files         "
	@echo "$(_YELLOW)   make fclean                  		clean and remove binaries files    "
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
