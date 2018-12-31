#ifndef TCRMEASUREDIALOG_H
#define TCRMEASUREDIALOG_H

#include <QDialog>
#include "TCR/TCRsetting/tcrsettingdialog.h"
#include<QQueue>
#include<QTimer>
#include<QModbusClient>
#include<QModbusDataUnit>
#include<QModbusTcpClient>
#include<QModbusRtuSerialMaster>
#include<QSerialPort>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QButtonGroup>
#include<QGraphicsScene>
#include<QGraphicsView>
#include"progressview.h"
#include "Dataop/tcrdataop.h"
#include"gloabaldefine.h"
#include<QDir>
#include<QMessageBox>
#include <QAxObject>
#include<LimeReport>
#include"Other/mymodbus.h"
#include<QPrinter>
#include<QPrintDialog>


//TCR 测量界面类

//TCR 设定
struct TCRsettings;

//数据进程
namespace Ui {
class TCRmeasureDialog;
}

class TCRmeasureDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TCRmeasureDialog(QWidget *parent = 0);
    ~TCRmeasureDialog();

private:
    Ui::TCRmeasureDialog *ui;
    TCRsettings m_settings;

    short m_stagecounter;//TCR进程计数
    QButtonGroup* m_btGroup;//TCR 按钮状态组
    QTimer *m_steadytime;//安定时间计时
    QTimer *m_errortime;//故障时间计时
    QTimer *m_probebantime;//探针测量禁用计时
    ProgressView *m_view;
    bool Isrunning;//是否运行

    bool Istest;//是否测试前确认
    short m_progressstage;//progressview 进程
    bool Iscreatedata;//是否创建了数据
    bool applyCalibrate;//修正应该用标志
    qreal coefficientdata[2];//修正系数

    int intervalDealytime;//基片之间间隙时间
    int intervalRDelaytime;//电阻之间间隙时间
    TCRdataop *m_tdataop;
    QVector<basestr> m_tcrbaseinfo;
    LimeReport::ReportEngine m_report;
    QVector<short> tempreportInfo;//临时报表基片信息



private:

    void Initial();//初始化
    void ButtonTextcolor(bool settingflag);//背测基片颜色标识

    void SetTCRButton(int no);//设定TCRButton
    void setTCRButtonplus(int no);//设定测试基片颜色
    void StopTCRtest(bool stopMb);
    void RefreshafterTest();//刷新TCR测试界面
    void MbStopoperation();//停止设备操作
    void RefreshProgress();//停止前刷新相关进程
    bool RecordData();//记录TCR数据
    bool Finaldeal();//为数据库添加时间标记，计算TCR系数
    QVector<short> getchannelNo(flag_t rinfo);
    void displaytempReport();

    bool createdData();//测试开始前生成数据库
    bool deleteData();//测试失败删除数据库
    void finalPrintData();//测试结束打印TCR结果
    void ModbusErrorDeal(QString errorinfo);//modbus 处理



public:
    void setData(int i,arrData* data);//指定数组地址
    void errorHandler(const QString &info);//故障处理
    void sendProbeban();//发送禁用信息
public slots:
    void CycleDeal();//循环处理
    void GetTCRsetting(TCRsettings setting);//获取TCRsetting
    void Overtimedeal();//超时处理
    void MeasureReady(bool issuccessful);//测试前确认
    void MbStopFinish();
    void Cyclefinish();//1周期结束处理
    void intervalDeal();//中断处理
    void setTCRlampindicator(short blate,short rno);//TCR指示灯
    void reciveInfo(QVector<basestr> info);//接收基板信息
    void delayProbeban();//Probe禁止警告延时
    void MBconnectInfo(bool state);//开机modbus连接提示
    void setCalibrate();
    void setCoefficient(qreal* data);
    void BeginMeasure();//TCRMeasure
signals:
    void starmeasure(bool istempfirst);//TCR start measure;
    void sendError(QString error);//send error
    void setDMMrange(int T2range,int T3range);//set rangee abandon?
    void setBaseDelay(int time);//set delay time
    void endcycleNotion();//cyclye end 通知TCRSel初始化
    void confirInfo();
    void banProbe();//禁用探针测量
    void accessProbe();//启用探针测量
    void stmeasuretoDMM(short pl,short rno);//通知DMM开始测量
    void transModbusError(const QString &errorinfo,const  int &state);
    void accessTCRM(const bool &switches);
    void sendTempmeasure(const QVector<short> &tempdata);

protected:
    //QModbusClient *m_modbusDevice;
protected:
    void showEvent(QShowEvent *event);

private slots:

    void on_TMExit_pushButton_clicked();
    void on_Execute_pushButton_clicked();
    void on_Confirm_pushButton_clicked();





public:
    arrData* T1data;
    arrData* T2data;
    arrData* T3data;
    arrData* T6data;
//follow realted Modbus
signals:
    void readfine(int i,float value);
    void startMb();
    void StopMb();
private:
    QThread m_Modbusthread;
    Mymodbus *m_modbus;
    bool m_modbusRun;
    void ModbusInit();
public slots:
    //void setPvaule(float value);
    void valueProcessDeal(const float &value);
    void finalStop();//测试结束停止

//follow related data;
private:
    QThread m_Datathread;
    void DatathreadInit();


};

#endif // TCRMEASUREDIALOG_H
