#include <bits/stdc++.h>
#include <openssl/sha.h>
using namespace std;
inline int read() {
	int x(0), neg(1); char ch(getchar());
	while(!isdigit(ch)) {
		if (ch == '-') neg = -1;
		ch = getchar();
	}
	while(isdigit(ch)) {
		x = (x << 1) + (x << 3) + (ch ^ 48);
		ch = getchar();
	}
	return x * neg;
}
const int MAXM = 1000;                  // Number of Maximum Blocks
const int MAXN = 1000000;               // Number of Maximum Users
string usr[MAXN + 5];                   // All users
struct node{
	string Hash_Total;
	string ele[MAXM + 5];
} bel[2][MAXM + 5];                     // All chunks
int Now = 0;                            // The current No. of chunks
int Lst_Number = 0;
int block[2];                           // Number of blocks now & next
int block_size[2];                      // Number of elements in the block now & next
int id[2];                              // Number of elements updated now & next
string Gossip_Value = "";
string Gossip_Value_Next = "";

// Use SHA256 Hash
inline string sha256(const string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ss << hex << setw(2) << setfill('0') << static_cast<int>(hash[i]);
    }

    return ss.str();
}
inline string Hash(string x) {
	return sha256(x);
}

// Get position of blocks and position in that block
inline pair<int,int> id_bel(int Id, int Num) {
	if (Id % block_size[Num] == 0) {
		return make_pair(Id / block_size[Num], block_size[Num]);
	}
	else {
		return make_pair((Id / block_size[Num]) + 1, Id % block_size[Num]);
	}
}

// Operation of Append
inline void Append_Single_Element() {
	pair<int, int> p1 = id_bel(id[Now] + 1, Now);
	pair<int, int> p2 = id_bel(id[Now ^ 1] + 1, Now ^ 1);
	pair<int, int> p3 = id_bel(id[Now ^ 1] + 2, Now ^ 1);
	if (p1.second == 1) {
		block[Now] += 1;
		bel[Now][p1.first].Hash_Total = "";
	}
	bel[Now][p1.first].ele[p1.second] = usr[id[Now] + 1];
	bel[Now][p1.first].Hash_Total = Hash(bel[Now][p1.first].Hash_Total + usr[id[Now] + 1]);

	if (p2.second == 1) {
		block[Now ^ 1] += 1;
		bel[Now ^ 1][p2.first].Hash_Total = "";
	}
	bel[Now ^ 1][p2.first].ele[p2.second] = usr[id[Now ^ 1] + 1];
	bel[Now ^ 1][p2.first].Hash_Total = Hash(bel[Now ^ 1][p2.first].Hash_Total + usr[id[Now ^ 1] + 1]);

	if (p3.second == 1) {
		block[Now ^ 1] += 1;
		bel[Now ^ 1][p3.first].Hash_Total = "";
	}
	bel[Now ^ 1][p3.first].ele[p3.second] = usr[id[Now ^ 1] + 2];
	bel[Now ^ 1][p3.first].Hash_Total = Hash(bel[Now ^ 1][p3.first].Hash_Total + usr[id[Now ^ 1] + 2]);
	id[Now] += 1;
	id[Now ^ 1] += 2;
}
inline void Append(string value) {
	string Hash_Value = Hash(value);
	usr[id[Now] + 1]  = Hash_Value;
	if (id[Now] == 0) {
		Lst_Number = 1;
		id[Now] = 1;
		id[Now ^ 1] = 1;
		block[Now] = 1;
		block[Now ^ 1] = 1;
		block_size[Now] = 1;
		block_size[Now ^ 1] = 1;
		bel[Now][1].ele[1] = Hash_Value;
		bel[Now ^ 1][1].ele[1] = Hash_Value;
		bel[Now][1].Hash_Total = Hash(Hash_Value);
		bel[Now ^ 1][1].Hash_Total = Hash(Hash_Value);
		return ;
	}
	if (id[Now] == 1) {
		id[Now] = 2;
		id[Now ^ 1] = 2;
		block[Now] = 2;
		block[Now ^ 1] = 2;
		block_size[Now] = 1;
		block_size[Now ^ 1] = 1;
		bel[Now][2].ele[1] = Hash_Value;
		bel[Now ^ 1][2].ele[1] = Hash_Value;
		bel[Now][2].Hash_Total = Hash(Hash_Value);
		bel[Now ^ 1][2].Hash_Total = Hash(Hash_Value);
		return ;
	}
	if (id[Now] == (Lst_Number * 2)) {
		Lst_Number = id[Now];
		Now ^= 1;
		id[Now ^ 1] = 0;
		block[Now ^ 1] = 0;
		block_size[Now ^ 1] = (int)(sqrt(id[Now] * 2));
	}
	Append_Single_Element();
}

// Get the Gossiped Value
inline void Gossip() {
	string Value = "";
	for (int i = 1; i <= block[Now] - 1; ++ i) {
		Value = Hash(Value + bel[Now][i].Hash_Total);
	}
	string tmp_Value = "";
	for (int i = 1; i <= id_bel(id[Now], Now).second; ++ i) {
		tmp_Value = Hash(tmp_Value + bel[Now][block[Now]].ele[i]);
	}
	Value = Hash(Value + tmp_Value);
	Gossip_Value = Value;
}
inline string Total_Version(int ver) {
	string Value = "", tmp_Value = "";
	pair<int, int> p = id_bel(ver, Now); 
	for (int i = 1; i <= p.first - 1; ++ i) {
		Value = Hash(Value + bel[Now][i].Hash_Total);
	}
	for (int i = 1; i <= p.second; ++ i) {
		tmp_Value = Hash(tmp_Value + bel[Now][p.first].ele[i]);
	}
	Value = Hash(Value + tmp_Value);
	return Value;
}

// Get the Next Gossiped Value
inline void Gossip_Next() {
	string Value = "";
	for (int i = 1; i <= block[Now ^ 1] - 1; ++ i) {
		Value = Hash(Value + bel[Now ^ 1][i].Hash_Total);
	}
	string tmp_Value = "";
	for (int i = 1; i <= id_bel(id[Now ^ 1], Now ^ 1).second; ++ i) {
		tmp_Value = Hash(tmp_Value + bel[Now ^ 1][block[Now ^ 1]].ele[i]);
	}
	Value = Hash(Value + tmp_Value);
	Gossip_Value_Next = Value;
}
inline string Total_Version_Next(int ver) {
	string Value = "", tmp_Value = "";
	pair<int, int> p = id_bel(ver, Now ^ 1); 
	for (int i = 1; i <= p.first - 1; ++ i) {
		Value = Hash(Value + bel[Now ^ 1][i].Hash_Total);
	}
	for (int i = 1; i <= p.second; ++ i) {
		tmp_Value = Hash(tmp_Value + bel[Now ^ 1][p.first].ele[i]);
	}
	Value = Hash(Value + tmp_Value);
	return Value;
}

// Membership Proof
vector<string> blk, elem;
inline void Membership_Proof(int Id) {
	blk.clear();
	elem.clear();
	pair<int, int> p = id_bel(Id, Now);
	for (int i = 1; i <= block[Now]; ++ i) {
		if (p.first == i) {
			if (i != block[Now]) {
				for (int j = 1; j <= block_size[Now]; ++ j) {
					if (j != p.second) elem.push_back(bel[Now][i].ele[j]);
				}
			}
			else {
				for (int j = 1; j <= id_bel(id[Now], Now).second; ++ j) {
					if (j != p.second) elem.push_back(bel[Now][i].ele[j]);
				}
			}
		}
		else {
			blk.push_back(bel[Now][i].Hash_Total);
		}
	}
}
inline bool Client_Membership_Proof(vector<string> a, vector<string> b, int Id, string STR) {
	string Value = "";
	pair<int, int> p = id_bel(Id, Now);
	for (int i = 1; i < p.first; ++ i) {
		Value = Hash(Value + a[i - 1]); 
	}

	string tmp_Value = "";
	for (int i = 1; i < p.second; ++ i) {
		tmp_Value = Hash(tmp_Value + b[i - 1]);
	}
	tmp_Value = Hash(tmp_Value + usr[Id]);
	for (int i = p.second; i <= b.size(); ++ i) {
		tmp_Value = Hash(tmp_Value + b[i - 1]);
	}
	Value = Hash(Value + tmp_Value);

	for (int i = p.first; i <= a.size(); ++ i) {
		Value = Hash(Value + a[i - 1]); 
	}

	if (Value == Gossip_Value) {
		return true;
	}
	else {
		return false;
	}
}

// Membership Proof (Next)
inline void Membership_Proof_Next(int Id) {
	blk.clear();
	elem.clear();
	for (int i = 1; i <= block[Now ^ 1]; ++ i) {
		pair<int, int> p = id_bel(Id, Now ^ 1);
		if (p.first == i) {
			if (i != block[Now ^ 1]) {
				for (int j = 1; j <= block_size[Now ^ 1]; ++ j) {
					if (j != p.second) elem.push_back(bel[Now ^ 1][i].ele[j]);
				}
			}
			else {
				// puts("in");
				for (int j = 1; j <= id_bel(id[Now ^ 1], Now ^ 1).second; ++ j) {
					if (j != p.second) elem.push_back(bel[Now ^ 1][i].ele[j]);
				}
			}
		}
		else {
			blk.push_back(bel[Now ^ 1][i].Hash_Total);
		}
	}
}
inline bool Client_Membership_Proof_Next(vector<string> a, vector<string> b, int Id, string STR) {
	string Value = "";
	pair<int, int> p = id_bel(Id, Now ^ 1);
	for (int i = 1; i < p.first; ++ i) {
		Value = Hash(Value + a[i - 1]); 
	}

	string tmp_Value = "";
	for (int i = 1; i < p.second; ++ i) {
		tmp_Value = Hash(tmp_Value + b[i - 1]);
	}
	tmp_Value = Hash(tmp_Value + STR);
	for (int i = p.second; i <= b.size(); ++ i) {
		tmp_Value = Hash(tmp_Value + b[i - 1]);
	}
	Value = Hash(Value + tmp_Value);

	for (int i = p.first; i <= a.size(); ++ i) {
		Value = Hash(Value + a[i - 1]); 
	}

	if (Value == Gossip_Value_Next) {
		return true;
	}
	else {
		return false;
	}
}

// Simplified Extension Proof
string Start = "", End = "";
vector<string> fir, sec, thi, fou;

/*
"Start" refers to Hash of all the blocks before ver1
"End" refers to Hash of all the elements before ver2
"fir" refers to Hash of all the elements before ver1 in the block
"sec" refers to Hash of all the elements between ver1 and ver2 in ver1's block
"thi" refers to Hash of all the blocks between ver1 and ver2 (the blocks of ver1 and ver2 are not included)
"fou" refers to Hash of all the elements before ver2 (include Hash of ver2) in ver2's block
*/

inline void Extension_Proof(int ver1, int ver2) {
	pair<int, int> p1 = id_bel(ver1, Now);
	pair<int, int> p2 = id_bel(ver2, Now);
	Start = "";
	End = Total_Version(ver2);
	fir.clear();
	sec.clear();
	thi.clear();
	fou.clear();
	if (p1.first != 1) {
		Start = Total_Version((p1.first - 1) * block_size[Now]);
	}
	for (int i = 1; i <= p1.second; ++ i) {
		fir.push_back(bel[Now][p1.first].ele[i]);
	}
	if (p1.first != p2.first) {
		for (int i = p1.second + 1; i <= block_size[Now]; ++ i) {
			sec.push_back(bel[Now][p1.first].ele[i]);
		}
		for (int i = p1.first + 1; i < p2.first; ++ i) {
			thi.push_back(bel[Now][i].Hash_Total);
		}
		for (int i = 1; i <= p2.second; ++ i) {
			fou.push_back(bel[Now][p2.first].ele[i]);
		}
	}
	else {
		for (int i = p1.second + 1; i <= p2.second; ++ i) {
			sec.push_back(bel[Now][p1.first].ele[i]);
		}
	}
}
inline bool Auditor_Extension_Proof(int ver1, int ver2, string st, string en, vector<string> a, vector<string> b, vector<string> c, vector<string> d) {
	string tmp_Value = "";
	string Value = "";
	for (int i = 0; i < a.size(); ++ i) {
		tmp_Value = Hash(tmp_Value + a[i]);
	}
	Value = Hash(st + tmp_Value);
	if (Value != Total_Version(ver1)) {
		// puts("in");
		return false;
	}

	for (int i = 0; i < b.size(); ++ i) {
		tmp_Value = Hash(tmp_Value + b[i]);
	}
	Value = Hash(st + tmp_Value);
	for (int i = 0; i < c.size(); ++ i) {
		Value = Hash(Value + c[i]);
	}
	tmp_Value = "";
	if (d.size() != 0) {
		for (int i = 0; i < d.size(); ++ i) {
			tmp_Value = Hash(tmp_Value + d[i]);
		}
	}
	if (tmp_Value != "") {
		Value = Hash(Value + tmp_Value);
	}
	if (Value != en) {
		return false;
	}
	else {
		return true;
	}
}

// Simplified Extension Proof (Next)
inline void Extension_Proof_Next(int ver1, int ver2) {
	pair<int, int> p1 = id_bel(ver1, Now ^ 1);
	pair<int, int> p2 = id_bel(ver2, Now ^ 1);
	Start = "";
	End = Total_Version_Next(ver2);
	fir.clear();
	sec.clear();
	thi.clear();
	fou.clear();
	if (p1.first != 1) {
		Start = Total_Version_Next((p1.first - 1) * block_size[Now ^ 1]);
	}
	for (int i = 1; i <= p1.second; ++ i) {
		fir.push_back(bel[Now ^ 1][p1.first].ele[i]);
	}
	if (p1.first != p2.first) {
		for (int i = p1.second + 1; i <= block_size[Now ^ 1]; ++ i) {
			sec.push_back(bel[Now ^ 1][p1.first].ele[i]);
		}
		for (int i = p1.first + 1; i < p2.first; ++ i) {
			thi.push_back(bel[Now ^ 1][i].Hash_Total);
		}
		for (int i = 1; i <= p2.second; ++ i) {
			fou.push_back(bel[Now ^ 1][p2.first].ele[i]);
		}
	}
	else {
		for (int i = p1.second + 1; i <= p2.second; ++ i) {
			sec.push_back(bel[Now ^ 1][p1.first].ele[i]);
		}
	}
}
inline bool Auditor_Extension_Proof_Next(int ver1, int ver2, string st, string en, vector<string> a, vector<string> b, vector<string> c, vector<string> d) {
	string tmp_Value = "";
	string Value = "";
	for (int i = 0; i < a.size(); ++ i) {
		tmp_Value = Hash(tmp_Value + a[i]);
	}
	Value = Hash(st + tmp_Value);
	if (Value != Total_Version_Next(ver1)) {
		// puts("in");
		return false;
	}

	for (int i = 0; i < b.size(); ++ i) {
		tmp_Value = Hash(tmp_Value + b[i]);
	}
	Value = Hash(st + tmp_Value);
	for (int i = 0; i < c.size(); ++ i) {
		Value = Hash(Value + c[i]);
	}
	tmp_Value = "";
	if (d.size() != 0) {
		for (int i = 0; i < d.size(); ++ i) {
			tmp_Value = Hash(tmp_Value + d[i]);
		}
	}
	if (tmp_Value != "") {
		Value = Hash(Value + tmp_Value);
	}
	if (Value != en) {
		return false;
	}
	else {
		return true;
	}
}

inline void Debug() {
	cout << "Total Users: " << id[Now] << " " << id[Now ^ 1] << endl;
	cout << "Number of Blocks: " << block[Now] << " " << block[Now ^ 1] << endl;
	cout << "Block Size: " << block_size[Now] << " " << block_size[Now ^ 1] << endl;
	cout << "Last Number: " << Lst_Number << endl;
	cout << "Sequence_Now: \n";
	for (int j = 1; j <= block[Now]; ++j) {
		cout << j << ": ";
		int tmp = block_size[Now];
		if (j != block[Now]) tmp = id[Now] - (j - 1) * block_size[Now];
		for (int k = 1; k <= tmp; ++k) {
			cout << bel[Now][j].ele[k] << " ";
		}
		puts("");
	}
	cout << "Sequence_Last: \n";
	for (int j = 1; j <= block[Now ^ 1]; ++j) {
		cout << j << ": ";
		int tmp = block_size[Now ^ 1];
		if (j != block[Now ^ 1]) tmp = id[Now ^ 1] - (j - 1) * block_size[Now ^ 1];
		for (int k = 1; k <= tmp; ++k) {
			cout << bel[Now ^ 1][j].ele[k] << " ";
		}
		puts("");
	}
	puts("");
}
int n;
signed main() {
	ios::sync_with_stdio(false);
	freopen("ATS_1.in", "r", stdin);
	freopen("ATS_1.out", "w", stdout);
	
	cin >> n;
	for (int i = 1; i <= n; ++ i) {
		// puts("Test");
		string opt;
		cin >> opt;
		if (opt == "Append") {
			// puts("Test 1");
			string str;
			cin >> str;
			Append(str);
		}
		else if (opt == "Membership_Proof") {
			puts("Test 2");
			int Id = 0;
			cin >> Id;
			if (Id > id[Now]) {
				puts("Invalid");
			}
			else {
				Membership_Proof(Id);
				Gossip();
				puts(Client_Membership_Proof(blk, elem, Id, usr[Id]) ? "True" : "False");
			}
		}
		else if (opt == "Membership_Proof_Next") {
			puts("Test 3");
			// Debug();
			int Id;
			cin >> Id;
			if (Id > id[Now ^ 1]) {
				puts("Invalid");
			}
			else {
				Membership_Proof_Next(Id);
				Gossip_Next();
				// cout << Gossip_Value_Next << endl;
				// cout << blk.size() << ' ' << elem.size() << endl;
				puts(Client_Membership_Proof_Next(blk, elem, Id, usr[Id]) ? "True" : "False");
			}
		}
		else if (opt == "Extension_Proof") {
			puts("Test 4");
			int ver1, ver2;
			cin >> ver1 >> ver2;
			if (ver1 > ver2 || ver2 > id[Now]) {
				puts("Invalid");
			}
			else {
				Extension_Proof(ver1, ver2);
				// Debug();
				/*
				cout << bel[Now][1].Hash_Total << endl;
				cout << ver1 << ' ' << ver2 << endl;
				
				cout << Total_Version(8) << endl;
				cout << Start << ' ' << End << endl;
				puts("First: ");
				for (int j = 0; j < fir.size(); ++ j) {
					cout << fir[j] <<' ';
				}
				puts("");
				puts("Second:");
				for (int j = 0; j < sec.size(); ++ j) {
					cout << sec[j] <<' ';
				}
				puts("");
				puts("Third:");
				for (int j = 0; j < thi.size(); ++ j) {
					cout << thi[j] <<' ';
				}
				puts("");
				puts("Fourth:");
				for (int j = 0; j < fou.size(); ++ j) {
					cout << fou[j] <<' ';
				}
				*/
				puts(Auditor_Extension_Proof(ver1, ver2, Start, End, fir, sec, thi, fou) ? "True" : "False");
			}
		}
		else if (opt == "Extension_Proof_Next") {
			puts("Test 5");
			int ver1, ver2;
			cin >> ver1 >> ver2;
			if (ver1 > ver2 || ver2 > id[Now ^ 1]) {
				puts("Invalid");
			}
			else {
				Extension_Proof_Next(ver1, ver2);
				puts(Auditor_Extension_Proof_Next(ver1, ver2, Start, End, fir, sec, thi, fou) ? "True" : "False");
			}
		}
		else {
			puts("Invalid");
		}
	}
}
