#include "previewwidgetweb.h"

#include "webview.h"

#include <QDebug>
#include <QImage>
#include <QRegExp>
#include <QVBoxLayout>

#define DEBUG_SIZE() qDebug() << "WebView" << m_webView->width() << "x" << m_webView->height();

static const QString HTML_BEGIN = "<html>"
                                  "<head>"
                                  "  <style>"
                                  "    html, body, table, td {"
                                  "      height:  100%;"
                                  "      width:   100%;"
                                  "      margin:  0px;"
                                  "      padding: 0px;"
                                  "      border:  0px;"
                                  "    }"
                                  "    table {"
                                  "      border-collapse: collapse;"
                                  "    }"
                                  "    td {"
                                  "      vertical-align: middle;"
                                  "      text-align:     center;"
//                                  "      border:         1px dashed red;"
                                  "    }"
                                  "  </style>"
                                  "</head>"
                                  "<body>"
                                  "  <table>"
                                  "    <tr>"
                                  "      <td>";
//                                         *** PNG/SVG CONTENT ***
static const QString HTML_END   = "      </td>"
                                  "    </tr>"
                                  "</table>"
                                  "</body>"
                                  "</html>";

/**
 * The preview widget based on a QwebView (SVG with drop shadows).
 *
 * @brief PreviewWidgetWeb::PreviewWidgetWeb
 * @param parent
 */
PreviewWidgetWeb::PreviewWidgetWeb(QWidget *parent)
    : PreviewWidget(parent)
{   
    m_webView = new WebView(this);
    m_webView->setPreviewWidgetWeb(this);

    // make WebView resizable
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_webView);
    setLayout(layout);

//    connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(loadFinished(bool)));
}

// TODO: Remove!
void PreviewWidgetWeb::setScrollArea (QScrollArea *scrollArea)
{
}

void PreviewWidgetWeb::load(const QByteArray &data)
{    
    QByteArray content;

    m_width  = -1;
    m_height = -1;

    if (m_mode == PngMode) {

        content = "<img src=\"data:image/png;base64," + data.toBase64() + "\">";

        QImage image;
        image.loadFromData(data);
        m_width  = image.width();
        m_height = image.height();

    } else if (m_mode == SvgMode) {

        QString svg = data;
        svg.replace(QRegExp("<[?]xml[^>]+>"), "");
        content = svg.toLatin1();

        /*{
            QFile file("/tmp/svg.xml"); // DEBUG
            file.open(QIODevice::WriteOnly);
            file.write(data);
            file.close();
        }*/

        // Examples:
        //  <svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" height="65px" style="width:158px;height:65px;" version="1.1" viewBox="0 0 158 65" width="158px">
        //  <svg width="100pt" height="222pt" viewBox="0.00 0.00 99.92 222.00" xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink">

        QRegExp re("<svg([^>]+)>");
        if (re.indexIn(content) > -1) {

            QString svgTag = re.cap(1);

            re = QRegExp("width=\"([0-9]+)(px|pt)\"|width:([0-9]+)(px|pt)");
            if (re.indexIn(svgTag) > -1) {
                m_width = re.cap(1).toInt();
                if(m_width == 0)
                    m_width = re.cap(3).toInt();

                if(re.cap(2) == "pt" || re.cap(4) == "pt")
                    m_width *= 1.333;
            }

            re = QRegExp("height=\"([0-9]+)(px|pt)\"|height:([0-9]+)(px|pt)");
            if (re.indexIn(svgTag) > -1) {
                m_height = re.cap(1).toInt();

                if(m_height == 0)
                    m_height = re.cap(3).toInt();

                if(re.cap(2) == "pt" || re.cap(4) == "pt")
                    m_height *= 1.333;
            }

            if(m_width == 0 && m_height == 0) {
                // not found -> search viewBox

                QRegExp re("viewBox=\"[0-9.]+ [0-9.]+ ([0-9.]+) ([0-9.]+)\"");
                if (re.indexIn(svgTag) > -1) {
                    m_width  = (int)re.cap(1).toFloat();
                    m_height = (int)re.cap(2).toFloat();
                }

                re = QRegExp ("[0-9]pt");
                if (re.indexIn(svgTag) > -1) {
                    // pt -> px
                    m_width  *= 1.333;
                    m_height *= 1.333;
                }
            }
        }
    }

    qDebug() << "=> Width =" << m_width;
    qDebug() << "=> Height =" << m_height;

    // make the picture a little larger to prevent scrollbars => smaller scale factor
    {
        if(m_width < 150)
            m_width  += 8;
        else
            m_width  += 16;

        if(m_height < 200)
            m_height += 12;
        else
            m_height += 16;
    }

    m_webView->setHtml(HTML_BEGIN + content + HTML_END, QUrl("/"));
    zoomImage();
    update();
}

// TODO: Remove!
/*
void PreviewWidgetWeb::loadFinished(bool done) {
    qDebug() << "loadFinished " << done;

    // see http://stackoverflow.duapp.com/questions/6002621/qwebview-undersampled-svg-rendering

//    QWebElementCollection col = m_webView->page()->mainFrame()->findAllElements("img");
//    foreach (QWebElement el, col) {
        //QImage image(el.geometry().width(), el.geometry().height(), QImage::Format_ARGB32);
        QImage image(m_width, m_height, QImage::Format_ARGB32);
        QPainter painter(&image);
        painter.begin(&image);
        m_webView->render(&painter);
        painter.end();
        qDebug() << "SIZE: " << image.width() << ";" << image.height() << " = " << image.isNull();

        // TODO: TESTING => Works, but the image is too big (2000x1000)!
        image.save("/tmp/IMAGE", "PNG");
//    }
}
*/

void PreviewWidgetWeb::setZoomScale(int zoom_scale)
{
    qDebug() << "zoomScale" << zoom_scale;
    if (m_zoomScale != zoom_scale) {
        m_zoomScale = zoom_scale;
        zoomImage();
        update();
    }
}

//a1e
void PreviewWidgetWeb::zoomFitBest()
{
    zoomOriginal();

    DEBUG_SIZE();

    float zW = (float)m_webView->width()  / m_width;
    float zH = (float)m_webView->height() / m_height;

    int zoomScale = ( zW < zH ? zW : zH ) * 100;
    setZoomScale(zoomScale);
}

//a1e
void PreviewWidgetWeb::zoomFitWidth()
{
    zoomOriginal();

    DEBUG_SIZE();

    float zW = (float)m_webView->width() / m_width;

    int zoomScale = zW * 100;
    setZoomScale(zoomScale);
}

//a1e
void PreviewWidgetWeb::zoomFitHeight()
{
    zoomOriginal();

    DEBUG_SIZE();

    float zH = (float)m_webView->height() / m_height;

    int zoomScale = zH * 100;
    setZoomScale(zoomScale);
}

//a1e
void PreviewWidgetWeb::wheelEvent(QWheelEvent *e)
{
  if( e->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) ) {
    onWheelZoom(e, 8);
  }
  else if( e->modifiers() == (Qt::ControlModifier | Qt::AltModifier) ) {
    onWheelZoom(e, 1);
  }
  else if( e->modifiers() == Qt::ControlModifier) {
    onWheelZoom(e, 4);
  }
}

void PreviewWidgetWeb::onWheelZoom(QWheelEvent *e, const int steps)
{
  if(e->delta() > 0) {
    for(int i = steps; i > 0; --i) zoomIn();
  }
  else {
    for(int i = steps; i > 0; --i) zoomOut();
  }
  e->accept();
}

// TODO: Remove!
void PreviewWidgetWeb::onWheelScroll(QWheelEvent *e)
{
}

// TODO: Remove!
void PreviewWidgetWeb::paintEvent(QPaintEvent *)
{
    //m_webView->resize(this->size());
}

void PreviewWidgetWeb::zoomImage()
{
    qreal zoom = qreal(m_zoomScale) / 100; // 200 -> 2.00 = double size
    m_webView->setZoomFactor(zoom);   
}
