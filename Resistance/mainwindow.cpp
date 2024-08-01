#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegExp>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadPorts();

    connect(&thread_1,&QThread::started,&port,[this](){port.connect(ui->cmbPorts->currentText());});// Сигнал для сохдания потока
    connect(&port,&cPort::threadQuit,&thread_1,&QThread::terminate);
    port.moveToThread(&thread_1);

    connect(&port,&cPort::dataReceived,this,&MainWindow::readData);//Получение данных с класса Порта
    connect(this,&MainWindow::portQuit,&port,&cPort::quitPort);//Сигнал закрытия порта
    connect(this,&MainWindow::unitsDataSend,&port,&cPort::setText);
    connect(this,&MainWindow::plotData,&ploter,&Plot::getData);
    connect(this,&MainWindow::sendResData,&sensor,&Detector::getResistance);

    QFont font;
    font.setPointSize(15);
    ui->cmbRes->addItem("100МОм");
    ui->cmbRes->addItem("10МОм");
    ui->cmbRes->addItem("1МОм");
    ui->cmbRes->addItem("100кОм");
    ui->cmbRes->addItem("10кОм");
    ui->cmbRes->addItem("1кОм");
    ui->cmbRes->addItem("100Ом");
    ui->cmbRes->setCurrentIndex(4);


    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"МОм"<<"кОм"<<"Ом");
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);


}

void MainWindow::loadPorts()
{
    foreach(auto port,QSerialPortInfo::availablePorts())
    {
        ui->cmbPorts->addItem(port.portName());
    }
}

void MainWindow::readData()
{
    if(port.resData!=NULL){
        port.resData.push_back(text1);
        ui->resEdit->setText(port.resData);
        data=port.resData;
    }
    emit plotData(port.resData);
    emit sendResData(port.resData);

}

void MainWindow::on_Start_clicked()
{
    if(startPort==false){
        thread_1.start();
        startPort=true;
    }else
    {
        QMessageBox::warning(this,"Ошибка подключения к порту","Порт уже открыт");
    }
}

void MainWindow::on_Stop_clicked()
{
    if(startPort==true){
        startPort=false;
        emit portQuit();
    }else
    {
        QMessageBox::warning(this,"Ошибка подключения к порту","Нет открытых портов");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_changeUn_clicked()
{
    QRegExp rx("[к-М-О]");
    text=ui->cmbRes->currentText();
    int pos=0;
    pos=rx.indexIn(text,pos);
    if(pos!=-1){
        text1=text;
        text1.remove(0,pos);
        text.remove(pos,3);//
    }
    unitsDataSend(text,text1);
}

void MainWindow::on_createPlot_clicked()
{
    ploter.show();
}



void MainWindow::on_writeOne_clicked()//ИЗМЕНИТЬ
{
    if(data.size()>2 && count<10)
    {
        QTableWidgetItem *tb = new QTableWidgetItem(data);
        ui->tableWidget->setItem(count,1,tb);
        data.clear();
        count=count+1;
    }
    else if(count==10 && data.size()>2)QMessageBox::critical(this,"Write error","No place to write!");
    else QMessageBox::critical(this,"Write error","No data to write!");
}


void MainWindow::on_deleteOne_clicked()
{
    if(count>0)
    {
        count=count-1;
        ui->tableWidget->setItem(count,0,NULL);
        ui->tableWidget->setItem(count,1,NULL);
        ui->tableWidget->setItem(count,2,NULL);

    }else QMessageBox::critical(this,"Delete error","No data to delete!");
}


void MainWindow::on_saveToJson_clicked()
{

    QFile fileJson(QFileDialog::getSaveFileName());//
    fileJson.open(QIODevice::WriteOnly);

    QJsonObject json;
    json["rowCount"] = ui->tableWidget->rowCount();
    json["columnCount"] = ui->tableWidget->columnCount();

    QJsonArray data;
    for(int i= 0; i<ui->tableWidget->rowCount(); i++){
        QJsonArray row;
        for(int j=0; j<ui->tableWidget->columnCount(); j++){
            if(ui->tableWidget->item(i,j)!=0){
                row.append(QJsonValue(ui->tableWidget->item(i,j)->text()));
            }
        }
        data.append(row);
    }
    json["data"] = data;
    QJsonDocument saveDoc(json);
    fileJson.write(saveDoc.toJson());
    fileJson.close();
}

void MainWindow::on_loadFromJson_clicked()
{
    QString val;
    QFile file;
    file.setFileName(QFileDialog::getOpenFileName());
    file.open(QIODevice::ReadWrite | QIODevice::Text);
    val = file.readAll();


    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject json = doc.object();
    QJsonArray data = json["data"].toArray();

    while(count>0){
        on_deleteOne_clicked();
    }

    for(int i=0;i<data.size();i++){
        if(data[i].toArray().size()>0){//Если в ячейках не пусто
            QJsonArray arr = data[i].toArray();
            count++;
            for (int j=0;j<3;j++){
                    QTableWidgetItem *tb = new QTableWidgetItem(arr[j].toString());
                    ui->tableWidget->setItem(i,j,tb);
            }
        }
    }
    file.close();
}

void MainWindow::on_selectDat_clicked()
{
    sensor.show();
}
