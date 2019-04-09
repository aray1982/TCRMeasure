#ifndef TCRDATADIALOG_H
#define TCRDATADIALOG_H

#include <QDialog>
#include<QDateTime>
#include"calanderdialog.h"
#include"gloabaldefine.h"
#include<LimeReport>
#include"Dataop/dataop.h"
#include<QPrinter>
#include<QPrintDialog>

//数据查询界面类

namespace Ui {
class TCRdataDialog;
}

class TCRdataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TCRdataDialog(QWidget *parent = 0);
    ~TCRdataDialog();
private slots:



    void on_St_pushButton_clicked();

    void on_Et_pushButton_clicked();

    void on_Query_pushButton_clicked();

    void on_Display_pushButton_clicked();

    void on_PSt_pushButton_clicked();

    void on_PEt_pushButton_clicked();

    void on_PQuery_pushButton_clicked();

    void on_PDisplay_pushButton_clicked();

    void on_delete_pushButton_clicked();

    void on_pdelete_pushButton_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_p_tableWidget_cellClicked(int row, int column);

    void on_pdf_pushButton_clicked();

    void on_Ppdf_pushButton_clicked();

    void showEvent(QShowEvent *event);



private:
    Ui::TCRdataDialog *ui;
    QDateTime starttime;
    QDateTime Endtime;
    CalanderDialog *m_Cdialog;
    LimeReport::ReportEngine m_report;
    bool applyCalibrate;
    qreal coefficientdata[2];
    LimeReport::PreviewReportWidget *m_preiew;

    QPrinter m_printer;

protected:
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void TCRTimeinfo(QDateTime st,QDateTime et);
    void TCRNameinfo(QString name);
    void ProbeTimeinfo(QDateTime st,QDateTime et);
    void ProbeNameinfo(QString name);
    void deleteData(int T_id,Tabletype type);
    void calculateATCR(int T_id);
public slots:
    void setTabelData(QList<QStringList> result);
    void setCalibrate();
    void setCoefficient(qreal* data);
    void setATCR(double *ATCR,int T_id);

    //follow as tested

};

#endif // TCRDATADIALOG_H
