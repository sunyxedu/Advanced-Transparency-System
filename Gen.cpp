#include <iostream>
#include <cstdlib>
#include <map>
using namespace std;
int n;
const int MAXN = 1000000;
string user[MAXN + 5];
int cnt = 0;
inline string Generate_User() {
    int len = rand() % 200;
    string str = "";
    for (int i = 1; i <= len; ++i) {
        char ch = (rand() % 26) + 65;
        str = str + ch;
    }
    return str;
}
signed main() {
    freopen("ATS_1.in", "w", stdout);
    srand(time(NULL));
    n = 1000000;
    cout << n << endl;
    puts("Append");
    puts("sunyx");
    user[++cnt] = "sunyx";
    for (int i = 2; i <= n; ++i) {
        int rd = (rand() % 100) + 1;
        if (rd >= 1 and rd <= 80) {
            puts("Append");
            string New_User = Generate_User();
            cout << New_User << endl;
            user[++cnt] = New_User;
        }
        else if (rd > 80 and rd <= 85) {
            puts("Membership_Proof");
            int id = (rand() % cnt) + 1;
            cout << id << endl;
        }
        else if (rd > 85 and rd <= 90) {
            puts("Membership_Proof_Next");
            int id = (rand() % cnt) + 1;
            cout << id << endl;
        }
        else if (rd > 90 and rd <= 95) {
            puts("Extension_Proof");
            int id1 = (rand() % cnt) + 1, id2 = (rand() % cnt) + 1;
            if (id1 > id2) {
                swap(id1, id2);
            }
            cout << id1 << ' ' << id2 << endl;
        }
        else {
            puts("Extension_Proof_Next");
            int id1 = (rand() % cnt) + 1, id2 = (rand() % cnt) + 1;
            if (id1 > id2) {
                swap(id1, id2);
            }
            cout << id1 << ' ' << id2 << endl;
        }
    }
}