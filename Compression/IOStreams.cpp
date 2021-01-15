//
// Created by quinn on 1/6/2021.
//

#include "IOStreams.h"

int IOStreams::InputStream::read(char *buf, int start, int end) {
    if (start < 0 || start == end) {
        return 0;
    }
    int bytesRead = 0;
    for (int count = start; count < end; count++) {
        const int b = get();
        if (b == -1) {
            return bytesRead != 0 ? bytesRead : -1;
        }
        buf[bytesRead++] = (char) b;
    }
    return bytesRead;
}

void IOStreams::OutputStream::write(const char *buf, int start, int end) {
    if (start < 0 || start == end) {
        return;
    }
    for (int count = start; count < end; count++) {
        put(buf[count]);
    }
}
