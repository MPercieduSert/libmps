#include "Histogramme.h"

using namespace widgetMPS;

Histogramme::Histogramme(const std::vector<double> & intervalle, const std::vector<QBrush> &aspet, const std::vector<double> donnee, QWidget * parent)
    : BarPlotter(parent), m_aspet(aspet), m_intervalle(intervalle)
 {setDonnee(donnee);}

void Histogramme::calculBar() {
    if(m_intervalle.size() < 3 || m_donnee.empty())
        return;
    std::vector<int> effectif(m_intervalle.size()-1);
    for(auto i = m_donnee.cbegin(); i != m_donnee.cend(); ++i) {
        if(*i >= m_intervalle[0]){
            int j = 0;
            while(j != effectif.size() - 1 && *i > m_intervalle[j])
                ++j;
            if(*i <= m_intervalle.back())
                effectif[j] += 1;
        }
    }
    m_bars.resize(std::count_if(effectif.cbegin(),effectif.cend(),[](int i) -> bool {return i != 0;}));
    for(int j = 0, i = 0; j != effectif.size(); ++j) {
        if(effectif[j] != 0) {
            m_bars[i].aspet = m_aspet[j];
            m_bars[i].gauche = m_intervalle[j];
            m_bars[i].hauteur = effectif[j];
            m_bars[i].largeur= m_intervalle[j + 1] - m_intervalle[j];
            ++i;
        }
    }
}
