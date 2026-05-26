#include "objecttreemodel.h"
#include <QIcon>

ObjectTreeModel::ObjectTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    // Создаём невидимый корень дерева
    // Он нужен для правильной работы модели, но не отображается
    m_rootNode = new TreeNode("СкрытыйКорень");
    createCategories();
}

ObjectTreeModel::~ObjectTreeModel()
{
    delete m_rootNode;  // Рекурсивно удалит все узлы
}

void ObjectTreeModel::createCategories()
{
    // Получаем все категории из реестра
    QList<Category> categories = ObjectCategories::allCategories();

    for (const auto &cat : categories) {
        // Создаём узел с данными из Categories
        TreeNode *categoryNode = new TreeNode(ObjectCategories::name(cat), m_rootNode);
        categoryNode->setProperty("Тип", ObjectCategories::info(cat).typePrefix);
        categoryNode->setProperty("Заблокировано", "true");

        m_rootNode->appendChild(categoryNode);
        m_categories.append(categoryNode);  // Сохраняем указатель
    }
}

// ============ ОБЯЗАТЕЛЬНЫЕ МЕТОДЫ МОДЕЛИ ============

// Создаёт QModelIndex для элемента
QModelIndex ObjectTreeModel::index(int row, int column,
                                   const QModelIndex &parent) const
{
    // Проверяем, что row и column корректны
    if (!hasIndex(row, column, parent))
        return QModelIndex();  // Пустой индекс = ошибка

    // Определяем родительский узел
    TreeNode *parentNode;
    if (!parent.isValid())
        parentNode = m_rootNode;  // Если родитель не указан — корень
    else
        parentNode = static_cast<TreeNode*>(parent.internalPointer());

    // Получаем дочерний узел
    TreeNode *childNode = parentNode->child(row);
    if (childNode)
        // createIndex связывает номер строки/колонки с узлом
        return createIndex(row, column, childNode);

    return QModelIndex();
}

// Находит родителя для элемента
QModelIndex ObjectTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNode *childNode = static_cast<TreeNode*>(index.internalPointer());
    TreeNode *parentNode = childNode->parentNode();

    // Если родитель — скрытый корень, возвращаем пустой индекс
    if (parentNode == m_rootNode)
        return QModelIndex();

    // Иначе создаём индекс родителя
    return createIndex(parentNode->row(), 0, parentNode);
}

// Сколько дочерних элементов у parent?
int ObjectTreeModel::rowCount(const QModelIndex &parent) const
{
    // Игнорируем колонки > 0 (у нас только одна колонка)
    if (parent.column() > 0)
        return 0;

    TreeNode *parentNode;
    if (!parent.isValid())
        parentNode = m_rootNode;  // Запрашивают детей корня
    else
        parentNode = static_cast<TreeNode*>(parent.internalPointer());

    return parentNode->childCount();
}

// Сколько колонок? У нас только имя объекта — 1 колонка
int ObjectTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)  // Не зависит от родителя
    return 1;
}

// Какие данные показывать для элемента?
QVariant ObjectTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeNode *node = static_cast<TreeNode*>(index.internalPointer());

    // Роль DisplayRole — текст, который виден в дереве
    if (role == Qt::DisplayRole) {
        return node->name();  // Просто имя объекта
    }

    // Роль DecorationRole — иконка слева от текста
    if (role == Qt::DecorationRole) {
        // Для категорий используем иконки из ObjectCategories
        if (node->parentNode() == m_rootNode) {
            Category cat = indexToCategory(index);
            return ObjectCategories::icon(cat);  // Берём иконку из реестра
        } else {
            return QIcon::fromTheme("text-x-generic");
        }
    }

    // Роль ToolTipRole — всплывающая подсказка
    if (role == Qt::ToolTipRole) {
        return QString("Тип: %1\nДочерних объектов: %2")
            .arg(node->property("Тип"))
            .arg(node->childCount());
    }

    return QVariant();  // Для остальных ролей — пусто
}

// Заголовки колонок
QVariant ObjectTreeModel::headerData(int section,
                                     Qt::Orientation orientation,
                                     int role) const
{
    // Только для горизонтального заголовка и роли DisplayRole
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if (section == 0)
            return "Дерево объектов";  // Название единственной колонки
    }
    return QVariant();
}

// Флаги — что можно делать с элементом?
Qt::ItemFlags ObjectTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;  // Для пустого индекса — ничего нельзя

    // Можно выбирать и перетаскивать
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

// Добавить объект в корень дерева
QModelIndex ObjectTreeModel::addObject(Category category, const QString &name)
{
    if (!ObjectCategories::isValid(category))
        return QModelIndex();

    TreeNode *categoryNode = m_categories[category];
    TreeNode *newNode = new TreeNode(name, categoryNode);

    // Устанавливаем свойства в зависимости от категории
    switch (category) {
    case ObjectCategories::ConfigRack:
        newNode->setProperty("Тип", "Корзина");
        newNode->setProperty("Количество слотов", "16");
        newNode->setProperty("Протокол", "Modbus");
        break;
    case ObjectCategories::TableSignal:
        newNode->setProperty("Тип", "Сигнал");
        newNode->setProperty("Частота, Гц", "50");
        newNode->setProperty("Амплитуда", "220");
        break;
    case ObjectCategories::D_Cadr:
        newNode->setProperty("Тип", "Кадр ВУ");
        newNode->setProperty("Разрешение", "1920x1080");
        newNode->setProperty("FPS", "30");
        break;
    case ObjectCategories::DP:
        newNode->setProperty("Тип", "Дискретный параметр");
        newNode->setProperty("Состояние", "0/1");
        newNode->setProperty("Адрес", "0x0000");
        break;
    case ObjectCategories::CategoryCount:
        break;
    }

    int row = categoryNode->childCount();
    QModelIndex categoryIdx = categoryIndex(category);

    beginInsertRows(categoryIdx, row, row);
    categoryNode->appendChild(newNode);
    endInsertRows();

    return index(row, 0, categoryIdx);
}

// Удалить объект
void ObjectTreeModel::removeObject(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    TreeNode *node = static_cast<TreeNode*>(index.internalPointer());
    TreeNode *parentNode = node->parentNode();
    int row = node->row();

    // СООБЩАЕМ VIEW ОБ УДАЛЕНИИ:
    beginRemoveRows(parent(index), row, row);
    parentNode->removeChild(row);
    endRemoveRows();
}

// Вспомогательный метод — получить узел по индексу
TreeNode *ObjectTreeModel::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
        return static_cast<TreeNode*>(index.internalPointer());
    return nullptr;
}

ObjectTreeModel::Category ObjectTreeModel::indexToCategory(const QModelIndex &index) const
{
    if (!index.isValid())
        return ObjectCategories::CategoryCount;  // Невалидный индекс

    TreeNode *node = static_cast<TreeNode*>(index.internalPointer());

    // Проверяем, является ли узел одной из категорий
    for (int i = 0; i < m_categories.size(); ++i) {
        if (node == m_categories[i])
            return static_cast<Category>(i);
    }

    return ObjectCategories::CategoryCount;  // Не категория
}

QModelIndex ObjectTreeModel::categoryIndex(Category category) const
{
    if (!ObjectCategories::isValid(category))
        return QModelIndex();

    int row = static_cast<int>(category);
    return createIndex(row, 0, m_categories[row]);
}