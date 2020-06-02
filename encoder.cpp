#include "encoder.hpp"
#include <iostream>

using namespace std; 

#include <sstream>
#include <algorithm>

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

		if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
			ResultOut.push_back(g.out);
		}else{
			ResultWire.push_back(g.out);
		}

		for(j = 0; j < g.fout.size(); j++){	// add encoder before encoded gate
			g_tmp = TargetGate[g.fout[j]];
			for(k = 0; k < TargetGate.size(); k++){
				if(top_order[k] == g.fout[j])
					break;
			}
			if(k >= XFlag){			// if g_tmp's top_order >= Xflag
				AddEncoder(g);		// problem here	
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
	for(i = 0; i < ResultOut.size()-1; i++){
		cout<<ResultOut[i]<<" , ";
	}
	cout<<ResultOut.back()<<" );"<<endl;
	
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
	string encoder_out1, encoder_out0;
	Gate encode_not, encode_buf;
	
	encode_not.gate_type = NOT_GATE;
	encode_not.in.push_back(g.out);
	encode_not.out = g.out + "_0";
	encode_not.gate_name = g.gate_name + "_en";
	ResultWire.push_back(encode_not.out);
	ResultGate.push_back(encode_not);
	
	encode_buf.gate_type = BUF_GATE;
	encode_buf.in.push_back(g.out);
	encode_buf.out = g.out + "_1";
	encode_buf.gate_name = g.gate_name + "_buf";
	ResultWire.push_back(encode_buf.out);
	ResultGate.push_back(encode_buf);

}

void Encoder::EncodeMultiInput(Gate g){
	Gate new_gate;
	string s;
	int i = 1;
	
	switch(g.gate_type){
		case AND_GATE:
			while(g.in.size() >= 2){
				s = to_string(i++);
				new_gate.gate_type = AND_GATE;
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
				EncodeAND(new_gate);
			}	
			break;
		case OR_GATE:
			while(g.in.size() >= 2){
				s = to_string(i++);
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
				s = to_string(i++);
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
				s = to_string(i++);
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
				s = to_string(i++);
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
				s = to_string(i++);
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
void Encoder::EncodeConstantInput(Gate g){
	if(g.in[0] == "1'b0"){
		in01 = "1'b0";
		in00 = "1'b1";
	}else if(g.in[0] == "1'b1"){
		in01 = "1'b1";
		in00 = "1'b0";
	}else if(g.in[0] == "1'bx"){
		in01 = "1'b0";
		in00 = "1'b0";
	}else{
		in01 = g.in[0] + "_1";
		in00 = g.in[0] + "_0";
	}
	if(g.in[1] == "1'b0"){
		in11 = "1'b0";
		in10 = "1'b1";
	}else if(g.in[1] == "1'b1"){
		in11 = "1'b1";
		in10 = "1'b0";
	}else if(g.in[1] == "1'bx"){
		in11 = "1'b0";
		in10 = "1'b0";
	}else{
		in11 = g.in[1] + "_1";
		in10 = g.in[1] + "_0";
	}
}
void Encoder::EncodeAND(Gate g){
	Gate and1, and0;
		
	EncodeConstantInput(g);
	if(find(TargetIn.begin(), TargetIn.end(), g.in[0]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[0];
		en_Input.gate_name = g.in[0];
		AddEncoder(en_Input);
	}
	
	if(find(TargetIn.begin(), TargetIn.end(), g.in[1]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[1];
		en_Input.gate_name = g.in[1];
		AddEncoder(en_Input);
	}

	and1.gate_type = AND_GATE;
	and1.gate_name = g.gate_name + "_1";
	and1.out = g.out + "_1";
	and1.in.push_back(in01);   //In01
	and1.in.push_back(in11);   //In11
	ResultGate.push_back(and1);
		
	and0.gate_type = OR_GATE;	
	and0.gate_name = g.gate_name + "_0";
	and0.out = g.out + "_0";
	and0.in.push_back(in00);  //In00
	and0.in.push_back(in10);  //In10
	ResultGate.push_back(and0);


	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(and1.out);
		ResultOut.push_back(and0.out);
	}else{
		ResultWire.push_back(and1.out);
		ResultWire.push_back(and0.out);
	}

}

void Encoder::EncodeOR(Gate g){
	Gate or1, or0;
	
	EncodeConstantInput(g);
	if(find(TargetIn.begin(), TargetIn.end(), g.in[0]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[0];
		en_Input.gate_name = g.in[0];
		AddEncoder(en_Input);
	}
	
	if(find(TargetIn.begin(), TargetIn.end(), g.in[1]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[1];
		en_Input.gate_name = g.in[1];
		AddEncoder(en_Input);
	}

	or1.gate_type = OR_GATE;
	or1.gate_name = g.gate_name + "_1";
	or1.out = g.out + "_1";
	or1.in.push_back(in01);  //In01
	or1.in.push_back(in11); //In11
	ResultGate.push_back(or1);
		
	or0.gate_type = AND_GATE;	
	or0.gate_name = g.gate_name + "_0";
	or0.out = g.out + "_0";
	or0.in.push_back(in00);  //In00
	or0.in.push_back(in10);  //In10
	ResultGate.push_back(or0);

	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(or1.out);
		ResultOut.push_back(or0.out);
	}else{
		ResultWire.push_back(or1.out);	
		ResultWire.push_back(or0.out);
	}

}

void Encoder::EncodeNAND(Gate g){
	Gate nand1, nand0;
	
	EncodeConstantInput(g);
	if(find(TargetIn.begin(), TargetIn.end(), g.in[0]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[0];
		en_Input.gate_name = g.in[0];
		AddEncoder(en_Input);
	}
	
	if(find(TargetIn.begin(), TargetIn.end(), g.in[1]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[1];
		en_Input.gate_name = g.in[1];
		AddEncoder(en_Input);
	}

	nand0.gate_type = AND_GATE;
	nand0.gate_name = g.gate_name + "_0";
	nand0.out = g.out + "_0";
	nand0.in.push_back(in01);  //In01
	nand0.in.push_back(in11);  //In11
	ResultGate.push_back(nand0);
		
	nand1.gate_type = OR_GATE;	
	nand1.gate_name = g.gate_name + "_1";
	nand1.out = g.out + "_1";
	nand1.in.push_back(in00);  //In00
	nand1.in.push_back(in10);  //In10
	ResultGate.push_back(nand1);

	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(nand1.out);
		ResultOut.push_back(nand0.out);
	}else{
		ResultWire.push_back(nand0.out);
		ResultWire.push_back(nand1.out);
	}

}

void Encoder::EncodeNOR(Gate g){
	Gate nor1, nor0;
	
	EncodeConstantInput(g);
	if(find(TargetIn.begin(), TargetIn.end(), g.in[0]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[0];
		en_Input.gate_name = g.in[0];
		AddEncoder(en_Input);
	}
	
	if(find(TargetIn.begin(), TargetIn.end(), g.in[1]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[1];
		en_Input.gate_name = g.in[1];
		AddEncoder(en_Input);
	}

	nor0.gate_type = OR_GATE;
	nor0.gate_name = g.gate_name + "_0";
	nor0.out = g.out + "_0";
	nor0.in.push_back(in01);  //In01
	nor0.in.push_back(in11); //In11
	ResultGate.push_back(nor0);
		
	nor1.gate_type = AND_GATE;	
	nor1.gate_name = g.gate_name + "_1";
	nor1.out = g.out + "_1";
	nor1.in.push_back(in00);  //In00
	nor1.in.push_back(in10);  //In10
	ResultGate.push_back(nor1);

	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(nor1.out);
		ResultOut.push_back(nor0.out);
	}else{
		ResultWire.push_back(nor0.out);
		ResultWire.push_back(nor1.out);
	}

}

void Encoder::EncodeNOT(Gate g){
	Gate buf0, buf1;
		
	if(g.in[0] == "1'b0"){
		in01 = "1'b0";
		in00 = "1'b1";
	}else if(g.in[0] == "1'b1"){
		in01 = "1'b1";
		in00 = "1'b0";
	}else if(g.in[0] == "1'bx"){
		in01 = "1'b0";
		in00 = "1'b0";
	}else{
		in01 = g.in[0] + "_1";
		in00 = g.in[0] + "_0";
	}
			
	if(find(TargetIn.begin(), TargetIn.end(), g.in[0]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[0];
		en_Input.gate_name = g.in[0];
		AddEncoder(en_Input);
	}
	
	buf1.gate_type = BUF_GATE;
	buf1.gate_name = g.gate_name + "_1";
	buf1.out = g.out + "_1";
	
	buf1.in.push_back(in00);
	ResultGate.push_back(buf1);

	buf0.gate_type = BUF_GATE;
	buf0.gate_name = g.gate_name + "_0";
	buf0.out = g.out + "_0";
	
	buf0.in.push_back(in01);
	ResultGate.push_back(buf0);

	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(buf1.out);
		ResultOut.push_back(buf0.out);
	}else{
		ResultWire.push_back(buf1.out);
		ResultWire.push_back(buf0.out);
	}
}

void Encoder::EncodeBUF(Gate g){
	Gate buf0, buf1;
	
	if(g.in[0] == "1'b0"){
		in01 = "1'b0";
		in00 = "1'b1";
	}else if(g.in[0] == "1'b1"){
		in01 = "1'b1";
		in00 = "1'b0";
	}else if(g.in[0] == "1'bx"){
		in01 = "1'b0";
		in00 = "1'b0";
	}else{
		in01 = g.in[0] + "_1";
		in00 = g.in[0] + "_0";
	}
	if(find(TargetIn.begin(), TargetIn.end(), g.in[0]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[0];
		en_Input.gate_name = g.in[0];
		AddEncoder(en_Input);
	}
	
	buf0.gate_type = BUF_GATE;
	buf0.gate_name = g.gate_name + "_0";
	buf0.out = g.out + "_0";
	buf0.in.push_back(in00);   //In00
	ResultGate.push_back(buf0);

	buf1.gate_type = BUF_GATE;
	buf1.gate_name = g.gate_name + "_1";
	buf1.out = g.out + "_1";
	buf1.in.push_back(in01);  //In01
	ResultGate.push_back(buf1);

	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(buf1.out);
		ResultOut.push_back(buf0.out);
	}else{
		ResultWire.push_back(buf0.out);
		ResultWire.push_back(buf1.out);
	}
}

void Encoder::EncodeXOR(Gate g){
	Gate and00, and01;
	Gate and10, and11;
	Gate or0, or1;
	
	EncodeConstantInput(g);
	if(find(TargetIn.begin(), TargetIn.end(), g.in[0]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[0];
		en_Input.gate_name = g.in[0];
		AddEncoder(en_Input);
	}
	
	if(find(TargetIn.begin(), TargetIn.end(), g.in[1]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[1];
		en_Input.gate_name = g.in[1];
		AddEncoder(en_Input);
	}

	and00.gate_type = AND_GATE;
	and00.gate_name = g.gate_name + "_and00";
	and00.in.push_back(in00); //In00
	and00.in.push_back(in10); //In10
	and00.out = g.gate_name + "_and00_o";
	ResultWire.push_back(and00.out);
	ResultGate.push_back(and00);
	
	and01.gate_type = AND_GATE;
	and01.gate_name = g.gate_name + "_and01";
	and01.in.push_back(in01); //In01
	and01.in.push_back(in11); //In11
	and01.out = g.gate_name + "_and01_o";
	ResultWire.push_back(and01.out);
	ResultGate.push_back(and01);

	or0.gate_type = OR_GATE;
	or0.gate_name = g.gate_name + "_or0";
	or0.in.push_back(and00.out);
	or0.in.push_back(and01.out);
	or0.out = g.out + "_0";
	ResultGate.push_back(or0);
	
	and10.gate_type = AND_GATE;
	and10.gate_name = g.gate_name + "_and10";
	and10.in.push_back(in00); //In00
	and10.in.push_back(in11); //In11
	and10.out = g.gate_name + "_and10_o";
	ResultWire.push_back(and10.out);
	ResultGate.push_back(and10);

	and11.gate_type = AND_GATE;
	and11.gate_name = g.gate_name + "_and11";
	and11.in.push_back(in01); //In01
	and11.in.push_back(in10); //In10
	and11.out = g.gate_name + "_and11_o";
	ResultWire.push_back(and11.out);
	ResultGate.push_back(and11);

	or1.gate_type = OR_GATE;
	or1.gate_name = g.gate_name + "_or1";
	or1.in.push_back(and10.out);
	or1.in.push_back(and11.out);
	or1.out = g.out + "_1";
	ResultGate.push_back(or1);
	
	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(or1.out);
		ResultOut.push_back(or0.out);
	}else{
		ResultWire.push_back(or1.out);
		ResultWire.push_back(or0.out);
	}
}

void Encoder::EncodeXNOR(Gate g){
	Gate and00, and01;
	Gate and10, and11;
	Gate or0, or1;
	
	EncodeConstantInput(g);
	if(find(TargetIn.begin(), TargetIn.end(), g.in[0]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[0];
		en_Input.gate_name = g.in[0];
		AddEncoder(en_Input);
	}
	
	if(find(TargetIn.begin(), TargetIn.end(), g.in[1]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[1];
		en_Input.gate_name = g.in[1];
		AddEncoder(en_Input);
	}

	
	and10.gate_type = AND_GATE;
	and10.gate_name = g.gate_name + "_and10";
	and10.in.push_back(in00); //In00
	and10.in.push_back(in10); //In10
	and10.out = g.gate_name + "_and10_o";
	ResultWire.push_back(and10.out);
	ResultGate.push_back(and10);
	
	and11.gate_type = AND_GATE;
	and11.gate_name = g.gate_name + "_and11";
	and11.in.push_back(in01); //In01
	and11.in.push_back(in11); //In11
	and11.out = g.gate_name + "_and11_o";
	ResultWire.push_back(and11.out);
	ResultGate.push_back(and11);

	or1.gate_type = OR_GATE;
	or1.gate_name = g.gate_name + "_or1";
	or1.in.push_back(and10.out);
	or1.in.push_back(and11.out);
	or1.out = g.out + "_1";
	ResultGate.push_back(or1);
	
	and00.gate_type = AND_GATE;
	and00.gate_name = g.gate_name + "_and00";
	and00.in.push_back(in00); //In00
	and00.in.push_back(in11); //In11
	and00.out = g.gate_name + "_and00_o";
	ResultWire.push_back(and00.out);
	ResultGate.push_back(and00);

	and01.gate_type = AND_GATE;
	and01.gate_name = g.gate_name + "_and01";
	and01.in.push_back(in01); //In01
	and01.in.push_back(in10); //In10
	and01.out = g.gate_name + "_and01_o";
	ResultWire.push_back(and01.out);
	ResultGate.push_back(and01);

	or0.gate_type = OR_GATE;
	or0.gate_name = g.gate_name + "_or0";
	or0.in.push_back(and00.out);
	or0.in.push_back(and01.out);
	or0.out = g.out + "_0";
	ResultGate.push_back(or0);

	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(or1.out);
		ResultOut.push_back(or0.out);
	}else{
		ResultWire.push_back(or1.out);
		ResultWire.push_back(or0.out);
	}
}

void Encoder::EncodeDC(Gate g){
	Gate and1, and0;
	
	EncodeConstantInput(g);
	if(find(TargetIn.begin(), TargetIn.end(), g.in[0]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[0];
		en_Input.gate_name = g.in[0];
		AddEncoder(en_Input);
	}
	
	if(find(TargetIn.begin(), TargetIn.end(), g.in[1]) != TargetIn.end()){
		Gate en_Input;
		en_Input.out = g.in[1];
		en_Input.gate_name = g.in[1];
		AddEncoder(en_Input);
	}

	and1.gate_type = AND_GATE;
	and1.gate_name = g.gate_name + "_1";
	and1.out = g.out + "_1";
	ResultWire.push_back(and1.out);
	and1.in.push_back(in01);  //c1
	and1.in.push_back(in10);  //d0
	ResultGate.push_back(and1);
		
	and0.gate_type = AND_GATE;
	and0.gate_name = g.gate_name + "_0";
	and0.out = g.out + "_0";
	ResultWire.push_back(and0.out);
	and0.in.push_back(in00);   //c0
	and0.in.push_back(in01);   //c1
	ResultGate.push_back(and0);

	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(and1.out);
		ResultOut.push_back(and0.out);
	}
}

void Encoder::EncodeMUX(Gate g){
	Gate and00, and01, and02;
	Gate and10, and11, and12;
	Gate or00, or01, or10, or11;
	
	string in20, in21;   //IN0, IN1, S
	if(g.in[0] == "1'b0"){
		in01 = "1'b0";
		in00 = "1'b1";
	}else if(g.in[0] == "1'b1"){
		in01 = "1'b1";
		in00 = "1'b0";
	}else if(g.in[0] == "1'bx"){
		in01 = "1'b0";
		in00 = "1'b0";
	}else{
		in01 = g.in[0] + "_1";
		in00 = g.in[0] + "_0";
	}
	
	if(g.in[1] == "1'b0"){
		in11 = "1'b0";
		in10 = "1'b1";
	}else if(g.in[1] == "1'b1"){
		in11 = "1'b1";
		in10 = "1'b0";
	}else if(g.in[1] == "1'bx"){
		in11 = "1'b0";
		in10 = "1'b0";
	}else{
		in11 = g.in[1] + "_1";
		in10 = g.in[1] + "_0";
	}

	if(g.in[2] == "1'b0"){
		in21 = "1'b0";
		in20 = "1'b1";
	}else if(g.in[2] == "1'b1"){
		in21 = "1'b1";
		in20 = "1'b0";
	}else if(g.in[2] == "1'bx"){
		in21 = "1'b0";
		in20 = "1'b0";
	}else{
		in21 = g.in[2] + "_1";
		in20 = g.in[2] + "_0";
	}

	and00.gate_type = AND_GATE;
	and00.gate_name = g.gate_name + "_and00";
	and00.in.push_back(in21); //S1
	and00.in.push_back(in10); //In10
	and00.out = g.gate_name + "_and00_o";
	ResultWire.push_back(and00.out);
	ResultGate.push_back(and00);

	and01.gate_type = AND_GATE;
	and01.gate_name = g.gate_name + "_and01";
	and01.in.push_back(in10); //In10
	and01.in.push_back(in00); //In00
	and01.out = g.gate_name + "_and01_o";
	ResultWire.push_back(and01.out);
	ResultGate.push_back(and01);

	and02.gate_type = AND_GATE;
	and02.gate_name = g.gate_name + "_and02";
	and02.in.push_back(in20); //S0
	and02.in.push_back(in00); //In00
	and02.out = g.gate_name + "_and02_o";
	ResultWire.push_back(and02.out);
	ResultGate.push_back(and02);

	or00.gate_type = OR_GATE;
	or00.gate_name = g.gate_name + "_or00";
	or00.in.push_back(and00.out);
	or00.in.push_back(and01.out);
	or00.out = g.gate_name + "_or00_o";
	ResultWire.push_back(or00.out);
	ResultGate.push_back(or00);

	or01.gate_type = OR_GATE;
	or01.gate_name = g.gate_name + "_or01";
	or01.in.push_back(or00.out);
	or01.in.push_back(and02.out);
	or01.out = g.out + "_0";
	ResultGate.push_back(or01);
	
	and10.gate_type = AND_GATE;
	and10.gate_name = g.gate_name + "_and10";
	and10.in.push_back(in21); //S1
	and10.in.push_back(in11); //In11
	and10.out = g.gate_name + "_and10_o";
	ResultWire.push_back(and10.out);
	ResultGate.push_back(and10);

	and11.gate_type = AND_GATE;
	and11.gate_name = g.gate_name + "_and11";
	and11.in.push_back(in20); //S0
	and11.in.push_back(in01); //In01
	and11.out = g.gate_name + "_and11_o";
	ResultWire.push_back(and11.out);
	ResultGate.push_back(and11);

	and12.gate_type = AND_GATE;
	and12.gate_name = g.gate_name + "_and12";
	and12.in.push_back(in11); //In11
	and12.in.push_back(in01); //In01
	and12.out = g.gate_name + "_and12_o";
	ResultWire.push_back(and12.out);
	ResultGate.push_back(and12);

	or10.gate_type = OR_GATE;
	or10.gate_name = g.gate_name + "_or10";
	or10.in.push_back(and10.out);
	or10.in.push_back(and11.out);
	or10.out = g.gate_name + "_or10_o";
	ResultWire.push_back(or10.out);
	ResultGate.push_back(or10);

	or11.gate_type = OR_GATE;
	or11.gate_name = g.gate_name + "_or11";
	or11.in.push_back(and12.out);
	or11.in.push_back(or10.out);
	or11.out = g.out + "_1";
	ResultGate.push_back(or11);

	if(find(TargetOut.begin(), TargetOut.end(), g.out) != TargetOut.end()){
		ResultOut.push_back(or11.out);
		ResultOut.push_back(or01.out);
	}else{
		ResultWire.push_back(or11.out);
		ResultWire.push_back(or01.out);
	}
}
