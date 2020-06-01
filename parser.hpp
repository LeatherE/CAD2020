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
	
	void GetInList();
	void GetOutList();
	void GetWireList();
	void GetGateList();
	void PrintCircuit();
	void BuildAdjList();
	
	void AddGate(Gate g);
	
	Gate GetGate(int v);
	int GetNumOfGate();
	
	void topsort_Call();
	void top_sort(int v);
	int first_xgate;
	int num_of_xgate;
	
	//void EncodeCircuit();
	
	int *top_order;

  private:
	bool IfAvailable();
	
	//string GetNextWire();
	void ModifyName();
	Gate GetNextGate();
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

#endif
