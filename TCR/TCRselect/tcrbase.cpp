#include "tcrbase.h"
#include <QGridLayout>
#include<QEvent>
#include<QKeyEvent>
#include<QDebug>

TCRBase::TCRBase(QWidget *parent,QString name,int No) : QWidget(parent),
    m_Button(new QPushButton(this)),
    m_basename(""),
    m_lable(name),
    m_chooseble(false),
    m_clickstate(false),
    m_resistancestate(0),
    m_stateindicator(new QCheckBox(this)),
    m_no(No)

{
    m_Button->setText(m_lable);
    QGridLayout *m_lat1=new QGridLayout;
    m_stateindicator->setStyleSheet("QCheckBox::indicator {width:16px;height: 16px;}"
                                    "QCheckBox::indicator:checked { image: url(:/QCheckboxPic/sc16); }"
                                    "QCheckBox::indicator:unchecked{ image: url(:/QCheckboxPic/suc16); }");
    m_stateindicator->setEnabled(false);

    m_lat1->addWidget(m_stateindicator,0,0);
    m_lat1->addWidget(m_Button,1,0);
    this->setLayout(m_lat1);
    m_Button->setObjectName("basebutton");
    connect(m_Button,&QPushButton::clicked,this,&TCRBase::on_basebutton_clicked);
    m_Button->installEventFilter(this);
}

void TCRBase::setResistance(int i)
{
    setbit(m_resistancestate,(i-1));
    if(m_resistancestate!=0)
    {
        m_chooseble=true;
    }

}

void TCRBase::clearResistance(int i)
{
    clrbit(m_resistancestate,(i-1));
    if(m_resistancestate!=0)
    {
        m_chooseble=true;
    }

}

void TCRBase::setchoosed()
{
    if(m_resistancestate!=0)
    {
        m_chooseble=true;
        m_stateindicator->setChecked(true);
        //QPalette   pal;
        //pal.setColor(QPalette::ButtonText, QColor(255,69,0));
        //m_Button->setPalette(pal);

    }
    else
    {
        m_chooseble=false;
        m_stateindicator->setChecked(false);
        //QPalette   pal;
        //pal.setColor(QPalette::ButtonText, QColor(0,0,0));
        //m_Button->setPalette(pal);
    }

}

void TCRBase::setbasename(QString name)
{
    m_basename=name;

}

bool TCRBase::isChoosed()
{
    return m_chooseble;

}



flag_t TCRBase::Rstate()
{
    return m_resistancestate;

}

void TCRBase::clickcancelstate()
{
     m_Button->setStyleSheet("");

}

QPushButton *TCRBase::thebutton()
{
    return m_Button;

}

QString TCRBase::returnbasename()
{
    return m_basename;

}

void TCRBase::InitialBase()
{
    m_resistancestate=0;
    setchoosed();
    setbasename("");
    m_Button->setStyleSheet("");

}

void TCRBase::on_basebutton_clicked()
{

    clicksurestate();
    emit clicksure(m_no);

}

void TCRBase::clicksurestate()
{
    m_Button->setStyleSheet("background-color:rgb(0,255,127)");

}



