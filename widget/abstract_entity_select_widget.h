/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/10/2020
 */
#ifndef ABSTRACT_ENTITY_SELECT_WIDGET_H
#define ABSTRACT_ENTITY_SELECT_WIDGET_H

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "bdd.h"
#include "id_combo_box.h"

namespace mps {
namespace widget {
/*! \ingroup groupe_Widget
 *\brief Classe mère des widget de séléction d'une entité.
 */
class abstract_entity_select_widget : public QWidget {
    Q_OBJECT
protected:
    QBoxLayout *m_main_layout;
    b2d::bdd &m_bdd;
public:
    //! Constructeur.
    abstract_entity_select_widget(b2d::bdd &bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget *parent = nullptr);

    //! Acceseur de l'identifiant de l'entité séléctionnée.
    virtual idt id() const = 0;
public slots:
    //! Mutateur de l'identifiant de l'entité séléctionnée.
    virtual void set_id(idt id) = 0;

signals :
    //! La valeur de l'identifiant change.
    void id_changed(idt id);
};

/*! \ingroup groupe_Widget
 *\brief Classe mère des widget de séléction d'une entitée par un idCombox.
 */
class combo_box_entity_select_widget : public widget::abstract_entity_select_widget{
    Q_OBJECT
protected:
    QLabel *m_label;     //!< Label du choix de l'annee.
    widget::id_combo_box *m_box;      //!< Choix de l'annee Scolaire.

public:
    //! Constructeur.
    combo_box_entity_select_widget(b2d::bdd &bdd, const QString &label = QString(),
                         Qt::Orientations orientation = Qt::Horizontal, QWidget *parent = nullptr);

    //! Acceseur de l'identifiant de l'entité séléctionnée.
    idt id() const override
        {return m_box->id();}
public slots:
    //! Mutateur de l'identifiant de l'entité séléctionnée.
    void set_id(idt id) override
        {m_box->set_current_index_id(id);}
};
}}
#endif // ABSTRACT_ENTITY_SELECT_WIDGET_H
