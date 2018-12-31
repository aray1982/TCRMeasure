#ifndef PROBETESTDIALOG_H
#define PROBETESTDIALOG_H

#include<QGraphicsScene>
#include <QDialog>
#include"displayop.h"
#include<QStandardItemModel>
#include<LimeReport>
#include"gloabaldefine.h"
#include"Dataop/probedata.h"
#include<QDir>
#include<QPrintDialog>
#include<QTimer>
#include<QMessageBox>

//Probe 测试界面
namespace Ui {
class ProbeTestDialog;
}

struct dataserial{
    int measuretimes;
    int currentbaseno;
};


class ProbeTestDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProbeTestDialog(QWidget *parent = 0);
    ~ProbeTestDialog();
    void SetData(int i,qreal* data);//获取数据地址

private slots:
    void on_Pmeasure_pushButton_clicked();

    void on_Plate_comboBox_currentIndexChanged(int index);

    void on_Exit_pushButton_clicked();

    void on_sub1_pushButton_clicked();

    void on_add1_pushButton_clicked();

    void on_sub10_pushButton_clicked();

    void on_add10_pushButton_clicked();

    void hideEvent(QHideEvent *event);

    void showEvent(QShowEvent *event);

    void on_Print_pushButton_clicked();

private:
    Ui::ProbeTestDialog *ui;
    QGraphicsScene* m_Scene;
    DisplayOp *m_Screen;
    QStandardItemModel *m_Model;
    dataserial m_dats;//record times and current no;
    LimeReport::ReportEngine m_preport;
    Probedata *m_pdataop;
    bool Isrecord;
    qreal* T4data;
    qreal* T5data;
    int currentT_id;
    qreal calidata[10];
    QPrintDialog *m_pdialog;
    int intervalTime;//Probe电阻测量间隙
    QMessageBox *m_warningBox;//警告TCR运行弹框
    QMessageBox *m_recordBox;//记录判断弹框
    bool isTCRnotify;
    int currentplateNo;


signals:
    void Psmeasure(int plate,bool Isfirst);//start probe measure
    void closeDialog();
    void triggerDMM(int plate ,int counttimes);//触发DMM,废弃？
    void triggerDMM(int plate,int counttimes,short Moption);//触发DMM,基片名称，次数，及测量优化项

private:
    void Initial();//初始化
    void RecordData(int i);//记录数据
    qreal Calibratecalculate(qreal Rr,qreal bias);//校准计算
    short recongnitionName(const QString &basename);//将输入名称转换为数字代码，方便测量进行优化
    void ButtonAccess(bool IsEnable);//禁用或启用相关按键

public slots:
    void SetRlamp(int rno,bool switches,int plateno);//指示灯处理，触发DMM指令
    void finishDatadeal(int plateno);//最终数据处理
    void Setcurrendata(QVector<QVariant> pbdata,bool isExist);
    void DisplayError(QString Error);
    void errorHandler(const QString &info);
    void setCalidata(qreal* data);
    void banTest();//禁用测试
    void accessTest();//启用测试
};

#endif // PROBETESTDIALOG_H
