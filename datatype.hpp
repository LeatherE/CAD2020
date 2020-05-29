#ifndef _DATATYPE_HPP_
#define _DATATYPE_HPP_

#include <string> 
#include <vector>
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
	//struct Gate *fout;
	vector<int> fout;
};

#endif
