/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef STANDARDNODEWIDGET_H
#define STANDARDNODEWIDGET_H

#include <QLabel>
#include "NodeView.h"

namespace widgetMPS {
class StandardNodeWidget;
/*! \ingroup groupeWidget
 * \brief Classe mère des sous noeuds standard.
 */
class AbstractSubNodeWidget : public QWidget {
    Q_OBJECT
protected:
    using NodeIndex = modelMPS::NodeIndex;
    using SubIndex = modelMPS::NodeIndex::SubIndex;
    NodeIndex m_index;                  //!< Index associé aux noeuds.
    QBoxLayout * m_mainLayout;          //!< Calque principale du sous-noeud.
public:
    //! Constructeur.
    AbstractSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent);

    //!< Destructeur.
    ~AbstractSubNodeWidget() override;

    //! Connecte les éléments du noeuds au model.
    virtual void connexion() const {}

    //! Déconnecte les éléments du noeuds au model.
    virtual void deconnexion() const {}

    //! Acceseur de l'index.
    NodeIndex index() const noexcept
        {return m_index;}
public slots:
    //! Met à jour les données du sous-noeud à partir des données du model.
    virtual void updateData() = 0;
};

/*! \ingroup groupeWidget
 * \brief Classe des sous-noeud composé d'un label.
 */
class LabelSubNodeWidget : public AbstractSubNodeWidget {
    Q_OBJECT
protected:
    QLabel * m_label;       //! Label du sous-noeud.
public:
    //! Constructeur.
    LabelSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent);

    //! Met à jour les données du label à partir des données du model.
    void updateData() override;
};

/*! \ingroup groupeWidget
 * \brief Classe standard des widgets des noeuds de l'arbre.
 */
class StandardNodeWidget : public AbstractNodeWidget {
    Q_OBJECT
    friend AbstractSubNodeWidget;
protected:
    using SubIndex = modelMPS::NodeIndex::SubIndex;
    std::multimap<SubIndex,AbstractSubNodeWidget*> m_cibleMap;      //!< Association des sous-index et des sous-noeuds.
    QBoxLayout * m_mainLayout;                                      //!< Calque principale.
public:
    //! Constructeur.
    StandardNodeWidget(const NodeIndex & index, ArcNodeViewWidget * parent = nullptr, int tp = NoType);

    //! Destructeur.
    ~StandardNodeWidget() override
        {m_cibleMap.clear();}

    //! Ajoute un sous-noeud.
    void addSubNodeWidget(AbstractSubNodeWidget * subNode);

public slots:
    //! Met à jour les données du widget à partir des données du model.
    void updateData() override;

    //! Met à jour les données du widget pour l'index index à partir des données du model.
    void updateData(const NodeIndex & index) override;
};
}
namespace delegateMPS {
/*! \ingroup groupeDelegate
 * \brief Delegate standard à un NodeView.
 */
class StandardNodeDelegate : public AbstractNodeDelegate {
public:
    using NodeIndex = modelMPS::NodeIndex;
    using AbstractNodeWidget = widgetMPS::AbstractNodeWidget;
    //! Constructeur.
    using AbstractNodeDelegate::AbstractNodeDelegate;
    //! Crée un widget
    AbstractNodeWidget * createWidget(const NodeIndex &index, widgetMPS::ArcNodeViewWidget * parent = nullptr) const override;
};
}
#endif // STANDARDNODEWIDGET_H
