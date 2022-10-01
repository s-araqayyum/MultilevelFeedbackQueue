/*Simulating the working of different scheduling algorithms by implementing
multi - level feedback queue for implementing these algorithms.*/

#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

//Queue Structure to push to an array implementing a queue
struct Queue{
    int PID;
    int ArrivalTime;
    int CPUBurstTime;
    int Priority;
    int WaitingTime;
    int TurnAroundTime;

    bool arrived;
    bool pushedToWaitingQueue;
    bool completed;
}QueuePQ[7],QueueRR8[4], QueueRR10[2], QueueFCFS[1];

//Function Declarations
void FileReading(); //Reading File and pushing contents to an array
void sortingFileContents(); //Putting file contents into first queue for later manipulation
void numberOfAlgorithmDecision(int numberOfAlgorithms); //Changing flags for schedulers
void implementPriorityScheduling();
void implementRoundRobin8();
void implementRoundRobin10();
void implementingFCFS();

//Miscellaneous
int fileContents[24]; //Pre-declared
int originalBurstTime[6];
int originalBurstTimeSecond[4];
int originalBurstTimeThird[2];
bool priorityFlag = false;
bool rr8Flag = false;
bool rr10Flag = false;
bool fcfsFlag = false;

//Relating to Queue Data
int averageWaitingTime;
int averageTurnAroundTime;

int main() {

    //6. Your program must take / read processes from the input file for scheduling
    FileReading();
    sortingFileContents();
    for(int i = 0; i < 6; i++){
        QueuePQ[i].pushedToWaitingQueue = false;
    }
    //1. Your simulation program must ask the user; how many algorithms you want to implement
    int numberOfAlgorithms;
    printf("Enter the number of algorithms you would like to implement:: ");
    scanf("%d", &numberOfAlgorithms);

    //2. After that, creates that number of queues.
    numberOfAlgorithmDecision(numberOfAlgorithms);

    if(priorityFlag){
        printf("\nPriority Scheduling with First Queue - Result:\n");
        implementPriorityScheduling();
    }

    if(rr8Flag){
        printf("\nRound Robin(Quantum 8) Scheduling with Second Queue - Result:\n");
        implementRoundRobin8();
    }

    if(rr10Flag){
        printf("\nRound Robin(Quantum 10) Scheduling with Third Queue - Result:\n");
        implementRoundRobin10();
    }

    if(fcfsFlag){
        printf("\nFirst-Come-First-Serve Scheduling with Fourth Queue - Result:\n");
        implementingFCFS();
    }

    return 0;
}

void FileReading() {

    FILE* filepointer;
    filepointer = fopen("input.txt", "r");

    int fileContentsIndex=0;
    int contentToRead;

    while(fscanf(filepointer, "%d", &contentToRead) > 0) {
        fileContents[fileContentsIndex] = contentToRead;
        fileContentsIndex++;
    }
    fclose(filepointer);
}

void sortingFileContents(){
    int queueIndex = 0;
    int originalBurstIndex = 0;

    for(int i = 0; i < 24; i++){
        QueuePQ[queueIndex].PID = fileContents[i];
        i++;
        QueuePQ[queueIndex].ArrivalTime = fileContents[i];
        i++;
        QueuePQ[queueIndex].CPUBurstTime = fileContents[i];
        originalBurstTime[originalBurstIndex] = fileContents[i];
        i++;
        QueuePQ[queueIndex].Priority = fileContents[i];

        queueIndex++;
        originalBurstIndex++;
    }
}

void numberOfAlgorithmDecision(int numberOfAlgorithms){
    if(numberOfAlgorithms == 1){
        priorityFlag = true;
    }
    else if(numberOfAlgorithms == 2){
        priorityFlag = true;
        rr8Flag = true;
    }
    else if(numberOfAlgorithms == 3){
        priorityFlag = true;
        rr8Flag = true;
        rr10Flag = true;
    }
    else if(numberOfAlgorithms == 4){
        priorityFlag = true;
        rr8Flag = true;
        rr10Flag = true;
        fcfsFlag = true;
    }
    else{
        printf("Invalid Number Error. Please choose a number between 1 to 4, based on present scheduling algorithms");
        exit(0);
    }
}

void implementPriorityScheduling() {
    int secondQueueIndex = 0; //Annotates the index of QueueRR8
    int indexInUse; //Annotates the index currently in use

    int processIncrementer = 0; //Processes either skipped or counted
    int numberOfProcesses = 6; //Generic number of processes hardcoded

    QueuePQ[7].Priority = INT_MIN; //An initial extra process to annotate a high priority

    int previousPosition = -1; //Position to alert when new position is taken

    for (int currentTime = 0; processIncrementer != numberOfProcesses; currentTime++){
        indexInUse = 7;
        for (int i = 0; i < numberOfProcesses; i++){
            if (QueuePQ[i].pushedToWaitingQueue == false && QueuePQ[indexInUse].Priority < QueuePQ[i].Priority && QueuePQ[i].ArrivalTime <= currentTime  && QueuePQ[i].completed == false) {

                if(rr8Flag == true && previousPosition != i && previousPosition>=0 && previousPosition<6 && QueuePQ[previousPosition].CPUBurstTime > 0 && QueuePQ[previousPosition].pushedToWaitingQueue == false){
                    int timeProcessSpentRunning = 0;

                    QueueRR8[secondQueueIndex].PID = QueuePQ[previousPosition].PID;
                    if(originalBurstTime[previousPosition] != QueuePQ[previousPosition].CPUBurstTime){ //Idea is to include the time spent processing so see when it arrives in the next queue
                        timeProcessSpentRunning = originalBurstTime[previousPosition] - QueuePQ[previousPosition].CPUBurstTime;
                    }

                    QueueRR8[secondQueueIndex].ArrivalTime = QueuePQ[previousPosition].ArrivalTime + timeProcessSpentRunning;
                    QueueRR8[secondQueueIndex].CPUBurstTime = QueuePQ[previousPosition].CPUBurstTime;

                    QueuePQ[previousPosition].pushedToWaitingQueue = true;
                    processIncrementer++;
                    secondQueueIndex++;
                }
                else{
                    indexInUse = i;
                    QueuePQ[indexInUse].arrived = true;
                }
            }
        }

        QueuePQ[indexInUse].CPUBurstTime--;

        if (QueuePQ[indexInUse].CPUBurstTime == 0){
            processIncrementer++;
            QueuePQ[indexInUse].completed = true;
            QueuePQ[indexInUse].TurnAroundTime = currentTime + 1 - QueuePQ[indexInUse].ArrivalTime;
            QueuePQ[indexInUse].WaitingTime = QueuePQ[indexInUse].TurnAroundTime - originalBurstTime[indexInUse];
        }
        previousPosition = indexInUse;
    }

    for(int i = 0; i < 6; i++){

        if(QueuePQ[i].arrived == false){ //If process got skipped, then arrival time will be same
            QueueRR8[secondQueueIndex].PID = QueuePQ[i].PID;
            QueueRR8[secondQueueIndex].ArrivalTime = QueuePQ[i].ArrivalTime;
            QueueRR8[secondQueueIndex].CPUBurstTime = QueuePQ[i].CPUBurstTime;
            secondQueueIndex++;
        }

    }

    printf("PID\tArrival Time\tCPU Burst Time\tWaiting Time\tTurnAround Time\n");
    for (int i = 0; i < 6; i++) {
        if(QueuePQ[i].CPUBurstTime == 0){ //Print only the completed processes
            printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", QueuePQ[i].PID, QueuePQ[i].ArrivalTime, originalBurstTime[i], QueuePQ[i].WaitingTime, QueuePQ[i].TurnAroundTime);
        }
    }

    for(int i =0; i < 6; i++){
        averageWaitingTime += QueuePQ[i].WaitingTime;
        averageTurnAroundTime += QueuePQ[i].TurnAroundTime;
    }

    double divisor;
    if(rr8Flag){
        divisor = 2.0;
    }
    else{
        divisor = 6.0;
    }

    printf("The average waiting time for this queue is %lf\n",averageWaitingTime/divisor);
    printf("The average turn-around time for this queue is %lf\n",averageTurnAroundTime/divisor);
}

void implementRoundRobin8() {

    int numberOfProcesses = 4;
    int thirdQueueIndex = 0;

    for(int i = 0; i < 4; i++){ //Storing original burst times for later display
        originalBurstTimeSecond[i] = QueueRR8[i].CPUBurstTime;
    }

    int position = 0;
    for (int i = 1; i < 4; i++){ //Starting the time index at the minimum arrival time
        if (QueueRR8[i].ArrivalTime < QueueRR8[position].ArrivalTime){
            position = i;
        }
    }

    for(int currentTime = QueueRR8[position].ArrivalTime, i = 0; numberOfProcesses!=0;){

        if(QueueRR8[i].CPUBurstTime <= 8 && QueueRR8[i].completed == false && QueueRR8[i].pushedToWaitingQueue == false){

            currentTime+= QueueRR8[i].CPUBurstTime ;
            QueueRR8[i].CPUBurstTime = 0;
            QueueRR8[i].completed = true;
        }
        else if(QueueRR8[i].CPUBurstTime > 8 && QueueRR8[i].completed == false && QueueRR8[i].pushedToWaitingQueue == false){
            QueueRR8[i].CPUBurstTime-=8;
            currentTime+=8;

            while(QueueRR8[i+1].ArrivalTime > currentTime && QueueRR8[i].CPUBurstTime!=0 && QueueRR8[i].CPUBurstTime%8 == 0){
                QueueRR8[i].CPUBurstTime-=8;
                currentTime+=8;
            }

            if(QueueRR8[i].CPUBurstTime == 0){
                QueueRR8[i].completed = true;
            }

            if(rr10Flag){
                numberOfProcesses--;
                QueueRR10[thirdQueueIndex].PID = QueueRR8[i].PID;
                QueueRR10[thirdQueueIndex].ArrivalTime = QueueRR8[i].ArrivalTime + 8;
                QueueRR10[thirdQueueIndex].CPUBurstTime = QueueRR8[i].CPUBurstTime;
                thirdQueueIndex++;
                QueueRR8[i].completed = true;
                QueueRR8[i].pushedToWaitingQueue = true;
            }
        }

        if(i == 3)
            if(QueueRR8[i].CPUBurstTime == 1){
                currentTime++;
                QueueRR8[i].CPUBurstTime = 0;
            }

        if(QueueRR8[i].CPUBurstTime == 0 && QueueRR8[i].pushedToWaitingQueue == false){
            numberOfProcesses--;
            QueueRR8[i].TurnAroundTime = currentTime - QueueRR8[i].ArrivalTime;
            QueueRR8[i].WaitingTime = QueueRR8[i].TurnAroundTime - originalBurstTimeSecond[i];
        }

        if(rr10Flag){
            if(QueueRR8[i+1].ArrivalTime <= currentTime){
                i++;
            }
            else{
                currentTime++;
            }
        }
        else{
            if(i == 4){
                i = 0;
            }
            else if(QueueRR8[i+1].ArrivalTime <= currentTime){
                i++;
            }
            else{
                i = 0;
            }
        }
    }

    printf("PID\tArrival Time\tCPU Burst Time\tWaiting Time\tTurnAround Time\n");
    for (int i = 0; i < 4; i++) {
        if(QueueRR8[i].pushedToWaitingQueue == false){
            printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", QueueRR8[i].PID, QueueRR8[i].ArrivalTime, originalBurstTimeSecond[i], QueueRR8[i].WaitingTime, QueueRR8[i].TurnAroundTime);
        }
    }

    averageTurnAroundTime=0;
    averageWaitingTime=0;
    for(int i =0; i < 4; i++){
        averageWaitingTime += QueueRR8[i].WaitingTime;
        averageTurnAroundTime += QueueRR8[i].TurnAroundTime;
    }

    double divisor;
    if(rr10Flag){
        divisor = 2.0;
    }
    else{
        divisor = 4.0;
    }

    printf("The average waiting time for this queue is %lf\n",averageWaitingTime/divisor);
    printf("The average turn-around time for this queue is %lf\n",averageTurnAroundTime/divisor);
}

void implementRoundRobin10(){
    int numberOfProcesses = 2;
    int fourthQueueIndex = 0;
    int position = 0;

    for(int i = 0; i < 2; i++){ //Storing original burst times for later display
        originalBurstTimeThird[i] = QueueRR10[i].CPUBurstTime;
    }

    for(int currentTime = QueueRR10[position].ArrivalTime, i = 0; numberOfProcesses!=0;){
        if(QueueRR10[i].CPUBurstTime <= 10 && QueueRR10[i].completed == false && QueueRR10[i].pushedToWaitingQueue == false){
            currentTime+= QueueRR10[i].CPUBurstTime ;
            QueueRR10[i].CPUBurstTime = 0;
            QueueRR10[i].completed = true;
        }
        else if(QueueRR10[i].CPUBurstTime > 10 && QueueRR10[i].completed == false && QueueRR10[i].pushedToWaitingQueue == false){
            if(fcfsFlag){
                numberOfProcesses--;
                QueueFCFS[fourthQueueIndex].PID = QueueRR10[i].PID;
                QueueFCFS[fourthQueueIndex].ArrivalTime = QueueRR10[i].ArrivalTime + 10;
                QueueFCFS[fourthQueueIndex].CPUBurstTime = QueueRR10[i].CPUBurstTime;
                fourthQueueIndex++;
                QueueRR10[i].completed = true;
                QueueRR10[i].pushedToWaitingQueue = true;
            }
            else{
                currentTime+=QueueRR10[i].CPUBurstTime;
                QueueRR10[i].CPUBurstTime = 0;
                QueueRR10[i].completed = true;
            }
        }

        if(QueueRR10[i].CPUBurstTime == 0){
            numberOfProcesses--;
            QueueRR10[i].TurnAroundTime = currentTime - QueueRR10[i].ArrivalTime;
            QueueRR10[i].WaitingTime = QueueRR10[i].TurnAroundTime - originalBurstTimeThird[i];
        }

        if(fcfsFlag){
            if(QueueRR10[i+1].ArrivalTime <= currentTime){
                i++;
            }
            else{
                currentTime++;
            }
        }
        else{
            if(i == 2){
                i = 0;
            }
            else if(QueueRR10[i+1].ArrivalTime <= currentTime){
                i++;
            }
            else{
                i = 0;
            }
        }
    }

    printf("PID\tArrival Time\tCPU Burst Time\tWaiting Time\tTurnAround Time\n");
    for (int i = 0; i < 2; i++) {
        if(QueueRR10[i].pushedToWaitingQueue == false){
            printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", QueueRR10[i].PID, QueueRR10[i].ArrivalTime, originalBurstTimeThird[i], QueueRR10[i].WaitingTime, QueueRR10[i].TurnAroundTime);
        }
    }

    averageTurnAroundTime=0;
    averageWaitingTime=0;
    for(int i = 0; i < 2; i++){
        averageWaitingTime += QueueRR10[i].WaitingTime;
        averageTurnAroundTime += QueueRR10[i].TurnAroundTime;
    }

    double divisor;
    if(fcfsFlag){
        divisor = 1.0;
    }
    else{
        divisor = 2.0;
    }

    printf("The average waiting time for this queue is %lf\n",averageWaitingTime/divisor);
    printf("The average turn-around time for this queue is %lf\n",averageTurnAroundTime/divisor);
}

void implementingFCFS(){
    int originalBurstTime = QueueFCFS[0].CPUBurstTime;
    int currentTime = 19;
    for(int i = 0; i < 1; i++){
        currentTime += QueueFCFS[i].CPUBurstTime;
        QueueFCFS[i].CPUBurstTime = 0;
        QueueFCFS[i].TurnAroundTime = currentTime - QueueFCFS[i].ArrivalTime;
        QueueFCFS[i].WaitingTime = QueueFCFS[i].TurnAroundTime - originalBurstTime;
    }


    printf("PID\tArrival Time\tCPU Burst Time\tWaiting Time\tTurnAround Time\n");
    for (int i = 0; i < 1; i++) {
        printf("%d\t\t%d\t\t%d\t\t%d\t\t%d\n", QueueFCFS[i].PID, QueueFCFS[i].ArrivalTime, originalBurstTime, QueueFCFS[i].WaitingTime, QueueFCFS[i].TurnAroundTime);
    }

    averageTurnAroundTime=0;
    averageWaitingTime=0;
    for(int i =0; i < 1; i++){
        averageWaitingTime += QueueFCFS[i].WaitingTime;
        averageTurnAroundTime += QueueFCFS[i].TurnAroundTime;
    }

    double divisor = 1.0;

    printf("The average waiting time for this queue is %lf\n",averageWaitingTime/divisor);
    printf("The average turn-around time for this queue is %lf\n",averageTurnAroundTime/divisor);
}
