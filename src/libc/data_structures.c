
#include <libc/stdlibx.h>
#include <libc/data_structures.h>

void stack_create(stack_t** Stack)
{
    (*Stack) = (stack_t*)calloc(sizeof(stack_t));
    (*Stack)->number_of_elements = 0;                 //I know that calloc uses 0 but you know ;))
    (*Stack)->end = (uint32_t*)calloc(sizeof(XANIN_PMMNGR_BLOCK_SIZE));
}

void stack_push(stack_t* Stack, const address_t const value)
{
    if(Stack->number_of_elements)
        Stack->end++;

    *(Stack->end) = value;
    Stack->number_of_elements++;
}

address_t stack_pop(stack_t* Stack)
{
    if(!Stack->number_of_elements)
        return INT32_MAX;
        
    uint32_t tmp = *(Stack->end);
    Stack->end--;
    Stack->number_of_elements--;
    return tmp;
}

bool stack_is_empty(stack_t* Stack)
{
    return Stack->number_of_elements == 0 ? true : false;
}

void stack_remove(stack_t** Stack)
{
    free((*Stack)->end);
    *Stack = NULL;
    return;
}

void queue_create(void)
{

}

void binary_tree_create(void)
{

}

void tree_create(void)
{

}