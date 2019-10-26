/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef ABSTRACTUNIQUESQL_H
#define ABSTRACTUNIQUESQL_H

/*! \defgroup groupeUniqueSql Conditions d'unicités
 * \ingroup groupeManager
 * \brief Ensemble des conditions d'unicités des entités.
 */

/*! \defgroup groupeUniqueSqlSpe Spéciation linkSql
 * \ingroup groupeUniqueSql
 * \brief Spéciations des conditions d'unicités.
 */

//#include "AbstractLinkSql.h"
//#include "AbstractManager.h"
#include "InfoBdd.h"
#include "ReqSql.h"

// Définition des conditions d'unicité des entités de type prédéfini.
//! \ingroup groupeUniqueSql
//! Macro définisant les constructeurs et destructeurs des conditions d'unicité.
#define CONSTR_DESTR_UNIQUE_SQL(ENTITY,TYPE) /*! \ingroup groupeUniqueSqlSpe */ \
    /*! \brief Conditions d'unicité de l'entité ENTITY. */ \
    class ENTITY ## UniqueSql: public TYPE<ENTITY>\
    {public:\
        /*! Construteur, transmettre en argument l'objet de requète utilisée par le manageur. */\
        ENTITY ## UniqueSql(const QString & table, const QVector<QMap<int,QString>> & att) : TYPE(table,att){}\
        /*! Destructeur.*/\
        ~ENTITY ## UniqueSql() {}

/*! \ingroup groupeUniqueSql
 * \brief Interface mère des classes conditions d'unicité.
 */
class AbstractUniqueSql : public ReqSql
{
public:
    enum {idUnique = 0};
    CONSTR_DEFAUT(AbstractUniqueSql)

    //! Destructeur.
    virtual ~AbstractUniqueSql() = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    virtual QString creer(const InfoBdd & info) const = 0;

    //! Ecrit les requètes d'unicité.
    virtual void initialise(const InfoBdd & info) =0;

    //! Construit la chaine coorespondant à la requête sql d'unicité
    QString uniqueString(const InfoBdd & info, int num = 0) const;
    
    //! Construit la chaine coorespondant à la requête sql de contraite d'unicité numeroté num + 1 si numero = true.
    QString uniqueCreerString(const InfoBdd & info, int num = 0, bool numero = false) const;

protected:
    using ReqSql::id;
    //! Accesseur de l'identifiant.
    unsigned id() const
        {return id(idUnique);}
};

/*! \ingroup groupeUniqueSql
 * \brief Interface mère des classes conditions d'unicité.
 */
template<class Ent> class AbstractUniqueSqlTemp : public AbstractUniqueSql
{
public:
    CONSTR_DEFAUT(AbstractUniqueSqlTemp)

    //! Destructeur.
    ~AbstractUniqueSqlTemp() override; //default

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    virtual bdd::ExisteUni existsUnique(Ent & entity) =0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    virtual QPair<bdd::ExisteUni,idt> existsUniqueId(const Ent & entity) = 0;
};

template<class Ent> AbstractUniqueSqlTemp<Ent>::~AbstractUniqueSqlTemp() {}

/*! \ingroup groupeUniqueSql
 * \brief Classe conditions d'unicité pour les entité ne possédant pas de condition d'unicité.
 */
template<class Ent> class NoUniqueSql : public AbstractUniqueSqlTemp<Ent>
{
public:
    CONSTR_DEFAUT(NoUniqueSql)

    //! Destructeur.
    ~NoUniqueSql() override = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    QString creer(const InfoBdd & /*info*/) const override
        {return QString();}

    //! Ecrit les requètes d'unicité.
    void initialise(const InfoBdd & info) override;

    //! L'attribut ne possède pas d'ensemble de valeur unique.
    bdd::ExisteUni existsUnique(Ent & /*entity*/) override
        {return bdd::ExisteUni::Aucun;}

    //! L'attribut ne possède pas d'ensemble de valeur unique.
    QPair<bdd::ExisteUni,idt> existsUniqueId(const Ent & /*entity*/) override
        {return QPair<bdd::ExisteUni,idt>();}
};

template<class Ent> void NoUniqueSql<Ent>::initialise(const InfoBdd & /*info*/) {}

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes condition d'unicité pour les entité possédant une condition d'unicité simple.
 */
template<class Ent> class SimpleUniqueSql : public AbstractUniqueSqlTemp<Ent>
{
protected:
    QString m_unique; //!< Requete Sql sur l'existence d'un ensemble d'attribus uniques.
    using AbstractUniqueSqlTemp<Ent>::typeAttributSqlString;
    using AbstractUniqueSqlTemp<Ent>::exec;
    using AbstractUniqueSqlTemp<Ent>::finish;
    using AbstractUniqueSqlTemp<Ent>::id;
    using AbstractUniqueSqlTemp<Ent>::next;
    using AbstractUniqueSqlTemp<Ent>::prepare;

public:
    using AbstractUniqueSqlTemp<Ent>::uniqueString;
    using AbstractUniqueSqlTemp<Ent>::uniqueCreerString;
    CONSTR_DEFAUT(SimpleUniqueSql)

    //! Destructeur.
    ~SimpleUniqueSql() override = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    QString creer(const InfoBdd & info) const override
        {return QString(",").append(uniqueCreerString(info));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a un seul ensemble de valeurs unique.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    bdd::ExisteUni existsUnique(Ent & entity) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a un seul ensemble de valeurs unique.
    QPair<bdd::ExisteUni,idt> existsUniqueId(const Ent & entity) override;

    //! Ecrit les requètes d'unicité.
    void initialise(const InfoBdd & info) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    virtual void bindValuesUnique(const Ent & entity) = 0;
};

template<class Ent> bdd::ExisteUni SimpleUniqueSql<Ent>::existsUnique(Ent & entity)
{
    prepare(m_unique);
    bindValuesUnique(entity);
    exec();
    if(next())
    {
        if(entity.id() == 0)
            entity.setId(id());
        else if(entity.id() != id())
        {
            finish();
            return bdd::Autre;
        }
        finish();
        return bdd::Tous;
    }
    else
    {
        finish();
        return bdd::Aucun;
    }
}

template<class Ent> QPair<bdd::ExisteUni,idt> SimpleUniqueSql<Ent>::existsUniqueId(const Ent & entity)
{
    QPair<bdd::ExisteUni,idt> resultat(bdd::ExisteUni::Aucun,0);
    prepare(m_unique);
    bindValuesUnique(entity);
    exec();
    if(next())
    {
        resultat.second = id();
        if(entity.id() == 0 || entity.id() == id())
            resultat.first = bdd::ExisteUni::Tous;
        else
            resultat.first = bdd::ExisteUni::Autre;
    }
    finish();
    return resultat;
}


template<class Ent> void SimpleUniqueSql<Ent>::initialise(const InfoBdd & info)
{
    m_unique = uniqueString(info);
    m_unique.squeeze();
}

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes conditions d'unicité pour les entité possédant deux conditions d'unicités.
 */
template<class Ent> class DoubleUniqueSql : public AbstractUniqueSqlTemp<Ent>
{
protected:
    QString m_unique_1; //!< Requete Sql sur l'existence de l'ensemble d'attribus uniques 1.
    QString m_unique_2; //!< Requete Sql sur l'existence de l'ensemble d'attribus uniques 2.
    using AbstractUniqueSqlTemp<Ent>::typeAttributSqlString;
    using AbstractUniqueSqlTemp<Ent>::exec;
    using AbstractUniqueSqlTemp<Ent>::finish;
    using AbstractUniqueSqlTemp<Ent>::id;
    using AbstractUniqueSqlTemp<Ent>::next;
    using AbstractUniqueSqlTemp<Ent>::prepare;

public:
    enum uniqueSet {UniqueSet1, UniqueSet2};
    using AbstractUniqueSqlTemp<Ent>::uniqueString;
    using AbstractUniqueSqlTemp<Ent>::uniqueCreerString;
    CONSTR_DEFAUT(DoubleUniqueSql)

    //! Destructeur.
    ~DoubleUniqueSql() override = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    QString creer(const InfoBdd & info) const override
        {return QString(",").append(uniqueCreerString(info,UniqueSet1))
                .append(",").append(uniqueCreerString(info,UniqueSet2));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    bdd::ExisteUni existsUnique(Ent & entity) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    QPair<bdd::ExisteUni,idt> existsUniqueId(const Ent & entity) override;

    //! Ecrit les requètes d'unicité.
    void initialise(const InfoBdd & info) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 1.
    virtual void bindValuesUnique_1(const Ent & entity) = 0;

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    virtual void bindValuesUnique_2(const Ent & entity) = 0;
};

template<class Ent> bdd::ExisteUni DoubleUniqueSql<Ent>::existsUnique(Ent & entity)
{
    unsigned idUni1 = 0;
    prepare(m_unique_1);
    bindValuesUnique_1(entity);
    exec();
    if(next())
        idUni1 = id();

    unsigned idUni2 = 0;
    prepare(m_unique_2);
    bindValuesUnique_2(entity);
    exec();
    if(next())
        idUni2 = id();
    finish();

    if(idUni1 == idUni2)
    {
        if(idUni1 == 0)
            return bdd::Aucun;
        else
        {
            if(entity.id() == 0)
                entity.setId(idUni1);
            else if(entity.id() != idUni1)
                return bdd::Autre;
            return bdd::Tous;
        }
    }
    else
    {
        if(idUni1 == 0)
        {
            if(entity.id() == 0)
                entity.setId(idUni2);
            else if(entity.id() != idUni2)
                return bdd::Autre;
            return bdd::Meme;
        }
        else
        {
            if(idUni2 == 0)
            {
                if(entity.id() == 0)
                    entity.setId(idUni1);
                else if(entity.id() != idUni1)
                    return bdd::Autre;
                return bdd::Meme;
            }
            else
                return bdd::Conflit;
        }
    }
}

template<class Ent> QPair<bdd::ExisteUni,idt> DoubleUniqueSql<Ent>::existsUniqueId(const Ent & entity)
{
    QPair<bdd::ExisteUni,idt> resultat(bdd::ExisteUni::Aucun,0);
    unsigned idUni1 = 0;
    prepare(m_unique_1);
    bindValuesUnique_1(entity);
    exec();
    if(next())
        idUni1 = id();

    unsigned idUni2 = 0;
    prepare(m_unique_2);
    bindValuesUnique_2(entity);
    exec();
    if(next())
        idUni2 = id();
    finish();

    if(idUni1 == idUni2)
    {
        if(idUni1 != 0)
        {
            resultat.second = idUni1;
            if(entity.id() == 0 || entity.id() == idUni1)
                resultat.first = bdd::ExisteUni::Tous;
            else
                resultat.first = bdd::ExisteUni::Autre;
        }
    }
    else
    {
        if(idUni1 == 0)
        {
            resultat.second = idUni2;
            if(entity.id() == 0 || entity.id() == idUni1)
                resultat.first = bdd::ExisteUni::Meme;
            else
                resultat.first = bdd::ExisteUni::Autre;
        }
        else
        {
            if(idUni2 == 0)
            {
                resultat.second = idUni1;
                if(entity.id() == 0 || entity.id() == idUni2)
                    resultat.first = bdd::ExisteUni::Meme;
                else
                    resultat.first = bdd::ExisteUni::Autre;
            }
            else
            {
                resultat.first = bdd::Conflit;
                resultat.second = idUni1;
            }
        }
    }
    return resultat;
}


template<class Ent> void DoubleUniqueSql<Ent>::initialise(const InfoBdd & info)
{
    m_unique_1 = uniqueString(info,UniqueSet1);
    m_unique_1.squeeze();
    m_unique_2 = uniqueString(info,UniqueSet2);
    m_unique_2.squeeze();
}

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes conditions d'unicité pour les entité possédant
 * deux clés avec exactement une non nulle et unique.
 */
template<class Ent> class RelationExactOneNotNullUniqueSql : public DoubleUniqueSql<Ent>
{
protected:
    using DoubleUniqueSql<Ent>::m_unique_1;
    using DoubleUniqueSql<Ent>::m_unique_2;
    using DoubleUniqueSql<Ent>::bindValue;
    using DoubleUniqueSql<Ent>::exec;
    using DoubleUniqueSql<Ent>::finish;
    using DoubleUniqueSql<Ent>::id;
    using DoubleUniqueSql<Ent>::next;
    using DoubleUniqueSql<Ent>::prepare;

public:
    enum {Id1Unique = 0,NbrUnique_1,Id2Unique = 0,NbrUnique_2,
          Id1UniqueSet = DoubleUniqueSql<Ent>::UniqueSet1,
          Id2UniqueSet = DoubleUniqueSql<Ent>::UniqueSet2
         };
    CONSTR_DEFAUT(RelationExactOneNotNullUniqueSql)

    //! Destructeur.
    ~RelationExactOneNotNullUniqueSql() override; //default

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    bdd::ExisteUni existsUnique(Ent & entity) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    QPair<bdd::ExisteUni,idt> existsUniqueId(const Ent & entity) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 1.
    void bindValuesUnique_1(const Ent &entity) override
        {bindValue(Id1Unique,entity.id1());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bindValuesUnique_2(const Ent &entity) override
        {bindValue(Id2Unique,entity.id2());}
};

template<class Ent> bdd::ExisteUni RelationExactOneNotNullUniqueSql<Ent>::existsUnique(Ent & entity)
{
    if(entity.id1() != 0 && entity.id2() == 0)
    {
        prepare(m_unique_1);
        bindValuesUnique_1(entity);
        exec();
        if(next())
        {
            entity.setId(id());
            finish();
            return bdd::Tous;
        }
        else
        {
            finish();
            return bdd::Aucun;
        }
    }

    if(entity.id1() == 0 && entity.id2() != 0)
    {
        prepare(m_unique_2);
        bindValuesUnique_2(entity);
        exec();
        if(next())
        {
            entity.setId(id());
            finish();
            return bdd::Tous;
        }
        else
        {
            finish();
            return bdd::Aucun;
        }
    }
    return bdd::Conflit;
}

template<class Ent> QPair<bdd::ExisteUni,idt> RelationExactOneNotNullUniqueSql<Ent>::existsUniqueId(const Ent & entity)
{
    QPair<bdd::ExisteUni,idt> resultat(bdd::ExisteUni::Aucun,0);
    if(entity.id1() != 0 && entity.id2() == 0)
    {
        prepare(m_unique_1);
        bindValuesUnique_1(entity);
        exec();
        if(next())
        {
            resultat.first = bdd::ExisteUni::Tous;
            resultat.second = id();
        }
        finish();
    }
    else if(entity.id1() == 0 && entity.id2() != 0)
    {
        prepare(m_unique_2);
        bindValuesUnique_2(entity);
        exec();
        if(next())
        {
            resultat.first = bdd::ExisteUni::Tous;
            resultat.second = id();
        }
        finish();
    }
    else
        resultat.first = bdd::Conflit;
    return resultat;
}

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes conditions d'unicité pour les entité possédant
 * deux clés avec exactement une non nulle et une condition
 * d'unicité sur la clé non nulle et d'autre attributs commums aux deux clés.
 */
template<class Ent> class AttsRelationExactOneNotNullUniqueSql : public RelationExactOneNotNullUniqueSql<Ent>
{
public:
    CONSTR_DEFAUT(AttsRelationExactOneNotNullUniqueSql)

    //! Destructeur.
    ~AttsRelationExactOneNotNullUniqueSql() override; //default

protected:
    //! Transmet les valeurs des attributs uniques aux requètes SQL préparées.
    virtual void bindValuesUnique(const Ent & entity) = 0;

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 1.
    void bindValuesUnique_1(const Ent &entity) override
    {
        RelationExactOneNotNullUniqueSql<Ent>::bindValuesUnique_1(entity);
        bindValuesUnique(entity);
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bindValuesUnique_2(const Ent &entity) override
    {
        RelationExactOneNotNullUniqueSql<Ent>::bindValuesUnique_2(entity);
        bindValuesUnique(entity);
    }
};

template<class Ent> AttsRelationExactOneNotNullUniqueSql<Ent>::~AttsRelationExactOneNotNullUniqueSql() {}
#endif // ABSTRACTUNIQUESQL_H
