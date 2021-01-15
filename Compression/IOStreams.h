//
// Created by quinn on 1/6/2021.
//

#ifndef COMPRESSION_IOSTREAMS_H
#define COMPRESSION_IOSTREAMS_H

namespace IOStreams {
class InputStream {
public:
    virtual int get() = 0;

    int read(char* buf, int start, int end);

    inline int read(char* buf, int end) {
        return read(buf, 0, end);
    }

    inline virtual void close() {}
};

class OutputStream {
public:
    virtual void put(char c) = 0;

    void write(const char* buf, int start, int end);

    inline void write(const char* buf, int end) {
        write(buf, 0, end);
    }

    inline virtual void close() {}

    inline virtual void flush() {}
};
}

#endif //COMPRESSION_IOSTREAMS_H
