#include "LibrarySystem.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
using namespace std;

// Seat 结构体实现
Seat::Seat(int r, int c, int d, int f) : row(r), col(c), status(FREE), reservedBy(' '), day(d), floor(f) {}

// Reservation 结构体实现
Reservation::Reservation(string u, int d, int f, int r, int c) : user(u), day(d), floor(f), row(r), col(c) {}

// LibrarySystem 类实现
LibrarySystem::LibrarySystem() : isLoggedIn(false) {
    // 初始化日期映射
    dayMap = {
        {"Sunday", 0}, {"Monday", 1}, {"Tuesday", 2}, {"Wednesday", 3},
        {"Thursday", 4}, {"Friday", 5}, {"Saturday", 6}
    };

    initializeLibrary();
    loadData();
}

LibrarySystem::~LibrarySystem() {
    saveData();
}

void LibrarySystem::initializeLibrary() {
    library.resize(DAYS, vector<vector<vector<Seat>>>(FLOORS,
        vector<vector<Seat>>(ROWS, vector<Seat>(COLS, Seat(0, 0, 0, 0)))));

    for (int day = 0; day < DAYS; day++) {
        for (int floor = 0; floor < FLOORS; floor++) {
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    library[day][floor][row][col] = Seat(row, col, day, floor);
                }
            }
        }
    }
}

void LibrarySystem::loadData() {
    // 加载座位数据
    ifstream seatFile(DATA_FILE, ios::binary);//以二进制形式解析
    if (seatFile) {
        for (int day = 0; day < DAYS; day++) {
            for (int floor = 0; floor < FLOORS; floor++) {
                for (int row = 0; row < ROWS; row++) {
                    for (int col = 0; col < COLS; col++) {
                        seatFile.read(reinterpret_cast<char*>(&library[day][floor][row][col].status), sizeof(char));//read后应接char类型
                        seatFile.read(reinterpret_cast<char*>(&library[day][floor][row][col].reservedBy), sizeof(char));
                    }
                }
            }
        }
        seatFile.close();
    }

    // 加载预约记录
    ifstream resFile(RESERVATION_FILE);
    if (resFile) {
        string user;
        int day, floor, row, col;
        while (resFile >> user >> day >> floor >> row >> col) {
            reservations.push_back(Reservation(user, day, floor, row, col));
        }
        resFile.close();
    }
}

void LibrarySystem::saveData() {
    // 保存座位数据
    ofstream seatFile(DATA_FILE, ios::binary);
    if (seatFile) {
        for (int day = 0; day < DAYS; day++) {
            for (int floor = 0; floor < FLOORS; floor++) {
                for (int row = 0; row < ROWS; row++) {
                    for (int col = 0; col < COLS; col++) {
                        seatFile.write(reinterpret_cast<const char*>(&library[day][floor][row][col].status), sizeof(char));
                        seatFile.write(reinterpret_cast<const char*>(&library[day][floor][row][col].reservedBy), sizeof(char));
                    }
                }
            }
        }
        seatFile.close();
    }

    // 保存预约记录
    ofstream resFile(RESERVATION_FILE);
    if (resFile) {
        for (const auto& res : reservations) {
            resFile << res.user << " " << res.day << " " << res.floor << " " << res.row << " " << res.col << endl;
        }
        resFile.close();
    }
}

void LibrarySystem::updateSeatStatus() {
    // 根据当前用户更新座位显示状态
    for (int day = 0; day < DAYS; day++) {
        for (int floor = 0; floor < FLOORS; floor++) {
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    Seat& seat = library[day][floor][row][col];
                    if (seat.status != FREE) {
                        if (currentUser == "Admin") {
                            // 管理员看到实际预约者
                            seat.status = seat.reservedBy;
                        }
                        else if (seat.reservedBy == currentUser[0]) {
                            // 当前用户自己的预约
                            seat.status = CURRENT_USER_RESERVED;
                        }
                        else {
                            // 其他用户的预约
                            seat.status = RESERVED;
                        }
                    }
                }
            }
        }
    }
}

bool LibrarySystem::isValidUser(const string& user) {
    if (user == "Admin") return true;
    if (user.length() == 1 && user[0] >= 'A' && user[0] <= 'Z') return true;
    return false;
}

void LibrarySystem::login() {
    string username;
    cout << "请输入用户名: ";
    cin >> username;

    // 清除输入缓冲区中的换行符，防止用户输入A后显示">A>A"而不是“>A”
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (isValidUser(username)) {
        currentUser = username;
        isLoggedIn = true;
        updateSeatStatus();
        cout << "登录成功" << endl;
    }
    else {
        cout << "无效的用户名，请输入A-Z的单个字母或Admin" << endl;
    }
}

void LibrarySystem::logout() {
    if (isLoggedIn) {
        cout << "用户 " << currentUser << " 已退出登录" << endl;
        currentUser = "";
        isLoggedIn = false;
    }
    else {
        cout << "当前未登录" << endl;
    }
}

void LibrarySystem::showFloor(const string& dayStr, int floor) {
    if (!isLoggedIn) {
        cout << "请先登录" << endl;
        return;
    }

    if (dayMap.find(dayStr) == dayMap.end()) {
        cout << "无效的日期，请输入正确的日期（如Monday）" << endl;
        return;
    }

    if (floor < 1 || floor > FLOORS) {
        cout << "无效的楼层，请输入1-5" << endl;
        return;
    }

    int day = dayMap[dayStr];
    int floorIndex = floor - 1;//数组从0开始而直观的楼层起始为1

    cout << dayStr << " Floor " << floor << " 的座位情况:" << endl;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            cout << library[day][floorIndex][row][col].status;
        }
        cout << endl;
    }
}

void LibrarySystem::reserveSeat(const string& dayStr, int floor, int row, int col) {
    if (!isLoggedIn) {
        cout << "请先登录" << endl;
        return;
    }

    if (currentUser == "Admin") {
        cout << "管理员不能预约座位" << endl;
        return;
    }

    if (dayMap.find(dayStr) == dayMap.end()) {
        cout << "无效的日期" << endl;
        return;
    }

    if (floor < 1 || floor > FLOORS || row < 1 || row > ROWS || col < 1 || col > COLS) {
        cout << "无效的座位位置" << endl;
        return;
    }

    int day = dayMap[dayStr];//将星期的英文映射为数字放入数组
    int floorIndex = floor - 1;
    int rowIndex = row - 1;
    int colIndex = col - 1;

    Seat& seat = library[day][floorIndex][rowIndex][colIndex];
    if (seat.status == FREE) {
        seat.reservedBy = currentUser[0];
        seat.status = CURRENT_USER_RESERVED;
        reservations.push_back(Reservation(currentUser, day, floorIndex, rowIndex, colIndex));
        saveData();
        cout << "OK" << endl;
    }
    else {
        cout << "该座位已被预约" << endl;
    }
}

void LibrarySystem::showReservations() {
    if (!isLoggedIn) {
        cout << "请先登录" << endl;
        return;
    }

    if (currentUser == "Admin") {
        cout << "所有预约记录:" << endl;
        for (const auto& res : reservations) {
            string dayStr;
            for (const auto& pair : dayMap) {
                if (pair.second == res.day) {
                    dayStr = pair.first;
                    break;
                }
            }
            cout << dayStr << " Floor " << (res.floor + 1) << " Seat " << (res.row + 1) << " " << (res.col + 1)
                << " (用户: " << res.user << ")" << endl;
        }
    }
    else {
        cout << "当前用户的预约:" << endl;
        bool hasReservation = false;
        for (const auto& res : reservations) {
            if (res.user == currentUser) {
                string dayStr;
                for (const auto& pair : dayMap) {
                    if (pair.second == res.day) {
                        dayStr = pair.first;
                        break;
                    }
                }
                cout << dayStr << " Floor " << (res.floor + 1) << " Seat " << (res.row + 1) << " " << (res.col + 1) << endl;
                hasReservation = true;
            }
        }
        if (!hasReservation) {
            cout << "暂无预约" << endl;
        }
    }
}

void LibrarySystem::clearData() {
    if (!isLoggedIn) {
        cout << "请先登录" << endl;
        return;
    }

    if (currentUser != "Admin") {
        cout << "只有管理员可以清空数据" << endl;
        return;
    }

    // 重置所有座位状态
    for (int day = 0; day < DAYS; day++) {
        for (int floor = 0; floor < FLOORS; floor++) {
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    library[day][floor][row][col].status = FREE;
                    library[day][floor][row][col].reservedBy = ' ';
                }
            }
        }
    }

    // 清空预约记录
    reservations.clear();

    // 删除数据文件
    remove(DATA_FILE.c_str());
    remove(RESERVATION_FILE.c_str());

    cout << "数据已清空" << endl;
}

void LibrarySystem::run() {
    
    string command;
    while (true) {
        if (isLoggedIn) {
            cout << currentUser << "> ";
        }
        else {
            cout << "> ";
        }

        // 读取完整的一行来处理命令
        string line;
        getline(cin, line);

        if (line.empty()) {
            continue; // 如果是空行，重新显示提示符
        }

        // 使用字符串流解析命令，只读
        istringstream iss(line);
        iss >> command;

        if (command == "Login") {
            login();
        }
        else if (command == "Exit") {
            logout();
        }
        else if (command == "Quit") {
            cout << "再见!" << endl;
            break;
        }
        else if (isLoggedIn) {
            // 登录后的命令
            if (command == "Reservation") {
                showReservations();
            }
            else if (command == "Clear") {
                clearData();
            }
            else if (command == "Reserve") {
                // 处理预约命令
                string dayStr, floorCmd, seatCmd;
                int floor, row, col;
                if (iss >> dayStr >> floorCmd >> floor >> seatCmd >> row >> col) {
                    if (floorCmd == "Floor" && seatCmd == "Seat") {
                        reserveSeat(dayStr, floor, row, col);
                    }
                    else {
                        cout << "error" << endl;
                    }
                }
                else {
                    cout << "error" << endl;
                }
            }
            else {
                // 处理日期楼层查询命令
                string dayStr = command;
                string floorCmd;
                int floor;

                if (iss >> floorCmd >> floor) {
                    if (floorCmd == "Floor") {
                        showFloor(dayStr, floor);
                    }
                    else {
                        cout << "error" << endl;
                    }
                }
                else {
                    cout << "error" << endl;
                }
            }
        }
        else {
            cout << "error,未知命令或请先登录" << endl;
        }
    }
}