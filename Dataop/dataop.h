#ifndef DATAOP_H
#define DATAOP_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QFile>
#include<QDebug>
#include<QDateTime>
//数据操作基类，操作数据库Mydatabase.db
enum class Tabletype{
    TCR,
    PROBE
};

class Dataop :public QObject
{
    Q_OBJECT
public:
     explicit Dataop(QObject *parent = nullptr);
    ~Dataop();
protected:
    QSqlDatabase m_db;
protected:
    bool connect();//连接
    void close();//关闭
    virtual bool insertRvalue(int T_id,int Rno,double value,int serialno);
public:   
    bool Initial();
    bool CreateTabels();
    bool Namequery(QString baseName,QList<QStringList>&result,Tabletype type);
    bool Timequery(QDateTime st,QDateTime et,QList<QStringList>&result,Tabletype type);
    virtual bool addcurrentDateTime(int T_id);
    virtual bool deleteData(int T_id);
    virtual bool createData(QString name, int &T_id);
    //virtual bool createData(QString name, int &T_id);
    bool deleteData(int T_id,Tabletype type);
    bool Calibdataquery(QVector<qreal> &data);
    bool saveCalibdata(QVector<qreal> data);






};

#endif // DATAOP_H
