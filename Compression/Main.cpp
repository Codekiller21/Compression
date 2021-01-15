#include <iostream>

#include <Windows.h>

#include "FileStreams.h"
#include "Archiver.h"

using namespace FileStreams;
using namespace std;

int main() {
    BufferedFileWriter output("testing\\Output.cmp");
    if (!output) {
        cout << "Couldn't open file\n";
        return -1;
    }
    Archiver::Archive a(&output, "testing\\vscode", true);
    try {
        a.archive();
    }
    catch (const Archiver::ArchiverException& e) {
        cerr << "Error in archiving: " << e.what() << endl;
        return -1;
    }
    output.close();

    cout << "\n\nDone compressing\n\n";
    Sleep(10000);
    BufferedFileReader reader("testing\\Output.cmp");
    if (!reader) {
        cerr << "Couldn't open file\n";
        return -1;
    }
    Archiver::UnArchive un(&reader, "testing\\decomp", true);
    try {
        un.unArchive();
    }
    catch (const Archiver::ArchiverException& e) {
        cerr << "Error in unarchiving: " << e.what() << endl;
        return -1;
    }
    return 0;
}
