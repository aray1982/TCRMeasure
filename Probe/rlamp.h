#ifndef RLAMP_H
#define RLAMP_H

#include <QObject>
#include<QGraphicsItem>
#include"constants.h"

//Probe测试图形类中指示灯类

class RLamp : public QGraphicsItem
{
public:
    RLamp(qreal x, qreal y,QString name);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    void ChangeColor(bool sw);
private:
    bool switcher;
    QString rname;
};

#endif // RLAMP_H
