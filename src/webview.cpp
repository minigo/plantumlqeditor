#include "src/webview.h"

#include "src/mainwindow.h"

#include <QDebug>
#include <QWheelEvent>


/**
 * A subclass of QWebView to catch the mouse wheel events.
 *
 * @brief WebView::WebView
 * @param parent
 */
WebView::WebView(QWidget *parent)
    : QWebView(parent)
{
}

/**
 * Set the preview widget for mouse wheel delegation.
 *
 * @brief WebView::setPreviewWidgetWeb
 * @param previewWidgetWeb
 */
void WebView::setPreviewWidgetWeb(PreviewWidgetWeb *previewWidgetWeb)
{
    m_previewWidgetWeb = previewWidgetWeb;
}

/**
 * Delegate all wheel events to the preview widget and QWebView.
 *
 * @brief WebView::wheelEvent
 * @param e
 */
void WebView::wheelEvent(QWheelEvent *e)
{
  e->ignore();
  m_previewWidgetWeb->wheelEvent(e);
  if(e->isAccepted()) return;

  QWebView::wheelEvent(e);
}
