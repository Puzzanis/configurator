#ifndef TREENODE_H
#define TREENODE_H

#include <QString>
#include <QList>
#include <QMap>

class TreeNode
{
public:
    // Конструктор: имя узла и указатель на родителя
    // parent = nullptr значит это корневой элемент
    explicit TreeNode(const QString &name, TreeNode *parent = nullptr);
    ~TreeNode();

    // Методы для работы с дочерними узлами
    void appendChild(TreeNode *child);        // Добавить ребёнка
    void removeChild(int row);                // Удалить ребёнка по номеру
    TreeNode *child(int row) const;           // Получить ребёнка
    int childCount() const;                   // Сколько детей?
    int row() const;                          // Какой я по счёту у родителя?
    TreeNode *parentNode() const;             // Кто мой родитель?

    // Данные узла
    QString name() const;                     // Имя объекта
    void setName(const QString &name);        // Переименовать

    // Свойства объекта (ключ-значение)
    QString property(const QString &key) const;
    void setProperty(const QString &key, const QString &value);
    QStringList propertyKeys() const;         // Список всех ключей

private:
    QString m_name;                           // Имя узла
    TreeNode *m_parent;                       // Родитель (nullptr для корня)
    QList<TreeNode*> m_children;              // Список детей
    QMap<QString, QString> m_properties;      // Свойства (тип, позиция и т.д.)
};

#endif // TREENODE_H