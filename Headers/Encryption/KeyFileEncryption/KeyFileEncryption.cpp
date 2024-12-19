  #include "KeyFileEncryption.h"
namespace asymmetric {
  string AKARE::password_key = "NULL";
  size_t AKARE::convertPassword() {
    size_t num = 0;
    for (int i=0; i<password_key.length(); i++) {
      num += password_key[i];
      srand(num);
      num += rand() % 100 + 1;
      num += rand();
      num += rand() % num + 1;
      srand(rand() + num + entropy::Val2);
      num += entropy::Val1;
      num += rand();
    };
    srand(num); srand(num+rand());
    for (int i = 0; i<(rand()%100000+1); i++) {
      srand(num);
      if ((rand()%100+1)>50) {num+=rand()%10000+1;};
    };
    return num;
  };

  //Гарантируем, что номера действительны
  bool AKARE::isValidNum(size_t num) {
    float* div = (float*)malloc(sizeof(float));
    *div = (float)(num) / (float)(staticPrivateKey);
    if (num%2 == 0 && isDecimal(*div) == false) {free(div); return true;}
    else {free(div); return false;};
  };

  //Серверная часть для генерации случайных четных чисел
  size_t AKARE::evenGeneratorBackend(size_t min, size_t max, size_t loop) {
    if (loop >= 200) {return 0;}; loop++;
    
    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<mt19937::result_type> dist6(min,max);
    
    size_t* val = (size_t*)malloc(sizeof(size_t));
    *val = dist6(rng);
    if (isValidNum(*val)) {return *val;}
    else {
      free(val);
      return evenGeneratorBackend(min, max, loop); 
    };
  };

  //Генерация четного число
  size_t AKARE::evenGenerator(size_t min, size_t max) {
    size_t gen = evenGeneratorBackend(min, max);
    
    if (gen == 0) {return evenGenerator();}
    else {return gen;};
  };

  //Проверка на то, является ли число десятичным
  bool AKARE::isDecimal(float inp) {
    if (size_t(inp) != inp) {return true;}
    else {return false;};
  };

  //Генерация действительного четного число на основе открытого ключа
  size_t AKARE::mod(size_t inp) {
    sMod(inp);
    srand(inp + convertPassword() + entropy::Val4);
    
    size_t val = rand();
    while (1) {
      if (!isValidNum(val)) {val = rand();}
      else {break;};
    };
    return val;
  };

  //С помощью пользовательского алгоритма начальное значение mod() для srand становится более случайным. Это должно обеспечить достаточную запутанную полуслучайность, чтобы результат был непредсказуемым без функции
  void AKARE::sMod(size_t &inp) {
    if (inp > bounds::genMin*2) {inp/=2;};
    inp += entropy::Val3;
    
    srand(inp - staticPrivateKey);
    if ((rand() % 100 + 1) > 50) {
      inp += convertPassword();
    };
    stringstream ss;
    string str;
    ss << inp;
    ss >> str;
    
    if (str[0]-'0' > 5) {inp*=4; inp+=rand();}; 
    
    ss.str("");
    ss << inp;
    ss >> str;
    ss.str("");
    
    if (str[2] < 5) {inp*=6;} else {inp*=8;};
    
    srand(inp);
    size_t ra = rand() % 100 + 1;
    
    if (ra > 50) {
      inp*=4;
      srand(inp+ra);
      if ((rand() % 100 + 1) > 50) {inp*=4;} else {inp/=2;};
    } 
    else {
      inp/=2;
      srand(inp*ra);
      if ((rand() % 100 + 1) > 50) {inp*=8;} else {inp/=2;};
    };

    srand(inp);
    ra = rand() % 100 + 1;
    if (ra < 20) {inp*=ra;} else {inp*=2;};
    
    size_t ra2 = rand() % 100 + 1, ra3 = rand() % 100 + 1;
    
    if (ra < ra2 && ra2 > ra3) {inp/=2;}
    else if (ra2 < ra && ra3 > ra) {inp*=2;}
    else if (ra < 70 && ra2 < 50 && ra3 < 60) {inp*=6;}
    else {inp*=8;};
    
    srand (ra+ra2+ra3+inp);
    if ((rand() % 100 + 1) >= 50) {inp/=2;} else {inp*=4;};

    ra = rand() % 2 + 1; ra--;

    if (ra == 1) {inp*=2; inp -= entropy::Val1;}
    else {inp/=2;};
    
    srand(inp+convertPassword());
    
    ra = fmod(rand(), rand());
    ra2 = fmod(ra, rand());
    ra3 = fmod(ra, ra2);
    
    srand(ra3);
    
    inp += ra + ra2 + ra3 + rand() + rand() + rand();

    inp += entropy::Val2;

    srand(rand() + rand() + rand() + ra3 + rand());

    ra = rand() % 100 + 1;
    ra2 = pow(ra3, ra);
    inp += ra + ra2;

    srand(rand()+ra+fmod(ra,ra3)+rand()+rand()+inp+ra2+rand());

    ra = rand() % 1000 + 1;
    for (int i = 0; i < ra; i++) {
      ra3 = fmod(rand(), fmod(rand(), rand()));
      ra2 = fmod(fmod(rand(), rand()), fmod(fmod(rand(), rand()), fmod(rand(), rand())));
      inp -= fmod(ra3, ra2) + ra;
    };
    srand(fmod(rand(), rand()));
    ra = rand() % 100 + 1;

    if (ra > 50) {inp+=convertPassword(); sMod(inp);};

    ra = rand() % 100 + 1;

    if (ra < 50) {inp -= fmod(rand(), rand()); inp -= convertPassword();};
    if (ra < 25) {inp -= (fmod(rand(), rand()) * fmod(rand(), ra2));};
    if (ra > 75) {inp-=rand()*rand()*rand(); inp -= entropy::Val3;};
    ra2 = rand() % 100 + 1;

    for (int i = 0; i < ra*ra2; i++) {
      srand(ra+ra2+ra3+inp+i+rand()+(rand()*rand()));
      srand(staticPrivateKey * rand() * rand() * rand());
      srand((rand() * rand()) + ra2 - (rand() * ra3) * rand());
      inp -= rand();
    };

    srand(staticPrivateKey+inp+rand()+rand()+ra+rand()+ra2+rand()+ra3);
    ra=rand()%1000+1; ra2=rand()%100+1; ra3=rand()%10+1;

    for (int i = 0; i < ra + ra2 * ra3; i++) {
      srand(rand()+staticPrivateKey+rand()*fmod(rand(),rand()));
      inp-=staticPrivateKey/(rand() % 100 + 2);
    };
    
    ra = fmod(rand(), rand());
    inp-=ra; inp += entropy::Val4;

    for (int i = 0; i < (rand()%100+1); i++) {
      srand(fmod(rand(), rand()));
      inp-=rand();
    };

    srand(staticPrivateKey*rand()+rand()*rand());
    ra = rand();
    srand(entropy::Val1+ra);
    ra *= rand() + staticPrivateKey;

    inp -= ra;

    srand(rand()+rand()+rand()+ra+ra2+ra3+rand()*inp);

    ra = rand();
    ra2 = rand();
    ra3 = rand();

    ra += ra2*ra3;

    ra *= (ra2+ra3) * (ra3*rand()) + rand() * ra2 + ra3 * rand();

    if (ra > rand()*rand()) {
      ra*=rand();
    } else {ra -= (rand()*rand());};

    srand(ra);

    ra = rand() % 100 + 1;

    for (int i = 0; i < ra; i++) {
      srand(rand()+staticPrivateKey+inp);
      if (i%2==0) {inp+=rand()+staticPrivateKey;}
      else {inp-=rand();};
    };

    ss << inp;
    ss << convertPassword();
    ss >> str;
    ss.str("");
    
    srand(fmod(inp, staticPrivateKey));
    ra = rand() % 100 + 1;
    
    inp+=str.length();
    
    for (int i = 0; i < ra; i++) {
      srand(fmod(rand(), rand()) + staticPrivateKey);
      short n = rand() % str.length() + 1;
      inp -= (str.length() * (str[n]-'0')) - rand();
    };
  };

  //Бета-функция, еще не фулл реализована
  void AKARE::numShuffleAlg(short &inp1, short &inp2, short &inp3, short &inp4, short &inp5, short &inp6, short &inp7, short &inp8, size_t randomSeed, bool inverse) {
    //Создание безопасного случайного начального числа
    srand(randomSeed);
    sMod(randomSeed);
    randomSeed = randomSeed * convertPassword() + rand();
    srand(randomSeed); //Теперь применим его

    short usedNums[8] = {-1,-1,-1,-1,-1,-1,-1,-1};
    //Назначьте случайный порядок
    for (unsigned short int i = 0; i<8; i++) {
      usedNums[i] = rand() % 8;
      //cout << usedNums[i] << endl;
      for (int y=0; y<i; y++) {
        if (usedNums[i] == usedNums[y]) {i--; break;};
      };
    };
    //присваивание номеров
    inp1 = usedNums[0];
    inp2 = usedNums[1];
    inp3 = usedNums[2];
    inp4 = usedNums[3];
    inp5 = usedNums[4];
    inp6 = usedNums[5];
    inp7 = usedNums[6];
    inp8 = usedNums[7];

    //Обратность указывает на то, нужна ли вам последовательность для отмены перетасовки... если обратное значение равно true, то числа проходят через алгоритм отмены перетасовки, который затем предоставляет последовательность чисел для отмены перетасовки. Это позволяет приложению иметь только одну функцию для отмены перетасовки, следовательно, shuffle8....
    if (inverse == true) {numDeShuffleAlg(inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8);};
    };

  void AKARE::numDeShuffleAlg(short &num1, short &num2, short &num3, short &num4, short &num5, short &num6, short &num7, short &num8) {
    //Метод перебора перетасованных чисел методом перебора методом грубой силы. Требует больших затрат ресурсов процессора и удлиняет процесс... но, тем не менее, эффективен...

    //Цель для достижения конечного результата
    string str = "01234567";
    string str2;
    //Перетасуйте так, как если бы это было сделано в алгоритме.
    str2 = str[num1]; 
    str2 += str[num2];
    str2 += str[num3];
    str2 += str[num4];
    str2 += str[num5];
    str2 += str[num6];
    str2 += str[num7];
    str2 += str[num8];
    //объявление чисел
    short shuNums[8] {num1, num2, num3, num4, num5, num6, num7, num8};
    /*
    Просматриваем перетасованную строку и сравниваю ее со строкой, которой она должна быть. 
 Она содержит строку, которая должна быть в i, а затем проверяем перетасованную строку и, когда находим совпадение, добавляем ее в массив. 
 Итак, если str[i] = 0, мы начнем с str2 @ 0 и будем увеличивать это число до тех пор, пока не найдем совпадение, а затем последовательно сохраним это число, что расшифровать перетасовку.
    */
    for (int i=0; i<8; i++) {
      for (int y=0; y<8; y++) {
        if (str2[y] == str[i]) {
          shuNums[i] = y; break;
        };
      };
    };
    //Присвоение номеров
    num1 = shuNums[0];
    num2 = shuNums[1];
    num3 = shuNums[2];
    num4 = shuNums[3];
    num5 = shuNums[4];
    num6 = shuNums[5];
    num7 = shuNums[6];
    num8 = shuNums[7];
  };

  //Перетасовывает строки длиной 8 символов, порядок выбирается случайным образом на основе переменной realKey, а также времени ее зацикливания.
  string AKARE::shuffle8(string inp, size_t looper, size_t realKey, bool unShuffle) {
    //Объявляет переменные с числами в случайном порядке
    short num1=0, num2=0, num3=0, num4=0, num5=0, num6=0, num7=0, num8=0;
    //Это функция, которая фактически определяет последовательность перетасовки
    numShuffleAlg(num1, num2, num3, num4, num5, num6, num7, num8, realKey, unShuffle);
    
    if (inp.length() != 8) {return "ERR INVALID LENGTH";};
    for (size_t i = 0; i < looper; i++) {
      //Перетасовка строки
      string str;
      str = inp[num1];
      str += inp[num2];
      str += inp[num3];
      str += inp[num4];
      str += inp[num5];
      str += inp[num6];
      str += inp[num7];
      str += inp[num8];
      inp = str;
    };       
    return inp;
  };
  //Добавим многопоточность для шифрования amd devrypt для повышения производительности
  string AKARE::encrypt(string inp, size_t &key) {
    if (inp.length() != bounds::expectedLength) {return "ERR INVALID LENGTH";};
    
    string result;
    key = evenGenerator();
    size_t realKey = mod(key / staticPrivateKey) + mod(key) + convertPassword();
    sMod(realKey);
    
    srand(realKey);
    
    for (unsigned int i = 0; i < inp.length(); i+=bounds::bitsInByte) {
      const unsigned short looper = (rand() % 100 + 1) + AKARE::convertPassword();
      result += shuffle8(inp.substr(i, bounds::bitsInByte), looper, realKey);
    };
    
    return result;
  };

  string AKARE::decrypt(string inp, size_t key) {
    if (inp.length() != bounds::expectedLength) {return "ERR INVALID LENGTH";};
    
    string result;
    size_t realKey = mod(key / staticPrivateKey) + mod(key) + convertPassword();
    sMod(realKey);
    
    srand(realKey);
    
    for (unsigned int i = 0; i < inp.length(); i+=bounds::bitsInByte) {
      const unsigned short looper = (rand() % 100 + 1) + AKARE::convertPassword();
      result += shuffle8(inp.substr(i, bounds::bitsInByte), looper, realKey, true);
    };
    
    return result;
  };
};