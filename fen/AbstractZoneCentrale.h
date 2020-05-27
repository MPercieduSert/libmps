/*Auteur: PERCIE DU SERT Maxime
 *Date: 28/11/2018
 */
#ifndef ABSTRACTZONECENTRALE_H
#define ABSTRACTZONECENTRALE_H

#include <QLayout>
#include <QWidget>
#include "FenFlags.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Zone centrale.
 */
class AbstractZoneCentrale : public QWidget {
    Q_OBJECT
protected:


public:
    //! Constructeur.
    AbstractZoneCentrale(QWidget * parent = nullptr) : QWidget(parent) {}

    //! Initialise le widget.
    //virtual void init(QWidget * parent) = 0;
signals:
    //! Signal d'activation/désactivation des actions.
    void actionPermise(const Action & action);

public slots:
    //! Demande à l'onglet actif de coller.
    virtual void coller() = 0;

    //! Demande à l'onglet actif de couper.
    virtual void couper() = 0;

    //! Demande à l'onglet actif de copier.
    virtual void copier() = 0;

    //! Demande à l'onglet actif d'effacer.
    virtual void effacer() = 0;

    //! Demande à l'onglet actif de sauvegarder.
    virtual void sauver() = 0;

    //! Demande à l'onglet actif de sauvegarder.
    virtual void supprimer() = 0;
};
}
#endif // ABSTRACTZONECENTRALE_H
