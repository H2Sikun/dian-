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
    if (isAdmin) {
        cout << "\t\t ---------------------------------------------------------\n";
        cout << "\t\t|       === ͼ�����λԤԼϵͳ����ָ��(����Ա) ===        |\n";
        cout << "\t\t|       Monday Floor n - �鿴��n��n����λ                 |\n";
        cout << "\t\t|       CancelDay Monday - ȡ����n����ԤԼ               |\n";
        cout << "\t\t|       AddSeat n - Ϊ��n������һ����λ                   |\n";
        cout << "\t\t|       RemoveSeat n m k - ɾ����n����λ(m��k��)          |\n";
        cout << "\t\t|       Reservation - �鿴����ԤԼ                        |\n";
        cout << "\t\t|       Clear - �����������                              |\n";
        cout << "\t\t|       Exit - �˳���¼                                   |\n";
        cout << "\t\t|       Help - ��ʾָ��                                   |\n";
        cout << "\t\t ---------------------------------------------------------\n";
    }
    else {
        cout << "\t\t ---------------------------------------------------------\n";
        cout << "\t\t|       === ͼ�����λԤԼϵͳ����ָ��(ѧ��) ===          |\n";
        cout << "\t\t|       Reserve Monday Floor n Seat m k - ԤԼ��λ        |\n";
        cout << "\t\t|       Reservation - �鿴�ҵ�ԤԼ                        |\n";
        cout << "\t\t|       Monday Floor n - �鿴��n��n����λ                 |\n";
        cout << "\t\t|       Exit - �˳���¼                                   |\n";
        cout << "\t\t|       Help - ��ʾָ��                                   |\n";
        cout << "\t\t ---------------------------------------------------------\n";
    }
}

void SeatReservationSystem::logout() {
    if (!isLoggedIn) {
        cout << "��ǰδ��¼" << endl;
        return;
    }

    saveData();
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
        cout << "error" << endl;
        return;
    }

    if (floor < 1 || floor > 5) {
        cout << "¥���ӦΪ1-5" << endl;
        return;
    }

    const auto& floorSeats = seatData[day][floor];

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            char status = floorSeats[i][j];
            if (!isAdmin) {
                // ��ͨ�û���ͼ
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
                // ����Ա��ͼ
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
        cout << "error" << endl;
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

    seatStatus = currentUser[0];

    string reservation = currentUser + ":" + day + " Floor " + to_string(floor) +
        " Seat " + to_string(row) + " " + to_string(col);
    reservations.push_back(reservation);

    cout << "��λԤԼ�ɹ�" << endl;
    saveData();
    return true;
}

void SeatReservationSystem::showUserReservations() {
    if (!checkLogin()) return;

    if (isAdmin) {
        cout << "����ԤԼ��¼:" << endl;
        if (reservations.empty()) {
            cout << "����ԤԼ��¼" << endl;
        }
        else {
            for (const auto& reservation : reservations) {
                cout << reservation << endl;
            }
        }
    }
    else {
        cout << "�ҵ�ԤԼ��¼:" << endl;
        bool hasReservation = false;
        string userPrefix = currentUser + ":";

        for (const auto& reservation : reservations) {
            if (reservation.find(userPrefix) == 0) {
                cout << reservation.substr(userPrefix.length()) << endl;
                hasReservation = true;
            }
        }

        if (!hasReservation) {
            cout << "����ԤԼ��¼" << endl;
        }
    }
}

// ����Ա���ܣ�ȡ��ĳ������ԤԼ
void SeatReservationSystem::cancelDayReservations(const string& day) {
    if (!checkLogin() || !isAdmin) {
        cout << "��Ҫ����ԱȨ��" << endl;
        return;
    }

    if (!isValidDay(day)) {
        cout << "error" << endl;
        return;
    }

    int cancelCount = 0;

    // �������������λ��ԤԼ
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

    cout << "��ȡ�� " << day << " ������ԤԼ���� " << cancelCount << " ����λ" << endl;
    saveData();
}

// ����Ա���ܣ�Ϊĳ��¥����һ����λ����չ��5x4��
void SeatReservationSystem::addSeat(int floor) {
    if (!checkLogin() || !isAdmin) {
        cout << "��Ҫ����ԱȨ��" << endl;
        return;
    }

    if (floor < 1 || floor > 5) {
        cout << "¥���ӦΪ1-5" << endl;
        return;
    }

    // Ϊ������ĸ�¥������һ����λ
    for (auto& dayPair : seatData) {
        // ��ӵ�5����λ
        dayPair.second[floor].push_back(vector<char>(4, AVAILABLE));
    }

    cout << "��Ϊ�� " << floor << " ������һ����λ������Ϊ5��4�У�" << endl;
    saveData();
}

// ����Ա���ܣ�ɾ��ָ����λ
void SeatReservationSystem::removeSeat(int floor, int row, int col) {
    if (!checkLogin() || !isAdmin) {
        cout << "��Ҫ����ԱȨ��" << endl;
        return;
    }

    if (floor < 1 || floor > 5) {
        cout << "¥���ӦΪ1-5" << endl;
        return;
    }

    if (row < 1 || row > 4 || col < 1 || col > 4) {
        cout << "��λλ��ӦΪ1-4" << endl;
        return;
    }

    // ȡ������λ���������ԤԼ
    for (auto& dayPair : seatData) {
        if (dayPair.second[floor][row - 1][col - 1] != AVAILABLE) {
            removeReservation(dayPair.first, floor, row, col);
        }
        // �����λΪ��ɾ������'X'��ʾ��
        dayPair.second[floor][row - 1][col - 1] = 'X';
    }

    cout << "��ɾ���� " << floor << " ����λ (" << row << ", " << col << ")" << endl;
    saveData();
}

// �����������Ƴ��ض���λ��ԤԼ��¼
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
        cout << "��Ҫ����ԱȨ��" << endl;
        return;
    }

    seatData.clear();
    reservations.clear();
    initializeSeatData();

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
                while (getline(floorStream, rowData, ',') && rowIndex < 5) { // ���5��
                    if (rowData.length() == 4) {
                        // ȷ�����㹻����
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