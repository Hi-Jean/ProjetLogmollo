#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codage.h"
#include "../Huffman/tree_etu.h"


// Creation d'un bloc, et du code de Huffman associé
// Ne stocke que les byteCoding avec nbOccurence != 0
struct descripteur_bloc * bloc_create(struct byteCoding indexedCodeTable[256]) {

    struct descripteur_bloc * ret = (struct descripteur_bloc *) malloc(sizeof(struct descripteur_bloc));

    if(ret == NULL) return NULL;

    t_byte * bloc = (t_byte *) malloc(MAX_BLOC_SIZE*sizeof(t_byte));

    if(bloc == NULL) {
        free(ret);
        return NULL;
    }
    ret->tete = (struct en_tete *) bloc;

    ret->tete->tailleBloc = MAX_BLOC_SIZE;
    ret->tete->nbBits = 0;
    ret->tete->tailleTable = 0;


    ret->table = (struct byteCoding *) ((ret->tete) + 1);

    struct tree_node * t = tree_create(indexedCodeTable);
    if(t == NULL) return NULL;

    tree_buildHuffmanCode(t,0,0);



    for(int i=0; i<256; i++) {
        if(indexedCodeTable[i].occurrence > 0) {
            *(ret->table + ret->tete->tailleTable) = indexedCodeTable[i];
            ret->tete->tailleTable++;
        }
    }

    ret->donneesCodees = (t_byte *) ret->table + ret->tete->tailleTable;

    ret->tete->nbMaxBits = (unsigned int) (&bloc[MAX_BLOC_SIZE] - ((t_byte *)ret->donneesCodees)) * 8;

    return ret;
}


// Lecture/Ecriture d'un bloc dans un fichier
t_byte * bloc_readFile(FILE* fichier, int num_bloc) {
    t_byte * ret = (t_byte *) malloc(MAX_BLOC_SIZE * sizeof(t_byte));

    fseek(fichier, num_bloc*MAX_BLOC_SIZE, SEEK_SET);

    fread((void*) ret, 1, MAX_BLOC_SIZE, fichier);

    return ret;
}


// retourne 1 si écriture complète du bloc réussi, 0 si écriture incomplète, -1 si échec
int bloc_writeFile(t_byte * bloc, FILE* fichier) {

    if( MAX_BLOC_SIZE == ((int) fwrite((void *) bloc, 1, MAX_BLOC_SIZE, fichier)))
        return 1;
    else return 0;

}


// Calcul de la place disponible dans le bloc(en nombre de bits)
int bloc_freeSpace(struct descripteur_bloc * bloc) {
    return bloc->tete->nbMaxBits - bloc->tete->nbBits;
}


// Ouverture d'un bloc (ie: construction de l'arbre et de la table de codage associé au bloc)
struct descripteur_bloc * bloc_descOpen(t_byte * bloc) {
    struct descripteur_bloc * ret = (struct descripteur_bloc *) malloc(sizeof(struct descripteur_bloc));

    if(ret == NULL) return NULL;

    ret->tete = (struct en_tete *) bloc;
    ret->table = (struct byteCoding *) (ret->tete + 1);
    ret->donneesCodees = (t_byte *) (ret->table + ret->tete->tailleTable);

    
    // arbre et table_complete à construire

    tree_resetByteCoding(ret->table_complete);
    tree_resetByteOccurrence(ret->table_complete);

    for(unsigned int i=0; i<ret->tete->tailleTable; i++) {
        ret->table_complete[ret->table[i].byte].occurrence = ret->table[i].occurrence;
        ret->table_complete[ret->table[i].byte].huffmanCode = ret->table[i].huffmanCode;
        ret->table_complete[ret->table[i].byte].nbBits = ret->table[i].nbBits;
    }

    ret->arbre = tree_create(ret->table_complete);


    return ret;
}





// Fermeture d'un bloc (libération de la mémoire utilisées)
void bloc_close(struct descripteur_bloc * bloc) {
    if(bloc != NULL) {
        free(bloc->tete);
        bloc->tete = NULL;
        bloc->table = NULL;
        bloc->donneesCodees = NULL;
        tree_destroy(bloc->arbre);
        bloc->arbre = NULL;
        free(bloc);
    }
}


// affichage des méta-données d'un bloc (en-tête, et table de codage)
void bloc_display(struct descripteur_bloc * bloc) {
    fprintf(stdout, "En-tete:\n");
    fprintf(stdout, "\tTaille courante bloc donnee (en bits): %d\n", bloc->tete->nbBits);
    fprintf(stdout, "\tTaille  max bloc donnee (en bits): %d\n", bloc->tete->nbMaxBits);
    fprintf(stdout, "\tTaille totale du bloc (en bits): %d\n", bloc->tete->tailleBloc);
    fprintf(stdout, "\tTaille table des valeurs d'octet: %d\n", bloc->tete->tailleTable);

    fprintf(stdout, "Table des valeurs d'octet:\n");
    for(unsigned int i=0; i<bloc->tete->tailleTable; i++) {
        struct byteCoding * b = (bloc->table) + i;
        fprintf(stdout, "\t%c : occ=%d  codeHuffman=%d (nbBit=%d)\n", (char) b->byte, b->occurrence, b->huffmanCode, b->nbBits);
    }

    fprintf(stdout, "\n");
}