#include "stdafx.h"
#include "CCopyOprate.h"
#include <iostream>  
#include <fstream>  
#include <cstring>  

#if defined(_WIN32)  
#   include <direct.h>  
#   include <io.h>  
#   include <shlobj.h>  
#   include <sys/stat.h>  
#   include <sys/types.h>  
#else // Linux  
#   include <dirent.h>  
#   include <unistd.h>  
#   include <sys/stat.h>  
#   include <sys/types.h>  
#   include <pwd.h>  
#endif

CopyDir::CopyDir()  
{  

}  

void  CopyDir::copy(const std::string& srcDirPath, const std::string& desDirPath)  
{  
	this->srcDirPath = srcDirPath;  
	std::string srcDir;  
#ifdef _WIN32  
	int n = 0;  
	while (srcDirPath.find('\\', n) != std::string::npos)  
	{  
		n = srcDirPath.find('\\', n) + 1;  
	}  
	if(n == 0)  
	{  
		std::cout << "src path error" << std::endl;  
		return;  
	}  
	srcDir = srcDirPath.substr(n-1, srcDirPath.size());  

#else  // Linux  
	int n = 0;  
	while (srcDirPath.find('/', n) != std::string::npos)  
	{  
		n = srcDirPath.find('/', n) + 1;  
	}  
	if(n == 0)  
	{  
		std::cout << "src path error" << std::endl;  
		return;  
	}  
	srcDir = srcDirPath.substr(n-1, srcDirPath.size());  

#endif  
	this->desDirPath = desDirPath + srcDir;  

	if(!make_dir(this->desDirPath))  
	{  
		//return;  
	}  

	std::vector<std::string> fileNameList;  
	if(!get_src_files_name(fileNameList))  
	{  
		return;  
	}  

	if(fileNameList.empty())  
	{  
		std::cout << "src dir is empty" << std::endl;  
		return;  
	}  

	do_copy(fileNameList);  
}  

void CopyDir::copyFileToDir(const std::string& srcDirPath, const std::string& desDirPath)
{
	std::string nowSrcFilePath, nowDesFilePath ;  
#ifdef _WIN32  
	nowSrcFilePath = srcDirPath;  
	nowDesFilePath = desDirPath;  

#else    
	return;
#endif  
	std::ifstream in(nowSrcFilePath.c_str());  
	//in.open(nowSrcFilePath, std::ifstream::in);  
	if(!in)  
	{  
		std::cout << "open src file : " << nowSrcFilePath << " failed" << std::endl;  
		return;  
	}  

	std::ofstream out(nowDesFilePath.c_str());  
	//out.open(nowDesFilePath,std::ifstream::out);  
	if(!out)  
	{  
		std::cout << "create new file : " << nowDesFilePath << " failed" << std::endl;  
		in.close();  
		return;  
	}  

	out << in.rdbuf();  

	out.close();  
	in.close();
}

bool  
CopyDir::make_dir (const std::string& pathName)  
{  
#ifdef _WIN32  
	if(::_mkdir(pathName.c_str()) < 0)  
	{  
		std::cout << "create path error" << std::endl;  
		return false;  
	}  
#else  // Linux  
	if(::mkdir(pathName.c_str(), S_IRWXU | S_IRGRP | S_IXGRP) < 0)  
	{  
		std::cout << "create path error" << std::endl;  
		return false;  
	}  
#endif  

	return true;  
}  

bool  CopyDir::get_src_files_name(std::vector<std::string>& fileNameList)  
{  
#ifdef _WIN32  
	_finddata_t file;  
	long lf;  
	std::string src = this->srcDirPath + "\\*.*";  
	if ((lf = _findfirst(src.c_str(), &file)) == -1)   
	{  
		std::cout << this->srcDirPath << " not found" << std::endl;  
		return false;  
	}  
	else{  
		while (_findnext(lf, &file) == 0)  
		{  
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)  
				continue;  
			fileNameList.push_back(file.name);  
		}  
	}  


	_findclose(lf);  
#else  // Linux  
	DIR *dir;  
	struct dirent *ptr;  

	if ((dir=opendir(this->srcDirPath.c_str())) == NULL)  
	{  
		std::cout << this->srcDirPath << " not found" << std::endl;  
		return false;  
	}  

	while ((ptr=readdir(dir)) != NULL)  
	{  
		if((ptr->d_name == ".") || (ptr->d_name == ".."))  //current / parent  
			continue;  
		else if(ptr->d_type == 8)  //file  
			fileNameList.push_back(ptr->d_name);  
		else if(ptr->d_type == 10)  //link file  
			continue;  
		else if(ptr->d_type == 4)  //dir  
			fileNameList.push_back(ptr->d_name);  
	}  
	closedir(dir);  

#endif  

	return true;  

}  

void  CopyDir::do_copy(const std::vector<std::string> &fileNameList)  
{  
#pragma omp parallel for  
	for (int i = 0; i < fileNameList.size(); i++)  
	{  
		std::string nowSrcFilePath, nowDesFilePath ;  
#ifdef _WIN32  
		nowSrcFilePath = this->srcDirPath + "\\" + fileNameList.at(i);  
		nowDesFilePath = this->desDirPath + "\\" + fileNameList.at(i);  

#else  
		nowSrcFilePath = this->srcDirPath + "/" + fileNameList.at(i);  
		nowDesFilePath = this->desDirPath + "/" + fileNameList.at(i);  

#endif  
		std::ifstream in(nowSrcFilePath.c_str());  
		//in.open(nowSrcFilePath, std::ifstream::in);  
		if(!in)  
		{  
			std::cout << "open src file : " << nowSrcFilePath << " failed" << std::endl;  
			continue;  
		}  

		std::ofstream out(nowDesFilePath.c_str());  
		//out.open(nowDesFilePath,std::ifstream::out);  
		if(!out)  
		{  
			std::cout << "create new file : " << nowDesFilePath << " failed" << std::endl;  
			in.close();  
			continue;  
		}  

		out << in.rdbuf();  

		out.close();  
		in.close();  
	}  
}  