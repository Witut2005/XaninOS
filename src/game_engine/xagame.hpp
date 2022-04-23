
#include <stdint.h>
#include <lib/colors.h>

/* USE IN CPP FILES NOT IN C BRUH */

struct screen_t
{
    uint16_t** cursor;
    uint8_t x;
    uint8_t y;
};

typedef struct screen_t screen_t;

extern screen_t Screen;

#define set_output_color(x,y) (x << 4 | y)

namespace xgm
{

class GeometryObject
{    

    public:
        virtual void create(uint32_t x, uint32_t y, uint32_t size_x, uint32_t size_y, uint8_t color) = 0;
        virtual void move(int32_t x, int32_t y) = 0;
        virtual void destroy() = 0;
        virtual void rotate_right_90() = 0;

};

class rectangle : public GeometryObject
{
    public:
        rectangle(uint32_t class_id);
        void create(uint32_t x, uint32_t y, uint32_t size_x, uint32_t size_y, uint8_t color);
        void move(int32_t x, int32_t y);
        void destroy();
        void rotate_right_90();

    private:
        uint32_t position_x;
        uint32_t position_y;
        uint32_t size_x;
        uint32_t size_y;
        uint32_t color;
        uint32_t class_id;
        bool is_destroyed;
};

extern "C" void __cxa_pure_virtual()
{
    return;
}

}

/*
extern "C" void class_test(void)
{

    struct GeometryObject* x;
    struct rectangle c;


    x = &c;
    x->create(10,10, 20,5, lred);

}
*/