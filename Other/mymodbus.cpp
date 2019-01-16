#include "mymodbus.h"
#include<QMutex>

Mymodbus::Mymodbus(QObject *parent) : QObject(parent),
    m_MBSetting(ModBSettings()),
    Isrun(false)
{
    connect(this,&Mymodbus::readfine,this,&Mymodbus::ChamberRead);

}

/*void Mymodbus::ChamberRead()
{
    if(m_sendtimer)
    {
        qDebug()<<"Stop timer";
        //m_sendtimer->stop();

    }

    if (m_modbus->state() != QModbusDevice::ConnectedState)
    {
        qDebug()<<"connect fail quit ";
        return;
    }
        else {
            qDebug()<<"first ser size"<<m_Serqueue.size();

            DataQueue newdt=m_Serqueue.head();
            if(m_Serqueue.size()>1)
             {
                 m_Serqueue.dequeue();
            }
            else if(m_Serqueue.size()==1)
            {
                DataQueue olddt=m_Serqueue.head();
                if(olddt.state!=0)
                {
                    DataQueue orig={0,0};
                    m_Serqueue.enqueue(orig);
                }
            }
            qDebug()<<"second ser size"<<m_Serqueue.size();//过程判断：判定队列大于1，处理队列头，并弹出
            //队列等于1，判断状态是否为0,为0跳过，不为0添加0序列，



            QModbusDataUnit Data(QModbusDataUnit::HoldingRegisters,0,1);//读取相关值
            QModbusDataUnit Svalue(QModbusDataUnit::HoldingRegisters,43,2);//写入SV相关值
            QModbusDataUnit Rsvalue(QModbusDataUnit::HoldingRegisters,46,2);//运行停止状态
            short value1=0;//停止代码
            short value2=1;//运行代码




                    int state=newdt.state;
                    //qDebug()<<"end ser size"<<Serqueue.size();
                        switch (state) {
                    case 0://读取相关数据
                            //qDebug()<<"Cycread"<<testno++<<endl;
                            if (auto *reply = m_modbus->sendReadRequest(Data, 1)) {

                            if (!reply->isFinished())
                            {

                                connect(reply, &QModbusReply::finished, this, &Mymodbus::readReady);
                            }
                            else
                                delete reply; // broadcast replies return immediately
                        } else {
                          sendError(tr("Read error: ") + m_modbus->errorString());

                        }
                            break;

                    case 1://写入SV
                            {
                            //qDebug()<<"SV"<<testno++<<endl;

                           short value=static_cast<short>(newdt.SV*10);
                            //kil=packFloat(newdt.SV);//写入SV数据转换
                            Svalue.setValue(0,static_cast<quint16>(value));
                            //Svalue.setValue(1,static_cast<quint16>(0));
                            //Svalue.setValue(4,static_cast<quint16>(value2));
                            if (auto *reply = m_modbus->sendWriteRequest(Svalue, 1)) {

                                if (!reply->isFinished()) {
                                    connect(reply, &QModbusReply::finished,this,&Mymodbus::writefinishReady);

                                } else {
                                    // broadcast replies return immediately
                                    reply->deleteLater();
                                }
                            } else {
                                sendError(tr("Write SV error: ") + m_modbus->errorString());

                            }

                        }

                        break;
                case 2://停止
                            //qDebug()<<"Time"<<testno++<<endl;

                             //kil=packFloat(newdt.SV);//写入SV数据转换
                             //Svalue.setValue(3,static_cast<quint16>(value1));
                            qDebug()<<"stop";

                             Rsvalue.setValue(1,static_cast<quint16>(value1));

                             //Rsvalue.setValue(1,static_cast<quint16>(1));

                                 if (auto *reply = m_modbus->sendWriteRequest(Rsvalue, 1)) {
                                     //bMV=true;
                                     if (!reply->isFinished()) {
                                         connect(reply, &QModbusReply::finished,this,&Mymodbus::writefinishReady);

                                     } else {
                                         // broadcast replies return immediately
                                         reply->deleteLater();
                                     }
                                 } else {
                                     sendError(tr("Stop error: ") + m_modbus->errorString());

                                 }

                                 break;
                        case 3://运行
                                    //qDebug()<<"Time"<<testno++<<endl;

                                     //kil=packFloat(newdt.SV);//写入SV数据转换
                            //Rsvalue.setValue(3,static_cast<quint16>(value2));
                            qDebug()<<"run";
                            qDebug()<<value2;
                            qDebug()<<Rsvalue.valueCount();
                                     Rsvalue.setValue(1,static_cast<quint16>(value2));

                                         if (auto *reply = m_modbus->sendWriteRequest(Rsvalue, 1)) {
                                             //bMV=true;
                                             if (!reply->isFinished()) {
                                                 connect(reply, &QModbusReply::finished,this,&Mymodbus::writefinishReady);

                                             } else {
                                                 // broadcast replies return immediately
                                                 reply->deleteLater();
                                             }
                                         } else {
                                             sendError(tr("Run error: ") + m_modbus->errorString());

                                         }

                                         break;

                }
                }

}*/

void Mymodbus::ChamberRead(const int &i, const float &value)
{
    if (m_modbus->state() != QModbusDevice::ConnectedState)
    {
        qDebug()<<"connect fail quit ";
        return;
    }
        else
    {
        QModbusDataUnit Data(QModbusDataUnit::HoldingRegisters,0,1);//读取相关值
        QModbusDataUnit Svalue(QModbusDataUnit::HoldingRegisters,46,2);//启动写入SV相关值，并启动
        QModbusDataUnit Svalue1(QModbusDataUnit::HoldingRegisters,43,2);//中间写入SV相关值

    switch(i)
    {
    case 0:
    {
        if (auto *reply = m_modbus->sendReadRequest(Data, 1)) {

        if (!reply->isFinished())
        {

            connect(reply, &QModbusReply::finished, this, &Mymodbus::readReady);
        }
        else
            delete reply; // broadcast replies return immediately
    } else {
     emit sendError(tr("Read error: ") + m_modbus->errorString());

    }
    }
        break;
    case 1:
    {
        qDebug()<<"start run!";
        //short Pvalue=static_cast<short>(value*10);
         //kil=packFloat(newdt.SV);//写入SV数据转换
         Svalue.setValue(0,static_cast<quint16>(0));
         Svalue.setValue(1,static_cast<quint16>(1));

         //Svalue.setValue(1,static_cast<quint16>(0));
         //Svalue.setValue(4,static_cast<quint16>(value2));
         if (auto *reply = m_modbus->sendWriteRequest(Svalue, 1)) {

             if (!reply->isFinished()) {
                 connect(reply, &QModbusReply::finished,this,&Mymodbus::writefinishReady);

             } else {
                 // broadcast replies return immediately
                 reply->deleteLater();
             }
         } else {
            emit sendError(tr("Write SV error: ") + m_modbus->errorString());

         }
    }
        break;
    case 2:
    {
        short Pvalue=static_cast<short>(value*10);
         //kil=packFloat(newdt.SV);//写入SV数据转换
         Svalue1.setValue(0,static_cast<quint16>(Pvalue));
         Svalue1.setValue(1,static_cast<quint16>(0));
         qDebug()<<"SV is "<<Pvalue;

         if (auto *reply = m_modbus->sendWriteRequest(Svalue1, 1)) {

             if (!reply->isFinished()) {
                 connect(reply, &QModbusReply::finished,this,&Mymodbus::writefinishReady);

             } else {
                 // broadcast replies return immediately
                 reply->deleteLater();
             }
         } else {
             sendError(tr("Write SV error: ") + m_modbus->errorString());

         }
    }
        break;
    default:
        break;



    }
    }

}

void Mymodbus::startNext()
{

    if(Isrun)
    {

       emit readfine(0,0);


    }


}



bool Mymodbus::MBconnect()
{
    m_modbus = new QModbusRtuSerialMaster(this);




            if (m_modbus->state() != QModbusDevice::ConnectedState) {
                m_modbus->setConnectionParameter(QModbusDevice::SerialPortNameParameter,
                    m_MBSetting.port);
                m_modbus->setConnectionParameter(QModbusDevice::SerialParityParameter,
                    m_MBSetting.parity);
                m_modbus->setConnectionParameter(QModbusDevice::SerialBaudRateParameter,
                    m_MBSetting.baud);
                m_modbus->setConnectionParameter(QModbusDevice::SerialDataBitsParameter,
                    m_MBSetting.dataBits);
                m_modbus->setConnectionParameter(QModbusDevice::SerialStopBitsParameter,
                   m_MBSetting.stopBits);

            }
            m_modbus->setTimeout(m_MBSetting.responseTime);
            m_modbus->setNumberOfRetries(3);
                if (!m_modbus->connectDevice()) {

                    qDebug()<<"error connect";
                    emit connectstate(false);
                    return false;

                } else {
                    qDebug()<<"connecting";
                    //DataQueue news={0,0};
                    //m_Serqueue.enqueue(news);//添加初始队列
                    emit connectstate(true);
                    return true;

                }



}

void Mymodbus::readReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
        if (!reply)
            return;


        qreal Pvalue=0;
        if (reply->error() == QModbusDevice::NoError) {
            const QModbusDataUnit unit = reply->result();

            short tempx=static_cast<short>(unit.value(0));

           // qDebug()<<tempx;
            Pvalue=tempx/10.0;
            qDebug()<<"Pvalue is "<<Pvalue;
            emit sendPvalue(Pvalue);
            qDebug()<<"emit signal";


            //m_sendtimer->setInterval(2000);
            //m_sendtimer->start(3000);
            //emit readfine();

                }

        else if (reply->error() == QModbusDevice::ProtocolError) {
            qDebug()<<"error1";
           emit sendError(tr("读取响应错误: %1 (Mobus 异常代码: 0x%2)").
                                        arg(reply->errorString()).
                                        arg(reply->rawResult().exceptionCode(), -1, 16));
            mutex.lock();
            Isrun=false;
            mutex.unlock();
            //Restart();
            return;
        } else {
            qDebug()<<"error2";
            emit sendError(tr("读取响应错误: %1 (代码: 0x%2)").
                                        arg(reply->errorString()).
                                        arg(reply->error(), -1, 16));
            mutex.lock();
            Isrun=false;
            mutex.unlock();
            qDebug()<<"error2";

            return;
        }


        reply->deleteLater();


}

void Mymodbus::writefinishReady()
{
    auto reply = qobject_cast<QModbusReply *>(sender());
            if (!reply)
                return;
            if (reply->error() == QModbusDevice::ProtocolError) {
                qDebug()<<"Werror1";
               emit sendError(tr("Write response error: %1 (Mobus exception: 0x%2)")
                    .arg(reply->errorString()).arg(reply->rawResult().exceptionCode(), -1, 16));
                mutex.lock();
                Isrun=false;
                mutex.unlock();

                return;
            } else if (reply->error() != QModbusDevice::NoError) {
                qDebug()<<"Werror2";
                emit sendError(tr("Write response error: %1 (code: 0x%2)").
                    arg(reply->errorString()).arg(reply->error(), -1, 16));
                mutex.lock();
                Isrun=false;
                mutex.unlock();

                return;

            }


            reply->deleteLater();
            startNext();


}

void Mymodbus::setRun()
{
    qDebug()<<"run";
    mutex.lock();
    Isrun=true;
    mutex.unlock();

}

void Mymodbus::stopRun()
{
    qDebug()<<"stop";
    mutex.lock();
    Isrun=false;
    mutex.unlock();

}



