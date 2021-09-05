#ifndef FILEMANAGER
#define FILEMANAGER

#include <FL/fl_types.h>

class FileManager 
{
public:
    FileManager();
    uchar* OpenNote();
    void SaveNote(uchar* rgb_data, int size);
};

#endif
