#ifndef INC_FILE_UTILS_H
#define INC_FILE_UTILS_H

#include <vector>
#include <string>

namespace FileUtils {
  typedef std::vector< std::string > VStrings;

  bool existsFile(const std::string& filename);
  bool dirExists(const std::string& dirName_in);
  size_t getFileSize(const std::string& filename);
  __int64	getFileDate(const std::string &filename);
  std::string getPath(const std::string& str);
  std::string getFilename(const std::string& str);
  std::string getFilenameName(const std::string& str);
  std::string	getLastDirectory(const std::string &apath);
  //std::string	getPathFrom(const std::string &path);
  std::string	getFileExtension(const std::string &path);


  VStrings getFilesFromDirectory(const std::string &path, bool set_fullname = false);
  void getFoldersFromDirectory(const std::string &root, VStrings &folders, bool set_fullname = false);

  bool copyFile(const std::string &src_path, const std::string &dst_path, bool overwrite);
  bool deleteFile(const std::string &path);
  void createDirectory(const std::string &folder);
  void deleteDirectory(const std::string &folder);
};


#endif // INC_FILE_UTILS_H