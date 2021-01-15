//
// Created by quinn on 1/6/2021.
//

#include "FileStreams.h"

#define DEFAULT_READER_BUFFER_SIZE 4096
#define DEFAULT_WRITER_BUFFER_SIZE 32768

FileStreams::BufferedFileReader::BufferedFileReader(const char* filename) {
    if (fopen_s(&fp, filename, "rb") != 0) {
        fp = nullptr;
        return;
    }
    buf = new char[DEFAULT_READER_BUFFER_SIZE];
    if (setvbuf(fp, buf, _IOFBF, DEFAULT_READER_BUFFER_SIZE) != 0) {
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
    return fp == nullptr;
}

FileStreams::BufferedFileWriter::BufferedFileWriter(const char* filename) {
    if (fopen_s(&fp, filename, "wb") != 0) {
        fp = nullptr;
        return;
    }
    buf = new char[DEFAULT_WRITER_BUFFER_SIZE];
    if (setvbuf(fp, buf, _IOFBF, DEFAULT_WRITER_BUFFER_SIZE) != 0) {
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
    return fp == nullptr;
}
