#include "dataset.h"

// Initialising the static variable
int DataSet::DataSetCounter=0;
// Initialize the static member variable
std::vector<DataSet*> DataSet::datasets;

DataSet::DataSet(QString& FileName)
{

   // Reading the data
    QFile file(FileName);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);

        // Step 1: Count the number of rows : Because GSL reuires knowledge of matrix size in advance
        while (!in.atEnd())
        {
          in.readLine();
          NumberOfRows++;
        }

        // Step 2: Allocate a GSL matrix to save the data for further processing
        Matrix=gsl_matrix_alloc(NumberOfRows,2);


        // Step 3: Storing te data from the file into the matrix
        in.seek(0);
        QSet<double> uniqueXValues;  // Use a set to keep track of unique x values
        for (int i=0;i<NumberOfRows;i++)
        {

            QString x, y;
            in >> x >> y; // This line assumes that the x and the y of the data are seperated by a tab or a comma


            double x_double,y_double;
            // This conversion makes sure the flag for te validy of the dataet is updated if
            // the app couldn't load it because it is has a non-number for example
            x_double=x.toDouble(&IsDataSetValid);
            y_double=y.toDouble(&IsDataSetValid);

            // Check if the x value already exists in the set
            if (uniqueXValues.contains(x_double))
            {
                IsDataSetValid = false;  // Set validity flag to false
                // Display error message
                QMessageBox ErrormsgBox;
                ErrormsgBox.setWindowTitle("Error");
                ErrormsgBox.setText("Error");
                ErrormsgBox.setInformativeText("The app encountered duplicate x values in the dataset." + QString::number(x_double));
                ErrormsgBox.setIcon(QMessageBox::Critical);
                ErrormsgBox.exec();

                // Free allocated memory because reading the file failed
                gsl_matrix_free(Matrix);
                break;  // Stop reading the rest of the data
            }

            uniqueXValues.insert(x_double);  // Add the x value to the set

            if (IsDataSetValid)
            {
              gsl_matrix_set(Matrix,i,0, x_double); // First column is x coordinate
              gsl_matrix_set(Matrix,i,1, y_double); //Second column is the y coordinate

            }

            else // When the function fails to read:
            {
              // Displaye error message
              QMessageBox ErrormsgBox;
              ErrormsgBox.setWindowTitle("Error");
              // ErrormsgBox.setWindowIcon(QIcon(":/icons/errorSymbol.svg"));
              ErrormsgBox.setText("Error");
              ErrormsgBox.setInformativeText("The app encountered a non-numeric character in the dataset.");
              ErrormsgBox.setIcon(QMessageBox::Critical);
              ErrormsgBox.exec();

              // Free allocated memeory because reading the file failed
              gsl_matrix_free(Matrix);


              // Stop reading the rest
              break;
            }
        }
    }


    if (IsDataSetValid) // Only increment the number of datsets if loading is succesfull
    {
    // Giving a deafult name to the dataset (D1 for first dataset, D2 for second dataset, etc):
     DataSetCounter++;
     DataSetName="D"+QString::number(DataSetCounter);
     datasets.push_back(this);
    }

}

DataSet::DataSet(QVector<double> x, QVector<double> y)
{
    // Set the number of rows in the dataset to the size of the input vectors
    NumberOfRows = x.size();
    // Allocate memory for a 2-column GSL matrix to store x and y values
    Matrix=gsl_matrix_alloc(NumberOfRows,2);
    // Populate the GSL matrix with x and y values from the input vectors
    for (int i=0;i<NumberOfRows;i++) {
        gsl_matrix_set(Matrix,i,0, x[i]);
        gsl_matrix_set(Matrix,i,1, y[i]);
    }
    // Check if the dataset is valid before incrementing the counter and adding it to the list of datasets
    if (IsDataSetValid)
    {
        // Giving a deafult name to the dataset (D1 for first dataset, D2 for second dataset, etc):
        DataSetCounter++;
        DataSetName="D"+QString::number(DataSetCounter);
        datasets.push_back(this);
    }
}

int DataSet::Size()
{ // Function to return the size of the dataset (number of rows)
    return NumberOfRows;
}

QString DataSet::getName()
{// Function to return the name of the dataset (number of rows)
    return DataSetName;
}

double *DataSet::getPoint(int i)
{ // Function to return the ith datapoint
    DataPoint[0]= gsl_matrix_get(Matrix,i,0); // x-coordinate
    DataPoint[1]= gsl_matrix_get(Matrix,i,1); // y-coordinate

    return DataPoint;
}
