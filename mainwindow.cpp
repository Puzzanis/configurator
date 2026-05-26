#include "mainwindow.h"
#include "objecttreemodel.h"
#include <QTreeView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ObjectTreeModel *model = new ObjectTreeModel(this);

    // Добавляем тестовые объекты
    model->addObject(ObjectCategories::ConfigRack, "Корзина №1");
    model->addObject(ObjectCategories::ConfigRack, "Корзина №2");

    model->addObject(ObjectCategories::TableSignal, "Сигнал АЦП");
    model->addObject(ObjectCategories::TableSignal, "Сигнал датчика");
    model->addObject(ObjectCategories::TableSignal, "Опорный сигнал");

    model->addObject(ObjectCategories::D_Cadr, "Кадр 1");
    model->addObject(ObjectCategories::D_Cadr, "Кадр 2");

    model->addObject(ObjectCategories::DP, "Параметр 0x01");
    model->addObject(ObjectCategories::DP, "Параметр 0x02");
    model->addObject(ObjectCategories::DP, "Параметр 0x03");

    QTreeView *view = new QTreeView(this);
    view->setModel(model);
    view->expandAll();
    view->setSelectionMode(QAbstractItemView::SingleSelection);

    setCentralWidget(view);
    resize(450, 450);
    setWindowTitle("Конфигуратор сигналов");
}

MainWindow::~MainWindow()
{
}