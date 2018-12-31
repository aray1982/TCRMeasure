#ifndef DISPLAYOP_H
#define DISPLAYOP_H

#include <QObject>
#include<QGraphicsScene>
#include"rlamp.h"
#include"baseplate.h"
//Probe测量电阻基片图形类，包含指示灯和基片
class RLamp;

class DisplayOp : public QObject
{
    Q_OBJECT
public:
    explicit DisplayOp(QGraphicsScene &scene,QObject *parent = nullptr);

signals:

public slots:
    void Changelampcolor(int rno,bool sw);//set lamp state
    void SetBaseName(int i);//set base name
    void SetName(QString name);//set plate name
private:
    QGraphicsScene &scene;
    QList<RLamp*> lamps;
    Baseplate *plate;
private:
    void CalcuPos(int num,int &x,int &y);//calculate lamp position
public:
    void closelamps();//make all lamps closed

};

#endif // DISPLAYOP_H
