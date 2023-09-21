
static inline void memcpy(uint8_t* dest, uint8_t* src, size_t size)
{
    for(int i = 0; i < size; i++)
        dest[i] = src[i];
}

static inline bool memcmp(uint8_t *dst, uint8_t* src, size_t size)
{
    for(int i = 0; i < size; i++)
        if(dst[i] != src[i])
            return false;

    return true;
    
}

static inline void memset(uint8_t *dst, uint8_t value, size_t size)
{
    for(int i = 0; i < size; i++)
        dst[i] = value;
}
