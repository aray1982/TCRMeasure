#include "calibratedialog.h"
#include "ui_calibratedialog.h"

CalibrateDialog::CalibrateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalibrateDialog),
    m_model(new QStandardItemModel(this)),
    m_testplate(1)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("阻值校准"));
    m_model->setColumnCount(4);
    m_model->setHeaderData(0,Qt::Horizontal,QString(tr("电阻编号")));
    m_model->setHeaderData(1,Qt::Horizontal,QString(tr("电阻标准值")));
    m_model->setHeaderData(2,Qt::Horizontal,QString(tr("电阻测量值")));
    m_model->setHeaderData(3,Qt::Horizontal,QString(tr("电阻校准值")));
    ui->tableView->setModel(m_model);

}

CalibrateDialog::~CalibrateDialog()
{
    delete ui;
}

void CalibrateDialog::SetData(QVector<qreal> data)
{
    m_data=data;
    displayData();
}

void CalibrateDialog::Setdataadress(int i, qreal *data)
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

void CalibrateDialog::MfinishDeal(int plate)
{
    qreal* tempdata;
    switch (plate) {
    case 1:
        tempdata=T4data;
        break;
    case 2:
        tempdata=T5data;
        break;
    default:
        break;
    }
    for(int i=0;i<10;i++)
    {
        m_data[10+i]=*(tempdata+i);
        qreal standdata=m_data.at(i);
        qreal caldata=Calibracalculate(standdata,*(tempdata+i));
        m_data[20+i]=caldata;

    }
    ui->Calibrate_pushButton->setEnabled(true);
    ui->Cexit_pushButton->setEnabled(true);
    ui->save_pushButton->setEnabled(true);
    displayData();

}

void CalibrateDialog::DisplayError(QString Error)
{
    QMessageBox::warning(NULL,tr("警告"),Error,QMessageBox::Yes);

}

void CalibrateDialog::errorHandler(const QString &info)
{
    QMessageBox::warning(NULL,tr("警告"),info,QMessageBox::Yes);

}

void CalibrateDialog::on_Calibrate_pushButton_clicked()
{
    tempstoreData();
    ui->Calibrate_pushButton->setEnabled(false);
    ui->Cexit_pushButton->setEnabled(false);
    ui->save_pushButton->setEnabled(false);
    emit Csmeasure(m_testplate,true);

}

void CalibrateDialog::on_save_pushButton_clicked()
{
    emit RecordData(m_data);

}

void CalibrateDialog::on_Cexit_pushButton_clicked()
{
    this->hide();


}

qreal CalibrateDialog::Calibracalculate(qreal Rstandard, qreal Rreal)
{
    if(Rstandard==Rreal)
    {
        return 0;
    }
    else
    {
        return Rstandard*Rreal/(Rstandard-Rreal);
    }

}

void CalibrateDialog::displayData()
{
    for(int i=0;i<10;i++)
    {
        m_model->setItem(i,0,new QStandardItem("R"+QString::number(i+1)));
        m_model->setItem(i,1,new QStandardItem(QString::number(m_data.at(i))));
        m_model->setItem(i,2,new QStandardItem(QString::number(m_data.at(i+10))));
        m_model->setItem(i,3,new QStandardItem(QString::number(m_data.at(i+20))));

    }


}

void CalibrateDialog::tempstoreData()
{
    for(int i=0;i<10;i++)
    {
        m_data[i]=m_model->item(i,1)->text().toDouble();
    }

}

void CalibrateDialog::on_Cplate_comboBox_currentIndexChanged(int index)
{
    if(index==0)
    {
        m_testplate=1;

    }
    else
    {
        m_testplate=2;

    }
}
