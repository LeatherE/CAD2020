#include "miter.hpp"

int main(){
	Miter m;
	
	m.GetGoldenCircuit();
	m.GetRevisedCircuit();
	m.RenameWire();
	m.FindOutput();
	m.PrintResult();
	
	return 0;
}

Miter::Miter(){
	PO.gate_type = OR_GATE;
	PO.out = "PO";
	ResultOut.push_back("PO");
	count = 0;
}

void Miter::PrintResult(){
	int i, j;
	cout<<"module miter( ";
	j = 0;
	for(i = 0; i < GoldenIn.size(); i++, j++){
		cout<<GoldenIn[i]<<" , ";
		if(j%7 == 6)
			cout<<endl;
	}
	/*for(i = 0; i < ResultOut.size()-1; i++, j++){
		cout<<ResultOut[i]<<" , ";
		if(j%7 == 6)
			cout<<endl;
	}*/
	cout<<"PO );"<<endl;
	
	cout<<"input ";
	for(i = 0; i < GoldenIn.size()-1; i++){
		cout<<GoldenIn[i]<<" , ";
		if(i%7 == 6)
			cout<<endl;
	}
	cout<<GoldenIn.back()<<" ;"<<endl;
	
	cout<<"output PO;"<<endl;
	/*if(ResultOut.size()>0){
		for(i = 0; i < ResultOut.size()-1; i++){
			cout<<ResultOut[i]<<" , ";
			if(i%7 == 6)
				cout<<endl;
		}
		cout<<ResultOut.back();
	}
	cout<<" ;"<<endl;*/
	
	cout<<"wire ";
	if(ResultWire.size()>0){
		for(i = 0; i < ResultWire.size()-1; i++){
			cout<<ResultWire[i]<<" , ";
			if(i%7 == 6)
				cout<<endl;
		}
		cout<<ResultWire.back();
	}
	cout<<" ;"<<endl;

	for(i = 0; i < ResultGate.size(); i++){
		switch(ResultGate[i].gate_type){
			case AND_GATE:
				cout<<"and ";
				break;
			case OR_GATE:
				cout<<"or ";
				break;
			case NAND_GATE:
				cout<<"nand ";
				break;
			case NOR_GATE:
				cout<<"nor ";
				break;
			case NOT_GATE:
				cout<<"not ";
				break;
			case BUF_GATE:
				cout<<"buf ";
				break;
			case XOR_GATE:
				cout<<"xor ";
				break;
			case XNOR_GATE:
				cout<<"xnor ";
				break;
		}
		//cout<<ResultGate[i].gate_name<<" ";
		cout<<"( "<<ResultGate[i].out<<" , ";
		for(j = 0; j < ResultGate[i].in.size() - 1; j++){
			cout<<ResultGate[i].in[j]<<" , ";
		}
		cout<<ResultGate[i].in.back()<<" );"<<endl;
	}
	cout<<endl;
	
	cout<<"or ( "<<PO.out<<" , ";
	for(j = 0; j < PO.in.size() - 1; j++){
		cout<<PO.in[j]<<" , ";
	}
	cout<<PO.in.back()<<" );"<<endl;
	cout<<"endmodule"<<endl;
}

void Miter::GetGoldenCircuit(){
	Gate g;
	
	cin>>tmp;
	while (tmp != "input")
		cin>>tmp;
	while (tmp != "output") {
		cin>>tmp;
		if (tmp != "," && tmp != ";" && tmp != "output"){
            		while (tmp.find(dot) !=std::string::npos){
                
                		if(tmp.find(dot)>0)GoldenIn.push_back(tmp.substr(0, tmp.find(dot)));
                		tmp.erase(0, tmp.find(dot)+1);
            		}
            		if(tmp.size()>0){
                
                		if(tmp.find(semi) !=std::string::npos){
                    
                    			GoldenIn.push_back(tmp.substr(0, tmp.size()-1));
                		}
                		else GoldenIn.push_back(tmp);
            		}
		}	
	}
	while (tmp != "wire") {
		cin>>tmp;
		if (tmp != "," && tmp != ";" && tmp != "wire"){
			while (tmp.find(dot) !=std::string::npos){
                if(tmp.find(dot)>0)GoldenOut.push_back(tmp.substr(0, tmp.find(dot)));
               	tmp.erase(0, tmp.find(dot)+1);
            }
            if(tmp.size()>0){
                if(tmp.find(semi) !=std::string::npos){
                    GoldenOut.push_back(tmp.substr(0, tmp.size()-1));
               	}
                else GoldenOut.push_back(tmp);
            }
		}	
	}
	
	while (tmp != ";") { // need wire a, b, c ; space before ;
		cin>>tmp;
		if (tmp != "," && tmp != ";"){
			while (tmp.find(dot) !=std::string::npos){
                
                if(tmp.find(dot)>0)GoldenWire.push_back(tmp.substr(0, tmp.find(dot)));
                tmp.erase(0, tmp.find(dot)+1);
            }
            if(tmp.size()>0){

                if(tmp.find(semi) !=std::string::npos){

                  		GoldenWire.push_back(tmp.substr(0, tmp.size()-1));
                }
                else GoldenWire.push_back(tmp);
			}
		}	
	}
	
	cin>>tmp;
	do{
		g = ReadNextGate();
		GoldenGate.push_back(g);
	} while (IfAvailable());
}

void Miter::GetRevisedCircuit(){
	Gate g;
	
	cin>>tmp;
	while (tmp != "input")
		cin>>tmp;
	while (tmp != "output") {
		cin>>tmp;
		if (tmp != "," && tmp != ";" && tmp != "output"){
            		while (tmp.find(dot) !=std::string::npos){
                
                		if(tmp.find(dot)>0)RevisedIn.push_back(tmp.substr(0, tmp.find(dot)));
                		tmp.erase(0, tmp.find(dot)+1);
            		}
            		if(tmp.size()>0){
                
                		if(tmp.find(semi) !=std::string::npos){
                    
                    			RevisedIn.push_back(tmp.substr(0, tmp.size()-1));
                		}
                		else RevisedIn.push_back(tmp);
            		}
		}	
	}
	while (tmp != "wire") {
		cin>>tmp;
		if (tmp != "," && tmp != ";" && tmp != "wire"){
			while (tmp.find(dot) !=std::string::npos){
                if(tmp.find(dot)>0)RevisedOut.push_back(tmp.substr(0, tmp.find(dot)));
               	tmp.erase(0, tmp.find(dot)+1);
            }
            if(tmp.size()>0){
                if(tmp.find(semi) !=std::string::npos){
                    RevisedOut.push_back(tmp.substr(0, tmp.size()-1));
               	}
                else RevisedOut.push_back(tmp);
            }
		}	
	}
	
	while (tmp != ";") { // need wire a, b, c ; space before ;
		cin>>tmp;
		if (tmp != "," && tmp != ";"){
			while (tmp.find(dot) !=std::string::npos){
                
                if(tmp.find(dot)>0)RevisedWire.push_back(tmp.substr(0, tmp.find(dot)));
                tmp.erase(0, tmp.find(dot)+1);
            }
            if(tmp.size()>0){

                if(tmp.find(semi) !=std::string::npos){

                  		RevisedWire.push_back(tmp.substr(0, tmp.size()-1));
                }
                else RevisedWire.push_back(tmp);
			}
		}	
	}
	
	cin>>tmp;
	do{
		g = ReadNextGate();
		RevisedGate.push_back(g);
	} while (IfAvailable());
}

Gate Miter::ReadNextGate(){
	Gate g;
	char c;

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
		
	tmp.clear();
	do{
		c = cin.get();
		if(c == ','){
			g.out = tmp;
			break;
		}else if(c != ' ' && c != '('){
			tmp.push_back(c);
		}
	}while(true);
	
	tmp.clear();
	do{
		c = cin.get();
		if(c == ';'){
			break;
		}else if(c == ',' || c == ')'){
			g.in.push_back(tmp);
			tmp.clear();
		}else if(c != ' '){
			tmp.push_back(c);
		}
	}while(true);
	c = cin.get();		// skip '\n'
	
	return g;
}

bool Miter::IfAvailable(){
	cin>>tmp;
	return (tmp != "endmodule");
}	

void Miter::FindOutput(){
	int i, j;
	string s;
	
	for (i = 0; i < GoldenOut.size(); i++){
		s = GoldenOut[i];
		for (j = 0; j < RevisedOut.size(); j++){
			if (s.compare(RevisedOut[j]) == 0){
				if(s[s.size()-2] == '_' && s[s.size()-1] == '1')
					AddMiter1(s, GoldenOut[++i], RevisedOut[j], RevisedOut[j+1]);
				else
					AddMiter4(s, RevisedOut[j]);
				break;
			} else if(s[s.size()-2] == '_' && s[s.size()-1] == '1'){
				if(s.compare(0, s.size()-2, RevisedOut[j]) == 0){
					AddMiter2(s, GoldenOut[++i], RevisedOut[j]);
					break;
				}	
			} else if(s.compare(0, RevisedOut[j].size()-2, RevisedOut[j]) == 0){
				AddMiter3(s, RevisedOut[j], RevisedOut[j+1]);
				break;
			}
		}
	}
	
}

void Miter::RenameWire(){
	int i, j;
	
	RenamePI();
	for(i = 0; i < GoldenWire.size(); i++){
		ResultWire.push_back(GoldenWire[i] + "_g");
	}
	for(i = 0; i < RevisedWire.size(); i++){
		ResultWire.push_back(RevisedWire[i] + "_r");
	}
	for(i = 0; i < GoldenOut.size(); i++){
		ResultWire.push_back(GoldenOut[i] + "_g");
	}
	for(i = 0; i < RevisedOut.size(); i++){
		ResultWire.push_back(RevisedOut[i] + "_r");
	}
	for(i = 0; i < GoldenGate.size(); i++){
		Gate g;
		g.gate_type = GoldenGate[i].gate_type;
		g.out = GoldenGate[i].out + "_g";
		//ResultWire.push_back(g.out);
		for(j = 0; j < GoldenGate[i].in.size(); j++){
			if(GoldenGate[i].in[j] == "1'b0" || GoldenGate[i].in[j] == "1'b1")
				g.in.push_back(GoldenGate[i].in[j]);
			else
				g.in.push_back(GoldenGate[i].in[j] + "_g");
		}
		ResultGate.push_back(g);
	}
	for(i = 0; i < RevisedGate.size(); i++){
		Gate g;
		g.gate_type = RevisedGate[i].gate_type;
		g.out = RevisedGate[i].out + "_r";
		//ResultWire.push_back(g.out);
		for(j = 0; j < RevisedGate[i].in.size(); j++){
			if(RevisedGate[i].in[j] == "1'b0" || RevisedGate[i].in[j] == "1'b1")
				g.in.push_back(RevisedGate[i].in[j]);
			else
				g.in.push_back(RevisedGate[i].in[j] + "_r");
		}
		ResultGate.push_back(g);
	}
}

void Miter::RenamePI(){
	int i;
	
	for(i = 0; i < GoldenIn.size(); i++){
		Gate buf_g, buf_r;
		buf_g.gate_type = BUF_GATE;
		buf_g.in.push_back(GoldenIn[i]);
		buf_g.out = GoldenIn[i] + "_g";
		ResultWire.push_back(buf_g.out);
		ResultGate.push_back(buf_g);
		buf_r.gate_type = BUF_GATE;
		buf_r.in.push_back(GoldenIn[i]);
		buf_r.out = GoldenIn[i] + "_r";
		ResultWire.push_back(buf_r.out);
		ResultGate.push_back(buf_r);
	}
}


void Miter::AddMiter1(string g1, string g0, string r1, string r0){
	Gate and1, and0, or0, not1, not0;
	string s;
	
	//ResultWire.push_back(r1);
	//ResultWire.push_back(r0);
	//ResultWire.push_back(g1);
	//ResultWire.push_back(g0);
	
	not1.gate_type = NOT_GATE;
	not1.out = r1 + "_n";
	not1.in.push_back(r1 + "_r");
	ResultWire.push_back(not1.out);
	ResultGate.push_back(not1);

	and1.gate_type = AND_GATE;
	and1.out = g1 + "_and";
	and1.in.push_back(g1 + "_g");
	and1.in.push_back(not1.out);
	ResultWire.push_back(and1.out);
	ResultGate.push_back(and1);
	
	not0.gate_type = NOT_GATE;
	not0.out = r0 + "_n";
	not0.in.push_back(r0 + "_r");
	ResultWire.push_back(not0.out);
	ResultGate.push_back(not0);

	and0.gate_type = AND_GATE;
	and0.out = g0 + "_and";
	and0.in.push_back(g0 + "_g");
	and0.in.push_back(not0.out);
	ResultWire.push_back(and0.out);
	ResultGate.push_back(and0);
	
	s = to_string(count++);
	
	or0.gate_type = OR_GATE;
	or0.out = "Miter_" + s;
	or0.in.push_back(and0.out);
	or0.in.push_back(and1.out);
	ResultGate.push_back(or0);
	ResultWire.push_back(or0.out);
	PO.in.push_back(or0.out);
	
}

void Miter::AddMiter2(string g1, string g0, string r){
	Gate and1, and0, or0, not0;
	string s;
	
	//ResultWire.push_back(r);
	//ResultWire.push_back(g1);
	//ResultWire.push_back(g0);
	
	not0.gate_type = NOT_GATE;
	not0.out = r + "_n";
	not0.in.push_back(r + "_r");
	ResultWire.push_back(not0.out);
	ResultGate.push_back(not0);

	and1.gate_type = AND_GATE;
	and1.out = g1 + "_and";
	and1.in.push_back(g1 + "_g");
	and1.in.push_back(not0.out);
	ResultWire.push_back(and1.out);
	ResultGate.push_back(and1);

	and0.gate_type = AND_GATE;
	and0.out = g0 + "_and";
	and0.in.push_back(g0 + "_g");
	and0.in.push_back(r + "_r");
	ResultWire.push_back(and0.out);
	ResultGate.push_back(and0);
	
	s = to_string(count++);
	
	or0.gate_type = OR_GATE;
	or0.out = "Miter_" + s;
	or0.in.push_back(and0.out);
	or0.in.push_back(and1.out);
	ResultGate.push_back(or0);
	ResultWire.push_back(or0.out);
	PO.in.push_back(or0.out);
}

void Miter::AddMiter3(string g, string r1, string r0){
	Gate and1, and0, or0, not2, not1, not0;
	string s;
	
	//ResultWire.push_back(g);
	//ResultWire.push_back(r1);
	//ResultWire.push_back(r0);
	
	not2.gate_type = NOT_GATE;
	not2.out = r1 + "_n";
	not2.in.push_back(r1 + "_r");
	ResultWire.push_back(not2.out);
	ResultGate.push_back(not2);

	not1.gate_type = NOT_GATE;
	not1.out = r0 + "_n";
	not1.in.push_back(r0 + "_r");
	ResultWire.push_back(not1.out);
	ResultGate.push_back(not1);
	
	not0.gate_type = NOT_GATE;
	not0.out = g + "_n";
	not0.in.push_back(g + "_g");
	ResultWire.push_back(not0.out);
	ResultGate.push_back(not0);

	and1.gate_type = AND_GATE;
	and1.out = r0 + "_and";
	and1.in.push_back(not1.out);
	and1.in.push_back(not0.out);
	ResultWire.push_back(and1.out);
	ResultGate.push_back(and1);

	and0.gate_type = AND_GATE;
	and0.out = r1 + "_and";
	and0.in.push_back(g + "_g");
	and0.in.push_back(not2.out);
	ResultWire.push_back(and0.out);
	ResultGate.push_back(and0);
	
	s = to_string(count++);
	
	or0.gate_type = OR_GATE;
	or0.out = "Miter_" + s;
	or0.in.push_back(and0.out);
	or0.in.push_back(and1.out);
	ResultGate.push_back(or0);
	ResultWire.push_back(or0.out);
	PO.in.push_back(or0.out);
}

void Miter::AddMiter4(string g, string r){
	Gate and1, and0, or0, not1, not0;
	string s;
	
	//ResultWire.push_back(g);
	//ResultWire.push_back(r);

	not1.gate_type = NOT_GATE;
	not1.out = r + "_n";
	not1.in.push_back(r + "_r");
	ResultWire.push_back(not1.out);
	ResultGate.push_back(not1);
	
	not0.gate_type = NOT_GATE;
	not0.out = g + "_n";
	not0.in.push_back(g + "_g");
	ResultWire.push_back(not0.out);
	ResultGate.push_back(not0);

	and1.gate_type = AND_GATE;
	and1.out = g + "_n_and";
	and1.in.push_back(r + "_r");
	and1.in.push_back(not0.out);
	ResultWire.push_back(and1.out);
	ResultGate.push_back(and1);

	and0.gate_type = AND_GATE;
	and0.out = r + "_n_and";
	and0.in.push_back(g + "_g");
	and0.in.push_back(not1.out);
	ResultWire.push_back(and0.out);
	ResultGate.push_back(and0);
	
	s = to_string(count++);
	
	or0.gate_type = OR_GATE;
	or0.out = "Miter_" + s;
	or0.in.push_back(and0.out);
	or0.in.push_back(and1.out);
	ResultGate.push_back(or0);
	ResultWire.push_back(or0.out);
	PO.in.push_back(or0.out);
}
