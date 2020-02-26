/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/12/2019
 */
#ifndef PREDEFZONECENTRALE_H
#define PREDEFZONECENTRALE_H

#include "AbstractTabZoneCentrale.h"
#include "PredefTab.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Zone centrale de type porte onglets avec des onglet prédéfinit.
 */
class PredefZoneCentrale : public AbstractTabZoneCentrale {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    explicit PredefZoneCentrale(PredefTab * tab, QWidget *parent = nullptr);

    //! Destructeur.
    ~PredefZoneCentrale() override = default;

    //! Accesseur du gestionnaire d'onglets.
    PredefTab * tab()
        {return static_cast<PredefTab *>(m_tab);}
};
}
#endif // PREDEFZONECENTRALE_H
