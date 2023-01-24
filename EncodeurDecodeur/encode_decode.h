#ifndef _ENCODEUR_H
#define _ENCODEUR_H
#include "../Huffman/tree_etu.h"
#include "../Codage/codage.h"

enum liste_commandes {
    ENCODE_CMD_APPEND, ENCODE_CMD_PUSH, ENCODE_CMD_ROLLBACK
};


void bloc_encodage(struct descripteur_bloc * desc, enum liste_commandes com);


int reg_input(struct descripteur_bloc * desc, t_byte * table_octet);


int reg_flush(struct descripteur_bloc * desc);


void reg_reset();


void bloc_decodage(struct descripteur_bloc * desc);



#endif