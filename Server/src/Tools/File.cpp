#include "File.h"

using namespace std;

File::File(string filename)
{
    setName(filename);
}

std::string File::getName()
{
    return fileNameNoExtension + extension;
}

void File::setPath(std::string filePath)
{
    int pos0 = filePath.find_last_of('/');
    string filename;
    if (pos0 == -1)
    {
        directory = "";
        filename = filePath;
    }
    else
    {
        directory = filePath.substr(0, pos0 + 1);
        filename = filePath.substr(pos0 + 1);
    }

    int pos = filename.find_last_of('.');
    if (pos == -1)
    {
        fileNameNoExtension = filename;
        extension = "";
    }
    else
    {
        fileNameNoExtension = filename.substr(0, pos);
        extension = filename.substr(pos);
    }
}

void File::setName(std::string name)
{
    int pos = name.find_last_of('.');
    if (pos == -1)
    {
        fileNameNoExtension = name;
        extension = "";
    }
    else
    {
        fileNameNoExtension = name.substr(0, pos);
        extension = name.substr(pos);
    }
}

string File::getPath()
{
    return directory + fileNameNoExtension + extension;
}