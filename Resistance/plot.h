#ifndef PLOT_H
#define PLOT_H

#include <QWidget>
#include <QVector>
#include <QTimer>
#include "qcustomplot.h"

namespace Ui {
class Plot;
}

class Plot : public QWidget
{
    Q_OBJECT

public:
    double graphData;
    explicit Plot(QWidget *parent = nullptr);
    ~Plot();

private:
    Ui::Plot *ui;
    QCustomPlot *customPlot;
    QTimer *timer;
    int N=0;
    QVector<double> x,y;
public slots:
    void getData(QString resData);
private slots:
    void slotChanged();
    void resizeEvent(QResizeEvent *event);
};

#endif // PLOT_H
