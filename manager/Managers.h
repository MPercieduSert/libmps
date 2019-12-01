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
        enum creationBdd{initiale,NbrCreationBdd};
    }


    //! Demande autorisation de modification
    enum DemandeAutorisationModificationBdd{
        Oui = 1,
        Non = 0
    };
}

namespace managerMPS {
/*! \ingroup groupeFile
 *  \brief Classe mère de la classe contenant les managers des entités pour la base de donnée.
 */
class Managers {
protected:
    using VersionBdd = entityMPS::VersionBdd;

    const szt m_nbrEntity;                          //!< Nombre d'entité associées à un managers de la base de donnée.
    QSqlQuery m_requete;                            //!< Requéte commune aux manageurs.
    std::vector<std::unique_ptr<AbstractManager>> m_managers;          //!< Tableau des manageurs.
    std::unique_ptr<ManagerSql<VersionBdd>> m_managerVersion;
    //!< Manager de l'entité version de la base de donnée.

public:
    //! Constructeur.
    Managers(szt nbrEntity, const QString & versionTable,
                     std::map<szt,std::unique_ptr<AbstractManager>> && managers = std::map<szt,std::unique_ptr<AbstractManager>>(),
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

    //! Retourne le manager des entités de ID, id.
    //virtual AbstractManager * get(int id) const = 0;
    AbstractManager & get(szt id) const {
        if(id < m_nbrEntity) {
            if(m_managers[id])
                return *m_managers[id];
            else
                throw std::invalid_argument(QString("Manager non initialisé: id : ").append(QString::number(id)).toStdString());
        }
        else
            throw std::invalid_argument("Identifiant d'entité invalide dans la fonction get.");
    }

    //! Méthode template permettant d'obtenir le manager correspondant à l'entité.
    template<class Ent> const AbstractManagerTemp<Ent> & get() const
        {return static_cast<const AbstractManagerTemp<Ent> &>(*m_managers[Ent::ID]);}

    //! Méthode template permettant d'obtenir le manager correspondant à l'entité.
    template<class Ent> AbstractManagerTemp<Ent> & get()
        {return static_cast<AbstractManagerTemp<Ent> &>(*m_managers[Ent::ID]);}

    //! Returne la version courante de la base de donnée.
    VersionBdd getVersion();

    //! Retourne le nombre d'entités associées à un manager dans la base de donnée.
    szt nbrEntity() const
        {return m_nbrEntity;}

    //! Retourne le numero de version de la base de donnée.
    int numVersion();

    //! Enregistre le numéro de version de la base de donnée.
    void saveVersion(int num);

    //! Modifie le pointeur vers l'objet requête.
    void setRequete(const QSqlQuery & req);

protected:
    //! Construit les informations d'un arbre pour la création des managers de type arbre.
    InfoBdd infoBddArbre(const QString & table) const;

    //! Mutateur de manager.
    template<class Ent> void setManager(std::unique_ptr<AbstractManagerTemp<Ent>> && manager)
        {m_managers[Ent::ID] = std::move(manager);}

    /*//! Retourne le manager des entités de ID, id sans vérification.
    virtual AbstractManager * getP(int id) const = 0;*/
};
/*
template<int NbrEntity> class Managers : public AbstractManagers
{
protected:
    std::array<AbstractManager *,NbrEntity> m_managers; //!< Tableau des manageurs.

public:
    //! Constructeur.
    Managers(const std::array<AbstractManager *,NbrEntity> & managers, const QSqlQuery & req = QSqlQuery())
        : AbstractManagers(req)
        {std::copy(managers.cbegin(),managers.cend(),m_managers.begin());}

    //! Destructeur.
    ~AbstractManagers()
    {
        for(std::array::const_iterator i = m_managers.cbegin(); i != m_managers.cend(); i++)
            delete *i;
    }

    //! Retourne le manager des entités de ID, id.
    AbstractManager * get(int id) const
    {
        if(id >= 0 and id < NbrEntity)
            return m_managers[id];
        else
            throw std::invalid_argument("Identifiant d'entité invalide dans la fonction get.");
    }

protected:
    //! Retourne le manager des entités de ID, id sans vérification.
    AbstractManager * getP(int id) const
        {return m_managers[id];}
}
*/
}
#endif // MANAGERS_H
