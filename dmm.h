#ifndef DMM_H
#define DMM_H
#include"NIinclude/nidmm.h"
#include <QObject>
#include<QMessageBox>
#include<QTimer>
#include"gloabaldefine.h"
//NI DMM 操作类




class Dmm : public QObject
{
    Q_OBJECT
public:
    explicit Dmm(QObject *parent = nullptr);



signals:
private:
    ViChar    resourceName[256];//设备硬件名称     = "Dev1";
    ViSession vi               ;//设备进程     = VI_NULL;
    ViBoolean idQuery          ;//设备硬件名称 Verifies that the device you initialize is one that the driver supports.
                                 //-DMM automatically performs this query, so setting this parameter is not necessary.
    ViBoolean reset            ;//Specifies whether to reset the instrument during the initialization procedure.
    ViStatus  error            ;//结果标识      = VI_SUCCESS;
    ViInt32   measurementType  ;//测量类型      = NIDMM_VAL_DC_VOLTS;
    ViReal64  powerlineFreq    ;//电源频率      = NIDMM_VAL_60_HERTZ;
    ViReal64  range            ;//测量范围      = 10.00;
    ViReal64  resolution       ;//测量分辨率      = 5.5;
    ViReal64  absoluteResolution  ;//设备硬件名称   = 0.000;
    ViReal64  reading             ;//读取数值   = 0.000;

    int PlateNo;
    int BaseNo;
    int tempBaseNo;
    int tempRNo;
    int delaytime;
    int TCRT2range,TCRT3range;

    short TCRBaseNo;//TCR Test current base
    short TCRRno;//TCR Test current Resistance
    short PCchooseNo;//Probe calibrate choose;
    TestType m_currentType;
    bool forbidError;//禁用故障报错，主要针对超量程
    QVector<short> Lrflag;//低阻判别码集合
    bool isoffset;//启动offset测量标志
public:
    qreal T3measureData[6][10];
    qreal T1measureData[6][10];
    qreal T2measureData[6][10];
    qreal T4measureData[10];
    qreal T5measureData[10];
    qreal T6measureData[6][10];
private:
    void Errorhandler(ViSession vi,ViStatus error);
    bool ResultJud(ViStatus status,ViSession vi,ViStatus&error);

    void RecordResults(int RNo,qreal value);
    void RecordResults(qreal value);//TCR
    void Initial();


public:

    arrData* GetData(int Plate);
    qreal *GetT4T5Data(int Plate);
    void Set2Wire();
    void Set4Wire();
    void SetAutoRange();
    void SetRange(qreal rangee);
    void SetResolution(qreal digitalResolution);
    void SetMeasureInfo(int plate, int base);
    void setdelaytime(int i);
    void SetProbe();//设定为Probe测试状态
    void SetCalibrate();//设定为Cali测试状态


public slots:

    bool TCRMeasure();
    bool ProbMeasure();
    bool CalibraMeasure();

    void ReadyMeasure(short BaseNo,short Rno);//tcr测试
    void ReadyMeasure(int plate,int resistancno);//Calibra测试
    void ReadyMeasure(int plate,int resistancno, short Moption);//probe优化测试

    void TCRrange(int T2range,int T3range);//设定TCR测量量程，分别对应四线制，两线制
    void RecieveTCRinfo();//接收从TCRM来的主板编号
signals:

    void sendError(const QString &Erroinfo,int plateNo);
    void MeasureFinish(int plate, bool Isfirst);//Probe 测试完成;
    void MeasureFinish(bool istempfirst);//TCR测量完成
    void CaliMeasureFinish(int plate,bool Isfirst);//校准测量完成


};


#endif // DMM_H
