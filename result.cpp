#include <iostream>
#include <fstream>
using namespace std;

void Output_ans();

int main() {
     
	Output_ans();
    
	return 0;
}
void Output_ans(){

	char ch;
    fstream inF;
	fstream miterF;
    //fstream outF;
	miterF.open("miter.v", ios::in);
    inF.open("out.txt", ios::in);
    if(!inF){
		cout << "EQ";
        inF.close();
    }
    else{
		cout <<"NEQ\n";
        while(!inF.eof()){
			inF.get(ch);
			if (ch != '='){
				cout << ch;
			}
			else {
				inF.get(ch);
				cout << " " << ch;
			}
		}
		inF.close();
    }
}