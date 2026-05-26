#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTreeView;      // Предварительное объявление
class QSplitter;      // чтобы не подключать
class PropertyPanel;  // все заголовки в .h файле
class ObjectTreeModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // Пока оставим пустым, будем добавлять постепенно
};

#endif // MAINWINDOW_H