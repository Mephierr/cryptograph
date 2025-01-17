#include "KeyFileEncryption_Includes.h"

namespace asymmetric {
  //Asymmetric
  //Key File
  //Abridged
  //RSA Ripoff
  //Encryption
  class AKARE {
    public:
    static string encrypt(string inp, size_t &key);
    static string decrypt(string inp, size_t key);


    static string password_key;
    private:
    //Измени это значение на другое, нестандартное, четное число для использования
    // (просто используйте evenGenerator, но измените границы, чтобы ваш max не был абсурдным, а ваш диапазон был достаточно широким)
    static constexpr size_t staticPrivateKey = 1417395678;
    //Формула - sMod(mod(publicKey / privateKey) + mod(publicKey)) + convertPassword();

    struct bounds {
      private:
      //Изменять это значение не рекомендуется
      static constexpr unsigned short multiplier = 2;

      public:
      static constexpr size_t genMin = asymmetric::AKARE::staticPrivateKey*multiplier;
      static constexpr size_t genMax = 99999999999999;
      static constexpr unsigned short expectedLength = 128;
      static constexpr unsigned short bitsInByte = 8;
    };


//Вручную установите для них значения от 0 до 65535
//Эти функции похожи на статический закрытый ключ, но они просто добавляют больше случайности
//к функциям, которые повысят безопасность алгоритма, даже если злоумышленник получит доступ к
//заголовку и алгоритму.
    struct entropy {
      public: 
      static constexpr unsigned short Val1 = 10048;
      static constexpr unsigned short Val2 = 65239;
      static constexpr unsigned short Val3 = 21386;
      static constexpr unsigned short Val4 = 5783;
    };

    static void sMod(size_t &inp);
    static size_t mod(size_t inp);
    static size_t convertPassword();

    static bool isDecimal(float inp);
    static bool isValidNum(size_t num);

    static size_t evenGeneratorBackend(size_t min, size_t max, size_t loop=0);

    static size_t evenGenerator(size_t min = bounds::genMin, size_t max = bounds::genMax);

    static void numShuffleAlg(short &inp1, short &inp2, short &inp3, short &inp4, short &inp5, short &inp6, short &inp7, short &inp8, size_t randomseed, bool inverse=false);

    static void numDeShuffleAlg(short &num1, short &num2, short &num3, short &num4, short &num5, short &num6, short &num7, short &num8);

    //Перемешивает бит каждого ключевого байта
    static string shuffle8(string inp, size_t looper, size_t realKey, bool unShuffle = false);
  };
};