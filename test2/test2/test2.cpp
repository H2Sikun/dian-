#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>

using namespace std;

// 解析文件并存储有效的键值对
bool parseFile(const string& filename, unordered_map<string, string>& keyValueMap) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开文件: " << filename << endl;
        return false;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {
        lineNumber++;

        // 跳过空行
        if (line.empty()) {
            continue;
        }

        // 查找第一个冒号的位置
        size_t colonPos = line.find(':');

        // 检查是否是有效的键值对行
        if (colonPos == string::npos || colonPos == 0 || colonPos == line.length() - 1) {
            // 格式错误的行，跳过
            continue;
        }

        // 提取键和值
        string key = line.substr(0, colonPos);
        string value = line.substr(colonPos + 1);

        // 检查键和值的长度是否符合要求（不超过10个字符）
        if (key.length() <= 10 && value.length() <= 10) {
            keyValueMap[key] = value;
        }
    }

    file.close();
    return true;
}

int main() {
    unordered_map<string, string> keyValueMap;

    // 解析data.txt文件
    if (!parseFile("data.txt", keyValueMap)) {
        return 1; // 文件打开失败，退出程序
    }

    // 与用户交互
    string input;
    

    while (getline(cin, input)) {
        if (input == "Quit") {
            break;
        }

        // 查找键并输出结果
        auto it = keyValueMap.find(input);
        if (it != keyValueMap.end()) {
            cout << it->second << endl;
        }
        else {
            cout << "Error" << endl;
        }

       
    }

    cout << "程序已退出。" << endl;
    return 0;
}
