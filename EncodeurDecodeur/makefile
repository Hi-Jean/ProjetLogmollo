CC_FLAGS = -W -Wall -Werror -g

test.out: encode_decode.c test_encode_decode.c ../Codage/codage.c ../Huffman/tree_etu.c ../Huffman/list.o
	gcc $(CC_FLAGS) encode_decode.c test_encode_decode.c ../Codage/codage.c ../Huffman/tree_etu.c ../Huffman/list.o -o test.out