#ifndef DETECTOR_H
#define DETECTOR_H

#include <QWidget>
#include <set>
#include <QTableWidget>

namespace Ui {
class Detector;
}

class Detector : public QWidget
{
    Q_OBJECT

public:
    explicit Detector(QWidget *parent = nullptr);
    ~Detector();

public slots:
    void getResistance(QString s);

private slots:
    void on_save_clicked();

    void on_loadFromJson_clicked();

    void on_cmbDetector_activated(const QString &arg1);

    void on_write_one_clicked();

    void on_delete_one_clicked();


private:
    bool write_table(QTableWidget *tw,int &count);
    void delete_table(QTableWidget *tw, int &count);
    void procJson(QJsonArray data,int &count,QTableWidget &tw);
    Ui::Detector *ui;
    int number=0;
    std::set<QString> nameDat;
    QString resData;
    int countTb1=0,countTb2=0,countTb3=0,countTb4=0,countTb5=0;
};

#endif // DETECTOR_H
