
#include <sys/net/netapi/objects/response.h>

void response_object_create(NetworkResponse** Object, uint32_t size_to_allocate)
{
    (*Object) = (NetworkResponse*)calloc(sizeof(NetworkResponse));
    (*Object)->data = (address_t)calloc(size_to_allocate);
}

void response_object_destroy(NetworkResponse** Object)
{
    free((*Object)->data);
    free(*Object);
    *Object = (NetworkResponse*)NULL;
}
