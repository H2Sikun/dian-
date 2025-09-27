#include "seat_reservation.h"
#include <sstream>

void printHelp(bool isAdmin = false) {
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

int main() {
    SeatReservationSystem system;
    string command;

    cout << "\t\t --------------------------------------------------\n";
    cout << "\t\t|       === ͼ�����λԤԼϵͳ����ָ�� ===         |\n";
    cout << "\t\t|       ��¼���ָ��                               |\n";
    cout << "\t\t|       Login - ��¼   (����Ա�û���Ϊ��Admin)     |\n";
    cout << "\t\t|       Exit - �˳���ǰ��¼                        |\n";
    cout << "\t\t|       Quit - �˳�����                            |\n";
    cout << "\t\t|       Help - ����ָ��                            |\n";
    cout << "\t\t|       ԤԼ���ָ��:                              |\n";
    cout << "\t\t|    eg:1. �鿴ԤԼ���                            |\n";
    cout << "\t\t|       Monday Floor 1(Monday--Sunday,Floor 1--5)  |\n";
    cout << "\t\t|       2. ԤԼ��λ                                |\n";
    cout << "\t\t|       Reserve Monday Floor 1 Seat 1 2            |\n";
    cout << "\t\t|       3. Reservation - ��ʾԤԼ���              |\n";
    cout << "\t\t|       4. Clear - ����û�����(����Ա)            |\n";
    cout << "\t\t|       5. ԤԼ״̬                                |\n";
    cout << "\t\t|       0 ��λ�� 1 ��λ��ԤԼ 2 ��λ���Լ�ԤԼ     |\n";
    cout << "\t\t --------------------------------------------------\n";

    while (true) {
        cout << "> ";
        getline(cin, command);

        if (command.empty()) continue;

        if (command == "Quit" || command == "quit") {
            system.saveData();
            cout << "�ټ�!" << endl;
            break;
        }
        else if (command == "Help" || command == "help") {
            // ������Ҫ�жϵ�ǰ�Ƿ��ǹ���Ա״̬
            // �򻯴�����ʾͨ�ð���
            cout << "ͨ��ָ��: Login, Quit, Help" << endl;
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