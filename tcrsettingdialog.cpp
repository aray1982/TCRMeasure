#include "tcrsettingdialog.h"
#include "ui_tcrsettingdialog.h"

TCRsettingDialog::TCRsettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TCRsettingDialog),
    m_settings(TCRsettings())
{
#ifdef  DEBUG_NI
    m_settings.steadyTime1=1;
    m_settings.steadyTime2=1;
#endif
    //qDebug()<<m_settings.Isprint;
    ui->setupUi(this);
    Initial();
}

TCRsettingDialog::~TCRsettingDialog()
{
    delete ui;
}

void TCRsettingDialog::Initial()
{
    ui->stage1stemp_lineEdit->setText(QString::number(m_settings.SV1));
    ui->stage2stemp_lineEdit->setText(QString::number(m_settings.SV2));
    ui->stage3stemp_lineEdit->setText(QString::number(m_settings.SV3));
    ui->stage1ctemp_lineEdit->setText(QString::number(m_settings.CalV1));
    ui->stage2ctemp_lineEdit->setText(QString::number(m_settings.CalV2));
    ui->stage3ctemp_lineEdit->setText(QString::number(m_settings.CalV3));

    ui->steadytime1_lineEdit->setText(QString::number(m_settings.steadyTime1));
    ui->steadytime2_lineEdit->setText(QString::number(m_settings.steadyTime2));

    ui->Dtime_lineEdit->setText(QString::number(m_settings.Dovertime));
    ui->Utime1_lineEdit->setText(QString::number(m_settings.Uovertime1));
    ui->Utime2_lineEdit->setText(QString::number(m_settings.Uovertime2));
    //qDebug()<<"Is print "<<m_settings.Isprint;
    ui->print_checkBox->setChecked(m_settings.Isprint);
    this->setWindowTitle(tr("TCR设定"));


}

void TCRsettingDialog::on_Tempsave_pushButton_clicked()
{
    m_settings.SV1=ui->stage1stemp_lineEdit->text().toDouble();
    m_settings.SV2=ui->stage2stemp_lineEdit->text().toDouble();
    m_settings.SV3=ui->stage3stemp_lineEdit->text().toDouble();
    m_settings.CalV1=ui->stage1stemp_lineEdit->text().toDouble();
    m_settings.CalV2=ui->stage2stemp_lineEdit->text().toDouble();
    m_settings.CalV3=ui->stage3stemp_lineEdit->text().toDouble();
}

void TCRsettingDialog::on_Stimesave_pushButton_clicked()
{
    m_settings.steadyTime1=ui->steadytime1_lineEdit->text().toInt();
    m_settings.steadyTime2=ui->steadytime2_lineEdit->text().toInt();
}

void TCRsettingDialog::on_otimesave_pushButton_clicked()
{
    m_settings.Dovertime=ui->Dtime_lineEdit->text().toInt();
    m_settings.Uovertime1=ui->Utime1_lineEdit->text().toInt();
    m_settings.Uovertime2=ui->Utime2_lineEdit->text().toInt();
}

void TCRsettingDialog::on_close_pushButton_clicked()
{
    emit sendTCRsetting(m_settings);
    close();
}

void TCRsettingDialog::on_printsave_pushButton_clicked()
{
    m_settings.Isprint=ui->print_checkBox->isChecked();
}
