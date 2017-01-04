#include "WindowsFile.h"
#include <handleapi.h>
#include <fileapi.h>

WindowsFile::WindowsFile(const std::string& filepath) : File(),
	m_Path(filepath),
	m_Handle(INVALID_HANDLE_VALUE)
{
}


WindowsFile::~WindowsFile()
{
}

bool WindowsFile::Open(FileMode mode)
{
	DWORD access;
	DWORD creation_dispostion;

	switch (mode)
	{
	case FileMode::ReadMode:
		access = GENERIC_READ;
		creation_dispostion = OPEN_ALWAYS;
		break;
	case FileMode::WriteMode:
		access = GENERIC_WRITE;
		creation_dispostion = CREATE_ALWAYS;
		break;
	case FileMode::ReadWrite:
		access = GENERIC_READ | GENERIC_WRITE;
		creation_dispostion = OPEN_ALWAYS;
		break;

	default:
		throw "Unknown FileMode";
	}
	m_Handle = CreateFileA(
		m_Path.c_str(),
		access,
		0,
		nullptr,
		creation_dispostion,
		FILE_ATTRIBUTE_NORMAL,
		nullptr);

	int error = GetLastError();

	return m_Handle != INVALID_HANDLE_VALUE;
}

size_t WindowsFile::Read(uint8_t* buffer, size_t size)
{
	DWORD bytesRead = 0;
	ReadFile(m_Handle, buffer, size, &bytesRead, nullptr);
	return static_cast<size_t>(bytesRead);
}

size_t WindowsFile::Write(const uint8_t* buffer, size_t size)
{
	DWORD bytesWritten = 0;
	WriteFile(m_Handle, buffer, size, &bytesWritten, nullptr);
	return static_cast<size_t>(bytesWritten);
}

void WindowsFile::Close()
{
	if (m_Handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_Handle);
		m_Handle = INVALID_HANDLE_VALUE;
	}
}
