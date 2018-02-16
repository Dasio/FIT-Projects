#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>

#define DEFAULT_ARGC 4
#define BUFFER_SIZE 513
#define COMMAND_MAX_LEN BUFFER_SIZE - 1
#define EXIT_COMMAND "exit\n"
#define EXIT_COMMAND_LEN 5

typedef struct
{
	char **args;
	char *input;
	char *output;
	bool bg;
	int argc;
	int maxArgc;
} programInfo;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condBuffer = PTHREAD_COND_INITIALIZER;
pthread_t parseThread;
pthread_t execThread;
pid_t activePid;

volatile bool run = true;

char buffer[BUFFER_SIZE];

/** Parse buffer into program info struct **/
void parseBuffer(programInfo *program);
/** Save argument from buffer based on position **/
void pushArgument(programInfo *program, size_t startPos, size_t endPos);
/** Same like push argument **/
void saveFile(char **dest, size_t startPos, size_t endPos);
/** Sets program variables to default values **/
void clearProgram(programInfo *program);
/** Thread for reading input **/
void *parseWorker(void* s);
/** Thread for parsing and executing commands **/
void *execWorker(void* s);

static inline void waitThread()
{
	pthread_mutex_lock(&mutex);
	pthread_cond_wait(&condBuffer, &mutex);
	pthread_mutex_unlock(&mutex);
}

static inline void signalThread()
{
	pthread_mutex_lock(&mutex);
	pthread_cond_signal(&condBuffer);
	pthread_mutex_unlock(&mutex);
}

void clearProgram(programInfo *program)
{
	if(program == NULL)
		return;
	program->bg = false;
	if(program->input)
		free(program->input);
	if(program->output)
		free(program->output);
	program->input = program->output = false;
	for(int i=0; program->args[i] != NULL; i++)
		free(program->args[i]);
	program->args[0] = NULL;
	program->argc = 0; 
}


void *parseWorker(void* s)
{
	(void)s;
	ssize_t bytes;
	while(run)
	{
		putchar('$');
		putchar(' ');
		fflush(stdout);
		bytes = read(fileno(stdin), buffer, BUFFER_SIZE);
		if(bytes == 0)
		{
			run = false;
			signalThread();
			break;
		}
		if(bytes > COMMAND_MAX_LEN)
		{
			fprintf(stderr, "Command input was too long\n");
			continue;
		}
		if (strncmp(EXIT_COMMAND, buffer, EXIT_COMMAND_LEN) == 0)
		{
			run = false;
			// Let the exec thread know that we are finished
			signalThread();
			break;
		}
		// Ignore empty commands
		if (buffer[0] == '\n')
			continue;

		// Buffer can contain extra data if previous command was longer
		buffer[bytes - 1] = '\0';
		signalThread();
		waitThread();

	}
	return NULL;
}


void *execWorker(void* s)
{
	(void)s;
	struct sigaction sigIgn;
	sigIgn.sa_handler = SIG_IGN;
	sigIgn.sa_flags = 0;
	sigemptyset(&sigIgn.sa_mask);

	programInfo program;
	program.args = malloc(sizeof(char*) * DEFAULT_ARGC);
	program.args[0] = NULL;
	program.argc = 0;
	program.maxArgc = DEFAULT_ARGC;
	program.output = program.input = NULL;
	program.bg = false;

	int inFile, outFile;
	while(run)
	{
		// Wait
		waitThread();
		// We are finished
		if(!run)
			break;
		parseBuffer(&program);
		int forkId = fork();
		if (forkId > 0)
		{
			if (!program.bg)
			{
				activePid = forkId;
				waitpid(forkId, NULL, 0);
			}
		}
		// Child
		else if (forkId == 0)
		{
			// Duplicate file descriptors for input and output
			if (program.input)
			{
				inFile = open(program.input, O_RDONLY);
				if (inFile == -1)
				{
					fprintf(stderr, "Failed to open file\n");
					exit(1);
				}
				if (dup2(inFile, fileno(stdin)) == -1)
				{
					fprintf(stderr, "Failed to duplicate file descriptor for input\n");
					exit(1);
				}
			}
			if (program.output)
			{
				outFile = open(program.output, O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
				if (outFile == -1)
				{
					fprintf(stderr, "Failed to create file\n");
					exit(1);
				}
				if (dup2(outFile, fileno(stdout)) == -1)
				{
					fprintf(stderr, "Failed to duplicate file descriptor for output\n");
					exit(1);
				}
			}
			if(program.bg)
				sigaction(SIGINT, &sigIgn, NULL);
			// Execute program
			execvp(program.args[0], program.args);
			// The exec() functions return only if an error has occurred.
			// Print error from exec
			fprintf(stderr, "%s\n", strerror(errno));

			if(program.input)
				close(inFile);
			if(program.output)
				close(outFile);
			exit(1);
		}
		else
		{
			fprintf(stderr, "Failed to fork()\n");
			run = false;
			break;
		}
		clearProgram(&program);
		signalThread();
	}
	free(program.args);
	return NULL;
}
enum FMStates
{
	New,
	Add,
	Input,
	InputAdd,
	Output,
	OutputAdd
};

/** Magic FSM **/
void parseBuffer(programInfo *program)
{
	int state = New;
	size_t bufferLen = strlen(buffer);
	size_t startPos, endPos;;
	for(size_t i=0; i<bufferLen; ++i)
	{
		switch(state)
		{
			case New:
				if(isspace(buffer[i]))
					state = New;
				else if(buffer[i] == '<')
					state = Input;
				else if(buffer[i] == '>')
					state = Output;
				else if(buffer[i] == '&')
					program->bg = true;
				else
				{
					startPos = i;
					endPos = i;
					if(i == bufferLen-1)
					{
						pushArgument(program, startPos, endPos);
					}
					state = Add;
				}
				break;
			case Add:
				if(buffer[i] == '<')
				{
					state = Input;
					endPos = i - 1;
					pushArgument(program, startPos, endPos);
				}
				else if(buffer[i] == '>')
				{
					state = Output;
					endPos = i - 1;
					pushArgument(program, startPos, endPos);
				}
				else if(buffer[i] == '&')
				{
					program->bg = true;
					endPos = i - 1;
					pushArgument(program, startPos, endPos);

				}
				else if (isspace(buffer[i]) || i == bufferLen-1)
				{
					endPos = i;
					if(isspace(buffer[i]))
						--endPos;
					state = New;
					pushArgument(program, startPos, endPos);
				}
				break;
			case Input:
				if(!isspace(buffer[i]))
				{
					state = InputAdd;
					startPos = endPos = i;
					if(i == bufferLen-1)
					{
						saveFile(&program->input, startPos, endPos);
					}
				}
				break;
			case InputAdd:
				if(buffer[i] == '>')
				{
					state = Output;
					endPos = i - 1;
					saveFile(&program->input, startPos, endPos);
				}
				else if(buffer[i] == '&')
				{
					program->bg = true;
					endPos = i - 1;
					saveFile(&program->input, startPos, endPos);

				}
				else if (isspace(buffer[i]) || i == bufferLen-1)
				{
					endPos = i;
					if(isspace(buffer[i]))
						--endPos;
					state = New;
					saveFile(&program->input, startPos, endPos);
				}
				break;
			case Output:
				if(!isspace(buffer[i]))
				{
					state = OutputAdd;
					startPos = endPos = i;
					if(i == bufferLen-1)
					{
						saveFile(&program->output, startPos, endPos);
					}
				}
				break;
			case OutputAdd:
				if(buffer[i] == '<')
				{
					state = Input;
					endPos = i - 1;
					saveFile(&program->output, startPos, endPos);
				}
				else if(buffer[i] == '&')
				{
					program->bg = true;
					endPos = i - 1;
					saveFile(&program->output, startPos, endPos);

				}
				else if (isspace(buffer[i]) || i == bufferLen-1)
				{
					endPos = i;
					if(isspace(buffer[i]))
						--endPos;
					state = New;
					saveFile(&program->output, startPos, endPos);
				}
				break;
		}
	}

}

void pushArgument(programInfo *program, size_t startPos, size_t endPos)
{
	program->argc++;
	// One for null pointer
	if(program->argc > program->maxArgc + 1)
	{
		program->maxArgc <<= 1;
		program->args = realloc(program->args, sizeof(char*) * program->maxArgc);
	}
	size_t len = endPos - startPos + 1;
	char *arg = malloc(sizeof(char) * (len + 1));
	memcpy(arg, buffer + startPos, len);
	arg[len] = '\0';
	program->args[program->argc - 1] = arg;
	program->args[program->argc] = NULL;
}

void saveFile(char **dest, size_t startPos, size_t endPos)
{
	size_t len = endPos - startPos + 1;
	if(*dest)
		free(*dest);
	*dest = malloc(sizeof(char) * (len + 1));
	memcpy(*dest, buffer + startPos, len);
	(*dest)[len] = '\0';
}



void sigIntHandler()
{
	if (kill(activePid, SIGINT) == 0)
		putchar('\n');
	fflush(stdout);
}

void sigChldHandler()
{
	int status;
	pid_t pid = wait(&status);
	// Non-bg process
	if(pid == -1)
		return;
	// Well ... printf shouldn't be used in signal handlers
	if (WIFEXITED(status))
		printf("\nProcess '%d' exited with status code %d\n", pid, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("\nProcess '%d' was terminated by a signal %d\n", pid, WTERMSIG(status));
	else if (WIFSTOPPED(status))
		printf("\nProcess '%d' was stoppedby a signal %d\n", pid, WSTOPSIG(status));
	putchar('$');
	putchar(' ');
	fflush(stdout);

}

int main(void)
{
	struct sigaction sigInt, sigChld;

	sigInt.sa_handler = sigIntHandler;
	sigInt.sa_flags = 0;
	sigemptyset(&sigInt.sa_mask);
	sigaction(SIGINT, &sigInt, NULL);

	sigChld.sa_handler = sigChldHandler;
	sigChld.sa_flags = 0;
	sigemptyset(&sigChld.sa_mask);
	sigaction(SIGCHLD, &sigChld, NULL);

	// Create threads for parsing commands and executing them
	while (pthread_create(&parseThread, NULL, parseWorker, NULL) != 0)
	{
		if (errno == EAGAIN)
			continue;
		fprintf(stderr, "parse pthread_create() err=%d\n", errno);
		return 1;
	}

	while (pthread_create(&execThread, NULL, execWorker, NULL) != 0)
	{
		if (errno == EAGAIN)
			continue;
		fprintf(stderr, "exec pthread_create() err=%d\n", errno);
		return 1;
	}
	int err;
	if ((err = pthread_join(execThread, NULL)) != 0)
	{
		fprintf(stderr, "exec pthread_join() err=%d\n", err);
	}

	if ((err = pthread_join(parseThread, NULL)) != 0)
	{
		fprintf(stderr, "parse pthread_join() err=%d\n", err);
	}


	return 0;
}
