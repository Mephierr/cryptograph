#include "Headers/includes.h"

// 1
void aesExample() {
	string actual_string;
	AES::aes_init(AES::OPTIONS::doGenerateKey); 

	// ключ вывода
	cout << "Key: ";
	for (int i = 0; i < sizeof(AES::KEY::key) / sizeof(AES::KEY::key[0]); ++i) {
		cout << AES::KEY::key[i] << " ";
	};
	cout << endl;

	cout << "Введите то, что вы хотите зашифровать:" << endl;
	cin.ignore();
	getline(cin >> noskipws, actual_string);

	cout << endl
		 << "Текст, подлежащий шифрованию:" << endl
		 << actual_string << endl;

	// Encryption, output ciphertext
	actual_string = AES::encrypt(actual_string);
	cout << "Зашифрованный текст:" << endl << actual_string << endl;
	// Decrypt, output plaintext
	actual_string = AES::decrypt(actual_string);
	cout << "Расшифрованный текст:" << endl << actual_string << endl;
	cout << "Нажмите любую клавишу, дабы продолжить" << endl;
	cin.ignore();
	return;
};

// 2
void textFromFileExample() {
	string input;
	cout << "Введите путь до файла: " << endl;
	cin.ignore();
	cin >> input;
	if (!AES::encryptFF(input)) {
		return;
	}; // выводит false если путь неверен
	ifstream readfile(input);
	string line;
	cout << "Зашифрованное: " << endl;
	while (getline(readfile, line)) {
		cout << line << endl;
	};
	readfile.close();
	if (!AES::decryptFF(input)) {
		return;
	};
	readfile.open(input);
	cout << "Расшифрованное: " << endl;
	while (getline(readfile, line)) {
		cout << line << endl;
	};
	readfile.close();
};

// 3
inline void wholeFileEncryptionExample() {
	AES::aes_init(AES::OPTIONS::doGenerateKey);

	cout << "Сгенерированный ключ: ";
	for (int i = 0; i < sizeof(AES::KEY::key) / sizeof(AES::KEY::key[0]); ++i) {
		cout << AES::KEY::key[i] << " ";
	};
	cout << endl << endl << endl;

  string pass;
  printf("Введите пароль\n");
	cin.ignore();
	cin >> pass;
  
  
	string input;
	printf(
		"Введите путь до файла (см. папку testFiles): \n");
	cin.ignore();
	cin >> input;

	if (!AES::encryptFile(input, pass)) {
		return;
	}; // выводит false при плохом пути до файла

	printf("Введите новый путь к файлу .aesenc\n");
	cin.ignore();
	string newInput;
	cin >> newInput;
  
  printf("Введите ваш пароль\n");
	cin.ignore();
	cin >> pass;
  
	printf("Зашифрован\nРасшифровка...\n");
	thread dec(
		AES::decryptFile,
		ref(newInput),
    ref(pass),
		"",
		(AES::FILE_FLAGS)(
			AES::FILE_FLAGS::deleteAesencFile |
			AES::FILE_FLAGS::deleteKeyFile));
	dec.join();
};

// 5
void aes_debug() {  
  cout << "AES SECTION" << endl;
	cout << "Тестирование функций файла" << endl << endl;
	cout << "1mb" << endl;

  cout << "Считывание" << endl;
	ifstream file("testFiles/1mb/1mb.txt");
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);
	char *buffer = new char[length];
	file.read(buffer, length);
	string text = buffer;
	delete[] buffer;
	file.close();
  cout << "Тестирование" << endl;

  cout << "Инициализация" << endl;
	AES::aes_init(AES::OPTIONS::doGenerateKey);
  cout << "Шифровка" << endl;
	AES::encryptFile("testFiles/1mb/1mb.txt", "test");
  cout << "Расшифровка" << endl;
	AES::decryptFile(
		"testFiles/1mb/1mb.aesenc",
    "test",
		"",
		(AES::FILE_FLAGS)(
			AES::FILE_FLAGS::deleteAesencFile |
			AES::FILE_FLAGS::deleteKeyFile));

  cout << "Сравнение" << endl;
	file.open("testFiles/1mb/1mb.txt");
	file.seekg(0, file.end);
	length = file.tellg();
	file.seekg(0, file.beg);
	buffer = new char[length];
	file.read(buffer, length);
	string text2 = buffer;
	delete[] buffer;
	file.close();

	if (text == text2) {
		cout << "Test 1 Успех" << endl;
	} else {
    for (long int i = 0; i < text.length(); i++) {
      cout << text[i] << " | " << text2[i] << " | " << (text[i] == text2[i]) << endl;
      if (text[i] != text2[i]) {break;};
    };
		cout << "Test 1 Провал" << endl;
	};

	cout << "Тестирование шифрования & дешифрования" << endl;
	text = "это тест";
	text2 = AES::encrypt(text);
	text2 = AES::decrypt(text2);

	if (text == text2) {
		cout << "Test 2 Успех" << endl;
	} else {
		cout << "Test 2 Провал" << endl;
	};

  cout << "Тестирование растрового изображения, проверьте изображение" << endl;
  AES::encryptFile("testFiles/bitmap/img.bmp", "test");
  AES::decryptFile("testFiles/bitmap/img.aesenc", "test", "", (AES::FILE_FLAGS)(AES::FILE_FLAGS::deleteAesencFile | AES::FILE_FLAGS::deleteKeyFile));
  cout << "заканчиваю" << endl;
};

string text[6] = {
	"AES = 1",
	"AES текст из файла = 2",
	"AES шифрование всего файла = 3",
  "Список всех файлов = 4",
	"AES Дебаг = 5",
	"Введите номер желаемого действия"};


int main() {   
  
  cout << "\n##########################\n"
    << "\nЧто вы хотите использовать?" << endl;

	cout << text[0] << endl;
	cout << text[1] << endl;
	cout << text[2] << endl;
	cout << text[3] << endl;
	cout << text[4] << endl;
	cout << text[5] << endl;

	char input;
	cin >> input;
	string files, folders;
	switch (input) {
	case '1':
		aesExample();
		break;
	case '2':
		textFromFileExample();
		break;
	case '3':
		wholeFileEncryptionExample();
		break;
	case '4':
		FileOP::searchDir((char *)"testFiles/", files, folders, true);
		cout << " ------ " << endl << files << endl << " ------ " << endl;
		break;
	case '5':
		aes_debug();
		break;

	default:
		cout << "Ошибка, введите 1, 2, 3, 4, 5, 6, or 7" << endl;
		return main();
	};
	return main();
};