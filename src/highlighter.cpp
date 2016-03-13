#include <QtGui>
#include <QColor>

#include "language.h"

/**
 * Highlighter.
 *
 * TODO:
 * - Multiline comments /' ... '/
 */
class HighLighter:
        public QSyntaxHighlighter
{
//    Q_OBJECT
    private:

        enum CodeType { UNKNOWN = -1, UML=10, SALT=20, DITAA=30, JCCKIT=40, DOT=50, PLANTUML=99 };

        // @see http://doc.qt.io/qt-5/qsyntaxhighlighter.html

        const  QRegExp REGEX_EMPTY      = QRegExp("^\\s*$");
        const  QRegExp REGEX_START_1    = QRegExp("^@start([a-z]+)");
        const  QRegExp REGEX_START_2    = QRegExp("^(salt|digraph|graph|ditaa|jcckit)");

        const  QRegExp REGEX_BEGIN_END  = QRegExp("(@(start|end)(uml|salt|ditaa|jcckit|dot))");
        const  QRegExp REGEX_DECORATION = QRegExp("(title .+|legend.*|endlegend|footer.*|end footer|header.*|end header)");
        const  QRegExp REGEX_KEYWORD    = QRegExp("\\b(object|class|interface|namespace|component|enum|sprite|static|abstract|package|cloud|node|database|folder|frame|participant|actor|as|group)\\b");
        const  QRegExp REGEX_KEYWORD2   = QRegExp("\\b(then|else|elseif|endif|if|while|endwhile|repeat while|repeat|start|stop|true|false|end fork|endfork|fork again|fork|detach|split|split again|end split)\\b");
        const  QRegExp REGEX_COLOR      = QRegExp("#([a-9a-f]{6}|\\w{3,20})");
        const  QRegExp REGEX_SKIN       = QRegExp("skinparam .+");
        const  QRegExp REGEX_OPTION     = QRegExp("(autonumber.*|hide footbox|scale .+)");
        const  QRegExp REGEX_MASK       = QRegExp("(\\\\[nt])");
        const  QRegExp REGEX_NOTE1      = QRegExp("(^note (left|right|top|bottom)( of)?|^note as|^node over|^note)");
        const  QRegExp REGEX_NOTE2      = QRegExp("^end note");
        const  QRegExp REGEX_SEPARATOR  = QRegExp("^[ \t]*([=_.-]{2} *.+ *[=_.-]{2}|[=_.-]{2})");
        const  QRegExp REGEX_COMMENT    = QRegExp("'");
        const  QRegExp REGEX_MLC_ON     = QRegExp("[/][']");
        const  QRegExp REGEX_MLC_OFF    = QRegExp("['][/]");
        const  QRegExp REGEX_HTML_TAG   = QRegExp("(<[^<>]+>|</[^>]+>)");
        //     const  QRegExp REGEX_ARROWS1    = QRegExp("(<[|]--?|[*]--|o--|[.][.]|--|<[|][.][.]|--?>|[.][.]>|[.][.][|]>|<--[*]|->>|-\\\\|\\\\\\\\-||-//|//-)");
        //     const  QRegExp REGEX_ARROWS2    = QRegExp("(-left|-right|-up|-down)");

        /*
    Class01 <|-- Class02
    Class03 *-- Class04
    Class05 o-- Class06
    Class07 .. Class08
    Class09 -- Class10
    Class11 <|.. Class12
    Class13 --> Class14
    Class15 ..> Class16
    Class17 ..|> Class18
    Class19 <--* Class20

    Bob -> Alice
    Bob ->> Alice
    Bob -\ Alice
    Bob \\- Alice
    Bob //-- Alice

    Bob ->o Alice
    Bob o\\-- Alice

    "0..*" - "1..*"
    Text^^   ^   ^^Text
            Arrow

    - short arrow
    ------ very long arror

    */
        QStringList arrows;

        QTextCharFormat format_start_end;
        QTextCharFormat format_decoration;
        QTextCharFormat format_keyword;
        QTextCharFormat format_keyword2;
        QTextCharFormat format_keyword3;
        QTextCharFormat format_color;
        QTextCharFormat format_skin;
        QTextCharFormat format_option;
        QTextCharFormat format_mask;
        QTextCharFormat format_arrow;
        QTextCharFormat format_html;
        QTextCharFormat format_separator;
        QTextCharFormat format_comment;
        QTextCharFormat format_error;
        QTextCharFormat format_line;

        bool multiLineComment;
        CodeType codeType = UNKNOWN;
        QString endtag;

        /**
         * Highlight all words in a line and mark the rest as failure.
         */
        bool highlightWords(const QString &text, const QRegExp &regex, const QTextCharFormat &format) {

            bool highlighted = false;
            int pos = 0;
            while ((pos = regex.indexIn(text, pos)) != -1) {

                if(!regex.cap(1).isEmpty() ) {
                    // highlight submatch only
                    int pos2 = regex.cap(0).indexOf(regex.cap(1));
                    if(pos2 >= -1) {
                        int pos2 = regex.cap(0).indexOf(regex.cap(1));
                        setFormat(pos + pos2, regex.cap(1).length(), format);
                        pos += regex.matchedLength();
                        highlighted = true;
                        continue;
                    }
                }

                // highlight match
                setFormat(pos, regex.matchedLength(), format);
                pos += regex.matchedLength();
                highlighted = true;
            }
            return highlighted;
        }

        /**
         * Highlight one word (match).
         */
        bool highlightWord(const QString &text, const QRegExp regex, const QTextCharFormat &format, const bool markError = true) {
            bool highlighted = false;
            int pos = regex.indexIn(text);
            if(pos > -1) {
                setFormat(pos, text.length() - pos, format);
                highlighted = true;

                // highlight wrong part
                if(markError) {
                    int good = pos+regex.matchedLength();
                    if(good < text.length()) {
                        setFormat(good, text.length() - good, format_error);
                    }
                }
            }
            return highlighted;
        }

        bool highlightRestOfLine(const QString &text, const QRegExp &regex, const QTextCharFormat &format) {
            bool highlighted = false;

            int pos = regex.indexIn(text);
            if(pos > -1) {
                setFormat(pos, text.length() - pos, format);
                highlighted = true;
            }
            return highlighted;
        }

        /**
         * @brief UML syntax highlighting.
         * @param text
         */
        void highlightBlockUml(const QString &text) {

            if(highlightRestOfLine(text, REGEX_MLC_ON, format_comment) == true) {
                multiLineComment=true;
            }

            if(highlightRestOfLine(text, REGEX_MLC_OFF, format_comment) == true) {
                multiLineComment=false;
            }

            if(multiLineComment == true) {
                setFormat(0, text.length(), format_comment);
                return;
            }

            //------------------------------------------------------------

            highlightWords(text, REGEX_KEYWORD, format_keyword);
            highlightWords(text, REGEX_KEYWORD2, format_keyword);

            highlightWords(text, REGEX_NOTE1,   format_keyword);
            highlightWord( text, REGEX_NOTE2,   format_keyword);

            highlightWords(text, REGEX_HTML_TAG, format_html);

            highlightWords(text, REGEX_MASK, format_mask );

            highlightWords(text, REGEX_COLOR, format_color );

            highlightWord(text, REGEX_SEPARATOR, format_separator);

            //    highlightWords(text, REGEX_ARROWS1, format_arrow);
            //    highlightWords(text, REGEX_ARROWS2, format_arrow);
            // 	highlightArrorws(text);

            //------------------------------------------------------------

            highlightWord(text, REGEX_SKIN,      format_skin);
            highlightWord(text, REGEX_OPTION,    format_option);

            highlightWord(text, REGEX_DECORATION, format_decoration);

            highlightRestOfLine(text, REGEX_COMMENT, format_comment);
        }

        /**
         * @brief Salt syntax highlighting.
         * @param text
         */
        void highlightBlockSalt(const QString &text) {

            highlightWords(text, QRegExp("(\\[[^\\]]+\\])"), format_keyword3);                       // Butoon [Some Text]
            highlightWords(text, QRegExp("(\\^[^\\^]+\\^)"), format_keyword3);                       // Droplist ^Some Text^
            highlightWords(text, QRegExp("(\"[^\"]+\")"), format_keyword3);                          // Input Field "Some Text"
            highlightWords(text, QRegExp("(\\[ ?\\]|\\[X\\]|\\( ?\\)|\\(X\\))"), format_keyword3);   // Checkbox, Radio button []  [ ]  [X]  ()  ( )  (X)
            highlightWords(text, QRegExp("(\\.\\.|~~|--|==|\\s\\|\\s)"), format_separator);          // Separator
            highlightWords(text, QRegExp("^\\s*(\\++)"), format_separator);                          // Tree Level
            highlightWords(text, REGEX_HTML_TAG, format_html);
            highlightWords(text, QRegExp("\\{([T/!#+-])"), format_separator);                        // Widget (Tab Strip, Tree) or Grid Lines
        }

        /**
         * @brief Dot syntax highlighting.
         * @param text
         */
        void highlightBlockDot(const QString &text) {
            highlightWords(text, QRegExp("^(digraph|graph)"), format_keyword);
            highlightWords(text, QRegExp("^\\s*(subgraph)"), format_keyword);
            highlightWords(text, QRegExp("(->|--)"), format_arrow); // TODO Fix me!!!
        }

        /**
         * @brief Ditaa syntax highlighting.
         * @param text
         */
        void highlightBlockDitaa(const QString &text) {
            highlightWords(text, QRegExp("[+|:><^v/\\\\*.=-]"), format_line);
            highlightWords(text, QRegExp("[{](c|d|o|s|io|mo|tr)[}]"), format_comment);
            highlightWords(text, QRegExp("c[0-9A-Z]{3}"), format_color);
        }

        /**
         * @brief JCCKit syntax highlighting.
         * @param text
         */
        void highlightBlockJcckit(const QString &text) {
//            highlightWords(text, QRegExp("c[0-9A-Z]{3}"), format_color); // TODO Fix me!!!
        }

    public:
        HighLighter(QTextDocument* document):
            QSyntaxHighlighter(document)
        {
            codeType = UNKNOWN;

            multiLineComment = false;

            format_start_end.setFontWeight(QFont::Bold);
            format_start_end.setForeground(Qt::darkMagenta);

            format_decoration.setFontWeight(QFont::Bold);
            format_decoration.setForeground(Qt::darkMagenta);

            format_keyword.setFontWeight(QFont::Bold);
            format_keyword.setForeground(Qt::blue);

            format_keyword2.setFontWeight(QFont::Bold);
            format_keyword2.setForeground(Qt::darkBlue);

            format_keyword3.setFontWeight(QFont::Bold);
            format_keyword3.setForeground(Qt::darkBlue);
            format_keyword3.setBackground(Qt::lightGray);

            format_color.setForeground(Qt::darkCyan);

            format_skin.setFontWeight(QFont::Normal);
            format_skin.setForeground(Qt::magenta);

            format_option.setFontWeight(QFont::Normal);
            format_option.setForeground(Qt::darkGreen);

            format_mask.setFontWeight(QFont::Normal);
            format_mask.setForeground(QColor(0xaa, 0xaa, 0xaa));

            format_arrow.setForeground(Qt::darkMagenta);
            format_arrow.setFontWeight(QFont::Bold);

            format_html.setFontWeight(QFont::Normal);
            format_html.setForeground(Qt::gray);

            format_comment.setFontItalic(true);
            format_comment.setBackground(QColor(240,240,240));
            format_comment.setForeground(Qt::darkGray);

            format_separator.setForeground(Qt::darkGreen);

            // ditaa line
            format_line.setBackground(QColor(240,240,240));
            format_line.setFontWeight(QFont::Bold);

            // error
            format_error.setFontWeight(QFont::Normal);
            format_error.setFontStrikeOut(true);
            format_error.setFontItalic(true);
            format_error.setForeground(Qt::red);
        }

        ~ HighLighter()
        {}

        void highlightBlock(const QString &text)
        {
            qDebug() << codeType << ':' << text;

            if(text.size() == 0 || text.contains(REGEX_EMPTY)) return;

            if(REGEX_START_1.indexIn(text) > -1) {
                // Start 1: @start<TYPE>

                QString type = REGEX_START_1.cap(1);

                if(type == "uml") {
                    codeType = UML;
                } else if(type == "salt") {
                    codeType = SALT;
                } else if(type == "dot") {
                    codeType = DOT;
                } else if(type == "ditaa") {
                    codeType = DITAA;
                } else if(type == "jcckit") {
                    codeType = JCCKIT;
                } else {
                    codeType = UNKNOWN;
                    return;
                }

                highlightWord(text, REGEX_START_1, format_start_end);

                endtag = "@end" + type;
                return;
            }

            switch(codeType) {
                case UML:

                    if(REGEX_START_2.indexIn(text) > -1) {
                        // Start 2: @startuml + <TYPE>

                        QString type = REGEX_START_2.cap(1);

                        if(type == "salt") {
                            codeType = SALT;
                        } else if(type == "digraph" || type == "graph") {
                            codeType = DOT;
                        } else if(type == "ditaa") {
                            codeType = DITAA;
                        } else if(type == "jcckit") {
                            codeType = JCCKIT;
                        }
                        return;
                    }

                    highlightBlockUml(text);
                    break;
                case SALT:
                    highlightBlockSalt(text);
                    break;
                case DITAA:
                    highlightBlockDitaa(text);
                    break;
                case JCCKIT:
                    highlightBlockJcckit(text);
                    break;
                case DOT:
                    highlightBlockDot(text);
                    break;
                case PLANTUML:
                    // highlightBlockPlantUml(text);
                    break;
                default:
                    return;
            }

            if(codeType != UNKNOWN && highlightWord( text, QRegExp(endtag), format_start_end)) {
                return;
            } else if(highlightWords( text, QRegExp("@start\\w*\\b"), format_error)) {
                return;
            }
        }
};
