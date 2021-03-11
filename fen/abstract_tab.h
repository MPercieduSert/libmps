/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/09/2018
 */
#ifndef ABSTRACTTAB_H
#define ABSTRACTTAB_H

#include <QTabWidget>
#include <QVariant>
#include "abstract_tab_module.h"
#include "fen_flags.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Fenêtre à onglet.
 */
class abstract_tab : public QTabWidget {
    Q_OBJECT
public:
    using tab_index = abstract_tab_module::tab_index;
protected:
    using map_index_tab = std::map<tab_index,abstract_tab_module *>;
    map_index_tab m_map_tab;       //!< Tableau par type des onglets ouverts
public:
    //! Constructeur
    abstract_tab(QWidget *parent = nullptr)
        : QTabWidget(parent) {
        setTabsClosable(true);
        connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(remove_tab(int)));
        connect(this,&abstract_tab::currentChanged,this,&abstract_tab::current_change);}

    //! Destructeur
    ~abstract_tab() override = default;

    //! Connect l'onglet aux signaux commun à tous les onglets.
    virtual void connect_tab(abstract_tab_module *tab);

    //! Créateur d'onglet.
    virtual abstract_tab_module *create_tab(const tab_index &pair,
                                            const std::vector<QVariant> &args = std::vector<QVariant>()) const = 0;

signals:
    //! Signal d'activation/désactivation des actions.
    void action_permise(flag);

public slots:
    //! Demande à l'onglet actif de coller.
    void coller()   {static_cast<abstract_tab_module*>(currentWidget())->coller();}

    //! Demande à l'onglet actif de couper.
    void couper()   {static_cast<abstract_tab_module*>(currentWidget())->couper();}

    //! Demande à l'onglet actif de copier.
    void copier()   {static_cast<abstract_tab_module*>(currentWidget())->copier();}

    //! Action à effectuer si la fenêtre courante change.
    void current_change();

    //! Demande à l'onglet actif d'effacer.
    void effacer()  {static_cast<abstract_tab_module*>(currentWidget())->effacer();}

    //! Ouvre un nouvel onglet et le crée si celui-ci n'existe pas.
    bool open_tab(const tab_index &pair, const std::vector<QVariant> &args = std::vector<QVariant>());

    //! Supprime un onglet.
    void remove_tab(int index);

    //! Supprime un onglet.
    void remove_tab(const tab_index &pair);

    //! Demande à l'onglet actif de sauvegarder.
    void sauver()     {static_cast<abstract_tab_module*>(currentWidget())->sauver();}

    //! Demande à l'onglet actif de supprimer.
    void supprimer()    {static_cast<abstract_tab_module*>(currentWidget())->supprimer();}
};
}}
#endif // TABMODULE_H
