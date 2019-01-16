#include "probedata.h"


Probedata::Probedata(QObject *parent):Dataop(parent)
{


}

Probedata::~Probedata()
{


}

bool Probedata::Probecalculate(QVector<qreal> data, qreal &Avg, qreal &CV)
{
    int dsize=data.size();
        qreal sum=0;
        foreach (qreal temp,data) {

                sum+=temp;



        }
        Avg=sum/dsize;
        if(Avg==0)
        {
            return false;
        }
        else
        {

            qreal std=0;
            if(data.size()==1)
            {
                CV=std;
            }
            else
            {
                foreach (qreal atemp, data) {

                        std+=qPow(atemp-Avg,2);




                }

                std/=(dsize-1);
                std=qSqrt(std);

                CV=std/Avg;
                return true;

            }




        }

}

bool Probedata::ProbecalculatewithEd(QVector<qreal> data, qreal &Avg, qreal &CV)
{
    int dsize=data.size();
        qreal sum=0;
        foreach (qreal temp,data) {
            if(temp>0)
            {
                sum+=temp;
            }
            else
            {
                dsize-=1;
                continue;
            }


        }
        Avg=sum/dsize;
        if(Avg==0)
        {
            return false;
        }
        else
        {

            qreal std=0;
            if(data.size()==1)
            {
                CV=std;
            }
            else
            {
                foreach (qreal atemp, data) {
                    if(atemp>0)
                    {
                        std+=qPow(atemp-Avg,2);
                    }
                    else
                    {
                        continue;
                    }



                }

                std/=(dsize-1);
                std=qSqrt(std);

                CV=std/Avg;
                return true;

            }




        }

}

bool Probedata::insertRvalue(int T_id, int Rno, double value, int serialno)
{

    if (!connect())
        {
            qDebug() << "Error: Failed to connect database." << m_db.lastError();
            return false;
        }
        else
        {
        QString findquery="select * from ProbeData where T_id="+QString::number(T_id)+" and SerialNo="+QString::number(serialno);
                QString insertvalue;
                QSqlQuery myquery;
                QString Rsno="R"+QString::number(Rno)+"value";

                insertvalue="UPDATE ProbeData SET "+Rsno+"="+QString::number(value,'g',10)+" where T_id="+QString::number(T_id)+" and SerialNo="+QString::number(serialno);
                qDebug()<<findquery;
                myquery.prepare(findquery);
                if(!myquery.exec())
                {
                    qDebug()<<myquery.lastError();
                    close();
                    return false;

                }
                else
                {
                    if(myquery.next())
                    {
                        myquery.clear();
                        qDebug()<<insertvalue;
                        myquery.prepare(insertvalue);
                        if(!myquery.exec())
                        {
                            qDebug()<<myquery.lastError();
                            close();
                            return false;

                        }
                        else
                        {
                            //qDebug()<<sql_query.next();
                            close();
                            return true;


                        }


                    }
                    else
                    {
                        qDebug()<<"find no serial Pb";
                        QString name;
                        QString getname="select batchname from ProbeName where T_id="+QString::number(T_id);
                        qDebug()<<getname;
                        myquery.clear();
                        myquery.prepare(getname);
                        if(!myquery.exec())
                        {
                            qDebug()<<myquery.lastError();
                            close();
                            return false;

                        }
                        else
                        {
                            while(myquery.next())
                            {
                                name=myquery.value(0).toString();
                            }
                            QString InsertPbsn="insert into ProbeData (batchname,T_id,SerialNo) VALUES('"+name+"',"+QString::number(T_id)+","+QString::number(serialno)+")";
                            qDebug()<<InsertPbsn;
                            myquery.clear();
                            myquery.prepare(InsertPbsn);
                            if(!myquery.exec())
                            {
                                qDebug()<<myquery.lastError();
                                close();
                                return false;

                            }
                            else
                            {
                                myquery.clear();
                                qDebug()<<insertvalue;
                                myquery.prepare(insertvalue);
                                if(!myquery.exec())
                                {
                                    qDebug()<<myquery.lastError();
                                    close();
                                    return false;

                                }
                                else
                                {
                                    //qDebug()<<sql_query.next();
                                    close();
                                    return true;


                                }



                            }



                            }


                        }






                    }
                }




}

bool Probedata::addcurrentDateTime(int T_id)
{
    if (!connect())
        {
            qDebug() << "Error: Failed to connect database." << m_db.lastError();
            return false;
        }
        else
        {
            QDateTime ctime=QDateTime::currentDateTime();
            QString Cdt=ctime.toString("yyyy-MM-dd hh:mm:ss");
            QSqlQuery sql_query;
            QString adt_sql="UPDATE ProbeName SET recordtime='"+Cdt+"' where T_id="+QString::number(T_id);
            qDebug()<<adt_sql;
            sql_query.prepare(adt_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            close();
            return false;

        }
        else
        {
            //qDebug()<<sql_query.next();
            close();
            return true;


        }

        }

}

bool Probedata::deleteData(int T_id)
{
    QString foreingON="PRAGMA foreign_keys = ON";
            QString del="delete from ProbeName where T_id="+QString::number(T_id);
            qDebug()<<del;
            if (!connect())
            {
                qDebug() << "Error: Failed to connect database." << m_db.lastError();
                return false;
            }
            else
            {
             QSqlQuery sql_query;
             sql_query.prepare(foreingON);
             if(sql_query.exec())
             {
                 sql_query.prepare(del);
                 if(sql_query.exec())
                 {
                     qDebug()<<"delete successful!";
                     close();
                     return true;


                 }
                 else
                 {
                     close();
                     return false;
                     qDebug()<<"delete fail!";

                 }

             }
             else
             {
                 close();
                 return false;
                 qDebug()<<"Foreign fail!";

             }
            }

}

bool Probedata::createData(QString name, int &T_id)
{
    if (!connect())
        {
            qDebug() << "Error: Failed to connect database." << m_db.lastError();
            return false;
        }
        else
        {
            QSqlQuery sql_query;
            QString adb_sql="insert into ProbeName (batchname) VALUES('"+name+"')";
            qDebug()<<adb_sql;
            sql_query.prepare(adb_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            close();
            return false;

        }
        else
        {
            QString find_maxid="select MAX(T_id) from ProbeName";

            sql_query.prepare(find_maxid);

                qDebug()<<find_maxid;
                if(!sql_query.exec())
                {
                    qDebug()<<sql_query.lastError();
                    close();
                    return false;
                }
                else
                {
                    int tid=0;
                    while(sql_query.next())
                    {
                        tid=sql_query.value(0).toInt();
                    }
                    T_id=tid;
                    qDebug()<<"max id"<<tid;
                    QString adc_sql="insert into ProbecalData (batchname,T_id) VALUES('"+name+"',"+QString::number(T_id)+")";
                    qDebug()<<adc_sql;
                    sql_query.clear();
                    sql_query.prepare(adc_sql);
                    if(!sql_query.exec())
                    {
                        qDebug()<<sql_query.lastError();
                        close();
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

}

bool Probedata::singleCalculate(int T_id, int Rno)
{
    switch(Rno){
        case 15:
        {
            if (!connect())
            {
                qDebug() << "Error: Failed to connect database." << m_db.lastError();
                return false;
            }
            else
            {
                QSqlQuery sql_query;

                QString adb_sql="select R1value, R2value,R3value,R4value,R5value from ProbeData where T_id="+QString::number(T_id);
                qDebug()<<adb_sql;
                sql_query.prepare(adb_sql);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
                close();
                return false;

            }
            else
            {
                QVector<qreal> tempdata;
                tempdata.clear();
                while(sql_query.next())
                {
                    for(int i=0;i<5;i++)
                    {
                        tempdata.append(sql_query.value(i).toDouble());
                    }

                }
                qreal tempavg=0;
                qreal tempcv=0;
                if(ProbecalculatewithEd(tempdata,tempavg,tempcv))
                {
                    tempcv=tempcv*100;
                    QString adr_sql="UPDATE ProbecalData SET avg15="+QString::number(tempavg,'g',8)+", cv15="+QString::number(tempcv)+" where T_id="+QString::number(T_id);;
                    qDebug()<<adr_sql;
                    sql_query.clear();
                    sql_query.prepare(adr_sql);
                    if(!sql_query.exec())
                    {
                        qDebug()<<sql_query.lastError();
                        close();
                        return false;
                    }
                    else
                    {
                        close();
                        return true;
                    }
                }
                else
                {
                    qDebug()<<"calculate fail!";
                    close();
                    return false;
                }
            }
            }


            break;
        }
        case 610:
        {
            if (!connect())
            {
                qDebug() << "Error: Failed to connect database." << m_db.lastError();
                return false;
            }
            else
            {
                QSqlQuery sql_query;

                QString adb_sql="select R6value, R7value,R8value,R9value,R10value from ProbeData where T_id="+QString::number(T_id);;
                qDebug()<<adb_sql;
                sql_query.prepare(adb_sql);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
                close();
                return false;

            }
            else
            {
                QVector<qreal> tempdata;
                tempdata.clear();
                while(sql_query.next())
                {
                    for(int i=0;i<5;i++)
                    {
                        tempdata.append(sql_query.value(i).toDouble());
                    }

                }
                qreal tempavg=0;
                qreal tempcv=0;
                if(ProbecalculatewithEd(tempdata,tempavg,tempcv))
                {
                    tempcv=tempcv*100;
                    QString adr_sql="UPDATE ProbecalData SET avg610="+QString::number(tempavg,'g',8)+", cv610="+QString::number(tempcv)+" where T_id="+QString::number(T_id);;
                    qDebug()<<adr_sql;
                    sql_query.clear();
                    sql_query.prepare(adr_sql);
                    if(!sql_query.exec())
                    {
                        qDebug()<<sql_query.lastError();
                        close();
                        return false;
                    }
                    else
                    {
                        close();
                        return true;
                    }
                }
                else
                {
                    qDebug()<<"calculate fail!";
                    close();
                    return false;
                }
            }
            }
            break;
        }
        default:
        {
            if (!connect())
            {
                qDebug() << "Error: Failed to connect database." << m_db.lastError();
                return false;
            }
            else
            {
                QSqlQuery sql_query;
                QString dataname="R"+QString::number(Rno)+"value";
                QString adb_sql="select "+dataname+" from ProbeData where T_id="+QString::number(T_id);
                qDebug()<<adb_sql;
                sql_query.prepare(adb_sql);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
                close();
                return false;

            }
            else
            {
                QVector<qreal> tempdata;
                tempdata.clear();
                while(sql_query.next())
                {
                    tempdata.append(sql_query.value(0).toDouble());
                }
                qreal tempavg=0;
                qreal tempcv=0;
                if(ProbecalculatewithEd(tempdata,tempavg,tempcv))
                {
                    tempcv=tempcv*100;
                    QString adr_sql="UPDATE ProbecalData SET avg"+QString::number(Rno)+"="+QString::number(tempavg,'g',8)+", cv"+QString::number(Rno)+"="+QString::number(tempcv)+" where T_id="+QString::number(T_id);
                    qDebug()<<adr_sql;
                    sql_query.clear();
                    sql_query.prepare(adr_sql);
                    if(!sql_query.exec())
                    {
                        qDebug()<<sql_query.lastError();
                        close();
                        return false;
                    }
                    else
                    {
                        close();
                        return true;
                    }
                }
                else
                {
                    qDebug()<<"calculate fail!";
                    close();
                    return false;
                }
            }
            }


            break;
        }
        }

}

bool Probedata::Isexist(int T_id)
{
    if (!connect())
        {
            qDebug() << "Error: Failed to connect database." << m_db.lastError();
            return false;
        }
        else
        {
            QSqlQuery sql_query;
            QString exist_sql="select * from ProbeName where T_id ="+QString::number(T_id);
            qDebug()<<exist_sql;
            sql_query.prepare(exist_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
            close();

            return false;
        }
        else
        {
            if(sql_query.next())
            {
                close();

                return true;

            }
            else
            {
                close();

                return false;
            }


        }

        }

}


bool Probedata::returnData(int T_id, int currentNO, QVector<QVariant> &pbdata, bool &isexist)
{
    pbdata.clear();
    isexist=false;
    if(Isexist(T_id))
        {
            if (!connect())
            {
                qDebug() << "Error: Failed to connect database." << m_db.lastError();
                //emit sendPbdata(pbdata,false);
                return false;
            }
            else
            {
                QString findquery="select * from ProbeData where T_id="+QString::number(T_id)+" and SerialNo="+QString::number(currentNO);

                QSqlQuery myquery;



                qDebug()<<findquery;
                myquery.prepare(findquery);
                if(!myquery.exec())
                {
                    qDebug()<<myquery.lastError();
                    //emit sendPbdata(pbdata,false);
                    close();
                    return false;

                }
                else
                {
                    if(myquery.next())
                    {
                        for(int i=0;i<10;i++)
                        {
                            pbdata.append(myquery.value(i+2));
                        }
                        //emit sendPbdata(pbdata,true);
                        isexist=true;
                        close();
                        return true;


                    }
                    else
                    {
                        //emit sendPbdata(pbdata,false);
                        close();
                        return true;


                    }
                }




            }

        }
        else
        {
            //emit sendPbdata(pbdata,false);
            close();

            return false;
        }


}




