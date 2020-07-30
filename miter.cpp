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

void RenamePI(Jt_Ntk &miter, Jt_Ntk &golden, Jt_Ntk &revised){
	int i;
	Jt_Node *n;
	
	Jt_NtkForEachPi( golden, n, i ){
		Jt_Node *buf_g = NULL, *buf_r = NULL;
		buf_g = new Jt_Node;
		buf_r = new Jt_Node;
		buf_g->Type = node_BUF;
		buf_g->in.push_back(string(Jt_NodeName(n)));
		buf_g->out = string(Jt_NodeName(n)) + "_g";
		GatePushBack(miter, buf_g, buf_g->Type);
		miter.Ntk_AllNode.push_back(buf_g);
		buf_r->Type = node_BUF;
		buf_r->in.push_back(string(Jt_NodeName(n)));
		buf_r->out = string(Jt_NodeName(n)) + "_r";
		GatePushBack(miter, buf_r, buf_r->Type);
		miter.Ntk_AllNode.push_back(buf_r);
	}
}

void RenameWire(Jt_Ntk &miter, Jt_Ntk &golden, Jt_Ntk &revised){
	int i, j;
	Jt_Node *n;
	
	Jt_NtkForEachPi( golden, n, i ){
		string NodeName = Jt_NodeName(n);
		AddPi(miter, NodeName);
	}
	RenamePI(miter, golden ,revised);
	
	Jt_NtkForEachNode(golden, n, i){
		if(Jt_NodeType(n)!=node_PI && Jt_NodeType(n)!=node_PO && Jt_NodeType(n)!=node_PI && Jt_NodeType(n)!=node_Const0 && Jt_NodeType(n)!=node_Const1 && Jt_NodeType(n)!=node_Constx){
			Jt_Node *g = NULL;
			g = new Jt_Node;
			g->out = n->out +"_g";
			g->Type = Jt_NodeType(n);
			for(int j = 0; j < n->in.size(); j++){
				if(n->in[j] != "1'b0" && n->in[j] != "1'b1")
					g->in.push_back(n->in[j] + "_g");
				else
					g->in.push_back(n->in[j]);
			}
			GatePushBack(miter, g, g->Type);
			miter.Ntk_AllNode.push_back(g);
		}
	}
	
	Jt_NtkForEachNode(revised, n, i){
		if(Jt_NodeType(n)!=node_PI && Jt_NodeType(n)!=node_PO && Jt_NodeType(n)!=node_PI && Jt_NodeType(n)!=node_Const0 && Jt_NodeType(n)!=node_Const1 && Jt_NodeType(n)!=node_Constx){
			Jt_Node *g = NULL;
			g = new Jt_Node;
			g->out = n->out +"_r";
			g->Type = Jt_NodeType(n);
			for(int j = 0; j < n->in.size(); j++){
				if(n->in[j] != "1'b0" && n->in[j] != "1'b1")
					g->in.push_back(n->in[j] + "_r");
				else
					g->in.push_back(n->in[j]);
			}
			GatePushBack(miter, g, g->Type);
			miter.Ntk_AllNode.push_back(g);
		}
	}

}

void AddMiter1(string g1, string g0, string r1, string r0, Jt_Ntk &miter, Jt_Node *PO){
	string s;
	Jt_Node *and1 = NULL, *and0 = NULL, *or0 = NULL, *not1 = NULL, *not0 = NULL;
	and1 = new Jt_Node;
	and0 = new Jt_Node;
	or0 = new Jt_Node;
	not1 = new Jt_Node;
	not0 = new Jt_Node;
	
	not1->Type = node_NOT;
	not1->out = r1 + "_n";
	not1->in.push_back(r1 + "_r");
	GatePushBack(miter, not1, not1->Type);
	miter.Ntk_AllNode.push_back(not1);

	and1->Type = node_AND;
	and1->out = g1 + "_and";
	and1->in.push_back(g1 + "_g");
	and1->in.push_back(not1->out);
	GatePushBack(miter, and1, and1->Type);
	miter.Ntk_AllNode.push_back(and1);
	
	not0->Type = node_NOT;
	not0->out = r0 + "_n";
	not0->in.push_back(r0 + "_r");
	GatePushBack(miter, not0, not0->Type);
	miter.Ntk_AllNode.push_back(not0);

	and0->Type = node_AND;
	and0->out = g0 + "_and";
	and0->in.push_back(g0 + "_g");
	and0->in.push_back(not0->out);
	GatePushBack(miter, and0, and0->Type);
	miter.Ntk_AllNode.push_back(and0);
	
	s = to_string(num_of_miter++);
	
	or0->Type = node_OR;
	or0->out = "Miter_" + s;
	or0->in.push_back(and0->out);
	or0->in.push_back(and1->out);
	GatePushBack(miter, or0, or0->Type);
	miter.Ntk_AllNode.push_back(or0);
	PO->in.push_back(or0->out);
}

void AddMiter2(string g1, string g0, string r, Jt_Ntk &miter, Jt_Node *PO){
	string s;
	Jt_Node *and1 = NULL, *and0 = NULL, *or0 = NULL, *not0 = NULL;
	and1 = new Jt_Node;
	and0 = new Jt_Node;
	or0 = new Jt_Node;
	not0 = new Jt_Node;
	
	not0->Type = node_NOT;
	not0->out = r + "_n";
	not0->in.push_back(r + "_r");
	GatePushBack(miter, not0, not0->Type);
	miter.Ntk_AllNode.push_back(not0);

	and1->Type = node_AND;
	and1->out = g1 + "_and";
	and1->in.push_back(g1 + "_g");
	and1->in.push_back(not0->out);
	GatePushBack(miter, and1, and1->Type);
	miter.Ntk_AllNode.push_back(and1);

	and0->Type = node_AND;
	and0->out = g0 + "_and";
	and0->in.push_back(g0 + "_g");
	and0->in.push_back(r + "_r");
	GatePushBack(miter, and0, and0->Type);
	miter.Ntk_AllNode.push_back(and0);
	
	s = to_string(num_of_miter++);
	
	or0->Type = node_OR;
	or0->out = "Miter_" + s;
	or0->in.push_back(and0->out);
	or0->in.push_back(and1->out);
	GatePushBack(miter, or0, or0->Type);
	miter.Ntk_AllNode.push_back(or0);
	
	PO->in.push_back(or0->out);
}

void AddMiter3(string g, string r1, string r0, Jt_Ntk &miter, Jt_Node *PO){
	string s;
	Jt_Node *and1 = NULL, *and0 = NULL;
	Jt_Node *or0 = NULL, *not2 = NULL, *not1 = NULL, *not0 = NULL;
	and1 = new Jt_Node;
	and0 = new Jt_Node;
	or0 = new Jt_Node;
	not2 = new Jt_Node;
	not1 = new Jt_Node;
	not0 = new Jt_Node;
	
	not2->Type = node_NOT;
	not2->out = r1 + "_n";
	not2->in.push_back(r1 + "_r");
	GatePushBack(miter, not2, not2->Type);
	miter.Ntk_AllNode.push_back(not2);

	not1->Type = node_NOT;
	not1->out = r0 + "_n";
	not1->in.push_back(r0 + "_r");
	GatePushBack(miter, not1, not1->Type);
	miter.Ntk_AllNode.push_back(not1);
	
	not0->Type = node_NOT;
	not0->out = g + "_n";
	not0->in.push_back(g + "_g");
	GatePushBack(miter, not0, not0->Type);
	miter.Ntk_AllNode.push_back(not0);

	and1->Type = node_AND;
	and1->out = r0 + "_and";
	and1->in.push_back(not1->out);
	and1->in.push_back(not0->out);
	GatePushBack(miter, and1, and1->Type);
	miter.Ntk_AllNode.push_back(and1);

	and0->Type = node_AND;
	and0->out = r1 + "_and";
	and0->in.push_back(g + "_g");
	and0->in.push_back(not2->out);
	GatePushBack(miter, and0, and0->Type);
	miter.Ntk_AllNode.push_back(and0);
	
	s = to_string(num_of_miter++);
	
	or0->Type = node_OR;
	or0->out = "Miter_" + s;
	or0->in.push_back(and0->out);
	or0->in.push_back(and1->out);
	GatePushBack(miter, or0, or0->Type);
	miter.Ntk_AllNode.push_back(or0);
	PO->in.push_back(or0->out);
}

void AddMiter4(string g, string r, Jt_Ntk &miter, Jt_Node *PO){
	string s;
	Jt_Node *xor0 = NULL;
	xor0 = new Jt_Node;
	
	s = to_string(num_of_miter++);
	xor0->Type = node_XOR;
	xor0->out = "Miter_" + s;
	xor0->in.push_back(r + "_r");
	xor0->in.push_back(g + "_g");
	GatePushBack(miter, xor0, xor0->Type);
	miter.Ntk_AllNode.push_back(xor0);
	PO->in.push_back(xor0->out);
}

void FindOutput(Jt_Ntk &miter, Jt_Ntk &golden, Jt_Ntk &revised, Jt_Node *PO){
	int i, j;
	Jt_Node *g_po, *r_po;
	string str_g, str_r;
	
	//Jt_NtkForEachPo( golden, g_po, i ){
	for ( i = 0; (i < Jt_NtkPONum(golden) && (g_po = golden.Ntk_PO[i])); i++){
		Jt_NtkForEachPo( revised, r_po, j ){
			str_g = string(Jt_NodeName(g_po));
			str_r = string(Jt_NodeName(r_po));
			if(str_g.compare(str_r) == 0){
				if(str_g[str_g.size()-2] == '_' && str_g[str_g.size()-1] == '1'){
					AddMiter1(str_g, string(Jt_NodeName(golden.Ntk_PO[++i])), str_r, string(Jt_NodeName(revised.Ntk_PO[j+1])), miter, PO);
				}
				else
					AddMiter4(str_g, str_r, miter, PO);
				break;
			} else if(str_g[str_g.size()-2] == '_' && str_g[str_g.size()-1] == '1'){
				if(str_g.substr(0, str_g.size()-2).compare(str_r) == 0){
					AddMiter2(str_g, string(Jt_NodeName(golden.Ntk_PO[++i])), str_r, miter, PO);
					break;
				}	
			} else if(str_g.compare(0, str_g.size(), str_r.substr(0, str_g.size())) == 0){
				AddMiter3(str_g, str_r, string(Jt_NodeName(revised.Ntk_PO[j+1])), miter, PO);
				break;
			}
		}
	}
}

void BuildMiter(Jt_Ntk &miter, Jt_Ntk &golden, Jt_Ntk &revised){
	
	Jt_Node *PO = NULL;
	PO = new Jt_Node;
	num_of_miter = 0;
	
	PO->Type = node_OR;
	PO->out = "PO";
	RenameWire(miter, golden ,revised);
	FindOutput(miter, golden, revised, PO);

	GatePushBack(miter, PO, PO->Type);
	miter.Ntk_AllNode.push_back(PO);
	AddPo(miter, PO->out);
	
	return;
}
