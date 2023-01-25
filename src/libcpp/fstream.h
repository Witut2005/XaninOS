

#include <xin_fs/xin.h>

namespace std
{

    struct FileStreamBitFlags 
    {
        uint8_t failbit:1;
        uint8_t eofbit:1;
    };

    class fstream
    {

        private:
            XinEntry* file; 
            uint32_t input_position_sequence;
            uint32_t output_position_sequence;
            FileStreamBitFlags FstreamFlags;


        public:
            fstream(char* file_name = nullptr, char* flags = nullptr)
            {
                if(file_name != nullptr && flags != nullptr) 
                    this->file = xin::fopen(file_name, flags);
            }

            void open(char* file_path, char* flags)
            {
                this->file = xin::fopen(file_path, flags);
            }

            bool good() 
            {
                this->FstreamFlags.failbit = this->file == nullptr;
            }

            void close() 
            {
                xin::fclose(&this->file);
            }

            size_t read(void* buf, size_t count) 
            {
                count = xin::fread(this->file, buf, count);
                if(!count)
                    this->FstreamFlags.failbit = true;
            }

            size_t write(void* buf, size_t count) 
            {
                count = xin::fwrite(this->file, buf, count);
                if(!count)
                    this->FstreamFlags.failbit = true;
            }

            size_t write(const void* buf, size_t count) 
            {
                count = xin::fwrite(this->file, (void*)buf, count);
                if(!count)
                    this->FstreamFlags.failbit = true;
            }


            fstream& operator >> (char* buf) 
            {
                uint32_t count = xin::fread(this->file,buf, 1);
                if(!count)
                    this->FstreamFlags.failbit = true;
            }

            fstream& operator << (char* buf) 
            {
                uint32_t count = xin::fwrite(this->file,buf, 1);
                if(!count)
                    this->FstreamFlags.failbit = true;
            }

            char get(void) 
            {
                char buf[1];
                uint32_t count = xin::fread(this->file, buf, 1);

                if(!count)
                    this->FstreamFlags.failbit = true;

                return buf[0];
            }

            void put(char x) 
            {
                uint32_t count = xin::fwrite(this->file, &x, 1);
                if(!count)
                    this->FstreamFlags.failbit = true;
            }

            FileStreamBitFlags rdstate() const
            {
                return this->FstreamFlags;
            }



    };


}

