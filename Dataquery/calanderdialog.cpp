#include "calanderdialog.h"
#include "ui_calanderdialog.h"

CalanderDialog::CalanderDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CalanderDialog),
    m_time(QDate::currentDate()),
    timeflag(1)
{
    ui->setupUi(this);
}

CalanderDialog::~CalanderDialog()
{
    delete ui;
}

void CalanderDialog::on_pushButton_2_clicked()
{
    //ui->calendarWidget->set
}

void CalanderDialog::on_Confirm_pushButton_clicked()
{
    m_time=ui->calendarWidget->selectedDate();
    this->hide();
}

void CalanderDialog::setTimeflag(short tf)
{
    timeflag=tf;
}

short CalanderDialog::getTimeflag()
{
    return timeflag;
}

QDate CalanderDialog::getDate()
{
    return m_time;
}

void CalanderDialog::on_CrT_pushButton_clicked()
{
    ui->calendarWidget->setSelectedDate(QDate::currentDate());
}
