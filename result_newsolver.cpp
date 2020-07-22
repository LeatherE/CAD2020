#include <iostream>
#include <fstream>
#include <vector>
#include<string.h>
using namespace std;

int main(int argc, char* argv[]){
    
	fstream sat_outF;
	fstream miterF;
	miterF.open(argv[1], ios::in);
    sat_outF.open(argv[2], ios::in);
	if(!sat_outF){
		cout << "EQ";
        sat_outF.close();
		miterF.close();
    }
    else{
		int  i;
		string bool_result,str;
		vector<bool> sat_result;
		vector<string> PI_name;
		
		sat_outF>>str;
        sat_outF>>str;
		if(str != "SATISFIABLE"){
			cout << "EQ";
			sat_outF.close();
			miterF.close();
			return 0;
		}
        sat_outF>>bool_result;
		sat_outF>>bool_result;
		while(bool_result != "0"){

            (bool_result.substr(0, 1) == "-")? sat_result.push_back(0):sat_result.push_back(1);
			sat_outF>>bool_result;
            if(bool_result == "v")sat_outF>>bool_result;
		}
		
		
		cout <<"NEQ\n";
        miterF>>str;
        miterF>>str;
        miterF>>str;
        while(str != "PO"){
            PI_name.push_back(str);
            miterF>>str;
            miterF>>str;
		}
		int first_PI_var = sat_result.size() - PI_name.size() - 1;
        
		for(i = 0; i < PI_name.size(); ++i){
			cout<<PI_name[i]<<" ";
			cout<<((sat_result[first_PI_var + i]>0) ? 1 : 0) <<endl;
		}
		cout<<endl;
		sat_outF.close();
		miterF.close();
    }
	
	return 0;
}