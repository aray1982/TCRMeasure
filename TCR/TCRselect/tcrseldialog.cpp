#include "tcrseldialog.h"
#include "ui_tcrseldialog.h"

TCRselDialog::TCRselDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCRselDialog),
    currentBaseno(1),
    m_basename(new QLineEdit(this)),
    m_baselable(new QLabel(this)),
    m_confirbutton(new QPushButton(this)),
    m_exitbutton(new QPushButton(this)),
    m_upboxs(new QCheckBox(this)),
    m_duboxs(new QCheckBox(this)),
    m_allboxs(new QCheckBox(this))
{
    ui->setupUi(this);
    QGridLayout *m_layout=new QGridLayout;

    m_layout->addWidget(Creativebasename(),0,0);

    m_layout->addWidget(CreativeT1(),1,0);

    m_layout->addWidget(CreativeT2(),1,1);

    m_layout->addWidget(CreativeT3(),2,0);

    m_layout->addWidget(CreativeT6(),2,1);
    m_layout->addWidget(CreativeResiatance(),3,0);
    setLayout(m_layout);
    slotInitial();
    setWindowTitle(tr("电阻选择"));
    setMinimumSize(800,400);

}

TCRselDialog::~TCRselDialog()
{
    delete ui;
}

void TCRselDialog::setRstate()
{
    flag_t temp=m_gpbase[currentBaseno-1]->Rstate();
    for(int i=0;i<10;i++)
    {
        if(cpare1(temp,i))
        {
            m_gpresist[i]->setChecked(true);
        }

    }


}

void TCRselDialog::slotInitial()
{
    foreach (QCheckBox *mybox, m_gpresist) {
        connect(mybox,&QCheckBox::clicked,this,&TCRselDialog::setRFlag);

    }//具体电阻控制
    foreach (TCRBase *thebase, m_gpbase) {
        connect(thebase,&TCRBase::clicksure,this,&TCRselDialog::setButtonstate);

    }//具体基板
    connect(m_confirbutton,&QPushButton::clicked,this,&TCRselDialog::measureResult);
    connect(m_upboxs,&QCheckBox::clicked,this,&TCRselDialog::setRall);
    connect(m_duboxs,&QCheckBox::clicked,this,&TCRselDialog::setRall);
    connect(m_allboxs,&QCheckBox::clicked,this,&TCRselDialog::setRall);
    connect(m_basename,&QLineEdit::textChanged,this,&TCRselDialog::RefreshName);
    connect(m_exitbutton,&QPushButton::clicked,this,&TCRselDialog::close);


}

QVector<basestr> TCRselDialog::getBasedetail()
{
    return m_baseresult;

}



void TCRselDialog::setRFlag()
{
    if(m_basename->text()=="")
    {
        QMessageBox::warning(NULL,tr("警告!"),tr("请输入基片名称，并确认"),QMessageBox::Yes);
        QCheckBox* thebox=qobject_cast<QCheckBox*>(sender());
        thebox->setChecked(false);
    }
    else
    {

    m_gpbase[currentBaseno-1]->setbasename(m_basename->text());
    QCheckBox *tempbox=qobject_cast<QCheckBox*>(sender());
    int destno=0;
    for(int i=0;i<10;i++)
    {
        if(tempbox==m_gpresist.at(i))
        {
            destno=i;
            break;
        }
        else
        {
            continue;
        }

    }

    if(m_gpresist.at(destno)->isChecked())
    {
        //qDebug()<<"desno"<<destno<<"checked";
        (m_gpbase[currentBaseno-1])->setResistance(destno+1);

    }
    else
    {
        //qDebug()<<"desno"<<destno<<"unchecked";
        (m_gpbase[currentBaseno-1])->clearResistance(destno+1);
    }

    m_gpbase[currentBaseno-1]->setchoosed();
    }



}

void TCRselDialog::setButtonstate(int no)
{
    currentBaseno=no+1;
    m_basename->setText(m_gpbase[no]->returnbasename());
    for(int i=0;i<24;i++)
    {
        if(i==no)
        {
            continue;

        }
        else
        {
            m_gpbase[i]->clickcancelstate();
        }
    }

    setcheckbox(m_gpbase[no]->Rstate());
    m_upboxs->setChecked(false);
    m_duboxs->setChecked(false);
    m_allboxs->setChecked(false);

}

void TCRselDialog::measureResult()
{
    m_baseresult.clear();
    for(int i=0;i<24;i++)
    {
        if(m_gpbase[i]->isChoosed())
        {
            basestr temp;
            temp.sno=i;
            temp.name=m_gpbase[i]->returnbasename();
            temp.rstate=m_gpbase[i]->Rstate();
            temp.dataid=0;
            m_baseresult.append(temp);
        }
        else{
            continue;
        }
    }
    if(m_baseresult.isEmpty())
    {
        QMessageBox::warning(NULL,tr("警告"),tr("没有电阻被选中"),QMessageBox::Yes);
    }
    else{
        /*foreach (basestr temp, m_baseresult) {
           qDebug()<<temp.sno;
           qDebug()<<temp.name;
           qDebug()<<temp.rstate;
           qDebug()<<temp.dataid;
        }*/
        close();
        emit baseRinfo(m_baseresult);

    }



}

void TCRselDialog::setRall()
{
    if(m_basename->text()=="")
    {
        QMessageBox::warning(NULL,tr("警告!"),tr("请输入基片名称，并确认"),QMessageBox::Yes);
        QCheckBox* thebox=qobject_cast<QCheckBox*>(sender());
        thebox->setChecked(false);
    }
    else
    {

    m_gpbase[currentBaseno-1]->setbasename(m_basename->text());
    QCheckBox *tempbox=qobject_cast<QCheckBox*>(sender());
    //int destno=0;
    if(tempbox==m_upboxs)
    {
        if(tempbox->isChecked())
        {
            for(int i=0;i<5;i++)
            {
                (m_gpbase[currentBaseno-1])->setResistance(i+1);
                m_gpresist.at(i)->setChecked(true);
            }
        }
        else
        {
            for(int i=0;i<5;i++)
            {
                (m_gpbase[currentBaseno-1])->clearResistance(i+1);
                m_gpresist.at(i)->setChecked(false);
            }

        }
    }
    else if(tempbox==m_duboxs)
    {
        if(tempbox->isChecked())
        {
            for(int i=5;i<10;i++)
            {
                (m_gpbase[currentBaseno-1])->setResistance(i+1);
                m_gpresist.at(i)->setChecked(true);
            }
        }
        else
        {
            for(int i=5;i<10;i++)
            {
                (m_gpbase[currentBaseno-1])->clearResistance(i+1);
                m_gpresist.at(i)->setChecked(false);
            }

        }

    }
    else if(tempbox==m_allboxs)
    {
        if(tempbox->isChecked())
        {
            for(int i=0;i<10;i++)
            {
                (m_gpbase[currentBaseno-1])->setResistance(i+1);
                m_gpresist.at(i)->setChecked(true);
            }
        }
        else
        {
            for(int i=0;i<10;i++)
            {
                (m_gpbase[currentBaseno-1])->clearResistance(i+1);
                m_gpresist.at(i)->setChecked(false);
            }

        }

    }
    m_gpbase[currentBaseno-1]->setchoosed();


    }



}

void TCRselDialog::RefreshName(QString newname)
{
    m_gpbase[currentBaseno-1]->setbasename(newname);

}



void TCRselDialog::setcheckbox(flag_t rstate)
{
    for(int i=0;i<10;i++)
    {
        if(cpare(rstate,i))
        {
            m_gpresist[i]->setChecked(true);
        }
        else
        {
            m_gpresist[i]->setChecked(false);
        }

    }




}

QGroupBox *TCRselDialog::Creativebasename()
{
    QGroupBox *tempbox=new QGroupBox(tr("基片名称"),this);
    QHBoxLayout *templayout=new QHBoxLayout;
    templayout->addWidget(m_basename);
    m_confirbutton->setObjectName("confirButton");
    m_confirbutton->setText(tr("测量"));
    //m_confirbutton->setMinimumWidth(70);
    templayout->addWidget(m_confirbutton);

    m_exitbutton->setObjectName("exitButton");
    m_exitbutton->setText(tr("退出"));
    //m_exitbutton->setMinimumWidth(70);
    templayout->addWidget(m_exitbutton);
    templayout->addStretch(1);
    tempbox->setLayout(templayout);
    return tempbox;



}

QGroupBox *TCRselDialog::CreativeT1()
{
    QGroupBox *tempbox=new QGroupBox(tr("T1(两线制)"),this);
    QHBoxLayout *templayout=new QHBoxLayout;
    for(int i=0;i<6;i++)
    {
        QString name="T1A"+QString::number(i+1);
        TCRBase *temp=new TCRBase(this,name,i);
        m_gpbase.append(temp);
        templayout->addWidget(temp);

    }
    templayout->addStretch(1);
    tempbox->setLayout(templayout);
    return tempbox;
}

QGroupBox *TCRselDialog::CreativeT2()
{
    QGroupBox *tempbox=new QGroupBox(tr("T2(两线制)"),this);
    QHBoxLayout *templayout=new QHBoxLayout;
    for(int i=6;i<12;i++)
    {
        QString name="T2A"+QString::number(i-6+1);
        TCRBase *temp=new TCRBase(this,name,i);
        m_gpbase.append(temp);
        templayout->addWidget(temp);

    }
    templayout->addStretch(1);
    tempbox->setLayout(templayout);
    return tempbox;
}

QGroupBox *TCRselDialog::CreativeT3()
{
    QGroupBox *tempbox=new QGroupBox(tr("T3(四线制)"),this);
    QHBoxLayout *templayout=new QHBoxLayout;
    for(int i=12;i<18;i++)
    {
        QString name="T3A"+QString::number(i-12+1);
        TCRBase *temp=new TCRBase(this,name,i);
        m_gpbase.append(temp);
        templayout->addWidget(temp);

    }
    templayout->addStretch(1);
    tempbox->setLayout(templayout);
    return tempbox;

}

QGroupBox *TCRselDialog::CreativeT6()
{
    QGroupBox *tempbox=new QGroupBox(tr("T6(四线制)"),this);
    QHBoxLayout *templayout=new QHBoxLayout;
    for(int i=18;i<24;i++)
    {
        QString name="T6A"+QString::number(i-18+1);
        TCRBase *temp=new TCRBase(this,name,i);
        m_gpbase.append(temp);
        templayout->addWidget(temp);

    }
    templayout->addStretch(1);
    tempbox->setLayout(templayout);
    return tempbox;

}

QGroupBox *TCRselDialog::CreativeResiatance()
{
    QGroupBox *tempbox=new QGroupBox(tr("电阻选择"),this);
    //QGroupBox *mtempbox=new QGroupBox(tr("电阻复选"),this);
    QHBoxLayout *templayout=new QHBoxLayout;
    QHBoxLayout *templayout1=new QHBoxLayout;
    //QHBoxLayout *mhtemplayout1=new QHBoxLayout;
    for(int i=0;i<5;i++)
    {
        QCheckBox *temp=new QCheckBox(this);
        temp->setLayoutDirection(Qt::RightToLeft);
        temp->setStyleSheet("QCheckBox::indicator {width:16px;height: 16px;}"
                            "QCheckBox::indicator:checked { image: url(:/QCheckboxPic/sc16); }"
                            "QCheckBox::indicator:unchecked{ image: url(:/QCheckboxPic/suc16); }");
        temp->setText("R"+QString::number(i+1));
        m_gpresist.append(temp);
        templayout->addWidget(temp);
    }


    m_upboxs->setLayoutDirection(Qt::RightToLeft);
    m_upboxs->setText("R1~5");
    m_allboxs->setLayoutDirection(Qt::RightToLeft);
    m_allboxs->setText("R1~10");
    m_upboxs->setStyleSheet("QCheckBox::indicator {width:16px;height: 16px;}"
                            "QCheckBox::indicator:checked { image: url(:/QCheckboxPic/sc16); }"
                            "QCheckBox::indicator:unchecked{ image: url(:/QCheckboxPic/susc16); }"
                            "QCheckBox{color:rgb(0,0,205)}");
    m_allboxs->setStyleSheet("QCheckBox::indicator {width:16px;height: 16px;}"
                             "QCheckBox::indicator:checked { image: url(:/QCheckboxPic/sc16); }"
                             "QCheckBox::indicator:unchecked{ image: url(:/QCheckboxPic/susc16); }"
                             "QCheckBox{color:rgb(0,0,205)}");


    QVBoxLayout *boxlayout=new QVBoxLayout();

    boxlayout->addWidget(m_upboxs);
    boxlayout->addWidget(m_allboxs);

    templayout->addWidget(m_upboxs);
    templayout->addWidget(m_allboxs);


     //Add Rcheckboxs1
    for(int i=0;i<5;i++)
    {
        QCheckBox *temp=new QCheckBox(this);
        temp->setLayoutDirection(Qt::RightToLeft);
        temp->setText("R"+QString::number(i+6));
        temp->setStyleSheet("QCheckBox::indicator {width:16px;height: 16px;}"
                            "QCheckBox::indicator:checked { image: url(:/QCheckboxPic/sc16); }"
                            "QCheckBox::indicator:unchecked{ image: url(:/QCheckboxPic/suc16); }");

        m_gpresist.append(temp);
        templayout1->addWidget(temp);
    }
    m_duboxs->setLayoutDirection(Qt::RightToLeft);
    m_duboxs->setText("R6~10");

    m_duboxs->setStyleSheet("QCheckBox::indicator {width:16px;height: 16px;}"
                            "QCheckBox::indicator:checked { image: url(:/QCheckboxPic/sc16); }"
                            "QCheckBox::indicator:unchecked{ image: url(:/QCheckboxPic/susc16); }"
                            "QCheckBox{color:rgb(0,0,205)}");


    boxlayout->addWidget(m_duboxs);
    templayout1->addWidget(m_duboxs);
    templayout->addStretch(1);
    templayout->setSpacing(15);
    templayout1->addStretch(1);
    templayout1->setSpacing(15);

    QVBoxLayout *tempVlayout=new QVBoxLayout(this);
    tempVlayout->addLayout(templayout);
    tempVlayout->addLayout(templayout1);
    tempbox->setLayout(tempVlayout);



    return tempbox;


}

void TCRselDialog::InitialTest()
{
    foreach (TCRBase *base, m_gpbase) {
        base->InitialBase();

    }
    currentBaseno=1;
    m_basename->setText("");
    m_upboxs->setChecked(false);
    m_duboxs->setChecked(false);
    m_allboxs->setChecked(false);
    foreach (QCheckBox*temp, m_gpresist) {
        temp->setChecked(false);

    }
}
