#include "tcrdataop.h"
#include<QMessageBox>


TCRdataop::TCRdataop(QObject *parent):Dataop(parent),
    hcalvalue(125.0),
    ncalvalue(25.0),
    lcalvalue(-55.0)
{

}

TCRdataop::~TCRdataop()
{

}

bool TCRdataop::insertRvalue(int T_id, int Rno, double value, int serialno)
{
    if (!connect())
        {
            qDebug() << "Error: Failed to connect database." << m_db.lastError();
            return false;
        }
        else
        {
            QString insertvalue;
            QSqlQuery myquery;

        switch (serialno+1){
        case 1:
            insertvalue="UPDATE TCRDATA SET Rfirst="+QString::number(value,'g',10)+" where T_id="+QString::number(T_id)+" and resistanceno="+QString::number(Rno+1);
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

            break;
        case 2:
            insertvalue="UPDATE TCRDATA SET Rfirst2="+QString::number(value,'g',10)+" where T_id="+QString::number(T_id)+" and resistanceno="+QString::number(Rno+1);
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

            break;
        case 3:
            insertvalue="UPDATE TCRDATA SET Rsecond="+QString::number(value,'g',10)+" where T_id="+QString::number(T_id)+" and resistanceno="+QString::number(Rno+1);
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

            break;
        case 4:
            insertvalue="UPDATE TCRDATA SET Rsecond2="+QString::number(value,'g',10)+" where T_id="+QString::number(T_id)+" and resistanceno="+QString::number(Rno+1);
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

            break;
        case 5:
            insertvalue="UPDATE TCRDATA SET Rthird="+QString::number(value,'g',10)+" where T_id="+QString::number(T_id)+" and resistanceno="+QString::number(Rno+1);
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

            break;
        case 6:
            insertvalue="UPDATE TCRDATA SET Rthird2="+QString::number(value,'g',10)+" where T_id="+QString::number(T_id)+" and resistanceno="+QString::number(Rno+1);
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

            break;
            }

        }

}

/*void TCRdataop::setTCRcalvalue(qreal htemp, qreal ltemp, qreal ntemp)
{
    hcalvalue=htemp;
    lcalvalue=ltemp;
    ncalvalue=ntemp;

}*/

bool TCRdataop::UpdateTCR(int T_id)
{
    if(!connect())
        {
            qDebug()<<"fail to connect"<<m_db.lastError();
            return false;
        }
        else{
            for(int i=0;i<10;i++)
            {
        QSqlQuery sql_query;
        QList<QVariant> temp;
        QString select_sql = "select * from TCRData where T_id ="+QString::number(T_id)+" and resistanceno ="+QString::number(i+1);
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
                for(int j=0;j<12;j++)
                {
                    temp.append(sql_query.value(j));

                }
            }
            qDebug()<<temp.size();
            double htcr1=0;
            double htcr2=0;
            double ctcr1=0;
            double ctcr2=0;
            if(temp.at(2).toString()!="")
            {
                htcr1=calculateHTCR(temp.at(6).toDouble(),temp.at(4).toDouble());
                htcr2=calculateHTCR(temp.at(7).toDouble(),temp.at(5).toDouble());
                ctcr1=calculateCTCR(temp.at(2).toDouble(),temp.at(4).toDouble());
                ctcr2=calculateCTCR(temp.at(3).toDouble(),temp.at(5).toDouble());

            }

            QString insert_sql="update TCRData set H1TCR ="+QString::number(htcr1)+", H2TCR="+QString::number(htcr2)+",C1TCR="+QString::number(ctcr1)+",C2TCR="+QString::number(ctcr2)+" where T_id="+QString::number(T_id)+" and resistanceno ="+QString::number(i+1);
            qDebug()<<insert_sql;
            if(!sql_query.exec(insert_sql))
            {
                close();
                qDebug()<<sql_query.lastError();
                return false;
            }
            else
            {
                qDebug()<<"insert resistanceno "<<i<<"successful!";
                continue;
            }


        }
        }
            close();
            return true;

            }

}

bool TCRdataop::addcurrentDateTime(int T_id)
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
            QString adt_sql="UPDATE TCRName SET recordtime='"+Cdt+"' where T_id="+QString::number(T_id);
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

bool TCRdataop::deleteData(int T_id)
{
    QString foreingON="PRAGMA foreign_keys = ON";
            QString del="delete from TCRName where T_id="+QString::number(T_id);
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

bool TCRdataop::createData(QString name, int &T_id, const qreal &temp1, const qreal &temp2, const qreal &temp3)
{
    if (!connect())
        {
            qDebug() << "Error: Failed to connect database." << m_db.lastError();
            return false;
        }
        else
        {
            QSqlQuery sql_query;
            QString adb_sql="insert into TCRName (basename,Temp1,Temp2,Temp3) VALUES('"+name+"',"+QString::number(temp1)+","+QString::number(temp2)+","+QString::number(temp3)+")";
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
            QString find_maxid="select MAX(T_id) from TCRName";

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
                     QString adr_sql="insert into TCRData (T_id,basename,resistanceno) VALUES ("+QString::number(tid)+",'"+name+"',";
                     for(int i=0;i<10;i++)
                     {
                         QString adrf_sql=adr_sql+QString::number(i+1)+")";
                         sql_query.prepare(adrf_sql);
                         if(!sql_query.exec())
                         {
                             qDebug()<<sql_query.lastError();
                             close();
                             return false;

                         }
                         else
                         {
                             continue;
                         }


                     }
                     close();
                     return true;
                }

        }

        }


}

void TCRdataop::disconnectDB()
{
    close();

}

bool TCRdataop::insertTempreportValue(QVector<QStringList> data)
{
    if (!connect())
        {
            qDebug() << "Error: Failed to connect database." << m_db.lastError();
            //QMessageBox::warning(NULL,"warning! connect fail!",m_db.lastError().text(),QMessageBox::Yes);
            return false;
        }
        else
        {
            QSqlQuery sql_query;
            QString acdb_sql="insert into Tempreport VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";

            foreach (QStringList singledata, data) {
               sql_query.clear();
               sql_query.prepare(acdb_sql);
               sql_query.addBindValue(singledata.at(0));
               sql_query.addBindValue(singledata.at(1));
               sql_query.addBindValue(singledata.at(2).toDouble());
               sql_query.addBindValue(singledata.at(3).toDouble());
               sql_query.addBindValue(singledata.at(4).toDouble());
               sql_query.addBindValue(singledata.at(5).toDouble());
               sql_query.addBindValue(singledata.at(6).toDouble());
               sql_query.addBindValue(singledata.at(7).toDouble());
               sql_query.addBindValue(singledata.at(8).toDouble());
               sql_query.addBindValue(singledata.at(9).toDouble());
               sql_query.addBindValue(singledata.at(10).toDouble());
               sql_query.addBindValue(singledata.at(11).toDouble());
               if(!sql_query.exec())
               {
                   qDebug()<<sql_query.lastError();
                   //QMessageBox::warning(NULL,"warning! sql fail!",sql_query.lastError().text(),QMessageBox::Yes);
                   close();
                   return false;

               }
               else
               {
                   continue;
               }

            }
            close();
            return true;



    }


}

bool TCRdataop::deleteTempreport()
{
    if (!connect())
        {
            qDebug() << "Error: Failed to connect database." << m_db.lastError();
            return false;
        }
        else
        {
            QSqlQuery sql_query;
            QString det_sql="delete from Tempreport";
            sql_query.prepare(det_sql);
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

void TCRdataop::setcalculateValue(const qreal &ltemp, const qreal &mtemp, const qreal &htemp)
{
    hcalvalue=htemp;
    ncalvalue=mtemp;
    lcalvalue=ltemp;


}

bool TCRdataop::debugCalTCR(QVector<int> &totaldata)
{
    if (!connect())
        {
            qDebug() << "Error: Failed to connect database." << m_db.lastError();
            //QMessageBox::warning(NULL,"warning! connect fail!",m_db.lastError().text(),QMessageBox::Yes);
            return false;
        }
        else
        {
            QSqlQuery sql_query;
            QString acdb_sql="select T_id from TCRName";
            sql_query.prepare(acdb_sql);
            if(!sql_query.exec())
            {
                qDebug()<<sql_query.lastError();
                close();
                return false;

            }
            else
            {
                QVector<int> totaldatas;
                totaldatas.clear();
                while(sql_query.next())
                {
                    int tid=sql_query.value(0).toInt();
                    totaldatas.append(tid);
                    //this->UpdateTCR(tid);

                }
                totaldata=totaldatas;

                close();
                return true;
            }
    }

}

qreal TCRdataop::calculateHTCR(qreal Hvalue, qreal Nvalue)
{
    if(Hvalue<0)
    {
        return 0;
    }
    else if(Nvalue<0)
    {
        return 0;
    }
    else
    {
        return ((Hvalue-Nvalue)/((hcalvalue-ncalvalue)*Nvalue))*1000000;
    }


}

qreal TCRdataop::calculateCTCR(qreal Lvalue, qreal Nvalue)
{
    if(Lvalue<0)
    {
        return 0;
    }
    else if(Nvalue<0)
    {
        return 0;
    }
    else
    {
        return ((Lvalue-Nvalue)/((lcalvalue-ncalvalue)*Nvalue))*1000000;
    }


}




