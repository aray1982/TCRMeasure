#include "dataop.h"


Dataop::Dataop(QObject *parent):QObject(parent)
{

}

Dataop::~Dataop()
{

}

bool Dataop::connect()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("MyDataBase.db");

    m_db.setUserName("Jeff Zhang");
    m_db.setPassword("guanzizai510082");

    return m_db.open();


}

void Dataop::close()
{
    QString connection;
    connection=m_db.connectionName();
    m_db.close();
    m_db=QSqlDatabase();
    m_db.removeDatabase(connection);


}

bool Dataop::insertRvalue(int T_id, int Rno, double value, int serialno)
{
    return true;

}

bool Dataop::Initial()
{
    if(QFile::exists("./MyDataBase.db"))
        {
            return false;//如果存在数据库,即为负
        }

        else
        {
            m_db = QSqlDatabase::addDatabase("QSQLITE");
            m_db.setDatabaseName("MyDataBase.db");
            m_db.setUserName("Jeff Zhang");
            m_db.setPassword("guanzizai510082");
            close();
            return true; //新建数据库，为真

        }

}

bool Dataop::CreateTabels()
{
    QStringList cr_queries;
    cr_queries.append("CREATE TABLE TCRName (\
                          basename   STRING (50) NOT NULL,\
                          recordtime DATETIME,\
                          T_id       INTEGER     PRIMARY KEY AUTOINCREMENT,\
                          Temp1      REAL (1),\
                          Temp2      REAL (1),\
                          Temp3      REAL (1) \
                      );");
        cr_queries.append("CREATE TABLE TCRData (\
                          basename     STRING (50),\
                          resistanceno INT (2),\
                          Rfirst       DOUBLE (6),\
                          Rfirst2      DOUBLE (6),\
                          Rsecond      DOUBLE (6),\
                          Rsecond2     DOUBLE (6),\
                          Rthird       DOUBLE (6),\
                          Rthird2      DOUBLE (6),\
                          C1TCR        DOUBLE (1),\
                          C2TCR        DOUBLE (1),\
                          H1TCR        DOUBLE (1),\
                          H2TCR        DOUBLE (1),\
                          T_id         INTEGER         REFERENCES TCRName (T_id) ON DELETE CASCADE\
                                                                             ON UPDATE CASCADE\
                      );");
        cr_queries.append("CREATE TABLE ProbeName (\
                          batchname  STRING (50),\
                          recordtime DATETIME,\
                          T_id       INTEGER     PRIMARY KEY AUTOINCREMENT\
                      );");
        cr_queries.append("CREATE TABLE ProbeData (\
                          batchname STRING (50),\
                          SerialNo  INT,\
                          R1value   DOUBLE (6),\
                          R2value   DOUBLE (6),\
                          R3value   DOUBLE (6),\
                          R4value   DOUBLE (6),\
                          R5value   DOUBLE (6),\
                          R6value   DOUBLE (6),\
                          R7value   DOUBLE (6),\
                          R8value   DOUBLE (6),\
                          R9value   DOUBLE (6),\
                          R10value  DOUBLE (6),\
                          T_id      INTEGER    REFERENCES ProbeName (T_id) ON DELETE CASCADE\
                                                                           ON UPDATE CASCADE\
                      );");
        cr_queries.append("CREATE TABLE ProbecalData (\
                          batchname STRING (50),\
                          avg1      DOUBLE (6),\
                          avg2      DOUBLE (6),\
                          avg3      DOUBLE (6),\
                          avg4      DOUBLE (6),\
                          avg5      DOUBLE (6),\
                          avg6      DOUBLE (6),\
                          avg7      DOUBLE (6),\
                          avg8      DOUBLE (6),\
                          avg9      DOUBLE (6),\
                          avg10     DOUBLE (6),\
                          cv1       DOUBLE (1),\
                          cv2       DOUBLE (1),\
                          cv3       DOUBLE (1),\
                          cv4       DOUBLE (1),\
                          cv5       DOUBLE (1),\
                          cv6       DOUBLE (1),\
                          cv7       DOUBLE (1),\
                          cv8       DOUBLE (1),\
                          cv9       DOUBLE (1),\
                          cv10      DOUBLE (1),\
                          avg15     DOUBLE (6),\
                          avg610    DOUBLE (6),\
                          cv15      DOUBLE (1),\
                          cv610     DOUBLE (1),\
                          T_id      INTEGER     REFERENCES ProbeName (T_id) ON DELETE CASCADE\
                                                                            ON UPDATE CASCADE\
                      );");
        cr_queries.append("CREATE TABLE Tempreport (\
                          plateno  STRING,\
                          basename STRING,\
                          [1]      DOUBLE (6),\
                          [2]      DOUBLE (6),\
                          [3]      DOUBLE (6),\
                          [4]      DOUBLE (6),\
                          [5]      DOUBLE (6),\
                          [6]      DOUBLE (6),\
                          [7]      DOUBLE (6),\
                          [8]      DOUBLE (6),\
                          [9]      DOUBLE (6),\
                          [10]     DOUBLE (6) \
                      );");
        cr_queries.append("CREATE TABLE CalibrationData (\
                          R1st   DOUBLE (6),\
                          R2st   DOUBLE (6),\
                          R3st   DOUBLE (6),\
                          R4st   DOUBLE (6),\
                          R5st   DOUBLE (6),\
                          R6st   DOUBLE (6),\
                          R7st   DOUBLE (6),\
                          R8st   DOUBLE (6),\
                          R9st   DOUBLE (6),\
                          R10st  DOUBLE (6),\
                          R1rl   DOUBLE (6),\
                          R2rl   DOUBLE (6),\
                          R3rl   DOUBLE (6),\
                          R4rl   DOUBLE (6),\
                          R5rl   DOUBLE (6),\
                          R6rl   DOUBLE (6),\
                          R7rl   DOUBLE (6),\
                          R8rl   DOUBLE (6),\
                          R9rl   DOUBLE (6),\
                          R10rl  DOUBLE (6),\
                          R1cal  DOUBLE (6),\
                          R2cal  DOUBLE (6),\
                          R3cal  DOUBLE (6),\
                          R4cal  DOUBLE (6),\
                          R5cal  DOUBLE (6),\
                          R6cal  DOUBLE (6),\
                          R7cal  DOUBLE (6),\
                          R8cal  DOUBLE (6),\
                          R9cal  DOUBLE (6),\
                          R10cal DOUBLE (6) \
                      );");
        if (!connect())
            {

                qDebug() << "Error: Failed to connect database." << m_db.lastError();
                return false;
            }
            else
            {
                QSqlQuery sql_query;
                for(QStringList::iterator it=cr_queries.begin();it!=cr_queries.end();it++)
                {
                    sql_query.prepare(*it);
                    if(!sql_query.exec())

                    {
                        qDebug()<<*it;
                        qDebug()<<sql_query.lastError();
                        close();
                        return false;
                    }
                    else
                    {
                        qDebug()<<"successful execute "+*it;

                    }

                }
                close();
                return true;




            }



}

bool Dataop::Namequery(QString baseName, QList<QStringList> &result, Tabletype type)
{
    result.clear();
    switch (type) {
    case Tabletype::TCR:
    {
        if(!connect())
            {
                qDebug()<<"fail to connect"<<m_db.lastError();
                return false;
            }
            else{
            QSqlQuery sql_query;

            QString select_sql = "select * from TCRName where basename like '%"+baseName+"%' order by recordtime DESC";
            qDebug()<<select_sql;
            if(!sql_query.exec(select_sql))
            {
                close();
                qDebug()<<sql_query.lastError();
                return false;
            }
            else
            {
                while(sql_query.next())
                {
                    QStringList temp1;
                    temp1.append(sql_query.value(0).toString());
                    temp1.append(sql_query.value(1).toString());
                    temp1.append(sql_query.value(2).toString());
                    temp1.append(sql_query.value(5).toString());
                    result.append(temp1);
                }
                close();
                if(result.isEmpty())
                {
                    qDebug()<<"No result!";
                    return false;
                }
                else{
                return true;
                }

            }
            }
    }

        break;
    case Tabletype::PROBE:
    {
        if(!connect())
            {
                qDebug()<<"fail to connect"<<m_db.lastError();
                return false;
            }
            else{
            QSqlQuery sql_query;

            QString select_sql = "select * from ProbeName where batchname like '%"+baseName+"%' order by recordtime DESC";
            qDebug()<<select_sql;
            if(!sql_query.exec(select_sql))
            {
                close();
                qDebug()<<sql_query.lastError();
                return false;
            }
            else
            {
                while(sql_query.next())
                {
                    QStringList temp1;
                    temp1.append(sql_query.value(0).toString());
                    temp1.append(sql_query.value(1).toString());
                    temp1.append(sql_query.value(2).toString());
                    result.append(temp1);
                }
                close();
                if(result.isEmpty())
                {
                    qDebug()<<"No result!";
                    return false;
                }
                else{
                    qDebug()<<result.at(0).at(0);
                return true;
                }

            }
            }
    }

        break;
    default:
        break;
    }

}

bool Dataop::Timequery(QDateTime st, QDateTime et, QList<QStringList> &result, Tabletype type)
{

    qDebug()<<"is type"<<(static_cast<int>(type));
    switch (type) {
    case Tabletype::TCR:
    {
        if(!connect())
            {
                qDebug()<<"fail to connect"<<m_db.lastError();
                return false;
            }
            else{
            QSqlQuery sql_query;

            QString select_sql = "select * from TCRName where recordtime >datetime('"+st.toString("yyyy-MM-dd hh:mm:ss")+"') and recordtime <datetime('"+et.toString("yyyy-MM-dd hh:mm:ss")+"') order by recordtime DESC";
            qDebug()<<select_sql;
            result.clear();
            if(!sql_query.exec(select_sql))
            {
                close();
                qDebug()<<sql_query.lastError();
                return false;
            }
            else
            {
                while(sql_query.next())
                {

                    QStringList temp1;
                    temp1.append(sql_query.value(0).toString());
                    temp1.append(sql_query.value(1).toString());
                    temp1.append(sql_query.value(2).toString());
                    temp1.append(sql_query.value(5).toString());
                    result.append(temp1);
                }
                close();
                if(result.isEmpty())
                {
                    qDebug()<<"No result!";
                    return false;
                }
                else{
                return true;
                }

            }
            }
    }

        break;
    case Tabletype::PROBE:
    {
        if(!connect())
            {
                qDebug()<<"fail to connect"<<m_db.lastError();
                return false;
            }
            else{
            QSqlQuery sql_query;

            QString select_sql = "select * from ProbeName where recordtime >datetime('"+st.toString("yyyy-MM-dd hh:mm:ss")+"') and recordtime <datetime('"+et.toString("yyyy-MM-dd hh:mm:ss")+"') order by recordtime DESC";
            qDebug()<<select_sql;
            result.clear();
            if(!sql_query.exec(select_sql))
            {
                close();
                qDebug()<<sql_query.lastError();
                return false;
            }
            else
            {
                while(sql_query.next())
                {

                    QStringList temp1;
                    temp1.append(sql_query.value(0).toString());
                    temp1.append(sql_query.value(1).toString());
                    temp1.append(sql_query.value(2).toString());
                    result.append(temp1);
                }
                close();
                if(result.isEmpty())
                {
                    qDebug()<<"No result!";
                    return false;
                }
                else{
                return true;
                }

            }
            }
    }

        break;
    default:
        break;
    }


}

bool Dataop::addcurrentDateTime(int T_id)
{
     return true;

}

bool Dataop::deleteData(int T_id)
{
   return true;


}



bool Dataop::createData(QString name, int &T_id)
{
    return true;
}

bool Dataop::deleteData(int T_id, Tabletype type)
{
    QString foreingON="PRAGMA foreign_keys = ON";
    QString delp="delete from ProbeName where T_id="+QString::number(T_id);
    QString delt="delete from TCRName where T_id="+QString::number(T_id);
    switch (type) {
    case Tabletype::TCR:
    {
        if(!connect())
            {
                qDebug()<<"fail to connect"<<m_db.lastError();
                return false;
            }
            else{
            QSqlQuery sql_query;

            qDebug()<<delt;
            if(!sql_query.exec(foreingON))
            {
                close();
                qDebug()<<sql_query.lastError();
                return false;
            }
            else
            {
                sql_query.clear();
                sql_query.prepare(delt);
                if(!sql_query.exec())
                {
                    close();
                    qDebug()<<sql_query.lastError();
                    return false;

                }
                else
                {
                    close();
                    qDebug()<<"delete successful!";
                    return true;

                }




            }
            }
    }

        break;
    case Tabletype::PROBE:
    {
        if(!connect())
            {
                qDebug()<<"fail to connect"<<m_db.lastError();
                return false;
            }
            else{
            QSqlQuery sql_query;

            qDebug()<<delp;
            if(!sql_query.exec(foreingON))
            {
                close();
                qDebug()<<sql_query.lastError();
                return false;
            }
            else
            {
                sql_query.clear();
                sql_query.prepare(delp);
                if(!sql_query.exec())
                {
                    close();
                    qDebug()<<sql_query.lastError();
                    return false;

                }
                else
                {
                    close();
                    qDebug()<<"delete successful!";
                    return true;

                }




            }
            }
    }

        break;
    default:
        break;
    }


}

bool Dataop::Calibdataquery(QVector<qreal> &data)
{
    data.clear();
    if(!connect())
        {
            qDebug()<<"fail to connect"<<m_db.lastError();
            return false;
        }
        else{
        QSqlQuery sql_query;

        QString select_sql = "select * from CalibrationData";
        qDebug()<<select_sql;
        if(!sql_query.exec(select_sql))
        {
            close();
            qDebug()<<sql_query.lastError();
            return false;
        }
        else
        {
            while(sql_query.next())
            {
                for(int i=0;i<30;i++)
                {
                    data.append(sql_query.value(i).toDouble());
                }
            }
            close();
            if(data.isEmpty())
            {
                qDebug()<<"No result!";
                return false;
            }
            else
            {
            return true;
            }

        }
        }

}

bool Dataop::saveCalibdata(QVector<qreal> data)
{
    if(!connect())
        {
            qDebug()<<"fail to connect"<<m_db.lastError();
            return false;
        }
        else
    {
        QSqlQuery sql_query;

        QString delete_sql="delete from CalibrationData";
        QString insert_sql="insert into CalibrationData values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";

        if(!sql_query.exec(delete_sql))
        {
            close();
            qDebug()<<sql_query.lastError();
            return false;
        }
        else
        {
            sql_query.clear();
            sql_query.prepare(insert_sql);
            for(int i=0;i<30;i++)
            {
                sql_query.bindValue(i,QVariant(data.at(i)));
            }
            if(!sql_query.exec())
            {
                close();
                qDebug()<<sql_query.lastError();
                return false;
            }
            else
            {
                close();
                return true;
            }

        }

    }


}





