#ifndef URLLINEEDIT_H
#define URLLINEEDIT_H

#include <QtCore/QUrl>
#include <QtGui/QWidget>
#include <QtGui/QStyleOptionFrame>
#include <QWebView>
#include <QWebFrame>
#include <QAbstractButton>

QT_BEGIN_NAMESPACE
class QLineEdit;
QT_END_NAMESPACE

/*
    Clear button on the right hand side of the search widget.
    Hidden by default
    "A circle with an X in it"
 */
class ClearButton : public QAbstractButton
{
    Q_OBJECT

public:
    ClearButton(QWidget *parent = 0);
    void paintEvent(QPaintEvent *event);

public slots:
    void textChanged(const QString &text);
};

class ClearButton;
class ExLineEdit : public QWidget
{
    Q_OBJECT

public:
    ExLineEdit(QWidget *parent = 0);

    inline QLineEdit *lineEdit() const { return m_lineEdit; }

    void setLeftWidget(QWidget *widget);
    QWidget *leftWidget() const;

    QSize sizeHint() const;

    QVariant inputMethodQuery(Qt::InputMethodQuery property) const;
protected:
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void inputMethodEvent(QInputMethodEvent *e);
    bool event(QEvent *event);

protected:
    void updateGeometries();
    void initStyleOption(QStyleOptionFrameV2 *option) const;

    QWidget *m_leftWidget;
    QLineEdit *m_lineEdit;
    ClearButton *m_clearButton;
};

class UrlIconLabel;
class UrlLineEdit : public ExLineEdit
{
    Q_OBJECT

public:
    UrlLineEdit(QWidget *parent = 0);
    void setWebView(QWebView *webView);

protected:
    void paintEvent(QPaintEvent *event);
    void focusOutEvent(QFocusEvent *event);

private slots:
    void webViewUrlChanged(const QUrl &url);
    void webViewIconChanged();
    void progressChanged(int value);
    void returnPressed();

private:
    QLinearGradient generateGradient(const QColor &color) const;
    QWebView *m_webView;
    UrlIconLabel *m_iconLabel;
    QColor m_defaultBaseColor;
    int progress;

};


#endif // URLLINEEDIT_H

