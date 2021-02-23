/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/03/2019
 */
#ifndef PREDEFTAB_H
#define PREDEFTAB_H

#include "abstract_tab.h"
#include "abstract_tab_menu.h"
#include "tab_gestion_predef.h"
#include "tab_mot_cle.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Classe de création des onglets prédéfinis.
 */
class predef_tab : public abstract_tab {
    Q_OBJECT
protected:
    b2d::bdd_predef &m_bdd;          //!< Référence à la base de donnée.

public:
    enum tab_name{Gestion_Bdd_Tab_Id,
                  Gestion_Type_Tab_Id,
                  Nbr_Predef_Tab_Id};


    //! Constructeur.
    explicit predef_tab(b2d::bdd_predef &bdd, QWidget *parent = nullptr)
        : abstract_tab(parent), m_bdd(bdd) {}

    //! Destructeur par default.
    ~predef_tab() override = default;

    //! Créateur d'onglet.
    abstract_tab_module *create_tab(const tab_index &pair,
                                    const std::vector<QVariant> &args = std::vector<QVariant>()) const override;
};
}}
#endif // PREDEF_TAB_H
