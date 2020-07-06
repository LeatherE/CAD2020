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

int num_of_miter;


void BuildMiter(Circuit &miter, Circuit &golden, Circuit &revised){
	
	Gate PO;
	num_of_miter = 0;
	PO.gate_type = OR_GATE;
	PO.out = "PO";
	RenameWire(miter, golden ,revised);
	FindOutput(miter, golden, revised, PO);
	miter.OutList.push_back("PO");
	miter.GateList.push_back(PO);
	return;
}

void RenameWire(Circuit &miter, Circuit &golden, Circuit &revised){
	int i, j;
	
	for(i = 0; i < golden.InList.size(); i++){
		miter.InList.push_back(golden.InList[i]);
	}
	RenamePI(miter, golden ,revised);
	for(i = 0; i < golden.WireList.size(); i++){
		miter.WireList.push_back(golden.WireList[i] + "_g");
	}
	for(i = 0; i < revised.WireList.size(); i++){
		miter.WireList.push_back(revised.WireList[i] + "_r");
	}
	for(i = 0; i < golden.OutList.size(); i++){
		miter.WireList.push_back(golden.OutList[i] + "_g");
	}
	for(i = 0; i < revised.OutList.size(); i++){
		miter.WireList.push_back(revised.OutList[i] + "_r");
	}
	for(i = 0; i < golden.GateList.size(); i++){
		Gate g;
		g.gate_type = golden.GateList[i].gate_type;
		g.out = golden.GateList[i].out + "_g";
		for(j = 0; j < golden.GateList[i].in.size(); j++){
			if(golden.GateList[i].in[j] == "1'b0" || golden.GateList[i].in[j] == "1'b1")
				g.in.push_back(golden.GateList[i].in[j]);
			else
				g.in.push_back(golden.GateList[i].in[j] + "_g");
		}
		miter.GateList.push_back(g);
	}
	for(i = 0; i < revised.GateList.size(); i++){
		Gate g;
		g.gate_type = revised.GateList[i].gate_type;
		g.out = revised.GateList[i].out + "_r";
		for(j = 0; j < revised.GateList[i].in.size(); j++){
			if(revised.GateList[i].in[j] == "1'b0" || revised.GateList[i].in[j] == "1'b1")
				g.in.push_back(revised.GateList[i].in[j]);
			else
				g.in.push_back(revised.GateList[i].in[j] + "_r");
		}
		miter.GateList.push_back(g);
	}
}

void RenamePI(Circuit &miter, Circuit &golden, Circuit &revised){
	int i;
	
	for(i = 0; i < golden.InList.size(); i++){
		Gate buf_g, buf_r;
		buf_g.gate_type = BUF_GATE;
		buf_g.in.push_back(golden.InList[i]);
		buf_g.out = golden.InList[i] + "_g";
		miter.WireList.push_back(buf_g.out);
		miter.GateList.push_back(buf_g);
		buf_r.gate_type = BUF_GATE;
		buf_r.in.push_back(golden.InList[i]);
		buf_r.out = golden.InList[i] + "_r";
		miter.WireList.push_back(buf_r.out);
		miter.GateList.push_back(buf_r);
	}
}

void FindOutput(Circuit &miter, Circuit &golden, Circuit &revised, Gate &PO){
	int i, j;
	string s;
	
	for (i = 0; i < golden.OutList.size(); i++){
		s = golden.OutList[i];
		for (j = 0; j < revised.OutList.size(); j++){
			if (s.compare(revised.OutList[j]) == 0){
				if(s[s.size()-2] == '_' && s[s.size()-1] == '1')
					AddMiter1(s, golden.OutList[++i], revised.OutList[j], revised.OutList[j+1], miter, PO);
				else
					AddMiter4(s, revised.OutList[j], miter, PO);
				break;
			} else if(s[s.size()-2] == '_' && s[s.size()-1] == '1'){
				if(s.substr(0, s.size()-2).compare(revised.OutList[j]) == 0){
					AddMiter2(s, golden.OutList[++i], revised.OutList[j], miter, PO);
					break;
				}	
			} else if(s.compare(0, s.size(), revised.OutList[j].substr(0, s.size())) == 0){
				AddMiter3(s, revised.OutList[j], revised.OutList[j+1], miter, PO);
				break;
			}
		}
	}
	
}

void AddMiter1(string g1, string g0, string r1, string r0, Circuit &miter, Gate &PO){
	Gate and1, and0, or0, not1, not0;
	string s;

	not1.gate_type = NOT_GATE;
	not1.out = r1 + "_n";
	not1.in.push_back(r1 + "_r");
	miter.WireList.push_back(not1.out);
	miter.GateList.push_back(not1);

	and1.gate_type = AND_GATE;
	and1.out = g1 + "_and";
	and1.in.push_back(g1 + "_g");
	and1.in.push_back(not1.out);
	miter.WireList.push_back(and1.out);
	miter.GateList.push_back(and1);
	
	not0.gate_type = NOT_GATE;
	not0.out = r0 + "_n";
	not0.in.push_back(r0 + "_r");
	miter.WireList.push_back(not0.out);
	miter.GateList.push_back(not0);

	and0.gate_type = AND_GATE;
	and0.out = g0 + "_and";
	and0.in.push_back(g0 + "_g");
	and0.in.push_back(not0.out);
	miter.WireList.push_back(and0.out);
	miter.GateList.push_back(and0);
	
	s = to_string(num_of_miter++);
	
	or0.gate_type = OR_GATE;
	or0.out = "Miter_" + s;
	or0.in.push_back(and0.out);
	or0.in.push_back(and1.out);
	miter.GateList.push_back(or0);
	miter.WireList.push_back(or0.out);
	PO.in.push_back(or0.out);
	
}

void AddMiter2(string g1, string g0, string r, Circuit &miter, Gate &PO){
	Gate and1, and0, or0, not0;
	string s;
	
	not0.gate_type = NOT_GATE;
	not0.out = r + "_n";
	not0.in.push_back(r + "_r");
	miter.WireList.push_back(not0.out);
	miter.GateList.push_back(not0);

	and1.gate_type = AND_GATE;
	and1.out = g1 + "_and";
	and1.in.push_back(g1 + "_g");
	and1.in.push_back(not0.out);
	miter.WireList.push_back(and1.out);
	miter.GateList.push_back(and1);

	and0.gate_type = AND_GATE;
	and0.out = g0 + "_and";
	and0.in.push_back(g0 + "_g");
	and0.in.push_back(r + "_r");
	miter.WireList.push_back(and0.out);
	miter.GateList.push_back(and0);
	
	s = to_string(num_of_miter++);
	
	or0.gate_type = OR_GATE;
	or0.out = "Miter_" + s;
	or0.in.push_back(and0.out);
	or0.in.push_back(and1.out);
	miter.GateList.push_back(or0);
	miter.WireList.push_back(or0.out);
	PO.in.push_back(or0.out);
}

void AddMiter3(string g, string r1, string r0, Circuit &miter, Gate &PO){
	Gate and1, and0, or0, not2, not1, not0;
	string s;
	
	not2.gate_type = NOT_GATE;
	not2.out = r1 + "_n";
	not2.in.push_back(r1 + "_r");
	miter.WireList.push_back(not2.out);
	miter.GateList.push_back(not2);

	not1.gate_type = NOT_GATE;
	not1.out = r0 + "_n";
	not1.in.push_back(r0 + "_r");
	miter.WireList.push_back(not1.out);
	miter.GateList.push_back(not1);
	
	not0.gate_type = NOT_GATE;
	not0.out = g + "_n";
	not0.in.push_back(g + "_g");
	miter.WireList.push_back(not0.out);
	miter.GateList.push_back(not0);

	and1.gate_type = AND_GATE;
	and1.out = r0 + "_and";
	and1.in.push_back(not1.out);
	and1.in.push_back(not0.out);
	miter.WireList.push_back(and1.out);
	miter.GateList.push_back(and1);

	and0.gate_type = AND_GATE;
	and0.out = r1 + "_and";
	and0.in.push_back(g + "_g");
	and0.in.push_back(not2.out);
	miter.WireList.push_back(and0.out);
	miter.GateList.push_back(and0);
	
	s = to_string(num_of_miter++);
	
	or0.gate_type = OR_GATE;
	or0.out = "Miter_" + s;
	or0.in.push_back(and0.out);
	or0.in.push_back(and1.out);
	miter.GateList.push_back(or0);
	miter.WireList.push_back(or0.out);
	PO.in.push_back(or0.out);
}

void AddMiter4(string g, string r, Circuit &miter, Gate &PO){
	Gate and1, and0, or0, not1, not0, xor0;
	string s;

	s = to_string(num_of_miter++);
	xor0.gate_type = XOR_GATE;
	xor0.out = "Miter_" + s;
	xor0.in.push_back(r + "_r");
	xor0.in.push_back(g + "_g");
	miter.WireList.push_back(xor0.out);
	PO.in.push_back(xor0.out);
	miter.GateList.push_back(xor0);
}
