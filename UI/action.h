#ifndef ACTION_H
#define ACTION_H
#include "state.h"

class Action
{
public:
    /* Constructor-destructor */
    Action( QString label, State* s, State* e, QPoint sP, QPoint eP ) : label(label), start(s), end(e), startPos(sP), endPos(eP){};
    ~Action(){qInfo()<<"Action from state: "<<this->start->getLabel()<<" to state: "<<this->end->getLabel()<<"DEAD";}

    void print();

    /* Start state setter-getter */
    void setStart(State* &s){ start = s; };
    State* getStart() const { return start; };

    /* End state setter-getter */
    void setEnd(State* &e){ end = e; };
    State* getEnd() const { return end; };

    /* Start anchor position setter-getter */
    void setStartPoint(QPoint* s){ startPos = *s; };
    QPoint getStartPoint() const { return startPos; };

    /* End anchor position setter-getter */
    void setEndPoint(QPoint *e){ endPos = *e; };
    QPoint getEndPoint() const { return endPos; };

    /* Set start and end anchors (R/L/U/D) */
    void setStartAnchor(QString s){ startAnchor = s; };
    void setEndAnchor(QString s){ endAnchor = s; };

    /* start replaced, end replaced*/
    void replaceStart();
    void replaceEnd();

    /* Action label setter-getter */
    void setLabel(QString l){ label = l; };
    QString getLabel() const { return label; };

    /* Add split to splits vector */
    void addSplit( QPoint split ) { splits.append(split); };

    /* Access splits vector -> please make const */
    void getSplits( QVector<QPoint>::iterator &begin, QVector<QPoint>::iterator &end) { begin = splits.begin(); end = splits.end(); };

    uint getSplitsSize(){ return splits.size(); };

private:
    QString label;
    State* start;
    State* end;
    QPoint startPos;
    QPoint endPos;
    QVector <QPoint> splits;
    QString startAnchor;//R/L/U/D
    QString endAnchor;  //R/L/U/D
};

#endif // ACTION_H
