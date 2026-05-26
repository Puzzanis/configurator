#include "categories.h"

// Таблица данных категорий
// Порядок ДОЛЖЕН совпадать с порядком в enum!
const CategoryInfo ObjectCategories::s_categoryData[] = {
    // name                  iconName                typePrefix              canHaveChildren
    {"📊 Конфигурация корзин", "folder-documents",    "Категория:ConfigRack", true},
    {"📶 Таблица сигналов",    "network-transmit",    "Категория:Signal",     true},
    {"🎬 Кадры для ВУ",        "camera-video",        "Категория:D_Cadr",     true},
    {"🔢 Дискретный параметр",  "x-office-spreadsheet","Категория:DP",         true}
};

CategoryInfo ObjectCategories::info(Category category)
{
    if (isValid(category))
        return s_categoryData[category];
    return CategoryInfo();
}

QString ObjectCategories::name(Category category)
{
    if (isValid(category))
        return s_categoryData[category].name;
    return QString();
}

QIcon ObjectCategories::icon(Category category)
{
    if (isValid(category))
        return QIcon::fromTheme(s_categoryData[category].iconName);
    return QIcon();
}

QList<ObjectCategories::Category> ObjectCategories::allCategories()
{
    QList<Category> result;
    for (int i = 0; i < CategoryCount; ++i) {
        result.append(static_cast<Category>(i));
    }
    return result;
}

bool ObjectCategories::isValid(int categoryId)
{
    return categoryId >= 0 && categoryId < CategoryCount;
}