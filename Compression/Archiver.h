//
// Created by quinn on 1/7/2021.
//

#ifndef COMPRESSION_ARCHIVER_H
#define COMPRESSION_ARCHIVER_H

#include <filesystem>
#include <string>
#include <exception>
#include <cstring>
#include <cstdint>
#include <iostream>

#include "IOStreams.h"
#include "FileStreams.h"

namespace Archiver {
enum FileTypes {
    File,
    StartDir,
    EndDir
};

struct FileHeader {
    char fileName[261];
    FileTypes fileType;
    uintmax_t fileSize;
};

union HeaderEncoder {
    FileHeader archive;
    char bytes[sizeof(archive)];
};

class ArchiverException : public std::exception {
    std::string message;
public:
    explicit ArchiverException(const char* message);

    [[nodiscard]] const char* what() const noexcept override;
};

class Archive {
    IOStreams::OutputStream* output;
    std::string location;
    static const int BUF_SIZE = 4096;
    char buf[BUF_SIZE]{};
    bool verbose;

    void dirRecur(const std::filesystem::path& root);
    void readFile(const std::string& filename);
public:
    explicit Archive(IOStreams::OutputStream* output, const std::string& location, bool verbose);

    void archive();
};

class UnArchive {
    IOStreams::InputStream* input;
    std::filesystem::path location;
    static const int BUF_SIZE = 4096;
    char buf[BUF_SIZE]{};
    char headerBuf[sizeof(HeaderEncoder)]{};
    bool verbose;
public:
    explicit UnArchive(IOStreams::InputStream* input, const std::string& location, bool verbose);

    void unArchive();
};
}

#endif //COMPRESSION_ARCHIVER_H
