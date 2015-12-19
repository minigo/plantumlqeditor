#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebView>

#include <src/previewwidgetweb.h>

class QWebView;
class PreviewWidgetWeb;

class WebView : public QWebView
{
    Q_OBJECT
public:

    explicit WebView(QWidget *parent = 0);
    void setPreviewWidgetWeb(PreviewWidgetWeb *previewWidgetWeb);

public slots:

    void wheelEvent(QWheelEvent *e);

protected:

private:

    PreviewWidgetWeb *m_previewWidgetWeb;

private slots:

};

#endif // WEBVIEW_H
