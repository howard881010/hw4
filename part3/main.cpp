#include"mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
#include "stdlib.h"
#include "string.h"
#include <math.h>
PwmOut pin5(D5), pin6(D6);
Ticker servo_ticker;
BufferedSerial pc(USBTX,USBRX);
BufferedSerial uart(D1,D0);
BufferedSerial xbee(A1, A0);

BBCar car(pin5, pin6, servo_ticker);

xbee.set_baud(9600);
uart.set_baud(9600);
Thread t1;
Timer t;
EventQueue queue(64 * EVENTS_EVENT_SIZE);
char recvall[30], tmp[20];
int yes = 1;

void April();
void RD();

//parallax_ping  ping10(pin10);
int main() {
    char recv[1];

    t1.start(callback(&queue, &EventQueue::dispatch_forever));
    queue.call(April);
    int now;
    while(1) {
        if(uart.readable()) {
            uart.read(recv, sizeof(recv));
            if(recv[0] == '(') {
                now = 0;
            } else if(recv[0] == ')') {
                tmp[now] = recv[0];
                if(yes) {
                    strcpy(recvall, tmp);
                }
                strcpy(tmp, "");
            } else if (recv[0] != ',') {
                tmp[now++] = recv[0];
            }
        }
    }
}

void April() {
    char n[2][5];
    int tx, angle;
    int count = 0;
    int len;
    int turn = 0;
    float r;
    char buff[25];
    float dis, val;
    while(1) {
        yes = 0;
        int i = 0;
        int j = 0;
        count = 0;
        while(recvall[i] != ')') {
            if (recvall[i] == ' ') {
                j++;
                count = 0;
                i++;
            }
            n[j][count++] = recvall[i];
            i++;
        }
        tx = atoi(n[0]);
        angle = atoi(n[1]);
        len = strlen(recvall);

        for (i = 0; i < 2; i++) {
            for (j = 0; j < 5; j++) {
                n[i][j] = '\0';
            }
        }
        for (i = 0; i < 30; i++) {
            recvall[i] = '\0';
        }
        yes = 1;
        if (angle > 3 && angle < 40) {
            turn = 1;
            car.goStraight(30);
        } else if (angle < 357 && angle > 320) {
            turn = 2;
            car.goStraight(30);
        } else if (len == 0) {
            printf("NO\n");
            if (turn == 2) {
                car.turn(30,1);
                ThisThread::sleep_for(75ms);
                car.stop();
            } else if (turn == 1) {
                car.turn(-30,1);
                ThisThread::sleep_for(75ms);
                car.stop();
            }
        } else {
            if (tx <= -2) {
                car.turn(30,1);
                turn = 0;
            } else if (tx >= 2) {
                car.turn(-30,1);
                turn = 0;
            } else {
                car.stop();
                ThisThread::sleep_for(500ms);
                strcpy(buff, "");
                /*dis = ping10;
                sprintf(buff, "distance %.2f, angle = %d\r\n", (float)ping1, angle);
                xbee.write(buff, sizeof(buff));*/
                sprintf(buff, "angle = %d\r\n", angle);
                xbee.write(buff, sizeof(buff));
                ThisThread::sleep_for(100ms);
            }
        }
        ThisThread::sleep_for(150ms);
    }
}
