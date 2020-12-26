/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/08/2018
 */
#ifndef MANAGERS_H
#define MANAGERS_H

//#include <iostream>
#include <memory>
#include "ManagerSql.h"
#include "UniqueSqlBase.h"
#include "EntityDivers.h"

namespace bddMPS {
    //! Version de création de la base de données.
    namespace bddVersion  {
        enum creationBdd{Initiale,
                         Creation,
                         NbrVersion
                        };
        enum typeBdd {LibraryType,
                      NbrType};
    }
    //! Demande autorisation de modification
    enum DemandeAutorisationModificationBdd{
        Oui = 1,
        Non = 0
    };

    //! Structure contenant les donnée et variable associé à chaque table.
    struct Table {
        QString name;                           //!< Nom de la table (entitée).
        QString nameBdd;                        //!< Nom de la table dans la base de donnée.
        std::vector<QString> namesAttributs;    //!< Liste des noms des attributs.
        std::map<post,QString> foreignKeys;      //!< Map des clés étrangères.
        std::vector<Table> tablesAnnexe;        //! Vecteur des tables annexes
    };
}

namespace managerMPS {
/*! \ingroup groupeFile
 *  \brief Classe mère de la classe contenant les managers des entités pour la base de donnée.
 */
class Managers {
protected:
    using VersionBdd = entityMPS::VersionBdd;

    const entidt m_nbrEntity;                          //!< Nombre d'entité associées à un managers de la base de donnée.
    QSqlQuery m_requete;                            //!< Requéte commune aux manageurs.
    std::vector<std::unique_ptr<AbstractManager>> m_managers;           //!< Tableau des manageurs.
    std::unique_ptr<ManagerSql<VersionBdd>> m_managerVersion;           //!< Manager de l'entité version de la base de donnée.

public:
    //! Constructeur.
    Managers(entidt nbrEntity, const QString & versionTable,
                     std::map<entidt,std::unique_ptr<AbstractManager>> && managers = std::map<entidt,std::unique_ptr<AbstractManager>>(),
                     const QSqlQuery & req = QSqlQuery());

    //! Destructeur.
//    ~Managers()
//    {
//        for(auto i = m_managers.cbegin(); i != m_managers.cend(); i++) {
//            if(*i)
//                delete *i;
//        }
//        if(m_managerVersion)
//            delete m_managerVersion;
//    }

    //! Creé la table de l'entité VersionBdd
    void creerVersion();

    //! Retourne la version courante de la base de donnée pour un type donnée.
    VersionBdd getVersion(idt type);

    //! Renvoie l'indice du manager associé à l'entité de nom name ou nbrEntity s'il n'existe pas.
    entidt find(const QString & name) const{
        entidt i = 0;
        while(i < m_nbrEntity && (!valide(i) || info(i).name() != name))
            ++i;
        return i;
    }

    //! Retourne un référence sur le manager des entités de ID, id.
    AbstractManager & get(entidt id)
        {return getP(id);}

    //! Retourne un référence sur le manager des entités de ID, id.
    const AbstractManager & get(entidt id) const
        {return getP(id);}


    //! Méthode template permettant d'obtenir le manager correspondant à l'entité.
    template<class Ent> const AbstractManagerTemp<Ent> & get() const
        {return static_cast<const AbstractManagerTemp<Ent> &>(*m_managers[Ent::ID]);}

    //! Méthode template permettant d'obtenir le manager correspondant à l'entité.
    template<class Ent> AbstractManagerTemp<Ent> & get()
        {return static_cast<AbstractManagerTemp<Ent> &>(*m_managers[Ent::ID]);}

    //! Teste s'il existe une version d'un type donnée.
    bool existsVersion(numt type);

    //! Renvoie les informations sur la table associée à l'entité d'identifiant id.
    const InfoBdd & info(entidt id) const;

    //! Fabrique d'entité de classe nomée entity.
    std::unique_ptr<Entity> makeEntity(const QString & entity) const;

    //! Retourne le nombre d'entités associées à un manager dans la base de donnée.
    entidt nbrEntity() const noexcept
        {return m_nbrEntity;}

    //! Retourne le numero de version de la base de donnée.
    int numVersion(numt type);

    //! Enregistre le numéro de version de la base de donnée.
    void saveVersion(int num, numt type);

    //! Modifie le pointeur vers l'objet requête.
    void setRequete(const QSqlQuery & req);

    //! Teste si le manager d'identifiant id est valide.
    bool valide(entidt id) const noexcept
        {return id < m_nbrEntity && m_managers[id];}
protected:
    //! Construit les informations d'un arbre pour la création des managers de type arbre.
    InfoBdd infoBddArbre(const QString & table) const;

    //! Mutateur de manager.
    template<class Ent> void setManager(std::unique_ptr<AbstractManagerTemp<Ent>> && manager)
        {m_managers[Ent::ID] = std::move(manager);}

    AbstractManager & getP(entidt id) const {
        if(id < m_nbrEntity) {
            if(m_managers[id])
                return *m_managers[id];
            else
                throw std::invalid_argument(QString("Manager non initialisé: id : ").append(QString::number(id)).toStdString());
        }
        else
            throw std::invalid_argument("Identifiant d'entité invalide dans la fonction get.");
    }
};
}
#endif // MANAGERS_H
