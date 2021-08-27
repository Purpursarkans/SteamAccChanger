#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <tlhelp32.h>
#include <cstdio>

using namespace std;

void killProcessByName(const char *filename);

void KillSteam()
{
    //change to steam -shutdown
    killProcessByName("steam.exe");
}
void KillSteamExtreme()
{
    killProcessByName("steam.exe");
    killProcessByName("steamwebhelper.exe");
    killProcessByName("SteamService.exe");
}

void killProcessByName(const char *filename)
{
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    PROCESSENTRY32 pEntry;
    pEntry.dwSize = sizeof (pEntry);
    BOOL hRes = Process32First(hSnapShot, &pEntry);
    while (hRes)
    {
        if (strcmp(pEntry.szExeFile, filename) == 0)
        {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0,
                                          (DWORD) pEntry.th32ProcessID);
            if (hProcess != NULL)
            {
                TerminateProcess(hProcess, 9);
                CloseHandle(hProcess);
            }
        }
        hRes = Process32Next(hSnapShot, &pEntry);
    }
    CloseHandle(hSnapShot);
}

DWORD GetProcessByExeName(wchar_t* ExeName)
{
PROCESSENTRY32W pe32;
pe32.dwSize = sizeof(PROCESSENTRY32W);

HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
if (hProcessSnap == INVALID_HANDLE_VALUE)
{
    MessageBoxW(NULL, L"Error CreateToolhelp32Snapshot", L"error", MB_OK);
    return false;
}

if (Process32FirstW(hProcessSnap, &pe32))
{
    do
    {
        if (_wcsicmp(pe32.szExeFile, ExeName) == 0)
        {
            CloseHandle(hProcessSnap);
            return pe32.th32ProcessID;
        }
    } while (Process32NextW(hProcessSnap, &pe32));
}

CloseHandle(hProcessSnap);
return 0;

}

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
    system("cls || clear");

    string line;
    ifstream file("login.txt"); // окрываем файл для чтения
    if (file.is_open())
    {
        int i = 0;
        cout << "0 - Настройки" << endl;
        cout << "Аккаунт: " << endl;
        while (getline(file, line))
        {
            i++;
            cout << i << " - " << line << endl;
        }
    }
    else
    {
        cout << "Не был найден файл login.txt, создание файла рядом с программой" << endl
             << "Необходимо добавить логины через настройки" << endl;

        ofstream fout("login.txt");
        fout << "template login" << endl;
        fout.close();
        system("pause");
        main();
    }
    file.close();


    int i;
    cout << ">> ";
    cin >> i;

    if (i == 0)
    {
        system("cls");
        cout << "Настройки:" << endl;
        cout << "0 - Меню" << endl;
        cout << "1 - Добавить аккаунт" << endl;
        cout << "2 - Удалить аккаунт" << endl;
        cout << ">> ";
        cin >> i;

        if(i == 0)
        {
            main();
        }
        else if(i == 1)
        {
            ofstream fout("login.txt", ios_base::app);
            system("cls");
            string acc;
            cout << "Добавление аккаунта:" << endl;
            cout << "Введите логин аккаунта или 0(ноль) для возврата назад:" << endl;

            cout << ">> ";
            cin >> acc;

            if(acc == "0")
            {
                main();
            }

            if(!(fout << acc << endl))
            {
                cout << "Ошибка добавления логина" << endl;
                system("pause");
                fout.close();
            }

            cout << "Добавлен аккаунт: " << acc << endl;
            fout.close();
        }
        else if (i == 2)
        {
            system("cls");
            cout << "Удаление аккаунта:" << endl;

            cout << "0 - Меню" << endl;

            string acc;
            string delacc;
            int accdel;


            file.open("login.txt");

            for (int chetchik = 1; getline(file, acc); chetchik++)
            {
                cout << chetchik << " - " << acc << endl;
            }
            file.close();

            file.open("login.txt");

            cout << ">> ";
            cin >> accdel;

            if (accdel == 0)
            {
                main();
            }

            for(int j = 0; j < accdel; j++)
            {
                getline(file, delacc);
            }
            file.close();

            file.open("login.txt");
            ofstream fout("login2.txt");

            for (int chetchik = 1; getline(file, acc); chetchik++)
            {
                if(chetchik == accdel) { continue; }
                fout << acc << endl;
            }

            file.close();
            fout.close();

            fout.open("login.txt");
            file.open("login2.txt");

            for (int chetchik = 1; getline(file, acc); chetchik++)
            {
                fout << acc << endl;
            }

            file.close();
            fout.close();

            cout << "Удален аккаунт: " << delacc << endl;

            if(!(remove( "login2.txt" )))
            {
                cout << "Ошибка удаления файла" << endl;
            }

            else
            {
                cout << "Файл успешно удалён" << endl;
            }
        }
        main();
    }


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

    int timerT = 0;

    if (GetProcessByExeName(L"steam.exe") != 0)
    {
        cout << "Найден запущенный стим" << endl;
        cout << "Завершение стима" << endl;
        KillSteam();

        Sleep(3000);

        while((GetProcessByExeName(L"steam.exe") != 0) || (GetProcessByExeName(L"steamwebhelper.exe") != 0) || (GetProcessByExeName(L"SteamService.exe") != 0))
        {

            cout << "Стим завершается слишком долго... Повторная попытка закрытия стима" << endl;
            KillSteamExtreme();
            timerT++;
            if(timerT > 10)
            {
                cout << "Стим не может закрыться слишком долго, попробуйте закрыть его вручную.\nЕсли стим закрыт, но ничего не происходит - отставьте багрепорт с указанием незакрытых служб стима на\n>>> github.com/Purpursarkans/SteamAccChanger/issues" << endl << endl;
                system("pause");
                return 0;
            }
            Sleep(1000);
        }
    }

    cout << "Запуск стима из C:\\Program Files (x86)\\Steam (если стим находится не в этой папке, запуска не произойдет)" << endl;
    system("start \"Steam.exe\" \"C:\\Program Files (x86)\\Steam\\Steam.exe\"");
    Sleep(3000);
    ShowWindow(GetConsoleWindow(), SW_MINIMIZE);
    cout << "Консоль закроется через 3 секунды" << endl;
    Sleep(3000);


    return 0;
}
