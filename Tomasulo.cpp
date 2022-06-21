
#include<iostream>
#include <vector>
#include<string>
#include <sstream>
#include<fstream>
#include <iomanip>
#define add_cyc  2;
#define mul_cyc  5;

#define div_cyc  10;
using namespace std;
using std::vector;
struct Instruction
{
	string operation;
	int rd, rs1, rs2, imm;
};
struct RS {
	string operand,rs1,rs2;
};
struct Buffer{
	int c,RS_num;
	bool occupy;
};

int cyc_count = 1;//current cycle
bool change;//check cycle change or not
vector<int> RF = { 0, 0, 2, 4, 6, 8 };
vector<RS> reservation(5);	//RS1-3 is add/sub; RS4-5 is mul/div
bool checkRS[5] = { true,true,true,true,true };//check RS is emty or not(true=empty;false=not empty)
vector<string>RAT(6);
Buffer Abuff;
Buffer Mbuff;
vector<Instruction>Data;

//define
void readInsts();//load instruction into vector
void print();//print cycle status
void Issue();//assgin instruction
void Dispatch();//execute
void Wr_back(Buffer & buff);//write value back to reservation and register

int main() 
{
	
	readInsts();//load instructions
	
	
	while (1) {
		if (Data.empty()) {//check whether data and RS is empty
			int k=0;
			for (int k = 0; k < 5; k++) {
				if (checkRS[k] == false) {
					break;
				}
			}
			if (k == 5) {
				break;
			}
		}
		change = false;//preset cycle doesn't change
		Wr_back(Abuff);
		Wr_back(Mbuff);
		Dispatch();
		Issue();
		
		
		
		if (change) {//若cycle changed
			print();

		}
		cyc_count +=1;//add to next cycle
	}
}

void readInsts()
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
		Instruction initial;
		int i = 0;
		while (getline(infile, linestr)) {//get lines of data
			//cout << linestr<<endl;
			Data.push_back(initial);
			istringstream ss(linestr);
			string operation;
			getline(ss, operation, ' ');
			//cout << operation;
			Data[i].operation = operation;
			string item;//rd
			string item1;//rs1
			string item2;//rs2
			getline(ss, item, ',');
			ss.ignore();//ignore space
			getline(ss, item1, ',');
			item = item.substr(1, 1);//取F?的?數字
			item1 = item1.substr(1, 1);
			Data[i].rd = stoi(item);
			Data[i].rs1 = stoi(item1);
			ss.ignore();
			getline(ss, item2);
			if (operation == "ADDI") {
				Data[i].imm = stoi(item2);
				//cout << data[i].imm;
			}
			else {
				item2 = item2.substr(1, 1);
				Data[i].rs2 = stoi(item2);
				//cout << data[i].rs2;
			}
			i++;
		}
	}
	infile.clear();
	infile.seekg(0);
	infile.close();
}
void Issue() {
	//check there is instruction in data available to issue
	if (!Data.empty()) {
		//determine which RS to put in
		//for add ALU
		
		if (Data[0].operation == "ADD" || Data[0].operation == "ADDI" || Data[0].operation == "SUB") {
			for (int i = 0; i <= 2; i++) {
				if (checkRS[i] == true) {//check RS is empty
					if (Data[0].operation == "SUB") {
						reservation[i].operand = "-";
					}
					else {
						reservation[i].operand = "+";
					}
				
				//put rs1 into RS　need to check RAT is empty or not
				//empty->get value from RF; not empty->get value from RAT
				if (!RAT[Data[0].rs1].empty()) {
					
					reservation[i].rs1 = RAT[Data[0].rs1];
				}
				else {
					reservation[i].rs1= to_string(RF[Data[0].rs1]);
				}
				//put rs2 into RS ->check whether is imm or not FIRST
				//cout << Data[0].operation << endl;
				if (Data[0].operation == "ADDI") {
					reservation[i].rs2 = to_string(Data[0].imm);
				}
				else if (RAT[Data[0].rs2]=="") {
					reservation[i].rs2 = to_string(RF[Data[0].rs2]);
				}
				else {
					reservation[i].rs2 = RAT[Data[0].rs2];
					
				}
				//issue done RS[i]is not empty
				checkRS[i] = false;
				//update RAT
				RAT[Data[0].rd] = "RS" + to_string(i + 1);
				//delete the instruction that issued
				Data.erase(Data.begin());
				//cycle change
				change = true;
				break;
				}
			}
		}else{//for mul ALU
			for (int i = 3; i <= 4; i++) {
				if (checkRS[i] == true) {//check RS is empty
					
					if (Data[0].operation == "MUL") {
						reservation[i].operand = "*";
					}
					else {
						reservation[i].operand = "/";
					}
				
				//put rs1 into RS　need to check RAT is empty or not
				//empty->get value from RF; not empty->get value from RAT
				if (!RAT[Data[0].rs1].empty()) {

					reservation[i].rs1 = RAT[Data[0].rs1];
				}
				else {
					reservation[i].rs1 = to_string(RF[Data[0].rs1]);
				}
				//put rs2 into RS, SAME as rs1
				if (!RAT[Data[0].rs2].empty()) {
					reservation[i].rs2 = RAT[Data[0].rs2];
				}
				else {
					reservation[i].rs2 = to_string(RF[Data[0].rs2]);
				}
				//issue done RS[i]is not empty
				checkRS[i] = false;
				//update RAT
				RAT[Data[0].rd] = "RS" + to_string(i + 1);
				//delete the instruction that issued
				Data.erase(Data.begin());
				//cycle change
				change = true;
				break;
				}
			}
		}
	}
}
void Dispatch() {
	//check ALU has been occupy or not
	if (!Abuff.occupy) {//no occupy -> dispatch
		for (int i = 0; i < 2; i++) {
			if (checkRS[i] == false) {
				if (reservation[i].rs1[0] != 'R' && reservation[i].rs2[0] != 'R') {
					Abuff.c = cyc_count + add_cyc;
					Abuff.occupy = true;
					Abuff.RS_num = i + 1;
					change = true;
				}
			}
		}
	}
	if (!Mbuff.occupy) {
		for (int i =3; i < 4; i++) {
			if (checkRS[i] == false) {
				//string s1 = reservation[i].rs1.substr(0, 2);
				//string s2 = reservation[i].rs2.substr(0, 2);
				if (reservation[i].rs1[0] != 'R' && reservation[i].rs2[0] != 'R') {
					if (reservation[i].operand == "*") {
						Mbuff.c = cyc_count +mul_cyc;
						Mbuff.occupy = true;
						Mbuff.RS_num = i + 1;
						change = true;
					}else{
						Mbuff.c = cyc_count + div_cyc;
						Mbuff.occupy = true;
						Mbuff.RS_num = i + 1;
						change = true;
					}
				}
			}
		}
	}
}
void Wr_back(Buffer & buff) {
	int back_val;
	//current cycle==buffer output cycle time
	if (cyc_count == buff.c) {
		
		if(reservation[buff.RS_num - 1].operand=="+") {
			back_val = stoi(reservation[buff.RS_num - 1].rs1) + stoi(reservation[buff.RS_num - 1].rs2);
		}
		else if (reservation[buff.RS_num - 1].operand == "-") {
			back_val = stoi(reservation[buff.RS_num - 1].rs1) - stoi(reservation[buff.RS_num - 1].rs2);
		}
		else if (reservation[buff.RS_num - 1].operand == "*") {
			back_val = stoi(reservation[buff.RS_num - 1].rs1) * stoi(reservation[buff.RS_num - 1].rs2);
		}
		else {
			back_val = stoi(reservation[buff.RS_num - 1].rs1) / stoi(reservation[buff.RS_num - 1].rs2);
		}
		buff.occupy = false;
		for (int i = 0; i < 5; i++)
		{
			//update RAT & RF
			if (RAT[i] == "RS" + to_string(buff.RS_num))
			{
				RAT[i] = "";
				RF[i] = back_val;
			}
			//capture buffer value and update RS
			if (reservation[i].rs1 == "RS" + to_string(buff.RS_num)) {
				reservation[i].rs1 = to_string(back_val);
			}
			if (reservation[i].rs2 == "RS" + to_string(buff.RS_num)) {
				reservation[i].rs2 = to_string(back_val);
			}
		}
		checkRS[buff.RS_num - 1] = true;
		reservation[buff.RS_num - 1].operand = "";
		reservation[buff.RS_num - 1].rs1 = "";
		reservation[buff.RS_num - 1].rs2 = "";
		change = true;

	}
}
void print() {
	cout << "\nCycle: " << cyc_count << "\n";
	cout<< "   _ RF __" << endl;
	for (int i = 0; i < RF.size(); i++) {
		cout << "F" << i << " |" << setw(4)<< RF[i] << " |" << endl;
	}
	cout << "   -------" << endl;
	cout << "   _ RAT__" << endl;
	for (int i = 0; i < RAT.size(); i++) {
		cout << "F" << i << " |" << setw(4) << RAT[i] << " |" << endl;

	}
	cout << "   -------" << endl;
	cout << "   _ RS________________" << endl;
	for (int i = 0; i <=2 ; i++) {
		cout << "RS" << i + 1 << " |" << setw(4) << reservation[i].operand << " |" << setw(4) << reservation[i].rs1
			<< " |" << setw(4) << reservation[i].rs2 << " |" << endl;
		
		if (i == 2) {
			cout << "   --------------------" << endl;
			cout << "Buffer: ";
			if (Abuff.occupy == true) {
				cout << "(RS" << Abuff.RS_num << ") " << reservation[Abuff.RS_num - 1].rs1 << " "
					<< reservation[Abuff.RS_num - 1].operand << " " << reservation[Abuff.RS_num - 1].rs2 << endl;
				
			}
			
			else {
				cout << "empty" << endl;
			}
		}
		
	}
	cout << endl;
	cout << "   --------------------" << endl;
	for (int i = 3; i <=4 ; i++) {
		cout << "RS" << i + 1 << " |" << setw(4) << reservation[i].operand << " |" << setw(4) << reservation[i].rs1
			<< " |" << setw(4) << reservation[i].rs2 << " |" << endl;
		;
		if (i == 4) {
			cout << "   --------------------" << endl;
			cout << "Buffer: ";
			if (Mbuff.occupy == true) {
				cout << "(RS" << Mbuff.RS_num << ") " << reservation[Mbuff.RS_num - 1].rs1<<" "
					<< reservation[Mbuff.RS_num - 1].operand <<" "<< reservation[Mbuff.RS_num - 1].rs2 << endl;
			}
			else {
				cout << "empty" << endl;
			}
		}
	}
}

