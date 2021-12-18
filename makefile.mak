# PROGRAM:    binary_search_tree
# PROGRAMMER: Kyin Edwards
# DATE WRITTEN:   4/15/2021

# Compiler variables.
CXXFLAGS = -Wall -Werror -std=c++11

# Rule to link object code files to create executable file.

binary_search_tree: binary_search_tree.o
	g++ $(CXXFLAGS) -o binary_search_tree binary_search_tree.o

# Rules to compile source code files to object code.
binary_search_tree.o: binary_search_tree.cpp bstree.h

# Pseudo-target to remove object code and executable files.
clean:
	rm -f binary_search_tree *.o
