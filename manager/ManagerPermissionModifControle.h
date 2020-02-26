/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERPERMISSIONMODIFCONTROLE_H
#define MANAGERPERMISSIONMODIFCONTROLE_H

#include "ManagerModifControle.h"
#include "ManagerPermission.h"

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template des différents manageurs à modification controlée de permission.
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
    ManagerPermissionModifControle (const InfoBdd & info,
                                    std::unique_ptr<AbstractGestionAutorisation<Ent>> && gestionAutorisation
                                                = std::make_unique<GestionAutorisationNoRestrictif<Ent>>(),
                                    std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique = std::make_unique<NoUniqueSql<Ent>>())
        : ManagerSql<Ent>(info, std::move(unique)),
          ManagerModifControle<Ent>(std::move(gestionAutorisation)) {}

    //! Destructeur.
    ~ManagerPermissionModifControle() override = default;

    //! Renvoie la liste des restrictions gérer par le gestionérer d'autorisarion.
    std::vector<bmps::autorisation> restriction() const override
        {return ManagerModifControle<Ent>::restriction();}

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent & entity) override
        {ManagerPerm::save(entity);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent & entity) override
        {ManagerPerm::save(entity);}

    //! Retourne le type du manager.
    virtual unsigned typeManager() const
        {return ManagerPerm::typeManager() | ManagerMC::typeManager();}

protected:
    /*//! Constructeur.
    ManagerPermissionModifControle() = default;*/

    //! Appelle la fonction d'insertion parent souhaitée.
    void addParent(Ent & entity) override
        {ManagerMC::add(entity);}

    //! Appelle la fonction d'insertion parent souhaitée.
    void addParent(const Ent & entity) override
        {ManagerMC::add(entity);}

//    //! Supprime de la table en base de donnée l'entité d'identifiant id.
//    bool del(idt id) override;

    //! Réimplemente modify.
    void modify(const Ent & entity) override
        {ManagerPerm::modify(entity);}

    //! Réimplemente modify.
    void modify(const Ent & entity, idt id) override
        {ManagerPerm::modify(entity,id);}

    //! Appelle la fonction de modification parent souhaitée.
    void modifyParent(const Ent & entity) override
        {ManagerMC::modify(entity);}

    //! Appelle la fonction de modification parent souhaitée.
    void modifyParent(const Ent & entity, idt id) override
        {ManagerMC::modify(entity,id);}
};

//template<class Ent> bool ManagerPermissionModifControle<Ent>::del(idt id)
//    {return ManagerMC::del(id);}

//! Manager de permission de type num à modification controlé.
template<class Ent> using ManagerPermissionNumModifControle = ManagerPermissionModifControle<ManagerPermissionNum<Ent>>;
//! Manager de permission de type code à modification controlé.
template<class Ent> using ManagerPermissionCodeModifControle = ManagerPermissionModifControle<ManagerPermissionCode<Ent>>;
}
#endif // MANAGERPERMISSIONMODIFCONTROLE_H
