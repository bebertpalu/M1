#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/wait.h>


using namespace std;

struct requete{//structure d'une requete
	long etiq;
	int n;
	int i;
	bool fin;
};

struct reponse{//structure d'une requete
	long etiq;
	int i;
};

struct finTraitement{
	long etiq;
};

pid_t create_process(void)
{
    pid_t pid;

    /* On fork() tant que l'erreur est EAGAIN */
    do {
	pid = fork();
    } while ((pid == -1) && (errno == EAGAIN));
	    return pid;
}

int main(){

	key_t cles = ftok("../fichier",10);
	if (cles < 0)
	{
		fprintf(stderr, "Erreur creation cles.\n");
		exit(1);
	}	
	
	int f_id = msgget(cles,IPC_CREAT|0666);//crée ou récupère la file de message
	if (f_id < 0)
	{
		fprintf(stderr, "Erreur creation file de message.\n");
		exit(1);
	}

	

	
	pid_t pid;

	/**
	*CODE DU PERE
	**/

	if (pid!=0){//code du pere 

	//creation des processus
	int nbProc = 5;
	for (int i=0; i<nbProc; i++)
	 {
		pid = create_process();	
	 }

	int nb;

	printf("saisir le nombre a decomposer : ");
	scanf("%d",&nb);
	
	//construction requete
	requete monNombre;
	monNombre.etiq=1;
	monNombre.n=nb;
	monNombre.i=2;

	//deposer le nombre dans la file
	msgsnd(f_id,&monNombre,sizeof(requete) - sizeof(long),0);

	//recupere les facteurs premiers
	reponse rep;
	int res=1;
	while(res =! nb){
		msgrcv(f_id,&rep,sizeof(reponse) - sizeof(long), '2', 0);
		cout<<rep.i<<endl;
		res = res * rep.i;
	}
	cout<<"Traitement terminé"<<endl;

	//Signale aux processus que le traitement est termine
	requete msgFin;
	msgFin.etiq=1;
	msgFin.fin=true;
	for (int i=0; i<nbProc; i++){
		msgsnd(f_id,&msgFin,sizeof(finTraitement) - sizeof(long),0);
	}

	while(wait(0) != -1);//attend que TOUS les fils soient morts

	cout<<"Fin d' execution"<<endl;
	
	}else{

	/**
	*CODE DES FILS
	**/

	//lit requete dans la liste
	requete req;
	reponse rep;
	do{
	requete req;
	msgrcv(f_id,&req,sizeof(requete) - sizeof(long), '1', 0);

	if (req.n > 1){
		if ((n%req.i)==0)
		{
			//depose dans la le facteur trouvé
			rep.etiq = 2;
			rep.i=req.i;
			msgsnd(f_id,&rep,sizeof(reponse) - sizeof(long),0);
			
			//renvoie autre facteur a decomposer
			

	}while(!req.fin);




	}


}