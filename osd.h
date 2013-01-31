#ifndef OSD_H
#define OSD_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QApplication>
#include <QPaintEvent>
#include <QTimer>
#include <QMenu>
#include <QDesktopWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QContextMenuEvent>
#include <QSettings>

class OSD : public QWidget
{
    Q_OBJECT

public:
    explicit OSD(QWidget *parent = 0);
    ~OSD();

    /*!
     * \brief Add a new speed data
     * \param speed
     */
    void add (const int & speed);

    /*!
     * \brief Task is suspended!
     */
    void clear ();

    /*!
     * \brief Set maximum speed indicator
     * \param maxSpeed
     */
    void setMaxSpeedIndicator (const int & maxSpeed);

    void setText (const QString & text);

    QMenu *osd_menu;

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void paintEvent(QPaintEvent *);
    void mousePressEvent( QMouseEvent* e );
    void mouseReleaseEvent( QMouseEvent* );
    void mouseMoveEvent( QMouseEvent* );
    void contextMenuEvent(QContextMenuEvent *e);
    void mouseDoubleClickEvent(QMouseEvent *);

signals:
    void doubleClicked ();
    void requestDownload (const QString & url);

private:
    bool m_dragging;
    QString m_text;
    QPoint m_dragOffset;
    int m_screen;
    QPoint m_position;
    bool osd_interr;

    static const int s_outerMargin = 15;
    QPoint fixupPosition( const QPoint& p );

    size_t osd_idx;
    int osd_maxSpeed;
    int osd_data [15];

public slots:

};

#endif // OSD_H
