#include <iostream> 
#include <string> 
#include <vector>
using namespace std; 

struct Gate
{
	string gate_type;
	string gate_name;
	string out;
	vector<string> in;
	struct Gate *fout;
};

int main(){
	string t;
	
	vector<Gate> gate_list;
	vector<string> input;
	vector<string> output;
	vector<string> wire;
	int i, j=0;
	
	cin>>t;
	while(t != "input")
		cin>>t;
	while(t != ";") {
		cin>>t;
		if(t != "," && t != ";")
			input.push_back(t);
	}
	cin>>t;
	while(t != ";") {
		cin>>t;
		if(t != "," && t != ";")
			output.push_back(t);
	}
	cin>>t;
	while(t != ";") {
		cin>>t;
		if(t != "," && t != ";")
			wire.push_back(t);
	}
	
	cout<<"input"<<endl;
	for(i = 0; i < input.size(); i++)
		cout<<input[i]<<" ";
	cout<<endl;
	cout<<"output"<<endl;
	for(i = 0; i < output.size(); i++)
		cout<<output[i]<<" ";
	cout<<endl;
	cout<<"wire"<<endl;
	for(i = 0; i < wire.size(); i++)
		cout<<wire[i]<<" ";
	cout<<endl;
	
	cin>>t;
	do{
		Gate g;
		g.gate_type = t;
		cin>>t;
		g.gate_name = t;
		cin>>t;				// skip (
		cin>>t;
		g.out = t;
		while(t != ");") {
			cin>>t;
			if(t != "," && t != ");")
				g.in.push_back(t);
		}
		gate_list.push_back(g);
		cin>>t;
	} while (t != "endmodule");
	
	for(i = 0; i < gate_list.size(); i++){
		cout<<gate_list[i].gate_type<<gate_list[i].gate_name<<endl;
		cout<<"out "<<gate_list[i].out<<endl;
		cout<<"in ";
		for(j = 0; j < gate_list[i].in.size(); j++)
			cout<<gate_list[i].in[j]<<" ";
		cout<<endl;
	}
	
	return 0;
}
