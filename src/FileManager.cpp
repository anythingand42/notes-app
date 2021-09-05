#include <stdio.h>
#include <Fl/Fl_Native_File_Chooser.H>

#include "FileManager.hpp"

FileManager::FileManager() {}

uchar* FileManager::OpenNote()
{
    Fl_Native_File_Chooser fnfc;
    long len;
    uchar* buf;
    fnfc.title("Open file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if ( fnfc.show() ) return NULL;
    FILE* f = fopen(fnfc.filename(), "rb");
    if (!f) {
        perror(fnfc.filename());
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    buf = new uchar[len];
    if (buf) {
        fread(buf, 1, len, f);
    }
    fclose(f);
    return buf;
}

void FileManager::SaveNote(uchar *rgb_data, int size)
{
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Save File As?");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    if (fnfc.show()) return;
    FILE* f = fopen(fnfc.filename(), "wb");
    if (!f) {
        perror(fnfc.filename());
        return;
    }
    fwrite(rgb_data, size, 1, f);
    fclose(f);
}