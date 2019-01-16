#ifndef GLOABALDEFINE_H
#define GLOABALDEFINE_H
#define DEBUG_NI
#define setbit(x,y) x|=(1<<y) //置位操作
#define clrbit(x,y) x&=~(1<<y) //清零操作
#define cpare(x,y) ((x&(1<<y))==qPow(2,y)) //比较操作
#define cpare1(x,y) (x&(1<<y))
#include<QObject>
typedef unsigned short flag_t;
using arrData=qreal[10];
struct basestr{
    int sno;//Positon
    QString name;//base name
    flag_t rstate;//Resistance state
    int dataid;
};
//static const QString verisonno="1.1.4";
enum TestType{TCR,Probe,Calibrate};
enum Measureoptions{Lr,Mr,Hr,Nr};
#pragma execution_character_set("utf-8")
//#include<QFile>
//#include<QDebug>
#endif //GLOABALDEFINE_H
//全局定义函数及操作
//DEBUG_NI 定义为调试状态，NI SWITCH 和 DMM 及串口提供调试状态



