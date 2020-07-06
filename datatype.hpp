#ifndef _DATATYPE_HPP_
#define _DATATYPE_HPP_

#include <string> 
#include <vector>
#include <deque>
using namespace std;

enum GateType {
	AND_GATE,
	OR_GATE,
	NAND_GATE,
	NOR_GATE,
	NOT_GATE,
	BUF_GATE,
	XOR_GATE,
	XNOR_GATE,
	DC_GATE,
	MUX_GATE
};

struct Gate
{
	GateType gate_type;
	string gate_name;
	string out;
	vector<string> in;
	bool XGate;
	vector<int> fout;
	vector<int> fin;
};

struct Circuit
{
	vector<string> InList;
	vector<string> OutList;
	vector<string> WireList;
	vector<Gate> GateList;
	int first_xgate;
	int num_of_xgate;
	int *top_order;
	vector<bool> visit;
	int top_index;
};

void ReadCircuit(char * file_name, Circuit &pNtk);
void CreateGate(Circuit &pNtk, vector<string> &gate_info);
void BuildAdjList(Circuit &pNtk);
void topsort_Call(Circuit &pNtk);
void top_sort(Circuit &pNtk, int v);
void PrintCircuit(Circuit &pNtk);

void EncodeCircuit(Circuit &encoded, Circuit &target);
void AddEncoder(Circuit &encoded, Gate &g);
void EncodeInput(vector<string> &EnIn, string &in);
void EncodeAND(Circuit &encoded, Gate &g, Circuit &target);
void EncodeOR(Circuit &encoded, Gate &g, Circuit &target);
void EncodeNAND(Circuit &encoded, Gate &g, Circuit &target);
void EncodeNOR(Circuit &encoded, Gate &g, Circuit &target);
void EncodeNOT(Circuit &encoded, Gate &g, Circuit &target);
void EncodeBUF(Circuit &encoded, Gate &g, Circuit &target);
void EncodeXOR(Circuit &encoded, Gate &g, Circuit &target);
void EncodeXNOR(Circuit &encoded, Gate &g, Circuit &target);
void EncodeDC(Circuit &encoded, Gate &g, Circuit &target);
void EncodeMUX(Circuit &encoded, Gate &g, Circuit &target);
void TransferMUX(Circuit &encoded, Gate &g, Circuit &target);

void BuildMiter(Circuit &miter, Circuit &golden, Circuit &revised);
void RenameWire(Circuit &miter, Circuit &golden, Circuit &revised);
void RenamePI(Circuit &miter, Circuit &golden, Circuit &revised);
void FindOutput(Circuit &miter, Circuit &golden, Circuit &revised, Gate &PO);
void AddMiter1(string g1, string g0, string r1, string r0, Circuit &miter, Gate &PO);
void AddMiter2(string g1, string g0, string r, Circuit &miter, Gate &PO);
void AddMiter3(string g, string r1, string r0, Circuit &miter, Gate &PO);
void AddMiter4(string g, string r, Circuit &miter, Gate &PO);
#endif
