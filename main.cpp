#include "datatype.hpp"
#include <iostream> 
#include <fstream>
#include <string> 
#include <vector>

using namespace std; 

int main(int argc, char* argv[]){
	Jt_Ntk miter, goldenCircuit, revisedCircuit, e_golden, e_revised;
	
	
	if(ReadCircuit(argv[1], goldenCircuit))
		return 0;
	//PrintCircuit(goldenCircuit);
	EncodeCircuit(e_golden, goldenCircuit);
	//PrintCircuit(e_golden);
	//cout<<"revised"<<endl;
	if(ReadCircuit(argv[2], revisedCircuit))
		return 0;
	EncodeCircuit(e_revised, revisedCircuit);
	//PrintCircuit(e_revised);
	
	BuildMiter(miter, e_golden, e_revised);
	PrintCircuit(miter);
	return 0;
}	