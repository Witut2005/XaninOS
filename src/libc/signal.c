
#include <stdbool.h>

bool* app_exit_variable;

void app_exit_variable_add(bool* new_api_exit_variable)
{
    app_exit_variable = new_api_exit_variable;
}

void terminate_app()
{
    *app_exit_variable = true;
}