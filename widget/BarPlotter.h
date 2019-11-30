/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef BARPLOTTER_H
#define BARPLOTTER_H

#include <cmath>
#include "Plotter.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Classe mère des histogramme.
 */
class BarPlotter : public Plotter {
    Q_OBJECT
public:
    /*! \ingroup groupeWidget
     * \brief Classe représentant une barre de l'histogramme.
     */
    class Bar{
    public:
        double gauche;      //!< Abscisse du coté gauche de la barre.
        double hauteur;     //!< Hauteur de la barre.
        double largeur;     //!< Largeur de la barre.
        QBrush aspet;       //!< Aspet de la Barre.

        //! Constructeur.
        Bar(double g = 0, double h = 0, double l = 0, const QBrush & a = QBrush())
            : gauche(g), hauteur(h), largeur(l), aspet(a) {}

        //! Renvoie l'abscisse du coté droit d'une barre.
        double droite() const
            {return gauche + largeur;}
    };

protected:
    std::vector<Bar> m_bars;
public:
    //!Constructeur.
    explicit BarPlotter(QWidget * parent = nullptr)
        : Plotter(parent) {}

    //! Constructeur avec données.
    explicit BarPlotter(const std::vector<Bar> & donnee, QWidget *parent)
        : Plotter(parent), m_bars(donnee) {}

    //! Destructeur par defaut.
    ~BarPlotter() override = default;

    //! Accesseur des données.
    const std::vector<Bar> & bars() const
        {return m_bars;}

    //! Déterminer les axes à partir des données.
    Axes AxesAuto() const;

    //! Modifie les axes à partir des données.
    void setAxesAuto()
        {setAxes(AxesAuto());}

    //! Mutateur des données.
    void setBars(const std::vector<Bar> & donnee) {
        m_bars = donnee;
        refreshPixmap();
    }
protected:
    //! Dessine le graphique.
    void draw(QPainter * painter) override;

    //! Détermine la graduation en fonction de l'étendue.
    double gradAuto(double etendue) const;
};
}
#endif // BARPLOTTER_H
