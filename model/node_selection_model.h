/*Auteur: PERCIE DU SERT Maxime
 *Date: 14/11/2020
 */
#ifndef NODE_SELECTION_MODEL_H
#define NODE_SELECTION_MODEL_H

#include "item_node_model.h"

namespace mps {
namespace model_base {
/*! \ingroup groupe_model
 *\brief Model de selection pour les view d'un Abstractnode_ptrModel.
 */
class node_selection_model : public QObject {
    Q_OBJECT
protected:
    item_node_model *m_model;           //!< Model associé à la selection.
    bool m_has_selection = false;       //!< Sélection non vide.
    node_index m_current_index;         //!< Index courant.
    std::list<node_index> m_selection;  //!< Sélection courant.

public:
    //! Drapeaux de sélection.
    enum selection_code : flag::flag_type {
        No_Update = 0x0,        //!< Pas de mis à jour de la sélection.
        Clear = 0x1,            //!< Efface la sélection Précédente.
        Select = 0x2,           //!< Sélectionne les index spécifiés.
        Deselect = 0x4,         //!< Désélectionne les index spécifiés.
        Toggle = 0x8,           //!< Sélectionne ou désélection les index en fonction de la sélection actuelle.
        Children = 0x10         //!< Sélection la descendance.
    };

    //! Constructeur.
    node_selection_model(item_node_model *model = nullptr, QObject *parent = nullptr);

    //! Accesseur de l'index courant.
    node_index current_index() const
        {return m_current_index;}

    //! Accesseur de l'état de sélection.
    bool has_selection() const noexcept
        {return m_has_selection;}

    //! Teste si index et l'index courant.
    bool is_current_index(const node_index &index)
        {return m_has_selection && m_current_index == index;}

    //! Teste si un index est sélectionné.
    bool is_selected(const node_index &index) {
        if(m_has_selection && m_model == index.model())
            return std::find(m_selection.cbegin(),m_selection.cend(),index) != m_selection.cend();
        return false;
    }

    //! Accesseur du model.
    item_node_model *model() const noexcept
        {return m_model;}

    //! Accesseur de la liste de index sélectionnés.
    const std::list<node_index> &selected_index() const
        {return m_selection;}

    //! Mutateur du model.
    void set_model(item_node_model *model)
        {m_model = model;}
public slots:
    //! Efface la sélection et l'index courant (émet selectionChanged et current_changed).
    void clear() {}

    //! Efface l'index courant (émet current_changed)
    void clear_current_index() {}

    //! Efface la sélection (émet selectionChanged).
    void clear_selection() {}

    //! Efface la sélection et l'index courant sans émettre de signaux.
    void reset() {}

    //! Sélectionne (émet selectionChanged)
    void select(const model_base::node_index &/*index*/, mps::flag /*selection_flag*/) {}

    //! Sélectionne (émet selectionChanged)
    void select(const std::list<model_base::node_index> &/*index*/, mps::flag /*selection_flag*/) {}

    //! Mutateur de l'index courant.
    void set_current_index(const model_base::node_index &index, mps::flag selection_flag);
signals:
    //! L'index courant a changé.
    void current_changed(const model_base::node_index &current, const model_base::node_index &previous);

    //! Le model a changé.
    void model_changed(model_base::item_node_model *model);

    //! La sélection à changé.
    void selection_changed(const std::list<model_base::node_index> &selected, const std::list<model_base::node_index> &deselected);
protected:
    //! Compare les deux sélections puis émét selectionChanged.
    void emit_selection_changed(const std::list<node_index> &/*new_selection*/, const std::list<node_index> &/*old_selection*/) {}
};
}}
#endif // NODE_SELECTION_MODEL_H
