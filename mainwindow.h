#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"tcrsettingdialog.h"
#include"gloabaldefine.h"
#include"tcrseldialog.h"
#include"tcrmeasuredialog.h"
#include"probetestdialog.h"
#include"dmm.h"
#include"switch.h"
#include"dataop.h"
#include"tcrdatadialog.h"
#include"calibratedialog.h"
#include<QFile>
#include<QDir>
#include"LimeReport"

//主操作界面

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_TCRsetting_pushButton_clicked();

    void on_TCRmeasure_pushButton_clicked();

    void on_Probe_pushButton_clicked();

    void on_MainExit_pushButton_clicked();

    void on_DataQuery_pushButton_clicked();

    void on_Cali_pushButton_clicked();



private:
    Ui::MainWindow *ui;
    TCRsettingDialog *m_TCRSettingDialog;
    TCRselDialog *m_TCRselDialog;
    TCRmeasureDialog *m_TCRMDialog;
    ProbeTestDialog *m_ProbeDialog;
    Dataop *m_dataop;
    TCRdataDialog *m_datadialog;
    CalibrateDialog *m_caldialog;
    Dmm *m_dmm;
    Switch *m_switch;
    LimeReport::ReportEngine m_report;





private:
    void TCRininitial();//TCR初始化
    bool Dataqueryintial();//数据查询初始化
    void enableTCRmeasure();
    void Proebeinitial();//探针卡初始化
    void CalibrateInitial();//校准初始化
    void ThreadInitial();//NI进程初始化
    bool ConfigFileinfoDeal(qreal *coef);//修正文件处理
    bool errorLogger(const QString &info);//故障记录文件处理
    void errorHandlerInitial();//故障处理初始化

public slots:
    void setTCRresult(QDateTime st,QDateTime et);//query result1
    void setTCRresult(QString name);//query result2
    void setProberesult(QDateTime st,QDateTime et);
    void setProberesult(QString name);
    void deleteData(int T_id, Tabletype type);
    void saveCalibradata(QVector<qreal> tempdata);
    void errorTransimit(QString info,int dialogNo);
    void accessTCRMbutton(const bool &switches);


signals:


private:
    QThread m_Dthread;//DMM进程
    QThread m_Sthread;//SWITCH进程
    //follw as debug;
    //void Regtest();



};

#endif // MAINWINDOW_H
