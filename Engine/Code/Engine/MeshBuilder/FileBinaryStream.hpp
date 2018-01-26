#pragma once
#include "Engine/MeshBuilder/BinaryStream.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

#pragma warning(disable:4996)

class FileBinaryStream
{
public:
    //----------------------------------------------------------------------------
    FileBinaryStream()
        : file_pointer(nullptr)
    {}

    //----------------------------------------------------------------------------
    ~FileBinaryStream()
    {
        close();
    }

    //----------------------------------------------------------------------------
    bool open_for_read(char const *filename)
    {
        ASSERT_OR_DIE(!is_open(), "Cannot open file for read!");
        file_pointer = fopen(filename, "rb"); // open for read & binary
        return is_open();
    }

    //----------------------------------------------------------------------------
    bool open_for_write(char const *filename)
    {
        ASSERT_OR_DIE(!is_open(), "Cannot open file for read!");
        file_pointer = fopen(filename, "wb"); // open for read & binary
        return is_open();
    }

    //----------------------------------------------------------------------------
    void close()
    {
        if (is_open()) {
            fclose(file_pointer);
            file_pointer = nullptr;
        }
    }

    //----------------------------------------------------------------------------
    // BinaryStream Impl
    //----------------------------------------------------------------------------
    virtual bool read_byte(byte_t *out) 
    {
        return (read_bytes(out, 1) == 1);
    }

    //----------------------------------------------------------------------------
    virtual bool write_byte(byte_t const &value) 
    {
        return (write_bytes(&value, 1) == 1);
    }

    //----------------------------------------------------------------------------
    // read/write 'count' bytes.  Returns number of bytes actually read. 
    // will return 0 on failure. 
    virtual unsigned int read_bytes(void *out_buffer, unsigned int const count)
    {
        unsigned int bytes_read = 0;
        if (is_open()) {
            bytes_read = (unsigned int)fread(out_buffer, 1, count, file_pointer);
        }

        return bytes_read;
    }

    //----------------------------------------------------------------------------
    virtual uint write_bytes(void const *data, uint size)
    {
        unsigned int bytes_read = 0;
        if (is_open()) {
            bytes_read = (unsigned int)fwrite(data, 1, size, file_pointer);
        }

        return bytes_read;
    }

    //----------------------------------------------------------------------------
    inline bool is_open() const { return nullptr != file_pointer; }

public:
    FILE *file_pointer;   // file pointer;
};