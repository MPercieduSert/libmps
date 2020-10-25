/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/10/2020
 */
#ifndef ABSTRACTENTITYSELECTWIDGET_H
#define ABSTRACTENTITYSELECTWIDGET_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "Bdd.h"
#include "IdComboBox.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief ComboBox de sélection d'un texte lier àun identifiant.
 */

class AbstractEntitySelectWidget : public QWidget {
    Q_OBJECT
protected:
    QLayout * m_mainLayout;
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
}
#endif // ABSTRACTENTITYSELECTWIDGET_H
