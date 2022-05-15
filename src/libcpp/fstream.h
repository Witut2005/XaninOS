

#include <libcpp/cxin.h>

namespace std
{


    class fstream
    {

        private:
            xin_entry* file; 


        public:
            explicit fstream(char* file_name = nullptr, const char* flags = nullptr)
            {
                if(file_name != nullptr) 
                    file = fopen(file_name, flags);
            }

            void open(char* file_path, const char* flags)
            {
                file = fopen(file_path, flags);
            }


            void close()
            {
                fclose(&file);
            }


    };


}
