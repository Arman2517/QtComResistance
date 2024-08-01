#include "detector.h"
#include "ui_detector.h"
#include <QSettings>
#include <QDir>
#include <QFileSystemModel>
#include <QDebug>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Detector::Detector(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Detector)
{
    ui->setupUi(this);

    ui->tb1->setRowCount(2);
    ui->tb1->setColumnCount(2);
    ui->tb1->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tb1->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tb1->setHorizontalHeaderLabels(QStringList()<<"-40"<<"кОм");
    for(int i=0;i<2;i++){
        QTableWidgetItem *ot = new QTableWidgetItem(i);//заполнение первого столбца
        ui->tb1->setItem(i,0,ot);
    }

    ui->tb2->setRowCount(2);
    ui->tb2->setColumnCount(2);
    ui->tb2->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tb2->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tb2->setHorizontalHeaderLabels(QStringList()<<"-10"<<"кОм");
    for(int i=0;i<2;i++){
        QTableWidgetItem *ot = new QTableWidgetItem(i);//заполнение первого столбца
        ui->tb2->setItem(i,0,ot);
    }

    ui->tb3->setRowCount(2);
    ui->tb3->setColumnCount(2);
    ui->tb3->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tb3->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tb3->setHorizontalHeaderLabels(QStringList()<<"25"<<"кОм");
    for(int i=0;i<2;i++){
        QTableWidgetItem *ot = new QTableWidgetItem(i);//заполнение первого столбца
        ui->tb3->setItem(i,0,ot);
    }

    ui->tb4->setRowCount(2);
    ui->tb4->setColumnCount(2);
    ui->tb4->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tb4->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tb4->setHorizontalHeaderLabels(QStringList()<<"40"<<"кОм");
    for(int i=0;i<2;i++){
        QTableWidgetItem *ot = new QTableWidgetItem(i);//заполнение первого столбца
        ui->tb4->setItem(i,0,ot);
    }

    ui->tb5->setRowCount(2);
    ui->tb5->setColumnCount(2);
    ui->tb5->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tb5->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tb5->setHorizontalHeaderLabels(QStringList()<<"60"<<"кОм");
    for(int i=0;i<2;i++){
        QTableWidgetItem *ot = new QTableWidgetItem(i);//заполнение первого столбца
        ui->tb5->setItem(i,0,ot);
    }


    QSettings settings("detector.conf",QSettings::IniFormat);
    settings.beginGroup("name");
    number=settings.value("count",-1).toInt();
    if(number>0){
        for(int i=number;i>0;i--){
            nameDat.insert(settings.value(QString::number(i),-1).toString());
            ui->cmbDetector->addItem(settings.value(QString::number(i),-1).toString());
        }
    }else number=0;
    settings.endGroup();


}

Detector::~Detector()
{
    delete ui;
}

void Detector::getResistance(QString s)
{
    //ui->reEdit->setText(s);
    resData=s;
}

void Detector::on_save_clicked()
{
    QSettings setting("detector.conf",QSettings::IniFormat);
    setting.beginGroup("name");
    QString nameEdit=ui->nameEdit->text();
    if(!nameDat.count(nameEdit)){
        number++;
        nameDat.insert(nameEdit);
        setting.setValue(QString::number(number,10),nameEdit);
        setting.setValue("count",number);
        QDir().mkdir(nameEdit);
        nameEdit.push_front("./");
        nameEdit.push_back("/");
        nameEdit.push_back("1");//изменить number(МОЖЕТ ПОВТОРЯТЬСЯ)
        nameEdit.push_back(".json");

        QFile file(nameEdit);
        file.open(QIODevice::WriteOnly);
        QJsonObject json;
        json["rowCount"] = ui->tb1->rowCount();
        json["columnCount"] = ui->tb1->columnCount();

        QJsonArray data;
        for(int i= 0 ; i<ui->tb1->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb1->columnCount(); j++){
                if(ui->tb1->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb1->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb1"] = data;

        while(data.count()){
            data.pop_back();
        }

        for(int i= 0 ; i<ui->tb2->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb2->columnCount(); j++){
                if(ui->tb2->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb2->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb2"] = data;

        while(data.count()){
            data.pop_back();
        }
        for(int i=0; i<ui->tb3->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb3->columnCount(); j++){
                if(ui->tb3->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb3->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb3"] = data;

        while(data.count()){
            data.pop_back();
        }

        for(int i= 0 ; i<ui->tb4->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb4->columnCount(); j++){
                if(ui->tb4->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb4->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb4"] = data;

        while(data.count()){
            data.pop_back();
        }

        for(int i= 0 ; i<ui->tb5->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb5->columnCount(); j++){
                if(ui->tb5->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb5->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb5"] = data;
        QJsonDocument saveDoc(json);
        file.write(saveDoc.toJson());
        file.close();

    }else{
        nameEdit.push_front("./");
        nameEdit.push_back("/");
        nameEdit.push_back("1");//изменить number(МОЖЕТ ПОВТОРЯТЬСЯ)
        nameEdit.push_back(".json");
        QFile file(nameEdit);
        file.open(QIODevice::WriteOnly);
        QJsonObject json;
        json["rowCount"] = ui->tb1->rowCount();
        json["columnCount"] = ui->tb1->columnCount();

        QJsonArray data;
        for(int i= 0 ; i<ui->tb1->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb1->columnCount(); j++){
                if(ui->tb1->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb1->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb1"] = data;

        while(data.count()){
            data.pop_back();
        }

        for(int i= 0 ; i<ui->tb2->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb2->columnCount(); j++){
                if(ui->tb2->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb2->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb2"] = data;

        while(data.count()){
            data.pop_back();
        }
        for(int i=0; i<ui->tb3->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb3->columnCount(); j++){
                if(ui->tb3->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb3->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb3"] = data;

        while(data.count()){
            data.pop_back();
        }

        for(int i= 0 ; i<ui->tb4->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb4->columnCount(); j++){
                if(ui->tb4->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb4->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb4"] = data;

        while(data.count()){
            data.pop_back();
        }

        for(int i= 0 ; i<ui->tb5->rowCount(); i++){
            QJsonArray row;
            for(int j=0; j<ui->tb5->columnCount(); j++){
                if(ui->tb5->item(i,j)!=0){
                    row.append(QJsonValue(ui->tb5->item(i,j)->text()));
                }
            }
            data.append(row);
        }
        json["tb5"] = data;
        QJsonDocument saveDoc(json);
        file.write(saveDoc.toJson());
        file.close();
    }
    setting.endGroup();

}


void Detector::on_loadFromJson_clicked()
{
    QString path("./");
    path.push_back(ui->cmbDetector->currentText());
    path.push_back("/");
    path.push_back(ui->listFile->currentItem()->text());
    while (countTb1>0){
        delete_table(ui->tb1,countTb1);
    }
    while (countTb2>0){
        delete_table(ui->tb2,countTb2);
    }
    while (countTb3>0){
        delete_table(ui->tb3,countTb3);
    }
    while (countTb4>0){
        delete_table(ui->tb4,countTb4);
    }
    while (countTb5>0){
        delete_table(ui->tb5,countTb5);
    }

    QString val;
    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    val = file.readAll();


    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();


    QString tb;
    tb="tb1";
    procJson(json[tb].toArray(),countTb1,*ui->tb1);

    tb="tb2";
    procJson(json[tb].toArray(),countTb2,*ui->tb2);

    tb="tb3";
    procJson(json[tb].toArray(),countTb3,*ui->tb3);

    tb="tb4";
    procJson(json[tb].toArray(),countTb4,*ui->tb4);

    tb="tb5";
    procJson(json[tb].toArray(),countTb5,*ui->tb5);
    file.close();

}

void Detector::procJson(QJsonArray data,int &count,QTableWidget &tw)
{
    for(int i=0;i<data.size();i++){
        QJsonArray arr = data[i].toArray();
        if(arr.size()>1 && arr[1]!=""){
            for (int j=0;j<3;j++) {//можно начать с j=1;
                QTableWidgetItem *tb = new QTableWidgetItem(arr[j].toString());
                tw.setItem(i,j,tb);
            }
            count=count+1;
        }
    }
}

void Detector::on_cmbDetector_activated(const QString &arg1)
{
    ui->listFile->clear();//
    QString path=arg1;
    path.push_front("./");

    QDir dir(path);
    QFileInfoList dirContent = dir.entryInfoList(QStringList()<<"*.json",QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    foreach(QFileInfo flInfo,dirContent)
    {
        QString name=flInfo.fileName();
        ui->listFile->addItem(name);//
    }

}

void Detector::on_write_one_clicked()
{
    if(ui->rb1->isChecked()){
        if(write_table(ui->tb1,countTb1)==1) countTb1=countTb1+1;
    }
    if(ui->rb2->isChecked()){
        if(write_table(ui->tb2,countTb2)==1) countTb2=countTb2+1;
    }
    if(ui->rb3->isChecked()){
        if(write_table(ui->tb3,countTb3)==1) countTb3=countTb3+1;
    }
    if(ui->rb4->isChecked()){
        if(write_table(ui->tb4,countTb4)==1) countTb4=countTb4+1;
    }
    if(ui->rb5->isChecked()){
        if(write_table(ui->tb5,countTb5)==1)countTb5=countTb5+1;
    }
}

bool Detector::write_table(QTableWidget *tw,int &count)
{
    if(resData.size()>1 && count<2)
    {
        QTableWidgetItem *tb = new QTableWidgetItem(resData);
        //QTableWidgetItem *tb1 = new QTableWidgetItem(tempData);
        tw->setItem(count,1,tb);
        //tw->setItem(count,2,tb1);
        resData="";
        //tempData="";

    }
    else if(count>1 && resData.size()>1){
        QMessageBox::critical(this,"Write error","No place to write!");
        return false;
    }
    else{
        QMessageBox::critical(this,"Write error","No data to write!");
        return false;
    }
    return true;
}

void Detector::on_delete_one_clicked()
{
    if(ui->rb1->isChecked()){
        delete_table(ui->tb1,countTb1);
    }
    if(ui->rb2->isChecked()){
        delete_table(ui->tb2,countTb2);
    }
    if(ui->rb3->isChecked()){
        delete_table(ui->tb3,countTb3);
    }
    if(ui->rb4->isChecked()){
        delete_table(ui->tb4,countTb4);
    }
    if(ui->rb5->isChecked()){
        delete_table(ui->tb5,countTb5);
    }
}

void Detector::delete_table(QTableWidget *tw, int &count)
{
    if(count>0){
        count=count-1;
        tw->setItem(count,1,NULL);
        tw->setItem(count,2,NULL);
    }else QMessageBox::critical(this,"Delete error","Nothing to delete!");
}


