/*
The MIT License (MIT)

Copyright (c) 2015

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
