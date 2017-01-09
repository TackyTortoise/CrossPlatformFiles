#ifdef __linux__
#include "LinuxFile.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

LinuxFile::LinuxFile(const std::string& filePath):
  m_Path(filePath),
  m_Handle(-1)
{}

LinuxFile::~LinuxFile(){}

bool LinuxFile::Open(FileMode mode)
{
  int creationFlags;
  switch(mode)
  {
    case FileMode::ReadMode:
      creationFlags = O_RDONLY;
      break;
    case FileMode::WriteMode:
      creationFlags = O_WRONLY  | O_CREAT;
      break;
    case FileMode::ReadWrite:
      creationFlags = O_RDWR  | O_CREAT;
      break;
    default:
      throw "Unknown FileMode";
  }

  m_Handle = open(m_Path.c_str(), creationFlags, 0644);

  if (m_Handle == -1)
    return false;

  return true;
}

size_t LinuxFile::Read(uint8_t* buffer, size_t size)
{
  return read(m_Handle, buffer, size);
}

size_t LinuxFile::Write(const uint8_t* buffer, size_t size)
{
  return write(m_Handle, buffer, size);
}

void LinuxFile::Close()
{
  close(m_Handle);
}
#endif