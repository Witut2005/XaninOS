

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

            size_t read(void* buf, size_t count)
            {
                xin::read(file, buf, count);
            }

            size_t write(void* buf, size_t count)
            {
                xin::write(file, buf, count);
            }

            fstream& operator >> (char* buf)
            {
                xin::read(file,buf, 1);
            }

            fstream& operator << (char* buf)
            {
                xin::write(file,buf, 1);
            }



    };


}

