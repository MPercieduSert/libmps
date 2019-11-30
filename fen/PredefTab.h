/*Auteur: PERCIE DU SERT Maxime
 *Date: 08/03/2019
 */
#ifndef PREDEFTAB_H
#define PREDEFTAB_H

#include "AbstractTab.h"
#include "AbstractTabMenu.h"
#include "TabMotCle.h"


namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Classe de création des onglets prédéfinis.
 */
class PredefTab : public AbstractTab {
    Q_OBJECT
public:
    //! Constructeur.
    explicit PredefTab(QWidget *parent = nullptr) : AbstractTab(parent) {}

    //! Destructeur par default.
    ~PredefTab() override = default;

    //! Créateur d'onglet.
    AbstractTabModule *createTab(std::pair<int,int> pair) const override;
};
}
#endif // PREDEFTAB_H
