#ifndef TCRDATAOP_H
#define TCRDATAOP_H

#include <QObject>
#include"dataop.h"
#include<QtMath>

//数据操作派生类，操作TCR数据
class TCRdataop : public Dataop
{
    Q_OBJECT
public:
    TCRdataop(QObject *parent = nullptr);
    ~TCRdataop();
private:
    qreal hcalvalue;//高温计算值
    qreal ncalvalue;//中温计算值
    qreal lcalvalue;//低温计算值
public:
    bool insertRvalue(int T_id, int Rno, double value, int serialno) override;//插入具体电阻值
    //void setTCRcalvalue(qreal htemp,qreal ltemp,qreal ntemp);//设定TCR计算温度
    bool UpdateTCR(int T_id);//更新TCR数据
    bool addcurrentDateTime(int T_id) override;
    bool deleteData(int T_id) override;
    bool createData(QString name, int &T_id,const qreal &temp1,const qreal &temp2,const qreal &temp3);
    void disconnectDB();
    bool insertTempreportValue(QVector<QStringList> data);//临时报表
    bool deleteTempreport();//删除临时报表
    void setcalculateValue(const qreal&ltemp,const qreal &mtemp, const qreal &htemp);//设定TCR计算温度
    bool debugCalTCR(QVector<int> &totaldata);//调试更正TCR数据
private:
    qreal calculateHTCR(qreal Hvalue,qreal Nvalue);//高温TCR计算
    qreal calculateCTCR(qreal Lvalue,qreal Nvalue);//低温TCR计算

};

#endif // TCRDATAOP_H
