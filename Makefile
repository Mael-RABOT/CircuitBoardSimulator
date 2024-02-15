SRC		:=	$(shell find . -type f -name '*.cpp')

OBJ		=	$(SRC:.cpp=.o)

CXX		=	g++
CFLAGS	= -std=c++20 -Wall -Wextra
V_FLAG  = --leak-check=full --show-leak-kinds=all --track-origins=yes
EXE		= nanotekspice
SAN		= -g3
TEST_SCRIPT = ./test/tester.sh

all:	$(EXE)

%.o:	%.cpp
		$(CXX) -o $@ -c $< $(CFLAGS) $(SAN)

$(EXE): $(OBJ)
		$(CXX) -o $(EXE) $^

test: re
		@$(TEST_SCRIPT)

clean:
		@rm -rf $(OBJ)
		@rm -rf vgcore*
		@rm -rf *.gcda
		@rm -rf *.gcno

fclean:	clean
		@rm -rf $(EXE)

re:	fclean all

val:
	make re && valgrind ./$(EXE)

val_full:
	make re && valgrind $(V_FLAG) ./$(EXE)
