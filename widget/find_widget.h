/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/04/2020
 */
#ifndef FIND_WIDGET_H
#define FIND_WIDGET_H

#include "find_model.h"
#include "standard_node_widget.h"

namespace mps {
namespace widget {
/*! \ingroup groupe_Widget
 *\brief Widget de recherche définissant un arbre de filtres à appliquer à un model de type abstract_colonnes_model.
 */
class find_widget : public QWidget {
    Q_OBJECT
protected:
    // Model &delegate
    using node_delegate = delegate::standard_node_delegate;
    using find_model = model_base::find_model;
    find_model *m_model = nullptr;                //!< Model de recherche.
    // Widget
    QPushButton *m_find_button;     //!< Bouton de recherche.
    QPushButton *m_reset_button;    //!< Bouton de réinitialisation de la recherche.
    node_view *m_view;              //!< Vue de l'arbre de recherche.

    // Calque
    QVBoxLayout *m_main_layout;     //!< Calque principal.
    QHBoxLayout *m_button_layout;  //!< Claque des boutton.
public:
    //! Constructeur.
    find_widget(find_model *model = new find_model(),QWidget *parent = nullptr);

    //! Constructeur.
    find_widget(model_base::abstract_colonnes_model *model,QWidget *parent = nullptr)
        : find_widget(new find_model(model),parent) {}

    //! Accesseur du Model.
    find_model *find() const
        {return m_model;}

    //! Mutateur du model de recheche.
    void set_find_model(find_model *model);

    //! Mutateur du model filtré.
    void set_model(model_base::abstract_colonnes_model *model) {
        if(m_model)
            m_model->set_model(model);
    }
};
}}
#endif // FIND_WIDGET_H
