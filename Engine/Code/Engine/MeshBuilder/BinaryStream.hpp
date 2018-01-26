#pragma once

typedef unsigned char byte_t;

class BinaryStream
{
public:
    // read/write a single byte.  Returns success.
    virtual bool read_byte(byte_t *out) = 0;      // = 0 -> Means this is a pure virtual
    virtual bool write_byte(byte_t const &value) = 0;

    // read/write 'count' bytes.  Returns number of bytes actually read. 
    // will return 0 on failure. 
    virtual unsigned int read_bytes(void *out_buffer, unsigned int const count) = 0;
    virtual unsigned int write_bytes(void const* buffer, unsigned int const size) = 0;
};