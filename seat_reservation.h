#ifndef SEAT_RESERVATION_H
#define SEAT_RESERVATION_H

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>

using namespace std;

// ��λ״̬
const char AVAILABLE = '0';
const char RESERVED_BY_CURRENT = '2';

class SeatReservationSystem {
private:
    // �洢�ṹ��day -> floor -> row -> col -> status
    map<string, map<int, vector<vector<char>>>> seatData;
    vector<string> reservations; // �洢ԤԼ��¼
    string currentUser;
    bool isAdmin;
    bool isLoggedIn;

    // �����ļ�·��
    const string DATA_FILE = "seat_data.dat";
    const string RESERVATION_FILE = "reservations.dat";

    // �����б�
    vector<string> days = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

public:
    SeatReservationSystem();

    // �û�����
    void login();
    void logout();
    bool checkLogin();

    // ���Ĺ���
    void showFloorSeats(const string& day, int floor);
    bool reserveSeat(const string& day, int floor, int row, int col);
    void showUserReservations();
    void clearAllData();

    // ���ݳ־û�
    void saveData();
    void loadData();

private:
    void initializeSeatData();
    bool isValidUser(const string& user);
    bool isValidDay(const string& day);
};

#endif