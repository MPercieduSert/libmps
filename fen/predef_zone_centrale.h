/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/12/2019
 */
#ifndef PREDEF_ZONE_CENTRALE_H
#define PREDEF_ZONE_CENTRALE_H

#include "abstract_tab_zone_centrale.h"
#include "predef_tab.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Zone centrale de type porte onglets avec des onglet prédéfinit.
 */
class predef_zone_centrale : public abstract_tab_zone_centrale {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    explicit predef_zone_centrale(predef_tab *tab, QWidget *parent = nullptr);

    //! Destructeur.
    ~predef_zone_centrale() override = default;

    //! Accesseur du gestionnaire d'onglets.
    predef_tab *tab()
        {return static_cast<predef_tab *>(m_tab);}
};
}}
#endif // PREDEF_ZONE_CENTRALE_H
