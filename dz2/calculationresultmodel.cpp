#include "calculationresultmodel.h"


CalculationResultModel::CalculationResultModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}
int CalculationResultModel::rowCount(const QModelIndex &parent) const
{

    if(CurrentResultIndex < 0)
    {
        return 0;
    }
    return m_calcresult[CurrentResultIndex].m_CalculatedResult.size();
}
int CalculationResultModel::columnCount(const QModelIndex &parent) const
{
    // if(CurrentResultIndex < 0)
    // {
    //     return 0;
    // }
    return 11;
}
QVariant CalculationResultModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= m_calcresult[CurrentResultIndex].m_CalculatedResult.size())
    {
        return QVariant();
    }
    const CalculationResultOneStep ResultOfOneStep = m_calcresult[CurrentResultIndex].m_CalculatedResult.at(index.row());
    int column = index.column();
    switch(role)
    {
    case Qt::DisplayRole:
        switch(column)
        {
            case 0: return QString::number(ResultOfOneStep.t, 'f', 3);
            case 1: return QString::number(ResultOfOneStep.x, 'f', 3);
            case 2: return QString::number(ResultOfOneStep.y, 'f', 3);
            case 3: return QString::number(ResultOfOneStep.Vx, 'f', 3);
            case 4: return QString::number(ResultOfOneStep.Vy, 'f', 3);
            case 5: return QString::number(ResultOfOneStep.r, 'f', 3);
            case 6: return QString::number(ResultOfOneStep.V, 'f', 3);
            case 7: return QString::number(ResultOfOneStep.U, 'f', 3);
            case 8: return QString::number(ResultOfOneStep.K, 'f', 3);
            case 9: return QString::number(ResultOfOneStep.E, 'f', 3);
            case 10: return QString::number(ResultOfOneStep.L, 'f', 3);
            default: return QVariant();
        }
    case Qt::EditRole:
        switch(column)
        {
            case 0: return ResultOfOneStep.t;
            case 1: return ResultOfOneStep.x;
            case 2: return ResultOfOneStep.y;
            case 3: return ResultOfOneStep.Vx;
            case 4: return ResultOfOneStep.Vy;
            case 5: return ResultOfOneStep.r;
            case 6: return ResultOfOneStep.V;
            case 7: return ResultOfOneStep.U;
            case 8: return ResultOfOneStep.K;
            case 9: return ResultOfOneStep.E;
            case 10: return ResultOfOneStep.L;
            default: return QVariant();
        }
    case Qt::TextAlignmentRole:
        return Qt::AlignVCenter;
    case Qt::BackgroundRole:
        return QColor(255, 255, 255);
    case Qt::ForegroundRole:
        break;
    }

    return QVariant();
}
QVariant CalculationResultModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0: return "t, м";
            case 1: return "x, м";
            case 2: return "y, м";
            case 3: return "Vx, м/c";
            case 4: return "Vy, м/c";
            case 5: return "r, м";
            case 6: return "V, м/с";
            case 7: return "U, Дж";
            case 8: return "K, Дж";
            case 9: return "E, Дж";
            case 10: return "L, кг*м^2/c";
            default: return QVariant();
        }
    }
    else
    {
        // return section + 1;
        return QVariant();
    }
}

void CalculationResultModel::AddResultOfCalculation(CalculationResult* NewCalculationResult)
{
    NewCalculationResult->resultId = m_calcresult.size() + 1;
    m_calcresult.push_back(*NewCalculationResult);
}
void CalculationResultModel::DeleteResultOfCalculation(int Index)
{

}
// void CalculationResultModel::SetCurrentReusltIndex(int index)//надо поменять так, что индекс был не по массиву, а по ID результата
// {
//     if(index < 0 || index > m_calcresult.size())
//     {
//         return;
//     }

//     beginResetModel();
//     CurrentResultIndex = index;
//     endResetModel();

//     //здесь дожны испускаться сигналы о том, что поменялись данные в таблице
// }

void CalculationResultModel::SetCurrentReusltIndex(int index)
{
    if(index <= 0 || index > m_calcresult[m_calcresult.size() - 1].resultId)
    {
        return;
    }

    beginResetModel();
    CurrentResultID = index;
    CurrentResultIndex = FindResultPlaceWithID(index);

    endResetModel();
}

void CalculationResultModel::SetNewResultOfCalculation(CalculationResult* NewCalculationResult)
{
    AddResultOfCalculation(NewCalculationResult);
    // SetCurrentReusltIndex(m_calcresult.size() - 1);
    SetCurrentReusltIndex(NewCalculationResult->resultId);
}
int CalculationResultModel::GetNumbersOfResults()
{
    return m_calcresult.size();
}

// QList<qreal> CalculationResultModel::GetListOfCurrentX(int index)//можно убирать
// {
//     if(index < 0 || index > CurrentResultIndex)
//     {
//         return nullptr;
//     }
//     QList<qreal> m_x;
//     QList<CalculationResultOneStep> CalculatedResult = m_calcresult[index].m_CalculatedResult;
//     QList<CalculationResultOneStep>::iterator it = CalculatedResult.begin();
//     while(it != CalculatedResult.end())
//     {
//         m_x.append(it->x);
//         it++;
//     }
//     return m_x;
// }

QList<qreal> CalculationResultModel::GetDataParametrList(int index, std::function<qreal(const CalculationResultOneStep&)> extractor)
{
    if (index < 0 || index >= m_calcresult.size()) {
        return QList<qreal>();
    }

    const auto & m_result = m_calcresult[index].m_CalculatedResult;
    QList<qreal> m_parametr;
    m_parametr.reserve(m_result.size());

    for (const auto &step : m_result) {
        m_parametr.append(extractor(step));
    }
    return m_parametr;
}

// QList<qreal> CalculationResultModel::GetListOfCurrentY(int index)//можно убирать
// {
//     if(index < 0 || index > CurrentResultIndex)
//     {
//         return nullptr;
//     }
//     QList<qreal> m_y;
//     QList<CalculationResultOneStep> CalculatedResult = m_calcresult[index].m_CalculatedResult;
//     QList<CalculationResultOneStep>::iterator it = CalculatedResult.begin();
//     while(it != CalculatedResult.end())
//     {
//         m_y.append(it->y);
//         it++;
//     }
//     return m_y;
// }

int CalculationResultModel::GetCurrentIndex()
{
    return CurrentResultIndex;
}

int CalculationResultModel::CountOfData()
{
    return m_calcresult.size();
}

int CalculationResultModel::FindResultPlaceWithID(int id)
{
    for(int i = 0; i < m_calcresult.size(); i++)
    {
        if(m_calcresult[i].resultId == id)
        {
            return i;
        }
    }
    qDebug() << "Такого ID нет";
    return 0;
}

