#include "datatype.hpp"
#include <iostream> 
#include <fstream>
#include <string> 
#include <vector>

using namespace std; 

int main(int argc, char* argv[]){
	Circuit miter, goldenCircuit, revisedCircuit, e_golden, e_revised;
	
	ReadCircuit(argv[1], goldenCircuit);
	topsort_Call(goldenCircuit);
	//PrintCircuit(goldenCircuit);
	EncodeCircuit(e_golden, goldenCircuit);
	//PrintEncodeResult(e_golden, goldenCircuit);
	
	ReadCircuit(argv[2], revisedCircuit);
	topsort_Call(revisedCircuit);
	EncodeCircuit(e_revised, revisedCircuit);
	//PrintEncodeResult(e_revised, revisedCircuit);
	
	BuildMiter(miter, e_golden, e_revised);
	PrintCircuit(miter);
	return 0;
}	

