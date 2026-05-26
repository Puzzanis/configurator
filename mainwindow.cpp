#include "mainwindow.h"
#include <QLabel>  // Временный элемент для проверки

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Временный код для проверки
    QLabel *testLabel = new QLabel("Привет! Qt работает!", this);
    testLabel->setAlignment(Qt::AlignCenter);
    setCentralWidget(testLabel);

    resize(800, 500);
}

MainWindow::~MainWindow()
{
}