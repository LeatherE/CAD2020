#include <string> 
#include <vector>
#include <sstream>
#include <iostream> 
#include <algorithm>
#include "parser.hpp"
#include "datatype.hpp"

using namespace std;

const string dot = ",";
const string semi = ";";

class Miter {
  public:
	Miter();
	void GetGoldenCircuit();
	void GetRevisedCircuit();
	void FindOutput();
	void RenameWire();
	
	void PrintResult();
	
  private:
	Gate ReadNextGate();
	Gate PO;
	string tmp;
	bool IfAvailable();
	
	vector<string> ResultIn;
	vector<string> ResultOut;
	vector<string> ResultWire;
	vector<Gate> ResultGate;
	
	vector<string> GoldenIn;
	vector<string> GoldenOut;
	vector<string> GoldenWire;
	vector<Gate> GoldenGate;
	
	vector<string> RevisedIn;
	vector<string> RevisedOut;
	vector<string> RevisedWire;
	vector<Gate> RevisedGate;
	
	int count;
	void RenamePI();
	void AddMiter1(string g1, string g0, string r1, string r0);
	void AddMiter2(string g1, string g0, string r);
	void AddMiter3(string g, string r1, string r0);
	void AddMiter4(string g, string r);
};


template <typename T>
string to_string(T pNumber)
{
 ostringstream oOStrStream;
 oOStrStream << pNumber;
 return oOStrStream.str();
}
