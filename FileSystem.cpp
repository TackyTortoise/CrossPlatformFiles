#define WIN32_MEAN_AND_LEAN
#include "FileSystem.h"
#include <iostream>
#include <algorithm>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include "WindowsFile.h"
#endif

#ifdef __linux__
#include "LinuxFile.h"
#include <dirent.h>
#include <unistd.h>
#include <cstring>

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
#endif



FileSystem::FileSystem()
{
}


FileSystem::~FileSystem()
{
}

void FileSystem::MountDirectory(const std::string& directory)
{
	#ifdef _WIN32
	WIN32_FIND_DATAA fileData;
	HANDLE handle = FindFirstFileA((directory + "*").c_str(), &fileData);

	if (handle == INVALID_HANDLE_VALUE)
	{
		std::cerr << "Unable to open directory" << directory << std::endl;
	}

	do
	{
		if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (std::strcmp(fileData.cFileName, ".") == 0 || std::strcmp(fileData.cFileName, "..") == 0)
				continue;
			MountDirectory(directory + fileData.cFileName + "\\");
		}
		else
		{
			m_Files.push_back({ directory, fileData.cFileName });
		}
	} while (FindNextFileA(handle, &fileData));

	if (GetLastError() != ERROR_NO_MORE_FILES)
	{
		std::cerr << "Mount directory failed for unexpected reason" << std::endl;
	}

	FindClose(handle);
	#endif

	#ifdef __linux__
		if (directory.back() == '.' || directory[directory.size() - 2] == '.')
		return;

		DIR* dp;
		struct dirent *dirp;

		if ((dp = opendir(directory.c_str())) == NULL)
		{
			std::cout << "Unable to open directory " << directory << std::endl;
			return;
		}

		while ((dirp = readdir(dp)) != NULL)
		{
			if (dirp->d_type == DT_DIR)
				MountDirectory(directory + dirp->d_name + "/");
			else
				m_Files.push_back({ directory, dirp->d_name });
		}

		closedir(dp);
	#endif
}

std::string FileSystem::GetPhysicalFilePath(const std::string& filename) const
{
	for (auto f : m_Files)
	{
		if (f.m_FileName.compare(filename) == 0)
			return f.m_Directory + f.m_FileName;
	}

	return "FILE NOT FOUND";
}

void FileSystem::GetFilesInDirectory(std::vector<std::string>& file_table, const std::string& directory, bool recursion) const
{
	file_table.clear();
	for (auto f : m_Files)
	{
		if (f.m_Directory.compare(directory) != 0)
		{
			file_table.push_back(f.m_FileName);
		}
	}
}

void FileSystem::GetFilesWithExtension(std::vector<std::string>& file_table, const std::string& extension) const
{
	file_table.clear();
	for (auto f : m_Files)
	{
		int dotPos = f.m_FileName.rfind(extension);
		if (dotPos == std::string::npos)
			continue;
		auto ext = f.m_FileName.substr(dotPos);
		if (ext.compare(extension) == 0)
			file_table.push_back(f.m_Directory + f.m_FileName);
	}
}

FileSystem::FileInfo* FileSystem::GetFileEntry(const std::string& cs) const
{
	auto it = std::find_if(m_Files.begin(), m_Files.end(), [cs](const FileInfo & entry)
	{
		return cs == entry.m_FileName;
	});

	return nullptr;
}

std::unique_ptr<File> FileSystem::GetFile(const std::string& filename) const
{
	auto physical_file_path = GetPhysicalFilePath(filename);

	if (!physical_file_path.empty())
	{
		#ifdef _WIN32
			return std::make_unique<WindowsFile>(physical_file_path);
		#endif
		#ifdef __linux__
			return std::make_unique<LinuxFile>(physical_file_path);
		#endif

	}

	return nullptr;
}

std::unique_ptr<File> FileSystem::GetOutputFile(const std::string& filename) const
{
#ifdef _WIN32
	std::wstring projDir;
	TCHAR pd[2048];
	int bytes = GetModuleFileName(nullptr, pd, 2048);
	if (bytes == 0)
	{
		MessageBox(nullptr, L"Failed to get project directory", L"Error", MB_OK);
	}

	projDir = pd;
	projDir = projDir.substr(0, projDir.rfind('\\') + 1);

	projDir += std::wstring(filename.begin(), filename.end());

	return std::make_unique<WindowsFile>(std::string(projDir.begin(), projDir.end()));
#endif

#ifdef __linux__
char buf[1024] = {0};
	ssize_t size = readlink("/proc/self/exe", buf, sizeof(buf));
	if (size == 0 || size == sizeof(buf))
	{
			std::cout << "Failed to get project directory" << std::endl;
	}

	std::string projDir(buf,size);
	projDir = projDir.substr(0, projDir.rfind('\\') + 1);

	projDir += filename;
	return std::make_unique<LinuxFile>(projDir);
#endif
	return nullptr;
}

void FileSystem::LogAllFiles() const
{
	std::for_each(m_Files.begin(), m_Files.end(), [](FileInfo x) {std::cout << x.m_Directory + x.m_FileName; std::cout << std::endl; });
}
