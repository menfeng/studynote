#include "Observer.h"
#include <iostream>
using namespace std;

Subject::Subject(){cout<<"Subject"<<endl;}
Subject::~Subject(){}
void Subject::SetState(const string& sdt){ _sdt = sdt;}
string Subject::GetState(){return _sdt;}
void Subject::Attach(Observer* obs){_obs.push_back(obs);}
void Subject::Detach(Observer* obs){
	for(vector<Observer *>::iterator it=_obs.begin(); it!=_obs.end();){
		if(*it==obs){
			it=_obs.erase(it);
		}
		else{
			++it;
		}
	}
}
void Subject::Notify(){
	for(vector<Observer *>::iterator it=_obs.begin();it!=_obs.end();it++){
		(*it)->Update();
	}
}


Observer::Observer(Subject *s){
	_sub=s;
	_sub->Attach(this);
	cout<<"Observer"<<endl;
}
Observer::~Observer(){
	_sub->Detach(this);
}
void Observer::PrintState(){cout<<_sdt<<"....."<<endl;}
void Observer::Update(){ _sdt = _sub->GetState(); }
