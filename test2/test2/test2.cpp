#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

// �����ļ����洢��Ч�ļ�ֵ��
bool parseFile(const string& filename, unordered_map<string, string>& keyValueMap) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "�޷����ļ�: " << filename << endl;
        return false;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;

        // ��������
        if (line.empty()) {
            continue;
        }

        // ���ҵ�һ��ð�ŵ�λ��
        size_t colonPos = line.find(':');

        // ����Ƿ�����Ч�ļ�ֵ����
        if (colonPos == string::npos || colonPos == 0 || colonPos == line.length() - 1) {
            // ��ʽ������У�����
            continue;
        }

        // ��ȡ����ֵ
        string key = line.substr(0, colonPos);
        string value = line.substr(colonPos + 1);

        // ������ֵ�ĳ����Ƿ����Ҫ�󣨲�����10���ַ���
        if (key.length() <= 10 && value.length() <= 10) {
            keyValueMap[key] = value;
        }
    }

    file.close();
    return true;
}

int main() {
    unordered_map<string, string> keyValueMap;

    // ����data.txt�ļ�
    if (!parseFile("data.txt", keyValueMap)) {
        return 1; // �ļ���ʧ�ܣ��˳�����
    }

    // ���û�����
    string input;
    

    while (getline(cin, input)) {
        if (input == "Quit") {
            break;
        }

        // ���Ҽ���������
        auto it = keyValueMap.find(input);
        if (it != keyValueMap.end()) {
            cout << it->second << endl;
        }
        else {
            cout << "Error" << endl;
        }

       
    }

    cout << "�������˳���" << endl;
    return 0;
}
