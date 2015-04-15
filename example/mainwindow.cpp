#include <QDebug>
#include <QCheckBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <functional>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ///////////////////////////////////////////////////
    // Constructor with Lambda
    m_example = Property<bool>(false, [&](bool value){
        emit exampleChanged(value);
    });

    // OR

    // Constructor with Bound Function
    auto function = std::bind(&MainWindow::exampleChanged, this, std::placeholders::_1);
    m_example = Property<bool>(false, function);
    ///////////////////////////////////////////////////

    connect(this, &MainWindow::exampleChanged, this, [](bool value){
        qDebug() << "Value Changed:" << value;
    });

    connect(ui->checkBox, &QCheckBox::clicked, this, &MainWindow::setExample);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::example() const
{
    return m_example;
}

void MainWindow::setExample(bool value)
{
    m_example = value;
}
