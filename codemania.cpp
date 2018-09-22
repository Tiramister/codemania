#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using ll = long long;

vector<ll> nums;
vector<string> strs;

string S;
ll N;
int numstate = 0;
int strstate = 0;

void execute(int var);

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

void exe_literal_num(int var) {
    if (numstate == -1) {
        numstate = var;
    } else {
        N = (N << 8) + var;
        --numstate;
    }
}

void exe_literal_str(int var) {
    if (strstate == -1) {
        strstate = N;
    }
    S.push_back(static_cast<char>(var));
    --strstate;
}

void execute(int var) {
    if (numstate != 0) {
        exe_literal_num(var);
        return;
    }
    if (strstate != 0) {
        exe_literal_str(var);
        return;
    }

    switch (var) {
    case 0b10100000: {
        // 文字列の入力
        cin >> S;
        break;
    }
    case 0b01100000: {
        // 数値の入力
        cin >> N;
        break;
    }
    case 0b10010000: {
        // 文字列の出力
        cout << S;
        break;
    }
    case 0b01010000: {
        // 数値の出力
        cout << N;
        break;
    }
    case 0b10000100: {
        // 文字列を追加
        strs.push_back(S);
        break;
    }
    case 0b01000100: {
        // 数値を追加
        nums.push_back(N);
        break;
    }
    case 0b10001000: {
        // 文字列を取り出し
        if (strs.empty()) {
            cout << "エラー: 文字列コンテナが空のまま取り出そうとしています。" << endl;
            exit(0);
        }
        S = strs.back();
        strs.pop_back();
        break;
    }
    case 0b01001000: {
        // 数値を取り出し
        if (nums.empty()) {
            cout << "エラー: 数値コンテナが空のまま取り出そうとしています。" << endl;
            exit(0);
        }
        N = nums.back();
        nums.pop_back();
        break;
    }
    case 0b10000000: {
        // cout << "文字列リテラルの処理" << endl;
        S = "";
        strstate = -1;
        break;
    }
    case 0b01000000: {
        // cout << "数値リテラルの処理" << endl;
        N = 0;
        numstate = -1;
        break;
    }
    }
}

int main(int argc, char* argv[]) {
    ifstream ifs;
    ifs.open(argv[1]);

    vector<pair<bool, int>> opes;
    opes.push_back(make_pair(false, 0));

    string str;
    while (!ifs.eof()) {
        getline(ifs, str);

        if (str.empty()) break;

        bool con;
        int var;
        tie(con, var) = parse(str);
        if (!con && var == 0) {
            break;
        }
        opes.push_back(make_pair(con, var));
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
