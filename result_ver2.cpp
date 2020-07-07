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
	//miterF.open("miter.v", ios::in);
    inF.open("out.txt", ios::in);
    if(!inF){
		cout << "EQ";
        inF.close();
    }
    else{
		cout <<"NEQ\n";
        string str;
        cin>>str;
        cin>>str;
        cin>>str;
        while(str != "PO"){
            
            cout<<str<<" ";
            cin>>str;
			inF.get(ch);
            cout<<ch<<endl;
            cin>>str;
		}
		inF.close();
    }
}