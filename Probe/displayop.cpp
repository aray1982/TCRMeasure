#include "displayop.h"
#include<QGraphicsProxyWidget>
#include<QLineEdit>



DisplayOp::DisplayOp(QGraphicsScene &scene, QObject *parent):
    QObject(parent),
    scene(scene),
    plate(new Baseplate(-50,-50))
{
    //QLineEdit* plineEdit=new QLineEdit("R1");
    //RLamp *b1=new RLamp(0,0);
    // lamp initial
    for(int i=0;i<10;i++)
    {
        int posx,posy;
        CalcuPos(i,posx,posy);
        QString rname="R"+QString::number(i+1);
        RLamp *temp=new RLamp(posx,posy,rname);
        scene.addItem(temp);
        lamps.append(temp);

    }
    //scene.addItem(lamp);
    scene.setSceneRect(-100,-100,200,200);
    scene.addItem(plate);



}

void DisplayOp::Changelampcolor(int rno, bool sw)
{
    lamps.at(rno-1)->ChangeColor(sw);
    scene.update();


}

void DisplayOp::SetBaseName(int i)
{
    plate->SetMark(i);
    scene.update();

}

void DisplayOp::SetName(QString name)
{
    plate->SetName(name);
    scene.update();

}

void DisplayOp::CalcuPos(int num, int &x, int &y)
{
    if(num<5)
    {
        y=-80;
        x=num*25-50;

    }
    else if(num>=5&&num<10)
    {
        y=80;
        x=(num-5)*25-50;

    }

}

void DisplayOp::closelamps()
{
    foreach (RLamp *temp, lamps) {
        temp->ChangeColor(false);

    }

}
