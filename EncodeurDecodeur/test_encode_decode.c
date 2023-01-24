#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Codage/codage.h"
#include "../Huffman/tree_etu.h"
#include "encode_decode.h"


// Recuperee sur internet pour verifier l'affichage des bits/octets
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 


// AJOUTEE
int main(void) {

    struct byteCoding tab1[256];
    tree_resetByteCoding(tab1);
    tree_resetByteOccurrence(tab1);

    char chaine1[] = "Nouvelle chaine de caracteres devant servir à la création d'une table d'encodage";

    tree_countByteOccurrence((t_byte*)chaine1, strlen(chaine1)+1, tab1);

    struct tree_node * t_test1 = tree_create(tab1);
    if(t_test1 == NULL) {
        printf("Erreur: arbre t_test1 null\n");
        return -1;
    }

    tree_buildHuffmanCode(t_test1,0,0);

    tree_display(t_test1,0);

    struct descripteur_bloc * desc = bloc_create(tab1);

    struct descripteur_bloc * d = bloc_descOpen((t_byte *) desc->tete);

    int esp_dispo = bloc_freeSpace(d);

    reg_input(d, (t_byte *) "N'hZ\n");
        /*
        N: code de Huffman = 48 ; nb de bits = 6 ---> 110000
        ': code de Huffman = 4 ; nb de bits = 6 ---> 000100
        h: code de Huffman = 45 ; nb de bits = 6 ---> 101101

        Z: NON PRESENT
            ---> ajout du joker (code huffman de \0 = 49 ; nb de bits = 6) = 110001
            ---> ajout de la valeur ASCII de Z = 01011010
        
        \n: NON PRESENT
            ---> ajout du joker (code huffman de \0 = 49 ; nb de bits = 6) = 110001
            ---> ajout de la valeur ASCII de \n = 00001010

        Ecrit dans la table de donnees codees les octets suivants :
            11000000 01001011 01110001 01011010 11000100 (001010) (incomplet -> pas ecrit)
            <----><-----><-----><====>-<======> <====><=========>
               N     '      h     \0      Z       \0      \n
        
        */
        
    

    if(d->tete->nbBits != 40)
        printf("Erreur: nb de bit ecrit = %d (attendu = 40)\n", d->tete->nbBits);
    
    if(bloc_freeSpace(d) != esp_dispo-40)
        printf("Erreur: espace disponible = %d (attendu = %d-40)\n", bloc_freeSpace(d), esp_dispo);
    
    if(d->donneesCodees[3] != 'Z')
        printf("Erreur: 4eme octet obtenu = '%c' (attendu = 'Z')\n", d->donneesCodees[3]);


    // Ecrire dans le bloc jusqu'a completion
    while(reg_input(d, (t_byte *) "TEst D'ecrItur3 d'un NoUveau meSs4ge: iL d0i7 3tre TR3S LONG p0ur pAs AvO1r de PR0bl3Mes d'AfFIchAGe\n"))
        continue;

    printf("\nNombre de bit ecrit : %d\n", d->tete->nbBits);

    // Pour verifier l'affichage des octets ecrit dans les donnees codees
    for(int i=0; i<10; i++) {
        printf("%c\t", (char) d->donneesCodees[i]);
        printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(d->donneesCodees[i]));
    }

    FILE* fic = fopen("test.bin", "wb");
    bloc_writeFile((t_byte *) d->tete, fic);

    fclose(fic);

    fic = fopen("test.bin", "rb");
    struct descripteur_bloc * recup = bloc_descOpen(bloc_readFile(fic, 0));

    bloc_decodage(recup);

    printf("\n");

    return 0;
}