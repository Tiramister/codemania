#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;
using ll = long long;

vector<ll> nums;
queue<ll> numq;
vector<string> strs;


pair<bool, int> parse(string S) {
    while (S.size() < 9) {
        S.push_back(' ');
    }
    bool con = (S[0] == '|');

    int res = 0;
    for (int i = 1; i <= 8; ++i) {
        res <<= 1;
        res += (S[i] == '_');
    }

    return make_pair(con, res);
}

void execute(int var) {
    switch (var) {
    case 0b10100000: {
        // 文字列の入力
        cout << "input str" << endl;
        string str;
        cin >> str;
        strs.push_back(str);
        break;
    }
    case 0b01100000: {
        // 数値の入力
        cout << "input num" << endl;
        ll n;
        cin >> n;
        nums.push_back(n);
        break;
    }
    case 0b10010000: {
        // 文字列の出力
        cout << "output str" << endl;
        cout << strs.back();
        strs.pop_back();
        break;
    }
    case 0b10010001: {
        // スペースの出力
        cout << " ";
        break;
    }
    case 0b10010010: {
        // 改行の出力
        cout << endl;
        break;
    }
    case 0b01010000: {
        // 数値の出力
        cout << "output num" << endl;
        cout << nums.back();
        nums.pop_back();
        break;
    }
    }
}

int main(int argc, char* argv[]) {
    cout << argv[1] << endl;
    ifstream ifs;
    ifs.open(argv[1]);

    vector<pair<bool, int>> opes;
    opes.push_back(make_pair(false, 0));

    string str;
    while (!ifs.eof()) {
        getline(ifs, str);
        cout << str << endl;

        if (str.empty()) break;

        bool con;
        int var;
        tie(con, var) = parse(str);
        if (!con && var == 0) {
            break;
        }
        opes.push_back(make_pair(con, var));
        cout << con << " " << var << endl;
    }
    ifs.close();

    int now = opes.size() - 1;
    while (now > 0) {
        bool con;
        int var;
        tie(con, var) = opes[now];

        execute(var);
        --now;
    }

    return 0;
}
