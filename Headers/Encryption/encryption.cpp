#include "encryption_includes.h"

#include "encryption.h"
namespace encryption {
  /////////////////////////////////////////////////////////
  ///////// AES ///////////////////////////////////////////
  /////////////////////////////////////////////////////////

  AESword AES::_4Bytes2Word(AESbyte& k1, AESbyte& k2, AESbyte& k3, AESbyte& k4) {
    AESword result(0x00000000);  
    AESword temp;  
    temp = k1.to_ulong();  // K1  
    temp <<= 24;  
    result |= temp;  
    temp = k2.to_ulong();  // K2  
    temp <<= 16;  
    result |= temp;  
    temp = k3.to_ulong();  // K3  
    temp <<= 8;  
    result |= temp;  
    temp = k4.to_ulong();  // K4  
    result |= temp;  
    return result;  
  };

  AESword AES::PosTrans(AESword& rw) {
    AESword high = rw << 8;  
    AESword low = rw >> 24;  
    return high | low;
  };

  AESword AES::SBoxTrans(AESword& sw)  {  
    AESword temp;  
    for(int i=0; i<32; i+=bitsInByte)  {  
        int row = sw[i+7]*bitsInByte + sw[i+6]*4 + sw[i+5]*2 + sw[i+4];  
        int col = sw[i+3]*bitsInByte + sw[i+2]*4 + sw[i+1]*2 + sw[i];  
        AESbyte val = AES::S_Box[row][col];  
        for(int j=0; j<bitsInByte; ++j)  
            temp[i+j] = val[j];  
    };  
    return temp;  
  };

  void AES::KeyExpansion(AESword w[expanded_key_size]) {  
    AESword temp;
    int i = 0;  
    //The first four of w [] are input keys  
    while(i < AESwords_in_key) {  
        w[i] = _4Bytes2Word(AES::KEY::key[4*i], AES::KEY::key[4*i+1], AES::KEY::key[4*i+2], AES::KEY::key[4*i+3]);  
        ++i;  
    };
  
    i = AESwords_in_key;  
  
    while(i < expanded_key_size) {  
      temp = w[i-1]; //Record the previous word  
      if(i % AESwords_in_key == 0) {
        AESword rwt = PosTrans(temp);
        w[i] = w[i-AESwords_in_key] ^ SBoxTrans(rwt) ^ WheelConst[i/AESwords_in_key-1];  
      }
      else {
        w[i] = w[i-AESwords_in_key] ^ temp;  
      };
      ++i;  
    };
  };

  void AES::SBoxTransSubBytes(AESbyte mtx[mtx_size]) {  
    for(int i=0; i<mtx_size; ++i)  {  
        int row = mtx[i][7]*8 + mtx[i][6]*4 + mtx[i][5]*2 + mtx[i][4];  
        int col = mtx[i][3]*8 + mtx[i][2]*4 + mtx[i][1]*2 + mtx[i][0];  
        mtx[i] = S_Box[row][col];  
    };
  };

  void AES::ShiftRows(AESbyte mtx[mtx_size]) {  
    //Вторая линия круга перемещается на один бит влево
    AESbyte temp = mtx[4];  
    for(int i=0; i<3; ++i) {
      mtx[i+4] = mtx[i+5];
    };

    mtx[7] = temp;

    //Третий круг перемещается на две позиции влево.
    for(int i=0; i<2; ++i) {  
        temp = mtx[i+8];  
        mtx[i+8] = mtx[i+10];  
        mtx[i+10] = temp;  
    };
    //Четвертая линия перемещает три круга влево
    temp = mtx[15];  
    for(int i=3; i>0; --i)  
        mtx[i+12] = mtx[i+11];  
    mtx[12] = temp;
  };  
  
  
  //Умножение над конечными полями GF(2^8)
  AESbyte AES::GaloisFieldsMul(AESbyte a, AESbyte b) {   
      AESbyte p = 0;  
      AESbyte hi_bit_set;  
      for (int counter = 0; counter < 8; counter++) {  
          if ((b & AESbyte(1)) != 0) {  
              p ^= a;  
          }  
          hi_bit_set = (AESbyte) (a & AESbyte(0x80));  
          a <<= 1;  
          if (hi_bit_set != 0) {  
              a ^= 0x1b; /* x^8 + x^4 + x^3 + x + 1 */  
          }  
          b >>= 1;  
      }  
      return p;  
  };

  void AES::MixColumns(AESbyte mtx[mtx_size])  {  
    AESbyte arr[4];  
    for(int i=0; i<4; ++i)  {  
        for(int j=0; j<4; ++j) {
          arr[j] = mtx[i+j*4];  
        };  
        mtx[i] = GaloisFieldsMul(0x02, arr[0]) ^ GaloisFieldsMul(0x03, arr[1]) ^ arr[2] ^ arr[3];  
        mtx[i+4] = arr[0] ^ GaloisFieldsMul(0x02, arr[1]) ^ GaloisFieldsMul(0x03, arr[2]) ^ arr[3];  
        mtx[i+8] = arr[0] ^ arr[1] ^ GaloisFieldsMul(0x02, arr[2]) ^ GaloisFieldsMul(0x03, arr[3]);  
        mtx[i+12] = GaloisFieldsMul(0x03, arr[0]) ^ arr[1] ^ arr[2] ^ GaloisFieldsMul(0x02, arr[3]);  
    }; 
  };

  void AES::AddRoundKey(AESbyte mtx[mtx_size], AESword k[4]) {  
    for(int i=0; i<4; ++i) {
      AESword k1 = k[i] >> 24;
      AESword k2 = (k[i] << 8) >> 24;
      AESword k3 = (k[i] << 16) >> 24;
      AESword k4 = (k[i] << 24) >> 24;
      mtx[i] = mtx[i] ^ AESbyte(k1.to_ulong());  
      mtx[i+4] = mtx[i+4] ^ AESbyte(k2.to_ulong());  
      mtx[i+8] = mtx[i+8] ^ AESbyte(k3.to_ulong());  
      mtx[i+12] = mtx[i+12] ^ AESbyte(k4.to_ulong());  
    };  
  };

  void AES::InvSBoxTransSubBytes(AESbyte mtx[mtx_size]) {  
    for(int i=0; i<mtx_size; ++i) {  
      int row = mtx[i][7]*8 + mtx[i][6]*4 + mtx[i][5]*2 + mtx[i][4];  
      int col = mtx[i][3]*8 + mtx[i][2]*4 + mtx[i][1]*2 + mtx[i][0];  
      mtx[i] = Inv_S_Box[row][col];
    };
  };

  void AES::InvShiftRows(AESbyte mtx[mtx_size]) {  
    //Второй круг перемещается на один бит вправо.
    AESbyte temp = mtx[7];  
    for(int i=3; i>0; --i)  
        mtx[i+4] = mtx[i+3];  
    mtx[4] = temp;  
    //Третий круг перемещается на два вправо.
    for(int i=0; i<2; ++i) {  
        temp = mtx[i+8];  
        mtx[i+8] = mtx[i+10];  
        mtx[i+10] = temp;  
    };
    //Круг четвертой линии перемещается на три вправо.
    temp = mtx[12];  
    for(int i=0; i<3; ++i)  
        mtx[i+12] = mtx[i+13];  
    mtx[15] = temp;  
  };  

  void AES::InvMixColumns(AESbyte mtx[mtx_size]) {  
    AESbyte arr[4];  
    for(int i=0; i<4; ++i) {  
        for(int j=0; j<4; ++j) {
          arr[j] = mtx[i+j*4];  
        };  
        mtx[i] = GaloisFieldsMul(0x0e, arr[0]) ^ GaloisFieldsMul(0x0b, arr[1]) ^ GaloisFieldsMul(0x0d, arr[2]) ^ GaloisFieldsMul(0x09, arr[3]);  
        mtx[i+4] = GaloisFieldsMul(0x09, arr[0]) ^ GaloisFieldsMul(0x0e, arr[1]) ^ GaloisFieldsMul(0x0b, arr[2]) ^ GaloisFieldsMul(0x0d, arr[3]);  
        mtx[i+8] = GaloisFieldsMul(0x0d, arr[0]) ^ GaloisFieldsMul(0x09, arr[1]) ^ GaloisFieldsMul(0x0e, arr[2]) ^ GaloisFieldsMul(0x0b, arr[3]);  
        mtx[i+12] = GaloisFieldsMul(0x0b, arr[0]) ^ GaloisFieldsMul(0x0d, arr[1]) ^ GaloisFieldsMul(0x09, arr[2]) ^ GaloisFieldsMul(0x0e, arr[3]);  
    };
  };

  //Простое преобразование char в шестнадцатеричный вид
  AESbyte AES::CONVERSIONS::char_to_byte_(char inp) {
    return static_cast<AESbyte>(inp);
  };

  char AES::CONVERSIONS::binary_to_char(AESbyte input) {
    unsigned int num = 0;
    string byte_str = input.to_string();
    int base = 1;
    for (short i = 7; i > -1; i--) {
      if (byte_str[i] == '1') {
        num += base;
      };
      base *= 2;
    };
    return char(num);
  };

  unsigned int AES::getRandomNum(unsigned int min, unsigned int max) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(min,max);
    return dist6(rng);
  };

  void AES::generate_key() {
    for (int x = 0; x < mtx_size; x++) {
      AESbyte bin = 0b00000000;
      for (int i = 0; i < bitsInByte; i++) {
        bin[i] = getRandomNum(0, 1);
      };
      AES::KEY::key[x] = bin;
    };
  };

  AESword AES::global_expanded_key[expanded_key_size];

  void AES::aes_init(OPTIONS genkey, string dummykey) {
    if (genkey == OPTIONS::doGenerateKey || dummykey.length() < mtx_size*bitsInByte) {
      generate_key();
      }
    else {
      for (int y = 0, n=0; y < mtx_size; y++, n+=bitsInByte) {
        AES::KEY::key[y] = CONVERSIONS::binStr_to_byte(dummykey.substr(n, n+bitsInByte));
      };
    };
    KeyExpansion(AES::global_expanded_key);
  };

  AESbyte AES::CONVERSIONS::binStr_to_byte(string input) {
    AESbyte result;
    for (int i = 0, y = bitsInByte-1; i < bitsInByte; i++, y--) {
      if (input[y] == '1') {
        result[i] = 1;
      }
    };
    return result;
  }
  
  //////////////////////////////////////////////////////
  ////////Encrypt & Decrypt Functions///////////////////
  //////////////////////////////////////////////////////

  void AES::cypher_encrypt(AESbyte in[mtx_size], AESword w[expanded_key_size]) {
    AESword key[4];  
    AES clone;
    for(int i=0; i<4; ++i) {
      key[i] = w[i];
    };
    clone.AddRoundKey(in, key);  
    for(int round=1; round<rounds_of_encryption; ++round) {  
      clone.SBoxTransSubBytes(in);  
      clone.ShiftRows(in);  
      clone.MixColumns(in);  
      for(int j=0; j<4; ++j) {
        key[j] = w[4*round+j];  
      };
      clone.AddRoundKey(in, key);  
    };
    clone.SBoxTransSubBytes(in);  
    clone.ShiftRows(in);
    for(int k=0; k<4; ++k){
      key[k] = w[4*rounds_of_encryption+k];  
    };
    clone.AddRoundKey(in, key);
  };  

  void AES::cypher_decrypt(AESbyte in[mtx_size], AESword w[expanded_key_size])  {
    AESword key[4];
    AES clone;
    for(int i=0; i<4; ++i) {
      key[i] = w[4*rounds_of_encryption+i];  
    };
    clone.AddRoundKey(in, key);  
    for(int round=rounds_of_encryption-1; round>0; --round) {
      clone.InvShiftRows(in);  
      clone.InvSBoxTransSubBytes(in);  
      for(int i=0; i<4; ++i)  
          key[i] = w[4*round+i];
      clone.AddRoundKey(in, key);  
      clone.InvMixColumns(in);  
    }  
    clone.InvShiftRows(in);  
    clone.InvSBoxTransSubBytes(in);  
    for(int i=0; i<4; ++i)  
        key[i] = w[i];  
    clone.AddRoundKey(in, key);  
  };  


  ///////////////////////////////////////////////////////
  /////Прохождение функций шифрования и дешифрования/////
  /////////это входные и выходные строки/////////////////
  ///////////////////////////////////////////////////////
  AESbyte AES::KEY::key[mtx_size]; //Определение ключа

  /////////////
  ///Encrypt///
  /////////////

  string AES::encrypt(string input) {
    unsigned int length = input.length();
    string output;
    AESword w[expanded_key_size];
    for (int i = 0; i < expanded_key_size; i++) {
      w[i] = global_expanded_key[i];
    };
    for (unsigned int loop = 0; loop < length; loop+=mtx_size) {
      
      AESbyte hex_val[mtx_size] = {SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE, SPACE_BYTE};

      //преобразование символа в двоичный байт
      for (int x = 0; x < mtx_size; x++) {
        if ((loop+x) < length) {
          hex_val[x] = CONVERSIONS::char_to_byte_(input[loop+x]);
        }
        else {
          hex_val[x] = SPACE_BYTE;
        };
      };

      cypher_encrypt(hex_val, w);
      //Преобразование двоичного кода в шестнадцатеричный, а затем превращение его в строку
      for (int x = 0; x < mtx_size; x++) {
        output += hex_val[x].to_string(); //Двоичный аутпут
      };
      
    };
    for (int i = 0; i < expanded_key_size; i++) {
      global_expanded_key[i] = w[i];
    };
    binary_compression::compress(output);
    return output;
  };

  /////////////
  ///Decrypt///
  /////////////

  string AES::decrypt(string input) {
    binary_compression::decompress(input);
    unsigned int length = input.length();
    string output;
    constexpr unsigned short arrSize = mtx_size;
    AESword w[expanded_key_size];
    for (int i = 0; i < expanded_key_size; i++) {
      w[i] = global_expanded_key[i];
    };

    for (unsigned int loop = 0; loop < length; loop+=arrSize*bitsInByte) {
      static AESbyte hex_val[arrSize];
      for (int x = loop, p = 0; p < arrSize; x+=bitsInByte, p++) {
        hex_val[p] = CONVERSIONS::binStr_to_byte(input.substr(x, x+bitsInByte));
      };
      
      cypher_decrypt(hex_val, w);
      for (int n = 0; n < arrSize; n++) {
        if (hex_val[n] != SPACE_BYTE) {
          output += CONVERSIONS::binary_to_char(hex_val[n]);
        }
      };
    };
    for (int i = 0; i < expanded_key_size; i++) {
      global_expanded_key[i] = w[i];
    };
    return output;
  };

  ////////////////
  ///File Stuff///
  ////////////////

  bool AES::encryptFF(string path) {
    string output;
    //Проверка на правильный путь
    ifstream infile(path);
    if (infile.good() == false) {return false;};
    //Получение текста из файла
    string text;
    string line;
    while (getline(infile, line)) {
      text += line + '\n';
    };
    infile.close();
    text = encrypt(text);
    ofstream outfile(path, ios::out | ios::trunc);
    outfile << text;
    outfile.close();
    return true;
  };

  bool AES::decryptFF(string path) {
    string output;
    //Проверка на правильный путь
    ifstream infile(path);
    if (infile.good() == false) {return false;};
    //Получение текста из файла
    string text;
    getline(infile, text); 
    infile.close();
    text = decrypt(text);
    ofstream outfile(path, ios::out | ios::trunc);
    outfile << text;
    outfile.close();
    return true;
  };

  ///////////////////////////////////////////////
  ///Создание собственных зашифрованных файлов///
  ///////////////////////////////////////////////

  void AES::FILES::BMP_::get(string path, string& data) {
    char* filename = const_cast<char*>(path.c_str());
    BMPbyte *pixels;
    int32 width, height, bytesPerPixel;
    //Получение размера пикселя с помощью width*height*bytesPerPixel
    BMP::ReadImage(filename, &pixels, &width, &height, &bytesPerPixel);
    data+=to_string(width)+FILES::BMP_::DATA_SEPARATOR;
    data+=to_string(height)+FILES::BMP_::DATA_SEPARATOR;
    data+=to_string(bytesPerPixel)+FILES::BMP_::DATA_SEPARATOR;
    string pix;
    for (unsigned int i=0; i<(width*height*bytesPerPixel); i++) {
      pix+=to_string(pixels[i]) + NUM_SEPARATOR;
    };
    
    rgb_compression::compress(pix, NUM_SEPARATOR);
    
    data+=pix;
    pix.clear();
  };

  void AES::FILES::BMP_::out(string path, string data) {
    //Преобразование путь str to char*
    char* filename = const_cast<char*>(path.c_str());
    data.erase(0, data.find_first_of(AES::FILES::EXTENSION_SEPERATOR)+1);
    int32 width, height, bytesPerPixel;
    string str;
    //получение ширины
    str = data.substr(0, data.find_first_of(DATA_SEPARATOR));
    width = stoi(str);
    //получение высоты
    data.erase(0, data.find_first_of(DATA_SEPARATOR)+1);
    str = data.substr(0, data.find_first_of(DATA_SEPARATOR));
    height = stoi(str);
    //получение bytesPerPixel
    data.erase(0, data.find_first_of(DATA_SEPARATOR)+1);
    str = data.substr(0, data.find_first_of(DATA_SEPARATOR));
    bytesPerPixel = stoi(str);
    data.erase(0, data.find_first_of(DATA_SEPARATOR)+1);

    rgb_compression::decompress(data, NUM_SEPARATOR);
    //Длина пикселей
    const unsigned int length = height*(width*bytesPerPixel);
    
    BMPbyte* pixels;    
    rgb_compression::asgnPix(pixels, data, NUM_SEPARATOR, length); data.clear();
    
    //Написание изображение
    BMP::WriteImage(filename, pixels, width, height, bytesPerPixel);
  };

  //Design: тип файла будет перед всеми данными.
  //В расшифрованном коде он будет разделен знаком ~.

  //пример
  //.{extension}~{data}
  //В случае растровых изображений это будет так
  //.bmp~{width}:{height}:{bytesPerPixel}:{rgb int},{rgb int},etc...
  
  //Расширение: .aesenc
  const string AES::FILES::FILE_EXTENSION = ".aesenc";
  const string AES::FILES::KEYFILE_NAME = "_KEYFILE";
  const string AES::FILES::KEYFILE_EXT = ".aeskey";
  const string AES::FILES::BMP_::identifier = ".bmp";

  //Классифицировать тип файла
  void AES::FILES::classify(string ext, AES::FILES::CLASSIFIER &type) {
    for (int i=0; i < FileOP::TXT::id_len; i++) {
      if (ext == FileOP::TXT::identifier[i]) {type=FILES::CLASSIFIER::_TEXT;break;};
    };
    
    if (ext == FILES::BMP_::identifier) {type=FILES::CLASSIFIER::_BITMAP;};

    if (ext == PNG::ID) {type=FILES::CLASSIFIER::_PNG;};
  };

  //Файл ключей
  bool AES::FILES::gen_key_file(string path) {
    path.erase(path.rfind('.'), path.length()); //Удалить расширение
    path+=KEYFILE_NAME;//изменить имя
    path+=KEYFILE_EXT;//добавить расширение
    FileOP::mkFile(path);
    
    string data;
    for (int i = 0; i < AES::mtx_size; i++) {
      data+=AES::KEY::key[i].to_string();
    };

    size_t key;
    data = AKARE::encrypt(data, key);
    
    binary_compression::compress(data);
    
    data = to_string(key) + '\n' + data;
    
    FileOP::TXT::write(path, data);
    return true;
  };

  bool AES::FILES::in_key_file(string path) {
    path.erase(path.rfind('.'), path.length()); //Удалить расширение
    
    path+=KEYFILE_NAME;//изменение имени
    path+=KEYFILE_EXT;//добавление расширения
    FileOP::checkPath(path);
    
    string data;
    FileOP::TXT::read(path, data);

    short fst = data.find_first_of('\n');

    if (fst != string::npos) {
      string ky = data.substr(0, fst);

      data = data.substr(fst+1, data.length()-fst);
      binary_compression::decompress(data);
 
      stringstream ss;
      ss << ky;
      size_t key;
      ss >> key;

      data = AKARE::decrypt(data, key);
        
    } else {
      binary_compression::decompress(data);
    };
    
    
    aes_init(AES::OPTIONS::noGenerateKey, data);
    return true;
  };

  bool AES::encryptFile(string path, string password, FILE_FLAGS flags) {
    AKARE::password_key = password;
    //checks if path is valid
    if(FileOP::isRealDir(path) == false) {return false;};
    
    AES::FILES::gen_key_file(path);

    string ext = path.substr(path.find_last_of('.'), path.length());

    //Add the extension + seperator to the data string
    string data = ext + FILES::EXTENSION_SEPERATOR;

    FILES::CLASSIFIER type = FILES::CLASSIFIER::_RETURN;
    FILES::classify(ext, type);
    
    switch (type) {
      case FILES::CLASSIFIER::_RETURN:
        return false;
      
      case FILES::CLASSIFIER::_TEXT:
        FileOP::TXT::read(path, data);
        break;
      
      case FILES::CLASSIFIER::_BITMAP:
        FILES::BMP_::get(path,data);
        break;
      
      case FILES::CLASSIFIER::_PNG:
        PNG::read(path, data);
        break;
      
      default:
        return false;
    };
    
    ext.clear(); //удаление, потому что это бесполезно
    
    //шифровка
    data = encrypt(data);
      
    //Создание нового файла и пути, используя старый путь, удаляя расширение из строки.
    if (flags == FILE_FLAGS::deleteInputFile) {remove(path.c_str());};
    path.erase(path.rfind('.'), path.length());
    path+=FILES::FILE_EXTENSION;
    //Запись, так почему бы не использовать текстовый вывод
    FileOP::TXT::write(path, data);
    return true;
  };

  bool AES::decryptFile(string path, string password, string keyFilePath, FILE_FLAGS flags) {
     AKARE::password_key = password;
     //проверка на действительность пути
     if(FileOP::isRealDir(path) == false || path.substr(path.find_last_of('.'), path.length()) != FILES::FILE_EXTENSION) {return false;}
    string data;
    //Точно так же, как в encryptFile
    FileOP::TXT::read(path, data);

    if (keyFilePath != "") {AES::FILES::in_key_file(keyFilePath);}
    else {AES::FILES::in_key_file(path);};    
    
    //Расшифровка
    data = decrypt(data);

    if (flags & AES::FILE_FLAGS::deleteAesencFile) {remove(path.c_str());};
    if (flags & AES::FILE_FLAGS::deleteKeyFile) {
      string pa = path;
      pa.erase(pa.rfind('.'), pa.length()); //Удаление расширения
      pa+=FILES::KEYFILE_NAME;//изменение имени
      pa+=FILES::KEYFILE_EXT;//добавление расширения
      remove(pa.c_str()); pa.clear();
    };
    
    //создание нового пути
    path.erase(path.find_last_of('.'), path.length());
    string ext = data.substr(data.find_first_of('.'), data.find_first_of(FILES::EXTENSION_SEPERATOR));
    path += ext; //добавление расширения к пути

    //Удаление этой части расшифрованных данных
    data.erase(data.find_first_of('.'), data.find_first_of(FILES::EXTENSION_SEPERATOR)+1);
    data.erase(data.length(), data.length());

    FILES::CLASSIFIER type = FILES::CLASSIFIER::_RETURN;
    FILES::classify(ext, type);
    ext.clear(); // очистка памяти
    switch (type) {
      case FILES::CLASSIFIER::_RETURN:
        return false;
      
      case FILES::CLASSIFIER::_TEXT:
        FileOP::TXT::write(path, data);
        break;
      
      case FILES::CLASSIFIER::_BITMAP:
        FILES::BMP_::out(path,data);
        break;

      case FILES::CLASSIFIER::_PNG:
        PNG::write(path, data);
        break;
      
      default:
        return false;
    };
    return true;
  };
};