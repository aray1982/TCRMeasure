#include "tcrdatadialog.h"
#include "ui_tcrdatadialog.h"
#include<QDir>
#include<QMessageBox>
#include<QPrinterInfo>


TCRdataDialog::TCRdataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCRdataDialog),
    m_Cdialog(new CalanderDialog(this)),
    applyCalibrate(false)

{
    ui->setupUi(this);
    this->setWindowTitle(tr("数据查询"));
    m_Cdialog->installEventFilter(this);
    m_preiew=m_report.createPreviewWidget();
    QGridLayout *playout=new QGridLayout;
    playout->setColumnMinimumWidth(0,1200);
    playout->addWidget(m_preiew);
    QHBoxLayout *boxlayout=new QHBoxLayout;
    boxlayout->addWidget(ui->query_tabWidget,1);
    boxlayout->addLayout(playout,9);
    setLayout(boxlayout);
    //ui->query_tabWidget


}

TCRdataDialog::~TCRdataDialog()
{
    delete ui;
}





void TCRdataDialog::on_St_pushButton_clicked()
{
    m_Cdialog->setTimeflag(1);
    m_Cdialog->exec();//model dialog



}

void TCRdataDialog::on_Et_pushButton_clicked()
{
    m_Cdialog->setTimeflag(2);
    m_Cdialog->exec();


}

bool TCRdataDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj==m_Cdialog){
        if(event->type()==QEvent::Hide){
            qDebug()<<"receive hide";
            short i=m_Cdialog->getTimeflag();
            qDebug()<<"i is "<<i;
            QDateTime time=QDateTime(m_Cdialog->getDate());
            qDebug()<<"QDatetime is "<<time.toString();
            if(ui->query_tabWidget->currentIndex()==0){
                switch (i){
                case 1:
                    ui->St_dateTimeEdit->setDateTime(time);
                    break;
                case 2:
                    ui->Et_dateTimeEdit->setDateTime(time);
                    break;
                }

            }
            else if(ui->query_tabWidget->currentIndex()==1)
            {
                switch (i){
                case 1:
                    ui->PSt_dateTimeEdit->setDateTime(time);
                    break;
                case 2:
                    ui->PEt_dateTimeEdit->setDateTime(time);
                    break;
                }

            }


            return true;
        }
        else{
            return false;
        }
    }
    else {
        return TCRdataDialog::eventFilter(obj,event);
    }

}

void TCRdataDialog::setTabelData(QList<QStringList> result)
{
    qDebug()<<"set Table data";
    if(result.count()==0)
    {
        QMessageBox::warning(NULL,tr("提示!"),tr("没有数据！"),QMessageBox::Yes);
        return;
    }

    if(ui->query_tabWidget->currentIndex()==0){
        int k=ui->tableWidget->rowCount();
        if(k>0)
        {
            for(int j=0;j<k;j++)
            {
                ui->tableWidget->removeRow(0);
            }
        }
        ui->tableWidget->clear();
        QStringList header;
        header<<tr("名称")<<tr("时间")<<tr("T_id")<<tr("高温温度");
        int i=ui->tableWidget->rowCount();
        ui->tableWidget->setColumnCount(4);
        ui->tableWidget->setHorizontalHeaderLabels(header);
        foreach (QStringList singledata, result) {
            ui->tableWidget->insertRow(i);

            ui->tableWidget->setColumnWidth(0,150);
            ui->tableWidget->setItem(i,0,new QTableWidgetItem(singledata.at(0)));
            //qDebug()<<singledata.at(1);
            ui->tableWidget->setColumnWidth(1,150);
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(singledata.at(1)));
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(singledata.at(2)));
            ui->tableWidget->setItem(i,3,new QTableWidgetItem(singledata.at(3)));
            ui->tableWidget->setColumnHidden(2,true);
            ui->tableWidget->setColumnHidden(3,true);

            i++;

        }

    }
    else if(ui->query_tabWidget->currentIndex()==1){
        int k=ui->p_tableWidget->rowCount();
        if(k>0)
        {
            for(int j=0;j<k;j++)
            {
                ui->p_tableWidget->removeRow(0);
            }
        }
        ui->p_tableWidget->clear();
        QStringList header;
        header<<tr("名称")<<tr("时间")<<"T_id";
        int i=ui->p_tableWidget->rowCount();
        ui->p_tableWidget->setColumnCount(3);
        ui->p_tableWidget->setHorizontalHeaderLabels(header);
        foreach (QStringList singledata, result) {
            ui->p_tableWidget->insertRow(i);
            ui->tableWidget->setColumnWidth(0,150);

            ui->p_tableWidget->setItem(i,0,new QTableWidgetItem(singledata.at(0)));
            //qDebug()<<singledata.at(1);
            ui->p_tableWidget->setColumnWidth(1,150);
            ui->p_tableWidget->setItem(i,1,new QTableWidgetItem(singledata.at(1)));
            ui->p_tableWidget->setItem(i,2,new QTableWidgetItem(singledata.at(2)));
            ui->p_tableWidget->setColumnHidden(2,true);

            i++;

        }
    }



}

void TCRdataDialog::setCalibrate()
{
    applyCalibrate=true;

}

void TCRdataDialog::setCoefficient(qreal *data)
{

       coefficientdata[0]=*data;
        coefficientdata[1]=*(data+1);

}

void TCRdataDialog::on_Query_pushButton_clicked()
{
    if(ui->Name_radioButton->isChecked())
    {
        qDebug()<<"name checked";
        emit TCRNameinfo(ui->lineEdit->text());

    }
    else if(ui->Time_radioButton->isChecked())
    {
        qDebug()<<"time checked";
        emit TCRTimeinfo(ui->St_dateTimeEdit->dateTime(),ui->Et_dateTimeEdit->dateTime());
    }
}

void TCRdataDialog::on_Display_pushButton_clicked()
{
    //int j=ui->tableWidget->rowCount();
    if(m_report.reportName()=="")
        return;
    qDebug()<<m_report.reportName();
    QPrinter xprinter(QPrinterInfo::defaultPrinter());
    m_report.printReport(&xprinter);

}

void TCRdataDialog::on_PSt_pushButton_clicked()
{
    m_Cdialog->setTimeflag(1);
    m_Cdialog->exec();//model dialog

}

void TCRdataDialog::on_PEt_pushButton_clicked()
{
    m_Cdialog->setTimeflag(2);
    m_Cdialog->exec();//model dialog
}

void TCRdataDialog::on_PQuery_pushButton_clicked()
{
    if(ui->PName_radioButton->isChecked())
    {
        qDebug()<<"name checked Pb";
        emit ProbeNameinfo(ui->p_lineEdit->text());

    }
    else if(ui->PTime_radioButton->isChecked())
    {
        qDebug()<<"time checked Pb";
        emit ProbeTimeinfo(ui->PSt_dateTimeEdit->dateTime(),ui->PEt_dateTimeEdit->dateTime());
    }

}

void TCRdataDialog::on_PDisplay_pushButton_clicked()
{
    if(m_report.reportName()=="")
        return;
    //m_report.printReport(0);
    QPrinter xprinter(QPrinterInfo::defaultPrinter());
    m_report.printReport(&xprinter);
}

void TCRdataDialog::on_delete_pushButton_clicked()
{
    int j=ui->tableWidget->rowCount();
    if(j==0)
    {
        return;
    }
    int i=ui->tableWidget->currentRow();
    QTableWidgetItem* temp=ui->tableWidget->item(i,2);
    //QTableWidgetItem *temp=ui->tableWidget->currentItem();
    emit deleteData(temp->text().toInt(),Tabletype::TCR);
    ui->tableWidget->removeRow(i);



}

void TCRdataDialog::on_pdelete_pushButton_clicked()
{
    int j=ui->p_tableWidget->rowCount();
    if(j==0)
    {
        return;
    }
    int i=ui->p_tableWidget->currentRow();
    QTableWidgetItem* temp=ui->p_tableWidget->item(i,2);
    //QTableWidgetItem *temp=ui->tableWidget->currentItem();
    emit deleteData(temp->text().toInt(),Tabletype::PROBE);
    ui->p_tableWidget->removeRow(i);



}



void TCRdataDialog::on_tableWidget_cellClicked(int row, int column)
{

    QTableWidgetItem* temp=ui->tableWidget->item(row,2);
    QTableWidgetItem* temp1=ui->tableWidget->item(row,3);
    qreal highTemp=temp1->text().toDouble();
    qDebug()<<"hightem"<<highTemp;
    //QTableWidgetItem *temp=ui->tableWidget->currentItem();
    //对采用报表作判断，如果应用修正且高温段在100度以上，实施修正

        if(applyCalibrate)
        {
            if(highTemp>=100)
            {
                m_report.loadFromFile(":/Report/TCRrpbv");
                m_report.dataManager()->setReportVariable("Rbias",coefficientdata[0]);
                m_report.dataManager()->setReportVariable("Rcore",coefficientdata[1]);

            }
            else
            {
                m_report.loadFromFile(":/Report/TCRrp");
            }


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
        m_report.dataManager()->setReportVariable("T_id",temp->text());



    m_preiew->refreshPages();


}

void TCRdataDialog::on_p_tableWidget_cellClicked(int row, int column)
{

    QTableWidgetItem* temp=ui->p_tableWidget->item(row,2);
    m_report.loadFromFile(":/Report/Prrp");
    QString currentDir=QDir::currentPath();
    //qDebug()<<currentDir;
    QString path=currentDir+"/Mydatabase.db";
    //qDebug()<<path;
    m_report.dataManager()->setReportVariable("datasource",path);

    m_report.dataManager()->setReportVariable("T_id",temp->text());
    m_preiew->refreshPages();

}

void TCRdataDialog::on_pdf_pushButton_clicked()
{
    if(m_report.reportName()=="")
        return;
    int i=ui->tableWidget->currentRow();
    QTableWidgetItem* temp=ui->tableWidget->item(i,0);
    //QTableWidgetItem *temp=ui->tableWidget->currentItem();



    m_report.printToPDF(temp->text());
}

void TCRdataDialog::on_Ppdf_pushButton_clicked()
{
    if(m_report.reportName()=="")
        return;
    int i=ui->p_tableWidget->currentRow();
    QTableWidgetItem* temp=ui->p_tableWidget->item(i,0);
    //QTableWidgetItem *temp=ui->tableWidget->currentItem();



    m_report.printToPDF(temp->text());
}

void TCRdataDialog::showEvent(QShowEvent *event)
{
    QDate currentdate=QDate::currentDate();
    QDate begindate=currentdate.addDays(-30);
    QDate enddate=currentdate.addDays(1);
    ui->St_dateTimeEdit->setDate(begindate);
    ui->Et_dateTimeEdit->setDate(enddate);
    ui->PSt_dateTimeEdit->setDate(begindate);
    ui->PEt_dateTimeEdit->setDate(enddate);
    ui->PTime_radioButton->setChecked(true);
    ui->Time_radioButton->setChecked(true);

}


