#include "rlamp.h"
#include<QPainter>

static const qreal LAMP_RADIUS = 10;
RLamp::RLamp(qreal x, qreal y,QString name):
    switcher(false),
    rname(name)
{
    setPos(x,y);

}

QRectF RLamp::boundingRect() const
{
    return QRectF(-TILE_SIZE,    -TILE_SIZE,
                  TILE_SIZE * 2, TILE_SIZE * 2 );

}

void RLamp::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), switcher?(Qt::green):(Qt::black));
    //painter->drawRect(0,0,TILE_SIZE,TILE_SIZE);

    painter->restore();


}

QPainterPath RLamp::shape() const
{

        QPainterPath p;
        p.addEllipse(QPointF(TILE_SIZE / 2, TILE_SIZE / 2), LAMP_RADIUS, LAMP_RADIUS);
        p.addText(QPointF(TILE_SIZE / 2, -10),QFont("Arial", 10),rname);

        return p;
}

void RLamp::ChangeColor(bool sw)
{
    switcher=sw;
    this->update();
}
