/** @file rivercrossing.c
 * @brief Implementacia riesenia problemu "rivercrossing"
 * @author David Mikus, FIT (xmikus15@stud.fit.vutbr.cz)
 * @date 29.4.2014
 */
#include "rivercrossing.h"

const char *shmn[MAX_MEM] = {"/xmikus15_CounterMemory","/xmikus15_HackersCountMemory",
	"/xmikus15_SerfsCountMemory","/xmikus15_WaitingMembersMemory", "/xmikus15_WaitingToFinishMemory"
	}; /**< Mena suborov pre zdielanu pamet  */	
sem_t *semaphore[MAX_SEM]; /**< Pole ukazatelov na semafory ktore budu procesy vyuzivat */
int *shm[MAX_MEM]; /**< Pole  ukazatelov na zdielanu pamet ktore budu procesy vyuzivat */
int mShm[MAX_MEM]; /**< Popisovace subor zdielanej pamete */
FILE *output = NULL; /**< Subor do ktoreho sa bude zapisovat vypis procesov */
/**
 * @brief Hlavny proces ktory sa o vsetko stara. Viac priamo v zdrojovom kode
 * 
 * @param argc Pocet argumentov
 * @param **argv Ukazatel na pole argumentov
 */
int main(int argc,char **argv)
{
	/* Spracuje a do struktury ulozi argumenty */
	TParams params = {.statusCode=OK};
	processArguments(argc,argv,&params);
	if (params.statusCode==ERR)
	{
		fprintf(stderr,"Zle zadane argumenty!\n");
		return EXIT_FAILURE;
	}
	output = fopen("rivercrossing.out","w");
	if(output==NULL)
	{
		fprintf(stderr,"Nepodarilo sa otvorit subor pre zapis!\n");
		return ERR_SYS;
	}
	/* Vytvori a inicialiazuje semafory a zdielanu pamet */
	int state = init();
	if(state!=OK)
	{
		if(state == ERR_S) fprintf(stderr,"Nepodarilo sa alokovat semafory\n");
		else if(state == ERR_SHM) fprintf(stderr,"Nepodarilo sa alokovat zdielanu pamet\n");
		/* Vycisti pripadne otvoreny semafory alebo zdielanu pamet */
		cleanAndExit();
	}
	/* Signal handleri */
	signal(SIGTERM,cleanAndExit);
	signal(SIGINT,cleanAndExit);
	pid_t pid;   
	/* Line buffering pre spravny zapis */;
	setvbuf(output,NULL,_IOLBF,0);
	pid = fork();
	if(pid==0)
	{
		signal(SIGTERM,childExit);
		signal(SIGINT,childExit);
		/* Generator hackerov */
		generate(params.p,params.h,true,params.r);
	}
	else if(pid>0)
	{
		pid=fork();
		if(pid == 0)
		{
			signal(SIGTERM,childExit);
			signal(SIGINT,childExit);
			/* Generator serfov */
			generate(params.p,params.s,false,params.r);
		}
		else if(pid<0)
		{
			fprintf(stderr,"Nepodarilo sa forknut generatora serfov\n");
			cleanAndExit();
		}	   
	}
	else
	{
		fprintf(stderr,"Nepodarilo sa forknut generatora hackerov\n");
		cleanAndExit();
	}
	/* Hl. proces caka na skoncenie oboch fork-nutych procesov */
	wait(NULL);
	wait(NULL);
	if(fclose(output)!=0)
	{
		fprintf(stderr,"Nepodarilo sa zatvorit subor\n");		
	}
	state=cleanup();
	if (state!=OK)
	{
		if(state&ERR_S) fprintf(stderr,"Nepodarilo sa dealokovat semafor\n");
		if(state&ERR_SHM) fprintf(stderr,"Nepodarilo sa dealokovat zdielanu pamet\n");
		return ERR_SYS;
	}
	return EXIT_SUCCESS;
}
void generate(int p,int delay,bool isHacker,int sailTime)
{
	/* Pouzity aj pid, pre lepsiu "nahodnost" */
	srand(time(NULL)&getpid());
	pid_t pid;
	for(int i=0;i<p;i++)
	{
		/* Generator sa uspi na nahodny cas */
		usleep((rand()%(delay+1))*1000);
		pid=fork();
		if(pid==0) /* Vygenerovana osoba */
		{
			member(i+1,isHacker,rand()%(sailTime+1),2*p);
		}
		else if (pid<0)
		{
			fprintf(stderr,"Nepodarilo sa forknut osobu\n");
			childExit();
		}
	}
	if (output) fclose(output);
	/* Caka kym vsetci vytvoreni potomkovia skoncia */
	while(wait(NULL) > 0);
	exit(EXIT_SUCCESS);
}
void member(int id,bool isHacker,int sailTime,int maxMembers)
{
	char name[] = "hacker";
	if (!isHacker) strcpy(name,"serf");
	bool isCaptain=false;
	sem_wait(semaphore[EXCLUSIVE]);
	fprintf(output,"%d: %s: %d: started\n",++(*shm[COUNTER]),name,id);
	sem_post(semaphore[EXCLUSIVE]);
	/* Caka na "signal" kedy moze vstupit na molo */ 
	sem_wait(semaphore[MOLO]);
	sem_wait(semaphore[EXCLUSIVE]);
	/* Zvysi sa pocet hacker/serf-ov na mole */
	if (isHacker) (*shm[HACKERS_COUNT])++;
		else (*shm[SERFS_COUNT])++;
	fprintf(output,"%d: %s: %d: waiting for boarding: %d: %d\n",++(*shm[COUNTER]),name,id,*shm[HACKERS_COUNT],*shm[SERFS_COUNT]);
	sem_post(semaphore[EXCLUSIVE]); 
	sem_wait(semaphore[EXCLUSIVE]);
	/* Osoba co nastupila, zistuje ci je mozne spravit vhodnu skupinu
	 * Ak ano, stane sa kapitan a pusti ostatnych na lod */
	if(*shm[HACKERS_COUNT]==4)
	{
		for(int i=0;i<4;i++) sem_post(semaphore[HACKER]);
		*shm[HACKERS_COUNT]-=4;
		isCaptain=true;
	}
	else if (*shm[SERFS_COUNT]==4)
	{
		for(int i=0;i<4;i++) sem_post(semaphore[SERF]);
		*shm[SERFS_COUNT]-=4;
		isCaptain=true;
	}
	else if (*shm[HACKERS_COUNT]>=2 && *shm[SERFS_COUNT]>=2)
	{
		for(int i=0;i<2;i++) sem_post(semaphore[HACKER]);
		for(int i=0;i<2;i++) sem_post(semaphore[SERF]);
		*shm[HACKERS_COUNT]-=2;
		*shm[SERFS_COUNT]-=2;
		isCaptain=true;
	}
	/* Ak nie je mozne spravit skupinu, pusti sa na MOLO dalsiu osoba */
	else sem_post(semaphore[MOLO]);
	sem_post(semaphore[EXCLUSIVE]);
	/* Caka na nalodenie, teda kym sa utvori vhodna skupina */
	if (isHacker) sem_wait(semaphore[HACKER]);
	 else sem_wait(semaphore[SERF]);
	sem_wait(semaphore[EXCLUSIVE]);
	fprintf(output,"%d: %s: %d: boarding: %d: %d\n",++(*shm[COUNTER]),name,id,*shm[HACKERS_COUNT],*shm[SERFS_COUNT]);
	(*shm[WAITING_MEMBERS])++;
	/* Vypisal sa posledny "boarding", moze pustit ostatnych, vratane seba dalej */
	if(*shm[WAITING_MEMBERS]==4)
	{
		 for(int i=0;i<4;i++) sem_post(semaphore[BOARDING]);
		 /* Inicializacia na nulu pre dalsie pouzitie */
		 *shm[WAITING_MEMBERS]=0;
	}
	sem_post(semaphore[EXCLUSIVE]);
	/* Caka, kym sa vsetci nalodia */
	sem_wait(semaphore[BOARDING]);  
	sem_wait(semaphore[EXCLUSIVE]);
	fprintf(output,"%d: %s: %d: %s\n",++(*shm[COUNTER]),name,id,isCaptain?"captain":"member");
	(*shm[WAITING_MEMBERS])++;  
	if(*shm[WAITING_MEMBERS]==4)
	{
		 for(int i=0;i<4;i++) sem_post(semaphore[LANDING]);
		 /* Inicializacia na nulu pre dalsie pouzitie */
		 *shm[WAITING_MEMBERS]=0;
	}
	sem_post(semaphore[EXCLUSIVE]);
	/* Caka kym vsetci vypisu svoju kategoriu */
	sem_wait(semaphore[LANDING]);
	if(isCaptain)
	{
		/* Kapitan sa uspi a potom uvolni kazdeho k vylodeniu */
		usleep(sailTime*1000);
		for(int i=0;i<4;i++) sem_post(semaphore[CAPTAIN]);
	}
	/* Caka sa na kapitana, teda ukoncenie plavby */
	sem_wait(semaphore[CAPTAIN]);  
	sem_wait(semaphore[EXCLUSIVE]);
	fprintf(output,"%d: %s: %d: landing: %d: %d\n",++(*shm[COUNTER]),name,id,*shm[HACKERS_COUNT],*shm[SERFS_COUNT]);
	/* Kolko osob sa uz vylodilo */
	(*shm[WAITING_MEMBERS])++;
	/* Ked sa vylodi posledna osoba, tak pusti na molo dalsieho */
	if(*shm[WAITING_MEMBERS]==4)
	{
		 sem_post(semaphore[MOLO]);
		 *shm[WAITING_MEMBERS]=0;
	}
	sem_post(semaphore[EXCLUSIVE]);
	sem_wait(semaphore[EXCLUSIVE]);
	/* Posledny proces co sa vylodi, tak pusti ostatnych k ukonceniu */
	(*shm[WAITING_TO_FINISH])++;
	if (*shm[WAITING_TO_FINISH]==maxMembers)
		for(int i=0;i<maxMembers;i++)
			sem_post(semaphore[FINISH]);
	sem_post(semaphore[EXCLUSIVE]);
	/* Caka na vylodenie vsetkych procesov */
	sem_wait(semaphore[FINISH]);
	sem_wait(semaphore[EXCLUSIVE]);
	fprintf(output,"%d: %s: %d: finished\n",++(*shm[COUNTER]),name,id);
	sem_post(semaphore[EXCLUSIVE]);
	if (output) fclose(output);
	exit(EXIT_SUCCESS);
}
void processArguments(int argc,char **argv,TParams *params)
{
	char *err;
	if (argc!=5)
	{
		 params->statusCode=ERR;
		 return;
	}
	for(int i=0;i<4;i++)
	{
		/* Pointerova aritmetika:
		 * Adresu struktury params(void*), pretypuje ju na (int*)
		 * pomocou [] derefencuje a ziska i-ty prvok struktury,
		 * kedze obsahuje len int-y ktore su ulozene za sebou */
		((int*)params)[i] = strtol(argv[i+1],&err,10);
		if (*err!=0) params->statusCode=ERR;
		if (i==0)
		{
			if (params->p<=0 || params->p%2!=0) params->statusCode=ERR;
		}
		else if (((int*)params)[i]<0 || ((int*)params)[i]>5000)
			params->statusCode=ERR;			
	}
}
int init()
{
	int initValue;
	/* Semafory */
	for(int i=0;i<MAX_SEM;i++)
	{
		if((semaphore[i] = mmap(NULL,sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_SHARED,0,0))==MAP_FAILED) return ERR_S;
		if (i==EXCLUSIVE || i==MOLO) initValue=1;
		else initValue=0;
		if(sem_init(semaphore[i],1,initValue)<0) return ERR_S;
	}
	/* Zdielana pamet */
	for(int i=0;i<MAX_MEM;i++)
	{
		if((mShm[i]=shm_open(shmn[i], O_RDWR|O_CREAT, 0600))<0) return ERR_SHM;
		if(ftruncate(mShm[i],sizeof(int))<0) return ERR_SHM;
		if((shm[i]= mmap(NULL,sizeof(int),PROT_READ|PROT_WRITE,MAP_SHARED,mShm[i],0))==MAP_FAILED) return ERR_SHM;
		*shm[i]=0;
	}   
	return OK;
}
int cleanup()
{
	int statusCode = OK;
	/* Semafory */
	for(int i=0;i<MAX_SEM;i++)
	{
		/* Odmapujeme pamet */
		if(munmap(semaphore[i],sizeof(sem_t))<0) statusCode |= ERR_S;
		/* Odstranime semafor */
		if(sem_destroy(semaphore[i])<0) statusCode |= ERR_S;
	}
	/* Zdielana pamet */
	for(int i=0;i<MAX_MEM;i++)
	{
		/* Odmapujeme pamet */
		if(munmap(shm[i], sizeof(int))<0) statusCode |= ERR_SHM;
		/* Zmazeme pamet */
		if(shm_unlink(shmn[i])<0) statusCode |= ERR_SHM;
		/* Zavrieme pamet */
		if(close(mShm[i])<0) statusCode |= ERR_SHM;
	}
	return statusCode;
}
void cleanAndExit()
{
	cleanup();
	if(output) fclose(output);
	/* Posle signal ostatnym procesom */
	killpg(0,SIGTERM);
	/* Caka kym sa vsetci ukoncia */
	while(wait(NULL)>0);
	exit(ERR_SYS);
}
void childExit()
{
	killpg(0,SIGTERM);
	while(wait(NULL)>0);
	exit(ERR_SYS);
}
