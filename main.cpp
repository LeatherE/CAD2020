#include "parser.hpp"
#include "encoder.hpp"
#include "datatype.hpp"
#include <iostream> 
#include <string> 
#include <vector>

using namespace std; 

int main(){
	int i;
	Circuit c;

	vector<string> w_tmp;
	vector<Gate> g_tmp;
	
	c.ReadInList();
	c.ReadOutList();
	c.ReadWireList();
	c.ReadGateList();
	
	c.BuildAdjList();
	//c.PrintCircuit();
	c.topsort_Call();
	
	Encoder e(c.GetInList(), c.GetOutList(), c.GetGateList(), c.first_xgate, c.top_order);

	e.EncodeCircuit();
	e.PrintResult();
	return 0;
}

