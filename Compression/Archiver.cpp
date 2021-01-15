//
// Created by quinn on 1/7/2021.
//

#include "Archiver.h"

using namespace std;
using namespace std::filesystem;

using Archiver::HeaderEncoder;
using Archiver::FileHeader;
using Archiver::FileTypes;
using Archiver::ArchiverException;

using FileStreams::BufferedFileReader;
using FileStreams::BufferedFileWriter;

#define UNEXPECTED_EOF_ERROR "Unexpected end of file"

Archiver::Archive::Archive(IOStreams::OutputStream* output, const std::string& location, bool verbose) {
    this->output = output;
    this->location = location;
    this->verbose = verbose;
}

void Archiver::Archive::archive() {
    path root(location);
    HeaderEncoder he = {};

    if (is_directory(root)) {
        if (verbose) {
            cout << "Dir: " << root.filename().string() << endl;
        }
        dirRecur(root);
    }
    else if (is_regular_file(root)) {
        if (verbose) {
            cout << "File: " << root.filename().string() << endl;
        }
        he.archive = {
                .fileType = File,
                .fileSize = file_size(root)
        };
        strcpy_s(he.archive.fileName, sizeof(he.archive.fileName), root.filename().string().c_str());
        output->write(he.bytes, sizeof(he));

        readFile(root.string());
    }
    else if (verbose) {
        cout << "Not a supported file type: " << root.filename().string() << endl;
    }
}

void Archiver::Archive::dirRecur(const path& root) {
    HeaderEncoder he = {};

    for (const path& p : directory_iterator(root)) {
        if (is_regular_file(p)) {
            if (verbose) {
                cout << "File: " << p.filename().string() << endl;
            }
            he.archive = {
                    .fileType = File,
                    .fileSize = file_size(p)
            };
            strcpy_s(he.archive.fileName, sizeof(he.archive.fileName), p.filename().string().c_str());
            output->write(he.bytes, sizeof(he));

            readFile(p.string());
        }
        else if (is_directory(p)) {
            if (verbose) {
                cout << "Dir: " << p.filename().string() << endl;
            }
            he.archive = {
                    .fileType = StartDir
            };
            strcpy_s(he.archive.fileName, sizeof(he.archive.fileName), p.filename().string().c_str());
            output->write(he.bytes, sizeof(he));

            dirRecur(p);

            he.archive = {
                    .fileType = EndDir
            };
            output->write(he.bytes, sizeof(he));
        }
        else if (verbose) {
            cout << "Not a supported file type: " << p.filename().string() << endl;
        }
    }
}

void Archiver::Archive::readFile(const string& filename) {
    BufferedFileReader reader(filename.c_str());
    if (!reader) {
        throw Archiver::ArchiverException(("Error opening file: " + filename).c_str());
    }
    int offset;
    while ((offset = reader.read(buf, BUF_SIZE)) != -1) {
        output->write(buf, offset);
    }
}

Archiver::UnArchive::UnArchive(IOStreams::InputStream* input, const string& location, bool verbose) {
    this->input = input;
    this->location = location;
    this->verbose = verbose;
}

void Archiver::UnArchive::unArchive() {
    HeaderEncoder he = {};

    // Create starting point
    if (exists(location)) {
        remove(location);
    }
    create_directory(location);

    do {
        // Read header
        int offset = input->read(headerBuf, sizeof(headerBuf));
        if (offset == -1) {
            return;
        }
        if (offset != sizeof(headerBuf)) {
            throw ArchiverException(UNEXPECTED_EOF_ERROR);
        }
        memcpy(he.bytes, headerBuf, sizeof(HeaderEncoder));

        if (he.archive.fileType == File) {
            // Open file
            BufferedFileWriter output((location / he.archive.fileName).string().c_str());
            if (!output) {
                throw ArchiverException(("Couldn't create file: " + (location / he.archive.fileName).string()).c_str());
            }
            if (verbose) {
                cout << "File: " << location / he.archive.fileName << endl;
            }
            // Read offset bytes
            uintmax_t sizeOfFile = he.archive.fileSize;
            if (input->read(buf, (int) (sizeOfFile % 4096)) != sizeOfFile % 4096) {
                throw ArchiverException(UNEXPECTED_EOF_ERROR);
            }
            output.write(buf, (int) (sizeOfFile % 4096));
            // Read rest of file
            sizeOfFile -= sizeOfFile % 4096;
            while (sizeOfFile != 0) {
                offset = input->read(buf, 4096);
                if (offset != 4096) {
                    throw ArchiverException(UNEXPECTED_EOF_ERROR);
                }
                output.write(buf, 4096);
                sizeOfFile -= 4096;
            }
        }
        else if (he.archive.fileType == StartDir) {
            location /= he.archive.fileName;
            create_directory(location);
            if (verbose) {
                cout << "Dir: " << location.string() << endl;
            }
        }
        else if (he.archive.fileType == EndDir) {
            location = location.parent_path();
        }
        else {
            throw ArchiverException("Bad header!");
        }
    } while (true);
}

Archiver::ArchiverException::ArchiverException(const char* message) {
    this->message = message;
}

const char* Archiver::ArchiverException::what() const noexcept {
    return message.c_str();
}
