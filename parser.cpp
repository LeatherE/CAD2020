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

using namespace::std;

// ===========================================================================
void print_Ntk_info(Jt_Ntk &pNtk){
	printf("Circuit Name : %s\n", Jt_NtkName(pNtk));
	printf("Num of Node : %d\n", Jt_NtkNodeNum(pNtk));
	printf("Num of Const0 (Add by TKS if not exist) : %d\n", Jt_NtkConst0Num(pNtk));
	printf("Num of Const1 (Add by TKS if not exist) : %d\n", Jt_NtkConst1Num(pNtk));
	printf("Num of Constx (Add by TKS if not exist) : %d\n", Jt_NtkConstxNum(pNtk));
	printf("Num of PI : %d\n", Jt_NtkPINum(pNtk));
	printf("Num of PO : %d\n", Jt_NtkPONum(pNtk));
	printf("Num of AND : %d\n", Jt_NtkANDNum(pNtk));
	printf("Num of OR : %d\n", Jt_NtkORNum(pNtk));
	printf("Num of NAND : %d\n", Jt_NtkNANDNum(pNtk));
	printf("Num of NOR : %d\n", Jt_NtkNORNum(pNtk));
	printf("Num of NOT : %d\n", Jt_NtkNOTNum(pNtk));
	printf("Num of BUF : %d\n", Jt_NtkBUFNum(pNtk));
	printf("Num of XOR : %d\n", Jt_NtkXORNum(pNtk));
	printf("Num of XNOR : %d\n", Jt_NtkXNORNum(pNtk));
	printf("Num of DCGATE : %d\n", Jt_NtkDCGATENum(pNtk));
	printf("Num of MUX : %d\n", Jt_NtkMUXNum(pNtk));
}
// ===========================================================================
void AddPi(Jt_Ntk &pNtk, string &Pi_Name){
	Jt_Node *pi = NULL;
	pi = new Jt_Node;

	pi->Name = new char[Pi_Name.length() + 1];
	strcpy(pi->Name, Pi_Name.c_str());
	pi->Type = node_PI;
	pi->XGate = false;
	pi->Value = _n_;
	pi->visit = 0;

	pNtk.Ntk_PI.push_back(pi);
	pNtk.Ntk_AllNode.push_back(pi);
	pNtk.node_map[ pi->Name ] = pi;
}
// ===========================================================================
void AddPo(Jt_Ntk &pNtk, string &Po_Name){
	Jt_Node *po = NULL;
	po = new Jt_Node;

	po->Name = new char[Po_Name.length() + 1];
	strcpy(po->Name, Po_Name.c_str());
	po->Type = node_PO;
	po->XGate = false;
	po->Value = _n_;
	po->visit = 0;

	pNtk.Ntk_PO.push_back(po);
	pNtk.Ntk_AllNode.push_back(po);
	pNtk.node_map[ po->Name ] = po;
}
// ===========================================================================
void AddWire(Jt_Ntk &pNtk, string &temp){
	char *cstr = new char[temp.length() + 1];
	strcpy(cstr, temp.c_str());
	map<char *, Jt_Node *, cmp_str>::iterator it = pNtk.node_map.find(cstr);		// Find this node is it exist

	if(it == pNtk.node_map.end()){		//	Not find
		Jt_Node *w = NULL;
		w = new Jt_Node;

		w->Name = new char[temp.length() + 1];
		strcpy(w->Name, cstr);
		w->Type = node_undefined;
		w->XGate = false;
		w->Value = _n_;
		w->visit = 0;
		pNtk.Ntk_AllNode.push_back(w);
		pNtk.node_map[ w->Name ] = w;
	}
}
// ===========================================================================
void GatePushBack(Jt_Ntk &pNtk, Jt_Node *n, NodeType_t &gType){
	if(gType == node_BUF)				pNtk.Ntk_BUF.push_back(n);
	else if(gType == node_AND)			pNtk.Ntk_OR.push_back(n);
	else if(gType == node_OR)			pNtk.Ntk_NAND.push_back(n);
	else if(gType == node_NAND)			pNtk.Ntk_NOR.push_back(n);
	else if(gType == node_NOR)			pNtk.Ntk_NOR.push_back(n);
	else if(gType == node_NOT)			pNtk.Ntk_NOT.push_back(n);
	else if(gType == node_XOR)			pNtk.Ntk_XOR.push_back(n);
	else if(gType == node_XNOR)			pNtk.Ntk_XNOR.push_back(n);
	else if(gType == node_DCGATE)		pNtk.Ntk_DCGATE.push_back(n);
	else if(gType == node_MUX)			pNtk.Ntk_MUX.push_back(n);
}
// ===========================================================================
bool CreateGate(Jt_Ntk &pNtk, vector<string> &gate_info){
	NodeType_t gType;

	if(gate_info[0] == "buf")				gType = node_BUF;
	else if(gate_info[0] == "and")			gType = node_AND;
	else if(gate_info[0] == "or")			gType = node_OR;
	else if(gate_info[0] == "nand")			gType = node_NAND;
	else if(gate_info[0] == "nor")			gType = node_NOR;
	else if(gate_info[0] == "not")			gType = node_NOT;
	else if(gate_info[0] == "xor")			gType = node_XOR;
	else if(gate_info[0] == "xnor")			gType = node_XNOR;
	else if(gate_info[0] == "_DC")			gType = node_DCGATE;
	else if(gate_info[0] == "_HMUX")		gType = node_MUX;

	char *cstr = new char[gate_info[2].length() + 1];
	strcpy(cstr, gate_info[2].c_str());

	
	map<char *, Jt_Node *, cmp_str>::iterator it = pNtk.node_map.find(cstr);		// Find this node is it exist

	if(it == pNtk.node_map.end()){		//	Not find
		return 1;
	}
	else{
		if(Jt_NodeType(it->second) == node_PO){
			Jt_Node *n = NULL;
			n = new Jt_Node;

			n->Name = new char[gate_info[1].length() + 1];
			strcpy(n->Name, gate_info[1].c_str());
			n->Type = gType;
			if(gType == node_DCGATE)
				n->XGate = true;
			n->Value = _n_;
			n->visit = 0;
			n->fo.push_back(it->second);
			
			n->out = gate_info[2];
			
			for(int i = 3; i < gate_info.size(); i++){
				cstr = new char[gate_info[i].length() + 1];
				strcpy(cstr, gate_info[i].c_str());
				map<char *, Jt_Node *, cmp_str>::iterator it_fi = pNtk.node_map.find(cstr);		// Find fi node is it exist
				n->fi.push_back(it_fi->second);
				it_fi->second->fo.push_back(n);
				
				n->in.push_back(gate_info[i]);
				if(gate_info[i] == "1'bx")
					n->XGate = true;
			}
			pNtk.Ntk_AllNode.push_back(n);
			GatePushBack(pNtk, n, gType);
			pNtk.node_map[ n->Name ] = n;
			it->second->fi.push_back(n);
		}
		else{
			it->second->Type = gType;
			if(gType == node_DCGATE)
				it->second->XGate = true;
			it->second->out = gate_info[2];
			for(int i = 3; i < gate_info.size(); i++){
				cstr = new char[gate_info[i].length() + 1];
				strcpy(cstr, gate_info[i].c_str());
				map<char *, Jt_Node *, cmp_str>::iterator it_fi = pNtk.node_map.find(cstr);		// Find fi node is it exist
				if(it_fi == pNtk.node_map.end()){
					return 1;
				}
				else{
					it->second->fi.push_back(it_fi->second);
					it_fi->second->fo.push_back(it->second);
				}
				
				it->second->in.push_back(gate_info[i]);
				if(gate_info[i] == "1'bx")
					it->second->XGate = true;
			}
			GatePushBack(pNtk, it->second, gType);
		}
	}
	return 0;
	
}
// ===========================================================================
void InitialNtk(Jt_Ntk &pNtk){
	Jt_Node *c0 = NULL;
	Jt_Node *c1 = NULL;
	Jt_Node *cx = NULL;

	c0 = new Jt_Node;	c1 = new Jt_Node;	cx = new Jt_Node;

	string c0_str = "1'b0";
	string c1_str = "1'b1";
	string cx_str = "1'bx";

	c0->Name = new char[c0_str.length() + 1];
	strcpy(c0->Name, c0_str.c_str());
	c1->Name = new char[c1_str.length() + 1];
	strcpy(c1->Name, c1_str.c_str());
	cx->Name = new char[cx_str.length() + 1];
	strcpy(cx->Name, cx_str.c_str());

	c0->Type = node_Const0;
	c1->Type = node_Const1;
	cx->Type = node_Constx;

	c0->XGate = false;
	c1->XGate = false;
	cx->XGate = false;
	
	c0->Value = _0_;
	c1->Value = _1_;
	cx->Value = _x_;

	c0->visit = 0;
	c1->visit = 0;
	cx->visit = 0;

	pNtk.Ntk_Const0.push_back(c0);
	pNtk.Ntk_AllNode.push_back(c0);
	pNtk.Ntk_Const1.push_back(c1);
	pNtk.Ntk_AllNode.push_back(c1);
	pNtk.Ntk_Constx.push_back(cx);
	pNtk.Ntk_AllNode.push_back(cx);


	pNtk.node_map[ c0->Name ] = c0;
	pNtk.node_map[ c1->Name ] = c1;
	pNtk.node_map[ cx->Name ] = cx;

}
// ===========================================================================
bool ReadCircuit(char * file_name, Jt_Ntk &pNtk){
	int node_ID = 0;
	InitialNtk(pNtk);
	//printf("Read file : %s\n", file_name);
	pNtk.Name = file_name;
	ifstream inFile;
	inFile.open(file_name);
	stringstream strStream;
	strStream << inFile.rdbuf();
	string str = strStream.str();

	// cout << str << endl;

	int str_len = str.length();

	size_t f_input = str.find("input");
	size_t f_output = str.find("output");
	size_t f_wire = str.find("wire");
	size_t f_end = str.find("endmodule");

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
					AddPi(pNtk, temp);
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
					AddPo(pNtk, temp);
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
					AddWire(pNtk, temp);
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
				if(CreateGate(pNtk, gate_info)){
					printf("Read file fail\n");
					return 1;
				}
				gate_info.clear();
			}

		}
		else{
			if(count_g == 0)	start_g = i;
			count_g++;
		}
	}

	inFile.close();

	return 0;
}

void PrintCircuit(Jt_Ntk &pNtk){
	Jt_Node *n;
	Jt_Node *nFi;
	//int i, j;
	cout<<"module top( ";
	Jt_NtkForEachPi( pNtk, n, i ){
		cout<<Jt_NodeName(n)<<" , ";
		if(i%5 == 4)
			cout<<endl;
	}
	Jt_NtkForEachPo( pNtk, n, i ){
		cout<<Jt_NodeName(n);
		if(i == Jt_NtkPONum(pNtk) - 1)
			cout<<" );"<<endl;
		else
			cout<<" , ";
		if(i%5 == 4)
			cout<<endl;		
	}
	
	cout<<"input ";
	Jt_NtkForEachPi( pNtk, n, i ){
		cout<<Jt_NodeName(n);
		if(i == Jt_NtkPINum(pNtk) - 1)
			cout<<" ;"<<endl;
		else{
			cout<<" , ";
			if(i%5 == 4)
				cout<<endl;	
		}
	}
	
	cout<<"output ";
	Jt_NtkForEachPo( pNtk, n, i ){
		cout<<Jt_NodeName(n);
		if(i == Jt_NtkPONum(pNtk) - 1)
			cout<<" ;"<<endl;
		else{
			cout<<" , ";
			if(i%5 == 4)
				cout<<endl;	
		}
	}
	
	cout<<"wire ";
	int j = 1;
	for ( int i = 0; (i < Jt_NtkNodeNum(pNtk) && (n = pNtk.Ntk_AllNode[i])); i++){
		if(Jt_NodeType(n)!=node_PI && Jt_NodeType(n)!=node_PO && Jt_NodeType(n)!=node_PI && Jt_NodeType(n)!=node_Const0 && Jt_NodeType(n)!=node_Const1 && Jt_NodeType(n)!=node_Constx){
			cout << n->out;
			//if(i == Jt_NtkNodeNum(pNtk) - 1)
			//	cout<<" ;"<<endl;
			if(i != Jt_NtkNodeNum(pNtk) - 1)
				cout<<" , ";
			
			if(j%5 == 0)
				cout<<endl;
			j++;
		}
		if(i == Jt_NtkNodeNum(pNtk) - 1)
			cout<<" ;"<<endl;
	}
	Jt_NtkForEachNode( pNtk, n, i ){
		if(Jt_NodeType(n) == node_BUF){
			cout<<"buf ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
			/*Jt_NodeForEachFi( n, nFi, j ){
				cout<<Jt_NodeName(nFi);
				if(j == Jt_NodeFiNum(n) - 1)
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}*/
		}
		else if(Jt_NodeType(n) == node_AND){
			cout<<"and ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
		}
		else if(Jt_NodeType(n) == node_OR){
			cout<<"or ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
		}
		else if(Jt_NodeType(n) == node_NAND){
			cout<<"nand ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
		}
		else if(Jt_NodeType(n) == node_NOR){
			cout<<"nor ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
		}
		else if(Jt_NodeType(n) == node_NOT){
			cout<<"not ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
		}
		else if(Jt_NodeType(n) == node_XOR){
			cout<<"xor ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
		}
		else if(Jt_NodeType(n) == node_XNOR){
			cout<<"xnor ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
		}
		else if(Jt_NodeType(n) == node_DCGATE){
			cout<<"_DC ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
		}
		else if(Jt_NodeType(n) == node_MUX){
			cout<<"_HMUX ";
			cout<<"( "<<n->out<<" , ";
			for(int j = 0; j < n->in.size(); j++){
				cout << n->in[j];
				if(j == (n->in.size() - 1))
					cout<<" );"<<endl;
				else
					cout<<" , ";
			}
		}
	}
	cout<<"endmodule"<<endl;
}
