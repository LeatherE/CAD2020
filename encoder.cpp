#include "datatype.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string>
#include <time.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

using namespace std; 

void EncodeCircuit(Circuit &encoded, Circuit &target){

	int i, j, k;
	Gate g;
	Gate en_Input;
	
	for(i = 0; i < target.InList.size(); i++){
		encoded.InList.push_back(target.InList[i]);
		en_Input.out = target.InList[i];
		en_Input.gate_name = target.InList[i];
		AddEncoder(encoded, en_Input);
	}
	
	for(i = 0; i < target.first_xgate ; i++){
		g = target.GateList[target.top_order[i]];
		
		if(g.gate_type == MUX_GATE)
			TransferMUX(encoded, g, target);
		else {
			encoded.GateList.push_back(g);
			if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
				encoded.OutList.push_back(g.out);
			}else{
				encoded.WireList.push_back(g.out);
			}	
		}


		for(j = 0; j < g.fout.size(); j++){	// add encoder before encoded gate
			for(k = 0; k < target.GateList.size(); k++){
				if(target.top_order[k] == g.fout[j])
					break;
			}
			if(k >= target.first_xgate){
				AddEncoder(encoded, g);
				break;
			}	
		}	
	}
	for(i = target.first_xgate; i < target.GateList.size(); i++){
		g = target.GateList[target.top_order[i]];
		switch(g.gate_type){
			case AND_GATE:
				EncodeAND(encoded, g, target);
				break;
			case OR_GATE:
				EncodeOR(encoded, g, target);
				break;
			case NAND_GATE:
				EncodeNAND(encoded, g, target);
				break;
			case NOR_GATE:
				EncodeNOR(encoded, g, target);
				break;
			case NOT_GATE:
				EncodeNOT(encoded, g, target);
				break;
			case BUF_GATE:
				EncodeBUF(encoded, g, target);
				break;
			case XOR_GATE:
				EncodeXOR(encoded, g, target);
				break;
			case XNOR_GATE:
				EncodeXNOR(encoded, g, target);
				break;
			case DC_GATE:
				EncodeDC(encoded, g, target);
				break;
			case MUX_GATE:
				EncodeMUX(encoded, g, target);
				break;
		}	
	}

}

void AddEncoder(Circuit &encoded, Gate &g){
	string encoder_out1, encoder_out0;
	Gate encode_not, encode_buf;
	
	encode_not.gate_type = NOT_GATE;
	encode_not.in.push_back(g.out);
	encode_not.out = g.out + "_0";
	encode_not.gate_name = g.gate_name + "_en";
	encoded.WireList.push_back(encode_not.out);
	encoded.GateList.push_back(encode_not);
	
	encode_buf.gate_type = BUF_GATE;
	encode_buf.in.push_back(g.out);
	encode_buf.out = g.out + "_1";
	encode_buf.gate_name = g.gate_name + "_buf";
	encoded.WireList.push_back(encode_buf.out);
	encoded.GateList.push_back(encode_buf);

}

void EncodeInput(vector<string> &EnIn, string &in){
	if(in == "1'b0"){
		EnIn.push_back("1'b1");
		EnIn.push_back("1'b0");
	}else if(in == "1'b1"){
		EnIn.push_back("1'b0");
		EnIn.push_back("1'b1");
	}else if(in == "1'bx"){
		EnIn.push_back("1'b0");
		EnIn.push_back("1'b0");
	}else{
		EnIn.push_back(in + "_0");
		EnIn.push_back(in + "_1");
	}
}

void EncodeAND(Circuit &encoded, Gate &g, Circuit &target){
		
	if(g.in.size() > 2){
		int i = 1;
		string s;
		while(g.in.size() >= 2){
			Gate new_gate;
			s = to_string(i++);
			new_gate.gate_type = AND_GATE;
			new_gate.gate_name = g.gate_name + "_" + s;
			if(g.in.size() == 2)
				new_gate.out = g.out;
			else
				new_gate.out = g.out + "_w" + s;
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			g.in.push_back(new_gate.out);
			EncodeAND(encoded, new_gate, target);
		}
	} 
	else{
		Gate and1, and0;
		vector<vector<string>> EnIn;
		for (int i = 0; i < g.in.size(); i++){
			vector<string> t;
			EncodeInput(t, g.in[i]);
			EnIn.push_back(t);
		}
		and1.gate_type = AND_GATE;
		and1.gate_name = g.gate_name + "_1";
		and1.out = g.out + "_1";
		and1.in.push_back(EnIn[0][1]);   //In01
		and1.in.push_back(EnIn[1][1]);   //In11
		encoded.GateList.push_back(and1);
			
		and0.gate_type = OR_GATE;	
		and0.gate_name = g.gate_name + "_0";
		and0.out = g.out + "_0";
		and0.in.push_back(EnIn[0][0]);  //In00
		and0.in.push_back(EnIn[1][0]);  //In10
		encoded.GateList.push_back(and0);

		if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
			encoded.OutList.push_back(and1.out);
			encoded.OutList.push_back(and0.out);
		}else{
			encoded.WireList.push_back(and1.out);
			encoded.WireList.push_back(and0.out);
		}
	}
}

void EncodeOR(Circuit &encoded, Gate &g, Circuit &target){
	
	if(g.in.size() > 2){
		int i = 1;
		string s;
		while(g.in.size() >= 2){
			Gate new_gate;
			s = to_string(i++);
			new_gate.gate_type = OR_GATE;
			new_gate.gate_name = g.gate_name + "_" + s;
			if(g.in.size() == 2)
				new_gate.out = g.out;
			else
				new_gate.out = g.out + "_w" + s;
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			g.in.push_back(new_gate.out);
			EncodeOR(encoded, new_gate, target);
		}
	} 
	else{
		Gate or1, or0;
		vector<vector<string>> EnIn;
		for (int i = 0; i < g.in.size(); i++){
			vector<string> t;
			EncodeInput(t, g.in[i]);
			EnIn.push_back(t);
		}
		or1.gate_type = OR_GATE;
		or1.gate_name = g.gate_name + "_1";
		or1.out = g.out + "_1";
		or1.in.push_back(EnIn[0][1]);  //In01
		or1.in.push_back(EnIn[1][1]); //In11
		encoded.GateList.push_back(or1);
			
		or0.gate_type = AND_GATE;	
		or0.gate_name = g.gate_name + "_0";
		or0.out = g.out + "_0";
		or0.in.push_back(EnIn[0][0]);  //In00
		or0.in.push_back(EnIn[1][0]);  //In10
		encoded.GateList.push_back(or0);

		if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
			encoded.OutList.push_back(or1.out);
			encoded.OutList.push_back(or0.out);
		}else{
			encoded.WireList.push_back(or1.out);	
			encoded.WireList.push_back(or0.out);
		}
	}
}

void EncodeNAND(Circuit &encoded, Gate &g, Circuit &target){
	if(g.in.size() > 2){
		int i = 1;
		Gate end;
		string s;
		while(g.in.size() > 2){
			Gate new_gate;
			s = to_string(i++);
			new_gate.gate_type = AND_GATE;
			new_gate.gate_name = g.gate_name + "_" + s;
			new_gate.out = g.out + "_w" + s;
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			g.in.push_back(new_gate.out);
			EncodeAND(encoded, new_gate, target);
		}
		end.gate_type = NAND_GATE;
		end.out = g.out;
		end.in.push_back(g.in.back());
		g.in.pop_back();
		end.in.push_back(g.in.back());
		g.in.pop_back();
		EncodeNAND(encoded, end, target);
	} 
	else{
		Gate nand1, nand0;
		vector<vector<string>> EnIn;
		for (int i = 0; i < g.in.size(); i++){
			vector<string> t;
			EncodeInput(t, g.in[i]);
			EnIn.push_back(t);
		}
		nand0.gate_type = AND_GATE;
		nand0.gate_name = g.gate_name + "_0";
		nand0.out = g.out + "_0";
		nand0.in.push_back(EnIn[0][1]);  //In01
		nand0.in.push_back(EnIn[1][1]);  //In11
		encoded.GateList.push_back(nand0);
			
		nand1.gate_type = OR_GATE;	
		nand1.gate_name = g.gate_name + "_1";
		nand1.out = g.out + "_1";
		nand1.in.push_back(EnIn[0][0]);  //In00
		nand1.in.push_back(EnIn[1][0]);  //In10
		encoded.GateList.push_back(nand1);

		if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
			encoded.OutList.push_back(nand1.out);
			encoded.OutList.push_back(nand0.out);
		}else{
			encoded.WireList.push_back(nand0.out);
			encoded.WireList.push_back(nand1.out);
		}
		
	}
}

void EncodeNOR(Circuit &encoded, Gate &g, Circuit &target){
	if(g.in.size() > 2){
		int i = 1;
		Gate end;
		string s;
		while(g.in.size() > 2){
			Gate new_gate;
			s = to_string(i++);
			new_gate.gate_type = OR_GATE;
			new_gate.gate_name = g.gate_name + "_" + s;
			new_gate.out = g.out + "_w" + s;
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			g.in.push_back(new_gate.out);
			EncodeOR(encoded, new_gate, target);
		}
		end.gate_type = NOR_GATE;
		end.out = g.out;
		end.in.push_back(g.in.back());
		g.in.pop_back();
		end.in.push_back(g.in.back());
		g.in.pop_back();
		EncodeNOR(encoded, end, target);
	} 
	else{
		Gate nor1, nor0;
		vector<vector<string>> EnIn;
		for (int i = 0; i < g.in.size(); i++){
			vector<string> t;
			EncodeInput(t, g.in[i]);
			EnIn.push_back(t);
		}
		nor0.gate_type = OR_GATE;
		nor0.gate_name = g.gate_name + "_0";
		nor0.out = g.out + "_0";
		nor0.in.push_back(EnIn[0][1]);  //In01
		nor0.in.push_back(EnIn[1][1]); //In11
		encoded.GateList.push_back(nor0);
			
		nor1.gate_type = AND_GATE;	
		nor1.gate_name = g.gate_name + "_1";
		nor1.out = g.out + "_1";
		nor1.in.push_back(EnIn[0][0]);  //In00
		nor1.in.push_back(EnIn[1][0]);  //In10
		encoded.GateList.push_back(nor1);

		if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
			encoded.OutList.push_back(nor1.out);
			encoded.OutList.push_back(nor0.out);
		}else{
			encoded.WireList.push_back(nor0.out);
			encoded.WireList.push_back(nor1.out);
		}
	}
}

void EncodeNOT(Circuit &encoded, Gate &g, Circuit &target){
	Gate buf0, buf1;
	
	vector<vector<string>> EnIn;
	for (int i = 0; i < g.in.size(); i++){
		vector<string> t;
		EncodeInput(t, g.in[i]);
		EnIn.push_back(t);
	}
	
	buf1.gate_type = BUF_GATE;
	buf1.gate_name = g.gate_name + "_1";
	buf1.out = g.out + "_1";
	buf1.in.push_back(EnIn[0][0]);
	encoded.GateList.push_back(buf1);

	buf0.gate_type = BUF_GATE;
	buf0.gate_name = g.gate_name + "_0";
	buf0.out = g.out + "_0";
	buf0.in.push_back(EnIn[0][1]);
	encoded.GateList.push_back(buf0);

	if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
		encoded.OutList.push_back(buf1.out);
		encoded.OutList.push_back(buf0.out);
	}else{
		encoded.WireList.push_back(buf1.out);
		encoded.WireList.push_back(buf0.out);
	}
}

void EncodeBUF(Circuit &encoded, Gate &g, Circuit &target){
	Gate buf0, buf1;
	vector<vector<string>> EnIn;
	for (int i = 0; i < g.in.size(); i++){
		vector<string> t;
		EncodeInput(t, g.in[i]);
		EnIn.push_back(t);
	}
	
	buf0.gate_type = BUF_GATE;
	buf0.gate_name = g.gate_name + "_0";
	buf0.out = g.out + "_0";
	buf0.in.push_back(EnIn[0][0]);   //In00
	encoded.GateList.push_back(buf0);

	buf1.gate_type = BUF_GATE;
	buf1.gate_name = g.gate_name + "_1";
	buf1.out = g.out + "_1";
	buf1.in.push_back(EnIn[0][1]);  //In01
	encoded.GateList.push_back(buf1);

	if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
		encoded.OutList.push_back(buf1.out);
		encoded.OutList.push_back(buf0.out);
	}else{
		encoded.WireList.push_back(buf0.out);
		encoded.WireList.push_back(buf1.out);
	}
}

void EncodeXOR(Circuit &encoded, Gate &g, Circuit &target){
	if(g.in.size() > 2){
		int i = 1;
		string s;
		while(g.in.size() >= 2){
			Gate new_gate;
			s = to_string(i++);
			new_gate.gate_type = XOR_GATE;
			new_gate.gate_name = g.gate_name + "_" + s;
			if(g.in.size() == 2)
				new_gate.out = g.out;
			else
				new_gate.out = g.out + "_w" + s;
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			g.in.push_back(new_gate.out);
			EncodeXOR(encoded, new_gate, target);
		}
	} 
	else{
		Gate and00, and01;
		Gate and10, and11;
		Gate or0, or1;
		vector<vector<string>> EnIn;
		for (int i = 0; i < g.in.size(); i++){
			vector<string> t;
			EncodeInput(t, g.in[i]);
			EnIn.push_back(t);
		}
		and00.gate_type = AND_GATE;
		and00.gate_name = g.gate_name + "_and00";
		and00.in.push_back(EnIn[0][0]); //In00
		and00.in.push_back(EnIn[1][0]); //In10
		and00.out = g.gate_name + "_and00_o";
		encoded.WireList.push_back(and00.out);
		encoded.GateList.push_back(and00);
		
		and01.gate_type = AND_GATE;
		and01.gate_name = g.gate_name + "_and01";
		and01.in.push_back(EnIn[0][1]); //In01
		and01.in.push_back(EnIn[1][1]); //In11
		and01.out = g.gate_name + "_and01_o";
		encoded.WireList.push_back(and01.out);
		encoded.GateList.push_back(and01);

		or0.gate_type = OR_GATE;
		or0.gate_name = g.gate_name + "_or0";
		or0.in.push_back(and00.out);
		or0.in.push_back(and01.out);
		or0.out = g.out + "_0";
		encoded.GateList.push_back(or0);
		
		and10.gate_type = AND_GATE;
		and10.gate_name = g.gate_name + "_and10";
		and10.in.push_back(EnIn[0][0]); //In00
		and10.in.push_back(EnIn[1][1]); //In11
		and10.out = g.gate_name + "_and10_o";
		encoded.WireList.push_back(and10.out);
		encoded.GateList.push_back(and10);

		and11.gate_type = AND_GATE;
		and11.gate_name = g.gate_name + "_and11";
		and11.in.push_back(EnIn[0][1]); //In01
		and11.in.push_back(EnIn[1][0]); //In10
		and11.out = g.gate_name + "_and11_o";
		encoded.WireList.push_back(and11.out);
		encoded.GateList.push_back(and11);

		or1.gate_type = OR_GATE;
		or1.gate_name = g.gate_name + "_or1";
		or1.in.push_back(and10.out);
		or1.in.push_back(and11.out);
		or1.out = g.out + "_1";
		encoded.GateList.push_back(or1);
		
		if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
			encoded.OutList.push_back(or1.out);
			encoded.OutList.push_back(or0.out);
		}else{
			encoded.WireList.push_back(or1.out);
			encoded.WireList.push_back(or0.out);
		}
	}
}

void EncodeXNOR(Circuit &encoded, Gate &g, Circuit &target){
	if(g.in.size() > 2){
		int i = 1;
		Gate end;
		string s;
		while(g.in.size() > 2){
			Gate new_gate;
			s = to_string(i++);
			new_gate.gate_type = XOR_GATE;
			new_gate.gate_name = g.gate_name + "_" + s;
			new_gate.out = g.out + "_w" + s;
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			new_gate.in.push_back(g.in.back());
			g.in.pop_back();
			g.in.push_back(new_gate.out);
			EncodeXOR(encoded, new_gate, target);
		}
		end.gate_type = XNOR_GATE;
		end.out = g.out;
		end.in.push_back(g.in.back());
		g.in.pop_back();
		end.in.push_back(g.in.back());
		g.in.pop_back();
		EncodeXNOR(encoded, end, target);
	}
	else{
		Gate and00, and01;
		Gate and10, and11;
		Gate or0, or1;
		vector<vector<string>> EnIn;
		for (int i = 0; i < g.in.size(); i++){
			vector<string> t;
			EncodeInput(t, g.in[i]);
			EnIn.push_back(t);
		}
		and10.gate_type = AND_GATE;
		and10.gate_name = g.gate_name + "_and10";
		and10.in.push_back(EnIn[0][0]); //In00
		and10.in.push_back(EnIn[1][0]); //In10
		and10.out = g.gate_name + "_and10_o";
		encoded.WireList.push_back(and10.out);
		encoded.GateList.push_back(and10);
		
		and11.gate_type = AND_GATE;
		and11.gate_name = g.gate_name + "_and11";
		and11.in.push_back(EnIn[0][1]); //In01
		and11.in.push_back(EnIn[1][1]); //In11
		and11.out = g.gate_name + "_and11_o";
		encoded.WireList.push_back(and11.out);
		encoded.GateList.push_back(and11);

		or1.gate_type = OR_GATE;
		or1.gate_name = g.gate_name + "_or1";
		or1.in.push_back(and10.out);
		or1.in.push_back(and11.out);
		or1.out = g.out + "_1";
		encoded.GateList.push_back(or1);
		
		and00.gate_type = AND_GATE;
		and00.gate_name = g.gate_name + "_and00";
		and00.in.push_back(EnIn[0][0]); //In00
		and00.in.push_back(EnIn[1][1]); //In11
		and00.out = g.gate_name + "_and00_o";
		encoded.WireList.push_back(and00.out);
		encoded.GateList.push_back(and00);

		and01.gate_type = AND_GATE;
		and01.gate_name = g.gate_name + "_and01";
		and01.in.push_back(EnIn[0][1]); //In01
		and01.in.push_back(EnIn[1][0]); //In10
		and01.out = g.gate_name + "_and01_o";
		encoded.WireList.push_back(and01.out);
		encoded.GateList.push_back(and01);

		or0.gate_type = OR_GATE;
		or0.gate_name = g.gate_name + "_or0";
		or0.in.push_back(and00.out);
		or0.in.push_back(and01.out);
		or0.out = g.out + "_0";
		encoded.GateList.push_back(or0);

		if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
			encoded.OutList.push_back(or1.out);
			encoded.OutList.push_back(or0.out);
		}else{
			encoded.WireList.push_back(or1.out);
			encoded.WireList.push_back(or0.out);
		}
	}
}

void EncodeDC(Circuit &encoded, Gate &g, Circuit &target){
	Gate and1, and0;
	vector<vector<string>> EnIn;
	for (int i = 0; i < g.in.size(); i++){
		vector<string> t;
		EncodeInput(t, g.in[i]);
		EnIn.push_back(t);
	}

	and1.gate_type = AND_GATE;
	and1.gate_name = g.gate_name + "_1";
	and1.out = g.out + "_1";
	encoded.WireList.push_back(and1.out);
	and1.in.push_back(EnIn[0][1]);  //c1
	and1.in.push_back(EnIn[1][0]);  //d0
	encoded.GateList.push_back(and1);	
	and0.gate_type = AND_GATE;
	and0.gate_name = g.gate_name + "_0";
	and0.out = g.out + "_0";
	encoded.WireList.push_back(and0.out);
	and0.in.push_back(EnIn[0][0]);   //c0
	and0.in.push_back(EnIn[1][0]);   //d0
	encoded.GateList.push_back(and0);
	if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
		encoded.OutList.push_back(and1.out);
		encoded.OutList.push_back(and0.out);
	}
}

void EncodeMUX(Circuit &encoded, Gate &g, Circuit &target){
	Gate and00, and01, and02;
	Gate and10, and11, and12;
	Gate or00, or01, or10, or11;
	vector<vector<string>> EnIn;
	for (int i = 0; i < g.in.size(); i++){
		vector<string> t;
		EncodeInput(t, g.in[i]);
		EnIn.push_back(t);
	}

	and00.gate_type = AND_GATE;
	and00.gate_name = g.gate_name + "_and00";
	and00.in.push_back(EnIn[2][1]); //S1
	and00.in.push_back(EnIn[1][0]); //In10
	and00.out = g.gate_name + "_and00_o";
	encoded.WireList.push_back(and00.out);
	encoded.GateList.push_back(and00);

	and01.gate_type = AND_GATE;
	and01.gate_name = g.gate_name + "_and01";
	and01.in.push_back(EnIn[1][0]); //In10
	and01.in.push_back(EnIn[0][0]); //In00
	and01.out = g.gate_name + "_and01_o";
	encoded.WireList.push_back(and01.out);
	encoded.GateList.push_back(and01);

	and02.gate_type = AND_GATE;
	and02.gate_name = g.gate_name + "_and02";
	and02.in.push_back(EnIn[2][0]); //S0
	and02.in.push_back(EnIn[0][0]); //In00
	and02.out = g.gate_name + "_and02_o";
	encoded.WireList.push_back(and02.out);
	encoded.GateList.push_back(and02);

	or00.gate_type = OR_GATE;
	or00.gate_name = g.gate_name + "_or00";
	or00.in.push_back(and00.out);
	or00.in.push_back(and01.out);
	or00.out = g.gate_name + "_or00_o";
	encoded.WireList.push_back(or00.out);
	encoded.GateList.push_back(or00);

	or01.gate_type = OR_GATE;
	or01.gate_name = g.gate_name + "_or01";
	or01.in.push_back(or00.out);
	or01.in.push_back(and02.out);
	or01.out = g.out + "_0";
	encoded.GateList.push_back(or01);
	
	and10.gate_type = AND_GATE;
	and10.gate_name = g.gate_name + "_and10";
	and10.in.push_back(EnIn[2][1]); //S1
	and10.in.push_back(EnIn[1][1]); //In11
	and10.out = g.gate_name + "_and10_o";
	encoded.WireList.push_back(and10.out);
	encoded.GateList.push_back(and10);

	and11.gate_type = AND_GATE;
	and11.gate_name = g.gate_name + "_and11";
	and11.in.push_back(EnIn[2][0]); //S0
	and11.in.push_back(EnIn[0][1]); //In01
	and11.out = g.gate_name + "_and11_o";
	encoded.WireList.push_back(and11.out);
	encoded.GateList.push_back(and11);

	and12.gate_type = AND_GATE;
	and12.gate_name = g.gate_name + "_and12";
	and12.in.push_back(EnIn[1][1]); //In11
	and12.in.push_back(EnIn[0][1]); //In01
	and12.out = g.gate_name + "_and12_o";
	encoded.WireList.push_back(and12.out);
	encoded.GateList.push_back(and12);

	or10.gate_type = OR_GATE;
	or10.gate_name = g.gate_name + "_or10";
	or10.in.push_back(and10.out);
	or10.in.push_back(and11.out);
	or10.out = g.gate_name + "_or10_o";
	encoded.WireList.push_back(or10.out);
	encoded.GateList.push_back(or10);

	or11.gate_type = OR_GATE;
	or11.gate_name = g.gate_name + "_or11";
	or11.in.push_back(and12.out);
	or11.in.push_back(or10.out);
	or11.out = g.out + "_1";
	encoded.GateList.push_back(or11);

	if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
		encoded.OutList.push_back(or11.out);
		encoded.OutList.push_back(or01.out);
	}else{
		encoded.WireList.push_back(or11.out);
		encoded.WireList.push_back(or01.out);
	}
}

void TransferMUX(Circuit &encoded, Gate &g, Circuit &target){
	Gate or0, not0, and1, and0;

	not0.gate_type = NOT_GATE;			// ~S
	not0.gate_name = g.gate_name + "_mux_n";
	not0.out = g.out + "_not_s";
	not0.in.push_back(g.in[2]);		// S
	encoded.WireList.push_back(not0.out);
	encoded.GateList.push_back(not0);
	
	and1.gate_type = AND_GATE;				//(~S&I0)
	and1.gate_name = g.gate_name + "_and_1";
	and1.out = g.out + "_and_1";
	and1.in.push_back(g.in[0]);   	//I0
	and1.in.push_back(not0.out);   	//~S
	encoded.WireList.push_back(and1.out);
	encoded.GateList.push_back(and1);
		
	and0.gate_type = AND_GATE;				// (S&I1)
	and0.gate_name = g.gate_name + "_and_0";
	and0.out = g.out + "_and_0";
	and0.in.push_back(g.in[1]);  	//I1
	and0.in.push_back(g.in[2]);  	//S
	encoded.WireList.push_back(and0.out);
	encoded.GateList.push_back(and0);

	or0.gate_type = OR_GATE;				// (~S&I0)+(S&I1)
	or0.gate_name = g.gate_name + "_or_0";
	or0.out = g.out;
	or0.in.push_back(and0.out);  	// (S&I1)
	or0.in.push_back(and1.out);  	// (~S&I0)
	encoded.GateList.push_back(or0);
	
	if(find(target.OutList.begin(), target.OutList.end(), g.out) != target.OutList.end()){
		encoded.OutList.push_back(or0.out);
	}else{
		encoded.WireList.push_back(or0.out);
	}
}	
