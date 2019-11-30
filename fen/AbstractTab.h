/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/09/2018
 */
#ifndef ABSTRACTTAB_H
#define ABSTRACTTAB_H

#include <QTabWidget>
#include "AbstractTabModule.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Fenêtre à onglet.
 */
class AbstractTab : public QTabWidget {
    Q_OBJECT
protected:
    std::map<std::pair<int,int>,AbstractTabModule *> m_mapTab;       //!< Tableau par type des onglets ouverts

public:
    //! Constructeur
    AbstractTab(QWidget *parent = nullptr)
        : QTabWidget(parent)
    {connect(this,&AbstractTab::currentChanged,this,&AbstractTab::currentChange);}

    //! Destructeur
    ~AbstractTab() override = default;

    //! Connect l'onglet aux signaux commun à tous les onglets.
    virtual void connectTab(AbstractTabModule * tab) const;

    //! Créateur d'onglet.
    virtual AbstractTabModule *createTab(std::pair<int,int> pair) const = 0;

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

public slots:
    //! Demande à l'onglet actif de coller.
    void coller()   {static_cast<AbstractTabModule*>(currentWidget())->coller();}

    //! Demande à l'onglet actif de couper.
    void couper()   {static_cast<AbstractTabModule*>(currentWidget())->couper();}

    //! Demande à l'onglet actif de copier.
    void copier()   {static_cast<AbstractTabModule*>(currentWidget())->copier();}

    //! Action à effectuer si la fenêtre courante change.
    void currentChange();

    //! Demande à l'onglet actif d'effacer.
    void effacer()  {static_cast<AbstractTabModule*>(currentWidget())->effacer();}

    //! Ouvre un nouvel onglet et le crée si celui-ci n'existe pas.
    bool openTab(const std::pair<int,int> & pair);

    //! Supprime un onglet.
    void removeTab(const std::pair<int, int> &pair);

    //! Demande à l'onglet actif de sauvegarder.
    void save()     {static_cast<AbstractTabModule*>(currentWidget())->save();}
};
}
#endif // TABMODULE_H
