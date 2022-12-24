#include "file_utils.h"
#include "string_utils.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h> 
#include <direct.h>
#include <fstream>
#include <sstream>
#include <cctype>       // std::tolower
#include <Windows.h>
#include <algorithm>
//#include <dirent.h>

namespace FileUtils {

  bool existsFile(const std::string &filename) {
    FILE *f = fopen(filename.c_str(), "rb");
    if (f) {
      fclose(f);
      return true;
    }
    return false;
  }

  bool dirExists(const std::string& dirName_in)
  {
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
      return false;  //something is wrong with your path!

    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
      return true;   // this is a directory!

    return false;    // this is not a directory!
  }

  size_t getFileSize(const std::string &filename) {
    size_t nbytes = 0;
    FILE *f = fopen(filename.c_str(), "rb");
    if (f) {
      fseek(f, 0, SEEK_END);
      nbytes = ftell(f);
      fclose(f);
    }
    return nbytes;
  }

  __int64	getFileDate(const std::string &filename) {
    struct _stat buffer;
    int ok = _stat(filename.c_str(), &buffer);
    if (!ok) {
      unsigned int st_mtime = (unsigned int)buffer.st_mtime;
      unsigned int st_ctime = (unsigned int)buffer.st_ctime;
      unsigned int st_time = (st_mtime > st_ctime) ? st_mtime : st_ctime;
      return st_time;
    }

    return 0;
  }

  std::string getPath(const std::string& str)
  {
    std::size_t found = str.find_last_of("/\\");
    std::string path;
    if (found != std::string::npos) {
      path = str.substr(0, found);
    }
    path = path + "/";
    return path;
  }

  std::string getFilename(const std::string& str)
  {
    std::size_t found = str.find_last_of("/\\");
    std::string filename = str.substr(found + 1);
    return filename;
  }

  std::string getFilenameName(const std::string& str)
  {
    std::size_t found = str.find_last_of("/\\");
    std::string filename = str.substr(found + 1);
    std::size_t found_name = filename.find_last_of(".");
    std::string name = filename.substr(0, found_name);
    return name;
  }

  std::string	getLastDirectory(const std::string &apath) {
    std::string path = apath;
    if (path.back() == '/' || path.back() == '\\') {
      path.erase(path.end()-1);
    }
    std::string::size_type pos = path.find_last_of("\\/");
    if (pos != std::string::npos)
      return path.substr(pos+1);
    return "";
  }

  //std::string	getPathFrom(const std::string &path) {
  //  std::string::size_type pos = path.find_last_of("\\/");
  //  if (pos != std::string::npos)
  //    return path.substr(0, pos + 1);
  //  return "./";
  //}

  std::string	getFileExtension(const std::string &path) {
    std::string::size_type pos = path.find_last_of(".");
    if (pos != std::string::npos) {
      std::string str = path.substr(pos);
      std::transform(str.begin(), str.end(), str.begin(), (int(*)(int)) std::tolower);
      return str;
    }
    return path;
  }


  void createDirectoryRecursively(const std::wstring &directory) {
    static const std::wstring separators(L"\\/");

    // If the specified directory name doesn't exist, do our thing
    DWORD fileAttributes = ::GetFileAttributesW(directory.c_str());
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) {

      // Recursively do it all again for the parent directory, if any
      std::size_t slashIndex = directory.find_last_of(separators);
      if (slashIndex != std::wstring::npos) {
        createDirectoryRecursively(directory.substr(0, slashIndex));
      }

      // Create the last directory on the path (the recursive calls will have taken
      // care of the parent directories by now)
      BOOL result = ::CreateDirectoryW(directory.c_str(), nullptr);
      if (result == FALSE) {
        throw std::runtime_error("Could not create directory");
      }

    }
    else { // Specified directory name already exists as a file or directory

      bool isDirectoryOrJunction =
        ((fileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ||
        ((fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT) != 0);

      if (!isDirectoryOrJunction) {
        throw std::runtime_error(
          "Could not create directory because a file with the same name exists"
          );
      }

    }
  }

  void getFilesFromDirectory(const std::string &pattern, VStrings &list_files, bool use_fullname) {
    WIN32_FIND_DATA find_data;
    std::string path = getPath(pattern);
    //sys_printf( "Path of pattern %s is %s\n", pattern.c_str(), path.c_str( ) );
    HANDLE handle = FindFirstFile(pattern.c_str(), &find_data);
    BOOL bContinue = TRUE;
    while (handle != INVALID_HANDLE_VALUE && bContinue) {
      //sys_printf( "Added is [%s]%s\n", path.c_str(), filename.c_str( ) );
      if ((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        //sys_printf( "Added is [%s]%s\n", path.c_str(), filename.c_str( ) );
        if (use_fullname) {
          std::string fullname = path + find_data.cFileName;
          list_files.push_back(fullname);
        }
        else {
          list_files.push_back(find_data.cFileName);
        }
      }
      bContinue = FindNextFile(handle, &find_data);
    }
    FindClose(handle);
  }

  VStrings getFilesFromDirectory(const std::string &apattern, bool set_fullname) {
    VStrings tokens;
    StringUtils::split(apattern, '/', tokens);
    std::string pattern;
    for (size_t i = 0; i < tokens.size() - 1; ++i) {
      std::string::size_type idx = tokens[i].find('*');
      if (idx != std::string::npos) { //found wildcard
        std::string folder_pattern = pattern + tokens[i];
        ++i;
        VStrings vfolders;
        getFoldersFromDirectory(folder_pattern, vfolders);
        std::string last_patterns;
        for (; i < tokens.size(); ++i) {
          if (i == tokens.size() - 1) //last
            last_patterns = last_patterns + tokens[i];
          else
            last_patterns = last_patterns + tokens[i] + "/";
        }
        VStrings list_files;
        for (size_t j = 0; j < vfolders.size(); ++j) {
          std::string sub_pattern = pattern + vfolders[j] + "/" + last_patterns;
          VStrings vfiles = getFilesFromDirectory(sub_pattern, set_fullname);
          for (size_t a = 0; a < vfiles.size(); ++a)
          {
            list_files.push_back(vfiles[a]);
          }
        }
        return list_files;
      }
      pattern = pattern + tokens[i] + "/";
    }
    VStrings list_files;
    getFilesFromDirectory(apattern, list_files, set_fullname);
    return list_files;
  }


  void getFoldersFromDirectory(const std::string &root, VStrings &folders, bool set_fullname ) {
    BOOL bContinue = TRUE;
    WIN32_FIND_DATA data;

    std::string path = root;
    std::string::size_type idx = path.find_last_of("\\/");
    if (idx != std::string::npos)
      path = root.substr(0, idx + 1);
    if (path.back() == '\\' || path.back() == '/')
      path.append("*");

    //sys_printf( "Scanning folder at %s\n", root.c_str() );
    //sys_printf( "Scanning path at %s\n", path.c_str() );

    HANDLE hFind = FindFirstFile(path.c_str(), &data);

    // If we have no error, loop thru the files in this dir
    while (hFind != INVALID_HANDLE_VALUE && bContinue) {
      // Check if this entry is a directory
      if ((data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) {
        // Discard ., .., .svn
        if (data.cFileName[0] != '.') {
          std::string fullname = data.cFileName;
          if (set_fullname) {
            fullname = root + fullname + "/";
          }
          folders.push_back(fullname);
          //sys_printf( "  Added %s\n", folders.back().c_str() );
        }
      }
      bContinue = FindNextFile(hFind, &data);
    }
    FindClose(hFind); // Free the dir structure
  }

  bool copyFile(const std::string &src_path, const std::string &dst_path, bool overwrite) {
    return CopyFileA(src_path.c_str(), dst_path.c_str(), !overwrite) == TRUE;
  }

  bool deleteFile(const std::string &path) {
    return DeleteFileA(path.c_str()) == TRUE;
  }

  void createDirectory(const std::string &folder) {
    std::wstring wfolder(folder.begin(), folder.end());
    static const std::wstring separators(L"\\/");
    std::size_t slashIndex = wfolder.find_last_of(separators);
    size_t len = wfolder.size();
    if (slashIndex == len - 1) { //finish with \\/? then delete
      wfolder = wfolder.substr(0, len - 1);
    }
    createDirectoryRecursively(wfolder);
  }

  void deleteDirectory(const std::string &adirectory) {
    //delete all files
    VStrings files;
    std::string pattern = adirectory + "/*.*";
    files = getFilesFromDirectory(pattern, true);
    for (size_t i = 0; i < files.size(); ++i)
      deleteFile(files[i]);

    //delete all folders
    VStrings folders;
    getFoldersFromDirectory(adirectory, folders, true);
    for (size_t i = 0; i < folders.size(); ++i) {
      deleteDirectory(folders[i]);
    }

    RemoveDirectory(adirectory.c_str());
  }

};