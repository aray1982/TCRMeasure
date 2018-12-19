#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_TCRSettingDialog(new TCRsettingDialog(this)),
    m_TCRselDialog(new TCRselDialog(this)),
    m_TCRMDialog(new TCRmeasureDialog(this)),
    m_ProbeDialog(new ProbeTestDialog(this)),
    m_dmm(new Dmm()),
    m_switch(new Switch()),
    m_dataop(new Dataop(this)),
    m_datadialog(new TCRdataDialog(this)),
    m_caldialog(new CalibrateDialog(this))
{
    if(m_dataop->Initial())
    {
        qDebug()<<"CreateTables";
        m_dataop->CreateTabels();
    }
    ui->setupUi(this);
    TCRininitial();
    Dataqueryintial();
    Proebeinitial();
    CalibrateInitial();
    errorHandlerInitial();
    this->setWindowTitle(tr("TCR 测量"));
    setWindowIcon(QIcon(QStringLiteral(":/Iccon/runflag")));

#ifdef DEBUG_NI
        ui->ver_label->setText("V"+verisonno+"d");
#else
        ui->ver_label->setText("V"+verisonno);
#endif







}

MainWindow::~MainWindow()
{
    m_Dthread.quit();
    m_Dthread.wait();
    m_Sthread.quit();
    m_Sthread.wait();
    delete ui;

}



void MainWindow::on_TCRsetting_pushButton_clicked()
{
    m_TCRSettingDialog->exec();

}

void MainWindow::TCRininitial()
{
     ui->TCRmeasure_pushButton->setEnabled(false);


     connect(m_TCRSettingDialog,&TCRsettingDialog::sendTCRsetting,this,&MainWindow::enableTCRmeasure);//解除测试限制



     connect(m_TCRSettingDialog,&TCRsettingDialog::sendTCRsetting,m_TCRMDialog,&TCRmeasureDialog::GetTCRsetting);//传送测量信息
     connect(m_TCRselDialog,&TCRselDialog::baseRinfo,m_switch,&Switch::reciveInfo);//传送测量板信息
     connect(m_TCRselDialog,&TCRselDialog::baseRinfo,m_TCRMDialog,&TCRmeasureDialog::reciveInfo);//传送测量板信息
     connect(m_TCRselDialog,&TCRselDialog::baseRinfo,m_TCRMDialog,&TCRmeasureDialog::show);//切换测试界面



     connect(m_TCRMDialog,&TCRmeasureDialog::starmeasure,m_switch,&Switch::TCRMeasureplus);

     connect(m_TCRMDialog,SIGNAL(stmeasuretoDMM(short,short)),m_dmm,SLOT(ReadyMeasure(short,short)));

     connect(m_dmm,SIGNAL(MeasureFinish()),m_switch,SLOT(TCRMeasureplus()));//dmm 测量完毕回传switch

     connect(m_switch,&Switch::TCRsegFmeasure,m_TCRMDialog,&TCRmeasureDialog::intervalDeal);//switch单片完成中间处理

     connect(m_switch,&Switch::TCRcyclefinish,m_TCRMDialog,&TCRmeasureDialog::Cyclefinish);//switch周期结束处理


     connect(m_switch,&Switch::TCRlampIndicator,m_TCRMDialog,&TCRmeasureDialog::setTCRlampindicator);//TCR指示灯
     connect(m_TCRMDialog,&TCRmeasureDialog::endcycleNotion,m_TCRselDialog,&TCRselDialog::InitialTest);//TCR测试完成，重新更新选择界面

     connect(m_TCRMDialog,&TCRmeasureDialog::transModbusError,this,&MainWindow::errorTransimit);
     connect(m_TCRMDialog,&TCRmeasureDialog::accessTCRM,this,&MainWindow::accessTCRMbutton);//测试时禁用TCR按键
     for(int i=0;i<3;i++)
     {
         m_TCRMDialog->setData(i,m_dmm->GetData(i));
         //qDebug()<<m_dmm->GetData(i);
     }
     m_TCRMDialog->setData(5,m_dmm->GetData(5));

}

void MainWindow::Dataqueryintial()
{

    connect(m_datadialog,SIGNAL(TCRTimeinfo(QDateTime,QDateTime)),this,SLOT(setTCRresult(QDateTime,QDateTime)));
    connect(m_datadialog,SIGNAL(TCRNameinfo(QString)),this,SLOT(setTCRresult(QString)));
    connect(m_datadialog,SIGNAL(ProbeTimeinfo(QDateTime,QDateTime)),this,SLOT(setProberesult(QDateTime,QDateTime)));
    connect(m_datadialog,SIGNAL(ProbeNameinfo(QString)),this,SLOT(setProberesult(QString)));
    connect(m_datadialog,&TCRdataDialog::deleteData,this,&MainWindow::deleteData);

    qreal bdata[2]={0};

    if(ConfigFileinfoDeal(bdata))
    {
        m_datadialog->setCalibrate();
        m_datadialog->setCoefficient(bdata);
        m_TCRMDialog->setCalibrate();
        m_TCRMDialog->setCoefficient(bdata);
        qDebug()<<"apply biase";
        return;
    }
    qDebug()<<"no apply biase";


}

void MainWindow::enableTCRmeasure()
{
    ui->TCRmeasure_pushButton->setEnabled(true);
}

void MainWindow::Proebeinitial()
{
        connect(m_ProbeDialog,&ProbeTestDialog::Psmeasure,m_switch,&Switch::ProbeMeasure);
        //connect(m_ProbeDialog,SIGNAL(triggerDMM(int,int)),m_dmm,SLOT(ReadyMeasure(int,int)));
        connect(m_ProbeDialog,SIGNAL(triggerDMM(int,int,short)),m_dmm,SLOT(ReadyMeasure(int,int,short)));
        connect(m_dmm,SIGNAL(MeasureFinish(int,bool)),m_switch,SLOT(ProbeMeasure   (int,bool)));
        connect(m_switch,SIGNAL(ProbelampIndicator(int,bool,int)),m_ProbeDialog,SLOT(SetRlamp(int,bool,int)));        
        connect(m_TCRMDialog,&TCRmeasureDialog::banProbe,m_ProbeDialog,&ProbeTestDialog::banTest);
        connect(m_TCRMDialog,&TCRmeasureDialog::accessProbe,m_ProbeDialog,&ProbeTestDialog::accessTest);


        m_ProbeDialog->SetData(0,m_dmm->GetT4T5Data(3));
        m_ProbeDialog->SetData(1,m_dmm->GetT4T5Data(4));



}

void MainWindow::CalibrateInitial()
{
    connect(m_caldialog,&CalibrateDialog::Csmeasure,m_switch,&Switch::CalibrMeasure);
    connect(m_dmm,&Dmm::CaliMeasureFinish,m_switch,&Switch::CalibrMeasure);
    connect(m_switch,SIGNAL(StartingMeasure(int,int)),m_dmm,SLOT(ReadyMeasure(int,int)));
    connect(m_switch,&Switch::CalibraFinish,m_caldialog,&CalibrateDialog::MfinishDeal);
    connect(m_caldialog,&CalibrateDialog::RecordData,this,&MainWindow::saveCalibradata);
    m_caldialog->Setdataadress(0,m_dmm->GetT4T5Data(3));
    m_caldialog->Setdataadress(1,m_dmm->GetT4T5Data(4));

}

void MainWindow::ThreadInitial()
{
    m_dmm->moveToThread(&m_Dthread);
    m_switch->moveToThread(&m_Sthread);
    connect(&m_Dthread, &QThread::finished,m_dmm,&Dmm::deleteLater);
    connect(&m_Sthread,&QThread::finished,m_switch,&Switch::deleteLater);
    m_Dthread.start();
    m_Sthread.start();

}

bool MainWindow::ConfigFileinfoDeal(qreal *coef)
{
   QString Path=QDir::currentPath();
   QString filename="config.ini";
   Path+="/"+filename;
   //qDebug()<<Path;
   QFile dealfile;
   dealfile.setFileName(Path);
   if(dealfile.exists())
   {
       QStringList Origdata;
       if(dealfile.open(QFile::ReadOnly|QIODevice::Text))
       {
           QTextStream in(&dealfile);
           while(!in.atEnd())
         {

           Origdata.push_back(in.readLine());


         }
           QStringList  biasconfi=Origdata.at(0).split(":");
           QString result=biasconfi.at(1);
           qDebug()<<"first info "<<biasconfi.at(0);
           qDebug()<<"result "<<result;
           if(result.toInt()==1)
           {
               dealfile.close();
               QStringList tempdata=Origdata.at(1).split(":");
               qreal bias=tempdata.at(1).toDouble();
               tempdata.clear();
               tempdata=Origdata.at(2).split(":");
               qreal core=tempdata.at(1).toDouble();
               (*coef)=bias;
               *(coef+1)=core;
               return true;

           }
           else
           {
               dealfile.close();
               return false;


           }


       }
       QMessageBox::warning(NULL,tr("警告！"),tr("配置文件打开失败"),QMessageBox::Yes);
       return false;
   }
   else
   {
       if(dealfile.open(QFile::WriteOnly|QIODevice::Truncate|QIODevice::Text))
       {
           QTextStream out(&dealfile);
           out<<"Bias config:1"<<endl;
           out<<"Rbias:3200000000"<<endl;
           out<<"Rcore:4000000000"<<endl;
           dealfile.close();
           (*coef)=3200000000;
           *(coef+1)=4000000000;
           return true;

       }
       QMessageBox::warning(NULL,tr("警告！"),tr("配置文件创建失败"),QMessageBox::Yes);
       return false;

   }
   //return dealfile.exists();


}

bool MainWindow::errorLogger(const QString &info)
{
    QString Path=QDir::currentPath();
    QString filename="error.log";
    Path+="/"+filename;
    //qDebug()<<Path;
    QFile dealfile;
    dealfile.setFileName(Path);
    if(dealfile.exists())
    {

        if(dealfile.open(QIODevice::WriteOnly|QIODevice::Append|QIODevice::Text))
        {
            QTextStream outstream(&dealfile);
            QString currentTime=QTime::currentTime().toString();
            outstream<<currentTime<<" "<<info<<endl;
            return true;

        }
        QMessageBox::warning(NULL,tr("警告！"),tr("故障文件打开失败"),QMessageBox::Yes);
        return false;
    }
    else
    {
        if(dealfile.open(QFile::WriteOnly|QIODevice::Append|QIODevice::Text))
        {
            QTextStream outstream(&dealfile);
            QString currentTime=QTime::currentTime().toString();
            outstream<<currentTime<<" "<<info<<endl;
            return true;

        }
        QMessageBox::warning(NULL,tr("警告！"),tr("故障文件创建失败"),QMessageBox::Yes);
        return false;

    }



}

void MainWindow::errorHandlerInitial()
{
    connect(m_switch,&Switch::sendError,this,&MainWindow::errorTransimit);
    connect(m_dmm,&Dmm::sendError,this,&MainWindow::errorTransimit);

}

void MainWindow::setTCRresult(QDateTime st, QDateTime et)
{
    QList<QStringList> temp;
        if(m_dataop->Timequery(st,et,temp,Tabletype::TCR))
        {
            qDebug()<<temp.count();
            qDebug()<<"query tTime successful!";

            m_datadialog->setTabelData(temp);
        }
        else{
            temp.clear();
            m_datadialog->setTabelData(temp);
            qDebug()<<"query Time  fail ,no result";
        }

}

void MainWindow::setTCRresult(QString name)
{
    QList<QStringList> temp;
        if(m_dataop->Namequery(name,temp,Tabletype::TCR))
        {
            qDebug()<<temp.count();
            qDebug()<<"query tname successful!";
            m_datadialog->setTabelData(temp);
        }
        else{
            temp.clear();
            m_datadialog->setTabelData(temp);
            qDebug()<<"query name fail ,no result";
        }

}

void MainWindow::setProberesult(QDateTime st, QDateTime et)
{
    QList<QStringList> temp;
    qDebug()<<"table type"<<(static_cast<int>(Tabletype::PROBE));
        if(m_dataop->Timequery(st,et,temp,Tabletype::PROBE))
        {
            qDebug()<<temp.count();
            qDebug()<<"query Time successful!";

            m_datadialog->setTabelData(temp);
        }
        else{
            temp.clear();
            m_datadialog->setTabelData(temp);
            qDebug()<<"query Time  fail ,no result";
        }


}

void MainWindow::setProberesult(QString name)
{
    QList<QStringList> temp;
        if(m_dataop->Namequery(name,temp,Tabletype::PROBE))
        {
            qDebug()<<temp.count();
            qDebug()<<"query name successful!";
            m_datadialog->setTabelData(temp);
        }
        else{
            temp.clear();
            m_datadialog->setTabelData(temp);
            qDebug()<<"query name fail ,no result";
        }

}

void MainWindow::deleteData(int T_id, Tabletype type)
{
    m_dataop->deleteData(T_id,type);

}

void MainWindow::saveCalibradata(QVector<qreal> tempdata)
{
    qDebug()<<"savea data ";
    m_dataop->saveCalibdata(tempdata);

}

void MainWindow::errorTransimit(QString info, int dialogNo)
{
    qDebug()<<"mainwindow error handler dialogno is"<<dialogNo<<"Error info is "<<info;
    switch(dialogNo){
    case 0:
        qDebug()<<"TCR error";
        m_TCRMDialog->errorHandler(info);
        break;
    case 1:
        qDebug()<<"Prob error";
        m_ProbeDialog->errorHandler(info);
        break;
    case 2:
        qDebug()<<"Cali error";
        m_caldialog->errorHandler(info);
        break;
    default:
        break;
    }
    //error.log deal
    errorLogger(info);

}

void MainWindow::accessTCRMbutton(const bool &switches)
{
    if(switches)
    {
        ui->TCRmeasure_pushButton->setEnabled(true);
        ui->TCRsetting_pushButton->setEnabled(true);
    }
    else
    {
        ui->TCRmeasure_pushButton->setEnabled(false);
        ui->TCRsetting_pushButton->setEnabled(false);
    }

}


void MainWindow::on_TCRmeasure_pushButton_clicked()
{
    m_TCRselDialog->exec();
}

void MainWindow::on_Probe_pushButton_clicked()
{
    qreal data[10]={0};
    QVector<qreal> tempdata;
    if(m_dataop->Calibdataquery(tempdata))
    {
        for(int i=0;i<10;i++)
        {
            //qDebug()<<"data info"<<tempdata.at(i+20);
            data[i]=tempdata.at(i+20);
        }
        m_ProbeDialog->setCalidata(data);

    }
    else{
        m_ProbeDialog->setCalidata(data);
            }
    m_dmm->SetProbe();
    m_ProbeDialog->exec();


}

void MainWindow::on_MainExit_pushButton_clicked()
{

    this->close();
}

void MainWindow::on_DataQuery_pushButton_clicked()
{
    m_datadialog->exec();
}

void MainWindow::on_Cali_pushButton_clicked()
{
    QVector<qreal> tempdata;
    tempdata.clear();
    if(m_dataop->Calibdataquery(tempdata))
    {
        m_caldialog->SetData(tempdata);
    }
    else{
        for(int i=0;i<30;i++)
        {
            tempdata.append(0);
        }
        m_caldialog->SetData(tempdata);
    }
    m_dmm->SetCalibrate();
    m_caldialog->exec();
}

