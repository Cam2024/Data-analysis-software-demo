#ifndef DATASETSVIEWER_H
#define DATASETSVIEWER_H

#include <QWidget>
#include <QDialog>
#include <QStandardItem>
#include <QMenu>
#include <QTableView>
#include <QVBoxLayout>
#include <QApplication>
#include <iostream>
#include <QContextMenuEvent>
#include <vector>
#include <QMessageBox>
#include <QFileDialog>
#include "dataset.h"

class DatasetsViewer : public QDialog
{
	Q_OBJECT
public:
    // DatasetsViewer is the constructor for the DatasetsViewer class
	explicit DatasetsViewer(QWidget *parent = nullptr);
	~DatasetsViewer();
    // Static model for dataset viewer
    static QStandardItemModel *model;

private:
	QTableView *tableView;
	QList<QStandardItem *> items;
    // Context menu and action for plotting selected datasets
	QMenu *ContextMenu=new QMenu(this);
    QAction* plotSelectedAction = new QAction("Plot Selected Datasets", this);
    QAction* plotHistogramAction = new QAction("Plot Histogram", this);
    QAction* saveAction = new QAction("Save Selected Dataset", this);
    QAction* deleteSelectedAction = new QAction("Delete Selected Datasets", this);
    QAction* refreshAction = new QAction("Refresh Datasets", this);
    // contextMenuEvent is overridden to handle the context menu event
	void contextMenuEvent(QContextMenuEvent *event);

public slots:
    // DatasetsToBePlotted is a slot to forward a signal to the parent window for plotting selected datasets
    void DatasetsToBePlotted();
    void PlotHistogram();
    void SaveDataset();
    void DeleteDatasets();
    void RefreshDatasets();

signals:
    // Plot_Signal is emitted to tell the parent window that the user wants selected datasets to be plotted
    void Plot_Signal(std::vector<DataSet *> ptr);
    void Plot_Histogram(DataSet *ptr);
};

#endif // DATASETSVIEWER_H
