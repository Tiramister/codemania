#include <iostream>
#include <string>
#include <vector>
using namespace std;
using ll = long long;

pair<bool, int> parse(string S) {
    if (S.size() != 9) {
        cout << "エラー: 入力長が9文字ではありません。" << endl;
        exit(0);
    }
    bool con = (S[0] == '|');

    int res = 0;
    for (int i = 1; i <= 8; ++i) {
        res <<= 1;
        res += (S[i] == '_');
    }

    return make_pair(con, res);
}


int main() {
    vector<pair<bool, int>> opes;
    opes.push_back(make_pair(false, 0));

    while (true) {
        string str;
        getline(cin, str);

        bool con;
        int var;
        tie(con, var) = parse(str);
        if (var == (1 << 8) - 1) {
            break;
        }
        opes.push_back(make_pair(con, var));
        cout << con << " " << var << endl;
    }

    return 0;
}
