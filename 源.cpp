#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
using namespace std;

map<string, int> map_key_guan;//统计32种关键词类型各自的数量
vector<int> case_counter;//统计switch结构中每一组的case数量
vector<pair<int, string>> level_key;//已经打上嵌套等级的关键词
int total_key_counter = 0;//关键词总数
int if_else_counter = 0;//if_else对数量
int if_elseif_else_counter = 0;//if_elseif_else对数量

//初始化关键词map，用于存储关键词出现的次数
void InitMap() {
	string key_guan[32] = { "auto","break","case","char","const","continue","default","do",
		"double","else","enum","extern","float","for","goto","if","int","long","register","return","short","signed",
		"sizeof","static","struct","switch","typedef","union","unsigned","void","volatile","while" };
	for (int i = 0; i < 32; i++)
		map_key_guan.insert(pair<string, int>(key_guan[i], 0));
}
//用于切割关键词片段
int MyCut(char j) {
	if (j == '\t' || j == ' ' || j == '(' || j == ')' || j == ':'
		|| j == '\n' || j == ';' || j == '#' || j == '=' || j == '!' || j == '?'
		|| j == '-' || j == '<' || j == '>') return 0;
	else if (j == '{' || j == '}') return 2;
	else return 1;
}
//读入文件数据并进行切割处理
int MyRead(string address) {
	//读入文件中的数据
	string c;//用来保存读入的单个字符
	string save;//用来保存读入的字符串
	string key_guan;//用来存储所有切分后的单词
	vector<string> unfiltered_key;//用来存储所有可能的关键词
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
	//进行预处理，将所有可能的关键词进行切分
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
	//打上嵌套等级
	int level = 0;//嵌套等级，初始为0
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
//switch-case统计函数
int SwitchCounter(int begin) {
	//统计每组switch中的case数量
	int case_num = 0;//当前switch结构中case数量
	int i = 0;//当前搜索序号
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
//If统计函数
int IfCounter(int begin) {
	//扫描switch情况
	int i = 0;//当前搜索序号
	for (i = begin; i < level_key.size(); i++) {
		if (level_key[i].second == "if") {
			int j;
			int flag = 0;//0表示if结构,1表示else结构,2表示if，else if结构,3表示if，else if，else结构
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
//统计函数，目前只统计if和switch
void Counter() {
	int i = 0;
	//开始统计switch case个数和if结构总数
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
//展示函数
void MyShow(int out_level) {
	//基础要求：输出关键字统计信息
	//进阶要求：输出有几组switch case结构，同时输出每组对应的case个数
	//拔高要求：输出有几组if else结构
	//终极要求：输出有几组if，else if，else结构
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
	//string address = "C:\\Users\\11765\\Source\\Repos\\C++词法分析\\test3.cpp";//传入文件地址
	string address;//传入文件地址
	int out_level = 4;//展示信息的完成等级
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