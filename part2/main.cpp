#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "stdlib.h"
#include "string.h"
#include <math.h>
PwmOut pin5(D5), pin6(D6);
Ticker servo_ticker;
BufferedSerial pc(USBTX,USBRX); //tx,rx
BufferedSerial uart(D1,D0); //tx,rx

BBCar car(pin5, pin6, servo_ticker);

Thread t1;
EventQueue queue(32 * EVENTS_EVENT_SIZE);

char recvall[20], tmp[20];
int cur;
int yes = 1;

void Correct();

int main() {
    char recv[1];
    uart.set_baud(9600);
    t1.start(callback(&queue, &EventQueue::dispatch_forever));
    queue.call(Correct);
    while(1) {
        if(uart.readable()) {
            uart.read(recv, sizeof(recv));
            if(recv[0] == ')') {
                tmp[cur] = recv[0];
                if(yes) {
                    strcpy(recvall, tmp);
                }
                strcpy(tmp, "");
            } else if (recv[0] == '(') {
                cur = 0;
            } else if (recv[0] != ',') {
                tmp[cur++] = recv[0];
            }
        }
    }
}

void Follow() {
    char arr[4][4];
    int i, j;
    int count = 0;
    int dx, dy;
    int deg;
    float a = 2.3;
    float r;
    int temp;

    while(1) {

        int x1 = atoi(arr[0]);
        int y1 = atoi(arr[1]);
        int x2 = atoi(arr[2]);
        int y2 = atoi(arr[3]);
        yes = 0;
        i = 0;
        j = 0;
        count = 0;


        while(recvall[i] != ')') {
            if (recvall[i] == ' ') {
                j++;
                count = 0;
                i++;
            }
            arr[j][count] = recvall[i];
            count++;
            i++;
        }
        if (y1 > y2) {
            temp = x1;
            x1 = x2;
            x2 = temp;
        }
        yes = 1;

        for (i = 0; i < 20; i++) {
            recvall[i] = '\0';
        }

        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++) {
                arr[i][j] = '\0';
            }
        }

        dx = x1- x2;
        dy = abs(y1 - y2);

        r = (float) sqrt(dx*dx + dy*dy);

        if (r == 0) {
            printf("stop\n");
            car.stop();
        }
        else {
            if (x1 > 92) {
                printf("LEFT\n");
                car.turn(-35, 1);

            }
            else if (x1 <68) {
                printf("RIGHT\n");
                car.turn(35, 1);

            } else {
                printf("Go-straight\n");
                car.goStraight(35);
            }
        }
        ThisThread::sleep_for(100ms);
    }

}
