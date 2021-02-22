/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERS_BASE_H
#define MANAGERS_BASE_H

#include <memory>
#include "manager_sql.h"
#include "unique_sql_base.h"
#include "entity_divers.h"

namespace mps {
namespace b2d {
    //! Version de création de la base de données.
    namespace bdd_version  {
        enum creation_bdd{Initiale,
                         Creation,
                         Nbr_Version
                        };
        enum type_bdd {Library_Type,
                      Nbr_Type};
    }
    //! Demande autorisation de modification
    enum demande_autorisation_modification_bdd{
        Oui = 1,
        Non = 0
    };

//    //! Structure contenant les donnée et variable associé à chaque table.
//    struct table {
//        QString name;                           //!< Nom de la table (entitée).
//        QString nameBdd;                        //!< Nom de la table dans la base de donnée.
//        std::vector<QString> names_attribut;    //!< Liste des noms des attributs.
//        std::map<post,QString> foreign_keys;    //!< Map des clés étrangères.
//        std::vector<table> tables_annexe;       //! Vecteur des tables annexes
//    };
}

namespace manager {
/*! \ingroup groupe_file
 * \brief Classe mère de la classe contenant les managers des entités pour la base de donnée.
 */
class managers_base {
protected:
    using version_bdd = entities::version_bdd;

    const entidt m_nbr_entity;                          //!< Nombre d'entité associées à un managers de la base de donnée.
    QSqlQuery m_requete;                            //!< Requéte commune aux manageurs.
    std::vector<std::unique_ptr<abstract_manager>> m_managers;           //!< Tableau des manageurs.
    std::unique_ptr<manager_sql<version_bdd>> m_manager_version;           //!< Manager de l'entité version de la base de donnée.

public:
    //! Constructeur.
    managers_base(entidt nbr_entity, const QString &version_table,
                  std::map<entidt,std::unique_ptr<abstract_manager>> &&managers
                        = std::map<entidt,std::unique_ptr<abstract_manager>>(),
                  const QSqlQuery &req = QSqlQuery());

    //! Creé la table de l'entité version_bdd
    void creer_version();

    //! Retourne la version courante de la base de donnée pour un type donnée.
    version_bdd get_version(idt type);

    //! Renvoie l'indice du manager associé à l'entité de nom name ou nbr_entity s'il n'existe pas.
    entidt find(const QString &name) const{
        entidt i = 0;
        while(i < m_nbr_entity &&(!valide(i) || info(i).name() != name))
            ++i;
        return i;
    }

    //! Retourne un référence sur le manager des entités de ID, id.
    abstract_manager &get(entidt id)
        {return get_v(id);}

    //! Retourne un référence sur le manager des entités de ID, id.
    const abstract_manager &get(entidt id) const
        {return get_v(id);}


    //! Méthode template permettant d'obtenir le manager correspondant à l'entité.
    template<class Ent> const abstract_manager_temp<Ent> &get() const
        {return static_cast<const abstract_manager_temp<Ent> &>(*m_managers[Ent::ID]);}

    //! Méthode template permettant d'obtenir le manager correspondant à l'entité.
    template<class Ent> abstract_manager_temp<Ent> &get()
        {return static_cast<abstract_manager_temp<Ent> &>(*m_managers[Ent::ID]);}

    //! Teste s'il existe une version d'un type donnée.
    bool exists_version(numt type);

    //! Renvoie les informations sur la table associée à l'entité d'identifiant id.
    const info_bdd &info(entidt id) const;

    //! Fabrique d'entité de classe nomée ent.
    std::unique_ptr<entity> make_entity(const QString &ent) const;

    //! Retourne le nombre d'entités associées à un manager dans la base de donnée.
    entidt nbr_entity() const noexcept
        {return m_nbr_entity;}

    //! Retourne le numero de version de la base de donnée.
    int num_version(numt type);

    //! Enregistre le numéro de version de la base de donnée.
    void save_version(int num, numt type);

    //! Modifie le pointeur vers l'objet requête.
    void set_requete(const QSqlQuery &req);

    //! Teste si le manager d'identifiant id est valide.
    bool valide(entidt id) const noexcept
        {return id < m_nbr_entity &&m_managers[id];}
protected:
    //! Construit les informations d'un arbre pour la création des managers de type arbre.
    info_bdd info_bdd_arbre(const QString &table) const;

    //! Mutateur de manager.
    template<class Ent> void set_manager(std::unique_ptr<abstract_manager_temp<Ent>> &&manager)
        {m_managers[Ent::ID] = std::move(manager);}

    abstract_manager &get_v(entidt id) const {
        if(id < m_nbr_entity) {
            if(m_managers[id])
                return *m_managers[id];
            else
                throw std::invalid_argument(QString("Manager non initialisé: id : ").append(QString::number(id)).toStdString());
        }
        else
            throw std::invalid_argument("Identifiant d'entité invalide dans la fonction get.");
    }
};
}}
#endif // MANAGERS_BASE_H
