#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>



using namespace std;

struct msgR{//structure d'une requete
	long etiq;
	double op1;
	double op2;
	char operation;
	int pid;
};

struct msgRep{//structure d'une reponse
	long etiq;
	double rep;
};

double solve (double op1, double op2){
	return op1 + op2;
}

int main(){

	key_t cles = ftok("../fichier",10);
	if (cles < 0)
	{
		fprintf(stderr, "Erreur creation cles.\n");
		exit(1);
	}	
	
	int f_id = msgget(cles,IPC_CREAT|0666);//crée ou récupère la file de message

	msgR requete;
	msgRep reponse;

	while(1){
	
	//extraction requete
	msgrcv(f_id,&requete,sizeof(msgR) - sizeof(long), '+', 0);
	cout<<"Extraction requete du processus "<<requete.pid<<endl;
	
	//construction reponse
	reponse.rep=solve(requete.op1,requete.op2);
	reponse.etiq=requete.pid;

	//depose reponse
	msgsnd(f_id,&reponse,sizeof(msgRep) - sizeof(long),0);

	}

	return 0;
}
