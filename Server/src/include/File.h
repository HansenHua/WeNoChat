/**
 * 封装了文件的常用方法
 */

#ifndef __FILE_H__
#define __FILE_H__

#include "Constants.h"
#include <string>
#include <cstdlib>

class File
{
public:
    File(std::string filePath);
    void setName(std::string name);
    std::string getName();
    void setPath(std::string path);
    std::string getPath();
    std::string fileNameNoExtension;
    std::string extension;
    std::string directory;
};

struct WriteFileTask
{
    std::string filename;
    fd_t fileFd;
    fd_t targetFd;
    size_t fileSize;
    size_t progress;
    WriteFileTask(fd_t targetFd = -1) : targetFd(targetFd) {}
};

#endif