#include <stdio.h>
#include <stdlib.h>

typedef struct deneme
{
    void* content;
    struct deneme *next;

}t_list;


t_list * lst_new_node(void *content)
{
    t_list *root;
    root = malloc(sizeof(t_list));

    root->content = content;
    return root;
}


int main()
{
    

}