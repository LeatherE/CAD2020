#ifndef _DATATYPE_HPP_
#define _DATATYPE_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
using namespace std;

typedef enum {
	_0_,
	_1_,
	_x_,
	_n_				// undefined
} val_t;

// node types
typedef enum {
	node_Const0,	// 0
	node_Const1,	// 1
	node_Constx,	// 2
	node_PI,		// 3
	node_PO,		// 4
	node_AND,		// 5
	node_OR,		// 6
	node_NAND,		// 7
	node_NOR,		// 8
	node_NOT,		// 9
	node_BUF,		// 10
	node_XOR,		// 11
	node_XNOR,		// 12
	node_DCGATE,	// 13
	node_MUX,		// 14
	node_undefined	// 15
} NodeType_t;

struct cmp_str
{
    bool operator()(char const *a, char const *b)
    {
        return strcmp(a, b) < 0;
    }
};

struct Jt_Node
{
	char *				Name;
	NodeType_t			Type;
	val_t				Value;
	vector<Jt_Node *> 	fi;
	vector<Jt_Node *> 	fo;
	bool				visit;
	string 				out;
	vector<string> 		in;
	bool 				XGate;
};

struct Jt_Ntk
{
	char *								Name;
	vector<Jt_Node *> 					Ntk_PI;
	vector<Jt_Node *> 					Ntk_PO;
	vector<Jt_Node *> 					Ntk_AND;
	vector<Jt_Node *> 					Ntk_OR;
	vector<Jt_Node *> 					Ntk_NAND;
	vector<Jt_Node *> 					Ntk_NOR;
	vector<Jt_Node *> 					Ntk_NOT;
	vector<Jt_Node *> 					Ntk_BUF;
	vector<Jt_Node *> 					Ntk_XOR;
	vector<Jt_Node *> 					Ntk_XNOR;
	vector<Jt_Node *> 					Ntk_DCGATE;
	vector<Jt_Node *> 					Ntk_MUX;
	vector<Jt_Node *> 					Ntk_AllNode;
	vector<Jt_Node *>					Ntk_Const0;
	vector<Jt_Node *>					Ntk_Const1;
	vector<Jt_Node *>					Ntk_Constx;
	vector<Jt_Node *> 					Ntk_Order;
	int 								num_of_xgate;
	map<char *, Jt_Node *, cmp_str>		node_map;
};

//void ReadCircuit(char * file_name, Jt_Ntk &pNtk);
//void CreateGate(Circuit &pNtk, vector<string> &gate_info);
bool ReadCircuit(char * file_name, Jt_Ntk &pNtk);
void EncodeCircuit(Jt_Ntk &encoded, Jt_Ntk &target);
void BuildMiter(Jt_Ntk &miter, Jt_Ntk &golden, Jt_Ntk &revised);
//void BuildAdjList(Circuit &pNtk);
//void topsort_Call(Circuit &pNtk);
//void top_sort(Circuit &pNtk, int v);
void PrintCircuit(Jt_Ntk &pNtk);

/*void EncodeCircuit(Circuit &encoded, Circuit &target);
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
void AddMiter4(string g, string r, Circuit &miter, Gate &PO);*/

//	Function
//bool ReadCircuit(char *, Jt_Ntk &);
void print_Ntk_info(Jt_Ntk &);
void x_Simulate(char *, Jt_Ntk &, Jt_Ntk &);

void AddPi(Jt_Ntk &pNtk, string &Pi_Name);
void AddPo(Jt_Ntk &pNtk, string &Po_Name);
void GatePushBack(Jt_Ntk &pNtk, Jt_Node *n, NodeType_t &gType);
void InitialNtk(Jt_Ntk &pNtk);

static inline int         	Jt_NtkNodeNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_AllNode.size();	}
static inline int         	Jt_NtkPINum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_PI.size();		}
static inline int         	Jt_NtkPONum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_PO.size();		}
static inline int         	Jt_NtkANDNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_AND.size();		}
static inline int         	Jt_NtkORNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_OR.size();		}
static inline int         	Jt_NtkNANDNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_NAND.size();		}
static inline int         	Jt_NtkNORNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_NOR.size();		}
static inline int         	Jt_NtkNOTNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_NOT.size();		}
static inline int         	Jt_NtkBUFNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_BUF.size();		}
static inline int         	Jt_NtkXORNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_XOR.size();		}
static inline int         	Jt_NtkXNORNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_XNOR.size();		}
static inline int         	Jt_NtkDCGATENum( Jt_Ntk &pNtk )          		{ return pNtk.Ntk_DCGATE.size();	}
static inline int         	Jt_NtkMUXNum( Jt_Ntk &pNtk )          			{ return pNtk.Ntk_MUX.size();		}
static inline int         	Jt_NtkConst0Num( Jt_Ntk &pNtk )          		{ return pNtk.Ntk_Const0.size();	}
static inline int         	Jt_NtkConst1Num( Jt_Ntk &pNtk )          		{ return pNtk.Ntk_Const1.size();	}
static inline int         	Jt_NtkConstxNum( Jt_Ntk &pNtk )          		{ return pNtk.Ntk_Constx.size();	}

static inline int         	Jt_NtkNodeNumInMap( Jt_Ntk &pNtk )        	  	{ return pNtk.node_map.size();		}
static inline char *      	Jt_NtkName( Jt_Ntk &pNtk )          			{ return pNtk.Name;					}

static inline char *      	Jt_NodeName( Jt_Node *pNode )          			{ return pNode->Name;				}
static inline NodeType_t    Jt_NodeType( Jt_Node *pNode )          			{ return pNode->Type;				}
static inline val_t      	Jt_NodeValue( Jt_Node *pNode )          		{ return pNode->Value;				}
static inline bool      	Jt_NodeVisit( Jt_Node *pNode )          		{ return pNode->visit;				}
static inline int      		Jt_NodeFiNum( Jt_Node *pNode )          		{ return pNode->fi.size();			}
static inline int      		Jt_NodeFoNum( Jt_Node *pNode )         			{ return pNode->fo.size();			}

namespace std {
    template<typename T>
    std::string to_string(const T &n) {
        std::ostringstream s;
        s << n;
        return s.str();
    }
}


// Iterator
#define Jt_NtkForEachPi( pNtk, pPi, i )											\
	for ( int i = 0; (i < Jt_NtkPINum(pNtk) && (pPi = pNtk.Ntk_PI[i])); i++)
#define Jt_NtkForEachPo( pNtk, pPo, i )											\
	for ( int i = 0; (i < Jt_NtkPONum(pNtk) && (pPo = pNtk.Ntk_PO[i])); i++)
#define Jt_NtkForEachAND( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkANDNum(pNtk) && (pNode = pNtk.Ntk_AND[i])); i++)
#define Jt_NtkForEachOR( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkORNum(pNtk) && (pNode = pNtk.Ntk_OR[i])); i++)
#define Jt_NtkForEachNAND( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkNANDNum(pNtk) && (pNode = pNtk.Ntk_NAND[i])); i++)
#define Jt_NtkForEachNOR( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkNORNum(pNtk) && (pNode = pNtk.Ntk_NOR[i])); i++)
#define Jt_NtkForEachNOT( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkNOTNum(pNtk) && (pNode = pNtk.Ntk_NOT[i])); i++)
#define Jt_NtkForEachBUF( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkBUFNum(pNtk) && (pNode = pNtk.Ntk_BUF[i])); i++)
#define Jt_NtkForEachXOR( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkXORNum(pNtk) && (pNode = pNtk.Ntk_XOR[i])); i++)
#define Jt_NtkForEachXNOR( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkXNORNum(pNtk) && (pNode = pNtk.Ntk_XNOR[i])); i++)
#define Jt_NtkForEachDCGATE( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkDCGATENum(pNtk) && (pNode = pNtk.Ntk_DCGATE[i])); i++)
#define Jt_NtkForEachMUX( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkMUXNum(pNtk) && (pNode = pNtk.Ntk_MUX[i])); i++)
#define Jt_NtkForEachNode( pNtk, pNode, i )											\
	for ( int i = 0; (i < Jt_NtkNodeNum(pNtk) && (pNode = pNtk.Ntk_AllNode[i])); i++)

#define Jt_NodeForEachFi( pNode, pFi, i )											\
	for ( int i = 0; (i < Jt_NodeFiNum(pNode) && (pFi = pNode->fi[i])); i++)
#define Jt_NodeForEachFo( pNode, pFo, i )											\
	for ( int i = 0; (i < Jt_NodeFoNum(pNode) && (pFo = pNode->fo[i])); i++)

#define Jt_VecForEachEntry( vec, pNode, i )											\
		for(int i = 0; (i < vec.size() && (pNode = vec[i])); i++)

#endif

