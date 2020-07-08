#include "datatype.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <time.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

using namespace std; 

void ReadCircuit(char * file_name, Circuit &pNtk){
	int node_ID = 0;
	
	
	ifstream inFile;
	inFile.open(file_name);
	stringstream strStream;
	strStream << inFile.rdbuf();
	string str = strStream.str();


	int str_len = str.length();

	size_t f_input = str.find("input");
	size_t f_output = str.find("output");
	size_t f_wire = str.find("wire");
	size_t f_end = str.find("endmodule");

	pNtk.num_of_xgate = 0;

	int f_gate = 0;

	if (f_input!=std::string::npos){
		// printf("Find input at %d\n", f_input);

		string temp = "";
		int count = 0, start = 0;

		for(int i = f_input + 6; i < f_output; i++)
		{
			if(str[i] == ' ' || str[i] == ',' || str[i] == '\n' || str[i] == '\t' || str[i] == ';'){
				if(count != 0){
					temp = temp.append(str, start, count);
					// cout << temp << endl;
					//AddPi(pNtk, temp);
					pNtk.InList.push_back(temp);
					temp = "";
					count = 0;
				}
			}
			else{
				if(count == 0)	start = i;
				count++;
			}
		}
	}

	if (f_output!=std::string::npos){
		// printf("Find output at %d\n", f_output);

		string temp = "";
		int count = 0, start = 0;

		for(int i = f_output + 7; i < f_wire; i++)
		{
			if(str[i] == ' ' || str[i] == ',' || str[i] == '\n' || str[i] == '\t' || str[i] == ';'){
				if(count != 0){
					temp = temp.append(str, start, count);
					// cout << temp << endl;
					//AddPo(pNtk, temp);
					pNtk.OutList.push_back(temp);
					node_ID++;
					temp = "";
					count = 0;
				}
			}
			else{
				if(count == 0)	start = i;
				count++;
			}
		}
	}

	if (f_wire!=std::string::npos){
		// printf("Find wire at %d\n", f_wire);

		string temp = "";
		int count = 0, start = 0;

		for(int i = f_wire + 5; i < str_len; i++)
		{
			if(str[i] == ' ' || str[i] == ',' || str[i] == '\n' || str[i] == '\t' || str[i] == ';'){
				if(count != 0){
					temp = temp.append(str, start, count);
					// cout << temp << endl;
					//AddWire(pNtk, temp);
					pNtk.WireList.push_back(temp);
					temp = "";
					count = 0;
				}

				if(str[i] == ';'){
					f_gate = i + 1;	
					break;
				}
			}
			else{
				if(count == 0)	start = i;
				count++;
			}
		}
	}

	string temp_g = "";
	vector<string> gate_info;
	int count_g = 0, start_g = 0;
	for(int i = f_gate; i < f_end; i++){
		if(str[i] == ' ' || str[i] == ',' || str[i] == '\n' || str[i] == '\t' || str[i] == ';' || str[i] == '(' || str[i] == ')'){
			if(count_g != 0){
				temp_g = temp_g.append(str, start_g, count_g);
				// cout << temp_g << endl;
				gate_info.push_back(temp_g);
				temp_g = "";
				count_g = 0;
			}

			if(str[i] == ';'){
				CreateGate(pNtk, gate_info);
				gate_info.clear();
			}

		}
		else{
			if(count_g == 0)	start_g = i;
			count_g++;
		}
	}

	inFile.close();

	return;
}

void CreateGate(Circuit &pNtk, vector<string> &gate_info){
	Gate g;
	
	g.XGate = false;
	if (gate_info[0] == "and")
		g.gate_type = AND_GATE;
	else if (gate_info[0] == "or")
		g.gate_type = OR_GATE;
	else if (gate_info[0] == "nand")
		g.gate_type = NAND_GATE;	
	else if (gate_info[0] == "nor")
		g.gate_type = NOR_GATE;
	else if (gate_info[0] == "not")
		g.gate_type = NOT_GATE;
	else if (gate_info[0] == "buf")
		g.gate_type = BUF_GATE;
	else if (gate_info[0] == "xor")
		g.gate_type = XOR_GATE;	
	else if (gate_info[0] == "xnor")
		g.gate_type = XNOR_GATE;
	else if (gate_info[0] == "_DC"){
		g.gate_type = DC_GATE;
		g.XGate = true;
		pNtk.num_of_xgate++;
	}
	else if (gate_info[0] == "_HMUX")
		g.gate_type = MUX_GATE;


	g.gate_name = gate_info[1];
	g.out = gate_info[2];
	
	for(int i = 3; i < gate_info.size(); i++){	
		if(gate_info[i] == "1'bx" || gate_info[i] == "1'bX"){
			g.XGate = true;
			pNtk.num_of_xgate++;
		}
		g.in.push_back(gate_info[i]);
	}
	pNtk.GateList.push_back(g);
	return;
}

void BuildAdjList(Circuit &pNtk){
	int i, j, k;
	string out_tmp;
	
	for(i = 0; i < pNtk.GateList.size(); i++){
		out_tmp = pNtk.GateList[i].out;
		for(j = 0; j < pNtk.GateList.size(); j++){
			for(k = 0; k < pNtk.GateList[j].in.size(); k++){
				if(out_tmp == pNtk.GateList[j].in[k]){
					pNtk.GateList[i].fout.push_back(j);
					break;
				}	
			}	
		}	
	}
}	

void topsort_Call(Circuit &pNtk){
	int i, j = 0, first = 0;
	
	BuildAdjList(pNtk);
	pNtk.first_xgate = pNtk.GateList.size();
	pNtk.top_order = new int[pNtk.GateList.size()];
	for (i = 0; i < pNtk.GateList.size(); i++){
		pNtk.visit.push_back(false);
	}
	pNtk.top_index = pNtk.GateList.size() - 1;	
	for (i = 0; i < pNtk.GateList.size(); i++){	 
		if (!pNtk.visit[i])		
			top_sort(pNtk, i);
	}
	
	for(i=0; i < pNtk.GateList.size(); i++){
		if(first == 0 && pNtk.GateList[pNtk.top_order[i]].XGate){
			first = 1;
			pNtk.first_xgate = i;
		}
	}
}	
	
void top_sort(Circuit &pNtk, int v){
	int i = 0;
	
	pNtk.visit[v] = 1;
	while(i < pNtk.GateList[v].fout.size()){
		if (!pNtk.visit[pNtk.GateList[v].fout[i]])
			top_sort(pNtk, pNtk.GateList[v].fout[i]);
		i++;
	}
	pNtk.top_order[pNtk.top_index--] = v;
}

void PrintCircuit(Circuit &c){
	int i, j;
	cout<<"module top( ";
	j = 0;
	for(i = 0; i < c.InList.size(); i++, j++){
		cout<<c.InList[i]<<" , ";
		if(j%7 == 6)
			cout<<endl;
	}
	for(i = 0; i < c.OutList.size()-1; i++, j++){
		cout<<c.OutList[i]<<" , ";
		if(j%7 == 6)
			cout<<endl;
	}
	cout<<c.OutList.back()<<" );"<<endl;
	
	cout<<"input ";
	for(i = 0; i < c.InList.size()-1; i++){
		cout<<c.InList[i]<<" , ";
		if(i%7 == 6)
			cout<<endl;
	}
	cout<<c.InList.back()<<" ;"<<endl;
	
	cout<<"output ";
	if(c.OutList.size()>0){
		for(i = 0; i < c.OutList.size()-1; i++){
			cout<<c.OutList[i]<<" , ";
			if(i%7 == 6)
				cout<<endl;
		}
		cout<<c.OutList.back();
	}
	cout<<" ;"<<endl;
	
	cout<<"wire ";
	if(c.WireList.size()>0){
		for(i = 0; i < c.WireList.size()-1; i++){
			cout<<c.WireList[i]<<" , ";
			if(i%7 == 6)
				cout<<endl;
		}
		cout<<c.WireList.back();
	}
	cout<<" ;"<<endl;

	for(i = 0; i < c.GateList.size(); i++){
		switch(c.GateList[i].gate_type){
			case AND_GATE:
				cout<<"and ";
				break;
			case OR_GATE:
				cout<<"or ";
				break;
			case NAND_GATE:
				cout<<"nand ";
				break;
			case NOR_GATE:
				cout<<"nor ";
				break;
			case NOT_GATE:
				cout<<"not ";
				break;
			case BUF_GATE:
				cout<<"buf ";
				break;
			case XOR_GATE:
				cout<<"xor ";
				break;
			case XNOR_GATE:
				cout<<"xnor ";
				break;
		}
		cout<<"( "<<c.GateList[i].out<<" , ";
		for(j = 0; j < c.GateList[i].in.size() - 1; j++){
			cout<<c.GateList[i].in[j]<<" , ";
		}
		cout<<c.GateList[i].in.back()<<" );"<<endl;
	}
	cout<<endl;
	cout<<"endmodule"<<endl;
}


