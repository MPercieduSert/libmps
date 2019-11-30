/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/09/2018
 */
#ifndef ABSTRACTTABMODULE_H
#define ABSTRACTTABMODULE_H

#include <QVBoxLayout>
#include <QWidget>
#include "typemps.h"

namespace fenMPS {
using namespace typeMPS;
/*! \ingroup groupeFen
 * \brief Classe mère des onglets.
 */
class AbstractTabModule : public QWidget {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    explicit AbstractTabModule(QWidget * parent = nullptr);

    //! Destructeur.
    virtual ~AbstractTabModule() override = default;

    //! Accesseur du titre.
    virtual QString title() const = 0;

signals:
    //! Signal d'activation/désactivation de l'action effacer.
    void effacerPermis(bool);

    //! Signal d'activation/désactivation de l'action coller.
    void collerPermis(bool);

    //! Signal d'activation/désactivation de l'action copier.
    void copierPermis(bool);

    //! Signal d'activation/désactivation de l'action couper.
    void couperPermis(bool);

    //! Signal d'activation/désactivation de l'action saugarder.
    void savePermis(bool);

    //! Demande l'ouverture d'un onglet.
    void openTabRequested(const std::pair<int,int> &);

public slots:
    //! Action à effectuer lorsque l'onglet devient actif.
    virtual void becomeCurrent();

    //! A réimplémenter pour effectuer l'action coller.
    virtual void coller()   {}

    //! A réimplémenter pour effectuer l'action copier.
    virtual void copier()   {}

    //! A réimplémenter pour effectuer l'action copier.
    virtual void couper()   {}

    //! A réimplémenter pour effectuer l'action effacer.
    virtual void effacer()   {}

    //! A réimplémenter pour effectuer l'action sauvegarder.
    virtual void save()   {}

    //! A réimplémenter pour effectuer l'action sauvegarder comme.
    virtual void saveAs()   {}
};
}
#endif // ABSTRACTTABMODULE_H
