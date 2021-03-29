#include <QtTest>
#include <QCoreApplication>
#include <QtWidgets>

#include "dialogs.h"

class TestNewRecordDialog : public QObject
{
    Q_OBJECT

public:
    TestNewRecordDialog();
    ~TestNewRecordDialog();

private slots:
    void initTestCase();  // before first test case
    void cleanupTestCase();  // after last test case

    void init();  // before each case
    void cleanup();  // after each case

    void TestConstruction();
    void TestFields();
    void TestAcceptButton();

private:
    NewRecordDialog newRecordDialog;
};

TestNewRecordDialog::TestNewRecordDialog()
{

}

TestNewRecordDialog::~TestNewRecordDialog()
{

}

void TestNewRecordDialog::initTestCase()
{
    qDebug("Called before everything else.");
}

void TestNewRecordDialog::cleanupTestCase()
{
    qDebug("Called after tests run.");
}

void TestNewRecordDialog::init()
{
    qDebug("Called before each case.");
    newRecordDialog.open();
}

void TestNewRecordDialog::cleanup()
{
    qDebug("Called after each case.");
    newRecordDialog.close();
}

void TestNewRecordDialog::TestConstruction()
{
    QVERIFY2(newRecordDialog.m_idLineEdit, "LineEdit ID not created");
    QVERIFY2(newRecordDialog.m_nameLineEdit, "LineEdit name not created");
    QVERIFY2(newRecordDialog.m_surnameLineEdit, "LineEdit surname not created");

    QVERIFY2(newRecordDialog.m_createRecordButton, "Create record button not created");
}

void TestNewRecordDialog::TestFields()
{
    QString validId("123123");
    QString invalidId1("123");
    QString invalidId2("123asd");

    QTest::keyClicks(newRecordDialog.m_idLineEdit, validId);
    QCOMPARE(newRecordDialog.m_idLineEdit->text(), validId);

    newRecordDialog.m_idLineEdit->clear();
    QTest::keyClicks(newRecordDialog.m_idLineEdit, invalidId1);
    QCOMPARE(newRecordDialog.m_idLineEdit->text(), invalidId1);

    newRecordDialog.m_idLineEdit->clear();
    QTest::keyClicks(newRecordDialog.m_idLineEdit, invalidId2);
    QCOMPARE(newRecordDialog.m_idLineEdit->text(), "123");
}

void TestNewRecordDialog::TestAcceptButton()
{
    QString validId("123123");
    QString invalidId1("123");

    QVERIFY2(!newRecordDialog.m_createRecordButton->isEnabled(), "Button is enabled at beginning");

    QTest::keyClicks(newRecordDialog.m_idLineEdit, invalidId1);
    QVERIFY2(!newRecordDialog.m_createRecordButton->isEnabled(), "Button is enabled after invalid input");

    newRecordDialog.m_idLineEdit->clear();
    QTest::keyClicks(newRecordDialog.m_idLineEdit, validId);
    QVERIFY2(newRecordDialog.m_createRecordButton->isEnabled(), "Button is disabled after valid input");
}

QTEST_MAIN(TestNewRecordDialog)

#include "test_dialogs.moc"
