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

void SeatReservationSystem::logout() {
    if (!isLoggedIn) {
        cout << "当前未登录" << endl;
        return;
    }

    saveData();
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
        cout << "error" << endl;
        return;
    }

    if (floor < 1 || floor > 5) {
        cout << "楼层号应为1-5" << endl;
        return;
    }

    const auto& floorSeats = seatData[day][floor];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            char status = floorSeats[i][j];
            if (!isAdmin) {
                // 普通用户视图
                if (status == AVAILABLE) {
                    cout << '0';
                }
                else if (status == currentUser[0]) {
                    cout << '2';
                }
                else {
                    cout << '1';
                }
            }
            else {
                // 管理员视图
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
        cout << "error" << endl;
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

    seatStatus = currentUser[0];

    string reservation = currentUser + ":" + day + " Floor " + to_string(floor) +
        " Seat " + to_string(row) + " " + to_string(col);
    reservations.push_back(reservation);

    cout << "座位预约成功" << endl;
    saveData();
    return true;
}

void SeatReservationSystem::showUserReservations() {
    if (!checkLogin()) return;

    if (isAdmin) {
        cout << "所有预约记录:" << endl;
        if (reservations.empty()) {
            cout << "暂无预约记录" << endl;
        }
        else {
            for (const auto& reservation : reservations) {
                cout << reservation << endl;
            }
        }
    }
    else {
        cout << "我的预约记录:" << endl;
        bool hasReservation = false;
        string userPrefix = currentUser + ":";

        for (const auto& reservation : reservations) {
            if (reservation.find(userPrefix) == 0) {
                cout << reservation.substr(userPrefix.length()) << endl;
                hasReservation = true;
            }
        }

        if (!hasReservation) {
            cout << "暂无预约记录" << endl;
        }
    }
}

// 管理员功能：取消某天所有预约
void SeatReservationSystem::cancelDayReservations(const string& day) {
    if (!checkLogin() || !isAdmin) {
        cout << "需要管理员权限" << endl;
        return;
    }

    if (!isValidDay(day)) {
        cout << "error" << endl;
        return;
    }

    int cancelCount = 0;

    // 清除该天所有座位的预约
    for (int floor = 1; floor <= 5; floor++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (seatData[day][floor][i][j] != AVAILABLE) {
                    removeReservation(day, floor, i + 1, j + 1);
                    seatData[day][floor][i][j] = AVAILABLE;
                    cancelCount++;
                }
            }
        }
    }

    cout << "已取消 " << day << " 的所有预约，共 " << cancelCount << " 个座位" << endl;
    saveData();
}

// 管理员功能：为某层楼增加一排座位（扩展到5x4）
void SeatReservationSystem::addSeat(int floor) {
    if (!checkLogin() || !isAdmin) {
        cout << "需要管理员权限" << endl;
        return;
    }

    if (floor < 1 || floor > 5) {
        cout << "楼层号应为1-5" << endl;
        return;
    }

    // 为所有天的该楼层增加一排座位
    for (auto& dayPair : seatData) {
        // 添加第5排座位
        dayPair.second[floor].push_back(vector<char>(4, AVAILABLE));
    }

    cout << "已为第 " << floor << " 层增加一排座位（现在为5排4列）" << endl;
    saveData();
}

// 管理员功能：删除指定座位
void SeatReservationSystem::removeSeat(int floor, int row, int col) {
    if (!checkLogin() || !isAdmin) {
        cout << "需要管理员权限" << endl;
        return;
    }

    if (floor < 1 || floor > 5) {
        cout << "楼层号应为1-5" << endl;
        return;
    }

    if (row < 1 || row > 4 || col < 1 || col > 4) {
        cout << "座位位置应为1-4" << endl;
        return;
    }

    // 取消该座位在所有天的预约
    for (auto& dayPair : seatData) {
        if (dayPair.second[floor][row - 1][col - 1] != AVAILABLE) {
            removeReservation(dayPair.first, floor, row, col);
        }
        // 标记座位为已删除（用'X'表示）
        dayPair.second[floor][row - 1][col - 1] = 'X';
    }

    cout << "已删除第 " << floor << " 层座位 (" << row << ", " << col << ")" << endl;
    saveData();
}

// 辅助函数：移除特定座位的预约记录
void SeatReservationSystem::removeReservation(const string& day, int floor, int row, int col) {
    string target = ":" + day + " Floor " + to_string(floor) + " Seat " + to_string(row) + " " + to_string(col);

    auto it = reservations.begin();
    while (it != reservations.end()) {
        if (it->find(target) != string::npos) {
            it = reservations.erase(it);
        }
        else {
            ++it;
        }
    }
}

void SeatReservationSystem::clearAllData() {
    if (!checkLogin() || !isAdmin) {
        cout << "需要管理员权限" << endl;
        return;
    }

    seatData.clear();
    reservations.clear();
    initializeSeatData();

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
                while (getline(floorStream, rowData, ',') && rowIndex < 5) { // 最多5排
                    if (rowData.length() == 4) {
                        // 确保有足够的行
                        if (seatData[dayStr][floor].size() <= rowIndex) {
                            seatData[dayStr][floor].resize(5, vector<char>(4, AVAILABLE));
                        }
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