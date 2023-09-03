
#include <sys/terminal/handlers/handlers.h>

void xtf_overflow_x_handler(Xtf* XtFrontend)
{
    if (XtFrontend->x >= XtFrontend->vwidth)
        XtFrontend->buffer[XtFrontend->size++].cell = SAFE_NEW_LINE | AS_COLOR(black);
}