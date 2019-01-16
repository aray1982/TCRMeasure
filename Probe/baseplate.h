#ifndef BASEPLATE_H
#define BASEPLATE_H
/*baseplate 为PRob测试中图标显示基板
*/
#include <QObject>
#include<QGraphicsItem>
#include"constants.h"

class Baseplate : public QGraphicsItem
{
public:
    Baseplate(qreal x, qreal y);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;

private:
    QString mark;
public:
    void SetMark(int BaseNo);
    void SetName(QString name);
};




#endif // BASEPLATE_H
