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
	void topsort_Call();
	void top_sort(int v);
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
	
	vector<bool> visit;
	int top_index;
	
	//string circuit_name;
	string tmp;
};

#endif
