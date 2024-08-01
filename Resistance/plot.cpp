#include "plot.h"
#include "ui_plot.h"

Plot::Plot(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Plot)
{
    ui->setupUi(this);
    customPlot=new QCustomPlot(this);

    ui->widget->setLocale(QLocale(QLocale::Russian,QLocale::Russia));

    customPlot->setInteraction(QCP::iRangeZoom,true);
    customPlot->setInteraction(QCP::iRangeDrag,true);
    customPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    customPlot->xAxis->setLabel("x");
    customPlot->yAxis->setLabel("y");

    customPlot->xAxis->setRange(0,100);
    customPlot->yAxis->setRange(0,10);
    customPlot->replot();
    timer=new QTimer(this);
    timer->start(500);
    connect(timer,&QTimer::timeout,this,&Plot::slotChanged);

}

void Plot::slotChanged()
{
    if(graphData==0){
        return;
    }
    x.append(N);
    y.append(graphData);
    N++;

    customPlot->clearGraphs();
    customPlot->addGraph();
    customPlot->graph(0)->setPen(QPen(Qt::blue));
    customPlot->graph(0)->setBrush(QBrush(QColor(0,0,255,20)));
    customPlot->graph(0)->setData(x,y);
    customPlot->replot();

}

void Plot::getData(QString resData)
{
    QRegExp rx("[к-М-О]");
    int pos=0;
    pos=rx.indexIn(resData,pos);
    if(pos!=-1){
        resData.remove(pos,10);
        graphData=resData.toDouble();
    }
}

void Plot::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    customPlot->setGeometry(0,0,this->width(),this->height());
}
Plot::~Plot()
{
    delete ui;
}
