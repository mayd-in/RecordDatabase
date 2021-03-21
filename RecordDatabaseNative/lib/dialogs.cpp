#include "dialogs.h"

#include <QtWidgets>

QRegExpValidator validatorId(QRegExp("^\\d{6,}$"));

NewRecordDialog::NewRecordDialog(QWidget* parent) : QDialog(parent)
{
    auto idLabel = new QLabel(tr("Record ID:"));
    m_idLineEdit = new QLineEdit;
    m_idLineEdit->setValidator(&validatorId);

    auto nameLabel = new QLabel(tr("Name:"));
    m_nameLineEdit = new QLineEdit;

    auto surnameLabel = new QLabel(tr("Surname:"));
    m_surnameLineEdit = new QLineEdit;

    auto formLayout = new QFormLayout;
    formLayout->addRow(idLabel, m_idLineEdit);
    formLayout->addRow(nameLabel, m_nameLineEdit);
    formLayout->addRow(surnameLabel, m_surnameLineEdit);

    auto createRecordButton = new QPushButton(tr("Create Record"));
    createRecordButton->setEnabled(false);

    auto buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(createRecordButton, QDialogButtonBox::AcceptRole);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel);

    auto layout = new QVBoxLayout();
    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);

    // CONNECTIONS
    connect(m_idLineEdit, &QLineEdit::textChanged, this, [this, createRecordButton](){
        createRecordButton->setEnabled(m_idLineEdit->hasAcceptableInput());
    });
    connect(m_nameLineEdit, &QLineEdit::textChanged, this, [this](const QString& text){
        m_nameLineEdit->setText(QLocale().toUpper(text));
    });
    connect(m_surnameLineEdit, &QLineEdit::textChanged, this, [this](const QString& text){
        m_surnameLineEdit->setText(QLocale().toUpper(text));
    });

    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewRecordDialog::beforeAccept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &NewRecordDialog::reject);
}

void NewRecordDialog::open()
{
    m_idLineEdit->clear();
    m_nameLineEdit->clear();
    m_surnameLineEdit->clear();

    QDialog::open();
}

void NewRecordDialog::beforeAccept()
{
    auto recordId = m_idLineEdit->text();
    auto name = m_nameLineEdit->text();
    auto surname = m_surnameLineEdit->text();

    emit recordCreated(recordId, name, surname);
}
