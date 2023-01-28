
#include <stdint.h>
#include <libcpp/utility.h>
#include <libc/stdiox.h>

#define VGA_TEXT_MEMORY 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 28

/* USE IN CPP FILES NOT IN C BRUH */

struct screen_t
{
    uint16_t** cursor;
    uint8_t x;
    uint8_t y;
};

typedef struct screen_t screen_t;

extern screen_t Screen;


namespace xgm
{
    namespace color
    {


    enum ColorAttributes
    {
        black_text = 0x0, 
        black = 0x00,

        blue_text = 0x1,
        blue = 0x10,
        
        green_text = 0x2,
        green = 0x20,

        cyan_text = 0x3,
        cyan = 0x30,

        red_text = 0x4,
        red = 0x40,

        magenta_text = 0x5,
        magenta = 0x50,

        brown_text = 0x6,
        brown = 0x60,

        lgray_text = 0x7,
        lgray = 0x70,

        lblue_text = 0x8,
        lblue = 0x80,

        dgray_text = 0x9,
        dgray = 0x90,

        lgreen_text = 0xA,
        lgreen = 0xA0,

        lcyan_text = 0xB,
        lcyan = 0xB0,

        lred_text = 0xC,
        lred = 0xC0,

        lmagenta_text = 0xD,
        lmagenta = 0xD0,

        yellow_text = 0xE,
        yellow = 0xE0,

        white_text = 0xF,
        white = 0xF0

    };
    }

enum class Direction
{
    UP, DOWN, LEFT, RIGHT, NONE
};

struct CollisionInfo
{
    bool is_collision;
    uint8_t color;
    xgm::Direction side;
};

CollisionInfo make_collision_info(bool x, uint8_t y, xgm::Direction z);

namespace Renderer
{

class ScreenManager
{
    private:
        uint8_t* screen_pointer;
    public:
        ScreenManager(void);
        uint8_t& operator [](uint32_t index);
        void vertical_line_create(uint8_t x, xgm::color::ColorAttributes color);
};
}

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
        void cell_remove(uint8_t x, uint8_t y);
        std::pair<uint32_t, uint32_t> size_get();
        std::pair<uint32_t, uint32_t> position_get();
        uint32_t sizex_get();
        uint32_t sizey_get();
        uint32_t positionx_get();
        uint32_t positiony_get();
        CollisionInfo collision_detect(); 

    private:
        uint32_t position_x;
        uint32_t position_y;
        uint32_t size_x;
        uint32_t size_y;
        uint32_t color;
        uint32_t class_id;
        uint8_t blank_cells_counter;
        std::pair<uint8_t, uint8_t>* BlankCells;
        bool is_destroyed;
};




}
