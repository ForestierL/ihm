#include "qhoversensitivebutton.h"
#include <QDebug>

QString icon;

QHoverSensitiveButton::QHoverSensitiveButton(QWidget *parent, QString _icon) : QPushButton(parent)
{
    icon = _icon;
    QSettings s("config.ini",QSettings::IniFormat);
    setIcon(QIcon(":/Ressources/" + icon + "-" + s.value("theme").toString() + ".png"));
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
    setFlat(true);
}

void QHoverSensitiveButton::changeIcon(){
    QSettings s("config.ini",QSettings::IniFormat);
    setIcon(QIcon(":/Ressources/" + icon + "-" + s.value("theme").toString() + ".png"));
}

void QHoverSensitiveButton::hoverEnter(QHoverEvent *)
{
    setIcon(QIcon(":/Ressources/" + icon + "-hover.png"));
    repaint();
}

void QHoverSensitiveButton::hoverLeave(QHoverEvent *)
{
    QSettings s("config.ini",QSettings::IniFormat);
    setIcon(QIcon(":/Ressources/" + icon + "-" + s.value("theme").toString() + ".png"));
    repaint();
}

void QHoverSensitiveButton::hoverMove(QHoverEvent *)
{
    repaint();
}

bool QHoverSensitiveButton::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent*>(event));
        return true;
        break;
    case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent*>(event));
        return true;
        break;
    case QEvent::HoverMove:
        hoverMove(static_cast<QHoverEvent*>(event));
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(event);
}
