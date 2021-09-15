#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
using namespace std;
map<string, int> MapKeyGuan;

int MyCut(char j) {
	if (j == ' ' || j == '(' || j == ')' || j == '{' || j == '}' || j == ':' 
		|| j == '\n' || j == ';' || j == '#' || j == '=' || j == '!' || j == '?' 
		|| j == '-' || j == '<' || j == '>' || (j <=57 &&j>=48)) return 0;
	else return 1;
}
void InitMap() {
	string KeyGuan[32] = { "auto","break","case","char","const","continue","default","do",
		"double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed",
		"sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while" };
	for (int i = 0;i<32;i++)
		MapKeyGuan.insert(pair<string, int>(KeyGuan[i], 0));
}
int main() {
	InitMap();
	string data;
	string save;
	string KeyGuan;
	vector<string> zu;
	int count = 0;
	string test = "break";
	int CaseCount = 0;
	ifstream infile("C:\\Users\\11765\\Source\\Repos\\C++´Ê·¨·ÖÎö\\Debug\\test3.cpp");
	cout << "Reading..." << endl;
	if (!infile.is_open()) {
		cout << "false" << endl;
	}
	while (!infile.eof()) {
		data = infile.get();
		save.append(data);
	}
	save[save.length()-1] = '\0';
	for (int i = 0; i < save.length(); i++) {
		if (MyCut(save[i])) {
			KeyGuan.push_back(save[i]);
		}
		else {
			if (KeyGuan.length() > 1) {
				cout << KeyGuan << endl;
				if (MapKeyGuan.find(KeyGuan) != MapKeyGuan.end()) {
					MapKeyGuan[KeyGuan] += 1;
					zu.push_back(KeyGuan);
					count++;
				}
			}
			KeyGuan.clear();
		}
	}
	cout << "total num: " << count << endl;
	cout << "switch num: " << MapKeyGuan["switch"] << endl;
	return 0;
}