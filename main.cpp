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

    // Êëþ÷ êîòîðûé áóäåì ñîçäàâàòü
    char szPath[] = ("Software\\Valve\\Steam");

    HKEY hKey;

    // Ñîçäàåì êëþ÷ â âåòêå HKEY_CURRENT_USER
    if((RegCreateKeyEx(HKEY_CURRENT_USER, szPath, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, NULL))){
    cout << "Ïðè ñîçäàíèè êëþ÷à ïðîèçîøëà îøèáêà" << endl;
    return 1;
    }
    // Ïèøåì òåñòîâóþ ñòðîêó â ñîçäàííûé êëþ÷
    if(RegSetValueEx(hKey, "AutoLoginUser", 0, REG_SZ, (BYTE*)szTestString, sizeof(szTestString))){
    cout << "Ïðè çàïèñè ñòðîêè ïðîèçîøëà îøèáêà" << endl;
    return 2;
    }

    // Çàêðûâàåì îïèñàòåëü êëþ÷à
    if(RegCloseKey(hKey) != ERROR_SUCCESS){
    cout << "Ïðè çàêðûòèè êëþ÷à ïðîèçîøëà îøèáêà" << endl;
    return 3;
    };

    char szBuf[MAX_PATH];
    DWORD dwBufLen = MAX_PATH;

    if(RegGetValue(HKEY_CURRENT_USER, szPath, "AutoLoginUser", RRF_RT_REG_SZ, NULL, (BYTE*) szBuf, &dwBufLen) != ERROR_SUCCESS){
    cout << "Ïðè ÷òåíèè ñòðîêè ïðîèçîøëà îøèáêà" << endl;
    return 4;
    }
    cout << "Òåêóùèé ëîãèí: " << szBuf << endl;
}


int main()
{
    // Óñòàíàâëèâàåì ðóññêóþ êîäîâóþ ñòðàíèöó äëÿ âûâîäà êèðèëëèöû
    setlocale(LC_ALL, "Rus");
    // Ñòðîêà êîòîðóþ áóäåì ïèñàòü â ðååñòð

    string line;
    ifstream file("login.txt"); // îêðûâàåì ôàéë äëÿ ÷òåíèÿ
    if (file.is_open())
    {
        int i = 0;
        cout << "Àêêàóíò: " << endl;
        while (getline(file, line))
        {
            i++;
            cout << i << " - " << line << endl;
        }
    }
    else
    {
        cout << "Íå áûë íàéäåí ôàéë login.txt, íåîáõîäèìî ñîçäàòü ôàéë login.txt ðÿäîì ñ èñïîëíÿåìûì ôàéëîì è âíåñòè òóäà ñâîè ëîãèíû, 1 ëîãèí íà 1 ñòðî÷êå. Ïðèìåð(ëîãèíîâ ìîæåò áûòü áîëüøå ÷åì 2): " << endl << "login_1\nlogin_2" << endl;
        cout << "Ôàéë ñ ïðèìåðîì áûë ñîçäàí, íåîáõîäèìî çàìåíèòü ëîãèíû íà ñâîè." << endl;

        ofstream fout("login.txt");
        fout << "login_1" << endl; // çàïèñü ñòðîêè â ôàéë
        fout << "login_2" << endl; // çàïèñü ñòðîêè â ôàéë
        fout.close(); // çàêðûâàåì ôàéë
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
        cout << "Âûáðàííûé àêêàóíò - " << line << endl;
    }
    file.close();
    ChangeAcc(line);
    cout << "Çàâåðøàþ ñòèì..." << endl;
    system("taskkill /f /t /im steam.exe");
    cout << "10 ñåê ïàóçà..." << endl;
    Sleep(10000);
    cout << "Çàïóñê ñòèìà èç C:\\Program Files (x86)\\Steam (åñëè ñòèì íàõîäèòñÿ íå â ýòîé ïàïêå, çàïóñêà íå ïðîèçîéäåò)" << endl;
    system("start \"Steam.exe\" \"C:\\Program Files (x86)\\Steam\\Steam.exe\"");
    cout << "Êîíñîëü çàêðîåòñÿ ÷åðåç 5 ñåêóíä" << endl;
    Sleep(5000);

    return 0;
}
