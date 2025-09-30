#include "LibrarySystem.h"
#include <iostream>

using namespace std;
void printHelp() {
    cout << "\t\t --------------------------------------------------\n";
    cout << "\t\t|       === 图书馆座位预约系统操作指南 ===         |\n";
    cout << "\t\t|       登录相关指令                               |\n";
    cout << "\t\t|       Login - 登录                               |\n";
    cout << "\t\t|       Exit - 退出当前登录                        |\n";
    cout << "\t\t|       Quit - 退出程序                            |\n";
    cout << "\t\t|       Help - 操作指南                            |\n";
    cout << "\t\t|       预约相关指令:                              |\n";
    cout << "\t\t|    eg:1. 查看预约情况                            |\n";
    cout << "\t\t|       Monday Floor 1(Monday--Sunday,Floor 1--5)  |\n";
    cout << "\t\t|       2. 预约座位                                |\n";
    cout << "\t\t|       Reserve Monday Floor 1 Seat 1 2            |\n";
    cout << "\t\t|       3. Reservation - 显示预约结果              |\n";
    cout << "\t\t|       4. Clear - 清空用户数据(管理员)            |\n";
    cout << "\t\t|       5. 预约状态                                |\n";
    cout << "\t\t|       0 座位空 1 座位被预约 2 座位被自己预约     |\n";
    cout << "\t\t --------------------------------------------------\n";
}

int main() {
    LibrarySystem system;
    printHelp();
    system.run();
    return 0;
}