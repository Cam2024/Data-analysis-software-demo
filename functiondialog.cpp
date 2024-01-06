#include "functiondialog.h"
#include "ui_functiondialog.h"

// Regular expression for validating mathematical expressions
QRegularExpression FunctionDialog::regex("^((D\\d+)|(\\d+)|((sin|cos|exp|sqrt|log|ln|abs)\\(.*?\\)))(\\s*([-+*/^]\\s*((D\\d+)|(\\d+)|((sin|cos|exp|sqrt|log|ln|abs)\\(.*?\\)))))*$");

// Regular expression for matching dataset names
QRegularExpression FunctionDialog::regex1("D\\d+");

// Constructor
FunctionDialog::FunctionDialog(QWidget *parent, QList<DataSet*> *DataSets) :
    QDialog(parent),
    ui(new Ui::FunctionDialog)
{
    ui->setupUi(this);
    AllDataSets = DataSets;

    // Connect the accepted signal to the slot
    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(on_okButtonClicked()));
}

// Destructor
FunctionDialog::~FunctionDialog()
{
    delete ui;
}

// Slot for handling OK button click
void FunctionDialog::on_okButtonClicked()
{
    QList<DataSet*> usedDataSets = {};

    // Get the text from the textEdit widget
    QString text = ui->textEdit->toPlainText();

    if (!regex.match(text.simplified()).hasMatch())
    {
        // Show a warning message if the input does not meet the requirements
        QMessageBox::warning(this, "Validation Failed", "Input does not match the required format");
        return;
    }

    QStringList matchedStrings;

    // Match dataset names in the text
    QRegularExpressionMatchIterator matchIterator = regex1.globalMatch(text);

    // Iterate through the matches
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        QString matchedString = match.captured();
        matchedStrings << matchedString;
    }

    for(int i = 0; i < matchedStrings.size(); i++)
    {
        bool dataSetFound = false; // Flag to indicate if a matching dataset is found

        for(int j = 0; j < AllDataSets->count(); j++)
        {
            if (matchedStrings[i].contains(AllDataSets->at(j)->getName()))
            {
                usedDataSets.append(AllDataSets->at(j));
                dataSetFound = true;
                break; // Break out of the inner loop once a match is found
            }
        }

        if (!dataSetFound)
        {
            // Handle the case when a matching dataset is not found, e.g., display a missing dataset warning and return
            QMessageBox::warning(this, "ERROR", QString("Missing dataset: %1").arg(matchedStrings[i]));
            return;
        }
    }

    if(usedDataSets.empty())
    {
        // Handle the case when no dataset
        QMessageBox::warning(this, "ERROR", "Please enter the name of at least one dataset");
        return;
    }

    if(usedDataSets.size() > 1)
    {
        // Check if x-axis data is consistent among selected datasets
        for (int i = 1; i < usedDataSets.size(); i++)
        {
            int rows = usedDataSets.at(0)->Size();

            if(rows != usedDataSets.at(i)->Size())
            {
                QMessageBox::warning(this, "Validation Failed", "Selected datasets have inconsistent x-axis data");
                usedDataSets.clear();
                return;
            }

            for (int j = 0; j < rows; j++)
            {
                double* x1 = usedDataSets.at(0)->getPoint(j); // Assuming getPoint returns DataPoint
                double* x2 = usedDataSets.at(i)->getPoint(j);

                if (x1[0] != x2[0])
                {
                    QMessageBox::warning(this, "Validation Failed", "Selected datasets have inconsistent x-axis data");
                    return;
                }
            }
        }
    }

    QString usedDataSetsString = "";
    for (int i = 0; i < usedDataSets.count(); i++)
    {
        usedDataSetsString.append(usedDataSets.at(i)->getName()).append(",");
    }
    usedDataSetsString.chop(1);

    // Parsing: converting the text expression into a mathematical formula then evaluating it:
    ATMSP<double> ParserObj; // An object to convert the expression string to bytecode (parsing)
    ATMSB<double> ByteCodeObj; // An object to evaluate the codebyte into numbers

    //Converting the text expression into a mathematical formula:
    ParserObj.parse(ByteCodeObj, text.toStdString(), usedDataSetsString.toStdString()); // Telling the parser what is the formula and what the datasets are

    // Computing the result:
    QVector<double> ResultX; // QVector to hold the results
    QVector<double> ResultY; // QVector to hold the results

    for (int i = 0; i < usedDataSets.at(0)->Size(); i++)
    {
        double* xValuePtr = usedDataSets.at(0)->getPoint(i); // Assuming X coordinates are from the first dataset
        double xValue = *xValuePtr;
        ResultX.push_back(xValue);
    }

    for (int i = 0; i < usedDataSets.at(0)->Size(); i++)
    {
        for (int j = 0; j < usedDataSets.count(); j++) {
            double* ValuePtr = usedDataSets.at(j)->getPoint(i);
            double y_value= *(++ValuePtr);
            ByteCodeObj.var[j] = y_value;
        }
        ResultY.push_back(ByteCodeObj.run());
    }

    DataSet *ResultDataSet = new DataSet(ResultX, ResultY);
    FunctionToBePlotted(ResultDataSet);

    // Handle parsing errors (unknown symbols, etc.)
    size_t err;

    if ((err = ParserObj.parse(ByteCodeObj, text.toStdString(), usedDataSetsString.toStdString()))) {
        // Expand error handling as needed
        QString string=QString::fromStdString(ParserObj.errMessage(err));
        qCritical() << string;
    }
}

// Function to emit a signal indicating that the dataset should be plotted
void FunctionDialog::FunctionToBePlotted(DataSet *ds)
{
    // Emit a signal to inform the parent window to plot the dataset when the user chooses XYPlot
    emit Plot_Function_SIGNAL(ds);
}
