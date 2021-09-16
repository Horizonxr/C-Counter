#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
using namespace std;
map<string, int> MapKeyGuan;
vector<string> Zu;
vector<int> case_counter;
int ifelse_counter = 0;
int if_elseif_else_counter = 0;
int MyCut(char j) {
	if (j == ' ' || j == '(' || j == ')' || j == ':'
		|| j == '\n' || j == ';' || j == '#' || j == '=' || j == '!' || j == '?'
		|| j == '-' || j == '<' || j == '>' || (j <= 57 && j >= 48)) return 0;
	else if (j == '{' || j == '}') return 2;
	else return 1;
}
void InitMap() {
	string KeyGuan[32] = { "auto","break","case","char","const","continue","default","do",
		"double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed",
		"sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while" };
	for (int i = 0; i < 32; i++)
		MapKeyGuan.insert(pair<string, int>(KeyGuan[i], 0));
}
void SwitchIfCounter() {
	int flag = 0;
	int caseCount = 0;
	int ifFlag = 0;//Flag=0表示只有if，1表示if-else,2表示if-elseif-else
	for (int i = 0; i < Zu.size(); i++) {
		if (Zu[i] == "switch") {
			for (int j = i; j <= Zu.size(); j++) {
				if (Zu[j] == "case") {
					caseCount++;
				}
				else if (Zu[j] == "}") {
					case_counter.push_back(caseCount);
					caseCount = 0;
					i = j;
					break;
				}
			}
		}
		//这里有点问题要改一下逻辑，存在嵌套的问题，switch或者if互相嵌套，考虑使用堆栈来解决
	}
}
int main() {
	InitMap();
	string data;
	string save;
	string KeyGuan;
	int count = 0;
	string test = "break";
	int CaseCount = 0;
	ifstream infile("C:\\Users\\11765\\Source\\Repos\\C++词法分析\\Debug\\test3.cpp");
	cout << "Reading..." << endl;
	if (!infile.is_open()) {
		cout << "false" << endl;
	}
	while (!infile.eof()) {
		data = infile.get();
		save.append(data);
	}
	save[save.length() - 1] = '\0';
	infile.close();
	for (int i = 0; i < save.length(); i++) {
		if (MyCut(save[i]) == 1) {
			KeyGuan.push_back(save[i]);
		}
		else if (MyCut(save[i]) == 2) {
			string s(1, save[i]);
			Zu.push_back(s);
		}
		else {
			if (KeyGuan.length() > 1) {
				cout << KeyGuan << endl;
				if (MapKeyGuan.find(KeyGuan) != MapKeyGuan.end()) {
					MapKeyGuan[KeyGuan] += 1;
					Zu.push_back(KeyGuan);
					count++;
				}
			}
			KeyGuan.clear();
		}
	}
	//开始统计switch case个数和if结构总数
	SwitchIfCounter();
	cout << "total num: " << count << endl;
	cout << "switch num: " << MapKeyGuan["switch"] << endl;
	cout << "case num:";
	for (int i = 0; i < case_counter.size(); i++) {
		cout << " " << case_counter[i];
	}
	cout << endl;
	return 0;
}