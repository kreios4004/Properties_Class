#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../property.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
        Q_PROPERTY(bool example READ example WRITE setExample NOTIFY exampleChanged)

    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();

        bool example() const;

    signals:
        void exampleChanged(bool value);

    public slots:
        void setExample(bool value);

    private:
        Ui::MainWindow *ui;
        Property<bool> m_example;
};

#endif // MAINWINDOW_H
