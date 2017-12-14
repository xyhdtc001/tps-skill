#pragma once

#include <string>
#include <vector>

class CopyDir  
{  
public:  
	CopyDir();  

	void copy(const std::string& srcDirPath, const std::string& desDirPath);  

	void copyFileToDir(const std::string& srcDirPath, const std::string& desDirPath);
public:  

private:  
	bool make_dir (const std::string& pathName);  
	//    bool mkdir (char const* pathname/*, mode_t mode*/);  
	bool get_src_files_name(std::vector<std::string>& fileNameList);  
	void do_copy(const std::vector<std::string>& fileNameList);  

private:  
	std::string srcDirPath, desDirPath;  


};  