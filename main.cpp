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

    // ���� ������� ����� ���������
    char szPath[] = ("Software\\Valve\\Steam");

    HKEY hKey;

    // ������� ���� � ����� HKEY_CURRENT_USER
    if((RegCreateKeyEx(HKEY_CURRENT_USER, szPath, 0, NULL, REG_OPTION_VOLATILE, KEY_WRITE, NULL, &hKey, NULL))){
    cout << "��� �������� ����� ��������� ������" << endl;
    return 1;
    }
    // ����� �������� ������ � ��������� ����
    if(RegSetValueEx(hKey, "AutoLoginUser", 0, REG_SZ, (BYTE*)szTestString, sizeof(szTestString))){
    cout << "��� ������ ������ ��������� ������" << endl;
    return 2;
    }

    // ��������� ��������� �����
    if(RegCloseKey(hKey) != ERROR_SUCCESS){
    cout << "��� �������� ����� ��������� ������" << endl;
    return 3;
    };

    char szBuf[MAX_PATH];
    DWORD dwBufLen = MAX_PATH;

    if(RegGetValue(HKEY_CURRENT_USER, szPath, "AutoLoginUser", RRF_RT_REG_SZ, NULL, (BYTE*) szBuf, &dwBufLen) != ERROR_SUCCESS){
    cout << "��� ������ ������ ��������� ������" << endl;
    return 4;
    }
    cout << "������� �����: " << szBuf << endl;
}


int main()
{
    // ������������� ������� ������� �������� ��� ������ ���������
    setlocale(LC_ALL, "Rus");
    // ������ ������� ����� ������ � ������

//    string szTestString_temp;// = "kriper_miper";
//    cout << "������� �����: ";
//    cin >> szTestString_temp;
//    ChangeAcc(szTestString_temp);

    string line;
    ifstream file("login.txt"); // �������� ���� ��� ������
    if (file.is_open())
    {
        int i = 0;
        cout << "�������: " << endl;
        while (getline(file, line))
        {
            i++;
            cout << i << " - " << line << endl;
        }
    }
    else
    {
        cout << "�� ��� ������ ���� login.txt, ���������� ������� ���� login.txt ����� � ����������� ������ � ������ ���� ���� ������, 1 ����� �� 1 �������. ������(������� ����� ���� ������ ��� 2): " << endl << "login_1\nlogin_2" << endl;
        cout << "���� � �������� ��� ������, ���������� �������� ������ �� ����." << endl;

        ofstream fout("login.txt");
        fout << "login_1" << endl; // ������ ������ � ����
        fout << "login_2" << endl; // ������ ������ � ����
        fout.close(); // ��������� ����
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
        cout << "��������� ������� - " << line << endl;
    }
    file.close();
    ChangeAcc(line);
    cout << "�������� ����..." << endl;
    system("taskkill /f /t /im steam.exe");
    cout << "10 ��� �����..." << endl;
    Sleep(10000);
    cout << "������ ����� �� C:\\Program Files (x86)\\Steam (���� ���� ��������� �� � ���� �����, ������� �� ����������)" << endl;
    system("start \"Steam.exe\" \"C:\\Program Files (x86)\\Steam\\Steam.exe\"");
    cout << "������� ��������� ����� 5 ������" << endl;
    Sleep(5000);

    return 0;
}
