#include "graphwindow.h"
#include "ui_graphwindow.h"

// Initializing the static variable
int GraphWindow::FigureCounter = 0;

// Constructor for single dataset plot
GraphWindow::GraphWindow(DataSet *DataSet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphWindow),
    selectedGraphIndex(-1),
    currentColor(Qt::blue),
    currentLineWidth(1),
    currentLineStyle(QCPGraph::lsLine)
{
    ui->setupUi(this);

    // Install an event filter on CustomWidget
    ui->customPlot->installEventFilter(this);

    // Increment the figure counter:
    FigureCounter++;

    // Create the XY graph (setting the parameters of the dataset graph):
    SetGraphSetting(DataSet);

    // Setting the parameters of the figure:
    SetFigureSetting();

    // Add items to comboBox
    ui->comboBoxLineStyle->addItems(QStringList() << "Line" << "Step Left" << "Step Right" << "Step Center" << "Impulse");

    // Setting the title of the figure
    this->setWindowTitle("Figure " + QString::number(FigureCounter));

    // Manually connect signals and slots
    connect(ui->pushButtonColor, SIGNAL(pressed()), this, SLOT(on_pushButtonColor_clicked()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui->comboBoxLineStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxLineStyle_currentIndexChanged(int)));
}

// Constructor for multiple dataset plot
GraphWindow::GraphWindow(std::vector<DataSet *> DataSet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GraphWindow),
    selectedGraphIndex(-1),
    currentColor(Qt::blue),
    currentLineWidth(1),
    currentLineStyle(QCPGraph::lsLine)
{
    ui->setupUi(this);

    // Install an event filter on CustomWidget
    ui->customPlot->installEventFilter(this);

    // Increment the figure counter:
    FigureCounter += DataSet.size();

    // Create the XY graph (setting the parameters of the dataset graph):
    SetGraphVectorSetting(DataSet);

    // Setting the parameters of the figure:
    SetFigureSetting();

    // Add items to comboBox
    ui->comboBoxLineStyle->addItems(QStringList() << "Line" << "Step Left" << "Step Right" << "Step Center" << "Impulse");

    // Setting the title of the figure
    this->setWindowTitle("Figure " + QString::number(FigureCounter));

    // Manually connect signals and slots
    connect(ui->pushButtonColor, SIGNAL(pressed()), this, SLOT(on_pushButtonColor_clicked()));
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(on_horizontalSlider_valueChanged(int)));
    connect(ui->comboBoxLineStyle, SIGNAL(currentIndexChanged(int)), this, SLOT(on_comboBoxLineStyle_currentIndexChanged(int)));
}

// Destructor
GraphWindow::~GraphWindow()
{
    // Called when the window of the figure is closed
    delete ui;
}

// Sets up the curve and plots it
void GraphWindow::SetGraphSetting(DataSet *DataSet)
{
    QCPGraph *currentGraph = ui->customPlot->addGraph();
    currentGraph->addData(DataSet);
    currentGraph->setPen(QPen(currentColor, currentLineWidth));
    currentGraph->setLineStyle(currentLineStyle);
    currentGraph->setName(DataSet->getName());
    currentGraph->rescaleAxes();
}

// Draw multiple datasets on the same graph
void GraphWindow::SetGraphVectorSetting(std::vector<DataSet *> DataSet)
{
    std::vector<QPen> colours = {QPen(Qt::blue), QPen(Qt::red), QPen(Qt::yellow), QPen(Qt::black), QPen(Qt::green), QPen(Qt::magenta), QPen(Qt::cyan)};
    for (unsigned long i = 0; i < DataSet.size(); i++) {
        QCPGraph *currentGraph = ui->customPlot->addGraph();
        currentGraph->addData(DataSet[i]);
        QPen pen;
        pen.setColor(colours[i % colours.size()].color());
        pen.setWidth(static_cast<int>(currentLineWidth));
        currentGraph->setPen(pen);
        currentGraph->setLineStyle(currentLineStyle);
        currentGraph->setName(DataSet[i]->getName());
        currentGraph->rescaleAxes();
    }
}

// Sets up the properties of the figure (that contains the curve)
void GraphWindow::SetFigureSetting()
{
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->xAxis2->setVisible(true);
    ui->customPlot->xAxis2->setTickLabels(false);
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setTickLabels(false);
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    // Make left and bottom axes always transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

// Slot for adjusting curve color
void GraphWindow::on_pushButtonColor_clicked()
{
    QColor newColor = colorDialog.getColor(currentColor, this, "Select Color");
    if (newColor.isValid()) {
        currentColor = newColor;
        if (selectedGraphIndex >= 0) {
            ui->customPlot->graph(selectedGraphIndex)->setPen(QPen(currentColor, currentLineWidth));
            ui->customPlot->replot();
        }
    }
}

// Slot for adjusting curve width
void GraphWindow::on_horizontalSlider_valueChanged(int value)
{
    currentLineWidth = value;
    if (selectedGraphIndex >= 0) {
        ui->customPlot->graph(selectedGraphIndex)->setPen(QPen(currentColor, currentLineWidth));
        ui->customPlot->replot();
    }
}

// Slot for adjusting line style
void GraphWindow::on_comboBoxLineStyle_currentIndexChanged(int index)
{
    QCPGraph::LineStyle newLineStyle = QCPGraph::lsLine;
    switch (index) {
    case 0:
        newLineStyle = QCPGraph::lsLine;
        break;
    case 1:
        newLineStyle = QCPGraph::lsStepLeft;
        break;
    case 2:
        newLineStyle = QCPGraph::lsStepRight;
        break;
    case 3:
        newLineStyle = QCPGraph::lsStepCenter;
        break;
    case 4:
        newLineStyle = QCPGraph::lsImpulse;
        break;
        // Add more cases for additional line styles if needed
    }

    currentLineStyle = newLineStyle;

    if (selectedGraphIndex >= 0) {
        ui->customPlot->graph(selectedGraphIndex)->setLineStyle(currentLineStyle);
        ui->customPlot->replot();
    }
}

// Event filter for handling mouse clicks on the graph
bool GraphWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == ui->customPlot && event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            // Find the graph that was clicked
            QCPAbstractPlottable *plottable = ui->customPlot->plottableAt(mouseEvent->pos());
            if (plottable) {
                QString graphName = plottable->name();
                int graphCount = ui->customPlot->graphCount();
                for (int i = 0; i < graphCount; ++i) {
                    if (ui->customPlot->graph(i)->name() == graphName) {
                        selectedGraphIndex = i;
                        break;
                    }
                }
            }
        }
    }

    // Call the base class implementation to handle other events
    return QDialog::eventFilter(object, event);
}
