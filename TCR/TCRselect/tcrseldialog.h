#ifndef TCRSELDIALOG_H
#define TCRSELDIALOG_H

#include <QDialog>
#include<QtMath>
#include"tcrbase.h"
#include<QRadioButton>
#include<QCheckBox>
#include"gloabaldefine.h"
#include<QLineEdit>
#include<QLabel>
#include<QGridLayout>
#include<QLineEdit>
#include<QLabel>
#include<QLayout>
#include<QKeyEvent>
#include<QBoxLayout>
#include<QHBoxLayout>
#include<QMessageBox>
#include<QGroupBox>


//TCR选择界面

namespace Ui {
class TCRselDialog;
}

class TCRselDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TCRselDialog(QWidget *parent = 0);
    ~TCRselDialog();

private:
    Ui::TCRselDialog *ui;
    QVector<TCRBase*> m_gpbase;
    QVector<QCheckBox*> m_gpresist;
    int currentBaseno;//first is 1
    QLineEdit *m_basename;
    QLabel *m_baselable;
    QPushButton *m_confirbutton;
    QPushButton *m_exitbutton;
    QVector<basestr> m_baseresult;
    QCheckBox* m_upboxs;
    QCheckBox* m_duboxs;
    QCheckBox* m_allboxs;

protected:
    void setRstate();
    void slotInitial();
public:
    QVector<basestr> getBasedetail();
    void InitialTest();

public slots:
    void setRFlag();//set single resitance state;
    void setButtonstate(int no);//set tcrbase button state;choose or not choose
    void measureResult();
    void setRall();//set all Resistance;
    void RefreshName(QString newname);
private:
    void setcheckbox(flag_t rstate);
    QGroupBox* Creativebasename();
    QGroupBox *CreativeT1();
    QGroupBox* CreativeT2();
    QGroupBox* CreativeT3();
    QGroupBox *CreativeT6();
    QGroupBox* CreativeResiatance();

signals:
    void baseRinfo(QVector<basestr> info);

};

#endif // TCRSELDIALOG_H
