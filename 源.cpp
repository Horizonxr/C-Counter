#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
using namespace std;
map<string, int> MapKeyGuan;//�����洢32�ֹؼ�������
vector<string> Zu;//�����洢���йؼ���
vector<int> case_counter;
vector<pair<int, string>> stack;
int KeyCount = 0;
int ifelse_counter = 0;
int if_elseif_else_counter = 0;

int MyCut(char j) {
	if (j == '\t' || j == ' ' || j == '(' || j == ')' || j == ':'
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
int SwitchIfCounter(int begin) {
	//ɨ��switch���
	int caseCount = 0;
	int i = 0;
	for (i = begin; i < stack.size(); i++) {
		if (stack[i].second == "switch") {
			int j;
			for (j = i + 1; j < stack.size() && stack[j].first > stack[i].first; j++) {
				if (stack[j].second == "case") caseCount++;
				else if (stack[j].second == "switch") {
					j = SwitchIfCounter(j);
				}
			}
			case_counter.push_back(caseCount);
			return j;
		}
		else if (stack[i].second == "if") {
			int j;
			int flag = 0;//0��ʾif�ṹ,1��ʾelse�ṹ,2��ʾif��else if�ṹ,3��ʾif��else if��else�ṹ
			for (j = i + 1; j < stack.size() && stack[j].first >= stack[i].first; j++) {
				if (stack[j].second == "else" && stack[j + 1].second == "if") {
					if (flag == 0) {
						flag = 2;
					}
					j++;
				}
				else if (stack[j].second == "else") {
					if (flag == 0) {
						ifelse_counter++;
					}
					else if (flag == 2) {
						if_elseif_else_counter++;
					}
					break;
				}
				else if (stack[j].second == "if") {
					j = SwitchIfCounter(j);
				}
			}
			return j;
		}
	}
	return i;
}
void MyRead() {
	//�����ļ��е�����
	string data;//�����������ĵ����ַ�
	string save;//�������������ַ���
	string KeyGuan;//�����洢�����зֺ�ĵ���
	string test = "break";
	int CaseCount = 0;
	ifstream infile("C:\\Users\\11765\\Source\\Repos\\C++�ʷ�����\\Debug\\test2.cpp");
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
					KeyCount++;
				}
			}
			KeyGuan.clear();
		}
	}
	//����Ƕ�׵ȼ�
	int level = 0;//��������Ƕ�׵ȼ�
	pair<int, string> temp;
	int caseCount = 0;
	//ͨ��������ϵȼ���ʶ
	for (int i = 0; i < Zu.size(); i++) {
		if (Zu[i] == "{") {
			level++;

		}
		else if (Zu[i] == "}") {
			level--;
		}
		temp.first = level;
		temp.second = Zu[i];
		stack.push_back(temp);
	}
}
int main() {
	InitMap();
	MyRead();
	int i = 0;
	//��ʼͳ��switch case������if�ṹ����
	while (1) {
		i = SwitchIfCounter(i);
		if (i == stack.size()) break;
	}

	cout << "total num: " << KeyCount << endl;
	cout << "switch num: " << MapKeyGuan["switch"] << endl;
	cout << "case num:";
	for (int i = 0; i < case_counter.size(); i++) {
		cout << " " << case_counter[i];
	}
	cout << endl;
	cout << "if-else num: " << ifelse_counter <<endl;
	cout << "if-elseif-else num:" << if_elseif_else_counter << endl;
	return 0;
}