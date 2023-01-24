#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree_etu.h"
#include "list.h"

int main ( void ) {

    struct byteCoding tab[256];
    tree_resetByteCoding(tab);
    tree_resetByteOccurrence(tab);

    char str[] = "aaaaaaCCChaine  de test !";
//char str[] = "\0\0";
    // Doit aussi compter les cractère de fin ? => strlen+1
    tree_countByteOccurrence((t_byte*)str, strlen(str)+1, tab);
//tree_countByteOccurrence((t_byte*)str, 3, tab);
    if(tab['e'].occurrence != 3)
        fprintf(stdout, "Erreur : nb occurence de 'e' attendu = 3 : obtenu = %d\n", tab['e'].occurrence);
    if(tab['s'].occurrence != 1)
        fprintf(stdout, "Erreur : nb occurence de 's' attendu = 2 : obtenu = %d\n", tab['s'].occurrence);
    if(tab['\0'].occurrence != 1)
        fprintf(stdout, "Erreur : nb occurence de '\\0' attendu = 1 : obtenu = %d\n", tab['\0'].occurrence);
    
    //tree_displayByteCoding(tab);

    struct tree_node * t = tree_create(tab);
    if(t == NULL) {
        printf("Erreur: arbre t est NULL\n");
        return -1;
    }
    tree_buildHuffmanCode(t,0,0);

    tree_display(t,0);
    // affiche horizontalement l'arbre créé
    // les noeuds affichent le nombre de caractère total de ses descendant -> (nb=nb_occurence_tot)
    // les feuilles affichent le caractère de la table de codage auquel elles sont liées, ainsi que leur nb d'occurence et leur code de huffman -> 'car' (nb=nb_occurence_ccar) : code_huffman


    
    if(*(t->feuille) != 26)
        fprintf(stdout, "Erreur: nb occurence de car total de car dans l'arbre attendu = 26 (obtenu = %d)\n", *(t->feuille));

    tree_destroy(t);

    /*
    // Seg fault -> arbre bien détruit
    tree_display(t,0);
    */

    return 0;
}