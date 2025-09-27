#include "seat_reservation.h"
#include <sstream>

void printHelp(bool isAdmin = false) {
    if (isAdmin) {
        cout << "\t\t ---------------------------------------------------------\n";
        cout << "\t\t|       === 图书馆座位预约系统操作指南(管理员) ===        |\n";
        cout << "\t\t|       Monday Floor n - 查看周n第n层座位                 |\n";
        cout << "\t\t|       CancelDay Monday - 取消周n所有预约               |\n";
        cout << "\t\t|       AddSeat n - 为第n层增加一排座位                   |\n";
        cout << "\t\t|       RemoveSeat n m k - 删除第n层座位(m行k列)          |\n";
        cout << "\t\t|       Reservation - 查看所有预约                        |\n";
        cout << "\t\t|       Clear - 清空所有数据                              |\n";
        cout << "\t\t|       Exit - 退出登录                                   |\n";
        cout << "\t\t|       Help - 显示指南                                   |\n";
        cout << "\t\t ---------------------------------------------------------\n";
    }
    else {
        cout << "\t\t ---------------------------------------------------------\n";
        cout << "\t\t|       === 图书馆座位预约系统操作指南(学生) ===          |\n";
        cout << "\t\t|       Reserve Monday Floor n Seat m k - 预约座位        |\n";
        cout << "\t\t|       Reservation - 查看我的预约                        |\n";
        cout << "\t\t|       Monday Floor n - 查看周n第n层座位                 |\n";
        cout << "\t\t|       Exit - 退出登录                                   |\n";
        cout << "\t\t|       Help - 显示指南                                   |\n";
        cout << "\t\t ---------------------------------------------------------\n";
    }
}

int main() {
    SeatReservationSystem system;
    string command;

    cout << "\t\t --------------------------------------------------\n";
    cout << "\t\t|       === 图书馆座位预约系统操作指南 ===         |\n";
    cout << "\t\t|       登录相关指令                               |\n";
    cout << "\t\t|       Login - 登录   (管理员用户名为：Admin)     |\n";
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

    while (true) {
        cout << "> ";
        getline(cin, command);

        if (command.empty()) continue;

        if (command == "Quit" || command == "quit") {
            system.saveData();
            cout << "再见!" << endl;
            break;
        }
        else if (command == "Help" || command == "help") {
            // 这里需要判断当前是否是管理员状态
            // 简化处理，显示通用帮助
            cout << "通用指令: Login, Quit, Help" << endl;
        }
        else if (command == "Login" || command == "login") {
            system.login();
        }
        else if (command.find("Floor") != string::npos) {
            stringstream ss(command);
            string day, floorCmd;
            int floor;
            ss >> day >> floorCmd >> floor;
            system.showFloorSeats(day, floor);
        }
        else if (command.find("Reserve") == 0) {
            stringstream ss(command);
            string cmd, day, floorCmd, seatCmd;
            int floor, row, col;
            ss >> cmd >> day >> floorCmd >> floor >> seatCmd >> row >> col;
            system.reserveSeat(day, floor, row, col);
        }
        else if (command.find("CancelDay") == 0) {
            stringstream ss(command);
            string cmd, day;
            ss >> cmd >> day;
            system.cancelDayReservations(day);
        }
        else if (command.find("AddSeat") == 0) {
            stringstream ss(command);
            string cmd;
            int floor;
            ss >> cmd >> floor;
            system.addSeat(floor);
        }
        else if (command.find("RemoveSeat") == 0) {
            stringstream ss(command);
            string cmd;
            int floor, row, col;
            ss >> cmd >> floor >> row >> col;
            system.removeSeat(floor, row, col);
        }
        else if (command == "Reservation" || command == "reservation") {
            system.showUserReservations();
        }
        else if (command == "Clear" || command == "clear") {
            system.clearAllData();
        }
        else if (command == "Exit" || command == "exit") {
            system.logout();
        }
        else {
            cout << "error-" << endl;
        }
    }

    return 0;
}