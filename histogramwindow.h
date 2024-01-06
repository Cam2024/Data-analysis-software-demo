#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include <QDialog>
#include <QMenu>
#include <vector>
#include "qcustomplot.h"
#include "dataset.h"

namespace Ui {
class HistogramWindow;
}

class HistogramWindow : public QDialog
{
    Q_OBJECT

public:
    explicit HistogramWindow(DataSet *DataSet,QWidget *parent = nullptr);
    ~HistogramWindow();
    void SetHistogramSetting(DataSet *DataSet); // Function to control the setting of the curve (dataset represenation in the figure)
    void SetFigureSetting(); // Function to control the setting of the figure itself

private slots:
    void on_pushButton_clicked();
    void on_horizontalSlider_valueChanged(int value);

private:
    Ui::HistogramWindow *ui;
    static int FigureCounter; // Number of Figures created in the app ( defined as static because it is shared among all objects of this class)
    QColor currentColor;
};

#endif // HISTOGRAMWINDOW_H
