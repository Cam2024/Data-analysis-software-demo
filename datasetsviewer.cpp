#include "datasetsviewer.h"

// Static member initialization
QStandardItemModel* DatasetsViewer::model = new QStandardItemModel();

DatasetsViewer::DatasetsViewer(QWidget* parent)
    : QDialog(parent), tableView(nullptr)
{
    QVBoxLayout* mainVBox = new QVBoxLayout(this);

    // Set headers for the columns
    model->setHorizontalHeaderLabels({ "Dataset Name", "Comment" });

    // Add new model data
    for (int i = 0; i < static_cast<int>(DataSet::datasets.size()); i++)
    {
        if ((i + 1) <= model->rowCount())
            continue;

        // Add data
        QStandardItem* itemName = new QStandardItem(DataSet::datasets[i]->getName());
        QStandardItem* itemComment = new QStandardItem();

        // Make the comment item editable
        itemComment->setFlags(itemComment->flags() | Qt::ItemIsEditable);
        itemName->setFlags(itemName->flags() & ~Qt::ItemIsEditable);

        QList<QStandardItem*> itemo;
        itemo << itemName << itemComment;

        model->appendRow(itemo);
    }

    // Create the table view
    tableView = new QTableView;
    tableView->setModel(model);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::MultiSelection);

    mainVBox->addWidget(tableView);
    setLayout(mainVBox);

    // Create context menu
    ContextMenu->addAction(plotSelectedAction);
    ContextMenu->addAction(plotHistogramAction);
    ContextMenu->addAction(saveAction);
    ContextMenu->addAction(deleteSelectedAction);
    ContextMenu->addAction(refreshAction);

    // Connect actions to slots
    connect(plotSelectedAction, SIGNAL(triggered()), this, SLOT(DatasetsToBePlotted()));
    connect(plotHistogramAction, SIGNAL(triggered()), this, SLOT(PlotHistogram()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(SaveDataset()));
    connect(deleteSelectedAction, SIGNAL(triggered()), this, SLOT(DeleteDatasets()));
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(RefreshDatasets()));
}

DatasetsViewer::~DatasetsViewer()
{
    // For later use, no need to delete
}

void DatasetsViewer::contextMenuEvent(QContextMenuEvent* event)
{
    // Display the context menu where the user clicks
    ContextMenu->popup(event->globalPos());
}

void DatasetsViewer::DatasetsToBePlotted()
{
    // Get selected datasets
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();
    std::vector<int> selectedRows;

    for (const auto& index : selectedIndexes)
    {
        selectedRows.push_back(index.row());
    }

    if (selectedIndexes.size() == 0)
    {
        // Inform the user if no datasets are selected for plotting
        QMessageBox::information(nullptr, "Information", "No datasets selected for Plot.");
        return;
    }

    // Construct the dataset vector based on selected rows
    std::vector<DataSet*> datasets;

    for (int i : selectedRows)
    {
        if (i < static_cast<int>(DataSet::datasets.size()))
        {
            datasets.push_back(DataSet::datasets.at(i));
        }
        else
        {
            qDebug() << "Index out of range!";
        }
    }

    // Emit signal to plot selected datasets
    emit Plot_Signal(datasets);
}

void DatasetsViewer::PlotHistogram()
{
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();

    if (selectedIndexes.size() != 1)
    {
        // Inform the user to select exactly one dataset for plotting a histogram
        QMessageBox::information(nullptr, "Information", "Please select exactly one dataset to plot a histogram.");
        return;
    }

    int selectedRow = selectedIndexes.at(0).row();
    DataSet* selectedDataset = DataSet::datasets.at(selectedRow);

    // Emit signal with the selected dataset for plotting histogram
    emit Plot_Histogram(selectedDataset);
}

void DatasetsViewer::SaveDataset()
{
    // Get selected datasets
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();
    std::vector<int> selectedRows;

    for (const auto& index : selectedIndexes)
    {
        selectedRows.push_back(index.row());
    }

    if (selectedRows.empty())
    {
        // Inform the user if no datasets are selected for saving
        QMessageBox::information(nullptr, "Information", "No datasets selected for saving.");
        return;
    }

    // Prompt the user for a file name and location to save the dataset
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Dataset"), "", tr("Text Files (*.txt);;All Files (*)"));

    if (filename.isEmpty())
    {
        // User canceled the operation or didn't provide a file name
        return;
    }

    QFile file(filename);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream stream(&file);

        // Write selected datasets information
        for (int row : selectedRows)
        {
            if (row < static_cast<int>(DataSet::datasets.size()))
            {
                DataSet* dataset = DataSet::datasets.at(row);

                // Get the size of the dataset
                int datasetSize = dataset->Size();

                // Write dataset values
                for (int i = 0; i < datasetSize; ++i)
                {
                    double* dataPoint = dataset->getPoint(i);

                    // Ensure the format: 0.0 0.0 (space separated)
                    stream << QString::number(dataPoint[0], 'g', 17) << " " << QString::number(dataPoint[1], 'g', 17) << "\n";
                }

                // Separate datasets with an empty line
                stream << "\n";
            }
            else
            {
                qDebug() << "Index out of range!";
            }
        }

        file.close();
    }
    else
    {
        qDebug() << "Failed to open file for writing.";
    }
}

void DatasetsViewer::DeleteDatasets()
{
    QModelIndexList selectedIndexes = tableView->selectionModel()->selectedRows();

    // Check if there is exactly one selected row
    if (selectedIndexes.size() != 1)
    {
        // Inform the user to select exactly one dataset for deletion
        QMessageBox::information(nullptr, "Information", "Please select exactly one dataset to delete.");
        return;
    }

    // Only one row selected, proceed with deletion
    int selectedRow = selectedIndexes[0].row();

    // Check if the row index is valid
    if (selectedRow < 0 || selectedRow >= static_cast<int>(DataSet::datasets.size()))
    {
        qDebug() << "Invalid row index!";
        return;
    }

    // Delete the dataset and remove the corresponding row from the model
    DataSet::datasets.erase(DataSet::datasets.begin() + selectedRow);
    model->removeRow(selectedRow);
}

void DatasetsViewer::RefreshDatasets()
{
    // Clear existing data from the model
    model->clear();

    // Set headers for the columns
    model->setHorizontalHeaderLabels({ "Dataset Name", "Comment" });

    // Add new model data
    for (int i = 0; i < static_cast<int>(DataSet::datasets.size()); i++)
    {
        // Add data
        QStandardItem* itemName = new QStandardItem(DataSet::datasets[i]->getName());
        QStandardItem* itemComment = new QStandardItem();

        // Make the comment item editable
        itemComment->setFlags(itemComment->flags() | Qt::ItemIsEditable);
        itemName->setFlags(itemName->flags() & ~Qt::ItemIsEditable);

        QList<QStandardItem*> itemo;
        itemo << itemName << itemComment;

        model->appendRow(itemo);
    }
}
