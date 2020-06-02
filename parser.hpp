#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <string> 
#include <vector>
#include "datatype.hpp"
using namespace std;

class Circuit {
  public:
    Circuit();
	//~Circuit() = default;
	
	void ReadInList();
	void ReadOutList();
	void ReadWireList();
	void ReadGateList();
	void PrintCircuit();
	void BuildAdjList();
	
	void AddGate(Gate g);
	
	Gate GetGate(int v);
	int GetNumOfGate();
	vector<string> GetInList();
	vector<string> GetOutList();
	vector<string> GetWireList();
	vector<Gate> GetGateList();
	
	void topsort_Call();
	void top_sort(int v);
	int first_xgate;
	int num_of_xgate;
	
	//void EncodeCircuit();
	
	int *top_order;

  private:
	bool IfAvailable();
	
	
	void ModifyName();
	Gate ReadNextGate();
	vector<string> InList;
	vector<string> OutList;
	vector<string> WireList;
	vector<Gate> GateList;
	
	void AddEncoder();
	
	
	vector<bool> visit;
	int top_index;
	
	//string circuit_name;
	string tmp;
};

/*class Circuit {
  public:
    Circuit();
	//~Circuit() = default;
	
	void GetInList();
	void GetOutList();
	void GetWireList();
	void GetGateList();
	void PrintCircuit();
	void PrintEncodeCircuit();
	void BuildAdjList();
	
	void AddGate(Gate g);
	void AddWire(string s);
	
	Gate GetGate(int v);
	int GetNumOfGate();
	
	void topsort_Call();
	void top_sort(int v);
	int first_xgate;
	int num_of_xgate;
	
	//for encode
	void EncodeCircuit();
	
	
	int *top_order;

  private:
	bool IfAvailable();
	void ModifyName();
	Gate GetNextGate();
	vector<string> InList;
	vector<string> OutList;
	vector<string> WireList;
	vector<Gate> GateList;
	
	//for encode
	vector<string> EncodeIn;
	vector<string> EncodeOut;
	vector<string> EncodeWire;
	vector<Gate> EncodeGate;
	void AddEncoder(Gate g);
	void EncodeMultiInput(Gate g);
	void EncodeAND(Gate g);
	void EncodeOR(Gate g);
	void EncodeNAND(Gate g);
	void EncodeNOR(Gate g);
	void EncodeNOT(Gate g);
	void EncodeBUF(Gate g);
	void EncodeXOR(Gate g);
	void EncodeXNOR(Gate g);
	void EncodeDC(Gate g);
	void EncodeMUX(Gate g);
	
	vector<bool> visit;
	int top_index;
	
	//string circuit_name;
	string tmp;
};*/

#endif
