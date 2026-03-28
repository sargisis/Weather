#pragma once

#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QPropertyAnimation>
#include <QStringList>
#include "../Interfaces/INavIterface.h"  // Интерфейс CRTP для навигации
#include "Layouts.h"

// NavigationLayout — это лэйаут, отображающий кнопку раскрытия и панель с кнопками.
// Использует CRTP-наследование от INavInterfaceCRTP для статического полиморфизма.
class NavigationLayout : public QVBoxLayout, public INavInterfaceCRTP<NavigationLayout>
{
    Q_OBJECT

public:
    explicit NavigationLayout(QWidget* parent = nullptr);

    // Реализация метода из CRTP-интерфейса — наполняет панель кнопками
    void NavBarSelectedOptionButtonsImpl();

    // Общий стиль для всех кнопок
    QString buttonStyle() const;

signals:
    // Сигналы для взаимодействия с другими частями приложения
    void weatherClicked();
    void languageClicked();
    void notificationsClicked();
    void profileClicked();
    
    // Сигнал выбора избранного города
    void favoriteCityClicked(const QString& city);

public:
    // Метод обновления списка избранных городов в сайдбаре
    void updateFavorites(const QStringList& favorites);

private:
    QVBoxLayout* m_favoritesLayout = nullptr;
};
