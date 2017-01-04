#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "File.h"

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	struct FileInfo
	{
		std::string m_Directory;
		std::string m_FileName;
	};

	void MountDirectory(const std::string & directory);
	std::string GetPhysicalFilePath(const std::string& filename) const;
	void GetFilesInDirectory(std::vector<std::string> & file_table, const std::string& directory, bool recursion = false) const;
	void GetFilesWithExtension(std::vector<std::string>& file_table, const std::string& extension) const;
	FileInfo* GetFileEntry(const std::string& cs) const;
	std::unique_ptr<File> GetFile(const std::string & filename) const;
	std::unique_ptr<File> GetOutputFile(const std::string & filename) const;
	void LogAllFiles() const;

private:
	std::vector<FileInfo> m_Files;
};
