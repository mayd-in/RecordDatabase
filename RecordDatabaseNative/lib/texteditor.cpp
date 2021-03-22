#include "texteditor.h"

#include <QtCore>
#include <QtWidgets>

TextEditor::TextEditor(QWidget *parent)
    : QTextEdit(parent)
    , actionFontSizeIncrease(new QAction(this))
    , actionFontSizeDecrease(new QAction(this))

    , actionTextBold(new QAction(this))
    , actionTextItalic(new QAction(this))
    , actionTextUnderline(new QAction(this))
    , actionFontColor(new QAction(this))

    , actionAlignLeft(new QAction(this))
    , actionAlignCenter(new QAction(this))
    , actionAlignRight(new QAction(this))
    , actionAlignJustify(new QAction(this))

    , actionIndentMore(new QAction(this))
    , actionIndentLess(new QAction(this))
{
    setupActions();

    updateFontProperties(currentCharFormat());
    updateFormatProperties();

    connect(this, &TextEditor::currentCharFormatChanged, this, &TextEditor::updateFontProperties);
    connect(this, &TextEditor::cursorPositionChanged, this, &TextEditor::updateFormatProperties);
}

void TextEditor::setupActions()
{
    // FONT
    actionFontSizeIncrease->setText(tr("Increase"));
    actionFontSizeIncrease->setIcon(QIcon::fromTheme("format-font-size-more"));
    actionFontSizeIncrease->setPriority(QAction::LowPriority);

    actionFontSizeDecrease->setText(tr("Decrease"));
    actionFontSizeDecrease->setIcon(QIcon::fromTheme("format-font-size-less"));
    actionFontSizeDecrease->setPriority(QAction::LowPriority);

    QFont bold;
    bold.setBold(true);
    actionTextBold->setText(tr("&Bold"));
    actionTextBold->setIcon(QIcon::fromTheme("format-text-bold"));
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setFont(bold);
    actionTextBold->setCheckable(true);
    actionTextBold->setPriority(QAction::LowPriority);

    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setText(tr("&Italic"));
    actionTextItalic->setIcon(QIcon::fromTheme("format-text-italic"));
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    actionTextItalic->setFont(italic);
    actionTextItalic->setCheckable(true);
    actionTextItalic->setPriority(QAction::LowPriority);

    QFont underline;
    underline.setItalic(true);
    actionTextUnderline->setText(tr("&Underline"));
    actionTextUnderline->setIcon(QIcon::fromTheme("format-text-underline"));
    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline->setFont(underline);
    actionTextUnderline->setCheckable(true);
    actionTextUnderline->setPriority(QAction::LowPriority);

    // ALIGNS
    actionAlignLeft->setText(tr("&Left"));
    actionAlignLeft->setIcon(QIcon::fromTheme("format-justify-left"));
    actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);

    actionAlignCenter->setText(tr("C&enter"));
    actionAlignCenter->setIcon(QIcon::fromTheme("format-justify-center"));
    actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);

    actionAlignRight->setText(tr("&Right"));
    actionAlignRight->setIcon(QIcon::fromTheme("format-justify-right"));
    actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);

    actionAlignJustify->setText(tr("&Justify"));
    actionAlignJustify->setIcon(QIcon::fromTheme("format-justify-fill"));
    actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);

    // INDENTS
    actionIndentMore->setText(tr("&Indent"));
    actionIndentMore->setIcon(QIcon::fromTheme("format-indent-more"));
    actionIndentMore->setShortcut(Qt::CTRL + Qt::Key_BracketRight);
    actionIndentMore->setPriority(QAction::LowPriority);

    actionIndentLess->setText(tr("&Unindent"));
    actionIndentLess->setIcon(QIcon::fromTheme("format-indent-less"));
    actionIndentLess->setShortcut(Qt::CTRL + Qt::Key_BracketLeft);
    actionIndentLess->setPriority(QAction::LowPriority);

    // CONNECTIONS
    connect(actionFontSizeIncrease, &QAction::triggered, this, [this]() {modifyFontSize(1);});
    connect(actionFontSizeDecrease, &QAction::triggered, this, [this]() {modifyFontSize(-1);});

    connect(actionTextBold, &QAction::triggered, this, &TextEditor::setTextBold);
    connect(actionTextItalic, &QAction::triggered, this, &TextEditor::setTextItalic);
    connect(actionTextUnderline, &QAction::triggered, this, &TextEditor::setTextUnderline);

    connect(actionFontColor, &QAction::triggered, this, &TextEditor::chooseTextColor);

    connect(actionAlignLeft, &QAction::triggered, this, [this]() {this->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);});
    connect(actionAlignCenter, &QAction::triggered, this, [this]() {this->setAlignment(Qt::AlignHCenter);});
    connect(actionAlignRight, &QAction::triggered, this, [this]() {this->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);});
    connect(actionAlignJustify, &QAction::triggered, this, [this]() {this->setAlignment(Qt::AlignJustify);});

    connect(actionIndentMore, &QAction::triggered, this, [this]() {this->modifyIndentation(1);});
    connect(actionIndentLess, &QAction::triggered, this, [this]() {this->modifyIndentation(-1);});
}

void TextEditor::modifyFontSize(const int amount)
{
    auto fontSize = currentCharFormat().font().pointSize();
    auto sizes = QFontDatabase::standardSizes();

    if (amount < 0) {
        auto lastSize = sizes.at(0);
        for (const auto size : sizes) {
            if (fontSize > size)
                lastSize = size;
            else
                break;
        }
        setFontSize(lastSize);
    }
    else if (amount > 0) {
        for (const auto size : sizes) {
            if (fontSize < size) {
                setFontSize(size);
                break;
            }
        }
    }
}

void TextEditor::setFontFamily(const QString& family)
{
    QTextCharFormat format;
    format.setFontFamily(family);
    mergeFormatOnWordOrSelection(format);
}

void TextEditor::setFontSize(const int size)
{
    if (size <= 0)
        return;
    QTextCharFormat format;
    format.setFontPointSize(size);
    mergeFormatOnWordOrSelection(format);
}

void TextEditor::setTextBold(const bool bold)
{
    QTextCharFormat format;
    format.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(format);
}

void TextEditor::setTextItalic(const bool italic)
{
    QTextCharFormat format;
    format.setFontItalic(italic);
    mergeFormatOnWordOrSelection(format);
}

void TextEditor::setTextUnderline(const bool underline)
{
    QTextCharFormat format;
    format.setFontUnderline(underline);
    mergeFormatOnWordOrSelection(format);
}

void TextEditor::chooseTextColor()
{
    QColor color = QColorDialog::getColor(textColor(), this);
    if (!color.isValid())
        return;
    QTextCharFormat format;
    format.setForeground(color);
    mergeFormatOnWordOrSelection(format);
}

void TextEditor::modifyIndentation(const int amount)
{
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    if (cursor.currentList()) {
        QTextListFormat listFormat = cursor.currentList()->format();
        // See whether the line above is the list we want to move this item into,
        // or whether we need a new list.
        QTextCursor above(cursor);
        above.movePosition(QTextCursor::Up);
        if (above.currentList() && listFormat.indent() + amount == above.currentList()->format().indent()) {
            above.currentList()->add(cursor.block());
        } else {
            listFormat.setIndent(listFormat.indent() + amount);
            cursor.createList(listFormat);
        }
    } else {
        QTextBlockFormat blockFormat = cursor.blockFormat();
        blockFormat.setIndent(blockFormat.indent() + amount);
        cursor.setBlockFormat(blockFormat);
    }
    cursor.endEditBlock();
}

void TextEditor::mergeFormatOnWordOrSelection(const QTextCharFormat& format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    mergeCurrentCharFormat(format);
}

void TextEditor::updateFontProperties(const QTextCharFormat& format)
{
    // Update Fonts
    QFont font = format.font();
    actionTextBold->setChecked(font.bold());
    actionTextItalic->setChecked(font.italic());
    actionTextUnderline->setChecked(font.underline());

    // Update Font Color
    QPixmap fontColorPixmap(16, 16);
    fontColorPixmap.fill(textColor());
    actionFontColor->setIcon(fontColorPixmap);
}

void TextEditor::updateFormatProperties()
{
    // Update Alignment
    Qt::Alignment a = alignment();
    if (a & Qt::AlignLeft)
        actionAlignLeft->setChecked(true);
    else if (a & Qt::AlignHCenter)
        actionAlignCenter->setChecked(true);
    else if (a & Qt::AlignRight)
        actionAlignRight->setChecked(true);
    else if (a & Qt::AlignJustify)
        actionAlignJustify->setChecked(true);
}
