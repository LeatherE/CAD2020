#include "encoder.hpp"
#include <iostream>

using namespace std; 



Encoder::Encoder(vector<string> Tinput, vector<string> Toutput, vector<Gate> Tgate, int first_xgate, int *T_top_order){
	TargetIn = Tinput;
	TargetOut = Toutput;
	TargetGate = Tgate;
	XFlag = first_xgate;
	top_order = T_top_order;
}

void Encoder::EncodeCircuit(){
	int i, j, k;
	Gate g, g_tmp;
		
	for(i = 0; i < XFlag ; i++){
		g = TargetGate[top_order[i]];
		ResultGate.push_back(g);
		for(j = 0; j < g.fout.size(); j++){	// add encoder before xgate
			g_tmp = TargetGate[g.fout[j]];
			for(k = 0; k < TargetGate.size(); k++){
				if(top_order[k] == g.fout[j])
					break;
			}
			if(k >= XFlag){			// if g_tmp's top_order >= Xflag
				AddEncoder(g);		// can improve	
				break;
			}	
		}	
	}
	
	for(i = XFlag; i < TargetGate.size(); i++){
		g = TargetGate[top_order[i]];
		switch(g.gate_type){
			case AND_GATE:
				if(g.in.size()>2)
					EncodeMultiInput(g);
				else
					EncodeAND(g);
				break;
			case OR_GATE:
				if(g.in.size()>2)
					EncodeMultiInput(g);
				else
					EncodeOR(g);
				break;
			case NAND_GATE:
				if(g.in.size()>2)
					EncodeMultiInput(g);
				else
					EncodeNAND(g);
				break;
			case NOR_GATE:
				if(g.in.size()>2)
					EncodeMultiInput(g);
				else
					EncodeNOR(g);
				break;
			case NOT_GATE:
				EncodeNOT(g);
				break;
			case BUF_GATE:
				EncodeBUF(g);
				break;
			case XOR_GATE:
				if(g.in.size()>2)
					EncodeMultiInput(g);
				else
					EncodeXOR(g);
				break;
			case XNOR_GATE:
				if(g.in.size()>2)
					EncodeMultiInput(g);
				else
					EncodeXNOR(g);
				break;
			case DC_GATE:
				EncodeDC(g);
				break;
			case MUX_GATE:
				EncodeMUX(g);
				break;
		}	
	}
}

void Encoder::PrintResult(){
	int i, j;
	cout<<"module encode( ";
	for(i = 0; i < TargetIn.size(); i++){
		cout<<TargetIn[i]<<" , ";
	}
	for(i = 0; i < TargetOut.size()-1; i++){
		cout<<TargetOut[i]<<" , ";
	}
	cout<<TargetOut.back()<<" );"<<endl;
	
	cout<<"input ";
	for(i = 0; i < TargetIn.size()-1; i++){
		cout<<TargetIn[i]<<" , ";
	}
	cout<<TargetIn.back()<<" ;"<<endl;
	
	cout<<"output ";
	if(ResultOut.size()>0){
		for(i = 0; i < ResultOut.size()-1; i++){
			cout<<ResultOut[i]<<" , ";
		}
		cout<<ResultOut.back();
	}
	cout<<" ;"<<endl;
	
	cout<<"wire ";
	if(ResultWire.size()>0){
		for(i = 0; i < ResultWire.size()-1; i++){
			cout<<ResultWire[i]<<" , ";
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
		cout<<ResultGate[i].gate_name<<" ";
		cout<<"( "<<ResultGate[i].out<<" ,";
		for(j = 0; j < ResultGate[i].in.size() - 1; j++){
			cout<<ResultGate[i].in[j]<<" ,";
		}
		cout<<ResultGate[i].in.back()<<" );"<<endl;
	}
	cout<<endl;
	cout<<"endmodule"<<endl;
}

void Encoder::AddEncoder(Gate g){
	
}

void Encoder::EncodeMultiInput(Gate g){
	Gate new_gate;
	string s;
	int i = 1;
	
	switch(g.gate_type){
		case AND_GATE:
			while(g.in.size() >= 2){
				//s = to_string(i++);
				new_gate.gate_type = AND_GATE;
				new_gate.gate_name = g.gate_name + "_t";
				//new_gate.gate_name = g.gate_name + "_" + s;
				if(g.in.size() == 2)
					new_gate.out = g.out;
				else
					new_gate.out = g.out + "_w" + s;
				ResultWire.push_back(new_gate.out);
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				g.in.push_back(new_gate.out);
				EncodeAND(new_gate);
			}	
			break;
		case OR_GATE:
			while(g.in.size() >= 2){
				//s = to_string(i++);
				new_gate.gate_type = OR_GATE;
				new_gate.gate_name = g.gate_name + "_" + s;
				if(g.in.size() == 2)
					new_gate.out = g.out;
				else
					new_gate.out = g.out + "_w" + s;
				ResultWire.push_back(new_gate.out);
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				g.in.push_back(new_gate.out);
				EncodeOR(new_gate);
			}
			break;
		case NAND_GATE:
			while(g.in.size() >= 2){
				//s = to_string(i++);
				new_gate.gate_type = NAND_GATE;
				new_gate.gate_name = g.gate_name + "_" + s;
				if(g.in.size() == 2)
					new_gate.out = g.out;
				else
					new_gate.out = g.out + "_w" + s;
				ResultWire.push_back(new_gate.out);
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				g.in.push_back(new_gate.out);
				EncodeNAND(new_gate);
			}
			break;
		case NOR_GATE:
			while(g.in.size() >= 2){
				//s = to_string(i++);
				new_gate.gate_type = NOR_GATE;
				new_gate.gate_name = g.gate_name + "_" + s;
				if(g.in.size() == 2)
					new_gate.out = g.out;
				else
					new_gate.out = g.out + "_w" + s;
				ResultWire.push_back(new_gate.out);
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				g.in.push_back(new_gate.out);
				EncodeNOR(new_gate);
			}
			break;
		case XOR_GATE:
			while(g.in.size() >= 2){
				//s = to_string(i++);
				new_gate.gate_type = XOR_GATE;
				new_gate.gate_name = g.gate_name + "_" + s;
				if(g.in.size() == 2)
					new_gate.out = g.out;
				else
					new_gate.out = g.out + "_w" + s;
				ResultWire.push_back(new_gate.out);
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				g.in.push_back(new_gate.out);
				EncodeXOR(new_gate);
			}
			break;
		case XNOR_GATE:
			while(g.in.size() >= 2){
				//s = to_string(i++);
				new_gate.gate_type = XNOR_GATE;
				new_gate.gate_name = g.gate_name + "_" + s;
				if(g.in.size() == 2)
					new_gate.out = g.out;
				else
					new_gate.out = g.out + "_w" + s;
				ResultWire.push_back(new_gate.out);
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				new_gate.in.push_back(g.in.back());
				g.in.pop_back();
				g.in.push_back(new_gate.out);
				EncodeXNOR(new_gate);
			}
			break;
	}
}

void Encoder::EncodeAND(Gate g){
	Gate and1, and0;
	
	and1.gate_type = AND_GATE;
	and1.gate_name = g.gate_name + "_1";
	and1.out = g.out + "_1";
	ResultWire.push_back(and1.out);
	and1.in.push_back(g.in[0] + "_1");
	and1.in.push_back(g.in[1] + "_1");
	ResultGate.push_back(and1);
		
	and0.gate_type = OR_GATE;	
	and0.gate_name = g.gate_name + "_0";
	and0.out = g.out + "_0";
	ResultWire.push_back(and0.out);
	and0.in.push_back(g.in[0] + "_0");
	and0.in.push_back(g.in[1] + "_0");
	ResultGate.push_back(and0);
	//if g.out is ouput wire{
	// ResultOut.push_back(and1.out);
	// ResultOut.push_back(and0.out);
	//}

}


void Encoder::EncodeOR(Gate g){
	
}
void Encoder::EncodeNAND(Gate g){
	
}
void Encoder::EncodeNOR(Gate g){
	
}
void Encoder::EncodeNOT(Gate g){
	
}
void Encoder::EncodeBUF(Gate g){
	
}
void Encoder::EncodeXOR(Gate g){
	
}

void Encoder::EncodeXNOR(Gate g){
	
}
void Encoder::EncodeDC(Gate g){
	
}

void Encoder::EncodeMUX(Gate g){
	
}
