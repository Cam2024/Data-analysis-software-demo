#ifndef FUNCTIONDIALOG_H
#define FUNCTIONDIALOG_H

#include <QDialog>
#include <QVector>
#include "atmsp.h"
#include "dataset.h"

namespace Ui {
class FunctionDialog;
}

class FunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FunctionDialog(QWidget *parent = nullptr, QList<DataSet*> *AllDataSets = nullptr);
    ~FunctionDialog();

private:
    Ui::FunctionDialog *ui;
    void on_okButtonClicked(QString function);
    QList<DataSet*> *AllDataSets = nullptr;
    static QRegularExpression regex;
    static QRegularExpression regex1;

public slots:
    void FunctionToBePlotted(DataSet *ds);

private slots:
    void on_okButtonClicked();

signals:
    void Plot_Function_SIGNAL(DataSet *ptr);
};

#endif // FUNCTIONDIALOG_H
