#include "osd.h"

OSD::OSD(QWidget *parent) :
    QWidget(parent, Qt::Window |
            Qt::X11BypassWindowManagerHint |
            Qt::FramelessWindowHint),
    m_dragging(false),
    osd_interr (false),
    osd_menu (new QMenu (this)),
    osd_idx (0),
    // 200k
    osd_maxSpeed (200 * 1024)
{
    memset (osd_data, 0, sizeof (osd_data));

    setAcceptDrops(true);
    setFixedSize(40 , 40);

    // show at right corner
    QDesktopWidget *desktop = QApplication::desktop();
    move (desktop->width() - 100, 30);

    show ();
}

OSD::~OSD()
{

}

void OSD::setText(const QString &text)
{
    m_text = text;
}

void OSD::mouseDoubleClickEvent(QMouseEvent *)
{
    emit doubleClicked();
}

void OSD::dragEnterEvent(QDragEnterEvent *e)
{
    emit requestDownload( e->mimeData()->text() );
}

void OSD::contextMenuEvent(QContextMenuEvent *e)
{
    osd_menu->exec (e->globalPos());
}

void OSD::clear()
{
    osd_interr = true;
    update ();
}

void OSD::mousePressEvent( QMouseEvent* e )
{
    m_dragOffset = e->pos();

    if(e->button() == Qt::LeftButton && !m_dragging)
    {
        grabMouse( Qt::SizeAllCursor );
        m_dragging = true;
    }
}


void OSD::mouseReleaseEvent( QMouseEvent* )
{
    if(m_dragging)
    {
        m_dragging = false;
        releaseMouse();
    }
}


void OSD::mouseMoveEvent( QMouseEvent* e )
{
    if( m_dragging && this == mouseGrabber() ) {

        // check if the osd has been dragged out of the current screen
        int currentScreen = QApplication::desktop()->screenNumber( e->globalPos() );
        if( currentScreen != -1 )
            m_screen = currentScreen;

        const QRect screen = QApplication::desktop()->screenGeometry( m_screen );

        // make sure the position is valid
        m_position = fixupPosition( e->globalPos() - m_dragOffset - screen.topLeft() );

        // move us to the new position
        move( m_position );

        // fix the position
        int midH = screen.width()/2;
        int midV = screen.height()/2;
        if( m_position.x() + width() > midH )
            m_position.rx() += width();
        if( m_position.y() + height() > midV )
            m_position.ry() += height();
    }
}


QPoint OSD::fixupPosition( const QPoint& pp )
{
    QPoint p(pp);
    const QRect& screen = QApplication::desktop()->screenGeometry( m_screen );
    int maxY = screen.height() - height() - s_outerMargin;
    int maxX = screen.width() - width() - s_outerMargin;

    if( p.y() < s_outerMargin )
        p.ry() = s_outerMargin;
    else if( p.y() > maxY )
        p.ry() = maxY;

    if( p.x() < s_outerMargin )
        p.rx() = s_outerMargin;
    else if( p.x() > maxX )
        p.rx() = screen.width() - s_outerMargin - width();

    p += screen.topLeft();

    return p;
}


void OSD::add(const int &speed)
{
    if (osd_interr) osd_interr = false;

    osd_data [osd_idx ++] = speed;
    if (osd_idx > sizeof(osd_data)) osd_idx = 0;

    update ();
}

void OSD::setMaxSpeedIndicator(const int &maxSpeed)
{
    osd_maxSpeed = maxSpeed;
}

void OSD::paintEvent(QPaintEvent *)
{
    QPainter painter (this);
    painter.setPen( QColor (Qt::black) );

    // draw the background and the frame
    QRect thisRect(0, 0, width() - 1 , height() - 1);
    painter.fillRect(thisRect, QColor::fromRgb(58, 117, 177) );

    if (! osd_interr)
    {
        // progress graph
        painter.setPen(QColor ("#3AD5FF"));

        int square_width_each = ( width() - 2 ) / sizeof(osd_data);
        int max_square_height = height() * 0.6;

        int x = 0.5 * ( width() - sizeof(osd_data) * square_width_each ) , y = 0;
        for (size_t i = osd_idx ; i < sizeof(osd_data) + osd_idx; ++ i)
        {
            if ( osd_data [i % sizeof(osd_data)] > osd_maxSpeed )
            {
                QRect rect (x, height() - 1 - max_square_height,
                            square_width_each , max_square_height);
                painter.fillRect(rect, QBrush (QColor ("#3AD5FF")) );
            }
            else
            {
                y = max_square_height *
                        (double)osd_data [i % sizeof(osd_data)]
                        / osd_maxSpeed;

                QRect rect (x, height() - 1 - y , square_width_each , y );
                painter.fillRect( rect , QBrush (QColor ("#3AD5FF")) );
            }

            x += square_width_each;
        }

        // progress text
        painter.setPen(QColor(Qt::white));

        QFontMetrics fm (font ());
        QRect fontRect ( (width() - fm.width(m_text)) / 2,
                         2, fm.width(m_text), fm.height());
        painter.drawText(fontRect, Qt::AlignCenter, m_text);
    }

    // frame
    painter.setPen( QColor (Qt::black) );
    painter.drawRect( thisRect );
}
