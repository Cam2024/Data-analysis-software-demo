#ifndef HELPDIALOG_H
#define HELPDIALOG_H

/********************************
 *
 *  This class is defined for the "Help" dialog,
 *  an object of this class is the window of the "Help" dialog in the app
 *
**********************************/

#include <QDialog>
#include <QComboBox>
#include <QTextBrowser>
#include <QStringList>
#include <QFile>

namespace Ui {
class HelpDialog;
}

class HelpDialog : public QDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = nullptr);
    ~HelpDialog();

private slots:
    void updateTextBrowser(int index);

private:
    Ui::HelpDialog *ui;
    QString helpFilePath;  // Store the help file path as a member variable
    QStringList readHelpTopicsFromFile(const QString &filePath);
    QString readHelpContentFromFile(const QString &filePath, const QString &selectedTopic);
    void showError(const QString &message); // For error handle
};

#endif // HELPDIALOG_H
