#ifndef SEAT_RESERVATION_H
#define SEAT_RESERVATION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

// 座位状态
const char AVAILABLE = '0';

class SeatReservationSystem {
private:
    // 存储结构：day -> floor -> row -> col -> status
    map<string, map<int, vector<vector<char>>>> seatData;
    vector<string> reservations;
    string currentUser;
    bool isAdmin;
    bool isLoggedIn;

    // 数据文件路径
    const string DATA_FILE = "seat_data.dat";
    const string RESERVATION_FILE = "reservations.dat";

    // 日期列表
    vector<string> days = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

public:
    SeatReservationSystem();

    // 用户管理
    void login();
    void logout();
    bool checkLogin();

    // 核心功能
    void showFloorSeats(const string& day, int floor);
    bool reserveSeat(const string& day, int floor, int row, int col);
    void showUserReservations();

    // 管理员功能
    void clearAllData();
    void cancelDayReservations(const string& day);
    void addSeat(int floor);
    void removeSeat(int floor, int row, int col);

    // 数据持久化
    void saveData();
    void loadData();

private:
    void initializeSeatData();
    bool isValidUser(const string& user);
    bool isValidDay(const string& day);
    void removeReservation(const string& day, int floor, int row, int col);
};

#endif