#include "treenode.h"

// Конструктор
TreeNode::TreeNode(const QString &name, TreeNode *parent)
    : m_name(name), m_parent(parent)
{
    // При создании объекта сразу задаём базовые свойства
    // Это как шаблон для новых объектов
    m_properties["Тип"] = "Объект";
    m_properties["Видимость"] = "true";
    m_properties["Позиция X"] = "0";
    m_properties["Позиция Y"] = "0";
    m_properties["Ширина"] = "100";
    m_properties["Высота"] = "100";
}

// Деструктор - удаляем всех детей рекурсивно
TreeNode::~TreeNode()
{
    // qDeleteAll удаляет все объекты в списке
    qDeleteAll(m_children);
    m_children.clear();
}

// Добавляем ребёнка в конец списка
void TreeNode::appendChild(TreeNode *child)
{
    m_children.append(child);
}

// Удаляем ребёнка по индексу
void TreeNode::removeChild(int row)
{
    if (row >= 0 && row < m_children.size()) {
        delete m_children.takeAt(row);  // takeAt забирает И удаляет из списка
    }
}

// Получаем ребёнка по индексу
TreeNode *TreeNode::child(int row) const
{
    if (row >= 0 && row < m_children.size())
        return m_children.at(row);
    return nullptr;  // Если индекс неправильный - возвращаем nullptr
}

// Сколько детей?
int TreeNode::childCount() const
{
    return m_children.size();
}

// Какая позиция этого узла у родителя?
int TreeNode::row() const
{
    if (m_parent)
        // Ищем себя в списке детей родителя
        return m_parent->m_children.indexOf(const_cast<TreeNode*>(this));
    return 0;  // Корневой узел всегда 0
}

// Кто родитель?
TreeNode *TreeNode::parentNode() const
{
    return m_parent;
}

// Получаем имя
QString TreeNode::name() const
{
    return m_name;
}

// Меняем имя
void TreeNode::setName(const QString &name)
{
    m_name = name;
}

// Получаем значение свойства по ключу
QString TreeNode::property(const QString &key) const
{
    // value() возвращает значение или пустую строку если ключа нет
    return m_properties.value(key, "");
}

// Устанавливаем свойство
void TreeNode::setProperty(const QString &key, const QString &value)
{
    m_properties[key] = value;  // Если ключ есть - обновит, если нет - создаст
}

// Возвращаем список всех ключей свойств
QStringList TreeNode::propertyKeys() const
{
    return m_properties.keys();
}