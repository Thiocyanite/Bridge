#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdbool.h>


struct sembuf semaforek;
int sid;

struct samochod{
    int masa;
    int id;
};

struct samochod inicjujauto(int liczba){
    int Ciezary[]={1,2,3};
    struct samochod self;

    self.masa=Ciezary[rand()%3];
    self.id=liczba;
    return self;
}

void jedz(struct samochod samochod1){
    printf("Samochod o numerze %d i wadze %d chce wjehac\n", samochod1.id, samochod1.masa);
    semaforek.sem_op=-(samochod1.masa);
    semop(sid, &semaforek, 1);
    printf("Samochod o numerze %d i wadze %d wjezdza\n", samochod1.id, samochod1.masa);
    sleep(4);
    printf("Samochod o numerze %d i wadze %d wyjezdza\n", samochod1.id, samochod1.masa);
    semaforek.sem_op=samochod1.masa;
    semop(sid, &semaforek,1);
    printf("Samochod o numerze %d i wadze %d wyjechal\n", samochod1.id, samochod1.masa);

}



int main() {
    srand(time(NULL));
    sid=semget(0x123, 1, 0600| IPC_CREAT);
    if (sid == -1){
        perror("semafor");
        exit(1);
    }
    semctl(sid,0,SETVAL,15);
    semaforek.sem_num=0;
    semaforek.sem_flg=0;


    struct samochod probne[9];
    int i=0;
    int *stat;

    printf("Z lewej na prawa\n");
    for (i=0;i<9;i++){
        probne[i]=inicjujauto(i);
        if ((fork())==0){
            jedz(probne[i]);
        }
    }
    wait(stat);
    printf("Z prawej na lewa\n");
    for (i=0;i<9;i++){
        probne[i]=inicjujauto(i);
        if ((fork())==0){
            jedz(probne[i]);
        }
    }
    wait(stat);

    return 0;
}