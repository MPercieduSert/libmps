#include "BarPlotter.h"

void BarPlotter::draw(QPainter * painter)
{
    QRect rect = rectGraph();

    for(QVector<Bar>::const_iterator i = m_bars.cbegin(); i != m_bars.cend(); ++i)
    {
        painter->setBrush((*i).aspet);
        painter->drawRect(rect.left() + std::floor(((*i).gauche - m_axes.xMin()) * rect.width() / m_axes.xEtendue()),
                          rect.bottom(),
                          std::floor((*i).largeur * rect.width() / m_axes.xEtendue()),
                          - std::floor((*i).hauteur * rect.height() / m_axes.yEtendue()));
    }
    drawAxes(painter);
}

BarPlotter::Axes BarPlotter::AxesAuto() const
{
    Axes axes;
    axes.setXMin(std::min_element(m_bars.cbegin(),m_bars.cend(),[](Bar i, Bar j) -> bool {return i.gauche < j.gauche;})->gauche);
    axes.setXMax(std::max_element(m_bars.cbegin(),m_bars.cend(),[](Bar i, Bar j) -> bool {return i.droite() < j.droite();})->droite());
    axes.setXOrigine(axes.xMin());
    axes.setYMin(std::min(
                     std::min_element(m_bars.cbegin(),m_bars.cend(),[](Bar i, Bar j) -> bool {return i.hauteur < j.hauteur;})->hauteur,
                     0.0));
    axes.setYMax(std::max_element(m_bars.cbegin(),m_bars.cend(),[](Bar i, Bar j) -> bool {return i.hauteur < j.hauteur;})->hauteur);
    axes.setYOrigine(axes.yMin());
    axes.setYGraduation(gradAuto(axes.yEtendue()));


    return axes;
}

double BarPlotter::gradAuto(double etendue) const
{
    double n = std::floor(std::log10(etendue));
    double m = etendue * std::pow(10,-n);
    if(m <= 1.2)
        return std::pow(10, n-1);
    if(m < 2.5)
        return 2 * std::pow(10, n-1);
    if(m <= 3)
        return 2.5 * std::pow(10, n-1);
    if(m <= 6)
        return 5 * std::pow(10, n-1);
    return std::pow(10, n);
}
