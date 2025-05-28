NAME		= minishell

RM			= rm -f
SRCS_MAIN	= main.c

SRCS		= create_list_tcmd0.c \
		create_list_tcmd1.c \
		expansion0.c \
		expansion1.c \
		expansion2.c \
		free_struct.c \
		ft_remove_quote.c \
		get_args.c \
		get_nbr_token.c \
		get_token_tabstr.c \
		get_token_utils0.c \
		get_token_utils1.c \
		parsing0.c \
		parsing1.c

LIBFT_DIR	= libft-complete
LIBFT		= $(LIBFT_DIR)/libft-complete.a

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -I$(LIBFT_DIR) -g
LFLAG		= -lreadline 

OBJS_MAIN   = $(SRCS_MAIN:.c=.o) $(SRCS:.c=.o)

$(NAME): $(LIBFT) $(OBJS_MAIN)
	$(CC) $(CFLAGS) $(LFLAG) $(OBJS_MAIN) $(LIBFT) -o $(NAME) 

$(LIBFT):
	make -C $(LIBFT_DIR)

all: $(NAME)

clean:
	make -C $(LIBFT_DIR) clean
	$(RM) $(OBJS_MAIN)

fclean: clean
	make -C $(LIBFT_DIR) fclean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

