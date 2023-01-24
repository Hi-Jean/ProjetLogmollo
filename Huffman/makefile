CC_FLAGS = -W -Wall -Werror -g

test.out: tree_etu.c test_tree_etu.c list.o tree_etu.h
	gcc $(CC_FLAGS) tree_etu.c test_tree_etu.c tree_etu.h list.o -o test.out