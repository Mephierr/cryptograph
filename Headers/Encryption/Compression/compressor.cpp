#include "compression_includes.h"

#include "compressor.h"
namespace COMPRESSION {
  const string binary_compression::replacementChars[rCharLength[0]][rCharLength[1]] = {
    {"01", "A"},
    {"02", "B"},
    {"03", "C"},
    {"04", "D"},
    {"05", "E"},
    {"06", "F"},
    {"07", "G"},
    {"08", "H"},
    {"09", "I"},

    {"10", "J"},
    {"11", "K"},
    {"12", "L"},
    {"13", "M"},
    {"14", "N"},
    {"15", "O"},
    {"16", "P"},
    {"17", "Q"},
    {"18", "R"},
    {"19", "S"},

    {"20", "T"},
    {"21", "a"},
    {"22", "b"},
    {"23", "c"},
    {"24", "d"},
    {"25", "e"},
    {"26", "f"},
    {"27", "g"},
    {"28", "h"},
    {"29", "i"},
    {"30", "U"},
    {"31", "j"},
    {"32", "k"},
    {"40", "V"},
    {"41", "l"},
    {"42", "m"},
    {"50", "W"},
    {"51", "n"},
    {"52", "o"},
    {"60", "X"},
    {"61", "p"},
    {"62", "q"},
    {"70", "Y"},
    {"71", "r"},
    {"72", "s"},
    {"80", "Z"},
    {"81", "t"},
    {"82", "u"},
    {"90", "@"},
    {"91", "v"},
    {"92", "w"}
    };

  const string binary_compression::thirdLayerMulRChars[thirdLayerMultiplier[0]][thirdLayerMultiplier[1]] = {
    {"0", "."},
    {"1", ","},
    {"2", "}"},
    {"3", "]"},
    {"4", "["},
    {"5", "{"},
    {"6", "%"},
    {"7", "*"},
    {"8", "&"},
    {"9", "$"}
    };
  //Это 2 самые расспространенные символы
  const string binary_compression::fourthLayerMulRChars[fourthLayerMultiplier[0]][fourthLayerMultiplier[1]] = {
    {"A}", "x"},
    {"Ta", "y"},
    {"BL", "z"},
    {"BJ", ","},
    {"AT", "."},
    {"CJ", ":"},
    {"LA", "("},
    {"AU", ")"},
    {"TJ", "|"},
    {"AV", "'"},
    {"jT", "/"},
    {"Ua", "\\"},
    {"Uj", "+"},
    {"UA", "-"},
    {"MA", "_"},
    {"BM", "~"},
    {"UJ", "`"},
    {"Ja", "<"},
    {"Dj", ">"},
    {"DJ", "#"},
    {"CL", "="},
    {"TL", "!"},
    {"NA", "^"},
    {"aA", "\""},
    {"aT", "€"},
    {"JL", " "},
    {"Ul", "\2"},
    {"Vj", "\1"},
    {"UL", "\7"},
    {"TM", "\13"},
    {"EJ", "\16"},
    {"Xj", "\17"},
    {"Vl", "\6"},
    {"VL", "\27"}
    };


  const string binary_compression::fifthLayerMulRChars[fifthLayerMultiplier[0]][fifthLayerMultiplier[1]] = {
    {"0", ""},
    {"1", ""},
    {"2", ""},
    {"3", "\3"},
    {"4", "\4"},
    {"5", "\5"}
    };

  void binary_compression::compress(string& input) {    
    string str;

    //Первое сжатие
    //Допустим, у вас есть 2 байта 0100001100111100.
    //Это будет сокращено до 010412021402.
    //01 то же самое, затем 04 означает повторение 0 четыре раза при декомпрессии, 12 = 11 и так далее....
    for (int i = 0, increment = 0; i < input.length(); i++) {
      for (increment = 1;; increment++) {
        if (input[i] != input[i+increment] || increment == 9) {break;};
      };
      if (increment > 1) {
      str+=input[i] + to_string(increment); i += increment-1;
      }
      else {
        str += input[i];
      };
      increment = 1;      
    };

    input = str;
    str.clear();

    //Уже второе сжатие сжатого материала
    for (int i = 0; i<input.length(); i++) {
      string twoBytes;

      if (i+1 == input.length()) {str+=input[i]; break;};
      
      for (int x = 0; x < 2; x++) {
        twoBytes+=input[i+x];
      };
      
      for (int r = 0; r < rCharLength[0]; r++) {
        if (replacementChars[r][0] == twoBytes) {
          str+=replacementChars[r][1];
          i++;
          break;
        }
        else if (r+1 == rCharLength[0]) {
          str+=input[i];
        }
      }
    };

    input = str;
    str.clear();
    
    //Третий уровень сжатия
    for (int i = 0, increment = 1; i < input.length(); i++) {
      for (increment = 1;; increment++) {
        if (input[i] != input[i+increment] || increment == 9) {break;};
      };
      
      if (increment > 1) {
        str+=input[i] + thirdLayerMulRChars[increment][1];
        i += increment-1;
      }
      else {
        str += input[i];
      };
      increment = 1;      
    };


    input = str;
    str.clear();

    //Четвертый уровень
    for (int i = 0; i<input.length(); i++) {
      string twoBytes;

      if (i+1 == input.length()) {str+=input[i]; break;};
      
      for (int x = 0; x < 2; x++) {
        twoBytes+=input[i+x];
      };
      
      for (int r = 0; r < fourthLayerMultiplier[0]; r++) {
        if (fourthLayerMulRChars[r][0] == twoBytes) {
          str+=fourthLayerMulRChars[r][1];
          i++;
          break;
        }
        else if (r+1 == fourthLayerMultiplier[0]) {
          str+=input[i];
        }
      }
    };

    input = str;
    str.clear();
    
    //Пятый лвл
    for (int i = 0, increment = 1; i < input.length(); i++) {
      for (increment = 1;; increment++) {
        if (input[i] != input[i+increment] || increment == 5) {break;};
      };
      
      if (increment > 2) {
        str+=input[i] + fifthLayerMulRChars[increment][1];
        i += increment-1;
      }
      else {
        str += input[i];
      };
      increment = 1;      
    };

    str+=breakChar;
    input = str; str.clear();
  };

  void binary_compression::decompress(string& input) {
    string output;
    //Декомпрессия пятого слоя
    for (int i = 0, pos = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {output+=breakChar; break;}
      for (int x = 0; x < fifthLayerMultiplier[0]; x++) {
        if (input[i] == (fifthLayerMulRChars[x][1][0])) {pos = x; break;};
      };
      if (pos > 2) {
        int rep = fifthLayerMulRChars[pos][0][0]-'0';
        for (int x = 0; x < rep-1; x++) {
          output+=input[i-1];
        };
      }
      else {
        output+=input[i];
      };
      pos=0;
    };
    input = output;
    output.clear();
    //Декомпрессия четвертого слоя
    for (int i = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {output+=breakChar; break;};
      
      for (int x = 0; x < fourthLayerMultiplier[0]; x++) {
        if (fourthLayerMulRChars[x][1][0] == input[i]) {
          output+=fourthLayerMulRChars[x][0];
          break;
        }
        else if (x+1 == fourthLayerMultiplier[0]) {
          output+=input[i];
        };
      };
    };
    
    input=output;
    output.clear();

    //Декомпрессия третьего слоя
    for (int i = 0, pos = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {output+=breakChar; break;}
      for (int x = 0; x < thirdLayerMultiplier[0]; x++) {
        if (input[i] == (thirdLayerMulRChars[x][1][0])) {pos = x; break;};
      };
      if (pos > 1) {
        int rep = thirdLayerMulRChars[pos][0][0]-'0';
        for (int x = 0; x < rep-1; x++) {
          output+=input[i-1];
        };
      }
      else {
        output+=input[i];
      };
      pos=0;
    };
    input = output;
    output.clear();

    //Декомпрессия второго слоя
    for (int i = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {output+=breakChar; break;};
      
      for (int x = 0; x < rCharLength[0]; x++) {
        if (replacementChars[x][1][0] == input[i]) {
          output+=replacementChars[x][0];
          break;
        }
        else if (x+1 == rCharLength[0]) {
          output+=input[i];
        };
      };
    };
    
    input=output;
    output.clear();

    //Бинарная декомпрессия
    for (int i = 0; i < input.length(); i++) {
      if (input[i] == breakChar) {break;};
      if (input[i+1] != '0' && input[i+1] != '1' && input[i+1] != breakChar) {
        for (int x = 0; x < input[i+1]-'0'; x++) {
          output += input[i];
        };
        i++;
      } else {output+=input[i];};
    };
    input = output; output.clear();
  };

  /////////////////////////
  ///  RGB  ///////////////
  /////////////////////////
  const string rgb_compression::CORE::VAR::firstLayerMulRChars[firstLayerMultiplier[0]][firstLayerMultiplier[1]] {
    {"0", ""}, //Не будет юзаться :33333
    {"1", ""}, 
    {"2", "|"},
    {"3", "$"},
    {"4", "'"},
    {"5", "]"},
    {"6", "["},
    {"7", "="},
    {"8", "+"},
    {"9", "-"},
    {"10", "@"},
    {"11", "!"},
    {"12", ">"},
    {"13", "<"},
    {"14", "/"},
    {"15", "?"},
    {"16", "_"},
    {"17", "\""},
    {"18", "\\"},
    {"19", "a"},
    {"20", "b"},
    {"21", "c"},
    {"22", "d"},
    {"23", "e"},
    {"24", "f"},
    {"25", "g"},
    {"26", "h"},
    {"27", "i"},
    {"28", "j"},
    {"29", "k"},
    {"30", "l"},
    {"31", "m"},
    {"32", "n"},
    {"33", "o"},
    {"34", "p"},
    {"35", "q"},
    {"36", "r"},
    {"37", "s"},
    {"38", "t"},
    {"39", "u"},
    {"40", "v"},
    {"41", "w"},
    {"42", "x"},
    {"43", "y"},
    {"44", "z"},
    {"45", "A"},
    {"46", "B"},
    {"47", "C"},
    {"48", "D"},
    {"49", "E"},
    {"50", "F"},
    {"51", "G"},
    {"52", "H"},
    {"53", "I"},
    {"54", "J"},
    {"55", "K"},
    {"56", "L"},
    {"57", "M"},
    {"58", "N"},
    {"59", "O"},
    {"60", "P"},
    {"61", "Q"},
    {"62", "R"},
    {"63", "S"},
    {"64", "T"},
    {"65", "U"},
    {"66", "V"},
    {"67", "W"},
    {"68", "X"},
    {"69", "Y"},
    {"70", "Z"},
    {"71", "€"},
    {"72", "\2"},
    {"73", "\1"},
    {"74", "\7"},
    {"75", "\13"},
    {"76", "\16"},
    {"77", "\17"},
    {"78", "\6"},
    {"79", "\27"}
};

  void rgb_compression::CORE::FUNC::get_chunk_count(unsigned long int &inp, string clone, char separator, bool div) {
    inp = count(clone.begin(), clone.end(), separator);
    //поскольку каждый пиксель имеет 3 значения, разделим на 3, чтобы получить фактическое значение фрагмента
    if (div == true) {
      inp/=3;
    };
  };

  void rgb_compression::CORE::FUNC::replaceSeparator(string &inp, char sep, char repl) {
    int len = inp.length()-1;
    for (int y = 0; y<len; y++) {
      if (inp[y] == sep) {inp[y] = repl;};
    };
  };

  void rgb_compression::CORE::COMP::compress(string &inp, char separator) {
    unique_ptr<rgb_compression::CORE::FUNC> func;
    unique_ptr<rgb_compression::CORE::VAR> var;
    string clone = inp;
    //колво кусков
    unsigned long int chunk_count; func->get_chunk_count(chunk_count, clone, separator);
    string chunks[chunk_count]; //Большое выделение памяти
    //назначить кусочки для массива
    for (int i=0; i<chunk_count && clone.length() != 0; i++) {
      //По три за раз, поскольку фрагмент состоит из трех значений, например
      //255,98,23,
      if (clone.length() == 0) {break;};
      int first = clone.find_first_of(separator) + 1;
      chunks[i] += func->hexify(stoi(clone.substr(0, first-1)));
      clone.erase(0, first);

      if (clone.length() == 0) {break;};
      first = clone.find_first_of(separator) + 1;
      chunks[i] += func->hexify(stoi(clone.substr(0, first-1)));
      clone.erase(0, first);
      
      if (clone.length() == 0) {break;};
      first = clone.find_first_of(separator) + 1;
      chunks[i] += func->hexify(stoi(clone.substr(0, first-1))) + separator;
      clone.erase(0, first);
      if (clone[0] == ' ') {break;};
    };
    //Чистка памяти
    inp.clear(); clone.clear();
    

    //////                           //////
    ///////////////////////////////////////
    ///// Место , где магия возможна //////
    ///////////////////////////////////////
    //////                           //////

    
    //Нахождение совпадающих фрагментов и соединение их соответствующим образом.
    for (unsigned long int i = 0; i<chunk_count;) {
      unsigned long int rep = 0;
      
      //Нахождение повторяющиеся фрагментов
      for (; rep < var->firstLayerMultiplier[0]-1;) {
        if (rep > var->firstLayerMultiplier[0]-1) {break;};
        if (chunks[rep + i] == chunks[i]) {rep++;}
        else {break;};
      };
      
      //Компрессировать, если повторится > 1
      if (rep > 1) {
        string temp;
        temp += var->sectionStart + chunks[i] + var->sectionEnd + var->firstLayerMulRChars[rep][1] + separator;
        //Делаем кое-что с запятыми, чтобы код работал ))
        func->replaceSeparator(temp, separator, var->_separator);
        inp += temp; temp.clear();
        i += rep;
      }
      else {
        func->replaceSeparator(chunks[i], separator, var->_separator);
        inp += chunks[i];
        i++;
      };
    };
  };

  void rgb_compression::compress(string &inp, char separator) {
    unique_ptr<CORE::COMP> comp;
    
    if (inp.length() > comp->sizeLimit) {
      unique_ptr<CORE::FUNC> func;
      string half1, half2, quarter1, quarter2, quarter3, quarter4,
      eighth1, eighth2, eighth3, eighth4, eighth5, eighth6, eighth7, eighth8;
      //Разебываем исходные материалы пополам
      func->halfify(inp, separator, half1);
      half2 = inp; inp.clear();
      
      func->halfify(half1, separator, quarter1);
      quarter2 = half1; half1.clear();
      
      //Четверти к восьмым и запуск потоков.
      func->halfify(quarter1, separator, eighth1);
      thread th1(comp->compress, ref(eighth1), separator);
      
      eighth2 = quarter1; quarter1.clear();
      thread th2(comp->compress, ref(eighth2), separator);
      
      func->halfify(quarter2, separator, eighth3);
      thread th3(comp->compress, ref(eighth3), separator);
      
      eighth4 = quarter2; quarter2.clear();
      thread th4(comp->compress, ref(eighth4), separator);

      ////////////////
      func->halfify(half2, separator, quarter3);      
      quarter4 = half2; half2.clear();
      ////////////////
      
      func->halfify(quarter3, separator, eighth5);
      thread th5(comp->compress, ref(eighth5), separator);
      
      eighth6 = quarter3; quarter3.clear();
      thread th6(comp->compress, ref(eighth6), separator);
      
      func->halfify(quarter4, separator, eighth7); func.reset();
      thread th7(comp->compress, ref(eighth7), separator);
      
      eighth8 = quarter4; quarter4.clear();
      thread th8(comp->compress, ref(eighth8), separator);
      
      th1.join(); th2.join(); th3.join(); th4.join();
      th5.join(); th6.join(); th7.join(); th8.join();
      
      inp+=eighth1;
      inp+=eighth2;
      inp+=eighth3;
      inp+=eighth4;   
      inp+=eighth5;
      inp+=eighth6;
      inp+=eighth7;
      inp+=eighth8;
      
    } else {comp->compress(inp, separator);}; comp.reset();
    
  };

  ///////////////////////////
  ////// Расшифровка ////////
  ///////////////////////////

  void rgb_compression::CORE::FUNC::halfify(string &cln, char sep, string &ret) {
    unsigned int len = cln.length();
    unsigned int param = (len/2)+10;
    int i;
    for (i=0; i<len; i++) {
      if (param-i < 20 && cln[i] == sep) {break;}
    }
    i++;
    ret = cln.substr(0, i);
    cln.erase(0, i);
  };

  void rgb_compression::CORE::DECOMP::decompress(string &inp, char separator) {
    unique_ptr<rgb_compression::CORE::FUNC> func;
    unique_ptr<rgb_compression::CORE::VAR> var;
    //тэйк всех 
    unsigned long int chunk_count = 0; func->get_chunk_count(chunk_count, inp, separator, false);
    string clone = inp;
    string chunks[chunk_count];
    //назначение кусочков для массива
    for (int i=0; i<chunk_count; i++) {
      int first = clone.find_first_of(separator) + 1;
      chunks[i] += clone.substr(0, first);  clone.erase(0, first);
    };
    clone.clear(); inp.clear();
    
    
    for (int i=0; i<chunk_count; i++) {
      string str = chunks[i];
      
      string mulChar;
      bool isCompressed = false;
      if (str[0] == var->sectionStart) {
        isCompressed=true;
        str.erase(0,1);
        mulChar = str[str.length()-2];
        str.erase(str.length()-4, 4); 
        };
      
      string str1, str2, str3;
      str1 = str.substr(0,2); str.erase(0,2);
      str2 = str.substr(0,2); str.erase(0,2);
      str3 = str.substr(0,2); str.erase(0,2);
      func->dehexify(str1); func->dehexify(str2); func->dehexify(str3);
      str.clear();

      //перестроить чанк
      if (isCompressed) {
        str+=var->sectionStart;
      };
      str+=str1+var->_separator+str2+var->_separator+str3;
      if (isCompressed) {
        str+=var->_separator;
        str+=var->sectionEnd;
        str+=mulChar;
        str+=separator;
      } else {
        str+=separator;
      };
      chunks[i]=str;
    };
    
    for (int i=0; i<chunk_count; i++) {
      string section = chunks[i];
      if (corruptChunk(section, separator) == false) {
        func->replaceSeparator(section, var->_separator, separator);
        //если первый символ равен {, то мы знаем, что это сжатый кусок большого мозга
        if (section[0] == var->sectionStart) {
          char mul = section[section.length()-2];
          section = section.substr(1, section.length()-4);
          int rep;
          //сколько раз повторять цикл, брррр
          for (rep = 0; rep<var->firstLayerMultiplier[0]; rep++) {
            if (var->firstLayerMulRChars[rep][1][0] == mul) {break;};
          };
          //Повторение фрагмента для str
          for (int x=0; x<rep; x++) {
            inp+=section;
          };
        }
        //Довольно понятно
        else {
          inp+=section;
        };
      };
    };
    errorChecker(inp, separator);
  };

  void rgb_compression::decompress(string &inp, char separator) {
    unique_ptr<CORE::FUNC> func;
    unique_ptr<CORE::DECOMP> decomp;
    string half1, half2, quarter1, quarter2, quarter3, quarter4, eighth1,
    eighth2, eighth3, eighth4, eighth5, eighth6, eighth7, eighth8;
    //Разебать исходные материалы пополам
    func->halfify(inp, separator, half1);
    half2 = inp; inp.clear();
    
    func->halfify(half1, separator, quarter1);
    quarter2 = half1; half1.clear();
    
    //без четверти восемь и запускаем потоки
    func->halfify(quarter1, separator, eighth1);
    thread th1(decomp->decompress, ref(eighth1), separator);
    
    eighth2 = quarter1; quarter1.clear();
    thread th2(decomp->decompress, ref(eighth2), separator);
    
    func->halfify(quarter2, separator, eighth3);
    thread th3(decomp->decompress, ref(eighth3), separator);
    
    eighth4 = quarter2; quarter2.clear();
    thread th4(decomp->decompress, ref(eighth4), separator);

    ////////////////
    func->halfify(half2, separator, quarter3);      
    quarter4 = half2; half2.clear();
    ////////////////
    
    func->halfify(quarter3, separator, eighth5);
    thread th5(decomp->decompress, ref(eighth5), separator);
    
    eighth6 = quarter3; quarter3.clear();
    thread th6(decomp->decompress, ref(eighth6), separator);
    
    func->halfify(quarter4, separator, eighth7); func.reset();
    thread th7(decomp->decompress, ref(eighth7), separator);
    
    eighth8 = quarter4; quarter4.clear();
    thread th8(decomp->decompress, ref(eighth8), separator);

    
    th1.join(); th2.join(); th3.join(); th4.join();
    th5.join(); th6.join(); th7.join(); th8.join();
    
    inp+=eighth1;
    inp+=eighth2;
    inp+=eighth3;
    inp+=eighth4;   
    inp+=eighth5;
    inp+=eighth6;
    inp+=eighth7;
    inp+=eighth8;
    
    decomp->errorChecker(inp, separator); decomp.reset();
  };

  void rgb_compression::CORE::DECOMP::errorChecker(string &cln, char sep) {
    constexpr short offset = 2;
    //проверка наличия повторяющихся запятых
    for (int i=0; i<cln.length(); i++) {
      if (cln[i] == sep) {
        if (cln[i+1] == sep) {
          cln.erase(i, 1); i-=offset;
        };
      };

      //удаление недопустимых символов
      bool isGood = false;      
      for (int x=0; x<vCharLen; x++) {  
        if (cln[i] == validChars[x] || cln[i] == sep) {
          isGood = true; break;    
        };
      };
      
      if (isGood == false) {
        cln.erase(i, 1); i-=offset;
      };
    };
  };

  bool rgb_compression::CORE::DECOMP::corruptChunk(string cnk, char sep) {
    bool containsEndRep = false;
    bool containsStartRep = false;
    
    if (cnk[0] == CORE::VAR::sectionStart) {containsStartRep = true;};
    if (cnk[cnk.length()-3] == CORE::VAR::sectionEnd) {containsEndRep =  true;};
    
    //Это повторяющийся кусок?
    if (containsStartRep != containsEndRep) {return true;};
    
    //Это закончится?
    if (cnk[cnk.length()-1] != sep) {return true;};
    
    return false;
  };

  /////////////////////////////
  // ~ Назначение пикселей ~ //
  /////////////////////////////
  void rgb_compression::asgnPix(unsigned char* &PIX, string data, char sep, size_t length) {
    //Указатели для удобства чтения и письма.
    unique_ptr<CORE::FUNC> func;
    unique_ptr<CORE::PIXELS> pixels;
    
    //Выделение памяти
    char* pix = (char*)malloc(length); 
    PIX = (unsigned char*)malloc(length);
    
    //Выясняем, как разделить входные данные на управляемые, поточные формы.
    //Будет 8 потоков

    string str1, str2, quar1, quar2, quar3, quar4, eth1, eth2, eth3, eth4, 
    eth5, eth6, eth7, eth8;
    //Половина
    func->halfify(data, sep, str1);
    str2 = data; data.clear();

    //Четверть
    func->halfify(str1, sep, quar1);
    quar2 = str1; str1.clear();
    func->halfify(str2, sep, quar3);
    quar4 = str2; str2.clear();

    //1/8
    func->halfify(quar1, sep, eth1);
    uInt _eth1[2] = {0, pixels->findLength(eth1, sep)};
    
    eth2 = quar1; quar1.clear();
    uInt _eth2[2] = {_eth1[1], pixels->findLength(eth2, sep)+_eth1[1]};
    
    func->halfify(quar2, sep, eth3);
    uInt _eth3[2] = {_eth2[1], pixels->findLength(eth3, sep)+_eth2[1]};
    
    eth4 = quar2; quar2.clear();
    uInt _eth4[2] = {_eth3[1], pixels->findLength(eth4, sep)+_eth3[1]};
     
    func->halfify(quar3, sep, eth5);
    uInt _eth5[2] = {_eth4[1], pixels->findLength(eth5, sep)+_eth4[1]};
    
    eth6 = quar3; quar3.clear();
    uInt _eth6[2] = {_eth5[1], pixels->findLength(eth6, sep)+_eth5[1]};
    
    func->halfify(quar4, sep, eth7);
    uInt _eth7[2] = {_eth6[1], pixels->findLength(eth7, sep)+_eth6[1]};
    
    eth8 = quar4; quar4.clear();
    uInt _eth8[2] = {_eth7[1], pixels->findLength(eth8, sep)+_eth7[1]};
    
    thread asgn1(pixels->asgnPixThr, ref(pix), eth1, sep, _eth1[0], _eth1[1]);
    thread asgn2(pixels->asgnPixThr, ref(pix), eth2, sep, _eth2[0], _eth2[1]);
    thread asgn3(pixels->asgnPixThr, ref(pix), eth3, sep, _eth3[0], _eth3[1]);
    thread asgn4(pixels->asgnPixThr, ref(pix), eth4, sep, _eth4[0], _eth4[1]);
    thread asgn5(pixels->asgnPixThr, ref(pix), eth5, sep, _eth5[0], _eth5[1]);
    thread asgn6(pixels->asgnPixThr, ref(pix), eth6, sep, _eth6[0], _eth6[1]);
    thread asgn7(pixels->asgnPixThr, ref(pix), eth7, sep, _eth7[0], _eth7[1]);
    thread asgn8(pixels->asgnPixThr, ref(pix), eth8, sep, _eth8[0], _eth8[1]);
    
    pixels.reset(); func.reset(); //Заполнение памяти, умными указателями
    
    asgn1.join();
    asgn2.join();
    asgn3.join();
    asgn4.join();
    asgn5.join();
    asgn6.join();
    asgn7.join();
    asgn8.join();
    
    PIX = reinterpret_cast<unsigned char*>(pix);
  };

  uInt rgb_compression::CORE::PIXELS::findLength(string str, char sep) {
    uInt num = 0;
    for (int i=0; i<str.length(); i++) {
      if (str[i] == sep) {
        num++;
      };
    };
    return num;
  };

  void rgb_compression::CORE::PIXELS::asgnPixThr(char* &pix, string data, char sep, uInt startPos, uInt endPos) {
    for (unsigned int i=startPos; i<endPos+1; i++) {
      //подстрока RGB-номер
      if (data == "") {break;};
      size_t pos = data.find_first_of(sep);      
      if (pos == string::npos) {break;}
      string sm = data.substr(0, pos);
      if (sm == "") {break;};
      
      //назначение
      char uiui = stoi(sm);      
      pix[i] = uiui;
      
      //удаление из данных
      if (data.find(sep) != string::npos && pos < data.length()) {
        data.erase(0, data.find_first_of(sep)+1);
      } else {break;};
    };
  };

  //HEX COMPRESSION
  string rgb_compression::CORE::FUNC::hexify(unsigned short inp) {
    unique_ptr<rgb_compression::CORE::FUNC> func;
    unique_ptr<rgb_compression::CORE::VAR> var;
    float num = inp; num/=var->hexSize;
    //Удалить конечные 0s
    stringstream ss;
    ss << setprecision(4) << num;
    string result;
    string nu;
    ss >> nu;
    string fnum, snum; //первый и второй номер

    bool sec = false;
    for (int i=0; i<nu.length(); i++){
      if (nu[i] == '.') {sec=true;};
      if (!sec) {fnum+=nu[i];}
      else if (sec && nu[i]!='.') {snum+=nu[i]; if(snum.length()==2){break;};};
    };
    nu.clear();
    
    float ssnum = 0; 
    if (snum.length() > 0) {
      ssnum = stof(snum) * .01; 
      ssnum *= var->hexSize;
    };
    int ffnum = stoi(fnum); int sssnum = ssnum;
    result = var->hexArr[ffnum]; result += var->hexArr[sssnum];
    return result;
  };

  void rgb_compression::CORE::FUNC::dehexify(string &inp) {
    unique_ptr<rgb_compression::CORE::VAR> var;
    string result;
    int res1 = 0; int res2 = 0;
    for (short i=0; i<var->hexSize; i++) {
      if (inp[0] == var->hexArr[i]) {res1=i*var->hexSize;};
      if (inp[1] == var->hexArr[i]) {res2=i;};
    };
    res1+=res2;
    result = to_string(res1);
    inp=result;
  };

//Многопоточный пример
  /*
  int ex = 2;
  thread th(&foo, ref(ex)); //int& in func
  th.join();
  */
};