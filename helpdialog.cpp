#include "helpdialog.h"
#include "ui_helpdialog.h"

// Constructor
HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpDialog)
{
    ui->setupUi(this);

    // Initialize member variable directly
    helpFilePath = ":/text/help.txt";

    // Read file content
    QStringList helpTopics = readHelpTopicsFromFile(helpFilePath);

    // Add items to ComboBox
    ui->comboBox->addItems(helpTopics);

    // Connect signals and slots using the new syntax
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTextBrowser(int)));

    updateTextBrowser(0);  // Default help topic
}

// Slot to update the TextBrowser based on selected ComboBox index
void HelpDialog::updateTextBrowser(int index)
{
    if (index >= 0 && index < ui->comboBox->count())
    {
        QString selectedTopic = ui->comboBox->itemText(index);

        // Read content for the selected topic
        QString topicContent = readHelpContentFromFile(helpFilePath, selectedTopic);

        // Update TextBrowser
        ui->textBrowser->setPlainText(topicContent);
    }
    else
    {
        showError("Invalid index selected.");
    }
}

// Read help topics from the specified file
QStringList HelpDialog::readHelpTopicsFromFile(const QString &filePath)
{
    QStringList topics;
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            if (line.endsWith(":"))
            {
                topics.append(line.left(line.length() - 1).trimmed());
            }
        }
        file.close();
    } else {
        showError("Error opening help topics file: " + file.errorString());
    }

    return topics;
}

// Read help content for the selected topic from the specified file
QString HelpDialog::readHelpContentFromFile(const QString &filePath, const QString &selectedTopic)
{
    QFile file(filePath);
    QString content;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        bool found = false;
        while (!in.atEnd())
        {
            QString line = in.readLine().trimmed();
            if (found && line.isEmpty())
            {
                break;
            }
            if (found) {
                content.append(line + "\n");
            }
            if (line == selectedTopic + ":")
            {
                found = true;
            }
        }
        file.close();
    }
    else
    {
        // For help file error handling
        showError("Error opening help content file: " + file.errorString());
    }

    return content;
}

// Show error message in the console
void HelpDialog::showError(const QString &message)
{
    qDebug() << "Error: " << message;
}

// Destructor
HelpDialog::~HelpDialog()
{
    delete ui;
}
