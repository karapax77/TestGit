#ifndef CALCULATIONRESULTMODEL_H
#define CALCULATIONRESULTMODEL_H

#include <QAbstractTableModel>
#include <QObject>
#include <QtCore>
#include <QColor>
enum class Method{Euler = 0, Runge_Kutta_4};

struct CalculationResultOneStep
{
    qreal t, x, y, Vx, Vy, r,V, U, K, E, L;
};

struct CalculationResult
{
    //номер результатов
    int resultId;

    //начальные условия
    qreal m, F, T, h, x0, y0, Vx0, Vy0;

    //описание метода, типа силы и статуса(закончен, прерван, ошибка) и время расчета(QDataTime)
    QString Method;
        //Method method; //enum

    QString ForceType;
        //TypeOfForceEnum ForceType

    QString status;
    QDateTime dataTime;

    //массив результатов расчетов QList<CalculationResultOneStep>
    QList<CalculationResultOneStep> m_CalculatedResult;
};

class CalculationResultModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CalculationResultModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    void AddResultOfCalculation(CalculationResult* NewCalculationResult);
    void DeleteResultOfCalculation(int Index);
    void SetCurrentReusltIndex(int Index);
    void SetNewResultOfCalculation(CalculationResult* NewCalculationResult);
    //QList<qreal> GetListOfCurrentX(int index);
    //QList<qreal> GetListOfCurrentY(int index);
    QList<qreal> GetDataParametrList(int index, std::function<qreal(const CalculationResultOneStep&)> extractor);
    int GetCurrentIndex();
    int GetNumbersOfResults();
    int CountOfData();
    int FindResultPlaceWithID(int id);
public slots:

private:
    QList<CalculationResult> m_calcresult;
    int CurrentResultIndex = -1;
    int CurrentResultID = 0;
};

#endif // CALCULATIONRESULTMODEL_H
