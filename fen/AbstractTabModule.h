/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/09/2018
 */
#ifndef ABSTRACTTABMODULE_H
#define ABSTRACTTABMODULE_H

#include <QVBoxLayout>
#include <QWidget>
#include "FenFlags.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Classe mère des onglets.
 */
class AbstractTabModule : public QWidget {
    Q_OBJECT
protected:
    const std::pair<int,int> m_pairIndex; //!< Référence de l'onglet

public:
    enum {NoId = -1};
    //! Constructeur.
    explicit AbstractTabModule(const std::pair<int,int> & pairIndex, QWidget * parent = nullptr);

    //! Destructeur.
    virtual ~AbstractTabModule() override = default;

    //! Accesseur du titre.
    virtual QString title() const = 0;

    //! Accesseur de pairIndex.
    const std::pair<int,int> & pairIndex() const noexcept
        {return m_pairIndex;}

signals:
    //! Signal d'activation/désactivation des actions.
    void actionPermise(flag);

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
    virtual void sauver()   {}

    //! A réimplémenter pour effectuer l'action sauvegarder comme.
    virtual void sauverAs()   {}

    //! A réimplémenter pour effectuer l'action supprimer.
    virtual void supprimer()   {}
};
}
#endif // ABSTRACTTABMODULE_H
