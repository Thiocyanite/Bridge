#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/sem.h>
#include <stdbool.h>


struct sembuf semaforek, kierunek;
int sid[2];

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
    semop(sid[0], &semaforek, 1);
    printf("Samochod o numerze %d i wadze %d wjezdza\n", samochod1.id, samochod1.masa);
    sleep(4);
    printf("Samochod o numerze %d i wadze %d wyjezdza\n", samochod1.id, samochod1.masa);
    semaforek.sem_op=samochod1.masa;
    semop(sid[0], &semaforek,1);
    printf("Samochod o numerze %d i wadze %d wyjechal\n", samochod1.id, samochod1.masa);
    kierunek.sem_op=1;
    semop(sid[1], &kierunek,1);
    exit(0);

}



int main() {
    srand(time(NULL));
    sid[0] =semget(0x123, 1, 0600| IPC_CREAT);
    sid[1] =semget(0x124, 1, 0600| IPC_CREAT);
    if (sid[0] == -1 || sid[1]==-1){
        perror("semafor");
        exit(1);
    }
    semctl(sid[0],0,SETVAL,20);
    semaforek.sem_num=0;
    semaforek.sem_flg=0;

    semctl(sid[1],0, SETVAL,0);
    kierunek.sem_num=0;
    kierunek.sem_flg=0;

    struct samochod probne[20];
    int i=0;
    int *stat;


    while (1) {
        printf("Z lewej na prawa\n");
        for (i = 0; i < 20; i++) {
            probne[i] = inicjujauto(i);
            if ((fork()) == 0) {
                jedz(probne[i]);
            }
        }
        kierunek.sem_op = -20;
        semop(sid[1], &kierunek, 1);
        printf("Z prawej na lewa\n");
        for (i = 0; i < 20; i++) {
            probne[i] = inicjujauto(i);
            if ((fork()) == 0) {
                jedz(probne[i]);
            }
        }
        kierunek.sem_op = -20;
        semop(sid[1], &kierunek, 1);
    }
    return 0;
}