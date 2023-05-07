
//https://cplusplus.com/reference/regex/ECMAScript/

namespace std
{
bool regex_match(char* str, char* pattern)
{
    while(*pattern != '\0')
    {
        
        switch(*pattern)
        {

            case '\\':
            {
                switch(*(++pattern))
                {
                    case 's':
                    {
                        if(*str != ' ')
                            return false;
                        break;
                    }

                    case 'S':
                    {
                        if(*str == ' ')
                            return false;
                        break;
                    }

                    case 'd':
                    {
                        if(!(*str >= '0' && *str <= '9'))
                            return false;
                        break;
                    }

                    case 'D':
                    {
                        if(*str >= '0' && *str <= '9')
                            return false;
                        break;
                    }

                    case '\0':
                    {
                        if(*str != '\0')
                            return false;
                        break;
                    }

                    case '\t':
                    {
                        if(*str != '\t')
                            return false;
                        break;
                    }

                    case '\n':
                    {
                        if(*str != '\n')
                            return false;
                        break;
                    }

                    case '\v':
                    {
                        if(*str != '\v')
                            return false;
                        break;
                    }

                    case '\f':
                    {
                        if(*str != '\f')
                            return false;
                        break;
                    }

                    case '\r':
                    {
                        if(*str != '\r')
                            return false;
                        break;
                    }

                    case 'w':
                    {
                        if(!(*str >= '!' && *str <= (char)126))
                            return false;
                        break;
                    }

                    case 'W':
                    {
                        if(*str >= '!' && *str <= (char)126)
                            return false;
                        break;
                    }
                
                }
                break;
            }

            case '.':
            {
                if(*str == '\n')
                    return false;
                break;
            }

            default:
            {
                if(*str != *pattern)
                    return false;
                break;
            }

        }

        str++;
        pattern++;
    }
    return true;
}
}