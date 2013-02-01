#include "searchlineedit.h"

/*
    Search icon on the left hand side of the search widget
    When a menu is set a down arrow appears
 */
SearchButton::SearchButton(QWidget *parent)
  : QAbstractButton(parent),
    m_menu(0)
{
    setObjectName(QLatin1String("SearchButton"));
#ifndef QT_NO_CURSOR
    setCursor(Qt::ArrowCursor);
#endif //QT_NO_CURSOR
    setFocusPolicy(Qt::NoFocus);
}

void SearchButton::mousePressEvent(QMouseEvent *event)
{
    if (m_menu && event->button() == Qt::LeftButton) {
        QWidget *p = parentWidget();
        if (p) {
            QPoint r = p->mapToGlobal(QPoint(0, p->height()));
            m_menu->exec(QPoint(r.x() + height() / 2, r.y()));
        }
        event->accept();
    }
    QAbstractButton::mousePressEvent(event);
}

void SearchButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainterPath myPath;

    int radius = (height() / 5) * 2;
    QRect circle(height() / 3 - 1, height() / 4, radius, radius);
    myPath.addEllipse(circle);

    myPath.arcMoveTo(circle, 300);
    QPointF c = myPath.currentPosition();
    int diff = height() / 7;
    myPath.lineTo(qMin(width() - 2, (int)c.x() + diff), c.y() + diff);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::darkGray, 2));
    painter.drawPath(myPath);

    if (m_menu) {
        QPainterPath dropPath;
        dropPath.arcMoveTo(circle, 320);
        QPointF c = dropPath.currentPosition();
        c = QPointF(c.x() + 3.5, c.y() + 0.5);
        dropPath.moveTo(c);
        dropPath.lineTo(c.x() + 4, c.y());
        dropPath.lineTo(c.x() + 2, c.y() + 2);
        dropPath.closeSubpath();
        painter.setPen(Qt::darkGray);
        painter.setBrush(Qt::darkGray);
        painter.setRenderHint(QPainter::Antialiasing, false);
        painter.drawPath(dropPath);
    }
    painter.end();
}

/*
    SearchLineEdit is an enhanced QLineEdit
    - A Search icon on the left with optional menu
    - When there is no text and doesn't have focus an "inactive text" is displayed
    - When there is text a clear button is displayed on the right hand side
 */
SearchLineEdit::SearchLineEdit(QWidget *parent) : ExLineEdit(parent),
    m_searchButton(new SearchButton(this))
{
    connect(lineEdit(), SIGNAL(textChanged(QString)),
            this, SIGNAL(textChanged(QString)));
    connect(lineEdit(), SIGNAL(returnPressed()),
            this, SIGNAL(returnPressed()));

    setLeftWidget(m_searchButton);
    m_inactiveText = tr("Search");

    QSizePolicy policy = sizePolicy();
    setSizePolicy(QSizePolicy::Preferred, policy.verticalPolicy());
}

void SearchLineEdit::paintEvent(QPaintEvent *event)
{
    if (lineEdit()->text().isEmpty() && !hasFocus() && !m_inactiveText.isEmpty()) {
        ExLineEdit::paintEvent(event);
        QStyleOptionFrameV2 panel;
        initStyleOption(&panel);
        QRect r = style()->subElementRect(QStyle::SE_LineEditContents, &panel, this);
        QFontMetrics fm = fontMetrics();
        int horizontalMargin = lineEdit()->x();
        QRect lineRect(horizontalMargin + r.x(), r.y() + (r.height() - fm.height() + 1) / 2,
                       r.width() - 2 * horizontalMargin, fm.height());
        QPainter painter(this);
        painter.setPen(palette().brush(QPalette::Disabled, QPalette::Text).color());
        painter.drawText(lineRect, Qt::AlignLeft|Qt::AlignVCenter, m_inactiveText);
    } else {
        ExLineEdit::paintEvent(event);
    }
}

void SearchLineEdit::resizeEvent(QResizeEvent *event)
{
    updateGeometries();
    ExLineEdit::resizeEvent(event);
}

void SearchLineEdit::updateGeometries()
{
    int menuHeight = height();
    int menuWidth = menuHeight + 1;
    if (!m_searchButton->m_menu)
        menuWidth = (menuHeight / 5) * 4;
    m_searchButton->resize(QSize(menuWidth, menuHeight));
}

QString SearchLineEdit::inactiveText() const
{
    return m_inactiveText;
}

QString SearchLineEdit::text()
{
    return m_lineEdit->text();
}

void SearchLineEdit::setInactiveText(const QString &text)
{
    m_inactiveText = text;
}

void SearchLineEdit::setMenu(QMenu *menu)
{
    if (m_searchButton->m_menu)
        m_searchButton->m_menu->deleteLater();
    m_searchButton->m_menu = menu;
    updateGeometries();
}

QMenu *SearchLineEdit::menu() const
{
    if (!m_searchButton->m_menu) {
        m_searchButton->m_menu = new QMenu(m_searchButton);
        if (isVisible())
            (const_cast<SearchLineEdit*>(this))->updateGeometries();
    }
    return m_searchButton->m_menu;
}

