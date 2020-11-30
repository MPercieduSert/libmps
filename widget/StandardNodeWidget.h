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
    virtual void updateData(flag /*role*/) {}
};

/*! \ingroup groupeWidget
 * \brief Classe des sous-noeuds composés d'une case à cocher.
 */
class CheckSubNodeWidget : public AbstractSubNodeWidget {
    Q_OBJECT
protected:
    QCheckBox * m_checkBox;       //! CheckBox du sous-noeud.
public:
    //! Constructeur.
    CheckSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override;

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override;

    //! Met à jour les données du label à partir des données du model.
    void updateData(flag role) override;
};

/*! \ingroup groupeWidget
 * \brief Classe des sous-noeuds composés d'un label.
 */
class LabelSubNodeWidget : public AbstractSubNodeWidget {
    Q_OBJECT
protected:
    QLabel * m_label;       //! Label du sous-noeud.
public:
    //! Constructeur.
    LabelSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent);

    //! Met à jour les données du label à partir des données du model.
    void updateData(flag role) override;
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
    ComboBoxSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override;

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override;

    //! Met à jour les données du label à partir des données du model.
    void updateData(flag role) override;
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
    DateSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override;

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override;

    //! Met à jour les données du label à partir des données du model.
    void updateData(flag role) override;
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
    LineEditSubNodeWidget(const NodeIndex & index, StandardNodeWidget * parent);

    //! Connecte les éléments du noeuds au model.
    void connexion() const override;

    //! Déconnecte les éléments du noeuds au model.
    void deconnexion() const override;

    //! Met à jour les données du label à partir des données du model.
    void updateData(flag role) override;
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
    void updateData(const NodeIndex & index,flag role) override;
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
    using AbstractSubNodeWidget = widgetMPS::AbstractSubNodeWidget;
    //! Constructeur.
    using AbstractNodeDelegate::AbstractNodeDelegate;

    //! Crée un noeud.
    AbstractNodeWidget * createNode(const NodeIndex &index, widgetMPS::ArcNodeViewWidget * parent = nullptr) const override;

    //! Crée un sous-noeud.
    virtual AbstractSubNodeWidget * createSubNode(const NodeIndex &index, widgetMPS::StandardNodeWidget * parent = nullptr) const;
};
}
#endif // STANDARDNODEWIDGET_H
