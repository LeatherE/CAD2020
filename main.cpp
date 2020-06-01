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
	
	c.GetInList();
	c.GetOutList();
	c.GetWireList();
	c.GetGateList();
	
	c.BuildAdjList();
	//c.PrintCircuit();
	c.topsort_Call();
	cout<<"number of xgate: "<<c.num_of_xgate<<endl;
	//for(i = 0; i < c
	
	
	
	return 0;
}

