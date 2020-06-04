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
	
	void FindRepeat();
	
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
	
	string tmp;
};


#endif
