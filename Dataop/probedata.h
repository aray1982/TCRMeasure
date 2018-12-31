#ifndef PROBEDATA_H
#define PROBEDATA_H

#include <QObject>
#include"dataop.h"
#include<QtMath>
//数据操作派生类，操作Probe数据

class Probedata : public Dataop
{
    Q_OBJECT
public:
    Probedata(QObject *parent = nullptr);
    ~Probedata();
private:
    bool Probecalculate(QVector<qreal> data, qreal &Avg,qreal &CV);//普通版本,准备废弃
    bool ProbecalculatewithEd(QVector<qreal> data, qreal &Avg,qreal &CV);//异常处理版本

public:
    bool insertRvalue(int T_id, int Rno, double value, int serialno) override;//插入具体电阻值
    bool addcurrentDateTime(int T_id) override;//添加时间标记
    bool deleteData(int T_id) override;//删除数据
    bool createData(QString name, int &T_id) override;//创建数据
    void disconnectDB();
    bool singleCalculate(int T_id,int Rno);//单次计算
    bool Isexist(int T_id);
    bool returnData(int T_id,int currentNO,QVector<QVariant>&pbdata,bool&isexist);
    //follow code as debug


};

#endif // PROBEDATA_H
