/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERPERMISSIONMODIFCONTROLE_H
#define MANAGERPERMISSIONMODIFCONTROLE_H

#include "ManagerModifControle.h"
#include "ManagerPermission.h"

namespace managerMPS {
/*! \ingroup groupeManager
 *\brief Classe template des différents manageurs à modification controlée de permission.
 */
template<class ManagerPerm> class ManagerPermissionModifControle : public ManagerPerm,
                                                                           public ManagerModifControle<typename ManagerPerm::EntType> {
protected:
    using Ent = typename ManagerPerm::EntType;
    using ManagerMC = ManagerModifControle<Ent>;
    using ManagerPerm::add;

public:
    using ManagerPerm::sameInBdd;
    using ManagerMC::del;

    //! Constructeur
    ManagerPermissionModifControle (const InfoBdd &info,
                                    std::unique_ptr<AbstractGestionRestriction<Ent>> &&gestionAutorisation
                                                = std::make_unique<AbstractGestionRestriction<Ent>>(),
                                    std::unique_ptr<AbstractUniqueSql_temp<Ent>> &&unique = std::make_unique<NoUniqueSql<Ent>>())
        : ManagerSql<Ent>(info, std::move(unique)),
          ManagerModifControle<Ent>(std::move(gestionAutorisation)) {}

    //! Destructeur.
    ~ManagerPermissionModifControle() override = default;

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent &ent) override
        {ManagerPerm::save(entity);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent &ent) override
        {ManagerPerm::save(entity);}

    //! Retourne le type du manager.
    virtual flag typeManager() const
        {return ManagerPerm::typeManager() | ManagerMC::typeManager();}

protected:
    //! Appelle la fonction d'insertion parent souhaitée.
    void addParent(Ent &ent) override
        {ManagerMC::add(entity);}

    //! Appelle la fonction d'insertion parent souhaitée.
    void addParent(const Ent &ent) override
        {ManagerMC::add(entity);}

    //! Réimplemente modify.
    void modify(const Ent &ent) override
        {ManagerPerm::modify(entity);}

    //! Réimplemente modify.
    void modify(const Ent &ent, idt id) override
        {ManagerPerm::modify(entity,id);}

    //! Appelle la fonction de modification parent souhaitée.
    void modifyParent(const Ent &ent) override
        {ManagerMC::modify(entity);}

    //! Appelle la fonction de modification parent souhaitée.
    void modifyParent(const Ent &ent, idt id) override
        {ManagerMC::modify(entity,id);}
};

//! Manager de permission de type num à modification controlé.
template<class Ent> using ManagerPermissionNumModifControle = ManagerPermissionModifControle<ManagerPermissionNum<Ent>>;
//! Manager de permission de type code à modification controlé.
template<class Ent> using ManagerPermissionCodeModifControle = ManagerPermissionModifControle<ManagerPermissionCode<Ent>>;
}
#endif // MANAGERPERMISSIONMODIFCONTROLE_H
