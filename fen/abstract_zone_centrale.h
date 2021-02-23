/*Auteur: PERCIE DU SERT Maxime
 *Date: 28/11/2018
 */
#ifndef ABSTRACT_ZONE_CENTRALE_H
#define ABSTRACT_ZONE_CENTRALE_H

#include <QLayout>
#include <QWidget>
#include "fen_flags.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Zone centrale.
 */
class abstract_zone_centrale : public QWidget {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    abstract_zone_centrale(QWidget *parent = nullptr) : QWidget(parent) {}

signals:
    //! Signal d'activation/désactivation des actions.
    void action_permise(flag);

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
}}
#endif // ABSTRACT_ZONE_CENTRALE_H
