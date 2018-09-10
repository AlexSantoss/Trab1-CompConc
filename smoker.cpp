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


/*
Três fumantes estão em uma mesa e cada um deles possui estoque ilimitado de cada um dos três ingredientes para fazer um cigarro.
Tabaco, fosforo e papel. Um agente externo sinaliza que dois deles podem colocar a mesa seus respectivos materiais para produzir um cigarro, 
e então um fumante pega esses dois recursos e produz um cigarro(juntamente com seu terceiro material) e fuma. 
Ao terminar, ele sinaliza ao agente que já terminou e o agente coloca mais 2 ingredientes.
*/

/*
Os erros do código são de starvation pois, um dos processos repetidamente continua tendo acesso aos recursos providos pelo
agente enquanto que os outros dois esperam seu término, pois o programa trava as threads restantes na hora da seção critica.
*/
