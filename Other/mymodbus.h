#ifndef MYMODBUS_H
#define MYMODBUS_H

#include <QObject>
#include<QtCore>
#include<QModbusClient>
#include<QModbusDataUnit>
#include<QModbusTcpClient>
#include<QModbusRtuSerialMaster>
#include <QObject>
#include <QEventLoop>
#include<QTimer>
#include<QSerialPort>
#include<QDebug>
#include"gloabaldefine.h"

//MODBUS连接类
struct ModBSettings {
    int parity = QSerialPort::NoParity;
    int baud = QSerialPort::Baud9600;
    int dataBits = QSerialPort::Data8;
    int stopBits = QSerialPort::OneStop;
    int responseTime = 5000;
    int numberOfRetries = 2;
#ifdef DEBUG_NI
     QString port="com3";
#else
     QString  port="com1";
#endif


};
/*
struct DataQueue{
        short state;//state:0 read,state:1 write sv,state 2: stop,state 3:run
        float SV;
};

*/
class Mymodbus : public QObject
{
    Q_OBJECT
public:
    explicit Mymodbus(QObject *parent = nullptr);

signals:


    void sendError(const QString &errorinfo);//故障处理
    void readfine(int,float);//回传信息号
    void sendPvalue(qreal value);//传送检测值
    void connectstate(bool state);

public slots:
    //void ChamberRead();//设备连读
    void ChamberRead(const int &i,const float &value);//设备连读
    void startNext();//发送下一次信号
    bool MBconnect();//连接MODBUS
    void readReady();//读取处理
    void writefinishReady();//写入完成处理
    void setRun();//启动连接
    void stopRun();//停止连接

private:
    QModbusClient *m_modbus;
    //QQueue<DataQueue> m_Serqueue;
    ModBSettings m_MBSetting;

    bool Isrun;//启动modbus
    //bool IssetSV;//设定SV值
    //float tempSvaule;//临时设定值
    QMutex mutex;
};

#endif // MYMODBUS_H
