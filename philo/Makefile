# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: weiyang <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/06 08:55:57 by weiyang           #+#    #+#              #
#    Updated: 2025/05/19 08:13:52 by weiyang          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = philo
FLAG = -Wall -Wextra -Werror
PATH_SRC = srcs/
SRC = actions.c dinner.c getter_setter.c init.c main.c monitor.c parse.c \
      synchro_utils.c utils.c wrapper_safe.c write.c

OBJ = $(addprefix $(PATH_SRC), $(SRC:.c=.o))
PATH_HEADER = includes/

all: $(NAME)

$(NAME): $(OBJ)
	cc -Wall -Wextra -Werror -pthread $(OBJ) -I$(PATH_HEADER) -o $(NAME)

$(PATH_SRC)%.o: $(PATH_SRC)%.c
	cc $(FLAG) -I$(PATH_HEADER) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
