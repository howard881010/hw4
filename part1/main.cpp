#include "mbed.h"
#include "bbcar.h"
#include "bbcar_rpc.h"
Ticker servo_ticker;
PwmOut pin5(D5), pin6(D6);
BBCar car(pin5, pin6, servo_ticker);
BufferedSerial xbee(D1, D0);
BufferedSerial pc(USBTX, USBRX);

Thread t;
EventQueue queue(32 * EVENTS_EVENT_SIZE);

void RP(Arguments *in, Reply *out);
RPCFunction reverp(&RP, "RP");


int main() {
    char buf[256], outbuf[256];
    FILE *devin = fdopen(&xbee, "r");
    FILE *devout = fdopen(&xbee, "w");
    while (1) {
        memset(buf, 0, 256);
        for( int i = 0; ; i++ ) {
            char recv = fgetc(devin);
            if(recv == '\n') {
                break;
            }
            buf[i] = fputc(recv, devout);
        }
        RPC::call(buf, outbuf);
    }
}

void RP (Arguments *in, Reply *out)   {
    bool success = true;

    double d1 = in->getArg<double>();
    double d2 = in->getArg<double>();
    char dir = in->getArg<char>();

    if (dir == 'SOUTH') {
        car.turn(110, 0.3);
        ThisThread::sleep_for(1500ms);
        car.stop();
        ThisThread::sleep_for(500ms);
    } else if (dir == 'NORTH') {
        car.turn(110, 0.3);
        ThisThread::sleep_for(800ms);
        car.stop();
        ThisThread::sleep_for(500ms);
    } else if (dir == 'EAST') {
        car.turn(110, 0.3);
        ThisThread::sleep_for(1000ms);
        car.stop();
        ThisThread::sleep_for(500ms);
    }

    double velocity = 16.3;
    double time1 = 1000*(d1 + 13.5) / velocity;
    double time2 = 1000*(d2 + 6.2) / velocity;

    car.goStraight(-110);
    ThisThread::sleep_for(time1);
    car.stop();
    ThisThread::sleep_for(500ms);
    car.turn(110, 0.3);
    ThisThread::sleep_for(500ms);
    car.stop();
    ThisThread::sleep_for(500ms);
    car.goStraight(-110);
    ThisThread::sleep_for(time2);
    car.stop();
}
