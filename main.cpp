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
	//cout<<"number of xgate: "<<c.num_of_xgate<<endl;

	

	
	/*w_tmp = c.GetInList();
	cout<<"input size: "<<w_tmp.size()<<endl;
	cout<<"input: "<<endl;
	for(i = 0; i < w_tmp.size(); i++)
		cout<<w_tmp[i]<<" ";
	cout<<endl;
	
	w_tmp = c.GetOutList();
	cout<<"output: "<<endl;
	for(i = 0; i < w_tmp.size(); i++)
		cout<<w_tmp[i]<<" ";
	cout<<endl;
	
	cout<<"wire: "<<endl;
	for(i = 0; i < w_tmp.size(); i++)
		cout<<w_tmp[i]<<" ";
	cout<<endl;*/
	Encoder e(c.GetInList(), c.GetOutList(), c.GetGateList(), c.first_xgate, c.top_order);

	e.EncodeCircuit();
	e.PrintResult();
	return 0;
}

