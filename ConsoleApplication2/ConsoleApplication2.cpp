#include <iostream>
#include <vector>
#include <map>
#include <queue>
using namespace std;

#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define REP(i,n) FOR(i, 0, n)
using VI = vector<int>;
using MII = map<int, int>;

MII dict;
constexpr auto InitNum = 3;;   // 設定

int judge_state(const VI &state) {
    if ((state[0] + state[1] + state[2]) == 0)
        return 1;
    else if ((state[3] + state[4] + state[5]) == 0)
        return 0;
    else
        return -1;
}

int mypow(int a, int n) {
    int res = 1;
    while (n > 0) {
        if (n % 2) res *= a;
        a *= a;
        n /= 2;
    }
    return res;
}

int hashkey(const VI& state) {
    int hs = 0;
    REP(i, 6) {
        hs += mypow(InitNum*4, 5-i) * state[i];
    }
    return hs;
}

int hashkey(int a) {
    int hs = 0;
    for(int i=0; i<6; i++) {
        hs += mypow(InitNum*4, i) * (a % 10);
        a /= 10;
        if (a == 0) break;
    }
    return hs;
}

bool add_dict(const VI& state, int k) {
    int hs = hashkey(state);
    if (dict.count(hs))
        return false;
    else {
        dict[hs] = k;
        return true;
    }
}

VI next_state(const VI &state, int k) {
    VI next(6);
    next = state;
    int stone = next[k];
    next[k] = 0;
    bool bonus = false;
    REP(i, stone) {
        int s = k + i + 1;
        if (s % 3 == 0) {
            stone--;
            if (i == stone) {
                bonus = true;
                break;
            }
        }
        next[s%6]++;
    }
    if (!bonus) {
        swap(next[0], next[3]);
        swap(next[1], next[4]);
        swap(next[2], next[5]);
    }
    return next;
}

void make_dict(const VI &initS) {
    queue<VI> que;
    que.push(initS);

    VI state(6);
    while (!que.empty()) {
        state = que.front(); que.pop();
        int k = judge_state(state);
        if (!add_dict(state, k) || k != -1) {
            continue;
        }
        else {
            REP(i, 3) {
                if (state[i] != 0) {
                    que.push(next_state(state, i));
                }
            }
        }
    }
}

bool isBonus(const VI& state, int k) {
    int stone = state[k];
    REP(i, stone) {
        int s = k + i + 1;
        if (s % 3 == 0) {
            stone--;
            if (i == stone)
                return true;
        }
    }
    return false;
}

int det_dict(const VI& state) {
    int win = dict.at(hashkey(state));
    if (win != -1)
        return win;

    VI next(6);
    REP(i, 3) {
        if (state[i] != 0) {
            next = next_state(state, i);
            if (det_dict(next) >= 1) {
                if (isBonus(state, i)) {
                    win = i + 1;
                    break;
                }
            }
            else {
                if (!isBonus(state, i)) {
                    win = i + 1;
                    break;
                }
            }
        }
    }
    if (win == -1) {
        dict[hashkey(state)] = 0;
        return 0;
    }
    else {
        dict[hashkey(state)] = win;
        return win;
    }
}

void init_state(VI &state) {
    REP(i, 6) {
        state[i] = InitNum;
    }
}

int main()
{
    VI state(6);
    init_state(state);

    make_dict(state);
    det_dict(state);

    int st, nx;
    cout << dict.size() << "  Patterns." << endl;
    cout << "終了は 0" << endl;
    while (1) {
        cout << "盤面： ";
        cin >> st;
        if (st == 0) break;
        st = hashkey(st);
        if (dict.count(st)) {
            nx = dict[st];
            if (nx != 0)
                cout << "次の一手は " << nx << endl;
            else
                cout << "負け確" << endl;
        }
        else {
            cout << "No Such State." << endl;
        }
    }
}