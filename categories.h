#ifndef CATEGORIES_H
#define CATEGORIES_H

#include <QObject>
#include <QString>
#include <QIcon>

// Структура для описания категории
struct CategoryInfo {
    QString name;           // Отображаемое имя
    QString iconName;       // Имя иконки (из темы)
    QString typePrefix;     // Префикс типа для свойств
    bool canHaveChildren;   // Можно ли добавлять объекты в эту категорию
};

// Класс-реестр категорий
class ObjectCategories
{
    Q_GADGET  // Позволяет использовать Q_PROPERTY и Q_ENUM без QObject

public:
    // Сами категории
    enum Category {
        ConfigRack = 0, // Конфигурация корзин
        TableSignal,    // Таблица сигналов
        D_Cadr,         // Кадры для ВУ
        DP,             // Дискретный параметр

        // Всегда последним — автоматический счётчик
        CategoryCount
    };
    Q_ENUM(Category)

    // Получить информацию о категории
    static CategoryInfo info(Category category);

    // Получить название категории
    static QString name(Category category);

    // Получить иконку категории
    static QIcon icon(Category category);

    // Получить все категории списком
    static QList<Category> allCategories();

    // Проверить валидность категории
    static bool isValid(int categoryId);

private:
    // Таблица с данными всех категорий
    static const CategoryInfo s_categoryData[];
};

#endif // CATEGORIES_H