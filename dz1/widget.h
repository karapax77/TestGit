#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>//в конечном итоге - не нужно
#include <QStandardItemModel>
#include <QtCore>
#include <QString>
#include <math.h>
#include <windows.h>
#include <QMessageBox>
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
public slots:
    void Simulation();
private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
