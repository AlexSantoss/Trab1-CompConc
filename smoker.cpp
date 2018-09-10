#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

mutex mtx;

/*
id 0 - tabaco
id 1 - papel
id 2 - fosforo
*/

void agent(){
	cout << "Colocando 2 novos materiais na mesa..." << endl;
}

void smoker(int id){
	while(1){
		mtx.lock();
		cout << "Fumante " << id << " esta fazendo o cigarro e fumando...\n\n";
		mtx.unlock();
		agent();
		this_thread::sleep_for(chrono::seconds(1));
	}
}

int main(){

	thread smokers[3];

	for(int i = 0; i < 3;i++){
		smokers[i] = thread(smoker,i);
	}

	for(int i = 0; i < 3;i++){
		smokers[i].join();
	}
	return 0;
}