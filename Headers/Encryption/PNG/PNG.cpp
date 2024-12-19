#include "PNG_Includes.h"

#include "PNG.h"
namespace PNG_H {
  const string PNG::ID = ".png";

  bool PNG::read(string path, string& data) {
    //Подготовка data var для того, чтобы к нему был применен буфер
    data.clear();
    data = "";

    //инициализация файла
    ifstream infile(path, std::ios::in | std::ios::binary | std::ios::ate);
    
    //получение длины файла:
    infile.seekg (0, infile.end);
    int length = infile.tellg();
    infile.seekg (0, infile.beg);

    //буфер для хранения данных
    char * buffer = new char [length];

    // считывание данных в виде блока:
    infile.read(buffer,length);

    //После прочтения закройте
    infile.close();
    
    //Присвоение его переменной данных
    for (int i=0; i<length; i++) {
      data += buffer[i];
    };
    
    //смерт
    delete[] buffer;
    
    return true;
  };

  bool PNG::write(string path, string data) {
    ofstream out(path); out << data; out.close(); //Функция в одну строку, лол
    return true;
  };
};