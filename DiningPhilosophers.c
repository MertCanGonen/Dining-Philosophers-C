//Mert Can Gönen - 181101039 - Bil 461 Homework 2

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sched.h>
#include <unistd.h>
#include <signal.h>


//semaphores
sem_t check; //it checks that if 2 philosophers want to eat rice but there is only 1. Then just one of them can eat. 
sem_t *sticks; // defined for the sticks on the table

//waiter and restaurant variables
int day = 1; // day variable to calculate 1000 days
int riceForService = 0; // daily rice which is served by waiter 
int riceForTotal = 0; // total rice for 1000 days which is served by waiter

//philosopher variables
int philosopherNumber; // number of philosophers
int *dailyPorsionNumber; // integer array that keeps number of daily portion for each philosopher (for philosopher0 we check dailyPorsionNumber[0])
int *totalPorsionNumbers; // integer array that keeps number of total portion after 1000 days for each philosopher
int *hungryDays; // if philosopher didnt eat in a day increase it, when he ate check the maxHungryDay and make changes.
int *maxHungryDays; // maximum streak of days that philosopher didnt eat

// WAITER PART

void* waiter(void* param){ 
    while (day <= 1000) {
        serve();
        day++;
        sleep(0.01); //10 ms sleep
    }
    pthread_exit(NULL); //1000 days is over, restaurant is closed, waiter went his home.
}

void serve() {
    //keep waiting philosophers to finish the rice on the table
    while(riceForService > 0){ //after waiter made service and slept, he woke up, if philosophers didnt finish the rice, waiter waits for them to finish it in every 10 ms. 
        sleep(0.01);
    }
    
    if(day > 1){ //we are making hungry calculations (we calculate eating numbers in eat method of philosophers. we just calculate hungry values)  
        int tmp = 1 + (rand() % philosopherNumber); // served rice number
        riceForService = tmp; //rice serviced for daily
        riceForTotal += tmp; //add to total rice number
        int i;
        for (i=0; i<philosopherNumber; i++){ //check all philosophers that did he/she ate or did not.
            if(dailyPorsionNumber[i] != 0){ //philosopher ate rice.
                if(maxHungryDays[i] < hungryDays[i]){ //check last streak and max streak to being hungry, if he ate in the last turn, then it is 0 and after this turn it is 0 to.
                    maxHungryDays[i] = hungryDays[i];
                }
                hungryDays[i] = 0; //reset it 
            } 
            else{ // didnt eat, increase hungry day
                hungryDays[i]++;
            }
            dailyPorsionNumber[i] = 0; //reset for the next day's portion (we already add his today's portion to the its index in the dailyPorsionNumbers and totalPorsionNumbers arrays)
        }
    }
}

// PHILOSOPHER PART

void eat(int pnum) {
    while(riceForService <= 0){ // no rice to eat
        sleep(2); //restaurant is opened, waiting for the waiter to make rice service for 2 seconds
    }
    riceForService = riceForService-1;
    dailyPorsionNumber[pnum]++;
    totalPorsionNumbers[pnum]++;

}
 
void take(int pnum) { //it is classical philosopher action, takes sticks, eats and thinks.
    int rightStick = (pnum+1)%philosopherNumber;
    
    sem_wait(&sticks[pnum]);
    sem_wait(&sticks[rightStick]);

    sem_wait(&check); //check other eating actions of other philosophers (it prevents that 2 philosophers eat same time, becauese there can be just 1 porsion rice)
    eat(pnum);
    sem_post(&check); 

    sem_post(&sticks[pnum]);
    sem_post(&sticks[rightStick]);

    sleep(2); //think
}

void* philosopher(void* param) { //it takes philosopher number as parameter, then complete actions according to it (thread function of philosophers thread)
    int* pnum = param; //philosopher number, we will send it as parameter while we create philosopher threads
    while (day <= 1000){ //if restaurant is not closed, keep trying to eat rice
        take(*pnum);
    }
    pthread_exit(NULL);
}

// start all actions (philosophers and waiter)

int main(int argc, char **argv) {

    philosopherNumber = atoi(argv[1]);
    if(philosopherNumber <= 1){ 
        printf("Give an integer which is bigger than 1! We have a great waiter service to our customers. 1 philosopher is too easy :)\n");
        exit(0);
    }

    printf("Number of philosophers is: %d\n", philosopherNumber);
    printf("1000 Days without priority!\n");

    int size = philosopherNumber * sizeof(int);
    dailyPorsionNumber = malloc(size); 
    totalPorsionNumbers = malloc(size);
    hungryDays = malloc(size);
    maxHungryDays = malloc(size);
    sticks = malloc(philosopherNumber*sizeof(sem_t));

    pthread_t waiter;
    pthread_t philosophers[philosopherNumber];
    
    int philosopherNumbers[philosopherNumber]; //array to send number of philosophers to send function "philosopher"
    
    // initials
    int i;
    for (i=0; i<philosopherNumber; i++){
        philosopherNumbers[i] = i;
		totalPorsionNumbers[i] = 0;
	}

    sem_init(&check, 0, 1); //binary semaphore to prevent that 2 philoophers try to eat same time
    for(i=0; i<philosopherNumber; i++){
        sem_init(&sticks[i], 0, 1);
    }

    //waiter thread
    pthread_create(&waiter, NULL, waiter, NULL);

    //philosopher threads  ********************************************************************************HOCAM BURADA SEGMENTATION FAULT ATIYOR*************************************************
    for (i=0; i<philosopherNumber; i++){
        pthread_create(&philosophers[i], NULL, philosopher, (void *)&philosopherNumbers[i]);
    }

    pthread_join(waiter, NULL);    
    for (i=0; i<philosopherNumber; i++){
        pthread_join(philosophers[i], NULL); 
    }

    // 1000 days is over, print the stats of waiter and philosophers
    printf("Waiter was serviced %d rices for first 1000 days.\n\n", riceForTotal);
    for (i=0; i<philosopherNumber; i++){
        printf("-Philosopher%d-\n", i+1);
        printf("Rice porsion for first 1000 days: %d\n", totalPorsionNumbers[i]);
        printf("Max hungry days streak for first 1000 days is: %d\n", maxHungryDays[i]);
    }

    // second 1000 days , with priority difference
    printf("\n------------------------------------------------------------------------------\n");
    printf("1000 Days with priority!\n");
    
    // restart process
    day = 1;
    riceForService = 0;
    riceForTotal = 0;
    for (i=0; i<philosopherNumber; i++){
        dailyPorsionNumber[i] = 0;
		totalPorsionNumbers[i] = 0;
        hungryDays[i] = 0;
        maxHungryDays[i] = 0;
	}
    
    sem_init(&check, 0, 1);
    for(i=0; i<philosopherNumber; i++){
        sem_init(&sticks[i], 0, 1);
    }

    //priority jobs
    struct sched_param priority;
    pthread_attr_t attr_prior;
    pthread_attr_init (&attr_prior);
    pthread_attr_getschedparam (&attr_prior, &priority);
    priority.sched_priority = (2);
    pthread_attr_setschedparam (&attr_prior, &priority);
    
    pthread_create(&waiter, NULL, waiter, NULL);
    for (i=0; i<philosopherNumber; i++){
        if (i == 0){ //first philosopher, make his/her priority higher.
            pthread_create(&philosophers[i], &attr_prior, philosopher, (void *)&philosopherNumbers[i]);
        }else {
            pthread_create(&philosophers[i], NULL, philosopher, (void *)&philosopherNumbers[i]);
        }
    }
    
    pthread_join(waiter, NULL);
    for (i=0; i<philosopherNumber; i++){
        pthread_join(philosophers[i], NULL); 
    }

    printf("Waiter was serviced %d rices for first 1000 days.\n\n", riceForTotal);
    for (i=0; i<philosopherNumber; i++){
        printf("-Philosopher%d-\n", i+1);
        printf("Rice porsion for first 1000 days: %d\n", totalPorsionNumbers[i]);
        printf("Max hungry days streak for first 1000 days is: %d\n", maxHungryDays[i]);
    }
    
    exit(0);
}  
