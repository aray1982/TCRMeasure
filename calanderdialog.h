#ifndef CALANDERDIALOG_H
#define CALANDERDIALOG_H

#include <QDialog>
#include <QDateTime>
/*calanderDialog为日期选择对话框，为数据查询对话框时使用
*/
namespace Ui {
class CalanderDialog;
}

class CalanderDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalanderDialog(QWidget *parent = 0);
    ~CalanderDialog();

private slots:
    void on_pushButton_2_clicked();

    void on_Confirm_pushButton_clicked();

    void on_CrT_pushButton_clicked();

private:
    Ui::CalanderDialog *ui;
    QDate m_time;
    short timeflag;
public slots:
    void setTimeflag(short tf);
public:
    short getTimeflag();
    QDate getDate();
};

#endif // CALANDERDIALOG_H
