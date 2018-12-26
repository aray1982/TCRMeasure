#ifndef TCRSETTINGDIALOG_H
#define TCRSETTINGDIALOG_H

#include <QDialog>
#include"gloabaldefine.h"

//TCR设定类

struct TCRsettings{
    qreal SV1=-55;
    qreal SV2=25;
    qreal SV3=125;
    qreal CalV1=-55;
    qreal CalV2=25;
    qreal CalV3=125;
    int steadyTime1=16;
    int steadyTime2=10;
    int Dovertime=300;
    int Uovertime1=120;
    int Uovertime2=120;
    int Pbdelaytime=1;//禁用外部测试时间
    bool Isprint=true;

};

namespace Ui {
class TCRsettingDialog;
}

class TCRsettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TCRsettingDialog(QWidget *parent = NULL);
    ~TCRsettingDialog();

private:
    Ui::TCRsettingDialog *ui;
private:
    void Initial();
public:
    TCRsettings m_settings;
signals:
    void sendTCRsetting(TCRsettings setting);
private slots:
    void on_Tempsave_pushButton_clicked();
    void on_Stimesave_pushButton_clicked();
    void on_otimesave_pushButton_clicked();
    void on_close_pushButton_clicked();
    void on_printsave_pushButton_clicked();
};

#endif // TCRSETTINGDIALOG_H
