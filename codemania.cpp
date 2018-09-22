#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
using ll = long long;

vector<ll> nums;
vector<string> strs;

string S;
char C;
ll N;

int numstate = 0;
int charstate = 0;

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

// 譜面から数値のbyte数と、数自体を複数行で受け取る
void exe_literal_num(int var) {
    if (numstate == -1) {
        numstate = var;
    } else {
        N = (N << 8) + var;
        --numstate;
    }
}

// 譜面をcharにキャスト
void exe_literal_char(int var) {
    C = static_cast<char>(var);
    charstate = 0;
}

void execute(int var) {
    if (numstate != 0) {
        exe_literal_num(var);
        return;
    }
    if (charstate != 0) {
        exe_literal_char(var);
        return;
    }

    switch (var) {
    /* -------------------- 標準入出力編 -------------------- */
    // Sに標準入力
    case 0b10100000: {
        cin >> S;
        break;
    }

    // Nに標準入力
    case 0b01100000: {
        cin >> N;
        break;
    }

    // Sを標準出力
    case 0b10010000: {
        cout << S;
        break;
    }

    // Sを標準出力、改行付き
    case 0b10010001: {
        cout << S << endl;
        break;
    }

    // Nを標準出力
    case 0b01010000: {
        cout << N;
        break;
    }

    // Nを標準出力、改行付き
    case 0b01010001: {
        cout << N << endl;
        break;
    }

    /* -------------------- 文字列スタック操作 -------------------- */
    // Sを文字列スタックに追加
    case 0b10000100: {
        strs.push_back(S);
        break;
    }

    // 文字列スタックの末尾を削除
    case 0b10001000: {
        if (strs.empty()) {
            cout << "エラー: 文字列コンテナが空のまま取り出そうとしています。" << endl;
            exit(0);
        }
        strs.pop_back();
        break;
    }

    // 文字列スタックの末尾をSにコピー
    case 0b10001001: {
        if (strs.empty()) {
            cout << "エラー: 文字列コンテナが空のまま参照しようとしています。" << endl;
            exit(0);
        }
        S = strs.back();
        break;
    }

    // 文字列スタックの末尾とSをswap
    case 0b10001100: {
        if (strs.empty()) {
            cout << "エラー: 文字列コンテナが空のまま参照しようとしています。" << endl;
            exit(0);
        }
        swap(S, strs.back());
        break;
    }

    /* -------------------- 数値スタック操作 -------------------- */
    // Nを数値スタックに追加
    case 0b01000100: {
        nums.push_back(N);
        break;
    }

    // 数値スタックの末尾を削除
    case 0b01001000: {
        if (nums.empty()) {
            cout << "エラー: 数値コンテナが空のまま取り出そうとしています。" << endl;
            exit(0);
        }
        nums.pop_back();
        break;
    }

    // 数値スタックの末尾をNにコピー
    case 0b01001001: {
        if (nums.empty()) {
            cout << "エラー: 数値コンテナが空のまま参照しようとしています。" << endl;
            exit(0);
        }
        N = nums.back();
        break;
    }

    // 数値スタックの末尾とNをswap
    case 0b01001100: {
        if (nums.empty()) {
            cout << "エラー: 数値コンテナが空のまま交換しようとしています。" << endl;
            exit(0);
        }
        swap(N, nums.back());
        break;
    }

    /* -------------------- 文字と文字列操作 -------------------- */
    // Sの末尾にCを追加
    case 0b11000100: {
        S.push_back(C);
        break;
    }

    // Sの末尾を削除
    case 0b11001000: {
        if (S.empty()) {
            cout << "エラー: 文字列が空のまま末尾を削除しようとしています。" << endl;
            exit(0);
        }
        S.pop_back();
        break;
    }

    // Sの末尾をCにコピー
    case 0b11001001: {
        if (S.empty()) {
            cout << "エラー: 文字列が空のまま末尾を文字に移そうとしています。" << endl;
            exit(0);
        }
        C = S.back();
        break;
    }

    /* -------------------- リテラル処理 -------------------- */
    // 文字リテラルの処理
    case 0b11000000: {
        charstate = -1;
        break;
    }

    // 数値リテラルの処理
    case 0b01000000: {
        N = 0;
        numstate = -1;
        break;
    }

    // Sを空文字列にする
    case 0b10000000: {
        S.clear();
        break;
    }

    /* -------------------- 数値の演算 -------------------- */
    // numsの先頭をNに加算
    case 0b01110000: {
        if (nums.empty()) {
            cout << "エラー: 数値コンテナが空のまま加算しようとしています。" << endl;
            exit(0);
        }
        N += nums.back();
        break;
    }

    // numsの先頭でNを減算
    case 0b01110001: {
        if (nums.empty()) {
            cout << "エラー: 数値コンテナが空のまま減算しようとしています。" << endl;
            exit(0);
        }
        N -= nums.back();
        break;
    }

    // numsの先頭でNを乗算
    case 0b01110010: {
        if (nums.empty()) {
            cout << "エラー: 数値コンテナが空のまま乗算しようとしています。" << endl;
            exit(0);
        }
        N *= nums.back();
        break;
    }

    // numsの先頭でNを除算
    case 0b01110011: {
        if (nums.empty()) {
            cout << "エラー: 数値コンテナが空のまま除算しようとしています。" << endl;
            exit(0);
        }
        N /= nums.back();
        break;
    }

    // numsの先頭でNを剰余算
    case 0b01110100: {
        if (nums.empty()) {
            cout << "エラー: 数値コンテナが空のまま剰余算しようとしています。" << endl;
            exit(0);
        }
        N %= nums.back();
        break;
    }

    /* -------------------- 文字列の演算 -------------------- */
    // Sの長さをNに保持
    case 0b10110000: {
        N = S.size();
        break;
    }

    // Sを反転
    case 0b10110011: {
        reverse(S.begin(), S.end());
        break;
    }

    /* -------------------- 文字の演算 -------------------- */
    // Cを数値にキャストしてNに保持
    case 0b11110001: {
        N = static_cast<ll>(C);
        break;
    }

    // Cが数値の場合、キャストしてNに保持
    case 0b11110010: {
        N = static_cast<ll>(C) - static_cast<ll>('0');
        break;
    }

    // CにNを加算
    case 0b11110011: {
        C += N;
        break;
    }
    }
    return;
}

int main(int argc, char* argv[]) {
    ifstream ifs;
    ifs.open(argv[1]);

    vector<pair<bool, int>> opes;
    opes.push_back(make_pair(false, 0));

    string str;
    while (!ifs.eof()) {
        getline(ifs, str);
        bool con;
        int var;
        tie(con, var) = parse(str);
        opes.push_back(make_pair(con, var));
    }
    ifs.close();

    int now = opes.size() - 1;
    while (now > 0) {
        bool con;
        int var;
        tie(con, var) = opes[now];

        if (con) {
            if (var > 0) {  // for(N)
                ll time = N;
                int begin = now;
                for (ll t = 0; t < time; ++t) {
                    now = begin;
                    while (true) {
                        bool con;
                        int var;
                        tie(con, var) = opes[now];
                        if (!con) break;
                        execute(var);
                        --now;
                    }
                }
            } else {  // while(N > 0)
                int begin = now;
                while (N > 0) {
                    now = begin;
                    while (true) {
                        bool con;
                        int var;
                        tie(con, var) = opes[now];
                        if (!con) break;
                        execute(var);
                        --now;
                    }
                }
            }
        } else {
            execute(var);
            --now;
        }
    }

    return 0;
}
