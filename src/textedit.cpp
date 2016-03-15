#include <QKeyEvent>
#include <QTextCursor>
#include <QTextBlock>
#include <QPainter>
#include <QTextEdit>
#include <QDebug>

#include "textedit.h"

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(TextEdit *editor) : QWidget(editor)
    {
        this->editor = editor;
    }

    QSize sizeHint() const
    {
        return QSize(editor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event)
    {
        editor->lineNumberAreaPaintEvent(event);
    }

private:
    TextEdit *editor;
};


TextEdit::TextEdit(QWidget *parent) :
    QPlainTextEdit(parent),
    _indentSize(4),
    _indentWithSpace(false),
    _autoIndent(true)
{
    setIndentSize(4);
    setLineWrapMode (QPlainTextEdit::NoWrap);

    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));

    updateLineNumberAreaWidth(0);
}

void TextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

int TextEdit::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 4 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}


void TextEdit::keyPressEvent(QKeyEvent *e)
{
    QTextCursor cursor;

    switch (e->key())
    {
        case Qt::Key_Down:
            if(e->modifiers() == (Qt::ControlModifier | Qt::AltModifier)) {
                // duplicate line

                cursor = textCursor();
                cursor.beginEditBlock();
                cursor.movePosition(QTextCursor::StartOfLine);
                cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
                if(!cursor.hasSelection()) break;
                QString line = cursor.selectedText();
                cursor.insertText(line);
                cursor.insertText("\n");
                cursor.insertText(line);
                cursor.endEditBlock();

                e->accept();
            }
            else if(e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {
                // move line down

                cursor = textCursor();
                cursor.beginEditBlock();
                cursor.movePosition(QTextCursor::StartOfLine);
                cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
                if(!cursor.hasSelection()) break;
                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                QString line = cursor.selectedText();
                cursor.removeSelectedText();
//                cursor.movePosition(QTextCursor::Down);
                cursor.insertText(line);
                cursor.endEditBlock();
                setFocus();

                e->accept();
            }

            break;

        case Qt::Key_Up:
            if(e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier)) {
                // move line up

                cursor = textCursor();
                cursor.beginEditBlock();
                cursor.movePosition(QTextCursor::StartOfLine);
                cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
                if(!cursor.hasSelection()) break;
                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor);
                QString line = cursor.selectedText();
                cursor.removeSelectedText();
                cursor.movePosition(QTextCursor::Up);
                cursor.insertText(line);
                //cursor.movePosition(QTextCursor::Up, QTextCursor::MoveAnchor, 4);
//                cursor.movePosition(QTextCursor::Up);
//                cursor.movePosition(QTextCursor::Up);
//                cursor.movePosition(QTextCursor::Up);
//                cursor.movePosition(QTextCursor::Up);
                cursor.endEditBlock();
                setFocus();

                e->accept();
            }

            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
        {
            QPlainTextEdit::keyPressEvent(e);
            QTextCursor updateCursor = textCursor();

            // Auto-indent
            if (_autoIndent)
            {
                QTextBlock block = updateCursor.block().previous();
                QString    data  = block.text();
                int        pos   = block.length();

                int i;
                if (pos >= data.length())
                    pos = data.length() - 1;

                for (i = pos; i >= 0; --i)
                {
                    if (data.at(i) == '\n')
                    {
                        break;
                    }
                }

                while ((i+1) < data.length() &&
                       data.at(i + 1).isSpace() &&
                       data.at(i + 1) != '\n' &&
                       data.at(i + 1) != '\r')
                {
                    updateCursor.insertText(data.at(i + 1));
                    i++;
                }
            }

            return; //break;
        }

        case Qt::Key_Tab:
        {
            QString indentLine;
            if (_indentWithSpace)
            {
                indentLine = indentLine.leftJustified(_indentSize, QChar(' '));
            }
            else
            {
                indentLine = QChar('\t');
            }

            textCursor().insertText(indentLine);

            break;
        }
    }

    QPlainTextEdit::keyPressEvent(e);
}

void TextEdit::paintEvent(QPaintEvent *e)
{
    // Update tab stops
    QString indentLine = QString().leftJustified(_indentSize, QChar(' '));
    setTabStopWidth(fontMetrics().width(indentLine));

    QPlainTextEdit::paintEvent(e);
}

void TextEdit::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

// a1e
void TextEdit::wheelEvent(QWheelEvent *e)
{
  const int MIN_FONTSIZE = 6;
  const int MAX_FONTSIZE = 80;
  
  int step = 0;
  if( e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) ) {
    step = 8;
  }
  else if( e->modifiers() == (Qt::ControlModifier | Qt::AltModifier) ) {
    step = 1;
  }
  else if( e->modifiers() == Qt::ControlModifier) {
    step = 4;
  }

  if(step > 0) {
    int fontSize = _font.pointSize();
    if(e->delta() > 0) {
      fontSize += step;
      if(fontSize > MAX_FONTSIZE) fontSize = MAX_FONTSIZE;
    }
    else {
      fontSize -= step;
      if(fontSize < MIN_FONTSIZE) fontSize = MIN_FONTSIZE;
    }
    _font.setPointSize(fontSize);
    QPlainTextEdit::setFont(_font);
    e->accept();
  }
  else {
    QPlainTextEdit::wheelEvent(e);
  }
}

void TextEdit::setFont (QFont & font)
{
  _font = font; // capture font
  QPlainTextEdit::setFont(_font);
}

void TextEdit::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void TextEdit::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
    {
        lineNumberArea->scroll(0, dy);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

bool TextEdit::isEmpty() {
    return QPlainTextEdit::toPlainText().isEmpty(); // w/o ditaa fix
}

QString TextEdit::toPlainText() {

    QString text = QPlainTextEdit::toPlainText();

    if(text.contains("ditaa") && text.contains("\\\n")) {
        qDebug() << "fix ditaa \\EOL problem";
        text.replace("\\\n", "\\ \n");
    }

    return text;
}



