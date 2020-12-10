/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef STANDARDNODEWIDGET_H
#define STANDARDNODEWIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QDateEdit>
#include <QLabel>
#include <QLineEdit>
#include "NodeView.h"

namespace widgetMPS {

/*! \ingroup groupeWidget
 * \brief Classe des sous-noeuds composés d'une case à cocher.
 */
class CheckSubNodeWidget : public SubNodeWidget {
    Q_OBJECT
protected:
    QCheckBox * m_checkBox;       //! CheckBox du sous-noeud.
public:
    //! Constructeur.
    CheckSubNodeWidget(const NodeIndex & index, QWidget * parent);
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubNode(flag role) override;
};

/*! \ingroup groupeWidget
 * \brief Classe des sous-noeuds composés d'un label.
 */
class LabelSubNodeWidget : public SubNodeWidget {
    Q_OBJECT
protected:
    QLabel * m_label;       //! Label du sous-noeud.
public:
    //! Constructeur.
    LabelSubNodeWidget(const NodeIndex & index, QWidget * parent);
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubNode(flag role) override;
};

/*! \ingroup groupeWidget
 * \brief Classe des sous-noeuds composés d'un label et d'une liste de choix.
 */
class ComboBoxSubNodeWidget : public LabelSubNodeWidget {
    Q_OBJECT
protected:
    QComboBox * m_comboBox;       //! Liste de choix.
public:
    //! Constructeur.
    ComboBoxSubNodeWidget(const NodeIndex & index, QWidget * parent);
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubNode(flag role) override;
};

/*! \ingroup groupeWidget
 * \brief Classe des sous-noeuds composés d'un label et d'une ligne d'édition de date.
 */
class DateSubNodeWidget : public LabelSubNodeWidget {
    Q_OBJECT
protected:
    QDateEdit * m_dateEdit;       //! Ligne d'édition de la date du sous-noeud.
public:
    //! Constructeur.
    DateSubNodeWidget(const NodeIndex & index, QWidget * parent);
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubNode(flag role) override;
};

/*! \ingroup groupeWidget
 * \brief Classe des sous-noeuds composés d'un label et d'une ligne d'édition de texte.
 */
class LineEditSubNodeWidget : public LabelSubNodeWidget {
    Q_OBJECT
protected:
    QLineEdit * m_lineEdit;       //! Ligne d'édition du texte du sous-noeud.
public:
    //! Constructeur.
    LineEditSubNodeWidget(const NodeIndex & index, QWidget * parent);
protected:
    //! Met à jour les données du label à partir des données du model.
    void updateDataSubNode(flag role) override;
};

/*! \ingroup groupeWidget
 * \brief Classe dessinant les nodes widgets à angles arrondis.
 */
class RoundedArcPainter : public NodeView::ArcPainter {
public:
    enum parametre {
              LeftExpandMargin = LeftNodeMargin + 10,
              HSpacing = 3,
              WidthLine = 2,
              HMaxLine = 100,
              WidthCircle = 0,
              Rayon = 3,
              Ecart = Rayon * 4,
              NbrCircle = 3,
              VSpacing = 3,
              HSizeCircle = 2 * Rayon + Ecart * (NbrCircle - 1) + 2 * WidthCircle,
              VSizeCircle = 2 * Rayon + 2 * WidthCircle,
              RightCircles = LeftNodeMargin + HSizeCircle,
              HeightCircles = 2 * VSpacing + VSizeCircle
    };

    //! Dessine l'arc liant les descendants.
    void drawArc(ArcNodeViewWidget * arc) const override;

    //! Dessine la zone permettant l'expansion du noeud.
    void drawExpandZone(ArcNodeViewWidget * arc) const override;

    //! Renvoie taille verticale de la zone de demande d'expansion.
    int heightExpandZone() const override {return HeightCircles;}

    //! Renvoie la marge à gauche pour tracer l'arc.
    int leftExpandedMargin() const override {return LeftExpandMargin;}

    //! Renvoie taille horizontale de la zone de demande d'expansion.
    int widthExpandZone() const override {return RightCircles;}
};

/*! \ingroup groupeWidget
 * \brief Classe dessinant les nodes widgets à angles arrondis.
 */
class RoundedNodePainter : public NodeWidget::NodePainter {
public:
    enum {Rayon = 10,
         NoSelectedWidth = 2,
         SelectedWidth = NoSelectedWidth,
         CurrentWidth = 2 * SelectedWidth};

protected:
    int m_widthLine = NoSelectedWidth;              //!< Épaisseur du trait.
    QColor m_colorLine;                             //!< Couleur de la ligne.
public:
    //! Constructeur.
    RoundedNodePainter()
        : m_colorLine(QGuiApplication::palette().color(QPalette::Active,QPalette::WindowText)) {}

    //! Dessine widget.
    void paint(QWidget * widget) override;

    //! Mutateur de l'état de sélection.
    void setEtatSelection(NodeWidget::EtatSelection etat) override;
};

/*! \ingroup groupeWidget
 * \brief Vue standard des données externe à l'arbre d'un modèle hérité de AbstractNodeModel.
 */
class StandardExterneNode : public QWidget {
    Q_OBJECT
protected:

};
}
namespace delegateMPS {
/*! \ingroup groupeDelegate
 * \brief Delegate standard à un NodeView.
 */
class StandardNodeDelegate : public AbstractNodeDelegate {
public:
    using NodeIndex = modelMPS::NodeIndex;
    using NodeWidget = widgetMPS::NodeWidget;
    using SubNodeWidget = widgetMPS::SubNodeWidget;
    //! Constructeur.
    using AbstractNodeDelegate::AbstractNodeDelegate;

    //! Crée un noeud.
    NodeWidget * createNode(const NodeIndex &index, QWidget * parent = nullptr) const override;

    //! Crée un sous-noeud.
    virtual SubNodeWidget * createSubNode(const NodeIndex &index, QWidget *parent = nullptr) const;
};
}
#endif // STANDARDNODEWIDGET_H
