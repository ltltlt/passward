/*************************************************************************
	> File Name: passward.cpp
	> Author: ty-l
	> Mail: liuty196888@gmail.com 
	> Created Time: 2016年03月15日 星期二 10时14分31秒
 ************************************************************************/

#include<iostream>
#include<map>
#include<string>
#include<fstream>
#include<cassert>
namespace lty{
	bool input();
	void output();
	using namespace std;
	class handler{
	public:
		handler():__signal(input()){
		}
		~handler(){
			output();
		}
		bool signal(){	return __signal;	}
	private:
		bool __signal;
	};
	typedef map<string,pair<string,string>> mymap;
	mymap storer;			//存储帐号和密码
	string Passward;		//登录密码
	inline bool store(string key,string account,string passward){
		return storer.insert(make_pair(key,make_pair(account,passward))).second;
	}
	inline pair<string,string> find(string key){
		return storer.find(key)->second;
	}
	inline void change(string key,string account,string passward){
		storer.erase(storer.find(key));
		store(key,account,passward);
	}
	void noPassward(){
		cout<<"No account yet, maybe you login first time or maybe you clear your message."<<endl;
		cout<<"create a passward: ";
		getline(cin,Passward,'\n');
		cout<<"Okay, it's done!"<<endl;
	}
	void userface(){
		cout<<"Welcome "<<endl;
		handler h;
		if(!h.signal()){
			noPassward();
		}
		cout<<"Passward: ";
		while(true){
			string passward;
			getline(cin,passward,'\n');
			if(passward==Passward)
				break;
			cout<<"error, try again: ";
		}
		while(true){
			cout<<"s(store passward)	f(find passward)	c(change passward)	q(quit)"<<endl;
			char c;
			cin>>c;
			cin.get();			//clear white space
			if(c=='s' or c=='c'){
				string name,account,passward;
				cout<<"input name please: ";
				std::getline(cin,name,'\n');				//cin will not throw space away, just ignore it
				cout<<"input account please: ";
				std::getline(cin,account,'\n');
				cout<<"input passward please: ";
				std::getline(cin,passward,'\n');
				if(c=='s' and store(name,account,passward))
					cout<<"Success."<<endl;
				else if(c=='s')
					cout<<"Already have this name!"<<endl;
				else
					change(name,account,passward);
			}else if(c=='f'){
				string name;
				cout<<"input name please: ";
				cin>>name;
				pair<string,string> temp=find(name);
				if(temp.first=="" and temp.second==""){
					cerr<<"Cannot find this name."<<endl;
				}else{
					cout<<"account:  "<<temp.first<<endl;
					cout<<"passward: "<<temp.second<<endl;
				}
			}else if(c=='q'){
				return;
			}else{
				cerr<<"Error!!!"<<endl;
			}
		}
	}
	bool input(){
		ifstream ifs;
		ifs.open("/home/ty-l/lty/passward",std::ios_base::in);
		if(ifs.is_open()){
			do{
				string signal;
				ifs>>signal;
				if(signal=="passward"){
					ifs.get();			//get white space
					getline(ifs,Passward,'\n');
				}else{
					cerr<<"No passward while have message!!"<<endl;
					exit(1);
				}
			}while(false);
			string name,account,passward;
			while(ifs>>name>>account>>passward){
				store(name,account,passward);
			}
			ifs.close();
			return true;
		}
		return false;
	}
	void output(){
		ofstream ofs("/home/ty-l/lty/passward");
		if(!ofs.is_open()){
			cerr<<"output error!!"<<endl;
			exit(1);
		}
		ofs<<"passward "<<Passward<<endl;
		for(mymap::iterator it=storer.begin();it!=storer.end();it++){
			ofs<<it->first<<' '<<it->second.first<<' '<<it->second.second<<endl;
		}
		ofs.close();
	}
};
