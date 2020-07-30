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

// ===========================================================================
void Ntk_DfsRec(Jt_Ntk &pNtk, vector<Jt_Node *> &p_order, Jt_Node *n){
	Jt_Node *fi;
	int i;

	if(Jt_NodeVisit(n))		return;

	n->visit = 1;
	Jt_NodeForEachFi(n, fi, i){
		Ntk_DfsRec(pNtk, p_order, fi);
	}
	if(!p_order.empty() && p_order.back()->XGate)
		n->XGate = true;
	p_order.push_back(n);
		
}
// ===========================================================================
void Ntk_Dfs(Jt_Ntk &pNtk, vector<Jt_Node *> &p_order){
	Jt_Node *n;
	int i;
	Jt_NtkForEachPo(pNtk, n, i){
		Ntk_DfsRec(pNtk, p_order, n);
	}
	Jt_NtkForEachNode(pNtk, n, i){
		if(!Jt_NodeVisit(n)){
			Ntk_DfsRec(pNtk, p_order, n);
		}
	}
}
// ===========================================================================
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
// ===========================================================================
void EncodeBUF(Jt_Ntk &encoded, Jt_Node *g){
	Jt_Node *buf0 = NULL, *buf1 = NULL;
	buf0 = new Jt_Node;
	buf1 = new Jt_Node;
	
	vector<vector <string> > EnIn;
	for (int i = 0; i < g->in.size(); i++){
		vector<string> t;
		EncodeInput(t, g->in[i]);
		EnIn.push_back(t);
	}
	buf0->Type = node_BUF;
	buf0->Name = g->Name;// + "_0";
	buf0->out = g->out + "_0";
	buf0->in.push_back(EnIn[0][0]);   //In00
	GatePushBack(encoded, buf0, buf0->Type);
	encoded.Ntk_AllNode.push_back(buf0);

	buf1->Type = node_BUF;
	buf1->Name = g->Name;// + "_1";
	buf1->out = g->out + "_1";
	buf1->in.push_back(EnIn[0][1]);  //In01
	GatePushBack(encoded, buf1, buf1->Type);
	encoded.Ntk_AllNode.push_back(buf1);
	
	if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
		AddPo(encoded, buf1->out);
		AddPo(encoded, buf0->out);
	}
}
// ===========================================================================
void EncodeAND(Jt_Ntk &encoded, Jt_Node *g){
		
	if(g->in.size() > 2){
		int i = 1;
		string s;
		while(g->in.size() >= 2){
			Jt_Node *new_gate = NULL;
			new_gate = new Jt_Node;	
			s = to_string(i++);
			new_gate->Type = node_AND;
			//new_gate.gate_name = g.gate_name + "_" + s;
			if(g->in.size() == 2){
				new_gate->out = g->out;
				new_gate->fo = g->fo;
			}
			else{
				new_gate->out = g->out + "_w" + s;
				//new_gate->fo.push_back() 
			}
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			g->in.push_back(new_gate->out);
			EncodeAND(encoded, new_gate);
		}
	} 
	else{
		Jt_Node *and1 = NULL, *and0 = NULL;
		and1 = new Jt_Node;
		and0 = new Jt_Node;
		vector<vector<string> > EnIn;
		for (int i = 0; i < g->in.size(); i++){
			vector<string> t;
			EncodeInput(t, g->in[i]);
			EnIn.push_back(t);
		}
		
		and1->Type = node_AND;
		and1->Name = g->Name;// +"_1";
		and1->out = g->out + "_1";
		and1->in.push_back(EnIn[0][1]);   //In01
		and1->in.push_back(EnIn[1][1]);   //In11
		GatePushBack(encoded, and1, and1->Type);
		encoded.Ntk_AllNode.push_back(and1);
		
		and0->Type = node_OR;
		and0->Name = g->Name;// +"_0";
		and0->out = g->out + "_0";
		and0->in.push_back(EnIn[0][0]);  //In00
		and0->in.push_back(EnIn[1][0]);  //In10
		GatePushBack(encoded, and0, and0->Type);
		encoded.Ntk_AllNode.push_back(and0);

		if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
			AddPo(encoded, and1->out);
			AddPo(encoded, and0->out);
		}
	}
}
// ===========================================================================
void EncodeOR(Jt_Ntk &encoded, Jt_Node *g){
	
	if(g->in.size() > 2){
		int i = 1;
		string s;
		while(g->in.size() >= 2){
			Jt_Node *new_gate = NULL;
			new_gate = new Jt_Node;
			s = to_string(i++);
			new_gate->Type = node_OR;
			//new_gate.gate_name = g.gate_name + "_" + s;
			if(g->in.size() == 2){
				new_gate->out = g->out;
				new_gate->fo = g->fo;
			}
			else{
				new_gate->out = g->out + "_w" + s;
				//new_gate->fo.push_back() 
			}
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			g->in.push_back(new_gate->out);
			EncodeOR(encoded, new_gate);
		}
	} 
	else{
		Jt_Node *or1 = NULL, *or0 = NULL;
		or1 = new Jt_Node;
		or0 = new Jt_Node;
		vector<vector<string> > EnIn;
		for (int i = 0; i < g->in.size(); i++){
			vector<string> t;
			EncodeInput(t, g->in[i]);
			EnIn.push_back(t);
		}
		or1->Type = node_OR;
		or1->Name = g->Name;// +"_1";
		or1->out = g->out + "_1";
		or1->in.push_back(EnIn[0][1]);   //In01
		or1->in.push_back(EnIn[1][1]);   //In11
		GatePushBack(encoded, or1, or1->Type);
		encoded.Ntk_AllNode.push_back(or1);
		
		or0->Type = node_AND;
		or0->Name = g->Name;// +"_0";
		or0->out = g->out + "_0";
		or0->in.push_back(EnIn[0][0]);  //In00
		or0->in.push_back(EnIn[1][0]);  //In10
		GatePushBack(encoded, or0, or0->Type);
		encoded.Ntk_AllNode.push_back(or0);

		if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
			AddPo(encoded, or1->out);
			AddPo(encoded, or0->out);
		}
	}
}
// ===========================================================================
void EncodeNAND(Jt_Ntk &encoded, Jt_Node *g){
	if(g->in.size() > 2){
		int i = 1;
		Jt_Node *end = NULL;
		end = new Jt_Node;
		string s;
		while(g->in.size() > 2){
			Jt_Node *new_gate = NULL;
			new_gate = new Jt_Node;
			s = to_string(i++);
			new_gate->Type = node_AND;
			//new_gate.gate_name = g.gate_name + "_" + s;
			new_gate->out = g->out + "_w" + s;
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			g->in.push_back(new_gate->out);
			EncodeAND(encoded, new_gate);
		}
		end->Type = node_NAND;
		end->out = g->out;
		end->fo = g->fo;
		end->in.push_back(g->in.back());
		g->in.pop_back();
		end->in.push_back(g->in.back());
		g->in.pop_back();
		EncodeNAND(encoded, end);
	} 
	else{
		Jt_Node *nand1 = NULL, *nand0 = NULL;
		nand1 = new Jt_Node;
		nand0 = new Jt_Node;
		vector<vector<string> > EnIn;
		for (int i = 0; i < g->in.size(); i++){
			vector<string> t;
			EncodeInput(t, g->in[i]);
			EnIn.push_back(t);
		}
		nand0->Type = node_AND;
		nand0->Name = g->Name;// +"_0";
		nand0->out = g->out + "_0";
		nand0->in.push_back(EnIn[0][1]);   //In01
		nand0->in.push_back(EnIn[1][1]);   //In11
		GatePushBack(encoded, nand0, nand0->Type);
		encoded.Ntk_AllNode.push_back(nand0);
		
		nand1->Type = node_OR;
		nand1->Name = g->Name;// +"_1";
		nand1->out = g->out + "_1";
		nand1->in.push_back(EnIn[0][0]);  //In00
		nand1->in.push_back(EnIn[1][0]);  //In10
		GatePushBack(encoded, nand1, nand1->Type);
		encoded.Ntk_AllNode.push_back(nand1);

		if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
			AddPo(encoded, nand1->out);
			AddPo(encoded, nand0->out);
		}		
	}
}
// ===========================================================================
void EncodeNOR(Jt_Ntk &encoded, Jt_Node *g){
	if(g->in.size() > 2){
		int i = 1;
		Jt_Node *end = NULL;
		end = new Jt_Node;
		string s;
		while(g->in.size() > 2){
			Jt_Node *new_gate = NULL;
			new_gate = new Jt_Node;
			s = to_string(i++);
			new_gate->Type = node_OR;
			//new_gate.gate_name = g.gate_name + "_" + s;
			new_gate->out = g->out + "_w" + s;
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			g->in.push_back(new_gate->out);
			EncodeOR(encoded, new_gate);
		}
		end->Type = node_NOR;
		end->out = g->out;
		end->fo = g->fo;
		end->in.push_back(g->in.back());
		g->in.pop_back();
		end->in.push_back(g->in.back());
		g->in.pop_back();
		EncodeNOR(encoded, end);
	} 
	else{
		Jt_Node *nor1 = NULL, *nor0 = NULL;
		nor1 = new Jt_Node;
		nor0 = new Jt_Node;
		vector<vector<string> > EnIn;
		for (int i = 0; i < g->in.size(); i++){
			vector<string> t;
			EncodeInput(t, g->in[i]);
			EnIn.push_back(t);
		}
		nor0->Type = node_OR;
		nor0->Name = g->Name;// +"_0";
		nor0->out = g->out + "_0";
		nor0->in.push_back(EnIn[0][1]);   //In01
		nor0->in.push_back(EnIn[1][1]);   //In11
		GatePushBack(encoded, nor0, nor0->Type);
		encoded.Ntk_AllNode.push_back(nor0);
		
		nor1->Type = node_AND;
		nor1->Name = g->Name;// +"_1";
		nor1->out = g->out + "_1";
		nor1->in.push_back(EnIn[0][0]);  //In00
		nor1->in.push_back(EnIn[1][0]);  //In10
		GatePushBack(encoded, nor1, nor1->Type);
		encoded.Ntk_AllNode.push_back(nor1);

		if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
			AddPo(encoded, nor1->out);
			AddPo(encoded, nor0->out);
		}
	}
}
// ===========================================================================
void EncodeNOT(Jt_Ntk &encoded, Jt_Node *g){
	Jt_Node *buf0 = NULL, *buf1 = NULL;
	buf0 = new Jt_Node;
	buf1 = new Jt_Node;
	
	vector<vector <string> > EnIn;
	for (int i = 0; i < g->in.size(); i++){
		vector<string> t;
		EncodeInput(t, g->in[i]);
		EnIn.push_back(t);
	}
	buf1->Type = node_BUF;
	buf1->Name = g->Name;// + "_1";
	buf1->out = g->out + "_1";
	buf1->in.push_back(EnIn[0][0]);
	GatePushBack(encoded, buf1, buf1->Type);
	encoded.Ntk_AllNode.push_back(buf1);

	buf0->Type = node_BUF;
	buf0->Name = g->Name;// + "_0";
	buf0->out = g->out + "_0";
	buf0->in.push_back(EnIn[0][1]);
	GatePushBack(encoded, buf0, buf0->Type);
	encoded.Ntk_AllNode.push_back(buf0);
	
	if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
		AddPo(encoded, buf1->out);
		AddPo(encoded, buf0->out);
	}
}
// ===========================================================================
void EncodeXOR(Jt_Ntk &encoded, Jt_Node *g){
	if(g->in.size() > 2){
		int i = 1;
		string s;
		while(g->in.size() >= 2){
			Jt_Node *new_gate = NULL;
			new_gate = new Jt_Node;
			s = to_string(i++);
			new_gate->Type = node_XOR;
			//new_gate.gate_name = g.gate_name + "_" + s;
			if(g->in.size() == 2){
				new_gate->out = g->out;
				new_gate->fo = g->fo;
			}
			else{
				new_gate->out = g->out + "_w" + s;
				//new_gate->fo.push_back() 
			}
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			g->in.push_back(new_gate->out);
			EncodeXOR(encoded, new_gate);
		}
	} 
	else{
		Jt_Node *and00 = NULL, *and01 = NULL, *and10 = NULL, *and11 = NULL, *or0 = NULL, *or1 = NULL;
		and00 = new Jt_Node;
		and01 = new Jt_Node;
		and10 = new Jt_Node;
		and11 = new Jt_Node;
		or0 = new Jt_Node;
		or1 = new Jt_Node;
		
		vector<vector<string> > EnIn;
		for (int i = 0; i < g->in.size(); i++){
			vector<string> t;
			EncodeInput(t, g->in[i]);
			EnIn.push_back(t);
		}
		
		and00->Type = node_AND;
		and00->Name = g->Name;// +"_and00";
		and00->out = g->out + "_and00_o";
		and00->in.push_back(EnIn[0][0]);
		and00->in.push_back(EnIn[1][0]);
		GatePushBack(encoded, and00, and00->Type);
		encoded.Ntk_AllNode.push_back(and00);
		
		and01->Type = node_AND;
		and01->Name = g->Name;// +"_and01";
		and01->out = g->out + "_and01_o";
		and01->in.push_back(EnIn[0][1]);   //In01
		and01->in.push_back(EnIn[1][1]);   //In11
		GatePushBack(encoded, and01, and01->Type);
		encoded.Ntk_AllNode.push_back(and01);		

		or0->Type = node_OR;
		or0->Name = g->Name;// +"_or0";
		or0->out = g->out + "_0";
		or0->in.push_back(and00->out);
		or0->in.push_back(and01->out);
		GatePushBack(encoded, or0, or0->Type);
		encoded.Ntk_AllNode.push_back(or0);
		
		and10->Type = node_AND;
		and10->Name = g->Name;// +"_and10";
		and10->out = g->out + "_and10_o";
		and10->in.push_back(EnIn[0][0]);
		and10->in.push_back(EnIn[1][1]);
		GatePushBack(encoded, and10, and10->Type);
		encoded.Ntk_AllNode.push_back(and10);
		
		and11->Type = node_AND;
		and11->Name = g->Name;// +"_and11";
		and11->out = g->out + "_and11_o";
		and11->in.push_back(EnIn[0][1]);
		and11->in.push_back(EnIn[1][0]);
		GatePushBack(encoded, and11, and11->Type);
		encoded.Ntk_AllNode.push_back(and11);		

		or1->Type = node_OR;
		or1->Name = g->Name;// +"_or1";
		or1->out = g->out + "_1";
		or1->in.push_back(and10->out);
		or1->in.push_back(and11->out);
		GatePushBack(encoded, or1, or1->Type);
		encoded.Ntk_AllNode.push_back(or1);
		
		if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
			AddPo(encoded, or1->out);
			AddPo(encoded, or0->out);
		}
	}
}
// ===========================================================================
void EncodeXNOR(Jt_Ntk &encoded, Jt_Node *g){
	if(g->in.size() > 2){
		int i = 1;
		Jt_Node *end = NULL;
		end = new Jt_Node;
		string s;
		while(g->in.size() > 2){
			Jt_Node *new_gate = NULL;
			new_gate = new Jt_Node;
			s = to_string(i++);
			new_gate->Type = node_XOR;
			//new_gate->gate_name = g->gate_name + "_" + s;
			new_gate->out = g->out + "_w" + s;
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			new_gate->in.push_back(g->in.back());
			g->in.pop_back();
			g->in.push_back(new_gate->out);
			EncodeXOR(encoded, new_gate);
		}
		end->Type = node_XNOR;
		end->out = g->out;
		end->fo = g->fo;
		end->in.push_back(g->in.back());
		g->in.pop_back();
		end->in.push_back(g->in.back());
		g->in.pop_back();
		EncodeXNOR(encoded, end);
	}
	else{
		Jt_Node *and00 = NULL, *and01 = NULL, *and10 = NULL, *and11 = NULL, *or0 = NULL, *or1 = NULL;
		and00 = new Jt_Node;
		and01 = new Jt_Node;
		and10 = new Jt_Node;
		and11 = new Jt_Node;
		or0 = new Jt_Node;
		or1 = new Jt_Node;
		vector<vector<string> > EnIn;
		for (int i = 0; i < g->in.size(); i++){
			vector<string> t;
			EncodeInput(t, g->in[i]);
			EnIn.push_back(t);
		}
		
		and10->Type = node_AND;
		and10->Name = g->Name;// +"_and10";
		and10->out = g->out + "_and10_o";
		and10->in.push_back(EnIn[0][0]);
		and10->in.push_back(EnIn[1][0]);
		GatePushBack(encoded, and10, and10->Type);
		encoded.Ntk_AllNode.push_back(and10);		
		
		and11->Type = node_AND;
		and11->Name = g->Name;// +"_and11";
		and11->out = g->out + "_and11_o";
		and11->in.push_back(EnIn[0][1]);
		and11->in.push_back(EnIn[1][1]);
		GatePushBack(encoded, and11, and11->Type);
		encoded.Ntk_AllNode.push_back(and11);
		
		or1->Type = node_OR;
		or1->Name = g->Name;// +"_or1";
		or1->out = g->out + "_1";
		or1->in.push_back(and10->out);
		or1->in.push_back(and11->out);
		GatePushBack(encoded, or1, or1->Type);
		encoded.Ntk_AllNode.push_back(or1);

		and00->Type = node_AND;
		and00->Name = g->Name;// +"_and00";
		and00->out = g->out + "_and00_o";
		and00->in.push_back(EnIn[0][0]);
		and00->in.push_back(EnIn[1][1]);
		GatePushBack(encoded, and00, and00->Type);
		encoded.Ntk_AllNode.push_back(and00);
		
		and01->Type = node_AND;
		and01->Name = g->Name;// +"_and01";
		and01->out = g->out + "_and01_o";
		and01->in.push_back(EnIn[0][1]);   //In01
		and01->in.push_back(EnIn[1][0]);   //In11
		GatePushBack(encoded, and01, and01->Type);
		encoded.Ntk_AllNode.push_back(and01);		

		or0->Type = node_OR;
		or0->Name = g->Name;// +"_or0";
		or0->out = g->out + "_0";
		or0->in.push_back(and00->out);
		or0->in.push_back(and01->out);
		GatePushBack(encoded, or0, or0->Type);
		encoded.Ntk_AllNode.push_back(or0);

		if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
			AddPo(encoded, or1->out);
			AddPo(encoded, or0->out);
		}
	}
}
// ===========================================================================
void EncodeDC(Jt_Ntk &encoded, Jt_Node *g){
	Jt_Node *and0 = NULL, *and1 = NULL;
	and0 = new Jt_Node;
	and1 = new Jt_Node;
	vector<vector<string> > EnIn;
	for (int i = 0; i < g->in.size(); i++){
		vector<string> t;
		EncodeInput(t, g->in[i]);
		EnIn.push_back(t);
	}

	and1->Type = node_AND;
	and1->Name = g->Name;// + "_1";
	and1->out = g->out + "_1";
	and1->in.push_back(EnIn[0][1]);  //c1
	and1->in.push_back(EnIn[1][0]);  //d0
	GatePushBack(encoded, and1, and1->Type);
	encoded.Ntk_AllNode.push_back(and1);
	
	and0->Type = node_AND;
	and0->Name = g->Name;// + "_0";
	and0->out = g->out + "_0";
	and0->in.push_back(EnIn[0][0]);   //c0
	and0->in.push_back(EnIn[1][0]);   //d0
	GatePushBack(encoded, and0, and0->Type);
	encoded.Ntk_AllNode.push_back(and0);
	
	if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
		AddPo(encoded, and1->out);
		AddPo(encoded, and0->out);
	}
}
// ===========================================================================
void EncodeMUX(Jt_Ntk &encoded, Jt_Node *g){
	
	Jt_Node *and00 = NULL, *and01 = NULL, *and02 = NULL;
	Jt_Node *and10 = NULL, *and11 = NULL, *and12 = NULL;
	Jt_Node *or00 = NULL, *or01 = NULL, *or10 = NULL, *or11 = NULL;
	and00 = new Jt_Node;
	and01 = new Jt_Node;
	and02 = new Jt_Node;
	and10 = new Jt_Node;
	and11 = new Jt_Node;
	and12 = new Jt_Node;
	or00 = new Jt_Node;
	or01 = new Jt_Node;
	or10 = new Jt_Node;
	or11 = new Jt_Node;
	vector<vector<string> > EnIn;
	for (int i = 0; i < g->in.size(); i++){
		vector<string> t;
		EncodeInput(t, g->in[i]);
		EnIn.push_back(t);
	}

	and00->Type = node_AND;
	and00->Name = g->Name;// + "_and00";
	and00->out = g->out + "_and00_o";
	and00->in.push_back(EnIn[2][1]);	//S1
	and00->in.push_back(EnIn[1][0]);	//In10
	GatePushBack(encoded, and00, and00->Type);
	encoded.Ntk_AllNode.push_back(and00);
	
	and01->Type = node_AND;
	and01->Name = g->Name;// + "_and01";
	and01->out = g->out + "_and01_o";
	and01->in.push_back(EnIn[1][0]);	//In10
	and01->in.push_back(EnIn[0][0]);	//In00
	GatePushBack(encoded, and01, and01->Type);
	encoded.Ntk_AllNode.push_back(and01);

	and02->Type = node_AND;
	and02->Name = g->Name;// + "_and02";
	and02->out = g->out + "_and02_o";
	and02->in.push_back(EnIn[2][0]);	//S0
	and02->in.push_back(EnIn[0][0]);	//In00
	GatePushBack(encoded, and02, and02->Type);
	encoded.Ntk_AllNode.push_back(and02);
	
	or00->Type = node_OR;
	or00->Name = g->Name;// + "_or00";
	or00->out = g->out + "_or00_o";
	or00->in.push_back(and00->out);
	or00->in.push_back(and01->out);
	GatePushBack(encoded, or00, or00->Type);
	encoded.Ntk_AllNode.push_back(or00);
	
	or01->Type = node_OR;
	or01->Name = g->Name;// + "_or01";
	or01->out = g->out + "_0";
	or01->in.push_back(or00->out);
	or01->in.push_back(and02->out);
	GatePushBack(encoded, or01, or01->Type);
	encoded.Ntk_AllNode.push_back(or01);
	
	and10->Type = node_AND;
	and10->Name = g->Name;// + "_and10";
	and10->out = g->out + "_and10_o";
	and10->in.push_back(EnIn[2][1]);	//S1
	and10->in.push_back(EnIn[1][1]);	//In11
	GatePushBack(encoded, and10, and10->Type);
	encoded.Ntk_AllNode.push_back(and10);
	
	and11->Type = node_AND;
	and11->Name = g->Name;// + "_and11";
	and11->out = g->out + "_and11_o";
	and11->in.push_back(EnIn[2][0]);	//S0
	and11->in.push_back(EnIn[0][1]);	//In01
	GatePushBack(encoded, and11, and11->Type);
	encoded.Ntk_AllNode.push_back(and11);
	
	and12->Type = node_AND;
	and12->Name = g->Name;// + "_and12";
	and12->out = g->out + "_and12_o";
	and12->in.push_back(EnIn[1][1]);	//In11
	and12->in.push_back(EnIn[0][1]);	//In01
	GatePushBack(encoded, and12, and12->Type);
	encoded.Ntk_AllNode.push_back(and12);
	
	or10->Type = node_OR;
	or10->Name = g->Name;// + "_or10";
	or10->out = g->out + "_or10_o";
	or10->in.push_back(and10->out);
	or10->in.push_back(and11->out);
	GatePushBack(encoded, or10, or10->Type);
	encoded.Ntk_AllNode.push_back(or10);
	
	or11->Type = node_OR;
	or11->Name = g->Name;// + "_or11";
	or11->out = g->out + "_1";
	or11->in.push_back(and12->out);
	or11->in.push_back(or10->out);
	GatePushBack(encoded, or11, or11->Type);
	encoded.Ntk_AllNode.push_back(or11);
	
	if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
		AddPo(encoded, or11->out);
		AddPo(encoded, or01->out);
	}
}
// ===========================================================================
void TransferMUX(Jt_Ntk &encoded, Jt_Node *g){
	Jt_Node *or0 = NULL, *not0 = NULL, *and0 = NULL, *and1 = NULL;
	or0 = new Jt_Node;
	not0 = new Jt_Node;
	and0 = new Jt_Node;
	and1 = new Jt_Node;
	
	not0->Type = node_NOT;		// ~S
	not0->Name = g->Name;// + "_mux_n";
	not0->out = g->out + "_not_s";
	not0->in.push_back(g->in[2]);	//S
	GatePushBack(encoded, not0, not0->Type);
	encoded.Ntk_AllNode.push_back(not0);
	
	and1->Type = node_AND;			//(~S&I0)
	and1->Name = g->Name;// + "_and_1";
	and1->out = g->out + "_and_1";
	and1->in.push_back(g->in[0]);	//I0
	and1->in.push_back(not0->out);	//~S
	GatePushBack(encoded, and1, and1->Type);
	encoded.Ntk_AllNode.push_back(and1);
	
	and0->Type = node_AND;			//(S&I1)
	and0->Name = g->Name;// + "_and_0";
	and0->out = g->out + "_and_0";
	and0->in.push_back(g->in[1]);	//I1
	and0->in.push_back(g->in[2]);	//S
	GatePushBack(encoded, and0, and0->Type);
	encoded.Ntk_AllNode.push_back(and0);
	
	or0->Type = node_OR;			//(~S&I0)+(S&I1)
	or0->Name = g->Name;// + "_or_0";
	or0->out = g->out;
	or0->in.push_back(and0->out);	//(S&I1)
	or0->in.push_back(and1->out);	//(~S&I0)
	GatePushBack(encoded, or0, or0->Type);
	encoded.Ntk_AllNode.push_back(or0);
	
	if(!g->fo.empty()&&Jt_NodeType(g->fo[0]) == node_PO){
		AddPo(encoded, or0->out);
	}
}
// ===========================================================================
void AddEncoder(Jt_Ntk &encoded, string g){
	
	string encoder_out1, encoder_out0;
	Jt_Node *encode_not = NULL, *encode_buf = NULL;
	encode_not = new Jt_Node;
	encode_buf = new Jt_Node;
	
	encode_not->Type = node_NOT;
	//encode_not->Name = g;// + "_en";
	encode_not->out = g + "_0";
	encode_not->in.push_back(g);
	GatePushBack(encoded, encode_not, encode_not->Type);
	encoded.Ntk_AllNode.push_back(encode_not);
	
	encode_buf->Type = node_BUF;
	//encode_buf->Name = g;// + "_buf";
	encode_buf->out = g + "_1";
	encode_buf->in.push_back(g);
	GatePushBack(encoded, encode_buf, encode_buf->Type);
	encoded.Ntk_AllNode.push_back(encode_buf);
}
// ===========================================================================
void EncodeCircuit(Jt_Ntk &encoded, Jt_Ntk &target){

	vector<Jt_Node *> target_order;
	
	Jt_Node *n, *pFi, *pFo;
	int i, j;
	Jt_NtkForEachNode(target, n, i)		n->visit = 0;
	Ntk_Dfs(target, target_order);
	
	InitialNtk(encoded);
	
	
	Jt_NtkForEachPi( target, n, i ){
		string NodeName = Jt_NodeName(n);
		AddPi(encoded, NodeName);
		Jt_NodeForEachFo( n, pFo, j ){
			if(pFo->XGate){
				Jt_Node *pi_en = NULL;
				pi_en = new Jt_Node;
				pi_en->out = "5566";//Jt_NodeName(n);
				pi_en->Name = Jt_NodeName(n);
				AddEncoder(encoded, string(Jt_NodeName(n)));//n->out);
				break;
			}
		}
	}
	
	Jt_VecForEachEntry(target_order, n, i){
		if(n->XGate){
			if(Jt_NodeType(n) == node_BUF){
				EncodeBUF(encoded, n);
			}
			else if(Jt_NodeType(n) == node_AND){
				EncodeAND(encoded, n);
			}
			else if(Jt_NodeType(n) == node_OR){
				EncodeOR(encoded, n);
			}
			else if(Jt_NodeType(n) == node_NAND){
				EncodeNAND(encoded, n);
			}
			else if(Jt_NodeType(n) == node_NOR){
				EncodeNOR(encoded, n);
			}
			else if(Jt_NodeType(n) == node_NOT){
				EncodeNOT(encoded, n);
			}
			else if(Jt_NodeType(n) == node_XOR){
				EncodeXOR(encoded, n);
			}
			else if(Jt_NodeType(n) == node_XNOR){
				EncodeXNOR(encoded, n);				
			}
			else if(Jt_NodeType(n) == node_DCGATE){
				EncodeDC(encoded, n);
			}
			else if(Jt_NodeType(n) == node_MUX){
				EncodeMUX(encoded, n);
			}
		}
		else if(Jt_NodeType(n) == node_MUX){
			TransferMUX(encoded, n);
			Jt_NodeForEachFo( n, pFo, j){
				if(pFo->XGate){
					AddEncoder(encoded, n->out);
					break;
				}
			}
		}
		else if(Jt_NodeType(n)!=node_PI && Jt_NodeType(n)!=node_PO && Jt_NodeType(n)!=node_PI && Jt_NodeType(n)!=node_Const0 && Jt_NodeType(n)!=node_Const1 && Jt_NodeType(n)!=node_Constx){
			encoded.Ntk_AllNode.push_back(n);
			if(!n->fo.empty()&&Jt_NodeType(n->fo[0]) == node_PO){
				AddPo(encoded, n->out);
			}
			Jt_NodeForEachFo( n, pFo, j){
				if(pFo->XGate){
					AddEncoder(encoded, n->out);
					break;
				}	
				
			}
		}
	}
	
	/*for(i = 0; i < target.InList.size(); i++){
		encoded.InList.push_back(target.InList[i]);
		en_Input.out = target.InList[i];
		en_Input.gate_name = target.InList[i];
		AddEncoder(encoded, en_Input);
	}*/
	
/*	for(i = 0; i < target.first_xgate ; i++){
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
*/
}

/*void AddEncoder(Circuit &encoded, Gate &g){
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
}	*/
