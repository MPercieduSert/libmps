/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/09/2018
 */
#ifndef ABSTRACTTAB_H
#define ABSTRACTTAB_H

#include <QTabWidget>
#include <QVariant>
#include "AbstractTabModule.h"
#include "FenFlags.h"

namespace fenMPS {
/*! \ingroup groupeFen
 *\brief Fenêtre à onglet.
 */
class AbstractTab : public QTabWidget {
    Q_OBJECT
protected:
    std::map<std::pair<int,int>,AbstractTabModule *> m_mapTab;       //!< Tableau par type des onglets ouverts

public:
    //! Constructeur
    AbstractTab(QWidget *parent = nullptr)
        : QTabWidget(parent) {
        setTabsClosable(true);
        connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(removeTab(int)));
        connect(this,&AbstractTab::currentChanged,this,&AbstractTab::currentChange);}

    //! Destructeur
    ~AbstractTab() override = default;

    //! Connect l'onglet aux signaux commun à tous les onglets.
    virtual void connectTab(AbstractTabModule *tab) const;

    //! Créateur d'onglet.
    virtual AbstractTabModule *createTab(const std::pair<int,int>&pair, const std::vector<QVariant> &args = std::vector<QVariant>()) const = 0;

signals:
    //! Signal d'activation/désactivation des actions.
    void actionPermise(flag);

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
    bool openTab(const std::pair<int,int> &pair, const std::vector<QVariant> &args = std::vector<QVariant>());

    //! Supprime un onglet.
    void removeTab(int index);

    //! Supprime un onglet.
    void removeTab(const std::pair<int, int> &pair);

    //! Demande à l'onglet actif de sauvegarder.
    void sauver()     {static_cast<AbstractTabModule*>(currentWidget())->sauver();}

    //! Demande à l'onglet actif de supprimer.
    void supprimer()    {static_cast<AbstractTabModule*>(currentWidget())->supprimer();}

};
}
#endif // TABMODULE_H
