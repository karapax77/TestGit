#include "widget.h"
#include "ui_widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("Домешнее задание №1");

    // TableModel = new QStandardItemModel(0, 11);
    // TableModel->setHeaderData(0, Qt::Horizontal, "Время, с");
    // TableModel->setHeaderData(1, Qt::Horizontal, "x, м");
    // TableModel->setHeaderData(2, Qt::Horizontal, "y, м");
    // TableModel->setHeaderData(3, Qt::Horizontal, "Vx, м/с");
    // TableModel->setHeaderData(4, Qt::Horizontal, "Vy, м/с");
    // TableModel->setHeaderData(5, Qt::Horizontal, "r, м");
    // TableModel->setHeaderData(6, Qt::Horizontal, "V, м/с");
    // TableModel->setHeaderData(7, Qt::Horizontal, "U, Дж");
    // TableModel->setHeaderData(8, Qt::Horizontal, "K, Дж");
    // TableModel->setHeaderData(9, Qt::Horizontal, "E, Дж");
    // TableModel->setHeaderData(10, Qt::Horizontal, "L, кг*м^2/c");
    // ui->Table->setModel(TableModel);
    // ui->Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ResultModel = new CalculationResultModel;
    ui->Table->setModel(ResultModel);
    ui->Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    GraphicOfTrajectoryCustomPlot = new QCustomPlot(this);

    // QHBoxLayout *LayoutOfInitDataAndTable = new QHBoxLayout(ui->groupBox);
    // QHBoxLayout *LayoutOfGraphics = new QHBoxLayout(ui->groupBox);
    // QVBoxLayout *mainLayout = new QVBoxLayout(ui->groupBox);

    // LayoutOfInitDataAndTable->addLayout(ui->LayoutOfInitData);
    // LayoutOfInitDataAndTable->addWidget(ui->GroupBoxResults);

    // LayoutOfGraphics->addWidget(ui->GroupBoxGraphics);

    // mainLayout->addLayout(LayoutOfInitDataAndTable);
    // mainLayout->addLayout(LayoutOfGraphics);

    QHBoxLayout *GraphicsBoxLayout = new QHBoxLayout(ui->GroupBoxGraphics);
    //
    ui->GroupBoxGraphics->layout()->addWidget(GraphicOfTrajectoryCustomPlot);
    GraphicsBoxLayout->addWidget(GraphicOfTrajectoryCustomPlot);



    connect(ui->ButtonStart, &QPushButton::clicked, this, &Widget::Simulation);
    connect(&futureWatcher, &QFutureWatcher<CalculationResult>::finished, this, &Widget::ShowNewCalculatedResult);
    connect(&futureWatcher, &QFutureWatcher<CalculationResult>::finished, this, &Widget::AddNewResultIntoList);
    connect(ui->ButtonExit, &QPushButton::clicked, this, &Widget::close);
    connect(ui->ButtonActivateResult, &QPushButton::clicked, this, &Widget::ActivateResult);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::Calculation(qreal &t, qreal &x, qreal &y, qreal &Vx, qreal &Vy, qreal &ax, qreal &ay, qreal &r, qreal &V, qreal &U, qreal &K, qreal &E, qreal &L, qreal &h, qreal &F, qreal &m)
{
    t = t + h;

    Vx = Vx + h*ax;
    Vy = Vy + h*ay;

    x = x + Vx*h;
    y = y + Vy*h;

    r = pow(pow(x, 2) + pow(y, 2), 0.5);
    ax = (-1)*(F/m)*(x/r);
    ay = (-1)*(F/m)*(y/r);
    V = pow(pow(Vx, 2) + pow(Vy, 2), 0.5);
    U = F*r;
    K = 0.5 * m * pow(V, 2);
    E = U + K;
    L = m*(x*Vy - y*Vx);
}
bool Widget::CenterCollision(qreal &r, qreal &e)
{
    if(r < e)
    {
        QMessageBox::information(this, "Расчёт оставнолен", "Произошло столкновение с центром");
        return true;
    }
    return false;
}
void Widget::InsertDataIntoTable(QList<qreal*> DataResult)//уже не нужно
{
    QList<qreal*>::iterator it = DataResult.begin();
    QList<QStandardItem*> items;
    while(it != DataResult.end())
    {
        items.append(new QStandardItem(QString::number(*(*it), 'f', 3)));
        it++;
    }
    this->TableModel->appendRow(items);
}
void Widget::Simulation()
{
    CalculationResult data;
    data.m = ui->EditWeight->value();
    data.F = ui->EditForce->value();
    data.x0 = ui->EditX0->value();
    data.y0 = ui->EditY0->value();
    data.Vx0 = ui->EditVx0->value();
    data.Vy0 = ui->EditVy0->value();
    data.T = ui->EditTime->value();
    data.h = ui->EditStepOfIntegration->value();

    CalculationResultOneStep FirstStep;
    FirstStep.t = 0;
    FirstStep.x = data.x0;
    FirstStep.y = data.y0;
    FirstStep.Vx = data.Vx0;
    FirstStep.Vy = data.Vy0;
    FirstStep.r = pow(pow(FirstStep.x, 2) + pow(FirstStep.y, 2), 0.5);
    FirstStep.V = pow(pow(FirstStep.Vx, 2) + pow(FirstStep.Vy, 2), 0.5);
    FirstStep.U = data.F*FirstStep.r;
    FirstStep.K = 0.5 * data.m * pow(FirstStep.V, 2);
    FirstStep.E = FirstStep.U + FirstStep.K;
    FirstStep.L = data.m*(FirstStep.x*FirstStep.Vy - FirstStep.y*FirstStep.Vx);

    qreal ax = (-1)*(data.F/data.m)*(FirstStep.x/FirstStep.r);
    qreal ay = (-1)*(data.F/data.m)*(FirstStep.y/FirstStep.r);

    data.m_CalculatedResult.append(FirstStep);

    QFuture<CalculationResult> ThreadOfCalculation = QtConcurrent::run(&Widget::CalculationThread, data, ax, ay);
    //&Widget::CalculationThread(data, ax, ay);
    // QFuture<CalculationResult> ThreadOfCalculation = QtConcurrent::run([this, data, ax, ay]() mutable {
    //     return this->CalculationThread(data, ax, ay);
    //});

    futureWatcher.setFuture(ThreadOfCalculation);
}



CalculationResult Widget::CalculationThread(CalculationResult result, qreal ax, qreal ay)//расчет всей траектории
{
    qreal t = result.m_CalculatedResult.at(0).t;
    qreal T = result.T;
    while(t <= T)
    {
        // result.m_CalculatedResult.append(CalculationEuler(result, ax, ay));
        // t = t + result.h;
        auto currentStep = CalculationEuler(result, ax, ay);
        result.m_CalculatedResult.append(currentStep);
        t = currentStep.t; // Обновить время!
    }
    return result;
}

CalculationResultOneStep Widget::CalculationEuler(CalculationResult result, qreal & ax, qreal & ay)//расчет одного шага Эйлером
{
    CalculationResultOneStep lastStep = result.m_CalculatedResult.last();
    CalculationResultOneStep currentStep;

    currentStep.t = lastStep.t + result.h;

    currentStep.Vx = lastStep.Vx + result.h*ax;
    currentStep.Vy = lastStep.Vy + result.h*ay;

    currentStep.x = lastStep.x + currentStep.Vx*result.h;
    currentStep.y = lastStep.y + currentStep.Vy*result.h;

    currentStep.r = pow(pow(currentStep.x, 2) + pow(currentStep.y, 2), 0.5);

    ax = (-1)*(result.F/result.m)*(currentStep.x/currentStep.r);
    ay = (-1)*(result.F/result.m)*(currentStep.y/currentStep.r);

    currentStep.V = pow(pow(currentStep.Vx, 2) + pow(currentStep.Vy, 2), 0.5);
    currentStep.U = result.F*currentStep.r;
    currentStep.K = 0.5 * result.m * pow(currentStep.V, 2);
    currentStep.E = currentStep.U + currentStep.K;
    currentStep.L = result.m*(currentStep.x*currentStep.Vy - currentStep.y*currentStep.Vx);

    return currentStep;
}

void Widget::ShowNewCalculatedResult()
{
    CalculationResult result = futureWatcher.result();
    ResultModel->SetNewResultOfCalculation(&result);

    TrajectoryGraphic();
}

void Widget::TrajectoryGraphic()
{
    // if(index < 0 || index > ResultModel->CountOfData())
    // {
    //     return;
    // }
    GraphicOfTrajectoryCustomPlot->clearPlottables();

    auto extractX = [](const CalculationResultOneStep &step) { return step.x; };
    auto extractY = [](const CalculationResultOneStep &step) { return step.y; };
    QList<qreal> X = ResultModel->GetDataParametrList(ResultModel->GetCurrentIndex(), extractX);
    QList<qreal> Y = ResultModel->GetDataParametrList(ResultModel->GetCurrentIndex(), extractY);

    QCPCurve *curve = new QCPCurve(GraphicOfTrajectoryCustomPlot->xAxis, GraphicOfTrajectoryCustomPlot->yAxis);
    curve->setData(X, Y);
    curve->setPen(QPen(Qt::red, 3));
    curve->setLineStyle(QCPCurve::lsLine);
    curve->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

    GraphicOfTrajectoryCustomPlot->xAxis->setLabel("x, м");
    GraphicOfTrajectoryCustomPlot->yAxis->setLabel("y, м");

    GraphicOfTrajectoryCustomPlot->rescaleAxes();
    GraphicOfTrajectoryCustomPlot->replot();
}

void Widget::AddNewResultIntoList()
{
    ui->ListWidgetResults->addItem(new QListWidgetItem(QString::number(ResultModel->GetCurrentIndex())));
}

void Widget::ActivateResult()
{
    if(!ui->ListWidgetResults->currentItem())
    {
        return;
    }
    //надо добавить вывод начальных условий в поля ввода
    int index = (ui->ListWidgetResults->currentItem()->text()).toInt();
    ResultModel->SetCurrentReusltIndex(index);
    TrajectoryGraphic();
}

// void Widget::DeleteResultFromList()
// {
//     if(!ui->ListWidgetResults->currentItem())
//     {
//         return;
//     }
//     int index = (ui->ListWidgetResults->currentItem()->text()).toInt();
// }
