#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    TableModel = new QStandardItemModel(0, 10);
    TableModel->setHeaderData(0, Qt::Horizontal, "Время");
    TableModel->setHeaderData(1, Qt::Horizontal, "x");
    TableModel->setHeaderData(2, Qt::Horizontal, "y");
    TableModel->setHeaderData(3, Qt::Horizontal, "Vx");
    TableModel->setHeaderData(4, Qt::Horizontal, "Vy");
    TableModel->setHeaderData(5, Qt::Horizontal, "r");
    TableModel->setHeaderData(6, Qt::Horizontal, "V");
    TableModel->setHeaderData(7, Qt::Horizontal, "U");
    TableModel->setHeaderData(8, Qt::Horizontal, "K");
    TableModel->setHeaderData(9, Qt::Horizontal, "E");
    ui->Table->setModel(TableModel);
    ui->Table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // QList<QStandardItem *> items;
    // items.append(new QStandardItem("1"));
    // items.append(new QStandardItem("2"));
    // TableModel->appendRow(items);

    connect(ui->ButtonStart, SIGNAL(clicked()), this, SLOT(Simulation()));//потом удали
}

Widget::~Widget()
{
    delete ui;
}
qreal Widget::round2(qreal num)
{
    int numint = int(num*100);
    num = numint / 100;
    return num;
}

void Widget::Simulation()
{
    int CountOfInitialData = 8;
    QHash<QString, qreal> data({{"m", (ui->EditWeight->text()).toDouble()},
                      {"F", (ui->EditForce->text()).toDouble()},
                      {"x", (ui->EditX0->text()).toDouble()},
                      {"y", (ui->EditY0->text()).toDouble()},
                      {"Vx", (ui->EditVx0->text()).toDouble()},
                      {"Vy", (ui->EditVy0->text()).toDouble()},
                      {"T", (ui->EditTime->text()).toDouble()},
                      {"h", (ui->EditStepOfIntegration->text()).toDouble()}});

    //qreal r = pow(pow(data["x"], 2) + pow(data["y"], 2), 0.5);
    data.insert("r", pow(pow(data["x"], 2) + pow(data["y"], 2), 0.5));
    //qreal V = pow(pow(data["Vx"], 2) + pow(data["Vy"], 2), 0.5);
    data.insert("V", pow(pow(data["Vx"], 2) + pow(data["Vy"], 2), 0.5));
    //qreal U = (-1)*data["F"]*r;
    data.insert("U", (-1)*data["F"]*data["r"]);
    //qreal K = 0.5 * data["m"] * pow(V, 2);
    data.insert("K", 0.5 * data["m"] * pow(data["V"], 2));
    //qreal E = U + K;
    data.insert("E", data["U"] + data["K"]);
    QList<qreal> DataResult({0, data["x"], data["y"], data["Vx"], data["Vy"], data["r"], data["V"], data["U"], data["K"], data["E"]});
    qDebug() << DataResult;
    data.insert("t", 0);
    data.insert("ax", 0);
    data.insert("ay", 0);
    QList<QStandardItem*> items;
    QList<qreal>::iterator it = DataResult.begin();
    while(it != DataResult.end())
    {
        items.append(new QStandardItem(QString::number(*it, 'g', 3)));
        it++;
    }
    TableModel->appendRow(items);
    //конец внесения в таблицу изначальных параметров
    items.clear();
    Sleep(500);
    while(data["t"] < data["T"])
    {
        //скорости из предыдущего момента для подсчета скоростей(радиус здесь старый)
        data["ax"] = ((-1)*data["F"]/data["m"])*(data["x"]/data["r"]);
        data["ay"] = ((-1)*data["F"]/data["m"])*(data["y"]/data["r"]);
        //характеристики текущего момента
        data["x"] = data["x"] + data["h"]*data["Vx"];
        data["y"] = data["y"] + data["h"]*data["Vy"];
        data["Vx"] = data["Vx"] + data["h"]*data["ax"];
        data["Vy"] = data["Vy"] + data["h"]*data["ay"];
        data["t"] = data["t"] + data["h"];
        //с помощью старого радиуса все посчитано, можно найтии текущий
        data["r"] = pow(pow(data["x"], 2) + pow(data["y"], 2), 0.5);
        //защита от столкновения с центром
        if(data["r"] < pow(10, -6))
        {
            qDebug() << "Столкновение с центром";
            break;
        }
        //подсчет энергии
        data["U"] = (-1)*data["F"]*data["r"];
        data["K"] = 0.5*data["m"]*pow(data["V"], 2);
        data["E"] = data["K"] + data["U"];

        items.append(new QStandardItem(QString::number(data["t"], 'g', 4)));
        items.append(new QStandardItem(QString::number(data["x"], 'g', 3)));
        items.append(new QStandardItem(QString::number(data["y"], 'g', 3)));
        items.append(new QStandardItem(QString::number(data["Vx"], 'g', 3)));
        items.append(new QStandardItem(QString::number(data["Vy"], 'g', 3)));
        items.append(new QStandardItem(QString::number(data["r"], 'g', 3)));
        items.append(new QStandardItem(QString::number(data["V"], 'g', 3)));
        items.append(new QStandardItem(QString::number(data["U"], 'g', 3)));
        items.append(new QStandardItem(QString::number(data["K"], 'g', 3)));
        items.append(new QStandardItem(QString::number(data["E"], 'g', 3)));
        TableModel->appendRow(items);
        //Sleep(500);
        items.clear();
    }
}



