#include "switch.h"
#include<QMessageBox>
#include<QDebug>
#include<stdio.h>
#include<QFile>
#include<QTextStream>
#include<QDir>

Switch::Switch(QObject *parent) : QObject(parent),
    Pxie2525(VI_NULL),
    Pxie2526(VI_NULL),
    Pxie2575(VI_NULL),
    error(VI_SUCCESS),
    counttimes(0),
    PlateNo(0),
    BaseNo(0),
    errorcount(0),
    currentTesttype(TestType::TCR),
    IstempTest(false)

{


    strcpy_s(PXI1Slot4.resourceName,"PXI1Slot4");

    //PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_95X1_MUX;

    strcpy_s(PXI1Slot5.resourceName,"PXI1Slot5");
    strcpy_s(PXI1Slot6.resourceName,"PXI1Slot6");



    InitialChannel();



}

void Switch::niSwitch_ErrorHandler(ViSession session, ViStatus error)
{
    ViChar *buffer = VI_NULL;
        ViChar *pos = VI_NULL;
        ViStatus errorNumber = VI_SUCCESS;
        ViInt32 bufferSize = 0;
        bufferSize = niSwitch_GetError(session, &errorNumber, 0, buffer);
        qDebug()<<"buffersieze ="<<bufferSize;
        //  the additional 256 bytes are for the static text before the description.
        pos = buffer = (ViChar*)malloc(bufferSize + 256);
        pos += sprintf(pos, "NI-SWITCH Error\n\nError:%d (0x%x)\n", error, error);
        niSwitch_GetError(session, &errorNumber, bufferSize, pos);
        //QMessageBox::information(NULL,"Caution",QString(QLatin1String(buffer)),QMessageBox::Yes);
        qDebug()<<QLatin1String(buffer);
        qDebug()<<"sendError";
        switch (currentTesttype) {
        case TestType::TCR:
            sendError(QString(QLatin1String(buffer)),0);
            qDebug()<<"send TCRerror";
            break;
        case TestType::Probe:
            sendError(QString(QLatin1String(buffer)),1);
            qDebug()<<"send proberror";
            break;
        case TestType::Calibrate:
            sendError(QString(QLatin1String(buffer)),2);
            qDebug()<<"send Calierror";
            break;
        default:
            break;
        }
        free(buffer);


}

bool Switch::ResultJud(ViStatus status, ViSession vi)
{
    qDebug()<<"errorcount"<<errorcount;
    errorcount++;


    if(status==VI_SUCCESS)
    {
        qDebug()<<"true status"<<status;
        qDebug()<<"Result good!";
        return true;

    }
    else
    {
        qDebug()<<status;
        //ErrorHandler(vi,status);
        qDebug()<<"false status"<<status;
        return false;

    }

}



void Switch::InitialChannel()
{
    //T3初始化
    int orig2575=77,orig2525=0;
    for(int i=0;i<6;i++)
    {
        //int orig2575=77,orig2525=0;
        QString firstchannel="ch"+QString::number(orig2575+i);
        QStringList temp;
        temp.append(firstchannel);
        for(int j=0;j<10;j++)
        {
            QString channels="ch"+QString::number(orig2525+j);
            temp.append(channels);
        }
        T3channel.append(temp);
        orig2525+=10;

    }
    //T6初始化
    orig2575=59;
    orig2525=0;
    for(int i=0;i<6;i++)
    {
        //int orig2575=77,orig2525=0;
        QString firstchannel="ch"+QString::number(orig2575+i);
        QStringList temp;
        temp.append(firstchannel);
        for(int j=0;j<10;j++)
        {
            QString channels="ch"+QString::number(orig2525+j);
            temp.append(channels);
        }
        T6channel.append(temp);
        orig2525+=10;

    }
    //T4初始化
    T4channels<<"ch84"<<"ch85"<<"ch86"<<"ch87"<<"ch88"<<"ch89"<<"ch90"<<"ch91"<<"ch92"<<"ch93";
    T4channels<<"ch60"<<"ch61"<<"ch62"<<"ch63"<<"ch64"<<"ch65"<<"ch66"<<"ch67"<<"ch68"<<"ch69";


    //T5初始化
    int orl1=72,orl2=167;
    for(int i=0;i<10;i++)
    {
        if(i%2==0)
        {
            T5channels.append("ch"+QString::number(orl1));
            orl1++;
        }
        else
        {
            T5channels.append("ch"+QString::number(orl2));
            orl2++;
        }
    }



    //T1初始化
    QString Oripath=":/Channel/";
    for(int i=0;i<6;i++)
    {
        QString Curfile=Oripath+"/t1a"+QString::number(i+1);//+".txt";
        //qDebug()<<Curfile;
        QFile Origfile(Curfile);
        if ( !Origfile.open(QFile::ReadOnly | QFile::Text) )
             {
                 QMessageBox::information(NULL,"Caution","NO Channel files!",QMessageBox::Yes);;
                 break;
                 return;
                 qDebug()<<"fail sw1";
             }
             else
             {
                 QTextStream in(&Origfile);
                 QStringList temp;
                 while(!in.atEnd())
                 {
                   temp.push_back(in.readLine());
                 }
                 for(int j=0;j<10;j++)
                 {
                     QString originfo=temp.at(j);
                     originfo.remove("->com;");
                     T1channel[i][j]=originfo;
                     //qDebug()<<T1channel[i][j];
                 }
        }
    }
    for(int i=0;i<6;i++)
    {
        QString Curfile=Oripath+"/t2a"+QString::number(i+1);//+".txt";
        //qDebug()<<Curfile;
        QFile Origfile(Curfile);
        if ( !Origfile.open(QFile::ReadOnly | QFile::Text) )
             {
                 //SendErrorMessage("Switch Channel2 file open fail!");
                 QMessageBox::information(NULL,"Caution","NO Channel files!",QMessageBox::Yes);
                 break;
                 return;
                 qDebug()<<"fail sw2";
             }
             else
             {
                 QTextStream in(&Origfile);
                 QStringList temp;
                 while(!in.atEnd())
                 {
                   temp.push_back(in.readLine());
                 }
                 for(int j=0;j<10;j++)
                 {
                     QString originfo=temp.at(j);
                     originfo.remove("->com;");
                     T2channel[i][j]=originfo;
                     //qDebug()<<T2channel[i][j];
                 }
        }
    }
}

void Switch::InitialTCR()
{
    m_tcrs.currentbase=TCRinfo.at(0);
    m_tcrs.currentNo=m_tcrs.currentbase.sno;
    serialNo=0;
    m_tcrs.cbasechannle=getchannelNo(m_tcrs.currentbase.rstate);
    IstempTest=false;
}

void Switch::InitialTempreport()
{
    short firstno=tempreportdata.at(0);
    m_tcrs.currentbase=TCRinfo.at(firstno);
    m_tcrs.currentNo=m_tcrs.currentbase.sno;
    serialNo=0;
    m_tcrs.cbasechannle=getchannelNo(m_tcrs.currentbase.rstate);
}

bool Switch::Tcrautoincr(short &sn)
{
         sn++;
         if(IstempTest==false)
         {
             if(sn>=TCRinfo.count())
             {
                 qDebug()<<"Test finish!";
                 return false;
             }
             else
             {
                 qDebug()<<"renew one";
                 m_tcrs.currentbase=TCRinfo.at(sn);
                 m_tcrs.cbasechannle=getchannelNo(m_tcrs.currentbase.rstate);
                 m_tcrs.currentNo=m_tcrs.currentbase.sno;
                 //m_tcrs.T3count=m_tcrs.cbasechannle.count();
                 return true;

             }
         }
         else {
             if(sn>=tempreportdata.count())
             {
                 qDebug()<<"temp Test finish!";
                 return false;
             }
             else
             {
                 qDebug()<<"temp test renew one";
                 short currentno=tempreportdata.at(sn);
                 m_tcrs.currentbase=TCRinfo.at(currentno);
                 m_tcrs.cbasechannle=getchannelNo(m_tcrs.currentbase.rstate);
                 m_tcrs.currentNo=m_tcrs.currentbase.sno;
                 //m_tcrs.T3count=m_tcrs.cbasechannle.count();
                 return true;

             }
         }


}

void Switch::Allswitchclose()
{
    if(Pxie2575!=VI_NULL)
    {
        niSwitch_DisconnectAll(Pxie2575);
        niSwitch_close(Pxie2575);
        Pxie2575=VI_NULL;

    }
    if(Pxie2525!=VI_NULL)
    {
        niSwitch_DisconnectAll(Pxie2525);
        niSwitch_close(Pxie2525);
        Pxie2525=VI_NULL;

    }
    if(Pxie2526!=VI_NULL)
    {
        niSwitch_DisconnectAll(Pxie2526);
        niSwitch_close(Pxie2526);
        Pxie2526=VI_NULL;

    }


}






bool Switch::Connect(ViSession vi, ViConstString channel1, ViConstString channel2)
{

    return ResultJud(niSwitch_Connect(vi,channel1,channel2),vi);
}

bool Switch::Disconnect(ViSession vi)
{
    return ResultJud(niSwitch_DisconnectAll(vi),vi);
}

bool Switch::Initial(ViChar* resourceName, ViConstString topology, ViSession *vi)
{
    return ResultJud(niSwitch_InitWithTopology(ViRsrc(resourceName),topology,VI_FALSE,VI_TRUE,vi),*vi);

}



void Switch::ProbeMeasure(int testplate, bool Isfirst)
{
    currentTesttype=TestType::Probe;
    int currentswitchNo=0;
    ViStatus switchError = VI_SUCCESS;
    switch (testplate) {
    case 1:
    {
        qDebug()<<"T4";
#ifdef DEBUG_NI
        PXI1Slot6.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_98X1_MUX;
#else
        PXI1Slot6.topology=NISWITCH_TOPOLOGY_2526_2_WIRE_79X1_MUX;
#endif
        PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_95X1_MUX;
        //QStringList Channels=ChannelInfo.at(BaseNo);
        if(Isfirst)
        {
            counttimes=0;
            ViConstString destchannel2 = "com";//PXie2575
            ViConstString destchannel1 = "com";//PXie2526
            ViConstString origchannel1,origchannel2;
            QByteArray ba1=T4channels.at(10).toLatin1();//2526
            origchannel1=ba1.data();
            QByteArray ba2=T4channels.at(0).toLatin1();//2575
            origchannel2=ba2.data();
            qDebug()<<"2526 channel"<<origchannel1;
            qDebug()<<"2575 channel"<<origchannel2;
            currentswitchNo=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel2,destchannel2));
            currentswitchNo=6;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot6.resourceName,PXI1Slot6.topology,VI_FALSE,VI_TRUE,&Pxie2526));
            niSwitchCheckErr(niSwitch_Connect(Pxie2526,origchannel1,destchannel1));

            //niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            //niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel1,destchannel1));
            //urrentseeionNO=6;
            //niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot6.resourceName,PXI1Slot6.topology,VI_FALSE,VI_TRUE,&Pxie2526));
            //niSwitchCheckErr(niSwitch_Connect(Pxie2526,origchannel2,destchannel2));
            counttimes++;
            //emit StartingMeasure(1,counttimes);
            emit ProbelampIndicator(counttimes,true,1);

        }
        else if(counttimes>=10)
        {
            //qDebug()<<"Final close";
            //currentswitchNo=4;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2575));
            //currentswitchNo=6;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2526));
            Allswitchclose();
            emit ProbelampIndicator(counttimes,false,1);
            //emit ProbeFinish(1);
            return;
        }
        else
        {
            //emit ProbelampIndicator(counttimes,false,1);
            //currentswitchNo=4;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2575));
            //currentswitchNo=6;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2526));
            ViConstString destchannel1 = "com";//2526
            ViConstString destchannel2 = "com";//2575
            ViConstString origchannel1,origchannel2;
            QByteArray ba1=T4channels.at(counttimes+10).toLatin1();//2526
            origchannel1=ba1.data();
            QByteArray ba2=T4channels.at(counttimes+0).toLatin1();//2575
            origchannel2=ba2.data();
            qDebug()<<"2526 channel"<<origchannel1;
            qDebug()<<"2575 channel"<<origchannel2;
            currentswitchNo=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel2,destchannel2));
            currentswitchNo=6;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot6.resourceName,PXI1Slot6.topology,VI_FALSE,VI_TRUE,&Pxie2526));
            niSwitchCheckErr(niSwitch_Connect(Pxie2526,origchannel1,destchannel1));
            counttimes++;
            qDebug()<<"countimes is"<<counttimes;
            emit ProbelampIndicator(counttimes,true,1);

        }
    } //;
        break;
    case 2:
    {
        qDebug()<<"T5";
        PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_1_WIRE_196X1_MUX;
        if(Isfirst)
        {
            qDebug()<<"first one start";
            counttimes=0;
            ViConstString destchannel = "com";
            QString channle=T5channels[counttimes];
            qDebug()<<channle;
            ViConstString origchannel;
            QByteArray ba1=channle.toLatin1();
            origchannel=ba1.data();
            //ViConstString testchannel="ch0";
            currentswitchNo=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel,destchannel));
            counttimes++;
            //emit StartingMeasure(2,counttimes);
            emit ProbelampIndicator(counttimes,true,2);


        }
        else if(counttimes>=10)
        {
            //currentswitchNo=4;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2575));
            Allswitchclose();
            emit ProbelampIndicator(counttimes,false,2);
            //emit ProbeFinish(2);

            return;
        }
        else
        {
            //emit ProbelampIndicator(counttimes,false,2);
            qDebug()<<"no first start";
            ViConstString destchannel = "com";
            QString channle=T5channels[counttimes];
            qDebug()<<channle;
            QByteArray ba1=channle.toLatin1();
            ViConstString origchannel=ba1.data();
            currentswitchNo=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel,destchannel));
            counttimes++;
            //emit StartingMeasure(2,counttimes);
            emit ProbelampIndicator(counttimes,true,2);


        }
    }
        //;
        break;
    default:
        break;
    }
Error:

    if (switchError < VI_SUCCESS)
    {
        switch (currentswitchNo) {
        case 4:
            qDebug()<<"Error1";
            niSwitch_ErrorHandler(Pxie2575, switchError);
            break;
        case 5:
            qDebug()<<"Error2";
            niSwitch_ErrorHandler(Pxie2525, switchError);
            break;
        case 6:
            qDebug()<<"Error3";
            niSwitch_ErrorHandler(Pxie2526, switchError);
            break;
        default:
            break;
        }
        //;
        Allswitchclose();
        qDebug()<<"Bug deal return ";
        return;
    }


}

void Switch::Probtest()
{
    ViStatus switchError = VI_SUCCESS;
    currentTesttype=TestType::Probe;
    qDebug()<<"Test";
#ifdef DEBUG_NI
    PXI1Slot6.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_98X1_MUX;
#else
    PXI1Slot6.topology=NISWITCH_TOPOLOGY_2526_2_WIRE_79X1_MUX;
#endif
    PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_95X1_MUX;
    //QStringList Channels=ChannelInfo.at(BaseNo);

        ViConstString destchannel2 = "com";//PXie2575
        ViConstString destchannel1 = "com";//PXie2526
        ViConstString origchannel1,origchannel2;
        QByteArray ba1=T4channels.at(10).toLatin1();//2526
        origchannel1=ba1.data();
        QByteArray ba2=T4channels.at(0).toLatin1();//2575
        origchannel2=ba2.data();
        qDebug()<<"2526 channel"<<origchannel1;
        qDebug()<<"2575 channel"<<origchannel2;
        //currentswitchNo=4;
        niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575);
        niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel2,destchannel2));
       // currentswitchNo=6;
        niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot6.resourceName,PXI1Slot6.topology,VI_FALSE,VI_TRUE,&Pxie2526));
       niSwitchCheckErr(niSwitch_Connect(Pxie2526,origchannel1,destchannel1));
        Error:
        qDebug()<<"Do nothing";


}










void Switch::Close(ViSession vi)
{
        niSwitch_close(vi);

}

void Switch::SetMeasureInfo(int plate, int base)
{
    PlateNo=plate;
    BaseNo=base;

}

QVector<short> Switch::getchannelNo(flag_t rinfo)
{
    QVector<short> temp;
        for(int i=0;i<10;i++)
        {
            if(cpare(rinfo,i))
            {
                temp.append(i);

            }
            else
            {
                continue;
            }
        }
        return temp;

}

void Switch::ReadyMeasure(const QVector<short> &tempDatainfo)
{
    tempreportdata.clear();
    tempreportdata=tempDatainfo;
    IstempTest=true;
    TCRMeasureplus(true);

}



void Switch::TCRMeasureplus(bool istempfirst)
{
    /* TCR测量流程：
     * 最初由TCRmeasure dialog 触发，m_tcrs为当前测试片信息，如果为空，则从TCRinfo里面提取，并作递增。
     * 测量过程不断清空m_tcrs中的当前通道cbaschanle,并执行接通，并触发信号反馈给TCRMeasure dialog 作为中转，由其触发DMM,DMM测量结束再返回本槽函数
     * 如果通道全部完成，触发单块基片完成信号。
     * 如果TCRinfo获取基片信息失败，则判断一个周期完成。触发周期完成信号。
*/

    if(istempfirst)
    {
        InitialTempreport();
    }
    currentTesttype=TestType::TCR;
    qDebug()<<"plus measure";
    short currentseeionNO=0;
    ViStatus switchError = VI_SUCCESS;
    if(m_tcrs.cbasechannle.isEmpty())
    {
        qDebug()<<"empty";
        if(Tcrautoincr(serialNo))
        {
            //一块基片测试完成
            qDebug()<<"serialNO"<<serialNo;
            qDebug()<<"seg finish! serialNo++,send segfinishNO";
            emit TCRlampIndicator(24,0);
            Allswitchclose();
            emit TCRsegFmeasure();

        }
        else
        {
            //1个周期测试完
            qDebug()<<"cycle finish!";
            Allswitchclose();
            InitialTCR();
            emit TCRcyclefinish();


            qDebug()<<"Test finishe send one Prog finish singal";

        }



    }
    else
    {
        qDebug()<<"ceshi";
        if(m_tcrs.currentNo<6)
        {
            //Disconnect(Pxie2575);
            qDebug()<<"T1";
            PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_1_WIRE_196X1_MUX;
            short channelNO;
            //m_tcrs.cbasechannle.removeOne(channelNO);
            channelNO=m_tcrs.cbasechannle.takeFirst();
            qDebug()<<channelNO;
            ViConstString destchannel = "com";
            QString channle=T1channel[m_tcrs.currentNo][channelNO];
            qDebug()<<channle;
            //emit TCRlampIndicator("T2",m_tcrs.currentNo,channelNO);

            ViConstString origchannel;
            QByteArray ba1=channle.toLatin1();
            origchannel=ba1.data();
            currentseeionNO=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel,destchannel));
            //emit TCRstartmeasure(m_tcrs.currentNo,channelNO);
            emit TCRlampIndicator(m_tcrs.currentNo,channelNO);
        }

        else if((m_tcrs.currentNo>=6)&(m_tcrs.currentNo<12))
        {
            //Disconnect(Pxie2575);
            qDebug()<<"T2";
            PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_1_WIRE_196X1_MUX;
            short channelNO;
            //m_tcrs.cbasechannle.removeOne(channelNO);
            channelNO=m_tcrs.cbasechannle.takeFirst();
            qDebug()<<channelNO;
            ViConstString destchannel = "com";
            QString channle=T2channel[m_tcrs.currentNo-6][channelNO];
            qDebug()<<channle;
            //emit TCRlampIndicator("T2",m_tcrs.currentNo,channelNO);
            //emit TCRlampIndicator(m_tcrs.currentNo);
            ViConstString origchannel;
            QByteArray ba1=channle.toLatin1();
            origchannel=ba1.data();
            currentseeionNO=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel,destchannel));
            emit TCRlampIndicator(m_tcrs.currentNo,channelNO);
        }
        else if((m_tcrs.currentNo>=12)&(m_tcrs.currentNo<18))
        {
            qDebug()<<"T3";
#ifdef  DEBUG_NI
    PXI1Slot5.topology=NISWITCH_TOPOLOGY_2527_1_WIRE_64X1_MUX;
#else
    PXI1Slot5.topology=NISWITCH_TOPOLOGY_2525_2_WIRE_64X1_MUX;
#endif
    PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_95X1_MUX;

        ViConstString destchannel1 = "com";
        ViConstString destchannel2 = "com0";
        ViConstString origchannel1,origchannel2;
        QByteArray ba1=T3channel.at(m_tcrs.currentNo-12).at(0).toLatin1();
        origchannel1=ba1.data();
        short channelNo;
        channelNo=m_tcrs.cbasechannle.takeFirst();


        QByteArray ba2=T3channel.at(m_tcrs.currentNo-12).at(channelNo+1).toLatin1();
        //emit TCRlampIndicator("T3",m_tcrs.currentNo-6,channelNo);
        //emit TCRlampIndicator(m_tcrs.currentNo);
        origchannel2=ba2.data();
        currentseeionNO=4;
        niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
        niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel1,destchannel1));
        currentseeionNO=5;
        niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot5.resourceName,PXI1Slot5.topology,VI_FALSE,VI_TRUE,&Pxie2525));
        niSwitchCheckErr(niSwitch_Connect(Pxie2525,origchannel2,destchannel2));
        emit TCRlampIndicator(m_tcrs.currentNo,channelNo);


    }
        else if((m_tcrs.currentNo>=18)&(m_tcrs.currentNo<24))
        {
            qDebug()<<"T6";
#ifdef  DEBUG_NI
    PXI1Slot6.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_95X1_MUX;
#else
    PXI1Slot6.topology=NISWITCH_TOPOLOGY_2526_2_WIRE_79X1_MUX;
#endif
    PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_95X1_MUX;
        ViConstString destchannel1 = "com";
        ViConstString destchannel2 = "com";
        ViConstString origchannel1,origchannel2;
        QByteArray ba1=T6channel.at(m_tcrs.currentNo-18).at(0).toLatin1();
        origchannel1=ba1.data();
        short channelNo;
        channelNo=m_tcrs.cbasechannle.takeFirst();


        QByteArray ba2=T6channel.at(m_tcrs.currentNo-18).at(channelNo+1).toLatin1();
        //emit TCRlampIndicator("T3",m_tcrs.currentNo-6,channelNo);
        //emit TCRlampIndicator(m_tcrs.currentNo);
        origchannel2=ba2.data();
        currentseeionNO=4;
        niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
        niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel1,destchannel1));
        currentseeionNO=6;
        niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot6.resourceName,PXI1Slot6.topology,VI_FALSE,VI_TRUE,&Pxie2526));
        niSwitchCheckErr(niSwitch_Connect(Pxie2526,origchannel2,destchannel2));
        //emit TCRstartmeasure(m_tcrs.currentNo,channelNo);
        emit TCRlampIndicator(m_tcrs.currentNo,channelNo);



        }
    }
Error:

    if (switchError < VI_SUCCESS)
    {
        switch (currentseeionNO) {
        case 4:
            niSwitch_ErrorHandler(Pxie2575, switchError);
            break;
        case 5:
            niSwitch_ErrorHandler(Pxie2525, switchError);
            break;
        case 6:
            niSwitch_ErrorHandler(Pxie2526, switchError);
            break;
        default:
            break;
        }
        //;
        Allswitchclose();
        qDebug()<<"Bug deal return ";
        return;
    }
    //


}

void Switch::reciveInfo(QVector<basestr> info)
{
    TCRinfo=info;
    foreach (basestr temp, TCRinfo) {
        qDebug()<<temp.sno;

    }
    InitialTCR();
}

void Switch::CalibrMeasure(int testplate, bool Isfirst)
{
    currentTesttype=TestType::Calibrate;
    int currentswitchNo=0;
    ViStatus switchError = VI_SUCCESS;
    switch (testplate) {
    case 1:
    {
        qDebug()<<"T4";
#ifdef DEBUG_NI
        PXI1Slot6.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_98X1_MUX;
#else
        PXI1Slot6.topology=NISWITCH_TOPOLOGY_2526_2_WIRE_79X1_MUX;
#endif
        PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_2_WIRE_95X1_MUX;
        //QStringList Channels=ChannelInfo.at(BaseNo);
        if(Isfirst)
        {
            counttimes=0;
            ViConstString destchannel2 = "com";//PXie2575
            ViConstString destchannel1 = "com";//PXie2526
            ViConstString origchannel1,origchannel2;
            QByteArray ba1=T4channels.at(10).toLatin1();//2526
            origchannel1=ba1.data();
            QByteArray ba2=T4channels.at(0).toLatin1();//2575
            origchannel2=ba2.data();
            qDebug()<<"2526 channel"<<origchannel1;
            qDebug()<<"2575 channel"<<origchannel2;
            currentswitchNo=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel2,destchannel2));
            currentswitchNo=6;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot6.resourceName,PXI1Slot6.topology,VI_FALSE,VI_TRUE,&Pxie2526));
            niSwitchCheckErr(niSwitch_Connect(Pxie2526,origchannel1,destchannel1));
            counttimes++;
            emit StartingMeasure(1,counttimes);
            //emit ProbelampIndicator(counttimes,true);

        }
        else if(counttimes>=10)
        {
            //qDebug()<<"Final close";
            //currentswitchNo=4;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2575));
            //currentswitchNo=6;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2526));
            Allswitchclose();
            //emit ProbelampIndicator(counttimes,false);
            emit CalibraFinish(1);
            return;
        }
        else
        {
            //emit ProbelampIndicator(counttimes,false);
            //currentswitchNo=4;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2575));
            //currentswitchNo=6;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2526));
            ViConstString destchannel1 = "com";//2526
            ViConstString destchannel2 = "com";//2575
            ViConstString origchannel1,origchannel2;
            QByteArray ba1=T4channels.at(counttimes+10).toLatin1();//2526
            origchannel1=ba1.data();
            QByteArray ba2=T4channels.at(counttimes+0).toLatin1();//2575
            origchannel2=ba2.data();
            qDebug()<<"2526 channel"<<origchannel1;
            qDebug()<<"2575 channel"<<origchannel2;
            currentswitchNo=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel2,destchannel2));
            currentswitchNo=6;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot6.resourceName,PXI1Slot6.topology,VI_FALSE,VI_TRUE,&Pxie2526));
            niSwitchCheckErr(niSwitch_Connect(Pxie2526,origchannel1,destchannel1));
            counttimes++;
            emit StartingMeasure(1,counttimes);
            //emit ProbelampIndicator(counttimes,true);

        }
    } //;
        break;
    case 2:
    {
        qDebug()<<"T5";
        PXI1Slot4.topology=NISWITCH_TOPOLOGY_2575_1_WIRE_196X1_MUX;
        if(Isfirst)
        {
            qDebug()<<"first one start";
            counttimes=0;
            ViConstString destchannel = "com";
            QString channle=T5channels[counttimes];
            qDebug()<<channle;
            ViConstString origchannel;
            QByteArray ba1=channle.toLatin1();
            origchannel=ba1.data();
            //ViConstString testchannel="ch0";
            currentswitchNo=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel,destchannel));
            counttimes++;
            emit StartingMeasure(2,counttimes);
            //emit ProbelampIndicator(counttimes,true);


        }
        else if(counttimes>=10)
        {
            //currentswitchNo=4;
            //niSwitchCheckErr(niSwitch_DisconnectAll(Pxie2575));
            Allswitchclose();
            //emit ProbelampIndicator(counttimes,false);
            emit CalibraFinish(2);

            return;
        }
        else
        {
            //emit ProbelampIndicator(counttimes,false);
            qDebug()<<"no first start";
            ViConstString destchannel = "com";
            QString channle=T5channels[counttimes];
            qDebug()<<channle;
            QByteArray ba1=channle.toLatin1();
            ViConstString origchannel=ba1.data();
            currentswitchNo=4;
            niSwitchCheckErr(niSwitch_InitWithTopology(PXI1Slot4.resourceName,PXI1Slot4.topology,VI_FALSE,VI_TRUE,&Pxie2575));
            niSwitchCheckErr(niSwitch_Connect(Pxie2575,origchannel,destchannel));
            counttimes++;
            emit StartingMeasure(2,counttimes);
            //emit ProbelampIndicator(counttimes,true);


        }
    }
        //;
        break;
    default:
        break;
    }
Error:

    if (switchError < VI_SUCCESS)
    {
        switch (currentswitchNo) {
        case 4:
            niSwitch_ErrorHandler(Pxie2575, switchError);
            break;
        case 5:
            niSwitch_ErrorHandler(Pxie2525, switchError);
            break;
        case 6:
            niSwitch_ErrorHandler(Pxie2526, switchError);
            break;
        default:
            break;
        }
        //;
        qDebug()<<"Bug deal return ";
        Allswitchclose();
        return;
    }


}





