#ifndef COMPONENT_H
#define COMPONENT_H

#include <QString>
#include <QtDebug>
#include <QSharedDataPointer>
class Component
{
public:
    Component(QString label);
    ~Component();
    void print();
    QString getLabel();
protected:
    QString label;

};

#endif // COMPONENT_H
