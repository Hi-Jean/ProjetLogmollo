#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Codage/codage.h"
#include "../Huffman/tree_etu.h"
#include "encode_decode.h"

static int registre = 0;
static int pos = 0;
static int save_registre = 0;
static unsigned int save_nbBits = 0;

void bloc_encodage(struct descripteur_bloc * desc, enum liste_commandes com) {

    switch(com) {
        case ENCODE_CMD_APPEND:

            desc->donneesCodees[desc->tete->nbBits/8] += (registre >> (pos-8) & 0xFF);

            desc->tete->nbBits += 8;
            pos -= 8;
            break;
        
        case ENCODE_CMD_PUSH:
            registre = 0;
            pos = 0;
            break;

        case ENCODE_CMD_ROLLBACK:
            registre = save_registre;
            desc->tete->nbBits = save_nbBits;
            break;

        default:
            break;
    }
}

// MODIFIEE
// Ajout du cas du joker
int reg_input(struct descripteur_bloc * desc, t_byte * table_octet) {
    save_registre = registre;
    save_nbBits = desc->tete->nbBits;
    int i=0;

    while(table_octet[i] != '\0') {

        if(desc->table_complete[table_octet[i]].occurrence > 0) {
            registre = (registre << desc->table_complete[table_octet[i]].nbBits) + desc->table_complete[table_octet[i]].huffmanCode;
            pos += desc->table_complete[table_octet[i]].nbBits;
        } else {
            registre = (registre << desc->table_complete['\0'].nbBits) + desc->table_complete['\0'].huffmanCode;
            pos += desc->table_complete['\0'].nbBits;
            registre = (registre << 8) + table_octet[i];
            pos += 8;
        }

        while(pos >= 8) {
            if(bloc_freeSpace(desc) < 8) {
                bloc_encodage(desc, ENCODE_CMD_ROLLBACK);
                return 0;
            } else {
                bloc_encodage(desc, ENCODE_CMD_APPEND);
            }
            
        }
        i++;
    }

    return 1;
}

// AJOUTEE
// Devait servir à vider le registre dans le bloc courant (même si il ne forme pas un bit complet)
int reg_flush(struct descripteur_bloc * desc) {

    if(bloc_freeSpace(desc) < pos) {
        printf("Erreur");
        return 1;
    }

    desc->donneesCodees[desc->tete->nbBits/8] += (registre << (8-pos+1) & 0xFF);

    desc->tete->nbBits += pos;
    pos = 0;

    return 0;
}


static unsigned int index_octet = 0;
static int registre_dec = 0;
static int pos_dec = -1;
static int save_pos = 0;

// AJOUT
void reg_reset() {
    registre_dec = 0;
    pos_dec = -1;
}

// AJOUT
void bloc_decodage(struct descripteur_bloc * desc) {
    struct tree_node * t;
    save_registre = registre_dec;
    save_pos = pos_dec;
    while(index_octet < desc->tete->nbBits/8) {

        t = desc->arbre;

        while(t->brch_d != NULL && t->brch_g != NULL) {
            if(pos_dec < 0) {
                registre_dec = (registre_dec << 8) + desc->donneesCodees[index_octet++];
                pos_dec += 8;
            }

            if((registre_dec >> pos_dec) % 2 == 0)
                t = t->brch_d;
            else
                t = t->brch_g;
            pos_dec--;
        }

        if(((struct byteCoding *)t->feuille)->byte != '\0') {
            printf("%c", ((struct byteCoding *)t->feuille)->byte);
        } else {
            registre_dec = (registre_dec << 8) + desc->donneesCodees[index_octet++];
            printf("%c", registre_dec >> (pos_dec+1));
        }

    }

}




