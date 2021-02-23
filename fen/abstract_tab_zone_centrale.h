/*Auteur: PERCIE DU SERT Maxime
 *Date: 06/03/2018
 */
#ifndef ABSTRACT_TAB_ZONE_CENTRALE_H
#define ABSTRACT_TAB_ZONE_CENTRALE_H

#include "abstract_zone_centrale.h"
#include "abstract_tab.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Zone centrale de type porte onglets.
 */
class abstract_tab_zone_centrale : public abstract_zone_centrale {
    Q_OBJECT
protected:
    abstract_tab *m_tab;            //!< Porte onglet.
    QHBoxLayout *m_main_layout;     //!< Calque Principale.

public:
    //! Constructeur.
    explicit abstract_tab_zone_centrale(abstract_tab *tab, QWidget *parent = nullptr);

    //! Destructeur.
    ~abstract_tab_zone_centrale() override = default;

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

    //! Demande l'ouverture d'un onglet.
    void open_tab(const abstract_tab::tab_index &pair, const std::vector<QVariant> &args = std::vector<QVariant>())
        {m_tab->open_tab(pair,args);}

    //! Demande à l'onglet actif de sauvegarder.
    void sauver() override    {m_tab->sauver();}

    //! Demande à l'onglet actif de sauvegarder.
    void supprimer() override    {m_tab->supprimer();}


};
}}
#endif // ABSTRACTTABZONECENTRALE_H
