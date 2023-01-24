/* -----------------------------------------------------------
   module list 
   -----------------------------------------------------------
   08/11/08	SR	creation
   -----------------------------------------------------------
*/

#ifndef _LIST_H_
#define _LIST_H_
struct _list_node {
	void * data;
	struct _list_node * next;
};
typedef struct _list_node s_node;

s_node * list_create(void);
s_node * list_insert(s_node * head, void * data);
s_node * list_append(s_node * head, void * data);
s_node * list_append2(s_node ** head, s_node * queue, void * data);
s_node * list_remove(s_node * head, void * data);
s_node * list_headRemove(s_node * head);
s_node * list_destroy(s_node * head);
int list_process(s_node * head, int (*fct)(s_node * node, 
			void * param), void * param, s_node ** last);
int list_isEmpty(s_node * head); 
s_node * list_orderedAppend(s_node ** head, int (*compare)(s_node *, void *), 
		void *param);
s_node * list_next(s_node * node);

// type en un type pointeur generique
#define GET_LIST_DATA(node, type) ((type)(node->data))
#define SET_LIST_DATA(node, mydata) (node->data=(void*)(mydata))
#endif
