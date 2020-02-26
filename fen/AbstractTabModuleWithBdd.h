/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/09/2018
 */
#ifndef ABSTRACTTABMODULEWITHBDD_H
#define ABSTRACTTABMODULEWITHBDD_H

#include "AbstractTabModule.h"
#include "Bdd.h"

namespace fenMPS {
namespace bmps = bddMPS;
/*! \ingroup groupeFen
 * \brief Mère des onglet utilisant la base de données.
 */
class AbstractTabModuleWithBdd : public AbstractTabModule {
    Q_OBJECT
protected:
    bmps::Bdd& m_bdd;    //!< Référence sur la base de donnée.

public:
    //! Constructeur.
    explicit AbstractTabModuleWithBdd(bmps::Bdd & bdd, const std::pair<int, int> &pairIndex, QWidget *parent = nullptr)
        : AbstractTabModule(pairIndex, parent), m_bdd(bdd) {}

    //! Destructeur.
    ~AbstractTabModuleWithBdd() override = default;

    //! Accesseur de la base de donnée.
    bmps::Bdd & bdd() const
        {return m_bdd;}
};
}
#endif // ABSTRACTTABMODULEWITHBDD_H
