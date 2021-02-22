/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/03/2019
 */
#ifndef PREDEFTAB_H
#define PREDEFTAB_H

#include "AbstractTab.h"
#include "AbstractTabMenu.h"
#include "Tabmot_cle.h"
#include "TabGestionBdd.h"
#include "TabGestionType.h"

namespace fenMPS {
/*! \ingroup groupeFen
 *\brief Classe de création des onglets prédéfinis.
 */
class PredefTab : public AbstractTab {
    Q_OBJECT
protected:
    b2d::bdd_predef &m_bdd;          //!< Référence à la base de donnée.

public:
    enum tabName{GestionBddTabId,
                 GestionTypeTabId,
                NbrPredefTabId};


    //! Constructeur.
    explicit PredefTab(b2d::bdd_predef &bdd, QWidget *parent = nullptr)
        : AbstractTab(parent), m_bdd(bdd) {}

    //! Destructeur par default.
    ~PredefTab() override = default;

    //! Créateur d'onglet.
    AbstractTabModule *createTab(const std::pair<int,int> &pair, const std::vector<QVariant> &args = std::vector<QVariant>()) const override;
};
}
#endif // PREDEFTAB_H
