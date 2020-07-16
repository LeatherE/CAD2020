#include <iostream>
#include <fstream>
#include <vector>
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
		int bool_result, i;
		string str;
		vector<int> sat_result;
		vector<string> PI_name;
		
		sat_outF>>str;
		if(str != "SAT"){
			cout << "EQ";
			sat_outF.close();
			miterF.close();
			return 0;
		}
		sat_outF>>bool_result;
		while(bool_result != 0){
			sat_result.push_back(bool_result);
			sat_outF>>bool_result;
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
		for(i = 0; i < PI_name.size(); i++){
			cout<<PI_name[i]<<" ";
			cout<<((sat_result[first_PI_var + i]>0) ? 1 : 0) <<endl;
		}
		cout<<endl;
		sat_outF.close();
		miterF.close();
    }
	
	return 0;
}