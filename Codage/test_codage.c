#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codage.h"
#include "../Huffman/tree_etu.h"


int main(void) {

    // INITIALISATION DE TABLEAUX DE BYTECODING
    // SERVENT AUX TEST DES FONCTIONS DU MODULE BLOC DE CODAGE
    struct byteCoding tab1[256];
    struct byteCoding tab2[256];
    tree_resetByteCoding(tab1);
    tree_resetByteCoding(tab2);
    tree_resetByteOccurrence(tab1);
    tree_resetByteOccurrence(tab2);

    char chaine1[] = "Chaine devant servir de comme base a la creation d'un tableau de 256 bytecoding !";
    char chaine2[] = "aabC       ?      aba   defgH  iijklMnobbPqrs    tuvjjjwaxyz";
    //char chaine2[] = "aa   jjjbbbn!ooooo!rrrbbaaa";

    tree_countByteOccurrence((t_byte*)chaine1, strlen(chaine1)+1, tab1);
    tree_countByteOccurrence((t_byte*)chaine2, strlen(chaine2)+1, tab2);

    struct tree_node * t_test1 = tree_create(tab1);
    if(t_test1 == NULL) {
        printf("Erreur: arbre t_test1 null\n");
        return -1;
    }

    tree_buildHuffmanCode(t_test1,0,0);

    struct tree_node * t_test2 = tree_create(tab2);
    if(t_test2 == NULL) {
        printf("Erreur: arbre t_test2 null\n");
        return -1;
    }
    tree_buildHuffmanCode(t_test2,0,0);



    // TESTS DES FONCTIONS DU MODULE BLOC DE CODAGE
    struct descripteur_bloc * desc_prim;
    struct descripteur_bloc * desc_sec;

    desc_prim = bloc_create(tab1);
    desc_sec = bloc_create(tab2);

    if(desc_prim->tete->nbBits != 0) printf("Erreur: Nombre de bit zone encodage du bloc desc_prim (attendu : 0 | obtenu : %d)\n", desc_prim->tete->nbBits);
    if(desc_sec->tete->nbBits != 0) printf("Erreur: Nombre de bit zone encodage du bloc desc_sec (attendu : 0 | obtenu : %d)\n", desc_sec->tete->nbBits);

    if(desc_prim->tete->tailleTable != 26) printf("Erreur: Nombre de bit zone encodage du bloc desc_prim (attendu : 26 | obtenu : %d)\n", desc_prim->tete->tailleTable);
    if(desc_sec->tete->tailleTable != 29) printf("Erreur: Nombre de bit zone encodage du bloc desc_sec (attendu : 29 | obtenu : %d)\n", desc_sec->tete->tailleTable);

    if(desc_prim->tete->tailleBloc != desc_sec->tete->tailleBloc) printf("Erreur: Taille bloc doit etre fixe : %d octets", MAX_BLOC_SIZE);




    FILE* fic = fopen("test.bin", "wb");
    if(fic == NULL) return -1;

    bloc_writeFile((t_byte*)desc_prim->tete, fic);
    bloc_writeFile((t_byte*)desc_sec->tete, fic);

    fclose(fic);

    fic = fopen("test.bin", "rb");
    if(fic == NULL) return -1;

    t_byte * bloc_cpy = bloc_readFile(fic, 1);

    fclose(fic);

    struct descripteur_bloc * ret = bloc_descOpen(bloc_cpy); 

    t_byte * bloc_ori = (t_byte *) desc_sec->tete;


    /*
    for(int i=0; i<MAX_BLOC_SIZE; i++) {
        if(bloc_cpy[i] != bloc_ori[i]) {
            printf("ERREUR: LES DEUX TABLEAUX DE t_byte SONT SENSES CORRESPONDRE\n");
            return 0;
        }
    }
    */
    // Comme boucle précédente
    if(memcmp((void *)bloc_cpy, (void *) bloc_ori, MAX_BLOC_SIZE)) {
        printf("ERREUR: LES DEUX TABLEAUX DE t_byte SONT SENSES CORRESPONDRE\n");
        return 0;
    }


    for(unsigned int i=0; i<desc_sec->tete->tailleTable; i++) {
        if(desc_sec->table[i].occurrence != ret->table_complete[desc_sec->table[i].byte].occurrence)
            printf("Erreur: table_complete de ret creee par bloc_descOpen devrait contenir meme valeur que table de desc_sec:\n\t%c ret=%d | %c desc_sec=%d)\n", ret->table_complete[desc_sec->table[i].byte].byte, ret->table_complete[desc_sec->table[i].byte].occurrence, desc_sec->table[i].byte, desc_sec->table[i].occurrence);
    }



    bloc_display(desc_sec);
    bloc_display(ret);

    bloc_close(desc_prim);
    bloc_close(desc_sec);
    desc_prim = NULL;
    desc_sec = NULL;

    free(bloc_cpy);

    return 1;
}


// gcc -W -Wall -Werror test_codage.c codage.c ../Huffman/tree_etu.c ../Huffman/list.o -o test.out