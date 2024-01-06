#include "datasetwindow.h"
#include "ui_datasetwindow.h"

DataSetWindow::DataSetWindow(DataSet* DataSet, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataSetWindow)
{
    // This is called when a dataset is to be plotted in a table displayed in an MDI subwindow
    ui->setupUi(this);

    // Initializing the member data
    DisplayedDataSet = DataSet;

    // Setting up the table
    ui->Table->setColumnCount(2); // 2 columns, one for x and one for y
    QTableWidgetItem* TableItem = nullptr; // Variable used to populate the table

    // Setting the header labels of the columns
    QStringList ColumnHeaders;
    ColumnHeaders << "x" << "y";
    ui->Table->setHorizontalHeaderLabels(ColumnHeaders);

    // Populating the table
    for (int i = 0; i < DataSet->Size(); i++)
    {
        ui->Table->insertRow(i); // Adds a new row to the table

        double* ValuePtr = DataSet->getPoint(i);
        QString x_value = QString::number(*ValuePtr);
        QString y_value = QString::number(*(++ValuePtr));

        TableItem = new QTableWidgetItem(x_value, 0); // Reading x coordinate
        ui->Table->setItem(i, 0, TableItem); // Setting the item in the first column to x

        TableItem = new QTableWidgetItem(y_value, 0); // Reading y coordinate
        ui->Table->setItem(i, 1, TableItem); // Setting the item in the second column to y
    }

    // Setting the title of the window
    this->setWindowTitle("Dataset: " + DataSet->getName());

    // Constructing the context menu so it is ready to be called whenever
    ConstructContextMenu(ContextMenu);

    // Setting icons for actions:
    const QIcon XYPlot_icon = QIcon(":/icons/graph.svg");
    XYPlot->setIcon(XYPlot_icon);
    Histogram->setIcon(XYPlot_icon);

    // Connecting actions to responses via signal-slot mechanism:
    connect(XYPlot, SIGNAL(triggered()), this, SLOT(DataSetToBePlotted()));
    connect(Histogram, SIGNAL(triggered()), this, SLOT(DataSetHistogram()));
}

DataSetWindow::~DataSetWindow()
{
    // This function is called when the window is closed (used for cleanup)
    delete PlotSubMenu;
    delete ContextMenu;
    delete XYPlot;
    delete Histogram;
    delete ui;
}

void DataSetWindow::contextMenuEvent(QContextMenuEvent *event)
{
    // This function is called when the user right-clicks on the dataset window
    ContextMenu->popup(event->globalPos()); // Displaying the menu where the user clicks
}

void DataSetWindow::ConstructContextMenu(QMenu *)
{
    // This function is called in the constructor to build the context menu so that it does not need to be built every time from scratch
    PlotSubMenu->addAction(XYPlot); // Add the action to the menu
    PlotSubMenu->addAction(Histogram); // Add the action to the menu
    ContextMenu->addMenu(PlotSubMenu); // Add the submenus to the main menu
}

void DataSetWindow::DataSetToBePlotted()
{
    emit Plot_XYPlot_SIGNAL(DisplayedDataSet); // Plotting X-Y Graphics Signal to graph window
}

void DataSetWindow::DataSetHistogram()
{
    emit Plot_Histogram_SIGNAL(DisplayedDataSet); // Plotting Histogram Graphic Signal to histogram window
}
