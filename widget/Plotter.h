/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef PLOTTER_H
#define PLOTTER_H

#include <cmath>
#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <QPainter>

/*! \ingroup groupeWidget
 * \brief Classe mère des graphiques avec des axes.
 */
class Plotter : public QWidget
{
    Q_OBJECT
public:
    class Axes
    {
    protected: 
       bool m_xAxe = true;          //!< Affichage ou non de l'axe des abscisses.
       bool m_xFleche = true;       //!< FLéche ou non au bout de l'axe des abscisses.
       bool m_xGrille = false;      //!< Affiche ou non une grille.

       bool m_yAxe = true;          //!< Affichage ou non de l'axe des ordonées.
       bool m_yFleche = true;       //!< FLéche ou non au bout de l'axe des ordonées.
       bool m_yGrille = false;      //!< Affiche ou non une grille.

       double m_xMax = 1;           //!< Maximum de l'axe des abscisses.
       double m_xMin = 0;           //!< Minimum de l'axe des abscisses.
       double m_xOrigine = 0;       //!< Abscisse de l'origine du repère.

       double m_yMax = 1;           //!< Maximum de l'axe des ordonées.
       double m_yMin = 0;           //!< Minimum de l'axe des ordonées.
       double m_yOrigine = 0;       //!< Ordonnée de l'origine du repère.

       QString m_xLabel;            //!< Légende de l'axe des abscisses.
       QString m_yLabel;            //!< Légende de l'axe des ordonées.
       QVector<double> m_xGraduation;   //!< Liste des graduations l'axe des abscisses.
       QVector<double> m_yGraduation;   //!< Liste des graduations de l'axe des ordonées.

    public:
        //! Constructeur.
        Axes() = default;

        //! Renvoie la l'étendue des abscisses.
        double xEtendue() const
            {return m_xMax - m_xMin;}
        //! Renvoie la l'étendue des ordonées.
        double yEtendue() const
            {return m_yMax - m_yMin;}

        //! Accesseur de xAxe.
        bool xAxe() const
            {return m_xAxe;}
        //! Accesseur de xFleche.
        bool xFleche() const
            {return m_xFleche;}
        //! Accesseur de xGraduation.
        const QVector<double> & xGraduation() const
            {return m_xGraduation;}
        //! Accesseur de xGrille.
        bool xGrille() const
            {return m_xGrille;}
        //! Accesseur de xLabel
        const QString & xLabel() const
            {return m_xLabel;}
        //! Accesseur de xMax.
        double xMax() const
            {return m_xMax;}
        //! Accesseur de xMin.
        double xMin() const
            {return m_xMin;}
        //! Accesseur de xNum Graduation.
        int xNumGraduation() const
            {return m_xGraduation.count();}
        //! Accesseur de xOrigine.
        double xOrigine() const
            {return m_xOrigine;}

        //! Accesseur de yAxe.
        bool yAxe() const
            {return m_yAxe;}
        //! Accesseur de yFleche.
        bool yFleche() const
            {return m_yFleche;}
        //! Accesseur de yGrduation.
        const QVector<double> & yGraduation() const
            {return m_yGraduation;}
        //! Accesseur de yGrille.
        bool yGrille() const
            {return m_yGrille;}
        //! Accesseur de yLabel
        const QString & yLabel() const
            {return m_yLabel;}
        //! Accesseur de yMax.
        double yMax() const
            {return m_yMax;}
        //! Accesseur de yMin.
        double yMin() const
            {return m_yMin;}
        //! Accesseur de yNum Graduation.
        int yNumGraduation() const
            {return m_yGraduation.count();}
        //! Accesseur de yOrigine.
        double yOrigine() const
            {return m_yOrigine;}

        //! Mutateur de xAxe.
        void setXAxe(bool axe)
            {m_xAxe = axe;}
        //! Mutateur de xFleche.
        void setXFleche(bool fleche)
            {m_xFleche = fleche;}
        //! Mutateur de xGrduation.
        void setXGraduation(double ecart);
        //! Mutateur de xGrduation.
        void setXGraduation(const QVector<double> valeur)
        {
            m_xGraduation = valeur;
            std::sort(m_xGraduation.begin(),m_xGraduation.end());
        }
        //! Mutateur de xGrille.
        void setXGrille(bool grille)
            {m_xGrille = grille;}
        //! Mutateur de xLabel
        void setXLabel(const QString & label)
            {m_xLabel = label;}
        //! Mutateur de xMax.
        void setXMax(double valeur)
            {m_xMax = valeur;}
        //! Mutateur de xMin.
        void setXMin(double min)
            {m_xMin = min;}
        //! Mutateur de xOrigine.
        void setXOrigine(double valeur)
            {m_xOrigine = valeur;}

        //! Mutateur de yAxe.
        void setYAxe(bool axe)
            {m_yAxe = axe;}
        //! Mutateur de yFleche.
        void setYFleche(bool fleche)
            {m_yFleche = fleche;}
        //! Mutateur de xGrduation.
        void setYGraduation(double ecart);
        //! Mutateur de xGrduation.
        void setYGraduation(const QVector<double> valeur)
        {
            m_yGraduation = valeur;
            std::sort(m_yGraduation.begin(), m_yGraduation.end());
        }
        //! Mutateur de xGrille.
        void setYGrille(bool grille)
            {m_yGrille = grille;}
        //! Mutateur de yLabel
        void setYLabel(const QString & label)
            {m_yLabel = label;}
        //! Mutateur de yMax.
        void setYMax(double valeur)
            {m_yMax = valeur;}
        //! Mutateur de yMin.
        void setYMin(double min)
            {m_yMin = min;}
        //! Mutateur de yOrigine.
        void setYOrigine(double valeur)
            {m_yOrigine = valeur;}
    };

protected:
    Axes m_axes;            //!< Propriétés des axes.
    QPixmap m_pixmap;       //!< Image du graphique.
    int m_marge;            //!< Taille de la marge.
public:
    //! Constructeur.
    explicit Plotter(QWidget *parent = nullptr);
    //! Destructeur par default.
    ~Plotter() override = default;
    //! Accesseurs des propriétés des axes.
    const Axes & axes() const
        {return m_axes;}
    //! Efface le graphique.
    void clearCurve(int id);
    //! Acceseurs de la taille de la marge en pixel.
    int marge() const
        {return m_marge;}
    //! Renvoie la taille minimum du widget.
    QSize minimumSizeHint() const override;
    //! Mutateur des proptiétés des axes.
    void setAxes(const Axes & axe)
        {m_axes = axe;}
    //! Accesseur de la taille de la marge.
    void setMarge(int marge)
        {m_marge = marge;}
    //! Renvoie la taille correcte du widget.
    QSize sizeHint() const override;

protected:
    //! Trace la graphique.
    virtual void draw(QPainter *painter);
    //! Trace une grille.
    void drawGrid(QPainter *painter);
    //! Trace le axes
    void drawAxes(QPainter *painter);
    //! Redessine le graphique.
    void paintEvent(QPaintEvent *event) override;
    //! Renvoie le rectangle où est dessiné le graphique.
    QRect rectGraph() const
        {return QRect(m_marge,m_marge,width() - 2 * m_marge, height() - 2 * m_marge);}
    //! Rafraichis le graphique.
    void refreshPixmap();
    //! Change la taille du widget.
    void resizeEvent(QResizeEvent *event) override;

signals:

public slots:
};

#endif // PLOTTER_H
