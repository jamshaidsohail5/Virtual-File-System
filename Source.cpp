#include<stdlib.h>
#include<conio.h>
#include<iostream>
#include<stdio.h>
#include<math.h>
#include<string>
#include<fstream>
#include<unordered_map>
using namespace std;
int main() {

	ofstream file_write("hard.hdd", std::ios::out | std::ios_base::app | ios::binary);
	ifstream check_file("hard.hdd", std::ios::in | std::ios::app | ios::binary);
	int check = 0;
	check_file >> check;
	check_file.close();

	unordered_map<string, unordered_map<string, int>>  directory;
	unordered_map<string, int> dir_temp;
	unordered_map<int, int> fcb;

	directory["local"] = dir_temp;
	string current_dir = "local";
	if (check == 0)
	{
		file_write << 512;
		file_write << " ";
		file_write << 1000000;
		file_write << " ";
		file_write << 0;
		file_write << " ";
		file_write << 3000000;
		file_write << " ";
		for (int i = 0; i < 10000; i++)
		{
			file_write << -2;
			file_write << " ";
		}
	}
	file_write.close();
	ifstream file_read("hard.hdd", std::ios::in | ios::binary);
	int block_size = 0, number_of_blocks = 0, block_index = 0, free_block = 0;
	int file_table[10000];
	file_read >> block_size;
	file_read >> number_of_blocks;
	file_read >> block_index;
	file_read >> free_block;
	if (check != 0) {
		char c = '\0';
		file_read.get(c);
		while (c != '>'&&!file_read.eof()) {
			file_read.get(c);
			unordered_map<string, int> dir_temp;
			if (c == '%')
			{
				char str[20];
				int i = 0;
				while (c != '/'&&!file_read.eof()) {
					file_read.get(c);
					str[i++] = c;
				}
				str[i - 1] = '\0';
				string s = str;
				directory[s] = dir_temp;
				file_read.get(c);
				while (c != '^'&&!file_read.eof()) {
					i = 0;
					while (c != ':'&&!file_read.eof()) {
						str[i++] = c;
						file_read.get(c);
					}
					str[i] = '\0';
					string s1 = str;
					int addr = 0;
					file_read >> addr;
					directory[s][s1] = addr;
					file_read.get(c);
					file_read.get(c);
				}
			}
		}
		int temp = 0;
		file_read >> temp;
		while (temp != -1) {
			file_read >> fcb[temp];
			file_read >> temp;
		}
	}
	for (int i = 0; i < 10000; i++)
		file_read >> file_table[i];
	file_read.close();













	int choice = 0;
	while (choice != 'q' - 48 && choice != 'Q' - 48) {
	K:
		system("cls");
		cout << "Current Directory is usr/" << current_dir << ".";
		cout << "\nPress 1 to Create New Directory";
		cout << "\nPress 2 to Create a File in Current Directory";
		cout << "\nPress 3 to Edit File";
		cout << "\nPress 4 to Delete File";
		cout << "\nPress 5 to Print Directory";
		cout << "\nPress 6 to Change Directory";
		cout << "\nPress 7 to Rename File";
		cout << "\nPress 8 to Rename Folder";
		cout << "\nPress 9 to Delete Folder";
		cout << "\nPress L to Print Few Lines from file";
		cout << "\nPress Q to Exit File Managment System";
		cout << "\nPress any key.";
		choice = _getch() - 48;
		if (choice == 1)
		{
			int flag = 1;
			char str[20];
			unordered_map<string, int> dir_temp;
			while (flag == 1) {
				cout << "\nenter directory name : ";
				cin.getline(str, 20, '\n');
				if (directory.count(str) < 1)
					flag = 0;
				if (flag == 1)
					cout << "\nDirectory already exists";
			}
			current_dir = str;
			directory[str] = dir_temp;
			cout << "\nYou have successfully created a folder";
		}
		else if (choice == 'l' - 48 || choice == 'L' - 48) {
			cout << "\navailable files are\n";
			int i = 1;
			for (auto& x : directory[current_dir])
			{
				cout << i++ << " : ";
				cout << x.first << "\n";
			}
			int flag = 1;
			char str[20];
			while (flag == 1) {
				cout << "\nEnter file name or back to go back: ";
				cin.getline(str, 20, '\n');
				if (strcmp(str, "back") == 0)
					goto K;
				if (directory[current_dir].count(str) > 0)
					flag = 0;
				if (flag == 1)
					cout << "\nFile does not exist";
			}
			int start, end;
			cout << "Enter Starting Line No: ";
			cin >> start;
			cout << "Enter Ending Line No:";
			cin >> end;
			start--;
			end--;
			start *= 50;
			end *= 50;
			int st;
			st = fcb[directory[current_dir][str]];
			int ft = st;
			char * file_str; int k = 0;
			if (file_table[(st - 3000000) / 512] != -2) {
				for (k; ft != -1; k++) {
					ft = file_table[(ft - 3000000) / 512];
				}
				file_str = new char[k * 512];
				ft = st;
				ifstream fin("hard.hdd", std::ios::in | ios::binary);
				for (int i = 0; ft != -1; i++) {
					fin.seekg(ft, ios::beg);
					char *buff = new char[513];
					int j = 0; char b = 0;
					for (; !(buff[j - 2] == '-' && buff[j - 1] == '>') && j < 512; j++) {
						fin.get(b);
						buff[j] = b;
					}
					buff[j - 2] = '\0';
					if (i == 0)
						strcpy(file_str, buff);
					else
						strcat(file_str, buff);
					delete(buff);
					ft = file_table[(ft - 3000000) / 512];
				}
				if (start < strlen(file_str) && start >= 0 && end >= 0 && start < end)
					for (int i = start; i < end&&i < strlen(file_str); i++)
						cout << file_str[i];
				else
					cout << "Invalid Starting or Ending Point";
				fin.close();
			}
		}
		else if (choice == 9)
		{
			cout << "\navailable folders are\n";
			int i = 1;
			for (auto& x : directory)
			{
				cout << i++ << " : ";
				cout << x.first << "\n";
			}
			int flag = 1;
			char str[20];
			bool check = false;
			unordered_map<string, int> dir_temp;
			while (flag == 1)
			{
				cout << "\nEnter Folder name or back to go back: ";
				cin.getline(str, 20, '\n');
				if (strcmp(str, "back") == 0)
					goto K;
				if (strcmp(str, "local") == 0)
				{
					cout << "\nYou Cannot delete local directory";
					check = true;
				}
				else
					check = false;
				if (directory.count(str) >= 1 && flag != 2 && !check)
					flag = 0;
				if (flag == 1 && !check)
					cout << "\nDirectory Doesn't Exists";
			}
			string temp1;
			current_dir = "local";
			for (auto& x : directory[str])
			{
				fcb.erase(directory[str][x.first]);
			}
			directory.erase(str);
			cout << "\nFolder Successfully Deleted.";
		}
		else if (choice == 8)
		{
			bool check = false;
			cout << "\navailable folders are\n";
			int i = 1;
			for (auto& x : directory)
			{
				cout << i++ << " : ";
				cout << x.first << "\n";
			}
			int flag = 1;
			char str[20], str1[20];
			unordered_map<string, int> dir_temp;
			while (flag == 1) {
				cout << "\nEnter file name or back to go back: ";
				cin.getline(str, 20, '\n');
				if (strcmp(str, "back") == 0)
					goto K;
				if (strcmp(str, current_dir.c_str()) == 0)
					check = true;
				else
					check = false;
				if (directory.count(str) >= 1)
					flag = 0;
				if (flag == 1)
					cout << "\nDirectory doesn't exists";
			}
			flag = 1;
			while (flag == 1) {
				cout << "\nEnter New Name : ";
				cin.getline(str1, 20, '\n');
				if (directory.count(str1) < 1)
					flag = 0;
				if (flag == 1)
					cout << "\nDirectory already exists";
			}
			current_dir = str;
			dir_temp = directory[str];
			directory.erase(str);
			directory[str1] = dir_temp;
			if (check)
				current_dir = str1;
			cout << "\nFolder Renamed";
		}
		else if (choice == 2) {
			int flag = 1;
			bool check = false, check1 = false;
			char str[20];
			while (flag != 0) {
				cout << "\nEnter file name or back to go back: ";
				cin.getline(str, 20, '\n');
				if (strcmp(str, "back") == 0)
					goto K;
				if (directory[current_dir].count(str) < 1)
					check = true;
				else
					check = false;
				int l = strlen(str);
				if (l >= 5 && str[l - 1] == 't' && str[l - 2] == 'x' && str[l - 3] == 't' && str[l - 4] == '.')
					check1 = true;
				else
					check1 = false;
				if (!check1)
					cout << "\nFile name not specified";
				if (!check)
					cout << "\nFile already exists";
				if (check&&check1)
					flag = 0;
			}
			cout << "\nyou have successfully created a file";
			directory[current_dir][str] = block_index;
			fcb[block_index] = free_block;
			block_index++;
		}
		else if (choice == 3) {
			cout << "\navailable files are\n";
			int i = 1;
			for (auto& x : directory[current_dir])
			{
				cout << i++ << " : ";
				cout << x.first << "\n";
			}
			int flag = 1;
			char str[20];
			while (flag == 1) {
				cout << "\nEnter file name and back to go back: ";
				cin.getline(str, 20, '\n');
				if (strcmp(str, "back") == 0)
					goto K;
				if (directory[current_dir].count(str) > 0)
					flag = 0;
				if (flag == 1)
					cout << "\nFile does not exist";
			}

			ofstream myfile1;
			myfile1.open(str);
			int st;
			st = fcb[directory[current_dir][str]];
			int ft = st;
			char * file_str; int k = 0;
			if (file_table[(st - 3000000) / 512] != -2) {
				for (k; ft != -1; k++) {
					ft = file_table[(ft - 3000000) / 512];
				}
				file_str = new char[k * 512];
				ft = st;
				ifstream fin("hard.hdd", std::ios::in | ios::binary);
				for (int i = 0; ft != -1; i++) {
					fin.seekg(ft, ios::beg);
					char *buff = new char[513];
					int j = 0; char b = 0;
					for (; !(buff[j - 2] == '-' && buff[j - 1] == '>') && j < 512; j++) {
						fin.get(b);
						buff[j] = b;
					}
					buff[j - 2] = '\0';
					if (i == 0)
						strcpy(file_str, buff);
					else
						strcat(file_str, buff);
					delete(buff);
					ft = file_table[(ft - 3000000) / 512];
				}

				myfile1 << file_str;
				fin.close();
			}

			myfile1.close();
			char command[20] = "notepad.exe ";
			strcat(command, str);
			system(command);
			ifstream t(str);
			int length1 = 0;
			t.seekg(0, std::ios::end);
			int length = t.tellg();
			t.seekg(0, std::ios::beg);
			char *buffer = new char[length];
			while (!t.eof())
			{
				t.get(buffer[length1]);
				length1++;
			}
			buffer[length1 - 1] = '\0';
			int arr_size = 1;

			if (length1 >= 480)
				arr_size = length1 / 480 + (length1 % 480 != 0);
			char **splited_array = new char*[arr_size];
			for (int i = 0; i < arr_size; i++) {
				splited_array[i] = new char[471];
				strncpy(splited_array[i], buffer + (i * 470), 470);
				splited_array[i][470] = '\0';
			}
			int start = fcb[directory[current_dir][str]];
			if (file_table[(start - 3000000) / 512] == -2) {

				for (int i = 1; i < arr_size; i++) {
					free_block += 512;
					if (i == 1)
						file_table[(start - 3000000) / 512] = free_block;
					else
						file_table[((free_block - 3000000) / 512) - 1] = free_block;

				}
				file_table[(free_block - 3000000) / 512] = -1;
				free_block += 512;
			}
			else {
				int st = start;
				int i = 1, ft = 0;
				for (; st != -1; i++) {
					ft = st;
					st = file_table[((st - 3000000) / 512)];
				}
				if (arr_size > i) {
					file_table[(ft - 3000000) / 512] = free_block;
					for (; i < arr_size; i++) {
						free_block += 512;
						file_table[((free_block - 3000000) / 512) - 1] = free_block;
					}
					file_table[(free_block - 3000000) / 512] = -1;
					free_block += 512;
				}
				else if (arr_size < i) {
					i = 1;
					st = start;
					for (; i < arr_size; i++)
						st = file_table[((st - 3000000) / 512)];
					file_table[((st - 3000000) / 512)] = -1;
				}

			}
			int write = start;
			int index = 0;
			ofstream myfile("hard.hdd", ios::out | ios::in | ios::binary);
			for (; write != -1; index++)
			{
				myfile.seekp(write);
				myfile << splited_array[index];
				myfile.put('-');
				myfile.put('>');
				int f = myfile.tellp();
				write = file_table[((write - 3000000) / 512)];
			}
			myfile.close();
			t.close();
			remove(str);
			cout << "File edited successfully";

		}
		else if (choice == 4) {
			cout << "\navailable files are\n";
			int i = 1;
			for (auto& x : directory[current_dir])
			{
				cout << i++ << " : ";
				cout << x.first << "\n";
			}
			int flag = 1;
			char str[20];
			while (flag == 1) {
				cout << "\nEnter file name or back to go back: ";
				cin.getline(str, 20, '\n');
				if (strcmp(str, "back") == 0)
					goto K;
				if (directory[current_dir].count(str) > 0)
					flag = 0;
				if (flag == 1)
					cout << "\nFile does not exist";
			}
			fcb.erase(directory[current_dir][str]);
			directory[current_dir].erase(str);
			cout << "\nFile deleted successfully";
		}
		else if (choice == 5) {
			cout << "\n--------------------Directory------------------\n";
			cout << "\nRoot\t\tFolders\t\tFiles\n";
			cout << "\nusr : \n";
			for (auto& x : directory) {
				cout << "\t\t" << x.first << " : ";
				for (auto& y : x.second) {
					cout << "\n\t\t\t\t" << y.first << " : " << y.second;
				}
				cout << "\n";
			}
		}
		else if (choice == 6) {
			int flag = 1;
			char str[20];
			while (flag == 1) {
				cout << "\navailable directories are\n";
				int i = 1;
				for (auto& x : directory)
				{
					cout << i++ << " : ";
					cout << x.first << "\n";
				}
				cout << "\nEnter Folder Name or Back to go back: ";
				cin.getline(str, 20, '\n');
				if (strcmp(str, "back") == 0)
					goto K;
				for (auto& x : directory)
				{
					if (x.first == str)
						flag = 0;
				}
				if (flag == 1)
					cout << "\nyour entered directory name is not valid";
			}
			current_dir = str;
		}
		else if (choice == 7) {
			cout << "\navailable files are\n";
			int i = 1;
			for (auto& x : directory[current_dir])
			{
				cout << i++ << " : ";
				cout << x.first << "\n";
			}
			int flag = 1;
			char str[20], str1[20];
			while (flag == 1) {
				cout << "\nEnter file to rename : ";
				cin.getline(str, 20, '\n');
				if (directory[current_dir].count(str) > 0)
					flag = 0;
				if (flag == 1)
					cout << "\nFile does not exist";
			}
			flag = 1;
			bool check = false, check1 = false;
			while (flag != 0) {
				cout << "\nEnter file name or back to go back: ";
				cin.getline(str1, 20, '\n');
				if (strcmp(str1, "back") == 0)
					goto K;
				if (directory[current_dir].count(str1) < 1)
					check = true;
				else
					check = false;
				int l = strlen(str1);
				if (l >= 5 && str1[l - 1] == 't' && str1[l - 2] == 'x' && str1[l - 3] == 't' && str1[l - 4] == '.')
					check1 = true;
				else
					check1 = false;
				if (!check1)
					cout << "\nFile name not specified";
				if (!check)
					cout << "\nFile already exists";
				if (check&&check1)
					flag = 0;
			}
			int temp = directory[current_dir][str];
			directory[current_dir].erase(str);
			directory[current_dir][str1] = temp;
			cout << "\nFile renamed successfully";
		}
		if (choice != 'q' - 48 && choice != 'Q' - 48)
			_getch();
	}
	fstream rewrite("hard.hdd", std::ios::out | std::ios::in | std::ios_base::ate | ios::binary);
	rewrite.seekp(0, std::ios::beg);
	rewrite << block_size;
	rewrite << " ";
	rewrite << number_of_blocks;
	rewrite << " ";
	rewrite << block_index;
	rewrite << " ";
	rewrite << free_block;
	rewrite << " ";

	for (auto& x : directory) {
		rewrite << "%";
		rewrite << x.first << "/";
		for (auto& y : x.second) {
			rewrite << y.first;
			rewrite << ":";
			rewrite << y.second << ",";
		}
		rewrite.put('^');

	}
	rewrite.put('>');
	for (auto& x : fcb) {
		rewrite << x.first;
		rewrite << " ";
		rewrite << x.second << " ";
	}
	rewrite << -1;
	rewrite << " ";
	for (int i = 0; file_table[i] != -2; i++) {
		rewrite << file_table[i];
		rewrite << " ";
	}
	rewrite.close();
	return 0;
}