#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
using namespace std;

map<string, int> map_key_guan;//ͳ��32�ֹؼ������͸��Ե�����
vector<int> case_counter;//ͳ��switch�ṹ��ÿһ���case����
vector<pair<int, string>> level_key;//�Ѿ�����Ƕ�׵ȼ��Ĺؼ���
int total_key_counter = 0;//�ؼ�������
int if_else_counter = 0;//if_else������
int if_elseif_else_counter = 0;//if_elseif_else������

//��ʼ���ؼ���map�����ڴ洢�ؼ��ʳ��ֵĴ���
void InitMap() {
	string key_guan[32] = { "auto","break","case","char","const","continue","default","do",
		"double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed",
		"sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while" };
	for (int i = 0; i < 32; i++)
		map_key_guan.insert(pair<string, int>(key_guan[i], 0));
}
//�����и�ؼ���Ƭ��
int MyCut(char j) {
	if (j == '\t' || j == ' ' || j == '(' || j == ')' || j == ':'
		|| j == '\n' || j == ';' || j == '#' || j == '=' || j == '!' || j == '?'
		|| j == '-' || j == '<' || j == '>') return 0;
	else if (j == '{' || j == '}') return 2;
	else return 1;
}
//�����ļ����ݲ������и��
int MyRead(string address) {
	//�����ļ��е�����
	string c;//�����������ĵ����ַ�
	string save;//�������������ַ���
	string key_guan;//�����洢�����зֺ�ĵ���
	vector<string> unfiltered_key;//�����洢���п��ܵĹؼ���
	ifstream infile(address);
	cout << "Reading..." << endl;
	if (!infile.is_open()) {
		cout << "false" << endl;
		return 0;
	}
	while (!infile.eof()) {
		c = infile.get();
		save.append(c);
	}
	save[save.length() - 1] = '\0';
	infile.close();
	//����Ԥ���������п��ܵĹؼ��ʽ����з�
	for (int i = 0; i < save.length(); i++) {
		if (MyCut(save[i]) == 1) {
			key_guan.push_back(save[i]);
		}
		else if (MyCut(save[i]) == 2) {
			string s(1, save[i]);
			unfiltered_key.push_back(s);
		}
		else {
			if (key_guan.length() > 1) {
				if (map_key_guan.find(key_guan) != map_key_guan.end()) {
					map_key_guan[key_guan] += 1;
					unfiltered_key.push_back(key_guan);
					total_key_counter++;
				}
			}
			key_guan.clear();
		}
	}
	//����Ƕ�׵ȼ�
	int level = 0;//Ƕ�׵ȼ�����ʼΪ0
	pair<int, string> temp;
	for (int i = 0; i < unfiltered_key.size(); i++) {
		if (unfiltered_key[i] == "{") {
			level++;

		}
		else if (unfiltered_key[i] == "}") {
			level--;
		}
		temp.first = level;
		temp.second = unfiltered_key[i];
		level_key.push_back(temp);
	}
	return 1;
}
//switch-caseͳ�ƺ���
int SwitchCounter(int begin) {
	//ͳ��ÿ��switch�е�case����
	int case_num = 0;//��ǰswitch�ṹ��case����
	int i = 0;//��ǰ�������
	for (i = begin; i < level_key.size(); i++) {
		if (level_key[i].second == "switch") {
			int j;
			for (j = i + 1; j < level_key.size() && level_key[j].first > level_key[i].first; j++) {
				if (level_key[j].second == "case") case_num++;
				else if (level_key[j].second == "switch") {
					j = SwitchCounter(j);
				}
			}
			case_counter.push_back(case_num);
			return j;
		}
	}
	return i;
}
//Ifͳ�ƺ���
int IfCounter(int begin) {
	//ɨ��switch���
	int i = 0;//��ǰ�������
	for (i = begin; i < level_key.size(); i++) {
		if (level_key[i].second == "if") {
			int j;
			int flag = 0;//0��ʾif�ṹ,1��ʾelse�ṹ,2��ʾif��else if�ṹ,3��ʾif��else if��else�ṹ
			for (j = i + 1; j < level_key.size() && level_key[j].first >= level_key[i].first; j++) {
				if (level_key[j].second == "else" && level_key[j + 1].second == "if") {
					if (flag == 0) {
						flag = 2;
					}
					j++;
				}
				else if (level_key[j].second == "else") {
					if (flag == 0) {
						if_else_counter++;
					}
					else if (flag == 2) {
						if_elseif_else_counter++;
					}
					break;
				}
				else if (level_key[j].second == "if") {
					j = IfCounter(j);
				}
			}
			return j;
		}
	}
	return i;
}
//ͳ�ƺ�����Ŀǰֻͳ��if��switch
void Counter() {
	int i = 0;
	//��ʼͳ��switch case������if�ṹ����
	while (1) {
		i = SwitchCounter(i);
		if (i == level_key.size()) break;
	}
	i = 0;
	while (1) {
		i = IfCounter(i);
		if (i == level_key.size()) break;
	}
}
//չʾ����
void MyShow(int out_level) {
	//����Ҫ������ؼ���ͳ����Ϣ
	//����Ҫ������м���switch case�ṹ��ͬʱ���ÿ���Ӧ��case����
	//�θ�Ҫ������м���if else�ṹ
	//�ռ�Ҫ������м���if��else if��else�ṹ
	if (out_level >= 1)
		cout << "total num: " << total_key_counter << endl;
	if (out_level >= 2) {
		cout << "switch num: " << map_key_guan["switch"] << endl;
		cout << "case num:";
		for (int i = 0; i < case_counter.size(); i++) {
			cout << " " << case_counter[i];
		}
		cout << endl;
	}
	if (out_level >= 3) {
		cout << "if-else num: " << if_else_counter << endl;
	}
	if (out_level == 4) {
		cout << "if-elseif-else num: " << if_elseif_else_counter << endl;
	}
}

int main() {
	InitMap();
	//string address = "C:\\Users\\11765\\Source\\Repos\\C++�ʷ�����\\test3.cpp";//�����ļ���ַ
	string address;//�����ļ���ַ
	int out_level = 4;//չʾ��Ϣ����ɵȼ�
	cout << "Please Enter file address and output level: " << endl;
	cin >> address;
	cin >> out_level;
	if (MyRead(address)) {
		Counter();
		MyShow(out_level);
	}
	else {
		cout << "fail in reading file" << endl;
	}
	return 0;
}