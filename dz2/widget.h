#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>//в конечном итоге - не нужно
#include <QStandardItemModel>
#include <QtCore>
#include <QString>
#include <math.h>
#include <QMessageBox>
#include <QtConcurrent>
#include <QFuture>
#include <QFutureWatcher>
#include "qcustomplot.h"
#include "calculationresultmodel.h"
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
    void ShowNewCalculatedResult();
    void TrajectoryGraphic();
    void AddNewResultIntoList();
    void ActivateResult();
private:
    CalculationResultModel *ResultModel;
    bool CenterCollision(qreal &r, qreal &e);
    void InsertDataIntoTable(QList<qreal*> DataResult);
    void Calculation(qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &, qreal &);
    static CalculationResult CalculationThread(CalculationResult result, qreal ax, qreal  ay);
    static CalculationResultOneStep CalculationEuler(CalculationResult result, qreal & ax, qreal & ay);
    QCustomPlot *GraphicOfTrajectoryCustomPlot = nullptr;
    QFutureWatcher<CalculationResult> futureWatcher;
    Ui::Widget *ui;
};
#endif // WIDGET_H
