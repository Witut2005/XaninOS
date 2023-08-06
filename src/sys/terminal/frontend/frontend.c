

#include <sys/macros.h>
#include <sys/terminal/backend/backend.h>
#include <sys/terminal/frontend/frontend.h>
#include <sys/pmmngr/alloc.h>
#include <lib/libc/stdiox.h>
#include <lib/libc/memory.h>


// if size_allocated is to small then error
Xtf* xtf_init(uint32_t buffer_size)
{
    Xtf* XtFrontend = (Xtf*)calloc(sizeof(XtFrontend)); // all variables = zeros    

    XtFrontend->vwidth = VGA_WIDTH; // 80
    XtFrontend->vheight = 1000; //useless
    XtFrontend->size_allocated = buffer_size >= XANIN_PMMNGR_BLOCK_SIZE * 2 ? buffer_size : XANIN_PMMNGR_BLOCK_SIZE * 2;
    XtFrontend->size = 0;
    XtFrontend->current_height = __vga_text_mode_height_get(); 
    XtFrontend->Cursor.position = -1;
    XtFrontend->scrolling_enabled = true;
    XtFrontend->buffer = (terminal_cell*)calloc(XtFrontend->size_allocated);
    XtFrontend->rows_changed = (uint8_t*)calloc(XtFrontend->current_height * sizeof(uint8_t));

    memset(XtFrontend->rows_changed, true, XtFrontend->current_height);

    return XtFrontend;
}

void xtf_destroy(Xtf* XtFrontend) 
{
    free(XtFrontend);
}

int xtf_buffer_nth_line_size_get(Xtf* XtFrontend, uint32_t line_number)
{
    int index = xtf_buffer_nth_line_index_get(XtFrontend, line_number);
    int size = 0;

    while((((char)XtFrontend->buffer[index] != '\n') || ((char)XtFrontend->buffer[index] != '\x1e')) && ((char)XtFrontend->buffer[index] != '\0'))
    {
        size++;
        index++;
    }

    return size;

}

int xtf_get_number_of_lines(Xtf* XtFrontend)
{
    const char* buffer = (char*)XtFrontend->buffer;
    int lines_total = 0;

    while(*buffer != '\0')
    {
        if (((char)*buffer == '\n') || ((char)*buffer == '\x1e'))
            lines_total++;

        buffer += 2;
    }

    return lines_total;

}

int xtf_buffer_nth_line_index_get(Xtf* XtFrontend, uint32_t line_number) // starting with 0
{
    const terminal_cell* buffer = XtFrontend->buffer;
    int current_line = 0;
    int index = 0;
 
    if(!line_number)
        return 0;

    while ((char)*buffer != '\0')
    {
        if (((char)*buffer == '\n') || ((char)*buffer == '\x1e'))
        {
            if (++current_line >= line_number)
                break;
        }

        buffer++;
        index++;
    }

    if ((char)*buffer == '\0')
        return -1;
    
    return index + 1;
}

int xtf_get_line_number_from_position(Xtf* XtFrontend, uint32_t position)
{

    if(position > XtFrontend->size)
        return XT_NO_SUCH_LINE;

    int current_line = 0;

    for(int i = 0; i < position; i++)
    {
        if((XtFrontend->buffer[i] == SAFE_NEW_LINE) || (XtFrontend->buffer[i] == NEW_LINE))
            current_line++;
    }

    return current_line;

}

void xtf_remove_last_cell(Xtf* XtFrontend)
{

    XtFrontend->rows_changed[XtFrontend->y] = true;
    if(XtFrontend->x == 0)
    {
        XtFrontend->y--;
        XtFrontend->rows_changed[XtFrontend->y] = true;

        if(XtFrontend->y < XtFrontend->y_begin)
            XtFrontend->y_begin = XtFrontend->y;

        XtFrontend->buffer[XtFrontend->size--] = '\0';
    }

    else
        XtFrontend->x--;

    XtFrontend->buffer[XtFrontend->size--] = '\0';
}

void xtf_cursor_on(Xtf* XtFrontend, color_t color)
{
    XANIN_DEBUG_RETURN();
    XtFrontend->Cursor.is_used = true;
    XtFrontend->Cursor.color = color;
}

void xtf_cursor_off(Xtf* XtFrontend)
{
    XtFrontend->Cursor.is_used = false;
}

void xtf_scrolling_on(Xtf* XtFrontend)
{
    XtFrontend->scrolling_enabled = true;
}

void xtf_scrolling_off(Xtf* XtFrontend)
{
    XtFrontend->scrolling_enabled = false;
}

void xtf_buffer_clear(Xtf* XtFrontend)
{
    XtFrontend->size = 0;
    XtFrontend->size_allocated = XANIN_PMMNGR_BLOCK_SIZE;
    XtFrontend->vwidth = VGA_WIDTH; // 80
    XtFrontend->vheight = 1000; //useless
    XtFrontend->buffer = (terminal_cell*)realloc(XtFrontend->buffer, XtFrontend->size_allocated);
    XtFrontend->current_height = VGA_HEIGHT; 
    XtFrontend->Cursor.position = -1;
    XtFrontend->x_screen = XtFrontend->y_screen = 0;

    XtFrontend->x = XtFrontend->y = XtFrontend->y_begin = 0;
    xtb_flush(XtFrontend);
}
