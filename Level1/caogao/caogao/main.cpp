#include "seat_reservation.h"
#include <sstream>
#include <iostream>
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
    SeatReservationSystem system;
    string command;

    cout << "欢迎使用图书馆座位预约系统!" << endl;
    printHelp();

    while (true) {
        cout << "> ";
        getline(cin, command);

        if (command == "Quit" || command == "quit") {
            system.saveData();
            cout << "程序已退出!" << endl;
            break;
        }
        else if (command == "Help" || command == "help") {
            printHelp();
        }
        else if (command == "Login" || command == "login") {
            system.login();
        }
        else if (command == "Exit" || command == "exit") {
            system.logout();
        }
        else if (command == "Clear" || command == "clear") {
            system.clearAllData();
        }
        else if (command == "Reservation" || command == "reservation") {
            system.showUserReservations();
        }
        else if (command.find("Floor") != string::npos) {
            stringstream ss(command);
            string day, floorCmd;
            int floor;

            ss >> day >> floorCmd >> floor;

            if (floor >= 1 && floor <= 5) {
                system.showFloorSeats(day, floor);
            }
            else {
                cout << "楼层号应为1-5" << endl;
            }
        }
        else if (command.find("Reserve") == 0) {
            stringstream ss(command);
            string cmd, day, floorCmd, seatCmd;
            int floor, row, col;

            ss >> cmd >> day >> floorCmd >> floor >> seatCmd >> row >> col;

            if (floor >= 1 && floor <= 5 && row >= 1 && row <= 4 && col >= 1 && col <= 4) {
                system.reserveSeat(day, floor, row, col);
            }
            else {
                cout << "参数错误: 楼层1-5, 行列1-4" << endl;
            }
        }
        else {
            cout << "error，请重新输入" << endl;
        }
    }

    return 0;
}