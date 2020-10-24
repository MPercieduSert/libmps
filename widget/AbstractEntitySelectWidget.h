/*Auteur: PERCIE DU SERT Maxime
 *Date: 24/10/2020
 */
#ifndef ABSTRACTENTITYSELECTWIDGET_H
#define ABSTRACTENTITYSELECTWIDGET_H

#include <QHBoxLayout>
#include <QVBoxLayout>
#include "IdComboBox.h"

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief ComboBox de sélection d'un texte lier àun identifiant.
 */

class AbstractEntitySelectWidget : public QWidget {
    Q_OBJECT
protected:
    QLayout * m_mainLayout;
public:
    //! Constructeur.
    AbstractEntitySelectWidget(Qt::Orientations orientation, QWidget * parent = nullptr);

    //! Acceseur de l'identifiant de l'entité séléctionnée.
    virtual idt id() const = 0;

    //! Mutateur de l'identifiant de l'entité séléctionnée.
    virtual void setId() = 0;

signals :
    //! La valeur de l'identifiant change.
    void idChanged(szt id);
};
}
#endif // ABSTRACTENTITYSELECTWIDGET_H
