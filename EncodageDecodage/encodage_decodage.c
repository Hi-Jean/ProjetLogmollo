#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Codage/codage.h"
#include "../Huffman/tree_etu.h"
#include "../EncodeurDecodeur/encode_decode.h"
#include "encodage_decodage.h"

static int nb_bloc_ecrit = 0;


void encodage(const char* out_path_name, char mode) {
    FILE* fout;

    struct byteCoding tab[256];
    tree_resetByteCoding(tab);
    tree_resetByteOccurrence(tab);
    char chaine[] = "Nouvelle chaine de caracteres devant servir à la création d'une table d'encodage";

    tree_countByteOccurrence((t_byte*)chaine, strlen(chaine)+1, tab);

    struct tree_node * t = tree_create(tab);
    tree_buildHuffmanCode(t,0,0);

    tree_display(t,0);

    struct descripteur_bloc * first = bloc_create(tab);
    struct descripteur_bloc * desc = bloc_descOpen((t_byte *) first->tete);

    

    if(mode == 'a')
        fout = fopen(out_path_name, "ab");
    else if(mode == 'w')
        fout = fopen(out_path_name, "wb");
    else
        return;


    tree_resetByteCoding(tab);
    tree_resetByteOccurrence(tab);

    int ajoute;
    char src[256];
    //fscanf(stdin, " %s", src);
    fgets(src, 256, stdin);

    while(src[0] != '\n') {
        ajoute = reg_input(desc, (t_byte *) src);

        if(!ajoute) {
            bloc_writeFile((t_byte *) desc->tete, fout);
            bloc_close(desc);

            nb_bloc_ecrit++;

            t = tree_create(tab);
            tree_buildHuffmanCode(t,0,0);
            desc = bloc_create(tab);

            tree_resetByteCoding(tab);
            tree_resetByteOccurrence(tab);
            continue;
        }

        tree_countByteOccurrence((t_byte*)src, strlen(src)+1, tab);
        
        fgets(src, 256, stdin);
    }

    nb_bloc_ecrit++;

    reg_flush(desc);

    bloc_writeFile((t_byte *) desc->tete, fout);
    bloc_close(desc);
    //bloc_close(first);

    fclose(fout);

    

}



void decodage(const char* in_path_name, int num_bloc) {

    if(num_bloc >= nb_bloc_ecrit) {
        printf("Erreur: bloc choisi inexistant\n");
        exit(1);
    }


    FILE* fin = fopen(in_path_name, "rb");

    t_byte* bloc = bloc_readFile(fin, num_bloc);
    struct descripteur_bloc * desc = bloc_descOpen(bloc);

    bloc_decodage(desc);
        
    fclose(fin);
}







