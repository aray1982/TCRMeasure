#include "progressview.h"

ProgressView::ProgressView(QWidget *parent):QChartView(parent),
    m_series(new QLineSeries()),
    m_chart(new QChart())
{
    Initial();


}

ProgressView::~ProgressView()
{

}




void ProgressView::Initial()
{
    m_series->append(QPointF(0,25));
    m_chart->legend()->hide();


    m_chart->addSeries(m_series);

    QFont font;
    font.setPixelSize(18);
    m_chart->setTitleFont(font);
    m_chart->setTitleBrush(QBrush(Qt::white));
    m_chart->setTitle(tr("TCR流程"));
    QLinearGradient backgroundGradient;
    backgroundGradient.setStart(QPointF(0, 0));
    backgroundGradient.setFinalStop(QPointF(0, 1));
    backgroundGradient.setColorAt(0.0, QRgb(0xd2d0d1));
    backgroundGradient.setColorAt(1.0, QRgb(0x4c4547));
    backgroundGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    m_chart->setBackgroundBrush(backgroundGradient);

    // Customize plot area background
    QLinearGradient plotAreaGradient;
    plotAreaGradient.setStart(QPointF(0, 1));
    plotAreaGradient.setFinalStop(QPointF(1, 0));
    plotAreaGradient.setColorAt(0.0, QRgb(0x555555));
    plotAreaGradient.setColorAt(1.0, QRgb(0x55aa55));
    plotAreaGradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    m_chart->setPlotAreaBackgroundBrush(plotAreaGradient);
    m_chart->setPlotAreaBackgroundVisible(true);





    QCategoryAxis *axisX = new QCategoryAxis();
    //QCategoryAxis *axisY = new QCategoryAxis();
    QValueAxis *axisY=new QValueAxis;
    axisY->setTickCount(5);


    // Customize axis colors
    QPen axisPen(QRgb(0xd18952));
    axisPen.setWidth(2);
    axisX->setLinePen(axisPen);
    axisY->setLinePen(axisPen);

    // Customize axis label colors
    QBrush axisBrush(Qt::white);
    axisX->setLabelsBrush(axisBrush);
    axisY->setLabelsBrush(axisBrush);

    // Customize grid lines and shades
    axisX->setGridLineVisible(false);
    axisY->setGridLineVisible(false);
    axisY->setShadesPen(Qt::NoPen);
    axisY->setShadesBrush(QBrush(QColor(0x99, 0xcc, 0xcc, 0x55)));
    axisY->setShadesVisible(true);
    axisY->setRange(-55,125);
    axisX->setRange(0,180);


    axisX->append(tr("降温"), 10);
    axisX->append(tr("低温安定1"), 35);
    axisX->append(tr("低温安定2"), 60);
    axisX->append(tr("升温1"), 70);
    axisX->append(tr("常温安定1"), 95);
    axisX->append(tr("常温安定2"), 120);
    axisX->append(tr("升温2"), 130);
    axisX->append(tr("高温安定1"), 155);
    axisX->append(tr("高温安定2"), 180);

    QFont labelsFont("宋体",10.5,QFont::Bold);
    labelsFont.setPixelSize(10.5);
    axisX->setLabelsFont(labelsFont);
    axisY->setLabelsFont(labelsFont);





    m_chart->setAxisX(axisX, m_series);
    m_chart->setAxisY(axisY, m_series);



    this->setChart(m_chart);
    this->setRenderHint(QPainter::Antialiasing);

    limitvalue[0]=-55;
    limitvalue[1]=25;
    limitvalue[2]=125;





}

void ProgressView::stageProgress(short i)
{
    switch (i) {
    case 0:
        m_series->clear();
        m_series->append(QPointF(0,25));
        break;
    case 1:
        m_series->append(QPointF(10,limitvalue[0]));
        break;
    case 2:
        m_series->append(QPointF(35,limitvalue[0]));
        break;
    case 3:
        m_series->append(QPointF(60,limitvalue[0]));
        break;
    case 4:
        m_series->append(QPointF(70,limitvalue[1]));
        break;
    case 5:
        m_series->append(QPointF(95,limitvalue[1]));
        break;
    case 6:
        m_series->append(QPointF(120,limitvalue[1]));
        break;
    case 7:
        m_series->append(QPointF(130,limitvalue[2]));
        break;
    case 8:
        m_series->append(QPointF(155,limitvalue[2]));
        break;
    case 9:
        m_series->append(QPointF(180,limitvalue[2]));
        break;
    default:
        break;
    }





}

void ProgressView::setLimitvalue(qreal *limitdata)
{
    limitvalue[0]=*limitdata;
    limitvalue[1]=*(limitdata+1);
    limitvalue[2]=*(limitdata+2);

    m_chart->axisY()->setRange(limitvalue[0],limitvalue[2]+5);



}

void ProgressView::setYRange(const qreal &lowlimit, const qreal &highlimit)
{
    m_chart->axisY()->setRange(lowlimit,highlimit);

}
