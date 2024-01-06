#include "parentwindow.h"
#include "ui_parentwindow.h"

ParentWindow::ParentWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::ParentWindow)
{
	ui->setupUi(this);

	setCentralWidget(ui->WindowsManager);
}

ParentWindow::~ParentWindow()
{ // This block is called when the app is closing (to clean up all memory allocation used when tha app started)
    delete ui;
}

void ParentWindow::on_actionLoad_Dataset_triggered()
{ // This block is called when the user triggers Load action to load a file

    // Opening a file dialog
    QString curPath=QDir::currentPath(); // Directs the "open file" to the current directory
    QString FileName=QFileDialog::getOpenFileName(this,tr("Open file"),curPath,tr("Text files (*.txt);;Images (*.png *.xpm *.jpg);;All files(*.*)"));

    if (FileName.isEmpty())
        return; //If no file is selected don't do anything further

    // Creating a new dataset in the app:
    AddedDataSet= new DataSet(FileName);
    if (AddedDataSet->IsDataSetValid) // Making sure the dataset is only dealt with if it was loaded succsessfully
    {
        AllDataSets.push_back(AddedDataSet); // Addding a pointer to the new dataset so that it can be accessed by the rest of the app

        // Creat a subWindow for the loaded DataSet:
        AddedDataSetWindow=new DataSetWindow(AddedDataSet,this);
        subWindow=ui->WindowsManager->addSubWindow(AddedDataSetWindow);
        AddedDataSetWindow->show(); // showing the new dataset window to the user (when it is added for the first time)

        // To enable the ParentWindow to plot the dataset when the user clicks on XYPlot option in the context menu
        // of an already displayed DataSetWidnow
        connect(AddedDataSetWindow,SIGNAL(Plot_XYPlot_SIGNAL(DataSet*)),this,SLOT(GraphWindowToBePlotted(DataSet*)));
        connect(AddedDataSetWindow,SIGNAL(Plot_Histogram_SIGNAL(DataSet*)),this,SLOT(HistogramWindowToBePlotted(DataSet*)));
    }
}

void ParentWindow::on_actionShow_All_Datasets_triggered()
{
    // Creat a subWindow :
    DatasetsViewer* datasetsViewer=new DatasetsViewer(this);
    subWindow=ui->WindowsManager->addSubWindow(datasetsViewer);
    datasetsViewer->show(); // showing the new dataset window to the user (when it is added for the first time)
    connect(datasetsViewer,SIGNAL(Plot_Signal(std::vector<DataSet*>)),this,SLOT(GraphWindowToBePlotted(std::vector<DataSet*>)));
    connect(datasetsViewer,SIGNAL(Plot_Histogram(DataSet*)),this,SLOT(HistogramWindowToBePlotted(DataSet*)));
}

void ParentWindow::on_actionPlot_Datasets_triggered()
{
    // Creat a subWindow :
    DatasetsViewer* datasetsViewer=new DatasetsViewer(this);
    subWindow=ui->WindowsManager->addSubWindow(datasetsViewer);
    datasetsViewer->show(); // showing the new dataset window to the user (when it is added for the first time)
    connect(datasetsViewer,SIGNAL(Plot_Signal(std::vector<DataSet*>)),this,SLOT(GraphWindowToBePlotted(std::vector<DataSet*>)));
    connect(datasetsViewer,SIGNAL(Plot_Histogram(DataSet*)),this,SLOT(HistogramWindowToBePlotted(DataSet*)));
}

void ParentWindow::GraphWindowToBePlotted(DataSet *ptr)
{ // This function is a slot that is called when the parentwindow is to plot a dataset in from a datasetwindow
	GraphWindow *AddedGraphWindow=new GraphWindow(ptr,this);
    subWindow=ui->WindowsManager->addSubWindow(AddedGraphWindow);
	AddedGraphWindow->show();
}

void ParentWindow::GraphWindowToBePlotted(std::vector<DataSet *> DataSet)
{
    GraphWindow *AddedGraphWindow=new GraphWindow(DataSet,this);
    subWindow=ui->WindowsManager->addSubWindow(AddedGraphWindow);
    AddedGraphWindow->show();
}

void ParentWindow::HistogramWindowToBePlotted(DataSet *ptr)
{ // This function is a slot that is called when the parentwindow is to plot a dataset in from a datasetwindow
    HistogramWindow *AddedHistogramWindow=new HistogramWindow(ptr,this);
    subWindow=ui->WindowsManager->addSubWindow(AddedHistogramWindow);
    AddedHistogramWindow->show();
}

void ParentWindow::on_actionFunction_triggered()
{// This function is called when the user clicks on "Function" option under "Analysis" menu
    FunctionDialog* Function_dlg = new FunctionDialog(this, &AllDataSets);
    connect(Function_dlg, SIGNAL(Plot_Function_SIGNAL(DataSet*)), this, SLOT(on_AllDataUpdate(DataSet*)));
    connect(Function_dlg,SIGNAL(Plot_Function_SIGNAL(DataSet*)),this,SLOT(GraphWindowToBePlotted(DataSet*)));
    Function_dlg->exec();
    delete Function_dlg;
}

void ParentWindow::on_AllDataUpdate(DataSet *ResultDataSet)
{
    AllDataSets.push_back(ResultDataSet);
    // Addding a pointer to the new dataset so that it can be accessed by the rest of the app
}

void ParentWindow::on_actionHelp_triggered()
{// This function is called when the user clicks on "Help" option under "Help" menu
    HelpDialog* Help_dlg=new HelpDialog(this);
    Help_dlg->exec();
    delete Help_dlg;
}

void ParentWindow::on_actionAbout_triggered()
{ // This function is called when the user clicks on "About" option under "About" menu
    AboutDialog* About_dlg=new AboutDialog(this);
    About_dlg->exec();
    delete About_dlg;
}
