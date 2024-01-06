#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H

/********************************
 *
 *  This class is defined to show a graph in a window,
 *  an object of this class is the window in which a graph is plotted
 *
 *  In this class, a figure is the frame of the plot of the curves, its
 *  properties include the axes, the grid, the title, the legend.
 *
 *  In this class, a graph is the curve plotted, its properties include
 *  the width of the line, its style, and its colour.
 *
**********************************/

#include <QDialog>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QColorDialog>
#include <QMenu>
#include <vector>
#include <qcustomplot.h>
#include <iostream>
#include <QDebug>
#include "dataset.h"

namespace Ui {
class GraphWindow;
}

class GraphWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GraphWindow(DataSet *DataSet,QWidget *parent = nullptr);
    // Constructor for displaying a single dataset graph

    explicit GraphWindow(std::vector<DataSet *>DataSet,QWidget *parent = nullptr);
    // Constructor for displaying multiple dataset graphs

    ~GraphWindow();
    void SetGraphSetting(DataSet *DataSet); // Function to control the setting of the curve (dataset representation in the figure)
    // Set the settings for a single dataset graph

    void SetGraphVectorSetting(std::vector<DataSet *>DataSet);
    // Set the settings for multiple dataset graphs

    void SetFigureSetting(); // Function to control the setting of the figure itself
    // Set the settings for the figure (graph window)

private:
    Ui::GraphWindow *ui;
    static int FigureCounter; // Number of Figures created in the app ( defined as static because it is shared among all objects of this class)
    int selectedGraphIndex;
    QColor currentColor;
    int currentLineWidth;
    QCPGraph::LineStyle currentLineStyle;
    // List of actions in the app (defined in the code rather than the UI)
    QColorDialog colorDialog; // Added member variable
protected:
    virtual bool eventFilter(QObject *object, QEvent *event) override;


private slots:
    void on_horizontalSlider_valueChanged(int value);
    void on_comboBoxLineStyle_currentIndexChanged(int index);
    void on_pushButtonColor_clicked();

};

#endif // GRAPHWINDOW_H
