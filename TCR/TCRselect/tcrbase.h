#ifndef TCRBASE_H
#define TCRBASE_H

#include <QWidget>
#include<QPushButton>
#include"gloabaldefine.h"
#include<QCheckBox>
#include<QGroupBox>

//TCR选择界面，基板操作类

class TCRBase : public QWidget
{
    Q_OBJECT
public:
    explicit TCRBase(QWidget *parent = nullptr,QString name ="TA",int No=0);
    void setResistance(int i); //set point r;
    void clearResistance(int i);//clear point r;
    void setchoosed();//jud choosed
    void setbasename(QString name);//set basename
    bool isChoosed();
    flag_t Rstate();
    void clickcancelstate();//取消定制状态
    QPushButton* thebutton();//return button
    QString returnbasename();
    void InitialBase();

signals:
    void clicksure(int i);

public slots:
    void on_basebutton_clicked();
private:
    void clicksurestate();



private:
    QPushButton *m_Button;
    flag_t m_resistancestate;
    bool m_chooseble;
    bool m_clickstate;
    QString m_basename;
    QString m_lable;
    QCheckBox *m_stateindicator;
    int m_no;
};

#endif // TCRBASE_H
