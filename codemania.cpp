#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using ll = long long;

vector<ll> nums;
vector<char> chars;

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

    return 0;
}
