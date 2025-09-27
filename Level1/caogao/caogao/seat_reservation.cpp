#include "seat_reservation.h"

SeatReservationSystem::SeatReservationSystem() : currentUser(""), isAdmin(false), isLoggedIn(false) {
    initializeSeatData();
    loadData();
}

void SeatReservationSystem::initializeSeatData() {
    // ��ʼ��7������ݣ�5��¥��ÿ��4x4��λ
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
        cout << "�����˳���ǰ��¼" << endl;
        return;
    }

    cout << "�������û���: ";
    string username;
    cin >> username;

    // ������뻺����
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (!isValidUser(username)) {
        cout << "��Ч���û�����������A-Z��Admin" << endl;
        return;
    }

    currentUser = username;
    isAdmin = (username == "Admin");
    isLoggedIn = true;

    cout << "��¼�ɹ�" << endl;
}
void SeatReservationSystem::logout() {
    if (!isLoggedIn) {
        cout << "��ǰδ��¼" << endl;
        return;
    }

    saveData(); // �˳�ǰ��������
    currentUser = "";
    isAdmin = false;
    isLoggedIn = false;
    cout << "�˳���¼�ɹ�" << endl;
}

bool SeatReservationSystem::checkLogin() {
    if (!isLoggedIn) {
        cout << "���ȵ�¼" << endl;
        return false;
    }
    return true;
}

void SeatReservationSystem::showFloorSeats(const string& day, int floor) {
    if (!checkLogin()) return;

    if (!isValidDay(day)) {
        cout << "��Ч������" << endl;
        return;
    }

    if (floor < 1 || floor > 5) {
        cout << "¥���ӦΪ1-5" << endl;
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
            // ����ǹ���Ա����ʾʵ��״̬���������ͨ�û���ֻ��ʾ0��1��2
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
        cout << "����Ա����ԤԼ��λ" << endl;
        return false;
    }

    if (!isValidDay(day)) {
        cout << "��Ч������" << endl;
        return false;
    }

    if (floor < 1 || floor > 5) {
        cout << "¥���ӦΪ1-5" << endl;
        return false;
    }

    if (row < 1 || row > 4 || col < 1 || col > 4) {
        cout << "��λλ��ӦΪ1-4" << endl;
        return false;
    }

    char& seatStatus = seatData[day][floor][row - 1][col - 1];

    if (seatStatus != AVAILABLE) {
        cout << "��λ�ѱ�ԤԼ" << endl;
        return false;
    }

    // ԤԼ��λ��ʹ���û���ĸ��ʶ
    seatStatus = currentUser[0];

    // ��¼ԤԼ��Ϣ
    string reservation = currentUser + ":" + day + " Floor " + to_string(floor) +
        " Seat " + to_string(row) + " " + to_string(col);
    reservations.push_back(reservation);

    cout << "OK" << endl;
    return true;
}

void SeatReservationSystem::showUserReservations() {
    if (!checkLogin()) return;

    if (isAdmin) {
        cout << "����Ա�鿴����ԤԼ��¼:" << endl;
        for (const auto& reservation : reservations) {
            cout << reservation << endl;
        }
    }
    else {
        cout << "��ǰ�û�ԤԼ��¼:" << endl;
        bool hasReservation = false;
        string userPrefix = currentUser + ":";

        for (const auto& reservation : reservations) {
            if (reservation.find(userPrefix) == 0) {
                // ȥ���û�ǰ׺��ʾ
                cout << reservation.substr(userPrefix.length()) << endl;
                hasReservation = true;
            }
        }

        if (!hasReservation) {
            cout << "����ԤԼ��¼" << endl;
        }
    }
}

void SeatReservationSystem::clearAllData() {
    if (!checkLogin()) return;

    if (!isAdmin) {
        cout << "��Ҫ����ԱȨ��" << endl;
        return;
    }

    // ����ڴ�����
    seatData.clear();
    reservations.clear();
    initializeSeatData();

    // ɾ�������ļ�
    remove(DATA_FILE.c_str());
    remove(RESERVATION_FILE.c_str());

    cout << "�������������" << endl;
}

void SeatReservationSystem::saveData() {
    // ������λ����
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

    // ����ԤԼ��¼
    ofstream resFile(RESERVATION_FILE);
    if (resFile.is_open()) {
        for (const auto& reservation : reservations) {
            resFile << reservation << "\n";
        }
        resFile.close();
    }
}

void SeatReservationSystem::loadData() {
    // ������λ����
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

    // ����ԤԼ��¼
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