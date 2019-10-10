/*Auteur: PERCIE DU SERT Maxime
 *Date: 06/03/2018
 */
#ifndef ABSTRACTTABZONECENTRALE_H
#define ABSTRACTTABZONECENTRALE_H

#include "AbstractZoneCentrale.h"
#include "AbstractTab.h"

/*! \ingroup groupeFen
 * \brief Zone centrale de type porte onglets.
 */
class AbstractTabZoneCentrale : public AbstractZoneCentrale
{
    Q_OBJECT
protected:
    AbstractTab * m_tab;            //!< Porte onglet.
    QHBoxLayout * m_mainLayout;     //!< Calque Principale.

public:
    //! Constructeur.
    explicit AbstractTabZoneCentrale(AbstractTab * tab, QWidget *parent = nullptr);

    //! Destructeur.
    ~AbstractTabZoneCentrale() override = default;

signals:

public slots:
    //! Demande à l'onglet actif de coller.
    void coller() override  {m_tab->coller();}

    //! Demande à l'onglet actif de couper.
    void couper() override  {m_tab->couper();}

    //! Demande à l'onglet actif de copier.
    void copier() override  {m_tab->copier();}

    //! Demande à l'onglet actif d'effacer.
    void effacer() override {m_tab->effacer();}

    //! Demande à l'onglet actif de sauvegarder.
    void save() override    {m_tab->save();}
};

#endif // ABSTRACTTABZONECENTRALE_H
