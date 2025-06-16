#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->quickWidget->setSource(QUrl("qrc:/qml.qml"));
    ui->quickWidget->show();

    h = 0.1;
    xBegin = -3;
    xEnd = 3 + h;

    N = (xEnd - xBegin)/h + 2;



    ui->widget->addGraph();
    ui->widget->graph(0)->addData(x, y);
    ui->widget->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::TimerSlot()
{
    if(time <= 20 * N)
    {
        if(X <= xEnd)
        {
            x.push_back(X);
            if(ui->radioButton->isChecked())
            {
                y.push_back(tan(X));
            }
            else if(ui->radioButton_2->isChecked())
            {
                y.push_back(cos(X));
            }
            else if(ui->radioButton_3->isChecked())
            {
                // формулы
                double a = X;
                y1.push_back(a);
                double b = -X;
                y2.push_back(b);
                double c = X*X;
                y3.push_back(c);
                double d = X+X;
                y4.push_back(d);
            }
            X += h;
        }
        time += 20;
    }
    else
    {
        time = 0;
        timer->stop();
    }

    if(!ui->radioButton_3->isChecked())
    {
        ui->widget->addGraph();
        ui->widget->graph(0)->addData(x, y);
    }
    else
    {
        ui->widget->addGraph();
        ui->widget->graph(0)->addData(x, y1);
        ui->widget->addGraph();
        ui->widget->graph(1)->addData(x, y2);
        ui->widget->addGraph();
        ui->widget->graph(2)->addData(x, y3);
        ui->widget->addGraph();
        ui->widget->graph(3)->addData(x, y4);
    }
    ui->widget->replot();
}

void MainWindow::ChangeFunc()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(TimerSlot()));
    ui->widget->clearGraphs();
    time = 0;
    timer->start(20);
    X = xBegin;
    x.clear();
    y.clear();
}


void MainWindow::on_radioButton_clicked()
{
    xBegin = -3;
    xEnd = 3 + h;
    N = (xEnd - xBegin)/h + 2;
    ui->widget->xAxis->setRange(-4, 4);
    ui->widget->yAxis->setRange(0, 9);
    ChangeFunc();
}


void MainWindow::on_radioButton_2_clicked()
{
    xBegin = -3;
    xEnd = 3 + h;
    N = (xEnd - xBegin)/h + 2;
    ui->widget->xAxis->setRange(-4, 4);
    ui->widget->yAxis->setRange(-2, 2);
    ChangeFunc();
}


void MainWindow::on_radioButton_3_clicked()
{
    ui->widget->xAxis->setRange(-8, 8);
    ui->widget->yAxis->setRange(-4, 5);
    xBegin = -7;
    xEnd = 7 + h;
    N = (xEnd - xBegin)/h + 2;
    ChangeFunc();
}

