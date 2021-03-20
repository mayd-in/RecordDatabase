from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *


class TextEditor(QTextEdit):
    def __init__(self, parent):
        super().__init__(parent)

        self.actionFontSizeIncrease = QAction(self.tr("Increase"), self)
        self.actionFontSizeDecrease = QAction(self.tr("Decrease"), self)

        self.actionTextBold = QAction(self.tr("&Bold"), self)
        self.actionTextItalic = QAction(self.tr("&Italic"), self)
        self.actionTextUnderline = QAction(self.tr("&Underline"), self)

        self.actionFontColor = QAction(self.tr("&Font Color"), self)

        self.actionAlignLeft = QAction(self.tr("&Left"), self)
        self.actionAlignCenter = QAction(self.tr("C&enter"), self)
        self.actionAlignRight = QAction(self.tr("&Right"), self)
        self.actionAlignJustify = QAction(self.tr("&Justify"), self)

        self.actionIndentMore = QAction(self.tr("&Indent"), self)
        self.actionIndentLess = QAction(self.tr("&Unindent"), self)

        self.setupActions()

        self.updateFontProperties(self.currentCharFormat())

        self.currentCharFormatChanged.connect(self.updateFontProperties)
        self.cursorPositionChanged.connect(self.updateFormatProperties)

    def setupActions(self):
        # Font
        self.actionFontSizeIncrease.setIcon(QIcon.fromTheme("format-font-size-more"))
        self.actionFontSizeIncrease.setPriority(QAction.LowPriority)

        self.actionFontSizeDecrease.setIcon(QIcon.fromTheme("format-font-size-less"))
        self.actionFontSizeDecrease.setPriority(QAction.LowPriority)

        boldFont = QFont()
        boldFont.setBold(True)
        self.actionTextBold.setIcon(QIcon.fromTheme("format-text-bold"))
        self.actionTextBold.setShortcut(QKeySequence.Bold)
        self.actionTextBold.setFont(boldFont)
        self.actionTextBold.setCheckable(True)
        self.actionTextBold.setPriority(QAction.LowPriority)

        italicFont = QFont()
        italicFont.setItalic(True)
        self.actionTextItalic.setIcon(QIcon.fromTheme("format-text-italic"))
        self.actionTextItalic.setShortcut(QKeySequence.Italic)
        self.actionTextItalic.setFont(italicFont)
        self.actionTextItalic.setCheckable(True)
        self.actionTextItalic.setPriority(QAction.LowPriority)

        underlineFont = QFont()
        underlineFont.setUnderline(True)
        self.actionTextUnderline.setIcon(QIcon.fromTheme("format-text-underline"))
        self.actionTextUnderline.setShortcut(QKeySequence.Underline)
        self.actionTextUnderline.setFont(underlineFont)
        self.actionTextUnderline.setCheckable(True)
        self.actionTextUnderline.setPriority(QAction.LowPriority)

        # Aligns
        self.actionAlignLeft.setIcon(QIcon.fromTheme("format-justify-left"))
        self.actionAlignLeft.setShortcut(Qt.CTRL + Qt.Key_L)
        self.actionAlignLeft.setCheckable(True)
        self.actionAlignLeft.setPriority(QAction.LowPriority)

        self.actionAlignCenter.setIcon(QIcon.fromTheme("format-justify-center"))
        self.actionAlignCenter.setShortcut(Qt.CTRL + Qt.Key_E)
        self.actionAlignCenter.setCheckable(True)
        self.actionAlignCenter.setPriority(QAction.LowPriority)

        self.actionAlignRight.setIcon(QIcon.fromTheme("format-justify-right"))
        self.actionAlignRight.setShortcut(Qt.CTRL + Qt.Key_R)
        self.actionAlignRight.setCheckable(True)
        self.actionAlignRight.setPriority(QAction.LowPriority)

        self.actionAlignJustify.setIcon(QIcon.fromTheme("format-justify-fill"))
        self.actionAlignJustify.setShortcut(Qt.CTRL + Qt.Key_J)
        self.actionAlignJustify.setCheckable(True)
        self.actionAlignJustify.setPriority(QAction.LowPriority)

        # Indents
        self.actionIndentMore.setIcon(QIcon.fromTheme("format-indent-more"))
        self.actionIndentMore.setShortcut(Qt.CTRL + Qt.Key_BracketRight)
        self.actionIndentMore.setPriority(QAction.LowPriority)

        self.actionIndentLess.setIcon(QIcon.fromTheme("format-indent-less"))
        self.actionIndentLess.setShortcut(Qt.CTRL + Qt.Key_BracketLeft)
        self.actionIndentLess.setPriority(QAction.LowPriority)

        # CONNECTIONS
        self.actionFontSizeIncrease.triggered.connect(lambda: self.modifyFontSize(1))
        self.actionFontSizeDecrease.triggered.connect(lambda: self.modifyFontSize(-1))

        self.actionTextBold.triggered.connect(self.setTextBold)
        self.actionTextItalic.triggered.connect(self.setTextItalic)
        self.actionTextUnderline.triggered.connect(self.setTextUnderline)

        self.actionFontColor.triggered.connect(self.chooseTextColor)

        self.actionAlignLeft.triggered.connect(lambda: self.setAlignment(Qt.AlignLeft | Qt.AlignAbsolute))
        self.actionAlignCenter.triggered.connect(lambda: self.setAlignment(Qt.AlignHCenter))
        self.actionAlignRight.triggered.connect(lambda: self.setAlignment(Qt.AlignRight | Qt.AlignAbsolute))
        self.actionAlignJustify.triggered.connect(lambda: self.setAlignment(Qt.AlignJustify))

        self.actionIndentMore.triggered.connect(lambda: self.modifyIndentation(1))
        self.actionIndentLess.triggered.connect(lambda: self.modifyIndentation(-1))

    def modifyFontSize(self, amount):
        fontSize = self.currentCharFormat().font().pointSize()
        sizes = QFontDatabase.standardSizes()

        if amount < 0:
            lastSize = sizes[0]
            for size in sizes:
                if fontSize > size:
                    lastSize = size
                else:
                    break
            self.setFontSize(lastSize)
        elif amount > 0:
            for size in sizes:
                if fontSize < size:
                    self.setFontSize(size)
                    break

    def setFontFamily(self, family):
        format = QTextCharFormat()
        format.setFontFamily(family)
        self.mergeFormatOnWordOrSelection(format)

    def setFontSize(self, size):
        try:
            size = float(size)
        except:
            return

        if size <= 0:
            return
        format = QTextCharFormat()
        format.setFontPointSize(size)
        self.mergeFormatOnWordOrSelection(format)

    def setTextBold(self, bold):
        format = QTextCharFormat()
        format.setFontWeight(QFont.Bold if bold else QFont.Normal)
        self.mergeFormatOnWordOrSelection(format)

    def setTextItalic(self, italic):
        format = QTextCharFormat()
        format.setFontItalic(italic)
        self.mergeFormatOnWordOrSelection(format)

    def setTextUnderline(self, underline):
        format = QTextCharFormat()
        format.setFontUnderline(underline)
        self.mergeFormatOnWordOrSelection(format)

    def chooseTextColor(self):
        color = QColorDialog.getColor(self.textColor())
        if not color.isValid():
            return
        format = QTextCharFormat()
        format.setForeground(color)
        self.mergeFormatOnWordOrSelection(format)

    def modifyIndentation(self, amount):
        cursor = self.textCursor()
        cursor.beginEditBlock()
        if cursor.currentList():
            listFormat = cursor.currentList().format()
            # See whether the line above is the list we want to move this item into,
            # or whether we need a new list.
            above = QTextCursor(cursor)
            above.movePosition(QTextCursor.Up)
            if above.currentList() and listFormat.indent() + amount == above.currentList().format().indent():
                above.currentList().add(cursor.block())
            else:
                listFormat.setIndent(listFormat.indent() + amount)
                cursor.createList(listFormat)
        else:
            blockFormat = cursor.blockFormat()
            blockFormat.setIndent(blockFormat.indent() + amount)
            cursor.setBlockFormat(blockFormat)
        cursor.endEditBlock()

    def mergeFormatOnWordOrSelection(self, format):
        cursor = self.textCursor()
        if not cursor.hasSelection():
            cursor.select(QTextCursor.WordUnderCursor)
        cursor.mergeCharFormat(format)
        self.mergeCurrentCharFormat(format)

    def updateFontProperties(self, format):
        font = format.font()
        self.actionTextBold.setChecked(font.bold())
        self.actionTextItalic.setChecked(font.italic())
        self.actionTextUnderline.setChecked(font.underline())

        fontColorPixmap = QPixmap(16,16)
        fontColorPixmap.fill(self.textColor())
        self.actionFontColor.setIcon(QIcon(fontColorPixmap))

    def updateFormatProperties(self):
        # Alignment
        alignment = self.alignment()
        if alignment & Qt.AlignmentFlag.AlignLeft:
            self.actionAlignLeft.setChecked(True)
        elif alignment & Qt.AlignmentFlag.AlignHCenter:
            self.actionAlignCenter.setChecked(True)
        elif alignment & Qt.AlignmentFlag.AlignRight:
            self.actionAlignRight.setChecked(True)
        elif alignment & Qt.AlignmentFlag.AlignJustify:
            self.actionAlignJustify.setChecked(True)
