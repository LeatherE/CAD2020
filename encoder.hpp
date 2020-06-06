#ifndef _ENCODER_HPP_
#define _ENCODER_HPP_

#include <string> 
#include <vector>
#include <sstream>
#include "datatype.hpp"
#include "parser.hpp"
using namespace std;

class Encoder {
  public:
    Encoder(vector<string> Tinput, vector<string> Toutput, vector<Gate> Tgate, int first_xgate, int *T_top_order);
	void EncodeCircuit();
	void PrintResult();
	
  private:
	vector<string> TargetIn;
	vector<string> TargetOut;
	vector<string> TargetWire;
	vector<Gate> TargetGate;
	int *top_order;
	int XFlag;
	int XgateNum;
	vector<string> ResultIn;
	vector<string> ResultOut;
	vector<string> ResultWire;
	vector<Gate> ResultGate;
	void AddEncoder(Gate g);
	void EncodeMultiInput(Gate g);
	void EncodeConstantInput(Gate g);
	string in00, in01, in10, in11;
	void EncodeAND(Gate g);
	void EncodeOR(Gate g);
	void EncodeNAND(Gate g);
	void EncodeNOR(Gate g);
	void EncodeNOT(Gate g);
	void EncodeBUF(Gate g);
	void EncodeXOR(Gate g);
	void EncodeXNOR(Gate g);
	void EncodeDC(Gate g);
	void EncodeMUX(Gate g);
	void TransferMUX(Gate g);
};



template <typename T>
string to_string(T pNumber)
{
 ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}

#endif
