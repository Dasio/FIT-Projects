#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>

// Nano seconds
#define HALF_SECOND 500000000L

typedef struct
{
	pthread_t thread;
	int id;
	int ticket;
	unsigned seed;
} threadInfo;

// Global variables

volatile int actualTicket = 0;
long int threadsCount; // N
long int passThroughs; // M
pthread_mutex_t mutexTicket = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CSCond = PTHREAD_COND_INITIALIZER;

// Function declarations
int getticket(void);
void await(int aenter);
void advance(void);
void printHelp(void);
void waitRandom(threadInfo *thread);
void printHelp();

int getticket(void)
{
	static volatile int nextTicket = -1;
	int localTicket;
	int err;
	err = pthread_mutex_lock(&mutexTicket);
	if (err != 0)
	{
		fprintf(stderr, "pthread_mutex_lock() err=%d\n", err);
	}
	localTicket = ++nextTicket;
	err = pthread_mutex_unlock(&mutexTicket);
	if (err != 0)
	{
		fprintf(stderr, "pthread_mutex_unlock() err=%d\n", err);
	}
	return localTicket;
}

void await(int aenter)
{
	int err;
	err = pthread_mutex_lock(&mutexTicket);
	if (err != 0)
	{
		fprintf(stderr, "pthread_mutex_lock() err=%d\n", err);
	}
	while(aenter != actualTicket)
	{
		err = pthread_cond_wait(&CSCond, &mutexTicket);
		if (err != 0)
		{
			fprintf(stderr, "pthread_cond_wait() err=%d\n", err);
		}
	}
}

void advance(void)
{
	int err;
	err = pthread_mutex_unlock(&mutexTicket);
	if (err != 0)
	{
		fprintf(stderr, "pthread_mutex_unlock() err=%d\n", err);
	}
	++actualTicket;
	err = pthread_cond_broadcast(&CSCond);
	if (err != 0)
	{
		fprintf(stderr, "pthread_cond_broadcast() err=%d\n", err);
	}
}

void waitRandom(threadInfo *thread)
{
	struct timespec req = {0};
	req.tv_nsec = rand_r(&thread->seed) % HALF_SECOND;
	errno = 0;
	// Sleep in loop until remaining time is zero
	while (nanosleep(&req, &req) == -1 && errno == EINTR);

}

void printHelp()
{
	printf("Run as ./program <threadsCount> <passThroughs>\n");
}

bool parseNumber(char *string, long int *number)
{
	if (number == NULL || string == NULL)
		return false;

	char *endptr;
	errno = 0;
	*number = strtol(string, &endptr, 10);
	// Range error
	if ((errno == ERANGE && (*number == LONG_MAX || *number == LONG_MIN)) || (errno != 0 && *number == 0))
	{
		perror("strtol");
		return false;
	}
	// It contains no digits or there are also another characters besides digits
	if(endptr == string ||  *endptr != '\0')
	{
		return false;
	}
	return true;
}

void *worker(void *info)
{
	threadInfo *thread = info;

	while ((thread->ticket = getticket()) < passThroughs)
	{
	    waitRandom(thread);
	    await(thread->ticket); 
	    printf("%d\t(%d)\n", thread->ticket, thread->id);
	    fflush(stdout);
	    advance();
	    waitRandom(thread);
	}
	return NULL;
}

int main(int argc, char **argv)
{
	if(argc <= 2)
	{
		printHelp();
		return 1;
	}
	
	if(!parseNumber(argv[1], &threadsCount) || !parseNumber(argv[2], &passThroughs))
	{
		printHelp();
		return 1;
	}
	threadInfo *threads = malloc(sizeof(threadInfo) * threadsCount);
	if(threads == NULL)
	{
		fprintf(stderr,"Failed to allocate memory\n");
		return 1;
	}
	for(int i=0; i<threadsCount; ++i)
	{
		threads[i].id = i + 1;
		threads[i].ticket = -1;
		threads[i].seed = getpid() * threads[i].id;
		while (pthread_create(&threads[i].thread, NULL, worker, &threads[i]) != 0)
		{
			//if (errno == EAGAIN)
			//	continue;
			fprintf(stderr, "pthread_create() err=%d\n", errno);
			free(threads);
			return 1;
		}
	}
	int err;
	for(int i=0; i<threadsCount; ++i)
	{
		if ((err = pthread_join(threads[i].thread, NULL)) != 0)
		{
			fprintf(stderr, "pthread_join() err=%d\n", err);
		}
	}
	free(threads);
	
	return 0;
}
