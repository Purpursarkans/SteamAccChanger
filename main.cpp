#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>

using namespace std;

int ChangeAcc(string szTestString_temp)
{
    int n = szTestString_temp.length();
    char szTestString[n + 1];
    strcpy(szTestString, szTestString_temp.c_str());

    // Ключ который будем создавать
    char szPath[] = ("Software\\Valve\\Steam");

    HKEY hKey;

    // Создаем ключ в ветке HKEY_CURRENT_USER
    if((RegCreateKeyEx(HKEY_CURRENT_USER, szPath, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, NULL))){
    cout << "При создании ключа произошла ошибка" << endl;
    return 1;
    }
    // Пишем тестовую строку в созданный ключ
    if(RegSetValueEx(hKey, "AutoLoginUser", 0, REG_SZ, (BYTE*)szTestString, sizeof(szTestString))){
    cout << "При записи строки произошла ошибка" << endl;
    return 2;
    }

    // Закрываем описатель ключа
    if(RegCloseKey(hKey) != ERROR_SUCCESS){
    cout << "При закрытии ключа произошла ошибка" << endl;
    return 3;
    };

    char szBuf[MAX_PATH];
    DWORD dwBufLen = MAX_PATH;

    if(RegGetValue(HKEY_CURRENT_USER, szPath, "AutoLoginUser", RRF_RT_REG_SZ, NULL, (BYTE*) szBuf, &dwBufLen) != ERROR_SUCCESS){
    cout << "При чтении строки произошла ошибка" << endl;
    return 4;
    }
    cout << "Текущий логин: " << szBuf << endl;
}


int main()
{
    system("chcp 65001");
    system("cls");
    // Устанавливаем русскую кодовую страницу для вывода кириллицы
    //setlocale(LC_ALL, "Rus");
    // Строка которую будем писать в реестр

    string line;
    ifstream file("login.txt"); // окрываем файл для чтения
    if (file.is_open())
    {
        int i = 0;
        cout << "Аккаунт: " << endl;
        while (getline(file, line))
        {
            i++;
            cout << i << " - " << line << endl;
        }
    }
    else
    {
        cout << "Не был найден файл login.txt, необходимо создать файл login.txt рядом с исполняемым файлом и внести туда свои логины, 1 логин на 1 строчке. Пример(логинов может быть больше чем 2): " << endl << "login_1\nlogin_2" << endl;
        cout << "Файл с примером был создан, необходимо заменить логины на свои." << endl;

        ofstream fout("login.txt");
        fout << "login_1" << endl; // запись строки в файл
        fout << "login_2" << endl; // запись строки в файл
        fout.close(); // закрываем файл
        system("pause");
        return 0;
    }
    int i;
    cout << ">> ";
    cin >> i;

    file.close();
    file.open("login.txt");
    if (file.is_open())
    {
        for(int j = 0; j < i; j++)
        {
            getline(file, line);
        }
        cout << "Выбранный аккаунт - " << line << endl;
    }
    file.close();
    ChangeAcc(line);
    cout << "Завершаю стим..." << endl;
    system("taskkill /f /t /im steam.exe");
    cout << "10 сек пауза..." << endl;
    Sleep(10000);
    cout << "Запуск стима из C:\\Program Files (x86)\\Steam (если стим находится не в этой папке, запуска не произойдет)" << endl;
    system("start \"Steam.exe\" \"C:\\Program Files (x86)\\Steam\\Steam.exe\"");
    ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
    cout << "Консоль закроется через 5 секунд" << endl;
    Sleep(5000);

    return 0;
}
