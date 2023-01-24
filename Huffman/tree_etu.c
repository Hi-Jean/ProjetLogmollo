/* ----------------------

---------------------- */

#include <stdio.h>
#include <stdlib.h>
#include "tree_etu.h"
#include "list.h"

/****** gestion du tableau de codage des octets ******/

// remet a zero le nombre d'occurrences des octets
void tree_resetByteOccurrence( struct byteCoding indexedCodeTable[256] ) {
    for(int i=0; i<256; i++) {
        indexedCodeTable[i].occurrence = 0;
    }
}

// remet a zero les codes octet
void tree_resetByteCoding( struct byteCoding indexedCodeTable[256] ) {
    for(int i=0; i<256; i++) {
        indexedCodeTable[i].nbBits = 0;
        indexedCodeTable[i].huffmanCode = 0;
        indexedCodeTable[i].byte = i;
    }
}

// compte de nombre d'occurrence des octets dans un tampon
void tree_countByteOccurrence( const t_byte * buffer, int size,
			       struct byteCoding indexedCodeTable[256] ) {
    for(int i=0; i<size; i++) {
        indexedCodeTable[buffer[i]].occurrence++;
    }
}

// affiche la table de codage des valeurs d'octet pour debug
void tree_displayByteCoding(struct byteCoding indexedCodeTable[256]) {
    fprintf(stdout,"Table de codage :\n");
    for(int i=0; i<256; i++)
        fprintf(stdout,"%03d %c (%d) : %d\n", i, i, indexedCodeTable[i].occurrence, indexedCodeTable[i].huffmanCode);
}
	


/****** gestion de l'arbre de Huffman ******/

// creation d'un noeud
struct tree_node * tree_createNode (struct tree_node * father, int * value ) {
    struct tree_node * n = (struct tree_node *) malloc(sizeof(struct tree_node));
    if(n != NULL) {
        n->feuille = value;
        n->pere = father;
    }
    return n;
}

// creation d'un noeud de codage et liaison avec ses fils
struct tree_node * tree_createCodingNode ( struct tree_node * left, struct tree_node * right ) {
    struct tree_node * n;
    int * value = (int*) malloc(sizeof(int));
    if(value != NULL){
        *value = *right->feuille + *left->feuille;
        n = tree_createNode(NULL, value);
        n->brch_d = right;
        n->brch_g = left;
        if(left != NULL)
            left->pere = n;
        if(right != NULL)
            right->pere = n;
    }
    return n;
}


    // Comparaison noeud de la list et data
    int compare(s_node * s, void * data) {

        struct tree_node * n = GET_LIST_DATA(s, struct tree_node *);
        struct byteCoding * b1 = (struct byteCoding *) n->feuille;

        struct tree_node * noeud_ajout = (struct tree_node *) data;
        struct byteCoding * b2 = (struct byteCoding *) noeud_ajout->feuille;

        int ret = b1->occurrence > b2->occurrence;

        return ret;
    }


// creation de l'arbre
struct tree_node * tree_create ( struct byteCoding * indexedCodeTable ) {
    struct tree_node * root = tree_createNode(NULL, 0);
    struct tree_node *n, *n1, *n2;
    
    s_node * l = list_create();

    for(int i=0; i<256; i++) {
        if(indexedCodeTable[i].occurrence > 0) {

            (void)list_orderedAppend(&l, &compare, (void *) tree_createNode(NULL, &indexedCodeTable[i].occurrence));
        }
    }

    /*
    //affiche tous les éléments contenu dans la liste
    s_node * test = l;
    while(test != NULL) {
        struct tree_node * n = GET_LIST_DATA(test, struct tree_node *);
        struct byteCoding * b = (struct byteCoding *) n->feuille;
        printf("(%d : %c)   ", b->occurrence, b->byte);
        test = test->next;
    }
    printf("\n\n");
    */    

    while(list_next(l) != NULL) {
        n1 = GET_LIST_DATA(l, struct tree_node *);
        n2 = GET_LIST_DATA(l->next, struct tree_node *);
        
        n = tree_createCodingNode(n1,n2);
        n->pere = NULL;
        
        l = list_headRemove(l);
        l = list_headRemove(l);

        // l = list_insert(l,n);
        (void)list_orderedAppend(&l, &compare, (void *) n);

        /*
        s_node * test = l;
        while(test != NULL) {
            struct tree_node * n = GET_LIST_DATA(test, struct tree_node *);
            tree_display(n,0);
            test = test->next;
        }
        printf("\n\n\n\n");
        */
        
    }
    
    root = GET_LIST_DATA(l, struct tree_node *);
    return root;
}

// destruction d'un arbre et liberation des donnees 
// (sauf au niveau des feuilles)
void tree_destroy( struct tree_node * root ) {
    if(root != NULL) {
        if(root->brch_d != NULL && root->brch_g != NULL) {
            free(root->feuille);
            tree_destroy(root->brch_d);
            tree_destroy(root->brch_g);
        }
        free(root);
        root = NULL;
    }
}

// construit les codes de huffman en parcourant l'arbre
void tree_buildHuffmanCode(struct tree_node * root, int level, int code) {

    if(root->brch_d == NULL && root->brch_g == NULL) {
        struct byteCoding * f = (struct byteCoding *) root->feuille;
        f->huffmanCode = code;
        f->nbBits = level;
    } else {
        tree_buildHuffmanCode(root->brch_d, level+1, code*2+0);
        tree_buildHuffmanCode(root->brch_g, level+1, code*2+1);
    }
}

// affichage de l'arbre
void tree_display( struct tree_node * root, int level ) {
    if(root->brch_d == NULL && root->brch_g == NULL) {
        struct byteCoding * f = (struct byteCoding *) root->feuille;
        for(int i=0; i<level; i++)
            fprintf(stdout,"\t");
        fprintf(stdout,"'%c'(nb=%d) : %d  %d\n",f->byte, f->occurrence, f->huffmanCode, f->nbBits);
    } else {
        struct byteCoding * n = (struct byteCoding *) root->feuille;

        tree_display(root->brch_d,level+1);

        for(int i=0; i<level; i++)
            fprintf(stdout,"\t");
        fprintf(stdout,"(nb=%d)\n", n->occurrence);

        tree_display(root->brch_g,level+1);
    }
}
	
