#include "qhoversensitivebutton.h"
#include <QDebug>

QString icon;

QHoverSensitiveButton::QHoverSensitiveButton(QWidget *parent, QString _icon) : QPushButton(parent)
{
    icon = _icon;
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

void QHoverSensitiveButton::hoverEnter(QHoverEvent *)
{
    setIcon(QIcon("E:\\Documents\\GitHub\\ihm\\" + icon + "-b.png"));
    repaint();
}

void QHoverSensitiveButton::hoverLeave(QHoverEvent *)
{
    setIcon(QIcon("E:\\Documents\\GitHub\\ihm\\" + icon + "-w.png"));
    repaint();
}

void QHoverSensitiveButton::hoverMove(QHoverEvent *)
{
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
