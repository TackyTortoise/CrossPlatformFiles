#pragma once
#include "File.h"
#include <string>
#include <stdio.h>

class LinuxFile : public File
{
public:
  LinuxFile(const std::string& filePath);
  ~LinuxFile();

  virtual bool Open(FileMode mode) override;
	virtual size_t Read(uint8_t* buffer, size_t size) override;
	virtual size_t Write(const uint8_t* buffer, size_t size) override;
	virtual void Close() override;

private:
  std::string m_Path;
  int m_Handle;
};
