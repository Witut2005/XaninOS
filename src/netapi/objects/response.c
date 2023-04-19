
#include <netapi/objects/response.h>

NetworkResponse* response_object_create(NetworkResponse** Object, uint32_t size_to_allocate)
{
    (*Object) = (NetworkResponse*)calloc(sizeof(NetworkResponse));
    (*Object)->data = (address_t)calloc(size_to_allocate);
}