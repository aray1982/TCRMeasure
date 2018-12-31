#include "probetestdialog.h"
#include "ui_probetestdialog.h"
#include<QDebug>
#include<QMessageBox>

ProbeTestDialog::ProbeTestDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProbeTestDialog),
    m_Scene(new QGraphicsScene(this)),
    m_Screen(new DisplayOp(*m_Scene,this)),
    m_Model(new QStandardItemModel(this)),
    m_pdataop(new Probedata(this)),
    currentT_id(-1),
    m_pdialog(new QPrintDialog(this)),
    intervalTime(1500),
    m_warningBox(new QMessageBox(this)),
    m_recordBox(new QMessageBox(this)),
    isTCRnotify(false),
    currentplateNo(-1)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("外部测量"));
    m_warningBox->setText(tr("TCR测试进行中"));
    m_recordBox->setText(tr("记录数据?"));
    m_recordBox->addButton(QMessageBox::Yes);
    m_recordBox->addButton(QMessageBox::No);
    //ui->graphicsView->setScene(m_Scene);
    Initial();

}

ProbeTestDialog::~ProbeTestDialog()
{
    //delete m_Screen;
    delete ui;

}

void ProbeTestDialog::on_Pmeasure_pushButton_clicked()
{
    int plateno=ui->Plate_comboBox->currentIndex()+1;
    //qDebug()<<plateno;
    if(ui->batchName_lineEdit->text()=="")
    {
        QMessageBox::warning(NULL,tr("警告!"),tr("没有编号"),QMessageBox::Yes);
    }
    else
    {
        m_dats.measuretimes++;
        ui->testtimes_label->setText(QString::number(m_dats.measuretimes));
        ui->batchName_lineEdit->setEnabled(false);
        ButtonAccess(false);

        emit Psmeasure(plateno,true);

    }



}

void ProbeTestDialog::Initial()
{
    m_Screen->SetName("T4");
    ui->graphicsView->setScene(m_Scene);
    m_Model->setColumnCount(2);
    m_Model->setHeaderData(0,Qt::Horizontal,QString(tr("电阻编号")));
    m_Model->setHeaderData(1,Qt::Horizontal,QString(tr("测量阻值")));
    ui->tableView->setModel(m_Model);
    ui->tableView->setColumnWidth(0,80);
    ui->tableView->setColumnWidth(1,120);
    m_dats.currentbaseno=1;
    m_dats.measuretimes=0;
    ui->baseno_label->setText(QString::number(m_dats.currentbaseno));
    ui->testtimes_label->setText(QString::number(m_dats.measuretimes));
    ui->batchName_lineEdit->setEnabled(true);
    ui->batchName_lineEdit->setText("");
    Isrecord=false;
    currentT_id=-1;
    isTCRnotify=false;


}

void ProbeTestDialog::RecordData(int i)
{
    qreal* tempdata;
    qreal calibradata[10];
    if(i==1)
    {
        tempdata=T4data;
    }
    else
    {
        tempdata=T5data;
    }
    if(ui->Bias_checkBox->isChecked())
    {
        for(int i=0;i<10;i++)
        {
            calibradata[i]=Calibratecalculate(*(tempdata+i),calidata[i]);
        }
        tempdata=calibradata;
    }



    if (m_recordBox->exec()==QMessageBox::Yes)
        {
        //ui->Pmeasure_pushButton->setEnabled(false);
        if(currentT_id==-1)
        {
            if(m_pdataop->createData(ui->batchName_lineEdit->text(),currentT_id))
            {
                qDebug()<<"c_id is"<<currentT_id;
                for(int i=0;i<10;i++)
                {
                    m_pdataop->insertRvalue(currentT_id,i+1,*(tempdata+i),ui->baseno_label->text().toInt());
                }
                for(int i=0;i<10;i++)
                {
                    if(m_pdataop->singleCalculate(currentT_id,i+1))
                    {
                        continue;
                    }
                    else
                    {
                        qDebug()<<"Calculate probe fail!";
                        break;
                    }

                }
                if(m_pdataop->singleCalculate(currentT_id,15))
                {
                    m_pdataop->singleCalculate(currentT_id,610);
                }
                else
                {
                    qDebug()<<"Calculate probe 15 fail!";

                }

            }
        }
        else
        {
            for(int i=0;i<10;i++)
            {
                m_pdataop->insertRvalue(currentT_id,i+1,*(tempdata+i),ui->baseno_label->text().toInt());
            }
            for(int i=0;i<10;i++)
            {
                if(m_pdataop->singleCalculate(currentT_id,i+1))
                {
                    continue;
                }
                else
                {
                    qDebug()<<"Calculate probe fail!";
                    break;
                }

            }
            if(m_pdataop->singleCalculate(currentT_id,15))
            {
                m_pdataop->singleCalculate(currentT_id,610);
            }
            else
            {
                qDebug()<<"Calculate probe 15 fail!";

            }

        }


        if((ui->baseno_label->text().toInt())==m_dats.currentbaseno)
         {
            m_dats.currentbaseno++;
            ui->baseno_label->setText(QString::number(m_dats.currentbaseno));
        }
        QTimer::singleShot(1000,[=](){ButtonAccess(true);});
        //ButtonAccess(true);


        qDebug()<<"clicked yes\n";

        }
        else
        {
        QTimer::singleShot(1000,[=](){ButtonAccess(true);});
        //ButtonAccess(true);


            qDebug()<<"clicked no\n";
        }


}

qreal ProbeTestDialog::Calibratecalculate(qreal Rr, qreal bias)
{
    if(bias==0)
    {
        return Rr;
    }
    else if(Rr==0)
    {
        return Rr;
    }
    else
    {
        return Rr*bias/(bias-Rr);
    }
}

short ProbeTestDialog::recongnitionName(const QString &basename)
{
   const int currentPlateNo=ui->Plate_comboBox->currentIndex();
    if(currentPlateNo==0)
    {
        if(ui->lRoptium_checkBox->isChecked())
        {
            return 1;
        }
        else
        {
            QString Rflag="R-[0123456789][0123456789].*$";

            QRegExp rx(Rflag);
            bool match=rx.exactMatch(basename);
            qDebug()<<"namematch "<<match;
            if(match)
            {
                QString code;
                code.append(basename.at(2));
                code.append(basename.at(3));
                qDebug()<<"code"<<code;
                bool ok=true;
                short codedata=code.toShort(&ok);
                return codedata;
            }
            else
            {
                return 99;
            }

        }
    }
    else
    {
        return 99;
    }


}

void ProbeTestDialog::ButtonAccess(bool IsEnable)
{
    if(IsEnable)
    {
        ui->Pmeasure_pushButton->setEnabled(true);
        ui->Exit_pushButton->setEnabled(true);
        ui->Print_pushButton->setEnabled(true);
        ui->lRoptium_checkBox->setEnabled(true);
        ui->Bias_checkBox->setEnabled(true);
        ui->Plate_comboBox->setEnabled(true);
        ui->add1_pushButton->setEnabled(true);
        ui->add10_pushButton->setEnabled(true);
        ui->sub1_pushButton->setEnabled(true);
        ui->sub10_pushButton->setEnabled(true);
    }
    else
    {
        ui->Pmeasure_pushButton->setEnabled(false);
        ui->Exit_pushButton->setEnabled(false);
        ui->Print_pushButton->setEnabled(false);
        ui->lRoptium_checkBox->setEnabled(false);
        ui->Bias_checkBox->setEnabled(false);
        ui->Plate_comboBox->setEnabled(false);
        ui->add1_pushButton->setEnabled(false);
        ui->add10_pushButton->setEnabled(false);
        ui->sub1_pushButton->setEnabled(false);
        ui->sub10_pushButton->setEnabled(false);
    }
}

void ProbeTestDialog::SetRlamp(int rno, bool switches, int plateno)
{
    if(rno==1)
    {
      m_Screen->Changelampcolor(rno,true);
      qDebug()<<"trigger "<<rno;
      short optimizecode=recongnitionName(ui->batchName_lineEdit->text());
      QTimer::singleShot(intervalTime,[this,rno,plateno,optimizecode](){emit triggerDMM(plateno,rno,optimizecode);});

      //emit triggerDMM(plateno,rno);
    }
    else
    {
        if(switches)
        {
            qDebug()<<"switch true";
            m_Screen->Changelampcolor(rno-1,false);
            m_Screen->Changelampcolor(rno,true);
            qDebug()<<"trigger "<<rno;
            short optimizecode=recongnitionName(ui->batchName_lineEdit->text());
            QTimer::singleShot(intervalTime,[this,rno,plateno,optimizecode](){emit triggerDMM(plateno,rno,optimizecode);});

        }
        else
        {
            qDebug()<<"switch false";
            m_Screen->Changelampcolor(10,false);
            finishDatadeal(plateno);
        }


    }






}

void ProbeTestDialog::DisplayError(QString Error)
{
    QMessageBox::warning(NULL,tr("警告"),Error,QMessageBox::Yes);

}

void ProbeTestDialog::errorHandler(const QString &info)
{
    QMessageBox::warning(NULL,tr("警告"),info,QMessageBox::Yes);
    m_Screen->closelamps();

}

void ProbeTestDialog::setCalidata(qreal *data)
{
    for(int i=0;i<10;i++)
    {

        calidata[i]=*(data+i);
         //qDebug()<<"calidata "<<calidata[i];
    }

}

void ProbeTestDialog::banTest()
{
    ui->Pmeasure_pushButton->setVisible(false);

    //m_warningBox->setWindowFlags(Qt::WindowStaysOnTopHint);
    m_warningBox->exec();
    //m_warningBox->setWindowFlags(Qt::WindowStaysOnTopHint);
    //m_warningBox->move(QPoint(0,100));


}

void ProbeTestDialog::accessTest()
{
    ui->Pmeasure_pushButton->setVisible(true);
    m_warningBox->hide();
    if(isTCRnotify)
    {
        RecordData(currentplateNo);
        isTCRnotify=false;

    }

}



void ProbeTestDialog::SetData(int i, qreal *data)
{
    switch (i) {
    case 0:
        T4data=data;
        break;
    case 1:
        T5data=data;
        break;
    default:
        break;
    }

}


void ProbeTestDialog::finishDatadeal(int plateno)
{
    qreal tempdata[10];

    switch(plateno){
    case 1:
    {
        for(int i=0;i<10;i++)
        {
            tempdata[i]=Calibratecalculate(*(T4data+i),calidata[i]);
        }
        if(ui->Bias_checkBox->isChecked())
        {
            for(int i=0;i<10;i++)
            {
                m_Model->setItem(i,0,new QStandardItem("R"+QString::number(i+1)));

                if((*(T4data+i))<=0)
                {
                    m_Model->setItem(i,1,new QStandardItem(tr("阻值异常")));
                }
                else
                {
                    QString str=QString("%1").arg(tempdata[i],0, 'E',5);
                    m_Model->setItem(i,1,new QStandardItem(str));
                }





            }

        }
        else
        {
            for(int i=0;i<10;i++)
            {
                m_Model->setItem(i,0,new QStandardItem("R"+QString::number(i+1)));

                if((*(T4data+i))<=0)
                {
                    m_Model->setItem(i,1,new QStandardItem(tr("阻值异常")));
                }
                else
                {
                    QString str=QString("%1").arg(*(T4data+i),0, 'E',5);
                    m_Model->setItem(i,1,new QStandardItem(str));
                }



            }

        }

    }


        break;
    case 2:
    {
        for(int i=0;i<10;i++)
        {
            tempdata[i]=Calibratecalculate(*(T5data+i),calidata[i]);
        }
        if(ui->Bias_checkBox->isChecked())
        {
            for(int i=0;i<10;i++)
            {
                m_Model->setItem(i,0,new QStandardItem("R"+QString::number(i+1)));
                //m_Model->setItem(i,1,new QStandardItem(QString::number(tempdata[i])));
                if((*(T5data+i))<=0)
                {
                    m_Model->setItem(i,1,new QStandardItem(tr("阻值异常")));
                }
                else
                {
                    QString str=QString("%1").arg(tempdata[i],0, 'E',5);
                    m_Model->setItem(i,1,new QStandardItem(str));
                }


            }

        }
        else
        {
            for(int i=0;i<10;i++)
            {
                m_Model->setItem(i,0,new QStandardItem("R"+QString::number(i+1)));
                //m_Model->setItem(i,1,new QStandardItem(QString::number(*(T5data+i))));
                if((*(T5data+i))<=0)
                {
                    m_Model->setItem(i,1,new QStandardItem(tr("阻值异常")));
                }
                else
                {
                    QString str=QString("%1").arg(*(T5data+i),0, 'E',5);
                    m_Model->setItem(i,1,new QStandardItem(str));

                }

            }

        }

    }

        break;
    default:
        break;
    }
    if(m_warningBox->isHidden())
    {
        RecordData(plateno);

    }
    else {
        isTCRnotify=true;
        currentplateNo=plateno;
    }





}



void ProbeTestDialog::Setcurrendata(QVector<QVariant> pbdata, bool isExist)
{

    if(isExist)
    {
        for(int i=0;i<10;i++)
        {
            m_Model->setItem(i,0,new QStandardItem("R"+QString::number(i+1)));
            bool ok=true;
            double ddata=pbdata.at(i).toDouble(&ok);
            QString str=QString("%1").arg(ddata,0, 'E',5);
            //qDebug()<<str;
            //m_Model->setItem(i,1,new QStandardItem(pbdata.at(i).toString()));
            if(ddata<=0)
            {
                m_Model->setItem(i,1,new QStandardItem(tr("阻值异常")));
            }
            else
            {
                m_Model->setItem(i,1,new QStandardItem(str));
            }


        }


    }
    else
    {
        QMessageBox::warning(NULL,tr("警告"),tr("没有数据"),QMessageBox::Yes);
    }

}

void ProbeTestDialog::on_Plate_comboBox_currentIndexChanged(int index)
{
    switch(index){
    case 0:
       m_Screen->SetName("T4");
       ui->lRoptium_checkBox->setVisible(true);
        break;
    case 1:
       m_Screen->SetName("T5");
       ui->lRoptium_checkBox->setVisible(false);
         break;
    default:
        break;
    }
}

void ProbeTestDialog::on_Exit_pushButton_clicked()
{
    Isrecord=true;
    //emit Pmfinish(ui->batchName_lineEdit->text());
    this->hide();
}

void ProbeTestDialog::on_sub1_pushButton_clicked()
{
    QVector<QVariant> tempdata;
    bool isexist=false;
    if((ui->baseno_label->text().toInt())==1)
    {
        ui->baseno_label->setText("1");
        if(m_pdataop->returnData(currentT_id,1,tempdata,isexist))
        {
            Setcurrendata(tempdata,isexist);
        }
        else
        {
            QMessageBox::warning(NULL,tr("提示"),tr("没有数据!"),QMessageBox::Yes);
        }
    }
    else
    {
        ui->baseno_label->setText(QString::number(ui->baseno_label->text().toInt()-1));
        if(m_pdataop->returnData(currentT_id,ui->baseno_label->text().toInt(),tempdata,isexist))
        {
            Setcurrendata(tempdata,isexist);
        }
        else
        {
            QMessageBox::warning(NULL,tr("提示"),tr("没有数据!"),QMessageBox::Yes);
        }

    }
}

void ProbeTestDialog::on_add1_pushButton_clicked()
{
    QVector<QVariant> tempdata;
    bool isexist=false;
    if((ui->baseno_label->text().toInt())>=m_dats.currentbaseno)
    {
        ui->baseno_label->setText(QString::number(m_dats.currentbaseno));

    }
    else
    {
        ui->baseno_label->setText(QString::number(ui->baseno_label->text().toInt()+1));
        if(m_pdataop->returnData(currentT_id,ui->baseno_label->text().toInt(),tempdata,isexist))
        {
            Setcurrendata(tempdata,isexist);
        }
        else
        {
            QMessageBox::warning(NULL,tr("提示"),tr("没有数据!"),QMessageBox::Yes);
        }
    }
}

void ProbeTestDialog::on_sub10_pushButton_clicked()
{
    QVector<QVariant> tempdata;
    bool isexist=false;
    if((ui->baseno_label->text().toInt())<=10)
    {
        ui->baseno_label->setText("1");
        if(m_pdataop->returnData(currentT_id,ui->baseno_label->text().toInt(),tempdata,isexist))
        {
            Setcurrendata(tempdata,isexist);
        }
        else
        {
            QMessageBox::warning(NULL,tr("提示"),tr("没有数据!"),QMessageBox::Yes);
        }
    }
    else
    {
        ui->baseno_label->setText(QString::number(ui->baseno_label->text().toInt()-10));
        if(m_pdataop->returnData(currentT_id,ui->baseno_label->text().toInt(),tempdata,isexist))
        {
            Setcurrendata(tempdata,isexist);
        }
        else
        {
            QMessageBox::warning(NULL,tr("提示"),tr("没有数据!"),QMessageBox::Yes);
        }

    }

}

void ProbeTestDialog::on_add10_pushButton_clicked()
{
    QVector<QVariant> tempdata;
    bool isexist=false;
    if((ui->baseno_label->text().toInt()+10)>=m_dats.currentbaseno)
    {
        ui->baseno_label->setText(QString::number(m_dats.currentbaseno));

    }
    else
    {
        ui->baseno_label->setText(QString::number(ui->baseno_label->text().toInt()+10));
        if(m_pdataop->returnData(currentT_id,ui->baseno_label->text().toInt(),tempdata,isexist))
        {
            Setcurrendata(tempdata,isexist);
        }
        else
        {
            QMessageBox::warning(NULL,tr("提示"),tr("没有数据!"),QMessageBox::Yes);
        }
    }

}

void ProbeTestDialog::hideEvent(QHideEvent *event)
{
    //qDebug()<<"Pb hide";
    if(Isrecord)
    {
        if(ui->batchName_lineEdit->isEnabled())
        {
          qDebug()<<"do nothing";
        }
        else
        {
            qDebug()<<"record data";
            m_pdataop->addcurrentDateTime(currentT_id);
            Initial();

        }

    }
    else
    {
        m_pdataop->deleteData(currentT_id);
        Initial();

    }


    emit closeDialog();
}

void ProbeTestDialog::showEvent(QShowEvent *event)
{
    const int i=ui->Plate_comboBox->currentIndex();
    if(i==0)
    {
        m_Screen->SetName("T4");
    }
    else
    {
        m_Screen->SetName("T5");
    }
    ui->lRoptium_checkBox->setChecked(false);
    ui->Bias_checkBox->setChecked(false);
}



void ProbeTestDialog::on_Print_pushButton_clicked()
{
    m_preport.loadFromFile(":/Report/Prrp");
    QString currentDir=QDir::currentPath();
    qDebug()<<currentDir;
    QString path=currentDir+"/Mydatabase.db";
    qDebug()<<path;
    m_preport.dataManager()->setReportVariable("datasource",path);
    m_preport.dataManager()->setReportVariable("T_id",currentT_id);
    m_preport.previewReport();
}
