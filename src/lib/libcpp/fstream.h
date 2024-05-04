

#include <lib/libc/file.h>

namespace std
{

struct FileStreamBitFlags
{
    uint8_t failbit : 1;
    uint8_t eofbit : 1;
};

class fstream
{

private:
    XinEntry* file;
    uint32_t input_position_sequence;
    uint32_t output_position_sequence;
    FileStreamBitFlags FstreamFlags;

public:
    fstream(const fstream&) = default;

    fstream(char* file_name = NULL, char* flags = NULL)
    {
        if (file_name != NULL && flags != NULL)
            this->file = fopen(file_name, flags);
    }

    void open(char* file_path, char* flags)
    {
        this->file = fopen(file_path, flags);
    }

    bool good()
    {
        this->FstreamFlags.failbit = (this->file == NULL);
        return this->FstreamFlags.failbit;
    }

    void close()
    {
        fclose(&this->file);
    }

    size_t read(void* buf, size_t count)
    {
        count = fread(this->file, buf, count);
        if (!count)
            this->FstreamFlags.failbit = true;
        return count;
    }

    size_t write(void* buf, size_t count)
    {
        count = fwrite(this->file, buf, count);
        if (!count)
            this->FstreamFlags.failbit = true;
        return count;
    }

    size_t write(const void* buf, size_t count)
    {
        count = fwrite(this->file, (void*)buf, count);
        if (!count)
            this->FstreamFlags.failbit = true;
        return count;
    }

    fstream& operator>>(char* buf)
    {
        uint32_t count = fread(this->file, buf, 1);
        if (!count)
            this->FstreamFlags.failbit = true;
        return (*this);
    }

    fstream& operator<<(char* buf)
    {
        uint32_t count = fwrite(this->file, buf, 1);
        if (!count)
            this->FstreamFlags.failbit = true;
        return (*this);
    }

    char get(void)
    {
        char buf[1];
        uint32_t count = fread(this->file, buf, 1);

        if (!count)
            this->FstreamFlags.failbit = true;

        return buf[0];
    }

    void put(char x)
    {
        uint32_t count = fwrite(this->file, &x, 1);
        if (!count)
            this->FstreamFlags.failbit = true;
    }

    FileStreamBitFlags rdstate() const
    {
        return this->FstreamFlags;
    }
};

}
