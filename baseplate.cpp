#include "baseplate.h"

#include<QPainter>
#include<QTextItem>

static const qreal BASE_LENGTH = 100;
Baseplate::Baseplate(qreal x, qreal y):
    mark("A")
{
    setPos(x,y);

}

QRectF Baseplate::boundingRect() const
{
    return QRectF(-BASE_SIZE,    -BASE_SIZE,
                  BASE_SIZE * 2, BASE_SIZE * 2 );

}

void Baseplate::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setFont(QFont("Arial", 60));

    //painter->fillPath(shape(), switcher?(Qt::green):(Qt::black));
    painter->drawRect(0, 0,BASE_LENGTH,BASE_LENGTH);
    painter->drawText(QPointF(3, BASE_SIZE+30),mark);

    painter->restore();


}

QPainterPath Baseplate::shape() const
{

        QPainterPath p;
        //p.addEllipse(QPointF(BASE_SIZE / 2, BASE_SIZE / 2), LAMP_RADIUS, LAMP_RADIUS);
        //p.addText(QPointF(BASE_SIZE / 2, -10),QFont("Arial", 10),"R1");
        //QGraphicsTextItem mark("R1",this);
        //mark.setFont(QFont("Arial", 5));
        //mark.setPos(0,20);
        return p;
}



void Baseplate::SetMark(int BaseNo)
{
    mark="A"+QString::number(BaseNo+1);
    this->update();

}

void Baseplate::SetName(QString name)
{
    mark=name;
    this->update();

}

