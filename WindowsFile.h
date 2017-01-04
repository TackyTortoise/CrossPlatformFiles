#pragma once
#include "File.h"
#include <string>
#include <windows.h>

class WindowsFile : public File
{
public:
	WindowsFile(const std::string& filepath);
	virtual ~WindowsFile();

	virtual bool Open(FileMode mode) override;
	virtual size_t Read(uint8_t* buffer, size_t size) override;
	virtual size_t Write(const uint8_t* buffer, size_t size) override;
	virtual void Close() override;
private:
	std::string m_Path;
	HANDLE m_Handle;
};

