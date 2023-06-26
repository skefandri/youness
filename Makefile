NAME = minishell

SRC_MAN = minishell.c ./utils/ft_split.c ./utils/ft_strdup.c ./src/lexer.c ./src/syntax_error.c\
		./utils/ft_bzero.c  ./utils/ft_substr.c ./utils/ft_strjoin.c ./utils/ft_strcmp.c ./utils/ft_utlis_2.c\
		./utils/ft_utils_1.c ./utils/ft_calloc.c ./utils/ft_strlcpy.c \

CFLAGS = -Wextra -Werror -Wall -g3 -lreadline -fsanitize=address -g3

all : $(NAME)

$(NAME) : $(SRC_MAN)
		cc  $(CFLAGS) $(SRC_MAN) -o $(NAME)

clean :
		rm -rf $(OBJS)

fclean : clean
		rm -rf $(NAME)

re : fclean all
