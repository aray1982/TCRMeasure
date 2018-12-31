#include "dmm.h"
#include<QDebug>

Dmm::Dmm(QObject *parent) : QObject(parent),
    resourceName("PXI1Slot2"),
    vi(VI_NULL),
    idQuery(VI_TRUE),
    reset(VI_TRUE),
    error(VI_SUCCESS),
    measurementType(NIDMM_VAL_2_WIRE_RES),
    powerlineFreq(NIDMM_VAL_60_HERTZ),
    range(-1.0),
    resolution(7.5),
    absoluteResolution(0.000),
    reading(0.000),
    delaytime(0),
    TCRT2range(-1),
    TCRT3range(100),
    BaseNo(0),
    PlateNo(0),
    PCchooseNo(0),
    forbidError(false),
    isoffset(false)
{

    Initial();




}




void Dmm::Errorhandler(ViSession vi, ViStatus error)
{
       ViChar   *errorText;
       ViStatus errorCode = VI_SUCCESS;
       ViInt32  bufferSize = 0;
       QString Errorinfo;

       if (error == VI_SUCCESS)
       {
          // No error, nothing to do
          return;
       }

       // First find the length of the error description.  Pass VI_NULL for the
       // buffer to retrieve the length of the error message.
       bufferSize = niDMM_GetError(vi, &errorCode, 0, VI_NULL);
       if(bufferSize > 0)
       {
          // Return code >0 from first call to GetError represents the size of
          // the description.  Call it again.
          // Ignore incoming IVI error code and return description from the driver
          // (trust that the IVI error code was properly stored in the session
          // by the driver)
          errorText = (ViChar *)malloc(bufferSize);
          niDMM_GetError(vi, &errorCode, bufferSize, errorText);
       }
       else
       {
          // Return code <= 0 from GetError indicates a problem.  This is expected
          // when the session is invalid (IVI spec requires GetError to fail).
          // Use GetErrorMessage instead.  It doesn't require a session.

          // Call niDMM_GetErrorMessage, pass VI_NULL for the buffer in order to retrieve
          // the length of the error message.
          errorCode = bufferSize;
          bufferSize = niDMM_GetErrorMessage(VI_NULL, errorCode, 0, VI_NULL);
          errorText = (ViChar *)malloc(bufferSize);

          // Get the error description
          niDMM_GetErrorMessage(VI_NULL, errorCode, bufferSize, errorText);
       }

       if(errorCode < VI_SUCCESS)
       {
          // Print Error
           qDebug()<<errorCode;
           Errorinfo=QString("Failure:error=%1;\nerror msg: %2.").arg(QString::number(error)).arg(QString(QLatin1String(errorText)));

           qDebug()<<QString("Failure:error=%1;\nerror msg: %2.").arg(QString::number(error)).arg(QString(QLatin1String(errorText)));
           //QMessageBox::warning(NULL,"Warning",QString("Failure:error=%1;\nerror msg: %2.").arg(QString::number(error)).arg(QString(QLatin1String(errorText))));

          //printf("Failure; error = %ld\n", error);
          //printf("error msg: %s\n", errorText);
       }
       else
       {
          // Print Warning
           qDebug()<<errorCode;
           Errorinfo=QString("Failure:error=%1;\nerror msg: %2.").arg(QString::number(error)).arg(QString(QLatin1String(errorText)));
           qDebug()<<QString("Failure:error=%1;\nerror msg: %2.").arg(QString::number(error)).arg(QString(QLatin1String(errorText)));
           //QMessageBox::warning(NULL,"Warning",QString("Failure:error=%1;\nerror msg: %2.").arg(QString::number(error)).arg(QString(QLatin1String(errorText))));
          //printf("warning = %ld\n", error);
          //printf("warning msg: %s\n\n", errorText);
       }

       if(!forbidError)
       {
           switch(m_currentType){
           case TestType::TCR:
               emit sendError(Errorinfo,0);
               break;
           case TestType::Probe:
               emit sendError(Errorinfo,1);
               break;
           case TestType::Calibrate:
               emit sendError(Errorinfo,2);
               break;
           default:
               break;

           }

       }



       free(errorText);
       return;

}

bool Dmm::ResultJud(ViStatus status,ViSession vi,ViStatus&error)
{
    if(VI_SUCCESS!=status)
    {
        Errorhandler(vi,status);
        return false;
    }
    else
    {
        error=status;
        return true;
    }

}

void Dmm::Set2Wire()
{
    measurementType=NIDMM_VAL_2_WIRE_RES;

}

void Dmm::Set4Wire()
{
    measurementType=NIDMM_VAL_4_WIRE_RES;
}

void Dmm::SetAutoRange()
{
    range=-1.0;
}

void Dmm::SetRange(qreal rangee)
{
    range=rangee;
}

void Dmm::SetResolution(qreal digitalResolution)
{
    resolution=digitalResolution;
}

void Dmm::SetMeasureInfo(int plate, int base)
{
    PlateNo=plate;
    BaseNo=base;
}

void Dmm::setdelaytime(int i)
{
    delaytime=i*1000;
}

void Dmm::SetProbe()
{
    PCchooseNo=1;
}

void Dmm::SetCalibrate()
{
    PCchooseNo=2;
}

void Dmm::RecordResults(int RNo,qreal value)
{
    switch (PlateNo) {
    case 0:
    {
        qDebug()<<"T1 Record";
        T1measureData[BaseNo][RNo-1]=value;
    }

        break;
    case 1:
    {
        qDebug()<<"T2 Record";
        T2measureData[BaseNo][RNo-1]=value;
    }

        break;
    case 2:
    {
        qDebug()<<"T3 Record";
        T3measureData[BaseNo][RNo-1]=value;
    }

        break;
    case 3:
    {
        qDebug()<<"T4 Record";
        T4measureData[RNo-1]=value;
    }

        break;
    case 4:
    {
        qDebug()<<"T5 Record";
        T5measureData[RNo-1]=value;
    }

        break;
    case 5:
    {
        qDebug()<<"T6 Record";
        T6measureData[BaseNo][RNo-1]=value;
    }
    default:
        break;
    }

}

void Dmm::RecordResults(qreal value)
{
    if(TCRBaseNo<6)
    {
        T1measureData[TCRBaseNo][TCRRno]=value;
    }
    else if(TCRBaseNo<12)
    {
        T2measureData[TCRBaseNo-6][TCRRno]=value;
    }
    else if(TCRBaseNo<18)
    {
        T3measureData[TCRBaseNo-12][TCRRno]=value;
    }
    else
    {
        T6measureData[TCRBaseNo-18][TCRRno]=value;
    }

}

void Dmm::Initial()
{
    Lrflag.clear();
    Lrflag.append(1);
    Lrflag.append(11);
    Lrflag.append(10);
    Lrflag.append(40);
    Lrflag.append(41);

}






arrData *Dmm::GetData(int Plate)
{
    switch (Plate) {
    case 0:
        return T1measureData;
        break;
    case 1:
        return T2measureData;
        break;
    case 2:
        return T3measureData;
        break;
    case 5:
        return T6measureData;
        break;

    }

}

qreal *Dmm::GetT4T5Data(int Plate)
{
    switch(Plate){
    case 3:
        return T4measureData;
        break;
    case 4:
        return T5measureData;
        break;
    }

}







bool Dmm::TCRMeasure()
{
    forbidError=false;

    if(ResultJud(niDMM_init(resourceName, idQuery, reset, &vi),vi,error))
    {
        if(TCRBaseNo<12)
        {
            //ResultJud(niDMM_ConfigureOffsetCompOhms(vi,NIDMM_VAL_OFFSET_COMP_OHMS_OFF),vi,error);
            range=-1.0;

        }
        else
        {
            range=100.0;

        }


         /*- Configure Measurement -------------------------------------------*/
        if(ResultJud(niDMM_ConfigureMeasurementDigits(vi, measurementType, range, resolution),vi,error))
        {
            /*- Configure Powerline Frequency -----------------------------------niDMM_SetAttributeViReal64(ViSession Instrument_Handle, ViConstString Channel_Name, ViAttr Attribute_ID, ViReal64 Attribute_Value)*/
            if(ResultJud(niDMM_ConfigurePowerLineFrequency (vi, powerlineFreq),vi,error))
            {/*- Read Measurement ------------------------------------------------

                if(measurementType==NIDMM_VAL_4_WIRE_RES)
                {
                    niDMM_SetAttributeViReal64(vi,"",NIDMM_ATTR_APERTURE_TIME,0.5);

                }*/
                if(TCRBaseNo>=12)
                {
                    if(ResultJud(niDMM_ConfigureOffsetCompOhms(vi,NIDMM_VAL_OFFSET_COMP_OHMS_ON),vi,error))
                        qDebug()<<"offset on set";
                    else
                        qDebug()<<"offset on fail";

                }


                forbidError=true;
                if(ResultJud(niDMM_Read (vi, NIDMM_VAL_TIME_LIMIT_AUTO, &reading),vi,error))
                {
                    /*- Get Absolute Resolution -------------------------------------*/
                    RecordResults(reading);
                    qDebug()<<"The "<<TCRRno<<"value is "<<reading;
                    emit MeasureFinish(false);
                    return true;

                }
                else
                {
                    ViBoolean isOverRange      = VI_FALSE;
                    //emit SendErrorMessage("DMM Read fail!");
                    niDMM_IsOverRange(vi,reading,&isOverRange);
                    if(isOverRange==VI_TRUE)
                    {
                        qDebug()<<"value is overrange";
                        //emit SendErrorMessage("value is overange");
                        RecordResults(-1.0);
                    }
                    else
                    {
                        RecordResults(-2.0);

                    }

                    emit MeasureFinish(false);

                    return false;
                }
               }



            else
            {
                //emit SendErrorMessage("DMM Configure PF fail!");

                if(vi)
                {
                    niDMM_close(vi);
                }
                return false;
            }
        }
        else
        {

            if(vi)
            {
                niDMM_close(vi);
            }
            return false;
        }

    }
    else
    {

        if(vi)
        {
            niDMM_close(vi);
        }
     return false;
    }


}

bool Dmm::ProbMeasure()
{
    forbidError=false;
    if(ResultJud(niDMM_init(resourceName, idQuery, reset, &vi),vi,error))
    {
         /*- Configure Measurement -------------------------------------------*/
        if(ResultJud(niDMM_ConfigureMeasurementDigits(vi, measurementType, range, resolution),vi,error))
        {
            //qDebug()<<range;
            /*- Configure Powerline Frequency -----------------------------------niDMM_SetAttributeViReal64(ViSession Instrument_Handle, ViConstString Channel_Name, ViAttr Attribute_ID, ViReal64 Attribute_Value)*/
            if(ResultJud(niDMM_ConfigurePowerLineFrequency (vi, powerlineFreq),vi,error))
            {/*- Read Measurement ------------------------------------------------*/
                /*if(ResultJud(niDMM_SetAttributeViReal64(vi,"", NIDMM_ATTR_APERTURE_TIME, 2.0),vi,error))
                {/*--Configure Aperture Time--------------------------------------
                    if(ResultJud(niDMM_ConfigureAutoZeroMode(vi,1),vi,error))
                    {*/
                if(isoffset)
                {
                    if(ResultJud(niDMM_ConfigureOffsetCompOhms(vi,NIDMM_VAL_OFFSET_COMP_OHMS_ON),vi,error))
                         qDebug()<<"offset on set";
                     else
                         qDebug()<<"offset on fail";

                }
                forbidError=true;

                if(ResultJud(niDMM_Read (vi, NIDMM_VAL_TIME_LIMIT_AUTO, &reading),vi,error))
                {
                    /*- Get Absolute Resolution -------------------------------------*/

                        qDebug()<<"test value is "<<reading;
                        RecordResults(tempRNo,reading);
                        //emit MeasureResult(qreal(reading),qreal(absoluteResolution));
                        if(tempRNo>=10)
                        {
                            if(vi)
                            {
                                niDMM_close(vi);
                            }
                        }
                        emit MeasureFinish(PlateNo-2, false);



                        return true;



                }
                else
                {
                    ViBoolean isOverRange      = VI_FALSE;
                    //emit SendErrorMessage("DMM Read fail!");
                    niDMM_IsOverRange(vi,reading,&isOverRange);
                    if(isOverRange==VI_TRUE)
                    {
                        qDebug()<<"value is overrange";
                        //emit SendErrorMessage("value is overange");
                        RecordResults(tempRNo,-1.0);
                    }
                    else
                    {
                        RecordResults(tempRNo,-2.0);

                    }

                    if(tempRNo>=10)
                    {
                        if(vi)
                        {
                            niDMM_close(vi);
                        }
                    }
                    emit MeasureFinish(PlateNo-2, false);
                    forbidError=false;
                    return true;
                }
                    }



            else
            {

                if(vi)
                {
                    niDMM_close(vi);
                }
                return false;
            }
        }
        else
        {

            if(vi)
            {
                niDMM_close(vi);
            }
            return false;
        }

    }
    else
    {

        if(vi)
        {
            niDMM_close(vi);
        }
     return false;
    }


}

bool Dmm::CalibraMeasure()
{
    forbidError=false;
    if(ResultJud(niDMM_init(resourceName, idQuery, reset, &vi),vi,error))
    {
         /*- Configure Measurement -------------------------------------------*/
        if(ResultJud(niDMM_ConfigureMeasurementDigits(vi, measurementType, range, resolution),vi,error))
        {
            /*- Configure Powerline Frequency -----------------------------------niDMM_SetAttributeViReal64(ViSession Instrument_Handle, ViConstString Channel_Name, ViAttr Attribute_ID, ViReal64 Attribute_Value)*/
            if(ResultJud(niDMM_ConfigurePowerLineFrequency (vi, powerlineFreq),vi,error))
            {/*- Read Measurement ------------------------------------------------*/
                /*if(ResultJud(niDMM_SetAttributeViReal64(vi,"", NIDMM_ATTR_APERTURE_TIME, 2.0),vi,error))
                {/*--Configure Aperture Time--------------------------------------
                    if(ResultJud(niDMM_ConfigureAutoZeroMode(vi,1),vi,error))
                    {*/
                if(isoffset)
                {
                    if(ResultJud(niDMM_ConfigureOffsetCompOhms(vi,NIDMM_VAL_OFFSET_COMP_OHMS_ON),vi,error))
                         qDebug()<<"offset on set";
                     else
                         qDebug()<<"offset on fail";

                }
                forbidError=true;

                if(ResultJud(niDMM_Read (vi, NIDMM_VAL_TIME_LIMIT_AUTO, &reading),vi,error))
                {
                    /*- Get Absolute Resolution -------------------------------------*/

                        RecordResults(tempRNo,reading);
                        //emit MeasureResult(qreal(reading),qreal(absoluteResolution));
                        if(tempRNo>=10)
                        {
                            if(vi)
                            {
                                niDMM_close(vi);
                            }
                        }
                        emit CaliMeasureFinish(PlateNo-2, false);



                        return true;


                }
                else
                {
                    ViBoolean isOverRange      = VI_FALSE;
                    //emit SendErrorMessage("DMM Read fail!");
                    niDMM_IsOverRange(vi,reading,&isOverRange);
                    if(isOverRange==VI_TRUE)
                    {
                        qDebug()<<"value is overrange";
                        //emit SendErrorMessage("value is overange");
                        RecordResults(tempRNo,-1.0);
                    }
                    else
                    {
                        RecordResults(tempRNo,-2.0);

                    }

                    if(tempRNo>=10)
                    {
                        if(vi)
                        {
                            niDMM_close(vi);
                        }
                    }
                    emit CaliMeasureFinish(PlateNo-2, false);
                }
                    }



            else
            {
                emit sendError("DMM Configure PF fail!",2);
                //emit SendErrorMessageC("DMM Configure PF fail!");
                if(vi)
                {
                    niDMM_close(vi);
                }
                return false;
            }
        }
        else
        {

            if(vi)
            {
                niDMM_close(vi);
            }
            return false;
        }

    }
    else
    {

        if(vi)
        {
            niDMM_close(vi);
        }
     return false;
    }


}




void Dmm::ReadyMeasure(short BaseNo, short Rno)
{
    m_currentType=TestType::TCR;
    TCRBaseNo=BaseNo;
    TCRRno=Rno;
    qDebug()<<"Dmm recieved";

    SetAutoRange();
    SetResolution(7.5);
    if(TCRBaseNo<12)
    {
        Set2Wire();
        //ResultJud(niDMM_ConfigureOffsetCompOhms(vi,NIDMM_VAL_OFFSET_COMP_OHMS_OFF),vi,error);
        SetRange(TCRT2range);
        delaytime=0;
    }
    else
    {
        Set4Wire();
        //ResultJud(niDMM_ConfigureOffsetCompOhms(vi,NIDMM_VAL_OFFSET_COMP_OHMS_ON),vi,error);
        SetRange(TCRT3range);
        delaytime=1000;
    }
    //QTimer::singleShot(delaytime,this,SLOT(TCRMeasure()));
    TCRMeasure();

}

void Dmm::ReadyMeasure(int plate, int resistancno)
{
    //qDebug()<<"Dmm Probe";
    PlateNo=plate+2;
    tempRNo=resistancno;
    switch(plate) {
        case 1:
        //SetAutoRange();
        SetRange(100.0);
        Set4Wire();
        SetResolution(7.5);
        break;
    case 2:
        SetRange(100000000);
        Set2Wire();
        SetResolution(7.5);

    }
    if(PCchooseNo==1)
    {
        qDebug()<<"Dmm Probe";
        m_currentType=TestType::Probe;
        ProbMeasure();
    }
    else if(PCchooseNo==2)
    {
        qDebug()<<"Dmm Calibra";
        m_currentType=TestType::Calibrate;

        CalibraMeasure();
    }



}

void Dmm::ReadyMeasure(int plate, int resistancno, short Moption)
{
    PlateNo=plate+2;
    tempRNo=resistancno;
    switch(plate) {
        case 1:
        //SetAutoRange();
        if(Lrflag.contains(Moption))
        {
            SetRange(100.0);
            isoffset=true;
            qDebug()<<"LR optium";

        }
        else
        {
            SetAutoRange();
            isoffset=false;
            qDebug()<<"Normal measure";
        }
        Set4Wire();
        SetResolution(7.5);
        break;
    case 2:
        SetRange(300000000);
        Set2Wire();
        SetResolution(7.5);

    }
    if(PCchooseNo==1)
    {
        qDebug()<<"Dmm Probe";
        m_currentType=TestType::Probe;
        ProbMeasure();
    }
    else if(PCchooseNo==2)
    {
        qDebug()<<"Dmm Calibra";
        m_currentType=TestType::Calibrate;

        CalibraMeasure();
    }


}



void Dmm::TCRrange(int T2range, int T3range)
{
    TCRT2range=T2range;
    TCRT3range=T3range;

}

void Dmm::RecieveTCRinfo()
{
    //QVector<arrData> tcrdata;
    //tcrdata.append(GetData(0));
    //tcrdata.append(GetData(1));
   // tcrdata.append(GetData(2));
    //tcrdata.append(GetData(5));
    //emit returnTCRdata(tcrdata);

}
