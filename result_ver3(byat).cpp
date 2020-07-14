#include <iostream>
#include <stdlib.h>
#include<stdio.h>
#include<string.h>
#include <queue>
using namespace std;

void Output_ans();

int main() {
     
	Output_ans();
    
	return 0;
}
void Output_ans(){
    
    char sat[4];
    int num;
	FILE * fp;
    if((fp=fopen("sat_out.txt","r"))==NULL){
          printf("open file error!!\n");
    }
    fscanf(fp,"%s\n",sat);
    string test;
    for(int i = 0;i<12;i++)cin>>test;
    num = atoi(test.c_str());
    while(test!="module")cin>>test;
    //cout<<num;
    //cout<<sat<<endl;
    if(sat == "USAT"){
		cout << "EQ";
        fclose(fp);
    }
    else{
		cout <<"NEQ\n";
        int gar;
        queue<string> name_que;
        string str;
        //cin>>str;
        cin>>str;
        cin>>str;
        //cout<<gar1<<" "<<gar2<<endl;
        while(str != "PO"){
            int ans;
            //cout<<str<<" ";
            name_que.push(str);
            cin>>str;
            cin>>str;
		}
        int times = num-name_que.size();
        for(int i = 0;i<times;i++){
            
            fscanf(fp,"%d",&gar);
        }
        int size = name_que.size();
        for(int i = 0;i<size;i++){
            fscanf(fp,"%d",&gar);
            cout<<name_que.front()<<" ";
            if(gar<0)cout<<"0"<<endl;
            else cout<<"1"<<endl;
            name_que.pop();
        }
        
        //cout<<name_que.size();
    }
    fclose(fp);
}

