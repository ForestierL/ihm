#include "filtermenu.h"

FilterMenu::FilterMenu(MainWindow *parent) : QHBoxLayout()
{
    initContent();
    _parent = parent;
}

void FilterMenu::initContent()
{
    initColorComboBox();
    initFeelingComboBox();
    initScoreComboBox();
    initDoFilterButton();

    addWidget(_color);
    addWidget(_feeling);
    addWidget(_score);
    addWidget(_doFilter);

    //connect(_doFilter, SIGNAL(clicked()), _parent, SLOT(une_fonction_dans_mainwindow_qui_execute_la_requete_de_selection()));
}

void FilterMenu::initColorComboBox()
{
    _color = new QComboBox();
    _color->addItem("---");
    _color->addItem("Blanc");
    _color->addItem("Bleu");
    _color->addItem("Cyan");
    _color->addItem("Gris");
    _color->addItem("Jaune");
    _color->addItem("Noir");
    _color->addItem("Orange");
    _color->addItem("Rouge");
    _color->addItem("Rose");
    _color->addItem("Vert");
    _color->addItem("Violet");
    _color->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    _color->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
}

void FilterMenu::initFeelingComboBox()
{
    _feeling = new QComboBox();
    _feeling->addItem("---");
    _feeling->addItem("Colère");
    _feeling->addItem("Dégoût");
    _feeling->addItem("Joie");
    _feeling->addItem("Peur");
    _feeling->addItem("Surprise");
    _feeling->addItem("Tristesse");
    _feeling->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    _feeling->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
}

void FilterMenu::initScoreComboBox()
{
    _score = new QComboBox();
    _score->addItem("---");
    _score->addItem("0");
    _score->addItem("1");
    _score->addItem("2");
    _score->addItem("3");
    _score->addItem("4");
    _score->addItem("5");
    _score->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    _score->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
}

void FilterMenu::initDoFilterButton()
{
    _doFilter = new QHoverSensitiveButton(nullptr, "filter");
}
