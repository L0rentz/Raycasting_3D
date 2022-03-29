NAME			=	raycasting3d

CXXDIRECTORIES	=   src
CXXINCLOOP		=	$(foreach d, $(CXXDIRECTORIES), -I$d)

CXXFLAGS		=	-g3 -Wall -Wextra $(CXXINCLOOP) -O3

SRC				= 	main.cpp \
					src/Core.cpp \
					src/Player.cpp \
					src/Map.cpp

OBJ				=	$(SRC:.cpp=.o)


LDFLAGS			=	-ldl -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lGL

RM				=	rm -rf

all: $(NAME)

$(NAME): $(OBJ)
	g++ -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
