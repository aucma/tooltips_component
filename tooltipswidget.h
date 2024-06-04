#ifndef TOOLTIPSWIDGET_H
#define TOOLTIPSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QBasicTimer>
#include <QDebug>

class tooltipsInfo
{
public:
    tooltipsInfo()
    {
        m_transparency = 220;
        m_bkcolor = QColor(54, 78, 128, m_transparency);
        m_contentColor = QColor(255, 255, 255);
    }

    enum showDirection
    {
        left,
        top,
        right,
        bottom
    };

    int m_transparency; //0-255
    QColor m_bkcolor;
    QColor m_contentColor;
    showDirection m_direction; //箭头方向
    QString m_content;
};


class tooltipswidget : public QWidget
{
    Q_OBJECT
public:
    explicit tooltipswidget(tooltipsInfo* info,  QWidget *parent = nullptr);
    bool eventFilter(QObject *target, QEvent *event) override;

    ~tooltipswidget()
    {
        qDebug() << "~tooltipswidget";
    }

    void timerEvent(QTimerEvent *event) override;
    void setTargetWidget(QWidget* targetWidget)
    {
        m_targetWidget = targetWidget;
        m_targetWidget->setMouseTracking(true);
        m_targetWidget->installEventFilter(this);
    }

    static void showtooltips(QWidget* TargetWidget, tooltipsInfo* tooltip = nullptr);
    static void showtooltips(QWidget* TargetWidget, QString content);
    static QList<QWidget*> TargetWidgetList;

signals:

private:
    template<typename T>
    int getWidgetHeight(T *w)
    {
        QStringList strs = w->text().split("\n");
        int w_width = w->width();
        int height = 0;
        QFontMetrics fm = w->fontMetrics();
        double mPixelPerCentimer = 1.0;
        int linesCount = 0;
        foreach (QString str, strs)
        {
            double tempWidth = fm.horizontalAdvance(str) / mPixelPerCentimer; //字数转行数
            linesCount += (tempWidth+w_width-1) / w_width;
        }
        height = fm.lineSpacing() * linesCount;
        return height;
    }

    void showtooltip();

private:
    tooltipsInfo* m_info;
    QLabel* m_content;
    QLabel* m_arrow; //箭头
    QBasicTimer m_showTimer;

    QWidget* m_targetWidget;
};

#endif // TOOLTIPSWIDGET_H
