static char program_parameters[50];
static char program_name[50];


char* get_program_parameter(char* where_to_store)
{
    
    int i;

    for(i=0; program_parameters[i] != '\0'; i++)
        where_to_store[i] = program_parameters[i];
    
    where_to_store[i] = '\0';

    return program_parameters;
}