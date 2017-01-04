#include "FileSystem.h"
#include <cassert>
#include <iostream>
#include "File.h"

int main()
{
	FileSystem system;

	system.MountDirectory("/home/dieter/Desktop/TestDirectory/");

	auto fullpath = system.GetPhysicalFilePath("file.txt");

	std::cout << fullpath << std::endl;

	std::vector<std::string> file_table;

	//Should return all files in C:\\TestDirectory\\Some\\Directory
	system.GetFilesInDirectory(file_table, "TestDirectory", true);

	for (auto & file : file_table)
	{
		std::cout << file << std::endl;
	}

	file_table.resize(0);
	std::cout << std::endl;
	//Should return all .txt files in C:\\TestDirectory and C:\\AnotherTestDirectory
	system.GetFilesWithExtension(file_table, ".txt");

	for (auto & path : file_table)
	{
		std::cout << path << std::endl;
	}


	std::unique_ptr<File> input_file = system.GetFile("input.txt");
	std::unique_ptr<File> output_file = system.GetOutputFile("output.txt");

	input_file->Open(File::FileMode::ReadMode);
	output_file->Open(File::FileMode::WriteMode);

	size_t read_byte_count, written_byte_count;
	std::vector<uint8_t> buffer;

	constexpr int buffer_size = 1024;

	buffer.resize(buffer_size);

	do
	{
		read_byte_count = input_file->Read(buffer.data(), buffer.size());
		written_byte_count = output_file->Write(buffer.data(), read_byte_count);

		assert(written_byte_count == read_byte_count);

	} while (read_byte_count > 0);

	input_file->Close();
	output_file->Close();

	return 0;
}
