#ifndef CALIBRATEDIALOG_H
#define CALIBRATEDIALOG_H

#include <QDialog>
#include<QStandardItemModel>
#include"gloabaldefine.h"
#include<QMessageBox>
/*电阻校准对话框
*/

namespace Ui {
class CalibrateDialog;
}

class CalibrateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CalibrateDialog(QWidget *parent = 0);
    ~CalibrateDialog();

private:
    Ui::CalibrateDialog *ui;
    QVector<qreal> m_data;
    qreal* T4data;
    qreal* T5data;
    QStandardItemModel *m_model;
    int m_testplate;
public:
    void SetData(QVector<qreal> data);//传递数据
    void Setdataadress(int i, qreal* data);//传递T4T5地址
    void MfinishDeal(int plate);//测试完成数据处理
    void DisplayError(QString Error);
    void errorHandler(const QString &info);
signals:
    void RecordData(QVector<qreal> data);
    void Csmeasure(int plateno, bool Isfirst);
private slots:
    void on_Calibrate_pushButton_clicked();
    void on_save_pushButton_clicked();
    void on_Cexit_pushButton_clicked();
    void on_Cplate_comboBox_currentIndexChanged(int index);

private:
    qreal Calibracalculate(qreal Rstandard,qreal Rreal);
    void displayData();
    void tempstoreData();
};

#endif // CALIBRATEDIALOG_H
