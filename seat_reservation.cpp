#include "seat_reservation.h"

SeatReservationSystem::SeatReservationSystem() : currentUser(""), isAdmin(false), isLoggedIn(false) {
    initializeSeatData();
    loadData();
}

void SeatReservationSystem::initializeSeatData() {
    // 初始化7天的数据，5层楼，每层4x4座位
    for (const auto& day : days) {
        for (int floor = 1; floor <= 5; floor++) {
            vector<vector<char>> floorSeats(4, vector<char>(4, AVAILABLE));
            seatData[day][floor] = floorSeats;
        }
    }
}

bool SeatReservationSystem::isValidUser(const string& user) {
    if (user == "Admin") return true;
    if (user.length() == 1 && user[0] >= 'A' && user[0] <= 'Z') return true;
    return false;
}

bool SeatReservationSystem::isValidDay(const string& day) {
    for (const auto& validDay : days) {
        if (validDay == day) return true;
    }
    return false;
}

void SeatReservationSystem::login() {
    if (isLoggedIn) {
        cout << "请先退出当前登录" << endl;
        return;
    }

    cout << "请输入用户名: ";
    string username;
    cin >> username;

    // 清空输入缓冲区
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (!isValidUser(username)) {
        cout << "无效的用户名，请输入A-Z或Admin" << endl;
        return;
    }

    currentUser = username;
    isAdmin = (username == "Admin");
    isLoggedIn = true;

    cout << "登录成功" << endl;
}
void SeatReservationSystem::logout() {
    if (!isLoggedIn) {
        cout << "当前未登录" << endl;
        return;
    }

    saveData(); // 退出前保存数据
    currentUser = "";
    isAdmin = false;
    isLoggedIn = false;
    cout << "退出登录成功" << endl;
}

bool SeatReservationSystem::checkLogin() {
    if (!isLoggedIn) {
        cout << "请先登录" << endl;
        return false;
    }
    return true;
}

void SeatReservationSystem::showFloorSeats(const string& day, int floor) {
    if (!checkLogin()) return;

    if (!isValidDay(day)) {
        cout << "无效的日期" << endl;
        return;
    }

    if (floor < 1 || floor > 5) {
        cout << "楼层号应为1-5" << endl;
        return;
    }

    if (seatData.find(day) == seatData.end() || seatData[day].find(floor) == seatData[day].end()) {
        cout << "error" << endl;
        return;
    }

    const auto& floorSeats = seatData[day][floor];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            char status = floorSeats[i][j];
            // 如果是管理员，显示实际状态；如果是普通用户，只显示0、1、2
            if (!isAdmin) {
                if (status == AVAILABLE) {
                    cout << '0';
                }
                else if (status == currentUser[0] || status == RESERVED_BY_CURRENT) {
                    cout << '2';
                }
                else {
                    cout << '1';
                }
            }
            else {
                cout << status;
            }
        }
        cout << endl;
    }
}

bool SeatReservationSystem::reserveSeat(const string& day, int floor, int row, int col) {
    if (!checkLogin()) return false;

    if (isAdmin) {
        cout << "管理员不能预约座位" << endl;
        return false;
    }

    if (!isValidDay(day)) {
        cout << "无效的日期" << endl;
        return false;
    }

    if (floor < 1 || floor > 5) {
        cout << "楼层号应为1-5" << endl;
        return false;
    }

    if (row < 1 || row > 4 || col < 1 || col > 4) {
        cout << "座位位置应为1-4" << endl;
        return false;
    }

    char& seatStatus = seatData[day][floor][row - 1][col - 1];

    if (seatStatus != AVAILABLE) {
        cout << "座位已被预约" << endl;
        return false;
    }

    // 预约座位，使用用户字母标识
    seatStatus = currentUser[0];

    // 记录预约信息
    string reservation = currentUser + ":" + day + " Floor " + to_string(floor) +
        " Seat " + to_string(row) + " " + to_string(col);
    reservations.push_back(reservation);

    cout << "OK" << endl;
    return true;
}

void SeatReservationSystem::showUserReservations() {
    if (!checkLogin()) return;

    if (isAdmin) {
        cout << "管理员查看所有预约记录:" << endl;
        for (const auto& reservation : reservations) {
            cout << reservation << endl;
        }
    }
    else {
        cout << "当前用户预约记录:" << endl;
        bool hasReservation = false;
        string userPrefix = currentUser + ":";

        for (const auto& reservation : reservations) {
            if (reservation.find(userPrefix) == 0) {
                // 去掉用户前缀显示
                cout << reservation.substr(userPrefix.length()) << endl;
                hasReservation = true;
            }
        }

        if (!hasReservation) {
            cout << "暂无预约记录" << endl;
        }
    }
}

void SeatReservationSystem::clearAllData() {
    if (!checkLogin()) return;

    if (!isAdmin) {
        cout << "需要管理员权限" << endl;
        return;
    }

    // 清空内存数据
    seatData.clear();
    reservations.clear();
    initializeSeatData();

    // 删除数据文件
    remove(DATA_FILE.c_str());
    remove(RESERVATION_FILE.c_str());

    cout << "所有数据已清空" << endl;
}

void SeatReservationSystem::saveData() {
    // 保存座位数据
    ofstream dataFile(DATA_FILE);
    if (dataFile.is_open()) {
        for (const auto& dayPair : seatData) {
            dataFile << dayPair.first << ";";
            for (const auto& floorPair : dayPair.second) {
                dataFile << floorPair.first << ":";
                for (const auto& row : floorPair.second) {
                    for (char status : row) {
                        dataFile << status;
                    }
                    dataFile << ",";
                }
                dataFile << "|";
            }
            dataFile << "\n";
        }
        dataFile.close();
    }

    // 保存预约记录
    ofstream resFile(RESERVATION_FILE);
    if (resFile.is_open()) {
        for (const auto& reservation : reservations) {
            resFile << reservation << "\n";
        }
        resFile.close();
    }
}

void SeatReservationSystem::loadData() {
    // 加载座位数据
    ifstream dataFile(DATA_FILE);
    if (dataFile.is_open()) {
        string line;
        while (getline(dataFile, line)) {
            stringstream ss(line);
            string dayStr;
            getline(ss, dayStr, ';');

            string floorData;
            while (getline(ss, floorData, '|')) {
                stringstream floorStream(floorData);
                string floorStr;
                getline(floorStream, floorStr, ':');
                int floor = stoi(floorStr);

                string rowData;
                int rowIndex = 0;
                while (getline(floorStream, rowData, ',') && rowIndex < 4) {
                    if (rowData.length() == 4) {
                        for (int colIndex = 0; colIndex < 4; colIndex++) {
                            seatData[dayStr][floor][rowIndex][colIndex] = rowData[colIndex];
                        }
                        rowIndex++;
                    }
                }
            }
        }
        dataFile.close();
    }

    // 加载预约记录
    ifstream resFile(RESERVATION_FILE);
    if (resFile.is_open()) {
        string line;
        while (getline(resFile, line)) {
            if (!line.empty()) {
                reservations.push_back(line);
            }
        }
        resFile.close();
    }
}