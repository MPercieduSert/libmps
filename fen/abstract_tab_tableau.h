/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/10/2018
 */
#ifndef ABSTRACT_TAB_TABLEAU_H
#define ABSTRACT_TAB_TABLEAU_H

#include <QStyledItemDelegate>
#include <QTableView>

#include "abstract_tab_module.h"
#include "table_model.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Classe mère des onglets contenant des tableaux
 */
class abstract_tab_tableau : public abstract_tab_module_with_bdd {
    Q_OBJECT
protected:
    QPushButton *m_add_column_bouton;        //!< Bouton pour ajouter une colonne
    QStyledItemDelegate *m_delegate;       //!< Déleguer
    model_base::table_model *m_model;          //!< Model
    QTableView *m_view;                    //!< Vue
    QPushButton *m_save_bouton;             //!< Bouton save
public:
    //! Constructeur.
    explicit abstract_tab_tableau(b2d::bdd &bdd, const tab_index &index, QWidget *parent = nullptr);

    //! Destructeur.
    ~abstract_tab_tableau() override = default;

    //! Action à effectuer lorsque l'onglet devient actif.
    void become_current() override;

    //! Renvoie un pointeur sur le model.
    virtual model_base::table_model *model() const
        {return m_model;}

signals:

public slots:
    //! Slot coorepondant à l'action coller.
    void coller() override;
    //! Slot coorepondant à l'action copier.
    void copier() override;
    //! Slot coorepondant à l'action copier.
    void couper() override;
    //! Slot coorepondant à l'action effacer.
    void effacer() override;
    //! Slot coorepondant à l'action sauver.
    void sauver() override   {m_model->save();}
    //! Slot coorepondant à l'action supprimer.
    void supprimer() override;
};
}}
#endif // ABSTRACT_TAB_TABLEAU_H
