#include "tooltipswidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QColor>
#include <QFontMetricsF>
#include <QDebug>
#include <QEvent>
#include <QPainter>

QList<QWidget*> tooltipswidget::TargetWidgetList;
#define MAX_WIDTH 200
tooltipswidget::tooltipswidget(tooltipsInfo* info,  QWidget *parent)
    : QWidget{parent}
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::ToolTip);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_DeleteOnClose);

    setMaximumWidth(MAX_WIDTH);

    m_info = info;

    m_content = new QLabel(this);
    m_content->setWordWrap(true);
    m_content->setAlignment(Qt::AlignCenter);

    m_arrow = new QLabel(this);

    m_arrow->installEventFilter(this);
    m_content->setText(m_info->m_content);
    m_content->setStyleSheet(QString("font-family:\"微软雅黑\"; font-size:12px; color:%1; background-color:%2; border-radius: 3px;").arg(m_info->m_contentColor.name(), m_info->m_bkcolor.name(QColor::HexArgb)));

    QLayout* layout = nullptr;
    if(m_info->m_direction == tooltipsInfo::left || m_info->m_direction == tooltipsInfo::right)
    {
        layout = new QHBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(0);
        QHBoxLayout* pLayout = static_cast<QHBoxLayout*>(layout);
        m_arrow->setFixedSize(7, 14);  //三角形
        if(m_info->m_direction == tooltipsInfo::left)
        {
            QVBoxLayout* vlayout = new QVBoxLayout;
            vlayout->addSpacerItem(new QSpacerItem(1 ,1, QSizePolicy::Fixed, QSizePolicy::Expanding));
            vlayout->addWidget(m_arrow);
            vlayout->addSpacerItem(new QSpacerItem(1 ,1, QSizePolicy::Fixed, QSizePolicy::Expanding));
            pLayout->addLayout(vlayout);

            pLayout->addWidget(m_content);
        }
        else
        {
            pLayout->addWidget(m_content);

            QVBoxLayout* vlayout = new QVBoxLayout;
            vlayout->addSpacerItem(new QSpacerItem(1 ,1, QSizePolicy::Fixed, QSizePolicy::Expanding));
            vlayout->addWidget(m_arrow);
            vlayout->addSpacerItem(new QSpacerItem(1 ,1, QSizePolicy::Fixed, QSizePolicy::Expanding));
            pLayout->addLayout(vlayout);
        }

        QFontMetricsF fontMetrics(m_content->font());
        QRectF rcText = fontMetrics.boundingRect(QRect(0, 0, MAX_WIDTH-m_arrow->width()-10, 999999), Qt::AlignTop | Qt::TextWordWrap, m_content->text());
        m_content->setFixedWidth(rcText.width()+20);
        m_content->setFixedHeight(getWidgetHeight(m_content)+20);

        resize(m_content->width() + m_arrow->width(), m_content->height());
    }
    else
    {
        layout = new QVBoxLayout(this);
        layout->setContentsMargins(0,0,0,0);
        layout->setSpacing(0);
        QVBoxLayout* pLayout = static_cast<QVBoxLayout*>(layout);
        m_arrow->setFixedSize(14, 7); //三角形
        if(m_info->m_direction == tooltipsInfo::top)
        {
            QHBoxLayout* hlayout = new QHBoxLayout;
            hlayout->addSpacerItem(new QSpacerItem(1 ,1, QSizePolicy::Expanding, QSizePolicy::Fixed));
            hlayout->addWidget(m_arrow);
            hlayout->addSpacerItem(new QSpacerItem(1 ,1, QSizePolicy::Expanding, QSizePolicy::Fixed));
            pLayout->addLayout(hlayout);

            pLayout->addWidget(m_content);
        }
        else
        {
            pLayout->addWidget(m_content);;

            QHBoxLayout* hlayout = new QHBoxLayout;
            hlayout->addSpacerItem(new QSpacerItem(1 ,1, QSizePolicy::Expanding, QSizePolicy::Fixed));
            hlayout->addWidget(m_arrow);
            hlayout->addSpacerItem(new QSpacerItem(1 ,1, QSizePolicy::Expanding, QSizePolicy::Fixed));
            pLayout->addLayout(hlayout);
        }

        QFontMetricsF fontMetrics(m_content->font());
        QRectF rcText = fontMetrics.boundingRect(QRect(0, 0, MAX_WIDTH-10, 999999), Qt::AlignCenter | Qt::TextWordWrap, m_content->text());
        m_content->setFixedWidth(rcText.width()+20);
        resize(m_content->width(), getWidgetHeight(m_content)+10 + m_arrow->width());
    }

    setLayout(layout);
}

void tooltipswidget::showtooltip()
{
    if(m_info->m_direction == tooltipsInfo::right)
    {
        QPoint srcPt = m_targetWidget->mapToGlobal(QPoint(0, 0));
        QRect targetRc = m_targetWidget->rect();

        QPoint destPt;
        destPt.setX(srcPt.x()-width());
        destPt.setY(srcPt.y()-(height() - targetRc.height())/2);

        move(destPt);
        m_showTimer.start(1000, this);
    }

    if(m_info->m_direction == tooltipsInfo::top)
    {
        QPoint srcPt = m_targetWidget->mapToGlobal(QPoint(0, 0));
        QRect targetRc = m_targetWidget->rect();

        QPoint destPt;
        destPt.setX(srcPt.x() - (width()-targetRc.width())/2);
        destPt.setY(srcPt.y() + targetRc.height());

        move(destPt);
        m_showTimer.start(1000, this);
    }

    if(m_info->m_direction == tooltipsInfo::left)
    {
        QPoint srcPt = m_targetWidget->mapToGlobal(QPoint(0, 0));
        QRect targetRc = m_targetWidget->rect();

        QPoint destPt;
        destPt.setX(srcPt.x()+targetRc.width());
        destPt.setY(srcPt.y()-(height() - targetRc.height())/2);

        move(destPt);
        m_showTimer.start(1000, this);
    }

    if(m_info->m_direction == tooltipsInfo::bottom)
    {
        QPoint srcPt = m_targetWidget->mapToGlobal(QPoint(0, 0));
        QRect targetRc = m_targetWidget->rect();

        QPoint destPt;
        destPt.setX(srcPt.x() - (width() - targetRc.width())/2);
        destPt.setY(srcPt.y() - height());

        move(destPt);
        m_showTimer.start(1000, this);
    }
}

void tooltipswidget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_showTimer.timerId())
    {
        m_showTimer.stop();
        show();
    }
    else
    {
        QWidget::timerEvent(event);
    }
}

void tooltipswidget::showtooltips(QWidget* TargetWidget, QString content)
{
    if(TargetWidget == nullptr)
        return;

    tooltipsInfo* tooltipinfo = new tooltipsInfo;
    tooltipinfo->m_content = content;
    tooltipinfo->m_direction = tooltipsInfo::bottom; //默认底部显示

    if(!TargetWidgetList.contains(TargetWidget))
    {
        tooltipswidget* tooltip = new tooltipswidget(tooltipinfo, TargetWidget);  //
        tooltip->setTargetWidget(TargetWidget);
        TargetWidgetList.append(TargetWidget);
    }
}
void tooltipswidget::showtooltips(QWidget* TargetWidget, tooltipsInfo* tooltipinfo)
{
    if(TargetWidget == nullptr)
        return;

    if(!TargetWidgetList.contains(TargetWidget))
    {
        tooltipswidget* tooltip = new tooltipswidget(tooltipinfo, TargetWidget);  //父窗口销毁本窗口自动销毁
        tooltip->setTargetWidget(TargetWidget);
        TargetWidgetList.append(TargetWidget);
    }
}

bool tooltipswidget::eventFilter(QObject *target, QEvent *event)
{
    if(m_arrow == target)
    {
        switch (event->type())
        {
        case QEvent::Paint:
            {
                QPainter painter(m_arrow);
                painter.setRenderHint(QPainter::Antialiasing, true);

                QBrush bk(m_info->m_bkcolor);
                painter.setBrush(bk);
                painter.setPen(Qt::NoPen);

               if(m_info->m_direction == tooltipsInfo::bottom)
               {
                    QPolygon trianglePolygon;
                    trianglePolygon << QPoint(0, 0) << QPoint(m_arrow->width(), 0) << QPoint(m_arrow->width()/2, m_arrow->height());
                    painter.drawPolygon(trianglePolygon);
               }
               else if(m_info->m_direction == tooltipsInfo::top)
               {
                   QPolygon trianglePolygon;
                   trianglePolygon << QPoint(0,  m_arrow->height()) << QPoint(m_arrow->width() /2, 0) << QPoint(m_arrow->width(), m_arrow->height());
                   painter.drawPolygon(trianglePolygon);
               }
               else if(m_info->m_direction == tooltipsInfo::left)
               {
                   QPolygon trianglePolygon;
                   trianglePolygon << QPoint(0, m_arrow->height()/2) << QPoint(m_arrow->width(), 0) << QPoint(m_arrow->width(), m_arrow->height());
                   painter.drawPolygon(trianglePolygon);
               }
               else if(m_info->m_direction == tooltipsInfo::right)
               {
                   QPolygon trianglePolygon;
                   trianglePolygon << QPoint(0, 0) << QPoint(m_arrow->width(), m_arrow->height()/2) << QPoint(0, m_arrow->height());
                   painter.drawPolygon(trianglePolygon);
               }
            }
            break;
        default:
            break;
        }
    }

    if(m_targetWidget == target)
    {
        switch (event->type())
        {
        case QEvent::Enter:
            showtooltip();
            break;
        case QEvent::Leave:
            m_showTimer.stop();
            hide();
            break;
        default:
            break;
        }
    }

    return QWidget::eventFilter(target, event);
}
