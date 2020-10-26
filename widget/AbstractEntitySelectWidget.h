/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/10/2020
 */
#ifndef ABSTRACTENTITYSELECTWIDGET_H
#define ABSTRACTENTITYSELECTWIDGET_H

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Bdd.h"
#include "IdComboBox.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Classe mère des widget de séléction d'une entité.
 */
class AbstractEntitySelectWidget : public QWidget {
    Q_OBJECT
protected:
    QBoxLayout * m_mainLayout;
    bddMPS::Bdd & m_bdd;
public:
    //! Constructeur.
    AbstractEntitySelectWidget(bddMPS::Bdd & bdd, Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Acceseur de l'identifiant de l'entité séléctionnée.
    virtual idt id() const = 0;

    //! Mutateur de l'identifiant de l'entité séléctionnée.
    virtual void setId(idt id) = 0;

signals :
    //! La valeur de l'identifiant change.
    void idChanged(szt id);
};

/*! \ingroup groupeWidget
 * \brief Classe mère des widget de séléction d'une entitée par un idCombox.
 */
class ComboBoxEntitySelectWidget : public widgetMPS::AbstractEntitySelectWidget{
    Q_OBJECT
protected:
    QLabel * m_label;     //!< Label du choix de l'annee.
    widgetMPS::IdComboBox * m_box;      //!< Choix de l'annee Scolaire.

public:
    //! Constructeur.
    ComboBoxEntitySelectWidget(bddMPS::Bdd & bdd, const QString & label = QString(),
                         Qt::Orientations orientation = Qt::Horizontal, QWidget * parent = nullptr);

    //! Acceseur de l'identifiant de l'entité séléctionnée.
    idt id() const override
        {return m_box->id();}

    //! Mutateur de l'identifiant de l'entité séléctionnée.
    void setId(idt id) override
        {m_box->setCurrentIndexId(id);}
};

}
#endif // ABSTRACTENTITYSELECTWIDGET_H
