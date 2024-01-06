#ifndef DATASETWINDOW_H
#define DATASETWINDOW_H

/********************************
 *
 *  This class is defined to show the dataset in a window,
 *  an object of this class is the window in which a dataset is shown to the user
 *
 *  A dataset should only be visible to the user through a DataSetWindow object
 *
 *
 *
 *
**********************************/


#include <QDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include <QAction>
#include "dataset.h"

namespace Ui {
class DataSetWindow;
}

class DataSetWindow : public QDialog
{
    Q_OBJECT

public:
    // Constructor with dataset and optional parent
    explicit DataSetWindow(DataSet* dataset, QWidget *parent = nullptr);

    // Destructor
    ~DataSetWindow();

    // Override context menu event
    void contextMenuEvent(QContextMenuEvent *event);

    // Construct context menu with specified menu
    void ConstructContextMenu(QMenu *menu);

public slots:
    // Slot for XY plot
    void DataSetToBePlotted();

    // Slot for histogram plot
    void DataSetHistogram();

private:
    Ui::DataSetWindow *ui;

    // Pointer to the dataset displayed in the window
    DataSet *DisplayedDataSet = nullptr;

    // Actions for XY plot and histogram plot
    QAction* XYPlot = new QAction("XY Plot", this);
    QAction* Histogram = new QAction("Histogram Plot", this);

    // Context menu and submenu for plot actions
    QMenu *ContextMenu = new QMenu(this);
    QMenu *PlotSubMenu = new QMenu("Plot");

signals:
    // Signal for XY plot
    void Plot_XYPlot_SIGNAL(DataSet *ptr);

    // Signal for histogram plot
    void Plot_Histogram_SIGNAL(DataSet *ptr);
};

#endif // DATASETWINDOW_H
