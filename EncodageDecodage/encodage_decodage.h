#ifndef _ENCODAGE_H
#define _ENCODAGE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Huffman/tree_etu.h"
#include "../Codage/codage.h"
#include "../EncodeurDecodeur/encode_decode.h"


void encodage(const char* out_path_name, char mode);

void decodage(const char* in_path_name, int num_bloc);



#endif