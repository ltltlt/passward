/*************************************************************************
	> File Name: passward.cpp
	> Author: ty-l
	> Mail: liuty196888@gmail.com 
	> Created Time: 2016年03月15日 星期二 10时14分31秒
 ************************************************************************/

#ifndef __PASSWARD_MANAGER
#define __PASSWARD_MANAGER
#include<iostream>
#include<fstream>
#include<map>
#include<string>
#include<algorithm>

namespace lty{
using namespace std;
	class handler{
	public:
		typedef map<string,pair<string,string>> mymap;
		handler():__secretKey("4294967295"){
			fs.open("/home/ty-l/lty/passward",ios_base::in);
			if(fs.is_open()){
				string signal;
				fs>>signal;
				if(signal=="passward"){
					fs.get();			//get white space
					getline(fs,__Passward,'\n');
				}else{
					cerr<<"No passward while have message!!"<<endl;
					cerr<<"automatically clear all message!!"<<endl;
					fs.close();
					fs.open("/media/ty-l/lty/passward",ios_base::out);		//删除信息（保证安全）
					fs.close();
					noPassward();
					return;
				}
				string name,account,passward;
				while(fs>>name>>account>>passward){
					int index=0;
					for_each(passward.begin(),passward.end(),[&](char& ch){
							ch-=__secretKey[index++];
							index%=__secretKey.size();
							});
					store(name,account,passward);
				}
				fs.close();
			}else
				noPassward();
		}
		~handler(){
			fs.open("/home/ty-l/lty/passward",ios_base::out);
			if(__Passward=="")
				return;
			fs<<"passward "<<__Passward<<endl;
			for(mymap::iterator it=__storer.begin();it!=__storer.end();it++){
				fs<<it->first<<' '<<it->second.first<<' '<<it->second.second<<endl;
			}
		}
		const string& passward(){	return __Passward;	}
		void noPassward(){
			cout<<"No account yet, maybe you login first time or maybe you clear your message."<<endl;
			cout<<"create a passward: ";
			getline(cin,__Passward,'\n');
			cout<<"Okay, it's done!"<<endl;
		}
		bool store(string key,string account,string passward){
			int index=0;
			for_each(passward.begin(),passward.end(),[&](char& ch){
					ch+=__secretKey[index++];
					index%=__secretKey.size();
					});
			return __storer.insert(make_pair(key,make_pair(account,passward))).second;
		}
		class notFound{	};
		pair<string,string> find(string key){
			int index=0;
			pair<string,string> temp=__storer.find(key)->second;
			if(temp.first=="" and temp.second=="")
				throw notFound();
			for_each(temp.second.begin(),temp.second.end(),[&](char& ch){
					ch-=__secretKey[index++];
					index%=__secretKey.size();
					});
			return temp;
		}
		void change(string key,string account,string passward){
			int index=0;
			for_each(passward.begin(),passward.end(),[&](char& ch){
					ch+=__secretKey[index++];
					index%=__secretKey.size();
					});
			__storer.erase(__storer.find(key));
			store(key,account,passward);
		}
	private:
		fstream fs;									//对应文件
		string __Passward;							//登录密码
		mymap __storer;								//存储帐号和密码
		const string __secretKey;					//加密密匙
	};
	void userface(){
		cout<<"Welcome "<<endl;
		handler h;
		cout<<"Passward: ";
		while(true){
			string passward;
			getline(cin,passward,'\n');
			if(passward==h.passward())
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
				getline(cin,name,'\n');				//cin will not throw space away, just ignore it
				cout<<"input account please: ";
				getline(cin,account,'\n');
				cout<<"input passward please: ";
				getline(cin,passward,'\n');
				if(c=='s' and h.store(name,account,passward))
					cout<<"Success."<<endl;
				else if(c=='s')
					cout<<"Already have this name!"<<endl;
				else
					h.change(name,account,passward);
			}else if(c=='f'){
				string name;
				cout<<"input name please: ";
				cin>>name;
				try{
					pair<string,string> temp=h.find(name);
					cout<<"account: "<<temp.first<<endl;
					cout<<"passward: "<<temp.second<<endl;
				}catch(handler::notFound&){
					cerr<<"Cannot find this name."<<endl;
				}
			}else if(c=='q'){
				return;
			}else{
				cerr<<"Error!!!"<<endl;
			}
		}
	}
};
#endif
