SRC		:=	$(shell find ./src -type f -name '*.cpp')
TEST_SRC := $(shell find -type f -name '*.cpp' ! -name 'main.cpp')

OBJ		 = $(SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)

CXX		 = g++
CFLAGS	 = -std=c++20 -Wall -Wextra
V_FLAG   = --leak-check=full --show-leak-kinds=all --track-origins=yes
EXE		 = nanotekspice
TEST_EXE = nanotester
SAN		 = -g3

all:	$(EXE)

%.o:	%.cpp
		$(CXX) -o $@ -c $< $(CFLAGS) $(SAN)

$(EXE): $(OBJ)
		$(CXX) -o $(EXE) $^

$(TEST_EXE): $(TEST_OBJ)
	    $(CXX) -o $(TEST_EXE) $^ -lcriterion --coverage

test: $(TEST_EXE)
		./$(TEST_EXE)

clean:
		@rm -rf $(OBJ)
		@rm -rf $(TEST_OBJ)
		@rm -rf vgcore*
		@rm -rf *.gcda
		@rm -rf *.gcno

fclean:	clean
		@rm -rf $(EXE)
		@rm -rf $(TEST_EXE)

re:	fclean all

val:
	make re && valgrind ./$(EXE)

val_full:
	make re && valgrind $(V_FLAG) ./$(EXE)
