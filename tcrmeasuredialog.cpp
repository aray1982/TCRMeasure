#include "tcrmeasuredialog.h"
#include "ui_tcrmeasuredialog.h"
#include<QPrinterInfo>

TCRmeasureDialog::TCRmeasureDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCRmeasureDialog),
    m_stagecounter(0),
    m_btGroup(new QButtonGroup(this)),
    m_steadytime(new QTimer(this)),
    m_errortime(new QTimer(this)),
    m_probebantime(new QTimer(this)),
    Isrunning(false),
    m_view(new ProgressView(this)),
    m_progressstage(0),
    Istest(true),
    intervalDealytime(1000),
    m_modbusRun(false),
    T1data(nullptr),
    T2data(nullptr),
    T3data(nullptr),
    T6data(nullptr),
    Iscreatedata(false),
    intervalRDelaytime(2000),
    applyCalibrate(false)
{
    //m_scene->addLine(0,0,100,100);

    ui->setupUi(this);
    Initial();
    setWindowTitle(tr("TCR测量"));

    //添加进程图
    QVBoxLayout *m_tclayout=new QVBoxLayout;
    m_tclayout->addWidget(m_view);
    //m_tclayout->addWidget(m_viwe);

    DatathreadInit();
    ui->groupBox_2->setLayout(m_tclayout);

    //follow as dataop thread




    ModbusInit();


}

TCRmeasureDialog::~TCRmeasureDialog()
{
    m_Modbusthread.quit();
    m_Modbusthread.wait();
    m_Datathread.quit();
    m_Datathread.wait();
    delete ui;
}


void TCRmeasureDialog::Initial()
{
    QHBoxLayout *layout=new QHBoxLayout;
    QHBoxLayout *layout1=new QHBoxLayout;
    QHBoxLayout *layout2=new QHBoxLayout;
    QHBoxLayout *layout3=new QHBoxLayout;
    for(int i=0;i<6;i++)
    {
        QString basname="T1A";
        QString objename="pushButton_";
        QPushButton *tempbutton=new QPushButton;
        tempbutton->setText(basname+QString::number(i+1));
        layout->addWidget(tempbutton);
        tempbutton->setObjectName(objename+QString::number(i));
        m_btGroup->addButton(tempbutton,i);
    }
    for(int i=6;i<12;i++)
    {
        QString basname="T2A";
        QString objename="pushButton_";
        QPushButton *tempbutton=new QPushButton;
        tempbutton->setText(basname+QString::number(i-6+1));
        layout1->addWidget(tempbutton);
        tempbutton->setObjectName(objename+QString::number(i));
        m_btGroup->addButton(tempbutton,i);
    }
    for(int i=12;i<18;i++)
    {
        QString basname="T3A";
        QString objename="pushButton_";
        QPushButton *tempbutton=new QPushButton;
        tempbutton->setText(basname+QString::number(i-12+1));
        layout2->addWidget(tempbutton);
        tempbutton->setObjectName(objename+QString::number(i));
        m_btGroup->addButton(tempbutton,i);
    }
    for(int i=18;i<24;i++)
    {
        QString basname="T6A";
        QString objename="pushButton_";
        QPushButton *tempbutton=new QPushButton;
        tempbutton->setText(basname+QString::number(i-18+1));
        layout3->addWidget(tempbutton);
        tempbutton->setObjectName(objename+QString::number(i));
        m_btGroup->addButton(tempbutton,i);
    }
    QVBoxLayout *Vlayout=new QVBoxLayout;
    Vlayout->addLayout(layout);
    Vlayout->addLayout(layout1);
    Vlayout->addLayout(layout2);
    Vlayout->addLayout(layout3);
    ui->Re_groupBox->setLayout(Vlayout);
    setTCRButtonplus(24);
    Istest=false;
    //ui->Execute_pushButton->setEnabled(false);
    ui->state_label->setText(tr("待机中"));
    //connect(this,&TCRmeasureDialog::sendError,this,&TCRmeasureDialog::DisplayError);
    //connect(this,&TCRmeasureDialog::)

    //窗口最大最小化
    m_btGroup->setExclusive(false);
    QList<QAbstractButton*> tdbuttons=m_btGroup->buttons();
    foreach(QAbstractButton *button,tdbuttons)
    {
        button->setCheckable(true);


    }

    Qt::WindowFlags flags=Qt::Dialog;

    flags |=Qt::WindowMinimizeButtonHint;

    setWindowFlags(flags);



}

void TCRmeasureDialog::ButtonTextcolor(bool settingflag)
{
    QList<QAbstractButton*> tdbuttons=m_btGroup->buttons();
    foreach (QAbstractButton *button, tdbuttons) {
        QPalette   pal;
        pal.setColor(QPalette::ButtonText, QColor(0,0,0));
        button->setEnabled(false);
        button->setChecked(false);
        button->setPalette(pal);
        button->setStyleSheet("");

    }//全部归零
    if(settingflag)
    {
        foreach(basestr base ,m_tcrbaseinfo)
        {
            int tempno=base.sno;
            QPushButton *tempb=static_cast<QPushButton*>(m_btGroup->button(tempno));
            QPalette   pal;
            pal.setColor(QPalette::ButtonText, QColor(255,69,0));
            tempb->setEnabled(true);
            tempb->setChecked(true);
            tempb->setStyleSheet("\
                            QPushButton {   \
                                color:red;    \
                            }   \
                            QPushButton:checked{\
                                background-color: rgb(0,197,205);}\
                            QPushButton:hover{  \
                                background-color: rgb(0,238,118); \
                                border-style: outset;  \
                              }"

                            );
            //tempb->setStyleSheet("QPushButton {color:black;}");
            tempb->setPalette(pal);
        }
    }



}


void TCRmeasureDialog::SetTCRButton(int no)
{

    QList<QAbstractButton*> tbuttons=m_btGroup->buttons();
        foreach (QAbstractButton * temp, tbuttons) {
            int curno=m_btGroup->id(temp);
            //qDebug()<<"currNo"<<curno;
            if(curno==no)
            {
                temp->setStyleSheet("background-color:rgb(84,255,159)");
            }
            else
            {
                temp->setStyleSheet("");

            }

        }

}

void TCRmeasureDialog::setTCRButtonplus(int no)
{
    QList<QAbstractButton*> tbuttons=m_btGroup->buttons();
        foreach (QAbstractButton * temp, tbuttons) {
            int curno=m_btGroup->id(temp);
            //qDebug()<<"currNo"<<curno;
            if(curno==no)
            {
                temp->setChecked(true);
            }
            else {
                temp->setDown(false);
                temp->setChecked(false);
            }


        }


}



void TCRmeasureDialog::StopTCRtest(bool stopMb)
{
    if(stopMb)
    {
     RefreshafterTest();
     m_progressstage=0;
     m_view->stageProgress(m_progressstage);
     ButtonTextcolor(false);
     m_modbusRun=false;
     emit StopMb();
    }
    else
    {
        RefreshafterTest();
        MbStopoperation();
        m_progressstage=0;
        m_view->stageProgress(m_progressstage);
        ButtonTextcolor(false);

    }
    emit accessTCRM(true);
    emit accessProbe();

}

void TCRmeasureDialog::RefreshafterTest()
{
    m_steadytime->stop();
    m_errortime->stop();
    m_stagecounter=0;
    disconnect(m_steadytime,&QTimer::timeout,this,&TCRmeasureDialog::BeginMeasure);
    disconnect(m_errortime,&QTimer::timeout,this,&TCRmeasureDialog::Overtimedeal);
    Isrunning=false;
    Istest=false;
    ui->Execute_pushButton->setStyleSheet("");
    ui->Execute_pushButton->setText(tr("实行"));
    ui->Execute_pushButton->setEnabled(false);

    setTCRButtonplus(24);

}

void TCRmeasureDialog::MbStopoperation()
{
    emit readfine(2,25);

    QTimer::singleShot(5000,this,&TCRmeasureDialog::finalStop);

}

void TCRmeasureDialog::RefreshProgress()
{
    m_view->stageProgress(m_progressstage);

}

bool TCRmeasureDialog::RecordData()
{
    foreach (basestr base, m_tcrbaseinfo) {
        int baseno=base.sno;
        QVector<short> Rdetail=getchannelNo(base.rstate);
        if(baseno<6)
        {

           foreach (short cno, Rdetail)
           {

                auto p=T1data+baseno;

                double value=*(*p+cno);

                qDebug()<<"baseno"<<baseno<<"Rno"<<cno<<"value"<<value;


               if(m_tdataop->insertRvalue(base.dataid,cno,value,m_stagecounter))
               {
                   continue;
               }
               else
               {
                   return false;
               }
           }

        }
        else if(baseno<12)
        {
                  foreach (short cno, Rdetail)
                  {

                      auto p=T2data+baseno-6;

                      double value=*(*p+cno);

                      qDebug()<<"baseno"<<baseno<<"Rno"<<cno<<"value"<<value;


                       if(m_tdataop->insertRvalue(base.dataid,cno,value,m_stagecounter))
                       {
                           continue;
                       }
                       else
                       {
                           return false;
                       }
                  }


        }
        else if(baseno<18)
        {
                      foreach (short cno, Rdetail)
                      {

                          auto p=T3data+baseno-12;

                          double value=*(*p+cno);

                          qDebug()<<"baseno"<<baseno<<"Rno"<<cno<<"value"<<value;


                           if(m_tdataop->insertRvalue(base.dataid,cno,value,m_stagecounter))
                           {
                               continue;
                           }
                           else
                           {
                               return false;
                           }
                      }


        }
        else if(baseno<24)
        {
                          foreach (short cno, Rdetail)
                          {

                              auto p=T6data+baseno-18;

                              double value=*(*p+cno);

                              qDebug()<<"baseno"<<baseno<<"Rno"<<cno<<"value"<<value;

                               if(m_tdataop->insertRvalue(base.dataid,cno,value,m_stagecounter))
                               {
                                   continue;
                               }
                               else
                               {
                                   return false;
                               }
                          }


        }
    }

    return true;
}

bool TCRmeasureDialog::Finaldeal()
{
    foreach (basestr base, m_tcrbaseinfo) {
        //int baseno=base.sno;
        if(m_tdataop->addcurrentDateTime(base.dataid))
        {
            qDebug()<<base.name<<"add datetime successful!";
            if(m_tdataop->UpdateTCR(base.dataid))
            {
                qDebug()<<"UdTCR"<<base.name<<"successful!";
                continue;
            }
            else
            {
                qDebug()<<"UdTCR"<<base.name<<"fail";
                return false;

            }
            return true;


        }
        else
        {
            qDebug()<<base.name<<"add datetime fail!";
            return false;
        }
        return true;
    }
}

QVector<short> TCRmeasureDialog::getchannelNo(flag_t rinfo)
{
    QVector<short> temp;
        for(int i=0;i<10;i++)
        {
            if(cpare(rinfo,i))
            {
                temp.append(i);

            }
            else
            {
                continue;
            }
        }
        return temp;



}

void TCRmeasureDialog::displaytempReport()
{

        QVector<QStringList> totaldata;
        QVector<basestr>::iterator it;
        foreach(short no, tempreportInfo)
        {
            basestr tempbase=m_tcrbaseinfo.at(no);
            QStringList singledata;
            QVector<short> valueserque=getchannelNo(tempbase.rstate);
            if(tempbase.sno<6)
            {

                QString plateno="T1A"+QString::number(tempbase.sno+1);
                singledata.append(plateno);
                singledata.append(tempbase.name);
                auto p=T1data+tempbase.sno;
                for(int i=0;i<10;i++)
                {
                    if(valueserque.contains(i))
                    {
                        double value=*(*p+i);
                        singledata.append(QString::number(value,'g',10));


                    }
                    else{
                        singledata.append(QString::number(0));
                    }

                }



            }
            else if(tempbase.sno<12)
            {
                QString plateno="T2A"+QString::number(tempbase.sno+1-6);
                singledata.append(plateno);
                singledata.append(tempbase.name);
                auto p=T2data+(tempbase.sno-6);
                for(int i=0;i<10;i++)
                {
                    if(valueserque.contains(i))
                    {
                        double value=*(*p+i);
                        singledata.append(QString::number(value,'g',10));


                    }
                    else{
                        singledata.append(QString::number(0));
                    }

                }
            }
            else if(tempbase.sno<18)
            {
                QString plateno="T3A"+QString::number(tempbase.sno+1-12);
                singledata.append(plateno);
                singledata.append(tempbase.name);
                auto p=T3data+(tempbase.sno-12);
                for(int i=0;i<10;i++)
                {
                    if(valueserque.contains(i))
                    {
                        double value=*(*p+i);
                        singledata.append(QString::number(value,'g',10));


                    }
                    else{
                        singledata.append(QString::number(0));
                    }

                }
            }
            else if(tempbase.sno<24)
            {
                QString plateno="T6A"+QString::number(tempbase.sno+1-18);
                singledata.append(plateno);
                singledata.append(tempbase.name);
                auto p=T6data+(tempbase.sno-18);
                for(int i=0;i<10;i++)
                {
                    if(valueserque.contains(i))
                    {
                        double value=*(*p+i);
                        singledata.append(QString::number(value,'g',10));


                    }
                    else{
                        singledata.append(QString::number(0));
                    }

                }
            }
            totaldata.append(singledata);



        }



        if(m_tdataop->insertTempreportValue(totaldata))
        {
            //qDebug()<<"create data successful!";
            m_report.loadFromFile(":/Report/Temprp");
            //m_report.loadFromFile(":/Remodel.lrxml");
            QString currentDir=QDir::currentPath();
            //qDebug()<<currentDir;
            QString path=currentDir+"/Mydatabase.db";
            m_report.dataManager()->setReportVariable("datasource",path);

            m_report.previewReport();
        }
        else
        {
            qDebug()<<"create data fail!";
            QMessageBox::warning(NULL,tr("警告"),tr("创建临时数据失败!"),QMessageBox::Yes);
        }



}



void TCRmeasureDialog::errorHandler(const QString &info)
{
    QMessageBox::warning(this,tr("警告"),info,QMessageBox::Yes);
    if(info.contains(tr("连接失败"),Qt::CaseInsensitive))
    {
        qDebug()<<"containt connect fail!";
        StopTCRtest(true);
    }
    else
    {
        StopTCRtest(false);
    }
    deleteData();


}

void TCRmeasureDialog::sendProbeban()
{
    disconnect(m_probebantime,&QTimer::timeout,this,&TCRmeasureDialog::sendProbeban);
    qDebug()<<"ban stage "<<m_stagecounter;
    m_probebantime->stop();
    emit banProbe();

}

bool TCRmeasureDialog::createdData()
{
    QVector<basestr>::iterator it;
    for(it=m_tcrbaseinfo.begin();it!=m_tcrbaseinfo.end();it++)
    {
        int T_id;

        if(m_tdataop->createData(it->name,T_id,m_settings.SV1,m_settings.SV2,m_settings.SV3))
        {
         it->dataid=T_id;
        }
        else
        {
            sendError(tr("创建数据文件失败！"));
            return false;
        }
    }
    return true;


}

bool TCRmeasureDialog::deleteData()
{
    QVector<basestr>::iterator it;
    for(it=m_tcrbaseinfo.begin();it!=m_tcrbaseinfo.end();it++)
    {
        int T_id=it->dataid;

        if(m_tdataop->deleteData(T_id))
        {
         continue;
        }
        else
        {
            sendError(tr("删除数据文件失败！"));
            return false;
        }
    }
    return true;


}

void TCRmeasureDialog::finalPrintData()
{
    if(m_settings.Isprint)
    {
        QVector<basestr>::iterator it;
        for(it=m_tcrbaseinfo.begin();it!=m_tcrbaseinfo.end();it++)
        {
            int TempT_id=it->dataid;
            if(applyCalibrate)
            {
                m_report.loadFromFile(":/Report/TCRrpbv");
                m_report.dataManager()->setReportVariable("Rbias",coefficientdata[0]);
                m_report.dataManager()->setReportVariable("Rcore",coefficientdata[1]);

            }
            else
            {
                m_report.loadFromFile(":/Report/TCRrp");
            }

            QString currentDir=QDir::currentPath();
            //qDebug()<<currentDir;
            QString path=currentDir+"/Mydatabase.db";
            //qDebug()<<path;
            m_report.dataManager()->setReportVariable("datasource",path);
            m_report.dataManager()->setReportVariable("T_id",TempT_id);
            QPrinter xprinter(QPrinterInfo::defaultPrinter());
            m_report.printReport(&xprinter);
        }

    }


}

void TCRmeasureDialog::ModbusErrorDeal(QString errorinfo)
{
    qDebug()<<"Error deal";
    emit transModbusError(errorinfo,0);

}



void TCRmeasureDialog::setData(int i, arrData *data)
{
    switch (i) {
    case 0:
        T1data=data;
        break;
    case 1:
        T2data=data;
        break;
    case 2:
        T3data=data;
        break;
    case 5:
        T6data=data;
        break;
    default:
        break;
    }


}








void TCRmeasureDialog::CycleDeal()
{
    switch(m_stagecounter){
        case 0:
            m_stagecounter++;
            ui->state_label->setText(tr("阶段1：安定2"));
            m_progressstage=3;
            RefreshProgress();
            setTCRButtonplus(24);
            m_steadytime->stop();

            m_steadytime->start(m_settings.steadyTime2*60000);
            connect(m_steadytime,&QTimer::timeout,this,&TCRmeasureDialog::BeginMeasure);
            emit accessProbe();
            delayProbeban();

            //QTimer::singleShot(m_setting.stime2,this,&TCRmeasureDialog::starmeasure);
            break;
        case 1:
            m_stagecounter++;
            ui->state_label->setText(tr("阶段2：升温"));
            m_progressstage=4;
            RefreshProgress();
            m_errortime->start(m_settings.Uovertime1*60000);
            connect(m_errortime,&QTimer::timeout,this,&TCRmeasureDialog::Overtimedeal);
            setTCRButtonplus(24);
            //Istest=false;
            //SetSV(m_settings.SV2);
            //Mbconnect();
            emit accessProbe();
            emit startMb();
            m_modbusRun=true;
            emit readfine(2,m_settings.SV2);
            break;
        case 2:
            m_stagecounter++;
            ui->state_label->setText(tr("阶段2：安定2"));
            m_progressstage=6;
            RefreshProgress();
            setTCRButtonplus(24);
            m_steadytime->stop();
            m_steadytime->start(m_settings.steadyTime2*60000);
            connect(m_steadytime,&QTimer::timeout,this,&TCRmeasureDialog::BeginMeasure);
            emit accessProbe();
            delayProbeban();
            //Istest=false;
            //QTimer::singleShot(m_setting.stime2,this,&TCRDialog::starmeasure);
            break;
        case 3:
            m_stagecounter++;
            ui->state_label->setText(tr("阶段3:升温"));
            m_progressstage=7;
            RefreshProgress();
            m_errortime->start(m_settings.Uovertime2*60000);
            connect(m_errortime,&QTimer::timeout,this,&TCRmeasureDialog::Overtimedeal);
            setTCRButtonplus(24);
            emit accessProbe();
            //Istest=false;
            emit startMb();
            m_modbusRun=true;
            emit readfine(2,m_settings.SV3);
            break;
        case 4:
            m_stagecounter++;
            ui->state_label->setText(tr("阶段3：安定2"));
            m_progressstage=9;
            RefreshProgress();
            setTCRButtonplus(24);
            m_steadytime->stop();
            m_steadytime->start(m_settings.steadyTime2*60000);
            connect(m_steadytime,&QTimer::timeout,this,&TCRmeasureDialog::BeginMeasure);
            emit accessProbe();
            delayProbeban();
            //Istest=false;
            //QTimer::singleShot(m_setting.stime2,this,&TCRDialog::starmeasure);
            break;
        case 5:
            m_stagecounter++;
            m_stagecounter=1;


            setTCRButtonplus(24);
            ui->state_label->setText(tr("测试结束"));
            Iscreatedata=false;
            emit accessProbe();
            StopTCRtest(false);
            if(!Finaldeal())
            {
                sendError(tr("记录数据失败!"));
            }
            else
            {
                qDebug()<<"Print Data";
                finalPrintData();
            }

            emit endcycleNotion();


            break;



        }

}

void TCRmeasureDialog::GetTCRsetting(TCRsettings setting)
{
    m_settings=setting;
    ui->lotemp_label->setText(QString::number(m_settings.SV1));
    ui->notemp_label->setText(QString::number(m_settings.SV2));
    ui->hitemp_label->setText(QString::number(m_settings.SV3));
    qDebug()<<"ltemp"<<m_settings.SV1;
    qDebug()<<"ntemp"<<m_settings.SV2;
    qDebug()<<"htemp"<<m_settings.SV3;
    m_tdataop->setcalculateValue(m_settings.CalV1,m_settings.CalV2,m_settings.CalV3);
    //m_view->setYRange(m_settings.SV1,m_settings.SV3);
    qreal temp[3]={m_settings.SV1,m_settings.SV2,m_settings.SV3};
    m_view->setLimitvalue(temp);

}



void TCRmeasureDialog::Overtimedeal()
{
    switch (m_stagecounter) {
        case 0:
            ui->state_label->setText(tr("降温超时"));
            break;
        case 2:
            ui->state_label->setText(tr("升温超时"));
        case 4:
            ui->state_label->setText(tr("升温2超时"));
            break;

        default:
            break;
        }
    StopTCRtest(false);



}

void TCRmeasureDialog::MeasureReady(bool issuccessful)
{
     Istest=issuccessful;
     if(Istest)
     {
         ui->Execute_pushButton->setEnabled(true);
     }
     else
     {
         ui->Execute_pushButton->setEnabled(false);
     }

}

void TCRmeasureDialog::MbStopFinish()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
            if (!reply)
                return;
            if (reply->error() == QModbusDevice::ProtocolError) {
                sendError(tr("写入响应错误: %1 (Modbus 异常代码: 0x%2)")
                    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));

                return;
            } else if (reply->error() != QModbusDevice::NoError) {
                sendError(tr("写入异常代码: %1 (代码: 0x%2)").
                    arg(reply->errorString()).arg(reply->error(), -1, 16));

                return;

            }
            qDebug()<<"Mb stop finish";
            if(Iscreatedata)
            {
                ui->Execute_pushButton->setEnabled(true);
            }
            else
            {
                ui->Execute_pushButton->setEnabled(false);
            }
            reply->deleteLater();
            //Mbdisconnect();


}


void TCRmeasureDialog::Cyclefinish()
{
    if(Istest)
    {
        displaytempReport();
        ui->Confirm_pushButton->setEnabled(true);
        Istest=false;
        setTCRButtonplus(24);

    }
    else
    {
        if(RecordData())
        {
            //m_stagecounter++;
            CycleDeal();
        }
        else
        {
            sendError(tr("记录数据错误"));
            StopTCRtest(false);
        }
        setTCRButtonplus(24);
    }




}

void TCRmeasureDialog::intervalDeal()
{
    QTimer::singleShot(intervalDealytime,this,SLOT(BeginMeasure()));

}

void TCRmeasureDialog::setTCRlampindicator(short blate, short rno)
{
    if(blate<24)
    {
        m_steadytime->stop();
        qDebug()<<"TCR lamp indicator m_stagecouter is"<<m_stagecounter;
        disconnect(m_steadytime,&QTimer::timeout,this,&TCRmeasureDialog::BeginMeasure);
        switch (m_stagecounter) {
            case 0:
            if(Istest)
            {
                ui->state_label->setText(tr("临时报表"));
            }
            else    ui->state_label->setText(tr("阶段1:测试1"));
                break;
            case 1:
                ui->state_label->setText(tr("阶段1:测试2"));
                break;
            case 2:
                ui->state_label->setText(tr("阶段2:测试1"));
                break;
            case 3:
                ui->state_label->setText(tr("阶段2:测试2"));
                break;
            case 4:
                ui->state_label->setText(tr("阶段3:测试1"));
                break;
            case 5:
                ui->state_label->setText(tr("阶段3:测试2"));
                break;

            default:
                break;
            }
        QTimer::singleShot(intervalRDelaytime,[this,blate,rno](){emit stmeasuretoDMM(blate,rno);});
    }
    setTCRButtonplus(blate);


}

void TCRmeasureDialog::reciveInfo(QVector<basestr> info)
{
    if(!Iscreatedata)
    {
        m_tcrbaseinfo=info;

    }
    ButtonTextcolor(true);


}

void TCRmeasureDialog::delayProbeban()
{
    switch (m_stagecounter) {
    case 0:
        m_probebantime->start((m_settings.steadyTime1-m_settings.Pbdelaytime)*60000);
        connect(m_probebantime,&QTimer::timeout,this,&TCRmeasureDialog::sendProbeban);
        break;
    case 1:
        m_probebantime->start((m_settings.steadyTime2-m_settings.Pbdelaytime)*60000);
        connect(m_probebantime,&QTimer::timeout,this,&TCRmeasureDialog::sendProbeban);
        break;
    case 2:
        m_probebantime->start((m_settings.steadyTime1-m_settings.Pbdelaytime)*60000);
        connect(m_probebantime,&QTimer::timeout,this,&TCRmeasureDialog::sendProbeban);
        break;
    case 3:
        m_probebantime->start((m_settings.steadyTime2-m_settings.Pbdelaytime)*60000);
        connect(m_probebantime,&QTimer::timeout,this,&TCRmeasureDialog::sendProbeban);
        break;
    case 4:
        m_probebantime->start((m_settings.steadyTime1-m_settings.Pbdelaytime)*60000);
        connect(m_probebantime,&QTimer::timeout,this,&TCRmeasureDialog::sendProbeban);
        break;
    case 5:
        m_probebantime->start((m_settings.steadyTime2-m_settings.Pbdelaytime)*60000);
        connect(m_probebantime,&QTimer::timeout,this,&TCRmeasureDialog::sendProbeban);
        break;
    default:
        break;
    }

}

void TCRmeasureDialog::MBconnectInfo(bool state)
{
    if(!state)
    {
        QMessageBox::warning(NULL,tr("警告"),tr("试验箱连接中断，请检查试验箱链接!"),QMessageBox::Yes);
    }

}

void TCRmeasureDialog::setCalibrate()
{
    applyCalibrate=true;

}

void TCRmeasureDialog::setCoefficient(qreal *data)
{
    coefficientdata[0]=*data;
    coefficientdata[1]=*(data+1);
}

void TCRmeasureDialog::BeginMeasure()
{
    emit starmeasure(false);

}

void TCRmeasureDialog::showEvent(QShowEvent *event)
{
    ui->Execute_pushButton->setEnabled(true);

}



void TCRmeasureDialog::on_TMExit_pushButton_clicked()
{
    close();
}

void TCRmeasureDialog::on_Execute_pushButton_clicked()
{

        if(!Isrunning)
        {
            Isrunning=true;
            //按钮状态
            ui->Execute_pushButton->setText(tr("停止"));
            ui->Execute_pushButton->setStyleSheet("background-color:lightgreen");
            m_stagecounter=0;
            setTCRButtonplus(24);
            Istest=false;

            if(createdData())
            {
                emit setBaseDelay(3);//基片延时
                emit startMb();
                m_modbusRun=true;
                emit readfine(1,0);

                ui->state_label->setText(tr("阶段1:降温"));
                m_progressstage=1;
                RefreshProgress();


                //降温超时处理
                connect(m_errortime,&QTimer::timeout,this,&TCRmeasureDialog::Overtimedeal);
                //m_errortime->setInterval();
                m_errortime->start(m_settings.Dovertime*60000);
                emit accessTCRM(false);

            }
            else
            {
                sendError(tr("创建初始数据失败！"));
                StopTCRtest(false);
            }

        }
        else
        {
            deleteData();
            StopTCRtest(false);

        }
}

void TCRmeasureDialog::on_Confirm_pushButton_clicked()
{


    //setTCRButtonplus(24);
    qDebug()<<m_tdataop->deleteTempreport();
    ui->Confirm_pushButton->setEnabled(false);
    Istest=true;

    //设定第一段温度
    emit readfine(2,m_settings.SV1);
    //开始临时报表测量

    tempreportInfo.clear();

    short tempno=0;

    qDebug()<<"tempreport";

    auto x=m_btGroup->buttons();

    foreach(auto button, x)
    {
        qDebug()<<"for cycle";
        if(button->isEnabled())
        {
            qDebug()<<"is enable ";
            if(button->isChecked())
            {
                qDebug()<<"temp no"<<tempno;
                tempreportInfo.append(tempno);

            }
            tempno++;

        }


    }
    foreach(auto z,tempreportInfo)
    {
        qDebug()<<z;
    }
    emit sendTempmeasure(tempreportInfo);
    //emit starmeasure(false);






}



void TCRmeasureDialog::ModbusInit()
{
        Mymodbus *md=new Mymodbus;
        md->moveToThread(&m_Modbusthread);

        connect(&m_Modbusthread, &QThread::finished, md, &QObject::deleteLater);
        //connect(this,&MainWindow::setstate,modbus,&Mymodbus::stateConfirm);
        connect(this,&TCRmeasureDialog::readfine,md,&Mymodbus::ChamberRead);
        connect(&m_Modbusthread,&QThread::started,md,&Mymodbus::MBconnect);
        connect(md,&Mymodbus::sendPvalue,this,&TCRmeasureDialog::valueProcessDeal);
        connect(this,&TCRmeasureDialog::startMb,md,&Mymodbus::setRun);
        connect(this,&TCRmeasureDialog::StopMb,md,&Mymodbus::stopRun);
        connect(md,&Mymodbus::sendError,this,&TCRmeasureDialog::ModbusErrorDeal);
        //connect(md,&Mymodbus::sendError,this,&TCRmeasureDialog::DisplayError);
        connect(md,&Mymodbus::connectstate,this,&TCRmeasureDialog::MBconnectInfo);
        m_Modbusthread.start();


}

void TCRmeasureDialog::valueProcessDeal(const float &value)
{
    //add 信号
    ui->temperature_label->setText(QString::number(value));
    switch(m_stagecounter){
    case 0:
    {
        if((value<(m_settings.SV1+1))&&(value>(m_settings.SV1-1)))
        {
            qDebug()<<"enter sv1 zone";
            ui->state_label->setText(tr("阶段1：安定1"));
            emit StopMb();//停止modbus读取
            m_modbusRun=false;
            m_progressstage=2;
            RefreshProgress();//显示进程刷新
            connect(m_steadytime,&QTimer::timeout,this,&TCRmeasureDialog::BeginMeasure);
            disconnect(m_errortime,&QTimer::timeout,this,&TCRmeasureDialog::Overtimedeal);
            m_steadytime->start(m_settings.steadyTime1*60000);
            delayProbeban();

        }
        else{
            if(m_modbusRun)
            QTimer::singleShot(10000,[this](){ emit readfine(0,0);});
        }
        break;
    }

    case 2:
    {
        if((value<(m_settings.SV2+1))&&(value>(m_settings.SV2-1)))
        {
            qDebug()<<"enter sv2 zone";
            emit StopMb();//停止modbus读取
            m_modbusRun=false;
            setTCRButtonplus(24);
            ui->state_label->setText(tr("阶段2：安定1"));
            m_progressstage=5;
            RefreshProgress();
            connect(m_steadytime,&QTimer::timeout,this,&TCRmeasureDialog::BeginMeasure);
            m_steadytime->start(m_settings.steadyTime1*60000);
            disconnect(m_errortime,&QTimer::timeout,this,&TCRmeasureDialog::Overtimedeal);
            delayProbeban();


        }
        else{
            if(m_modbusRun)
            QTimer::singleShot(10000,[this](){ emit readfine(0,0);});
        }
        break;

    }
    case 4:
    {
        if((value<(m_settings.SV3+1))&&(value>(m_settings.SV3-1)))
        {
            qDebug()<<"enter sv3 zone";
            emit StopMb();//停止modbus读取
            m_modbusRun=false;
            setTCRButtonplus(24);
            ui->state_label->setText(tr("阶段3：安定1"));
            m_progressstage=8;
            RefreshProgress();
            connect(m_steadytime,&QTimer::timeout,this,&TCRmeasureDialog::BeginMeasure);
            m_steadytime->start(m_settings.steadyTime1*60000);
            disconnect(m_errortime,&QTimer::timeout,this,&TCRmeasureDialog::Overtimedeal);
            delayProbeban();

        }
        else{
            if(m_modbusRun)
            QTimer::singleShot(10000,[this](){ emit readfine(0,0);});
            //emit readfine(0,0);
        }
        break;

    }
    default:
        break;
    }

}

void TCRmeasureDialog::finalStop()
{
    m_modbusRun=false;
    emit StopMb();

}

void TCRmeasureDialog::DatathreadInit()
{
    m_tdataop=new TCRdataop;
    m_tdataop->moveToThread(&m_Datathread);
    connect(&m_Datathread, &QThread::finished,m_tdataop, &QObject::deleteLater);
    m_Datathread.start();
}

QVector<short> getchannelNo(flag_t rinfo)
{
    QVector<short> temp;
        for(int i=0;i<10;i++)
        {
            if(cpare(rinfo,i))
            {
                temp.append(i);

            }
            else
            {
                continue;
            }
        }
        return temp;

}

