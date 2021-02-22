/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGER_PERMISSION_MODIF_CONTROLE_H
#define MANAGER_PERMISSION_MODIF_CONTROLE_H

#include "manager_modif_controle.h"
#include "manager_permission.h"

namespace mps {
namespace manager {
/*! \ingroup groupe_manager
 *\brief Classe template des différents manageurs à modification controlée de permission.
 */
template<class ManagerPerm> class manager_permission_modif_controle
        : public ManagerPerm,
          public manager_modif_controle<typename ManagerPerm::ent_type> {
protected:
    using Ent = typename ManagerPerm::ent_type;
    using ma_mc = manager_modif_controle<Ent>;
    using ManagerPerm::add;

public:
    using ManagerPerm::same_in_bdd;
    using ma_mc::del;

    //! Constructeur
    manager_permission_modif_controle (const info_bdd &info,
                                    std::unique_ptr<abstract_gestion_restriction<Ent>> &&gestion_autorisation
                                                = std::make_unique<abstract_gestion_restriction<Ent>>(),
                                    std::unique_ptr<abstract_unique_sql_temp<Ent>> &&unique
                                                = std::make_unique<no_unique_sql<Ent>>())
        : manager_sql<Ent>(info, std::move(unique)),
          manager_modif_controle<Ent>(std::move(gestion_autorisation)) {}

    //! Destructeur.
    ~manager_permission_modif_controle() override = default;

    //! Enregistre l'entité entity en base de donnée.
    void save(Ent &ent) override
        {ManagerPerm::save(ent);}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent &ent) override
        {ManagerPerm::save(ent);}

    //! Retourne le type du manager.
    virtual flag type_manager() const
        {return ManagerPerm::type_manager() | ma_mc::type_manager();}

protected:
    //! Appelle la fonction d'insertion parent souhaitée.
    void add_parent(Ent &ent) override
        {ma_mc::add(ent);}

    //! Appelle la fonction d'insertion parent souhaitée.
    void add_parent(const Ent &ent) override
        {ma_mc::add(ent);}

    //! Réimplemente modify.
    void modify(const Ent &ent) override
        {ManagerPerm::modify(ent);}

    //! Réimplemente modify.
    void modify(const Ent &ent, idt id) override
        {ManagerPerm::modify(ent,id);}

    //! Appelle la fonction de modification parent souhaitée.
    void modify_parent(const Ent &ent) override
        {ma_mc::modify(ent);}

    //! Appelle la fonction de modification parent souhaitée.
    void modify_parent(const Ent &ent, idt id) override
        {ma_mc::modify(ent,id);}
};

//! Manager de permission de type num à modification controlé.
template<class Ent> using manager_permission_num_modif_controle = manager_permission_modif_controle<manager_permission_num<Ent>>;
//! Manager de permission de type code à modification controlé.
template<class Ent> using manager_permission_code_modif_controle = manager_permission_modif_controle<manager_permission_code<Ent>>;
}}
#endif // MANAGER_PERMISSION_MODIF_CONTROLE_H
