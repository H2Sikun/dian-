#include "LibrarySystem.h"
#include <iostream>

using namespace std;
void printHelp() {
    cout << "\t\t -----------------------------------------------------------------------------------\n";
    cout << "\t\t|       === ͼ�����λԤԼϵͳ����ָ�� ===                                          |\n";
    cout << "\t\t|       ��¼���ָ��                                                                |\n";
    cout << "\t\t|       Login - ��¼                                                                |\n";
    cout << "\t\t|       Exit - �˳���ǰ��¼                                                         |\n";
    cout << "\t\t|       Quit - �˳�����                                                             |\n";
    cout << "\t\t|       Help - ����ָ��                                                             |\n";
    cout << "\t\t|       ԤԼ���ָ��:                                                               |\n";
    cout << "\t\t|    eg:1. �鿴ԤԼ���                                                             |\n";
    cout << "\t\t|       Monday Floor 1                                                              |\n";
    cout << "\t\t|       2. ԤԼ��λ                                                                 |\n";
    cout << "\t\t|       Reserve Monday Floor 1 Seat 1 2                                             |\n";
    cout << "\t\t|       3. Reservation - ��ʾԤԼ���                                               |\n";
    cout << "\t\t|       4. Clear - ����û�����(����Ա)                                             |\n";
    cout << "\t\t|       5. ԤԼ״̬                                                                 |\n";
    cout << "\t\t|       0 ��λ�� 1 ��λ��ԤԼ 2 ��λ���Լ�ԤԼ                                      |\n";
    cout << "\t\t|       6.ClearDay Monday - ����ĳһ������ԤԼ������Ա�����£�                      |\n";
    cout << "\t\t|       7.ShowDay Monday - �鿴ĳһ������ԤԼ������Ա�����£�                       |\n";
    cout << "\t\t|       8.Reservation - �鿴�����˵�ԤԼ������Ա�����£�                            |\n";
    cout << "\t\t|       9.AdminReserve A Monday Floor 1 Seat 2 3 - �����û�ԤԼ������Ա�����£�     |\n";
    cout << "\t\t -----------------------------------------------------------------------------------\n";
}

int main() {
    LibrarySystem system;
    printHelp();
    system.run();
    return 0;
}