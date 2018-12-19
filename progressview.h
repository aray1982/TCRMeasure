﻿#ifndef PROGRESSVIEW_H
#define PROGRESSVIEW_H

#include <QObject>
#include <QtCharts/QChartView>
#include <QtWidgets/QRubberBand>
#include <QtCharts/QLineSeries>
#include<QLineSeries>
#include<QCategoryAxis>
#include"gloabaldefine.h"
#include<QValueAxis>

//TCR测试流程图形类

QT_CHARTS_USE_NAMESPACE
class ProgressView : public QChartView
{
    Q_OBJECT
public:
    ProgressView(QWidget *parent=nullptr);
    ~ProgressView();
private:
    QLineSeries *m_series;

    QChart *m_chart;
private:
    void Initial();
public slots:
    void stageProgress(short i);
};

#endif // PROGRESSVIEW_H
