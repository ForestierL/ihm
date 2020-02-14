#include "filterform.h"

FilterForm::FilterForm(MainWindow *parent) : QHBoxLayout()
{
    initContent();
    _parent = parent;

}

void FilterForm::initContent()
{
    initColorComboBox();
    initFeelingComboBox();
    initScoreComboBox();
    initDoFilterButton();

    addWidget(_color);
    addWidget(_feeling);
    addWidget(_score);
    addWidget(_doFilter);

    connect(_doFilter, SIGNAL(clicked()), this, SLOT(emitParamFilter()));
}

void FilterForm::emitParamFilter()
{
    emit runFilter(_color->currentText(), _feeling->currentText(), _score->currentText());
}

void FilterForm::initColorComboBox()
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
    _color->setToolTip("Trier par couleur");

    QSize size;
    size.setWidth(150);
    size.setHeight(25);
    _color->setMinimumSize(size);
    _color->setMaximumSize(size);
}

void FilterForm::initFeelingComboBox()
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
    _color->setToolTip("Trier par émotion");

    QSize size;
    size.setWidth(150);
    size.setHeight(25);
    _feeling->setMinimumSize(size);
    _feeling->setMaximumSize(size);
}

void FilterForm::initScoreComboBox()
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
    _color->setToolTip("Trier par note");

    QSize size;
    size.setWidth(150);
    size.setHeight(25);
    _score->setMinimumSize(size);
    _score->setMaximumSize(size);
}

void FilterForm::initDoFilterButton()
{
    _doFilter = new QHoverSensitiveButton(nullptr, "filter");

    QSize size;
    size.setWidth(25);
    size.setHeight(25);
    _doFilter->setMinimumSize(size);
    _doFilter->setMaximumSize(size);
    _color->setToolTip("Lancer le filtrage");
}
