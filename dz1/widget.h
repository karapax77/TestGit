#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>//в конечном итоге - не нужно
#include <QStandardItemModel>
#include <QtCore>
#include <QString>
#include <math.h>
#include <windows.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    QStandardItemModel *TableModel;
    struct DataInMoment
    {
        qreal m;
        qreal F;
        qreal t;
        qreal x;
        qreal y;
        qreal Vx;
        qreal Vy;
        qreal r;
        qreal V;
        qreal U;
        qreal K;
        qreal E;
    };
    qreal round2(qreal num);//округление до 2 знаков
public slots:
    void Simulation();
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
