#ifndef OBJECTTREEMODEL_H
#define OBJECTTREEMODEL_H

#include <QAbstractItemModel>
#include "treenode.h"
#include "categories.h"  // Подключаем наш новый файл

class ObjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    // Используем категории из внешнего файла
    using Category = ObjectCategories::Category;

    explicit ObjectTreeModel(QObject *parent = nullptr);
    ~ObjectTreeModel();

    // Обязательные методы модели (без изменений)
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Методы для работы с объектами
    QModelIndex addObject(Category category, const QString &name);
    void removeObject(const QModelIndex &index);
    TreeNode *nodeFromIndex(const QModelIndex &index) const;
    QModelIndex categoryIndex(Category category) const;
    Category indexToCategory(const QModelIndex &index) const;

private:
    void createCategories();
    TreeNode *m_rootNode;

    // Динамический массив для категорий
    QList<TreeNode*> m_categories;  // Теперь QList, не C-массив
};

#endif // OBJECTTREEMODEL_H