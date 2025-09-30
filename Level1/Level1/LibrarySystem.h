#ifndef LIBRARY_SYSTEM_H
#define LIBRARY_SYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

// 座位状态常量
const char FREE = '0';
const char RESERVED = '1';
const char CURRENT_USER_RESERVED = '2';

// 数据结构定义
struct Seat {
    int row;
    int col;
    char status;
    char reservedBy;
    int day; // 0-6 表示周日到周六
    int floor;

    Seat(int r, int c, int d, int f);
};

struct Reservation {
    string user;
    int day;
    int floor;
    int row;
    int col;

    Reservation(string u, int d, int f, int r, int c);
};

class LibrarySystem {
private:
    vector<vector<vector<vector<Seat>>>> library; // [day][floor][row][col]
    vector<Reservation> reservations;
    string currentUser;
    bool isLoggedIn;

    // 日期字符串到数字的映射
    map<string, int> dayMap;

    const int DAYS = 7;
    const int FLOORS = 5;
    const int ROWS = 4;
    const int COLS = 4;

    const string DATA_FILE = "library_data.dat";
    const string RESERVATION_FILE = "reservations.dat";

    void initializeLibrary();
    void loadData();
    void saveData();
    void updateSeatStatus();
    bool isValidUser(const string& user);

public:
    LibrarySystem();
    ~LibrarySystem();

    void login();
    void logout();
    void showFloor(const string& dayStr, int floor);
    void reserveSeat(const string& dayStr, int floor, int row, int col);
    void showReservations();
    void clearData();
    void run();
};

#endif