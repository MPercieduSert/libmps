#include "Histogramme.h"

Histogramme::Histogramme(const QVector<double> & intervalle, const QVector<QBrush> &aspet, const QVector<double> donnee, QWidget * parent)
    : BarPlotter(parent), m_aspet(aspet), m_intervalle(intervalle)
{
    setDonnee(donnee);
}

void Histogramme::calculBar()
{
    if(m_intervalle.count() < 3 || m_donnee.isEmpty())
        return;
    QVector<int> effectif(m_intervalle.count()-1);
    for(QVector<double>::const_iterator i = m_donnee.cbegin(); i != m_donnee.cend(); ++i)
    {
        if(*i >= m_intervalle[0])
        {
            int j = 0;
            while(j != effectif.count() - 1 && *i > m_intervalle[j])
                ++j;
            if(*i <= m_intervalle.last())
                effectif[j] += 1;
        }
    }
    m_bars.resize(std::count_if(effectif.cbegin(),effectif.cend(),[](int i) -> bool {return i != 0;}));
    for(int j = 0, i = 0; j != effectif.count(); ++j)
    {
        if(effectif[j] != 0)
        {
            m_bars[i].aspet = m_aspet[j];
            m_bars[i].gauche = m_intervalle[j];
            m_bars[i].hauteur = effectif[j];
            m_bars[i].largeur= m_intervalle[j + 1] - m_intervalle[j];
            ++i;
        }
    }
}
