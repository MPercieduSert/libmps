/*Auteur: PERCIE DU SERT Maxime
 *Date: 14/11/2020
 */
#ifndef NODESELECTIONMODEL_H
#define NODESELECTIONMODEL_H

#include "ItemNodeModel.h"

namespace modelMPS {
/*! \ingroup groupeModel
 * \brief Model de selection pour les view d'un AbstractNodeModel.
 */
class NodeSelectionModel : public QObject {
    Q_OBJECT
protected:
    ItemNodeModel * m_model;        //!< Model associé à la selection.
    bool m_hasSelection = false;                //!< Sélection non vide.
    NodeIndex m_currentIndex;           //!< Index courant.
    std::list<NodeIndex> m_selection;   //!< Sélection courant.

public:
    //! Drapeaux de sélection.
    enum flagSelection : flag::flag_type {
        NoUpdate = 0x0,         //!< Pas de mis à jour de la sélection.
        Clear = 0x1,            //!< Efface la sélection Précédente.
        Select = 0x2,           //!< Sélectionne les index spécifiés.
        Deselect = 0x4,         //!< Désélectionne les index spécifiés.
        Toggle = 0x8,           //!< Sélectionne ou désélection les index en fonction de la sélection actuelle.
        Children = 0x10        //!< Sélection la descendance.
    };

    //! Constructeur.
    NodeSelectionModel(ItemNodeModel * model = nullptr, QObject * parent = nullptr);

    //! Accesseur de l'index courant.
    NodeIndex currentIndex() const
        {return m_currentIndex;}

    //! Accesseur de l'état de sélection.
    bool hasSelection() const noexcept
        {return m_hasSelection;}

    //! Teste si index et l'index courant.
    bool isCurrentIndex(const NodeIndex & index)
        {return m_hasSelection && m_currentIndex == index;}

    //! Teste si un index est sélectionné.
    bool isSelected(const NodeIndex & index) {
        if(m_hasSelection && m_model == index.model())
            return std::find(m_selection.cbegin(),m_selection.cend(),index) != m_selection.cend();
        return false;
    }

    //! Accesseur du model.
    ItemNodeModel * model() const noexcept
        {return m_model;}

    //! Accesseur de la liste de index sélectionnés.
    const std::list<NodeIndex> & selectedIndex() const
        {return m_selection;}

    //! Mutateur du model.
    void setModel(ItemNodeModel * model)
        {m_model = model;}
public slots:
    //! Efface la sélection et l'index courant (émet selectionChanged et currentChanged).
    void clear() {}

    //! Efface l'index courant (émet currentChanged)
    void clearCurrentIndex() {}

    //! Efface la sélection (émet selectionChanged).
    void clearSelection() {}

    //! Efface la sélection et l'index courant sans émettre de signaux.
    void reset() {}

    //! Sélectionne (émet selectionChanged)
    void select(const NodeIndex & index, flag selectionFlag) {}

    //! Sélectionne (émet selectionChanged)
    void select(const std::list<NodeIndex> & index, flag selectionFlag) {}

    //! Mutateur de l'index courant.
    void setCurrentIndex(const NodeIndex & index, flag selectionFlag);
signals:
    //! L'index courant a changé.
    void currentChanged(const NodeIndex & current, const NodeIndex & previous);

    //! Le model a changé.
    void modelChanged(ItemNodeModel * model);

    //! La sélection à changé.
    void selectionChanged(const std::list<NodeIndex> & selected, const std::list<NodeIndex> & deselected);
protected:
    //! Compare les deux sélections puis émét selectionChanged.
    void emitSelectionChanged(const std::list<NodeIndex> & newSelection, const std::list<NodeIndex> & oldSelection) {}
};
}
#endif // NODESELECTIONMODEL_H
