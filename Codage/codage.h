#ifndef _CODAGE_H
#define _CODAGE_H
#include "../Huffman/tree_etu.h"

#define MAX_BLOC_SIZE 32*1024

// En-tete bloc de codage
// Contient les informations globales du bloc
struct en_tete {
    unsigned int nbBits;
    unsigned int nbMaxBits;
    unsigned int tailleBloc;
    unsigned int tailleTable;
};


// Descripteur de bloc de codage
// Contient les pointeur vers les différentes parties du bloc
struct descripteur_bloc {
    struct en_tete * tete;
    struct byteCoding * table;
    t_byte * donneesCodees;
    struct tree_node * arbre;
    struct byteCoding table_complete[256];

};


// Creation d'un bloc, et du code de Huffman associé
// Ne stocke que les byteCoding avec nbOccurence != 0
struct descripteur_bloc * bloc_create(struct byteCoding indexedCodeTable[256]);

// Lecture/Ecriture d'un bloc dans un fichier
// Ecriture table --> juste les struct byteCoding 
// Fichiers binaire
t_byte * bloc_readFile(FILE* fichier, int num_bloc); 
int bloc_writeFile(t_byte * bloc, FILE* fichier);
    // retourne 1 si écriture complète du bloc réussi, 0 si écriture incomplète, -1 si échec


// Calcul de la place disponible dans le bloc(en nombre de bits)
int bloc_freeSpace(struct descripteur_bloc * bloc);

// Ouverture d'un bloc (ie: construction de l'arbre et de la table de codage associé au bloc)
struct descripteur_bloc * bloc_descOpen(t_byte * bloc);

// Fermeture d'un bloc (libération de la mémoire utilisées)
void bloc_close(struct descripteur_bloc * bloc);

// affichage des méta-données d'un bloc (en-tête, et table de codage)
void bloc_display(struct descripteur_bloc * bloc) ;

#endif
