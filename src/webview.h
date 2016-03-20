#ifndef WEBVIEW_H
#define WEBVIEW_H

#ifdef USE_WEBKIT
#include <QWebView>
#else
#include <QWebEngineView>
#define QWebView QWebEngineView
#endif

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
