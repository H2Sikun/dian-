#include "LibrarySystem.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <sstream>
using namespace std;

// Seat �ṹ��ʵ��
Seat::Seat(int r, int c, int d, int f) : row(r), col(c), status(FREE), reservedBy(' '), day(d), floor(f) {}

// Reservation �ṹ��ʵ��
Reservation::Reservation(string u, int d, int f, int r, int c) : user(u), day(d), floor(f), row(r), col(c) {}

// LibrarySystem ��ʵ��
LibrarySystem::LibrarySystem() : isLoggedIn(false) {
    // ��ʼ������ӳ��
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
    // ������λ����
    ifstream seatFile(DATA_FILE, ios::binary);//�Զ�������ʽ����
    if (seatFile) {
        for (int day = 0; day < DAYS; day++) {
            for (int floor = 0; floor < FLOORS; floor++) {
                for (int row = 0; row < ROWS; row++) {
                    for (int col = 0; col < COLS; col++) {
                        seatFile.read(reinterpret_cast<char*>(&library[day][floor][row][col].status), sizeof(char));//read��Ӧ��char����
                        seatFile.read(reinterpret_cast<char*>(&library[day][floor][row][col].reservedBy), sizeof(char));
                    }
                }
            }
        }
        seatFile.close();
    }

    // ����ԤԼ��¼
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
    // ������λ����
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

    // ����ԤԼ��¼
    ofstream resFile(RESERVATION_FILE);
    if (resFile) {
        for (const auto& res : reservations) {
            resFile << res.user << " " << res.day << " " << res.floor << " " << res.row << " " << res.col << endl;
        }
        resFile.close();
    }
}

void LibrarySystem::updateSeatStatus() {
    // ���ݵ�ǰ�û�������λ��ʾ״̬
    for (int day = 0; day < DAYS; day++) {
        for (int floor = 0; floor < FLOORS; floor++) {
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    Seat& seat = library[day][floor][row][col];
                    if (seat.status != FREE) {
                        if (currentUser == "Admin") {
                            // ����Ա����ʵ��ԤԼ��
                            seat.status = seat.reservedBy;
                        }
                        else if (seat.reservedBy == currentUser[0]) {
                            // ��ǰ�û��Լ���ԤԼ
                            seat.status = CURRENT_USER_RESERVED;
                        }
                        else {
                            // �����û���ԤԼ
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
    cout << "�������û���: ";
    cin >> username;

    // ������뻺�����еĻ��з�����ֹ�û�����A����ʾ">A>A"�����ǡ�>A��
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (isValidUser(username)) {
        currentUser = username;
        isLoggedIn = true;
        updateSeatStatus();
        cout << "��¼�ɹ�" << endl;
    }
    else {
        cout << "��Ч���û�����������A-Z�ĵ�����ĸ��Admin" << endl;
    }
}

void LibrarySystem::logout() {
    if (isLoggedIn) {
        cout << "�û� " << currentUser << " ���˳���¼" << endl;
        currentUser = "";
        isLoggedIn = false;
    }
    else {
        cout << "��ǰδ��¼" << endl;
    }
}

void LibrarySystem::showFloor(const string& dayStr, int floor) {
    if (!isLoggedIn) {
        cout << "���ȵ�¼" << endl;
        return;
    }

    if (dayMap.find(dayStr) == dayMap.end()) {
        cout << "��Ч�����ڣ���������ȷ�����ڣ���Monday��" << endl;
        return;
    }

    if (floor < 1 || floor > FLOORS) {
        cout << "��Ч��¥�㣬������1-5" << endl;
        return;
    }

    int day = dayMap[dayStr];
    int floorIndex = floor - 1;//�����0��ʼ��ֱ�۵�¥����ʼΪ1

    cout << dayStr << " Floor " << floor << " ����λ���:" << endl;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            cout << library[day][floorIndex][row][col].status;
        }
        cout << endl;
    }
}

void LibrarySystem::reserveSeat(const string& dayStr, int floor, int row, int col) {
    if (!isLoggedIn) {
        cout << "���ȵ�¼" << endl;
        return;
    }

    if (currentUser == "Admin") {
        cout << "����Ա����ԤԼ��λ" << endl;
        return;
    }

    if (dayMap.find(dayStr) == dayMap.end()) {
        cout << "��Ч������" << endl;
        return;
    }

    if (floor < 1 || floor > FLOORS || row < 1 || row > ROWS || col < 1 || col > COLS) {
        cout << "��Ч����λλ��" << endl;
        return;
    }

    int day = dayMap[dayStr];//�����ڵ�Ӣ��ӳ��Ϊ���ַ�������
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
        cout << "����λ�ѱ�ԤԼ" << endl;
    }
}

void LibrarySystem::showReservations() {
    if (!isLoggedIn) {
        cout << "���ȵ�¼" << endl;
        return;
    }

    if (currentUser == "Admin") {
        cout << "����ԤԼ��¼:" << endl;
        for (const auto& res : reservations) {
            string dayStr;
            for (const auto& pair : dayMap) {
                if (pair.second == res.day) {
                    dayStr = pair.first;
                    break;
                }
            }
            cout << dayStr << " Floor " << (res.floor + 1) << " Seat " << (res.row + 1) << " " << (res.col + 1)
                << " (�û�: " << res.user << ")" << endl;
        }
    }
    else {
        cout << "��ǰ�û���ԤԼ:" << endl;
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
            cout << "����ԤԼ" << endl;
        }
    }
}

void LibrarySystem::clearData() {
    if (!isLoggedIn) {
        cout << "���ȵ�¼" << endl;
        return;
    }

    if (currentUser != "Admin") {
        cout << "ֻ�й���Ա�����������" << endl;
        return;
    }

    // ����������λ״̬
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

    // ���ԤԼ��¼
    reservations.clear();

    // ɾ�������ļ�
    remove(DATA_FILE.c_str());
    remove(RESERVATION_FILE.c_str());

    cout << "���������" << endl;
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

        // ��ȡ������һ������������
        string line;
        getline(cin, line);

        if (line.empty()) {
            continue; // ����ǿ��У�������ʾ��ʾ��
        }

        // ʹ���ַ������������ֻ��
        istringstream iss(line);
        iss >> command;

        if (command == "Login") {
            login();
        }
        else if (command == "Exit") {
            logout();
        }
        else if (command == "Quit") {
            cout << "�ټ�!" << endl;
            break;
        }
        else if (isLoggedIn) {
            // ��¼�������
            if (command == "Reservation") {
                showReservations();
            }
            else if (command == "Clear") {
                clearData();
            }
            else if (command == "Reserve") {
                // ����ԤԼ����
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
                // ��������¥���ѯ����
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
            cout << "error,δ֪��������ȵ�¼" << endl;
        }
    }
}