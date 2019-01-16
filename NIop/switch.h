#ifndef SWITCH_H
#define SWITCH_H

#include <QObject>
#include"NIinclude/niswitch.h"
#include <stdlib.h>
#include <stdio.h>
#include<QVector>
#include"gloabaldefine.h"

#include<QStringList>
#include"TCR/TCRselect/tcrseldialog.h"

//NI switch 操作类

struct TCRsetting{
    short currentNo;
    short delaytime;
    basestr currentbase;
    QVector<short> cbasechannle;
    short T3count;

};
#define niSwitchCheckErr(fnCall)    if( switchError = (fnCall), switchError<VI_SUCCESS){goto Error;}  else

void niSwitch_ErrorHandler(ViSession session, ViStatus error);




class Switch : public QObject
{
    Q_OBJECT
public:
    explicit Switch(QObject *parent = nullptr);
    struct SwitchInfo{
        ViChar    resourceName[256];
        ViConstString topology;
    };

signals:
    //void StartingMeasure(int ResistanceNo);//abandon
    void StartingMeasure(int plate,int Resistancno);//singletest
    void ProbelampIndicator(int Rno,bool switcher,int plate);//Probetest
    void sendError(QString errorinfo,int plateno);

    //TCR
    void TCRlampIndicator(short baseno,short rno);//指示当前测试片状态
    void TCRstartmeasure(short Bplate,short br);//通知DMM开始测量
    void TCRcyclefinish();//周期结束
    void TCRsegFmeasure();//单独试片完成




private:
    void niSwitch_ErrorHandler(ViSession session, ViStatus error);
    void InitialChannel();

    //TCR
    void InitialTCR();
    void InitialTempreport();
    bool Tcrautoincr(short &sn);//TCR increase，基片结束后自增;
    void Allswitchclose();//关闭所有开关卡进程


public:
    void Close(ViSession vi);//
    void SetMeasureInfo(int plate, int base);

    //TCR
    QVector<short> getchannelNo(flag_t rinfo);//TCR转换信息为具体通道号码

public slots:
    void ReadyMeasure(const QVector<short> &tempDatainfo );//临时打印信息，转发TCRMeasureplus
    void TCRMeasureplus(bool istempfirst);//TCR测试
    void reciveInfo(QVector<basestr> info);

    void CalibrMeasure(int testplate,bool Isfirst);//校准测试
    void ProbeMeasure(int testplate,bool Isfirst);//探针卡测试

private:
    //通用信息
    ViSession Pxie2575;
    ViSession Pxie2525;
    ViSession Pxie2526;
    ViStatus error;
    SwitchInfo PXI1Slot4,PXI1Slot5,PXI1Slot6;
    QList<QStringList> T3channel;//T3 channels
    QList<QStringList> T6channel;
    QString  T1channel[6][10];//T1 channels
    QString  T2channel[6][10];//T2 channels
    QStringList T4channels;
    QStringList T5channels;
    QVector<QStringList> T2info;//TCR test info
    QVector<QStringList> T3info;//TCR test info
    //QVector<basestr> TCRinfo;
    //TCR相关
    TCRsetting m_tcrs;
    QVector<basestr> TCRinfo;
    //bool IsTCR;//判断当前是否是TCR测试
    TestType currentTesttype;//当前测试类型

    //单次测量
    int counttimes;
    int PlateNo;
    int BaseNo;
    short serialNo;//TCRsequece
    QVector<short> tempreportdata;//临时报表信息
    bool IstempTest;//临时报表测试
//follow as debug function:
    int errorcount;
signals:
    void segFmeasure();//单片完成

    void cyclefinish();//循环结束
    //void ProbeFinish(int plate);
    void CalibraFinish(int plate);
public slots:
    void Probtest();

};

#endif // SWITCH_H
