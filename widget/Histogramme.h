/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef HISTOGRAMME_H
#define HISTOGRAMME_H

#include "BarPlotter.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Classe mère des graphiques.
 */
class Histogramme : public BarPlotter
{
    Q_OBJECT
protected:
    std::vector<QBrush> m_aspet;        //!< Aspet des barres.
    std::vector<double> m_donnee;       //!< Série statistique continue brute.
    std::vector<double> m_intervalle;   //!< Extrémintées des intervalles.

public:
    //! Constructeur.
    explicit Histogramme(QWidget * parent = nullptr)
        : BarPlotter(parent) {}

    //! Constructeur.
    explicit Histogramme(const std::vector<double> & intervalle, const std::vector<QBrush> & aspet,
                const std::vector<double> donnee, QWidget * parent = nullptr);

    //! Destructeur par defaut.
    ~Histogramme() override = default;

    //! Accesseur des données.
    const std::vector<double> & donnee() const
        {return m_donnee;}
    //! Accesseur des intervalles.
    const std::vector<double> & intervalle() const
        {return m_intervalle;}
    //! Mutateurs des données.
    void setDonnee(const std::vector<double> & donnee)
    {
        m_donnee = donnee;
        std::sort(m_donnee.begin(),m_donnee.end());
        calculBar();
    }
    //! Mutateurs des intervalles.
    void setIntervalle(const std::vector<double> & intervalle)
        {m_intervalle = intervalle;}

protected:
    //! Détermine les barres en fonction des données et des intervalles
    void calculBar();
};
}
#endif // HISTOGRAMME_H
