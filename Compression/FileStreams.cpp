//
// Created by quinn on 1/6/2021.
//

#include "FileStreams.h"

#define DEFAULT_BUFFER_SIZE 4096

FileStreams::BufferedFileReader::BufferedFileReader(const char* filename) {
    fp = fopen(filename, "rb");
    if (!fp) {
        fp = nullptr;
        return;
    }
    buf = new char[DEFAULT_BUFFER_SIZE];
    if (setvbuf(fp, buf, _IOFBF, DEFAULT_BUFFER_SIZE) != 0) {
        fclose(fp);
        fp = nullptr;
        delete[] buf;
    }
}

FileStreams::BufferedFileReader::~BufferedFileReader() {
    if (fp != nullptr) {
        fclose(fp);
        fp = nullptr;
        delete[] buf;
    }
}

void FileStreams::BufferedFileReader::close() {
    if (fp != nullptr) {
        fclose(fp);
        fp = nullptr;
        delete[] buf;
    }
}

int FileStreams::BufferedFileReader::get() {
    return fgetc(fp);
}

bool FileStreams::BufferedFileReader::operator!() {
    return !fp;
}

FileStreams::BufferedFileWriter::BufferedFileWriter(const char* filename) {
    fp = fopen(filename, "wb");
    if (!fp) {
        fp = nullptr;
    }
    buf = new char[DEFAULT_BUFFER_SIZE];
    if (setvbuf(fp, buf, _IOFBF, DEFAULT_BUFFER_SIZE) != 0) {
        fclose(fp);
        fp = nullptr;
        delete[] buf;
    }
}

FileStreams::BufferedFileWriter::~BufferedFileWriter() {
    if (fp != nullptr) {
        fclose(fp);
        fp = nullptr;
        delete[] buf;
    }
}

void FileStreams::BufferedFileWriter::close() {
    if (fp != nullptr) {
        fclose(fp);
        fp = nullptr;
        delete[] buf;
    }
}

void FileStreams::BufferedFileWriter::put(char b) {
    fputc(b, fp);
}

void FileStreams::BufferedFileWriter::flush() {
    fflush(fp);
}

bool FileStreams::BufferedFileWriter::operator!() {
    return !fp;
}
