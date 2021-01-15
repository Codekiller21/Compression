//
// Created by quinn on 1/6/2021.
//

#ifndef COMPRESSION_FILESTREAMS_H
#define COMPRESSION_FILESTREAMS_H

#include <fstream>
#include <string>

#include "IOStreams.h"

namespace FileStreams {
class BufferedFileReader : public IOStreams::InputStream {
    FILE* fp;
    char* buf;
public:
    explicit BufferedFileReader(const char* filename);

    ~BufferedFileReader();

    int get() override;

    void close() override;

    bool operator!();
};

class BufferedFileWriter : public IOStreams::OutputStream {
    FILE* fp;
    char* buf;
public:
    explicit BufferedFileWriter(const char* filename);

    ~BufferedFileWriter();

    void put(char b) override;

    void close() override;

    void flush() override;

    bool operator!();
};
}

#endif //COMPRESSION_FILESTREAMS_H
