#include "FileOP_includes.h"

#include "FileOP.h"
namespace FileOperations {
  const string FileOP::TXT::identifier[id_len] = {".txt", ".md", ".cpp", ".h", ".cs", ".c"};

  bool FileOP::checkPath(string path) {
    ifstream infile(path);
    if (!infile.good()) {infile.close();return false;} else{infile.close(); return true;};
  };

  int FileOP::isFile(char* path) {
    //Проверка пути
    ifstream infile(path);
    if (!infile.good()) {infile.close();return -1;};
    infile.close();
    
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode); //true - это файл | false - каталог
  };

  void FileOP::searchDir(char* path, string &files, string &folders, bool doRecursiveSearch) {
    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(path)) != NULL) {
      /*вывести все файлы и каталоги внутри каталога */
      while ((ent = readdir (dir)) != NULL) {
        string p = path; p+=ent->d_name;
        char* newpath = const_cast<char*>(p.c_str());
        
        if (isFile(newpath) != -1) { 
          //Сортировка 
          if (isFile(newpath) == 1) { //Файлы
            files += static_cast<string>(newpath) + '\n';
          } else {
              folders += static_cast<string>(newpath) + '\n';
              if (doRecursiveSearch == true) {
                p = newpath;
                p+="/";
                newpath = const_cast<char*>(p.c_str());
                FileOP::searchDir(newpath, files, folders);
              };
            }; //Каталоги
        };
      }
      closedir (dir);
    } else {
      /* не удалось открыть каталог */
      files = "Could Not Open Dir\n" + static_cast<string>(path); 
      folders = "Could Not Open Dir\n" + static_cast<string>(path);
    }
  };

  bool FileOP::isRealDir(char* path) {
    ifstream file(path);
    return file.good(); file.close();
  };

  bool FileOP::isRealDir(string path) {
    ifstream file(path);
    return file.good(); file.close();
  };

  void FileOP::TXT::read(string path, string& data) {
    ifstream infile(path, ios::binary);
    //получить длину файла:
    infile.seekg (0, infile.end);
    int length = infile.tellg();
    infile.seekg (0, infile.beg);

    //буфер для хранения данных
    char * buffer = new char [length];

    // считывание данных в виде блока:
    infile.read(buffer,length);
    data += buffer;
    //Уроооо оно больше нинадааа
    delete[] buffer;
    infile.close();
  };

  void FileOP::TXT::write(string path, string data) {
    //Оригинальный файл
    ofstream {path}; //Создание... Не имеет значения, будет ли он перезаписан, потому что это все равно произойдет
    ofstream outfile(path, ios::out | ios::trunc);
    outfile << data; //запись в файл
    outfile.close(); //закрытие файла
  };

  bool FileOP::mkFile(string path) {
    ofstream outFile(path);
    if (outFile.good() == false) {outFile.close(); return false;} else {return true;};
  };

};