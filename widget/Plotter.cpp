#include "Plotter.h"

Plotter::Plotter(QWidget *parent) : QWidget(parent)
{
    setBackgroundRole(QPalette::Dark);
    setAutoFillBackground(true);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFocusPolicy(Qt::StrongFocus);
}

void Plotter::draw(QPainter *painter)
    {drawAxes(painter);}

void Plotter::drawAxes(QPainter *painter)
{
    QRect rect = rectGraph();
    if (!rect.isValid())
        return;
    QPen light = palette().light().color();
    if(m_axes.xAxe())
    {
        painter->setPen(light);
        int y0 = rect.top() + std::floor(rect.height() * (m_axes.yMax() - m_axes.yOrigine()) / m_axes.yEtendue());
        painter->drawLine(rect.left(), y0, rect.right(), y0);
        if(m_axes.xFleche())
        {
            int len = m_marge / 4;
            painter->drawLine(rect.right(), y0, rect.right()+len, y0);
            painter->drawLine(rect.right()+2*len/3, y0+5, rect.right()+len, y0);
            painter->drawLine(rect.right()+2*len/3, y0-5, rect.right()+len, y0);
        }
        for (QVector<double>::const_iterator i = m_axes.xGraduation().cbegin(); i != m_axes.xGraduation().cend(); ++i)
        {
            int x = rect.left() + (rect.width() -1) * (*i - m_axes.xMin()) / m_axes.xEtendue();
            painter->drawLine(x, y0, x, y0 + 5);
            if(m_axes.yAxe() && m_axes.xOrigine() == *i && m_axes.yOrigine() != m_axes.yMin())
            {
                if(m_axes.xOrigine() != m_axes.yOrigine() || !m_axes.yGraduation().contains(*i))
                        painter->drawText(x+5, y0 + 5, 100, 20,
                                            Qt::AlignLeft | Qt::AlignTop,
                                            QString::number(*i));
            }
            else
                painter->drawText(x - 50, y0 + 5, 100, 20,
                                    Qt::AlignHCenter | Qt::AlignTop,
                                    QString::number(*i));
        }
    }
    if(m_axes.yAxe())
    {
        painter->setPen(light);
        int x0 = rect.left() + std::floor(rect.width() * (m_axes.xOrigine() - m_axes.xMin()) / m_axes.xEtendue());
        painter->drawLine(x0, rect.bottom(), x0, rect.top());
        if(m_axes.yFleche())
        {
            int len = m_marge/4;
            painter->drawLine(x0, rect.top(), x0, rect.top()-len);
            painter->drawLine(x0+5, rect.top()-2*len/3, x0, rect.top()-len);
            painter->drawLine(x0-5, rect.top()-2*len/3, x0, rect.top()-len);
        }
        for (QVector<double>::const_iterator i = m_axes.yGraduation().cbegin(); i != m_axes.yGraduation().cend(); ++i)
        {
            int y = rect.bottom() - (rect.height() - 1) * (*i - m_axes.yMin()) / m_axes.yEtendue();
            painter->setPen(light);
            painter->drawLine(x0 - 5, y, x0, y);
            if(m_axes.xAxe() && m_axes.yOrigine() == *i && m_axes.xOrigine() != m_axes.xMin())
            {
                if(m_axes.xOrigine() != m_axes.xMin())
                {
                    if(m_axes.xOrigine() == m_axes.yOrigine())
                        painter->drawText(x0 - m_marge, y+5, m_marge - 5, 20,
                                          Qt::AlignRight | Qt::AlignTop,
                                          QString::number(*i));
                    else
                        painter->drawText(x0 - m_marge, y-20, m_marge - 5, 20,
                                          Qt::AlignRight | Qt::AlignBottom,
                                          QString::number(*i));
                }
            }
            else
                painter->drawText(x0 - m_marge, y - 10, m_marge - 5, 20,
                              Qt::AlignRight | Qt::AlignVCenter,
                              QString::number(*i));
        }
    }
}

void Plotter::drawGrid(QPainter *painter)
{
    QRect rect = rectGraph();
    if (!rect.isValid())
        return;
    QPen quiteDark(Qt::blue) ;//= palette().dark().color().light();
    QPen light = palette().light().color();
    for (int i = 0; i <= m_axes.xNumGraduation(); ++i) {
        int x = rect.left() + (i * (rect.width() - 1)
                               / m_axes.xNumGraduation());
        double label = m_axes.xMin() + (i * m_axes.xEtendue() / m_axes.xNumGraduation());
        painter->setPen(quiteDark);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(light);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, 20,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(label));
    }
    for (int j = 0; j <= m_axes.yNumGraduation(); ++j) {
        int y = rect.bottom() - (j * (rect.height() - 1)
                                 / m_axes.yNumGraduation());
        double label = m_axes.yMin() + (j * m_axes.yEtendue()/ m_axes.yNumGraduation());
        painter->setPen(quiteDark);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(light);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - m_marge, y - 10, m_marge - 5, 20,
                          Qt::AlignRight | Qt::AlignVCenter,
                          QString::number(label));
    }
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

QSize Plotter::minimumSizeHint() const
    {return QSize(6 * m_marge, 4 * m_marge);}

void Plotter::refreshPixmap()
{
    m_pixmap = QPixmap(size());
    m_pixmap.fill(QPalette::Dark);
    QPainter painter(&m_pixmap);
    painter.initFrom(this);
    draw(&painter);
    update();
}

void Plotter::resizeEvent(QResizeEvent * /* event */)
    {refreshPixmap();}

QSize Plotter::sizeHint() const
    {return QSize(6 * m_marge, 4 * m_marge);}

void Plotter::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, m_pixmap);
}

void Plotter::Axes::setXGraduation(double ecart)
{
    m_xGraduation.resize(std::floor((xOrigine() - xMin()) / ecart) + std::floor((xMax() - xOrigine()) / ecart) + 1);
    int i = 0;
    for(double xGrad = xOrigine(); xGrad <= xMax(); xGrad += ecart, ++i)
        m_xGraduation[i] = xGrad;
    for(double xGrad = xOrigine() - ecart; xGrad >= xMin(); xGrad -= ecart, ++i)
        m_xGraduation[i] = xGrad;
    std::sort(m_xGraduation.begin(), m_xGraduation.end());
}

void Plotter::Axes::setYGraduation(double ecart)
{
    m_yGraduation.resize(std::floor((yOrigine() - yMin()) / ecart) + std::floor((yMax() - yOrigine()) / ecart) + 1);
    int i = 0;
    for(double yGrad = yOrigine(); yGrad <= yMax(); yGrad += ecart, ++i)
        m_yGraduation[i] = yGrad;
    for(double yGrad = yOrigine() - ecart; yGrad >= yMin(); yGrad -= ecart, ++i)
        m_yGraduation[i] = yGrad;
    std::sort(m_yGraduation.begin(), m_yGraduation.end());
}
