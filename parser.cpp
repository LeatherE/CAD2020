#include "parser.hpp"
#include <iostream> 

const string dot = ",";
const string semi = ";";
using namespace std; 

Circuit::Circuit(){
	cin>>tmp;
	while (tmp != "input")
		cin>>tmp;
}	

bool Circuit::IfAvailable(){
	cin>>tmp;
	return (tmp != "endmodule");
}	

void Circuit::ReadInList(){
		
	while (tmp != "output") {
		cin>>tmp;
		if (tmp != "," && tmp != ";" && tmp != "output"){
            		while (tmp.find(dot) !=std::string::npos){
                
                		if(tmp.find(dot)>0)InList.push_back(tmp.substr(0, tmp.find(dot)));
                		tmp.erase(0, tmp.find(dot)+1);
            		}
            		if(tmp.size()>0){
                
                		if(tmp.find(semi) !=std::string::npos){
                    
                    			InList.push_back(tmp.substr(0, tmp.size()-1));
                		}
                		else InList.push_back(tmp);
            		}
		}	
	}
	//use do...while;
}

void Circuit::ReadOutList(){

	while (tmp != "wire") {
		cin>>tmp;
		if (tmp != "," && tmp != ";" && tmp != "wire"){
			while (tmp.find(dot) !=std::string::npos){
                if(tmp.find(dot)>0)OutList.push_back(tmp.substr(0, tmp.find(dot)));
               	tmp.erase(0, tmp.find(dot)+1);
            }
            if(tmp.size()>0){
                if(tmp.find(semi) !=std::string::npos){
                    OutList.push_back(tmp.substr(0, tmp.size()-1));
               	}
                else OutList.push_back(tmp);
            }
		}	
	}
	//use do...while;
}

void Circuit::ReadWireList(){
	
	while (tmp != ";") { // need wire a, b, c ; space before ;
		cin>>tmp;
		if (tmp != "," && tmp != ";"){
			while (tmp.find(dot) !=std::string::npos){
                if(tmp.find(dot)>0)WireList.push_back(tmp.substr(0, tmp.find(dot)));
                tmp.erase(0, tmp.find(dot)+1);
            }
            if(tmp.size()>0){

                if(tmp.find(semi) !=std::string::npos){

                  		WireList.push_back(tmp.substr(0, tmp.size()-1));
                }
                else WireList.push_back(tmp);
			}
		}	
	}
}

void Circuit::ReadGateList(){
	Gate g;
	
	num_of_xgate = 0;
	cin>>tmp;
	do{
		g = ReadNextGate();
		GateList.push_back(g);
	} while (IfAvailable());

}	

void Circuit::PrintCircuit(){
	int i, j;
	
	cout<<"input: "<<endl;
	for(i = 0; i < InList.size(); i++)
		cout<<InList[i]<<" ";
	cout<<endl;
	
	cout<<"output: "<<endl;
	for(i = 0; i < OutList.size(); i++)
		cout<<OutList[i]<<" ";
	cout<<endl;
	
	cout<<"wire: "<<endl;
	for(i = 0; i < WireList.size(); i++)
		cout<<WireList[i]<<" , ";
	cout<<endl;
	
	for(i = 0; i < GateList.size(); i++){
		cout<<GateList[i].gate_type<<GateList[i].gate_name<<endl;
		cout<<"out "<<GateList[i].out<<endl;
		cout<<"in ";
		for(j = 0; j < GateList[i].in.size(); j++)
			cout<<GateList[i].in[j]<<" ";
		cout<<endl;
		/*cout<<"fout_adj_list(vector): ";
		for(j = 0; j < GateList[i].fout.size(); j++)
			cout<<GateList[i].fout[j]<<" ";
		cout<<endl;*/
	}
	
	
	return;
}

void Circuit::BuildAdjList(){
	int i, j, k;
	string out_tmp;
	
	for(i = 0; i < GateList.size(); i++){
		out_tmp = GateList[i].out;
		//if(i == 1)
		//	cout<<out_tmp<<endl;
		for(j = 0; j < GateList.size(); j++){
			for(k = 0; k < GateList[j].in.size(); k++){
				if(out_tmp == GateList[j].in[k]){
					GateList[i].fout.push_back(j);
					break;
				}	
			}	
		}	
	}
}	

void Circuit::AddGate(Gate g){
	GateList.push_back(g);
}

int Circuit::GetNumOfGate(){
	return GateList.size();
}

Gate Circuit::GetGate(int v){
	return GateList[v];
}

vector<string> Circuit::GetInList(){
	return InList;
}
vector<string> Circuit::GetOutList(){
	return OutList;
}
vector<string> Circuit::GetWireList(){
	return WireList;
}
vector<Gate> Circuit::GetGateList(){
	return GateList;
}

void Circuit::topsort_Call(){
	int i, j = 0, first = 0;
	
	first_xgate = GateList.size();
	top_order = new int[GateList.size()];
	for (i = 0; i < GateList.size(); i++){
		visit.push_back(false);
	}
	top_index = GateList.size() - 1;	
	for (i = 0; i < GateList.size(); i++){	 
		if (!visit[i])		
			top_sort(i);
	}
	
	for(i=0; i < GateList.size(); i++){
		if(first == 0 && GateList[top_order[i]].XGate){
			first = 1;
			first_xgate = i;
		}
	}
}	
	
void Circuit::top_sort(int v){
	int i = 0;
	Gate g;
	
	visit[v] = 1;
	
	g = GateList[v];
	while(i < g.fout.size()){
		if (!visit[g.fout[i]])
			top_sort(g.fout[i]);
		i++;
	}
	top_order[top_index--] = v;
}

void Circuit::ModifyName(){
	if (tmp[0] == ',')
		tmp.erase(tmp.begin());
	if (tmp[tmp.size()-1] == ',' || tmp[tmp.size()-1] == ';')
		tmp.erase(tmp.end()-1);
}	

Gate Circuit::ReadNextGate(){
	Gate g;
	char c;
	int i = 0;
	
	g.XGate = false;
	//cin>>tmp;
	if (tmp == "and")
		g.gate_type = AND_GATE;
	else if (tmp == "or")
		g.gate_type = OR_GATE;
	else if (tmp == "nand")
		g.gate_type = NAND_GATE;	
	else if (tmp == "nor")
		g.gate_type = NOR_GATE;
	else if (tmp == "not")
		g.gate_type = NOT_GATE;
	else if (tmp == "buf")
		g.gate_type = BUF_GATE;
	else if (tmp == "xor")
		g.gate_type = XOR_GATE;	
	else if (tmp == "xnor")
		g.gate_type = XNOR_GATE;
	else if (tmp == "_DC"){
		g.gate_type = DC_GATE;
		g.XGate = true;
		num_of_xgate++;
	}
	else if (tmp == "_HMUX")
		g.gate_type = MUX_GATE;
	
	tmp.clear();
	do{
		c = cin.get();
		if(c == '('){
			g.gate_name = tmp;
			break;
		}else if(c != ' '){
			tmp.push_back(c);
		}
	}while(true);
	
	tmp.clear();
	do{
		c = cin.get();
		if(c == ','){
			g.out = tmp;
			break;
		}else if(c != ' '){
			tmp.push_back(c);
		}
	}while(true);
	
	tmp.clear();
	do{
		c = cin.get();
		if(c == ';'){
			break;
		}else if(c == ',' || c == ')'){
			if(tmp == "1'bx" || tmp == "1'bX"){
				g.XGate = true;
				num_of_xgate++;
			}
			g.in.push_back(tmp);
			tmp.clear();
		}else if(c != ' '){
			tmp.push_back(c);
		}
	}while(true);
	c = cin.get();		// skip '\n'

	return g;

}

