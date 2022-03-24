//victor Cox dining philosophers problem
// make sure to include -pthread flag in compilation
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

// Global variables
#define N 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0

// left and right defintion (just to simplify things)
#define LEFT (phnum + 4) % N
#define RIGHT (phnum + 1) % N

// initialize states and philosophers
int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

// no two pick up and putdown at same time 
sem_t mutex;
// control each philosipher
sem_t S[N];

void test(int phnum)
{
	// check if can eat
	if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
		// state that eating
		state[phnum] = EATING;
		sleep(2);

		// print state
		printf("Philosopher %d takes fork %d and %d\n", phnum + 1, LEFT + 1, phnum + 1);
		printf("Philosopher %d is Eating\n", phnum + 1);
		sem_post(&S[phnum]);
	}
}

// pickup fork
void pick_fork(int phnum) {
	sem_wait(&mutex);
	state[phnum] = HUNGRY;
	printf("Philosopher %d is Hungry\n", phnum + 1);
	test(phnum);
	sem_post(&mutex);

	// if unable to eat wait to be signalled
	sem_wait(&S[phnum]);
	sleep(1);
}

// put down fork
void put_fork(int phnum) {
	sem_wait(&mutex);
	// state that thinking
	state[phnum] = THINKING;
	printf("Philosopher %d putting fork %d and %d down\n", phnum + 1, LEFT + 1, phnum + 1);
	printf("Philosopher %d is thinking\n", phnum + 1);
	//check left and right philosopher
	test(RIGHT);
	test(LEFT);

	sem_post(&mutex);
}

void* philosopher(void* num){
	// should this be an infinite loop?
	while (1) {
		int* i = num;
		sleep(1);
		pick_fork(*i);
		put_fork(*i);
	}
}

int main() {
	pthread_t thread_id[N];
	// initialize semaphores
	sem_init(&mutex, 0, 1);
	for (int i = 0; i < N; i++) {
		sem_init(&S[i], 0, 0);
	}

	for (int i = 0; i < N; i++) {
		// make philosopher
		pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
		printf("Philosopher %d is thinking\n", i + 1);
	}

	for (int i = 0; i < N; i++) {
		pthread_join(thread_id[i], NULL);
	}
}

