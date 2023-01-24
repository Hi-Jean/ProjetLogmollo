#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Codage/codage.h"
#include "../Huffman/tree_etu.h"
#include "../EncodeurDecodeur/encode_decode.h"
#include "encodage_decodage.h"


int main(void) {

    encodage("out.bin", 'w');


    decodage("out.bin", 0);

    return 1;
}