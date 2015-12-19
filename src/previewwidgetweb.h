#ifndef PREVIEWWIDGETWEB_H
#define PREVIEWWIDGETWEB_H

#include <src/previewwidget.h>
#include <src/webview.h>

#include <QObject>
#include <QWidget>
#include <QWheelEvent>

class WebView;

class PreviewWidgetWeb : public PreviewWidget
{
    Q_OBJECT

public:
    explicit PreviewWidgetWeb(QWidget *parent = 0);

    void load(const QByteArray &data);
    void setScrollArea(QScrollArea *scrollArea);

public slots:
    void zoomFitBest();
    void zoomFitWidth();
    void zoomFitHeight();
    void wheelEvent(QWheelEvent *e);

protected:

private:
    static const int ZOOM_ORIGINAL_SCALE = 100;     

    void paintEvent(QPaintEvent *);
    void zoomImage();
    void setZoomScale(int new_scale);

    void onWheelZoom(QWheelEvent *e, const int steps);
    void onWheelScroll(QWheelEvent *e);

    WebView *m_webView;

    int m_width;
    int m_height;

private slots:
//    void loadFinished(bool);
};

#endif // PREVIEWWIDGETWEB_H
