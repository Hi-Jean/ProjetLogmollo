CC_FLAGS = -W -Wall -Werror -g

test.out: encodage_decodage.c test_encodage_decodage.c ../EncodeurDecodeur/encode_decode.c ../Codage/codage.c ../Huffman/tree_etu.c ../Huffman/list.o
	gcc $(CC_FLAGS) encodage_decodage.c test_encodage_decodage.c ../EncodeurDecodeur/encode_decode.c ../Codage/codage.c ../Huffman/tree_etu.c ../Huffman/list.o -o test.out