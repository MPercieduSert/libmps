/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACTLINKSQL_H
#define ABSTRACTLINKSQL_H

#include <array>
#include <QSqlQuery>
#include <stdexcept>
#include "ReqSql.h"
#include "Entity.h"

/*! \defgroup groupeLinkSql Liens
 * \ingroup groupeManager
 * \brief Ensemble des liens entre les entités de programation et les entités en base de donnée.
 */

/*! \ingroup groupeLinkSql
 * \brief Classe template frabriquant les liens avec la base de donnée à partir de l'ordre des attributs des entité.
 */
template<class Attribut, class AttPre, class AttSuiv, int Pos> class LinkSqlDichot;
template<class Attribut, int Pos = 0> using LinkSqlAttribut = LinkSqlDichot<Attribut, typename Attribut::AttPre, typename Attribut::AttSuiv, Pos>;

template<class Attribut, class AttPre, class AttSuiv, int Pos> class LinkSqlDichot :
        public LinkSqlAttribut<AttPre, Pos>,
        public LinkSqlAttribut<AttSuiv, AttPre::NbrAtt + Pos>
{
public:
    enum {PosPre = Pos, PosSuiv = AttPre::NbrAtt + Pos};
    using LinkPre = LinkSqlAttribut<AttPre, PosPre>;
    using LinkSuiv = LinkSqlAttribut<AttSuiv, PosSuiv>;

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(const Attribut & entity)
    {
        LinkPre::bindValues(entity);
        LinkSuiv::bindValues(entity);
    }

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(Attribut & entity)
    {
        LinkPre::bindValues(entity);
        LinkSuiv::bindValues(entity);
    }

    //! Hydrate l'entité entity avec à partir de la requète.
    void fromRequete(Attribut & entity) const
    {
        LinkPre::fromRequete(entity);
        LinkSuiv::fromRequete(entity);
    }
};

template<class Attribut, int Pos> class LinkSqlDichot<Attribut, NoAttribut, NoAttribut, Pos> : public virtual ReqSql
{
public:
    enum {PosPre = Pos};
    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(const Attribut & entity)
        {bindValue(PosPre - 1,entity.getToBdd());}

    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(Attribut & entity)
        {bindValue(PosPre - 1,entity.getToBdd());}

    //! Hydrate l'entité entity avec à partir de la requète.
    void fromRequete(Attribut & entity) const
        {entity.set(value<typename Attribut::AttType>(PosPre));}
};

template<> class LinkSqlDichot<Entity, NoAttribut, NoAttribut, 0> : public virtual ReqSql
{
public:
    enum {PosPre = 0};
    //! Transmet les valeurs des attributs à la requète SQL préparée.
    void bindValues(const Entity & /*entity*/)
        {}

    //! Hydrate l'entité entity avec à partir de la requète.
    void fromRequete(Entity & entity) const
        {entity.setId(value<Entity::AttType>(PosPre));}

    //! Accesseur de l'identifiant.
    int id(int pos = Entity::Id) const
        {return value<int>(pos);}

    //! Mutateur de l'identifiant.
    void setId(const Entity & entity, int pos = 0)
        {m_requete->bindValue(pos,entity.id());}
};

template<class Ent> class LinkSql : public LinkSqlAttribut<Ent>
{
public:
    using LinkSqlAttribut<Ent>::fromRequete;
    using LinkSqlAttribut<Ent>::id;
    CONSTR_DEFAUT(LinkSql)
    DESTR_VIDE(LinkSql)

    //! Crée dynamiquement une nouvelle entité de type Classe, l'hydrate à partir de la requète SQL.
    //! Puis retourne un  pointeur vers cette nouvelle entité.
    Ent * newFromRequete() const
    {
        Ent * entity = new Ent(id());
        fromRequete(*entity);
        return entity;
    }
};

#endif // ABSTRACTLINKSQL_H
