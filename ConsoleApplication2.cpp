
#include<iostream>
#include <vector>
#include<string>
#include <sstream>
#include<fstream>
using namespace std;
void readDictionary(vector<string>& buffer)
{
	//vector<string>temp;
	ifstream infile("test.txt", ios::in);
	if (!infile) {
		cout << "File could not be open." << endl;
		system("pause");
		exit(1);
	}
	else {
		string linestr;

		while (getline(infile, linestr)) {
			//cout << linestr<<endl;
			istringstream ss(linestr);
			string item;
			getline(ss, item, ' ');
			buffer.push_back(item);
			while (getline(ss, item, ',')) {
				ss.ignore();
				
				buffer.push_back(item);

			}
			
			
		}
	}
	
	

	infile.clear();
	infile.seekg(0);
	infile.close();
}
int main()
{
	vector<string> buffer;
	readDictionary(buffer);//open the file
	for (int i = 0; i < buffer.size(); i+=4) {
		cout << buffer[i] << buffer[i+1]<< buffer[i+2] << buffer[i + 3] << "\n";
	}
	vector<string>instructions = {"ADDI", "ADD","SUB","MUL","DIV"};
	/*for (size_t i = 0; i < instructions.size(); i++) {
		cout << instructions[i]<<" ";
	}*/

	vector<string>store;
	vector<int>RF;
	vector<string>RAT;
	vector<string>RS;
	
	
	
	
}
