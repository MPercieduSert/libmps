/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGER_MODIF_CONTROLE_H
#define MANAGER_MODIF_CONTROLE_H

#include "abstract_gestion_restriction.h"
#include "manager_sql.h"

namespace mps {
namespace manager {
/*! \ingroup groupe_manager
 *\brief Classe template mère des différents manageurs à modification controlée.
 */
template<class Ent> class base_manager_modif_controle {
protected:
    bool m_bypass = false;  //!< Booléen permettant de courtcircuité le gestionnaire de restriction.
    std::unique_ptr<abstract_gestion_restriction<Ent>>  m_gestion_restriction;   //!< Gestionnaire de restriction.
public:
    //! Constructeur. (Posséde le gestionnaire de restriction.)
    base_manager_modif_controle(std::unique_ptr<abstract_gestion_restriction<Ent>> && gestion_restriction)
        : m_gestion_restriction(std::move(gestion_restriction)) {}

    //! Destructeur.
    virtual ~base_manager_modif_controle() = default;

    //! Accesseur du gestionnaire de restriction.
    abstract_gestion_restriction<Ent> &gestion_restriction() const
        {return *m_gestion_restriction;}

    //! Ajoute des restrictions gérées par le gestionnaire.
    void add_enable_restriction(flag restriction)
        {m_gestion_restriction->add_enable_restriction(restriction);}

    //! Ajoute des restrictions de modification pour une entité donnée.
    virtual void add_restriction(idt id, flag restrict)
        {m_gestion_restriction->add_restriction(id,restrict);}

    //! Renvoie la liste des restrictions gérer par le gestionérer d'autorisarion.
    virtual flag enable_restriction() const
        {return m_gestion_restriction->enable_restriction();}

    //! Demande les restrictions de modification pour l'entité d'identifiant id.
    virtual flag get_restriction(idt id)
        {return m_bypass ? b2d::Aucune : m_gestion_restriction->get_restriction(id);}

    //! Mutateur des restrictions gérées par le gestionnaire.
    void set_enable_restriction(flag restriction)
        {m_gestion_restriction->set_enable_restriction(restriction);}

    //! Mutateur du gestionnaire d'autorisations.
    void set_gestion_restriction(abstract_gestion_restriction<Ent> *gestion_restriction)
        {m_gestion_restriction = gestion_restriction;}

    //! Modifie les restrictions de modification pour une entité donnée.
    virtual void set_restriction(idt id, flag restrict)
        {m_gestion_restriction->set_restriction(id,restrict);}

    //! Teste la restriction de modification pour une entité d'identifiant id.
    virtual bool test_restriction(idt id, flag restrict)
        {return m_bypass ? false : m_gestion_restriction->test_restriction(id, restrict);}

    //! Teste la restriction de modification pour une entité d'identifiant id avec les valeurs de ent.
    virtual bool test_restriction(idt id, flag restrict, const Ent &ent)
        {return m_bypass ? false : m_gestion_restriction->test_restriction(id, restrict, ent);}

protected:
    //! Met en place un court circuit du gestionnaire d'autorisations.
    void ouvrir()
        {m_bypass = true;}

    //! Met fin au court circuit du gestionnaire d'autorisations.
    void fermer()
        {m_bypass = false;}
};

/*! \ingroup groupe_manager
 *\brief Classe template manageurs à modification controlée.
 */
template<class Ent> class manager_modif_controle : public virtual manager_sql<Ent>, public base_manager_modif_controle<Ent> {
protected:
    using ma_sql = manager_sql<Ent>;
    using ma_bmc = base_manager_modif_controle<Ent>;

    using ma_bmc::m_gestion_restriction;

    using ma_sql::add;
    using ma_sql::message_erreurs;
    using ma_sql::table;
    using ma_bmc::ouvrir;
    using ma_bmc::fermer;

public:
    using ma_sql::exists_unique;
    using ma_sql::exists;
    using ma_sql::del;
    using ma_sql::get;
    using ma_sql::same_in_bdd;
    using ma_sql::save;

    //! Constructeur
    manager_modif_controle(const info_bdd &info,
                          std::unique_ptr<abstract_gestion_restriction<Ent>> &&gestion_restriction
                                      = std::make_unique<abstract_gestion_restriction<Ent>>(),
                          std::unique_ptr<abstract_unique_sql_temp<Ent>> &&unique = std::make_unique<no_unique_sql<Ent>>())
        : manager_sql<Ent>(info,std::move(unique)), base_manager_modif_controle<Ent>(std::move(gestion_restriction))
            {m_gestion_restriction->set_enable_restriction(b2d::Modif_Suppr);}

    //! Destructeur.
    ~manager_modif_controle() override = default;

    //! Ajoute des restrictions de modification pour l'entité d'identifiant id.
    void add_restriction(idt id, flag restrict) override
        {ma_bmc::add_restriction(id, restrict);}


    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override
        {return !test_restriction(id,b2d::Suppr) && ma_sql::del(id);}

    //! Demande les restriction de modification pour l'entité d'identifiant id.
    flag get_restriction(idt id) override
        {return ma_bmc::get_restriction(id);}

    //! Renvoie la liste des restrictions gérées par le manageur.
    flag enable_restriction() const override
        {return ma_bmc::enable_restriction();}

    //! Modifie les restrictions de modification pour l'entité d'identifiant id.
    void set_restriction(idt id, flag restrict) override
        {ma_bmc::set_restriction(id, restrict);}

    //! Teste la restriction de modification pour une entité d'identifiant id.
    bool test_restriction(idt id, flag restrict) override
        {return ma_bmc::test_restriction(id, restrict);}

    //! Teste la restriction de modification pour une entité d'identifiant id avec les valeurs de ent.
    bool test_restriction(idt id, flag restrict, const Ent &ent) override
        {return ma_bmc::test_restriction(id, restrict, ent);}

    //! Retourne le type du manager.
    virtual flag type_manager() const
        {return b2d::Modif_Controle_Type_Manager;}

protected:
    //! Constructeur.
    manager_modif_controle(std::unique_ptr<abstract_gestion_restriction<Ent>> &&gestion_restriction
                         = std::make_unique<abstract_gestion_restriction<Ent>>())
        : base_manager_modif_controle<Ent>(std::move(gestion_restriction))
        {m_gestion_restriction->set_enable_restriction(b2d::Modif_Suppr);}

    //! Met à jour l'entité entity en base de donnée.
    void modify(const Ent &ent) override {
        if(!test_restriction(ent.id(), b2d::Modif,ent))
            ma_sql::modify(ent);
        else
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas modifiable:\n")
                                        .append(ent.affiche()).toStdString());
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'ent.
    void modify(const Ent &ent, idt id) override {
        if(!test_restriction(id, b2d::Modif, ent))
            ma_sql::modify(ent,id);
        else
            throw std::invalid_argument(QString("Erreur d'autorisation de modification' : "
                                                "l'entité suivante n'est pas modifiable:\n")
                                        .append(ent.affiche()).toStdString());
    }

    //! Enregistre l'entité entity en base en ignorant les restrictions.
    void save_by_pass(Ent &ent) override {
        ouvrir();
        save(ent);
        fermer();
    }
};
}}
#endif // MANAGER_MODIF_CONTROLE_H
