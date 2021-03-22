#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>

class TextEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEditor(QWidget *parent = nullptr);

    void modifyFontSize(const int amount);
    void setFontFamily(const QString& family);
    void setFontSize(const int size);
    void setTextBold(const bool bold);
    void setTextItalic(const bool italic);
    void setTextUnderline(const bool underline);
    void chooseTextColor();

    QAction* const actionFontSizeIncrease;
    QAction* const actionFontSizeDecrease;

    QAction* const actionTextBold;
    QAction* const actionTextItalic;
    QAction* const actionTextUnderline;
    QAction* const actionFontColor;

    QAction* const actionAlignLeft;
    QAction* const actionAlignCenter;
    QAction* const actionAlignRight;
    QAction* const actionAlignJustify;

    QAction* const actionIndentMore;
    QAction* const actionIndentLess;

private:
    void setupActions();

    void modifyIndentation(const int amount);
    void mergeFormatOnWordOrSelection(const QTextCharFormat& format);

    // Slots
    void updateFontProperties(const QTextCharFormat& format);
    void updateFormatProperties();
};

#endif // TEXTEDITOR_H
