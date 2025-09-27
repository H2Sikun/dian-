#include "seat_reservation.h"
#include <sstream>
#include <iostream>
void printHelp() {
    cout << "\t\t --------------------------------------------------\n";
    cout << "\t\t|       === ͼ�����λԤԼϵͳ����ָ�� ===         |\n";
    cout << "\t\t|       ��¼���ָ��                               |\n";
    cout << "\t\t|       Login - ��¼                               |\n";
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
}

int main() {
    SeatReservationSystem system;
    string command;

    cout << "��ӭʹ��ͼ�����λԤԼϵͳ!" << endl;
    printHelp();

    while (true) {
        cout << "> ";
        getline(cin, command);

        if (command == "Quit" || command == "quit") {
            system.saveData();
            cout << "�������˳�!" << endl;
            break;
        }
        else if (command == "Help" || command == "help") {
            printHelp();
        }
        else if (command == "Login" || command == "login") {
            system.login();
        }
        else if (command == "Exit" || command == "exit") {
            system.logout();
        }
        else if (command == "Clear" || command == "clear") {
            system.clearAllData();
        }
        else if (command == "Reservation" || command == "reservation") {
            system.showUserReservations();
        }
        else if (command.find("Floor") != string::npos) {
            stringstream ss(command);
            string day, floorCmd;
            int floor;

            ss >> day >> floorCmd >> floor;

            if (floor >= 1 && floor <= 5) {
                system.showFloorSeats(day, floor);
            }
            else {
                cout << "¥���ӦΪ1-5" << endl;
            }
        }
        else if (command.find("Reserve") == 0) {
            stringstream ss(command);
            string cmd, day, floorCmd, seatCmd;
            int floor, row, col;

            ss >> cmd >> day >> floorCmd >> floor >> seatCmd >> row >> col;

            if (floor >= 1 && floor <= 5 && row >= 1 && row <= 4 && col >= 1 && col <= 4) {
                system.reserveSeat(day, floor, row, col);
            }
            else {
                cout << "��������: ¥��1-5, ����1-4" << endl;
            }
        }
        else {
            cout << "error������������" << endl;
        }
    }

    return 0;
}