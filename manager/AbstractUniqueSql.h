/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef ABSTRACTUNIQUESQL_H
#define ABSTRACTUNIQUESQL_H

/*! \defgroup groupeUniqueSql Conditions d'unicités
 * \ingroup groupeManager
 * \brief Ensemble des conditions d'unicités des entités.
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
        ENTITY ## UniqueSql(const QString & table, const std::vector<std::map<numt,QString>> & att) : TYPE(table,att){}\
        /*! Destructeur.*/\
        ~ENTITY ## UniqueSql() {}

namespace managerMPS {
/*! \ingroup groupeUniqueSql
 * \brief Interface mère des classes conditions d'unicité.
 */
class AbstractUniqueSql : public ReqSql {
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
    QString uniqueString(const InfoBdd & info, numt num = 0) const;
    
    //! Construit la chaine coorespondant à la requête sql de contraite d'unicité numeroté num + 1 si numero = true.
    QString uniqueCreerString(const InfoBdd & info, numt num = 0, bool numero = false) const;

protected:
    using ReqSql::id;
    //! Accesseur de l'identifiant.
    idt id() const
        {return id(idUnique);}
};

/*! \ingroup groupeUniqueSql
 * \brief Interface mère des classes conditions d'unicité.
 */
template<class Ent> class AbstractUniqueSqlTemp : public AbstractUniqueSql {
public:
    CONSTR_DEFAUT(AbstractUniqueSqlTemp)

    //! Destructeur.
    ~AbstractUniqueSqlTemp() override; //default

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    virtual bmps::existeUni existsUnique(Ent & entity) =0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    virtual std::pair<bmps::existeUni,idt> existsUniqueId(const Ent & entity) = 0;
};

template<class Ent> AbstractUniqueSqlTemp<Ent>::~AbstractUniqueSqlTemp() {}

/*! \ingroup groupeUniqueSql
 * \brief Classe conditions d'unicité pour les entité ne possédant pas de condition d'unicité.
 */
template<class Ent> class NoUniqueSql : public AbstractUniqueSqlTemp<Ent> {
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
    bmps::existeUni existsUnique(Ent & /*entity*/) override
        {return bmps::existeUni::Aucun;}

    //! L'attribut ne possède pas d'ensemble de valeur unique.
    std::pair<bmps::existeUni,idt> existsUniqueId(const Ent & /*entity*/) override
        {return std::pair<bmps::existeUni,idt>();}
};

template<class Ent> void NoUniqueSql<Ent>::initialise(const InfoBdd & /*info*/) {}

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes condition d'unicité pour les entité possédant une condition d'unicité simple.
 */
template<class Ent> class SimpleUniqueSql : public AbstractUniqueSqlTemp<Ent> {
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
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    bmps::existeUni existsUnique(Ent & entity) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a un seul ensemble de valeurs unique
    //! et renvoie l'identifiant du premier trouver.
    std::pair<bmps::existeUni,idt> existsUniqueId(const Ent & entity) override;

    //! Ecrit les requètes d'unicité.
    void initialise(const InfoBdd & info) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    virtual void bindValuesUnique(const Ent & entity) = 0;

    //! Execute la requète préparé puis teste l'identifiant de la requète avec l'entité.
    bmps::existeUni execUnique(Ent & entity) {
        exec();
        if(next()) {
            if(entity.isNew())
                entity.setId(id());
            else if(entity.id() != id()) {
                finish();
                return bmps::Autre;
            }
            finish();
            return bmps::Tous;
        }
        else {
            finish();
            return bmps::Aucun;
        }
    }

    //! Execute la requète préparé puis teste l'identifiant de la requète avec l'entité.
    std::pair<bmps::existeUni,idt> execUniqueId(const Ent & entity) {
        std::pair<bmps::existeUni,idt> resultat(bmps::existeUni::Aucun,0);
        exec();
        if(next()) {
            resultat.second = id();
            if(entity.isNew() || entity.id() == resultat.second)
                resultat.first = bmps::existeUni::Tous;
            else
                resultat.first = bmps::existeUni::Autre;
        }
        finish();
        return resultat;
    }

    //! Retourne l'identifiant corespondant la première requête d'unicité.
    idt idUnique(const Ent & entity) {
        prepare(m_unique);
        bindValuesUnique(entity);
        exec();
        return next()? id() : 0;
    }
};

template<class Ent> bmps::existeUni SimpleUniqueSql<Ent>::existsUnique(Ent & entity) {
    prepare(m_unique);
    bindValuesUnique(entity);
    return execUnique(entity);
}

template<class Ent> std::pair<bmps::existeUni,idt> SimpleUniqueSql<Ent>::existsUniqueId(const Ent & entity) {
    prepare(m_unique);
    bindValuesUnique(entity);
    return execUniqueId(entity);
}


template<class Ent> void SimpleUniqueSql<Ent>::initialise(const InfoBdd & info) {
    m_unique = uniqueString(info);
    m_unique.squeeze();
}

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes conditions d'unicité pour les entité possédant deux conditions d'unicités.
 */
template<class Ent> class DoubleUniqueSql : public SimpleUniqueSql<Ent> {
protected:
    QString m_unique_2; //!< Requete Sql sur l'existence de l'ensemble d'attribus uniques 2.

    using SimpleUniqueSql<Ent>::m_unique;
    using SimpleUniqueSql<Ent>::exec;
    using SimpleUniqueSql<Ent>::finish;
    using SimpleUniqueSql<Ent>::id;
    using SimpleUniqueSql<Ent>::idUnique;
    using SimpleUniqueSql<Ent>::next;
    using SimpleUniqueSql<Ent>::prepare;
    using SimpleUniqueSql<Ent>::uniqueCreerString;
    using SimpleUniqueSql<Ent>::uniqueString;

public:
    enum {UniqueSet1, UniqueSet2, NbrUniqueSet};
    CONSTR_DEFAUT(DoubleUniqueSql)

    //! Destructeur.
    ~DoubleUniqueSql() override = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    QString creer(const InfoBdd & info) const override
        {return SimpleUniqueSql<Ent>::creer(info).append(",").append(uniqueCreerString(info,UniqueSet2));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    bmps::existeUni existsUnique(Ent & entity) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques
    //! et renvoie l'identifiant du premier trouver.
    std::pair<bmps::existeUni,idt> existsUniqueId(const Ent & entity) override;

    //! Ecrit les requètes d'unicité.
    void initialise(const InfoBdd & info) override;

protected:
    //! Modifie l'identifiant de l'entité si elle est nouvelle sinon teste les identifiants.
    bmps::existeUni aucunAutreOuMeme(Ent & entity, idt idUni) const {
        if(idUni == 0)
            return bmps::Aucun;
        else {
            if(entity.isNew()) {
                entity.setId(idUni);
                return bmps::Tous;
            }
            else if(entity.id() != idUni)
                return bmps::Autre;
            return bmps::Tous;
        }
    }

    //! Teste les identifiants.
    std::pair<bmps::existeUni,idt> aucunAutreOuMemeId(const Ent & entity, idt idUni) const {
        if(idUni != 0)
            return std::pair<bmps::existeUni,idt>((entity.isNew() || entity.id() == idUni)?
                                                  bmps::existeUni::Tous :
                                                  bmps::existeUni::Autre,
                                              idUni);
        return std::pair<bmps::existeUni,idt>(bmps::existeUni::Aucun,0);
    }

    //! Modifie l'identifiant de l'entité si elle est nouvelle sinon teste les identifiants.
    bmps::existeUni autreOuMeme(Ent & entity, idt idUni) const {
        if(entity.isNew()) {
            entity.setId(idUni);
            return bmps::Meme;
        }
        else if(entity.id() == idUni)
            return bmps::Meme;
        return bmps::Autre;
    }

    //! Teste les identifiants.
    std::pair<bmps::existeUni,idt> autreOuMemeId(const Ent & entity, idt idUni) const {
        return std::pair<bmps::existeUni,idt>((entity.isNew() || entity.id() == idUni)?
                                       bmps::existeUni::Meme :
                                       bmps::existeUni::Autre,
                                   idUni);
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    virtual void bindValuesUnique_2(const Ent & entity) = 0;

    //! Retourne l'identifiant corespondant la seconde requête d'unicité.
    idt id2Unique(const Ent & entity) {
        prepare(m_unique_2);
        bindValuesUnique_2(entity);
        exec();
        return next()? id() : 0;
    }
};

template<class Ent> bmps::existeUni DoubleUniqueSql<Ent>::existsUnique(Ent & entity)
{
    auto idUni1 = idUnique(entity);
    auto idUni2 = id2Unique(entity);
    finish();

    if(idUni1 == idUni2)
        return aucunAutreOuMeme(entity, idUni1);
    else {
        if(idUni1 == 0)
            return autreOuMeme(entity, idUni2);
        else {
            if(idUni2 == 0)
                return autreOuMeme(entity, idUni1);
            else
                return bmps::Conflit;
        }
    }
}

template<class Ent> std::pair<bmps::existeUni,idt> DoubleUniqueSql<Ent>::existsUniqueId(const Ent & entity)
{
    auto idUni1 = idUnique(entity);
    auto idUni2 = id2Unique(entity);
    finish();

    if(idUni1 == idUni2)
        return aucunAutreOuMemeId(entity, idUni1);
    else {
        if(idUni1 == 0)
            return autreOuMemeId(entity, idUni2);
        else {
            if(idUni2 == 0)
                return autreOuMemeId(entity, idUni1);
            else
                return std::pair<bmps::existeUni,idt>(bmps::Conflit,
                                                  entity.id() != idUni1 ?
                                                        idUni1 :
                                                        idUni2);
        }
    }
}

template<class Ent> void DoubleUniqueSql<Ent>::initialise(const InfoBdd & info) {
    SimpleUniqueSql<Ent>::initialise(info);
    m_unique_2 = uniqueString(info,UniqueSet2);
    m_unique_2.squeeze();
}

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes conditions d'unicité pour les entité possédant
 * deux clés avec exactement une non nulle et unique.
 */
template<class Ent> class RelationExactOneNotNullUniqueSql : public virtual DoubleUniqueSql<Ent> {
protected:
    using DoubleUniqueSql<Ent>::m_unique;
    using DoubleUniqueSql<Ent>::m_unique_2;
    using DoubleUniqueSql<Ent>::bindValue;
    using DoubleUniqueSql<Ent>::exec;
    using DoubleUniqueSql<Ent>::execUnique;
    using DoubleUniqueSql<Ent>::execUniqueId;
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
    ~RelationExactOneNotNullUniqueSql() override = default; //default

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    bmps::existeUni existsUnique(Ent & entity) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques
    //! et renvoie l'identifiant du premier trouver.
    std::pair<bmps::existeUni,idt> existsUniqueId(const Ent & entity) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 1.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(Id1Unique,entity.id1());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bindValuesUnique_2(const Ent &entity) override
        {bindValue(Id2Unique,entity.id2());}
};

template<class Ent> bmps::existeUni RelationExactOneNotNullUniqueSql<Ent>::existsUnique(Ent & entity) {
    if(entity.id2() == 0) {
        if(entity.id1() != 0) {
            prepare(m_unique);
            bindValuesUnique(entity);
            return execUnique(entity);
        }
    }
    else if(entity.id1() == 0) {
        prepare(m_unique_2);
        bindValuesUnique_2(entity);
        return execUnique(entity);
    }
    return bmps::Conflit;
}

template<class Ent> std::pair<bmps::existeUni,idt> RelationExactOneNotNullUniqueSql<Ent>::existsUniqueId(const Ent & entity) {
    if(entity.id2() == 0) {
        if(entity.id1() != 0) {
            prepare(m_unique);
            bindValuesUnique(entity);
            return execUniqueId(entity);
        }
        else
            return std::pair<bmps::existeUni,idt>(bmps::existeUni::Aucun,0);
    }
    else if(entity.id1() == 0) {
        prepare(m_unique_2);
        bindValuesUnique_2(entity);
        return execUniqueId(entity);
    }
    else
        return std::pair<bmps::existeUni,idt>(bmps::existeUni::Conflit,0);
}

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes conditions d'unicité pour les entité possédant
 * deux clés avec exactement une non nulle et une condition
 * d'unicité sur la clé non nulle et d'autre attributs commums aux deux clés.
 */
template<class Ent> class AttsRelationExactOneNotNullUniqueSql : public RelationExactOneNotNullUniqueSql<Ent> {
public:
    CONSTR_DEFAUT(AttsRelationExactOneNotNullUniqueSql)

    //! Destructeur.
    ~AttsRelationExactOneNotNullUniqueSql() override; //default

protected:
    //! Transmet les valeurs des attributs uniques aux requètes SQL préparées.
    virtual void bindValuesUniqueAtts(const Ent & entity) = 0;

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 1.
    void bindValuesUnique(const Ent &entity) override {
        RelationExactOneNotNullUniqueSql<Ent>::bindValuesUnique(entity);
        bindValuesUniqueAtts(entity);
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bindValuesUnique_2(const Ent &entity) override {
        RelationExactOneNotNullUniqueSql<Ent>::bindValuesUnique_2(entity);
        bindValuesUniqueAtts(entity);
    }
};

template<class Ent> AttsRelationExactOneNotNullUniqueSql<Ent>::~AttsRelationExactOneNotNullUniqueSql() = default;

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes conditions d'unicité pour les entité possédant trois conditions d'unicités.
 */
template<class Ent> class TripleUniqueSql : public virtual DoubleUniqueSql<Ent> {
protected:
    QString m_unique_3; //!< Requete Sql sur l'existence de l'ensemble d'attribus uniques 2.

    using DoubleUniqueSql<Ent>::m_unique;
    using DoubleUniqueSql<Ent>::m_unique_2;
    using DoubleUniqueSql<Ent>::aucunAutreOuMeme;
    using DoubleUniqueSql<Ent>::aucunAutreOuMemeId;
    using DoubleUniqueSql<Ent>::autreOuMeme;
    using DoubleUniqueSql<Ent>::autreOuMemeId;
    using DoubleUniqueSql<Ent>::exec;
    using DoubleUniqueSql<Ent>::execUnique;
    using DoubleUniqueSql<Ent>::execUniqueId;
    using DoubleUniqueSql<Ent>::finish;
    using DoubleUniqueSql<Ent>::id;
    using DoubleUniqueSql<Ent>::idUnique;
    using DoubleUniqueSql<Ent>::id2Unique;
    using DoubleUniqueSql<Ent>::next;
    using DoubleUniqueSql<Ent>::prepare;
    using DoubleUniqueSql<Ent>::uniqueCreerString;
    using DoubleUniqueSql<Ent>::uniqueString;

public:
    enum uniqueSet {UniqueSet1, UniqueSet2, UniqueSet3 = DoubleUniqueSql<Ent>::NbrUniqueSet};
    CONSTR_DEFAUT(TripleUniqueSql)

    //! Destructeur.
    ~TripleUniqueSql() override = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    QString creer(const InfoBdd & info) const override
        {return DoubleUniqueSql<Ent>::creer(info).append(",").append(uniqueCreerString(info,UniqueSet3));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement trois ensembles de valeurs uniques.
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    bmps::existeUni existsUnique(Ent & entity) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement trois ensembles de valeurs uniques
    //! et renvoie l'identifiant du premier trouver.
    std::pair<bmps::existeUni,idt> existsUniqueId(const Ent & entity) override;

    //! Ecrit les requètes d'unicité.
    void initialise(const InfoBdd & info) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    virtual void bindValuesUnique_3(const Ent & entity) = 0;

    //! Retourne l'identifiant corespondant la troisième requête d'unicité.
    idt id3Unique(const Ent & entity) {
        prepare(m_unique_3);
        bindValuesUnique_3(entity);
        exec();
        return next()? id() : 0;
    }
};

template<class Ent> bmps::existeUni TripleUniqueSql<Ent>::existsUnique(Ent & entity) {
    auto idUni1 = idUnique(entity);
    auto idUni2 = id2Unique(entity);
    auto idUni3 = id3Unique(entity);
    finish();

    if(idUni1 == idUni2 && idUni1 == idUni3)
        return aucunAutreOuMeme(entity, idUni1);
    else {
        if(idUni1 == 0) {
            if(idUni2 == 0 || idUni2 == idUni3)
                return autreOuMeme(entity, idUni3);
            else if(idUni3 == 0)
                return autreOuMeme(entity, idUni2);
            else
                return bmps::Conflit;
        }
        else {
            if((idUni2 == 0 && idUni3 == idUni1)
                    || (idUni2 == idUni1 && idUni3 == 0))
                return autreOuMeme(entity, idUni1);
            else
                return bmps::Conflit;
        }
    }
}

template<class Ent> std::pair<bmps::existeUni,idt> TripleUniqueSql<Ent>::existsUniqueId(const Ent & entity) {
    auto idUni1 = idUnique(entity);
    auto idUni2 = id2Unique(entity);
    auto idUni3 = id3Unique(entity);
    finish();

    if(idUni1 == idUni2 && idUni1 == idUni3)
        return aucunAutreOuMemeId(entity, idUni1);
    else {
        if(idUni1 == 0) {
            if(idUni2 == 0 || idUni2 == idUni3)
                return autreOuMemeId(entity, idUni3);
            else if(idUni3 == 0)
                return autreOuMemeId(entity, idUni2);
            else
                return std::pair<bmps::existeUni,idt>(bmps::Conflit,
                                                  entity.id() != idUni2 ?
                                                        idUni2 :
                                                        idUni3);
        }
        else {
            if((idUni2 == 0 && idUni3 == idUni1)
                    || (idUni2 == idUni1 && idUni3 == 0))
                return autreOuMemeId(entity, idUni1);
            else {
                std::pair<bmps::existeUni,idt> resultat(bmps::existeUni::Conflit,0);
                if(entity.id() != idUni1)
                    resultat.second = idUni1;
                else if(idUni2 != 0)
                    resultat.second = idUni2;
                else
                    resultat.second = idUni3;
                return resultat;
            }
        }
    }
}

template<class Ent> void TripleUniqueSql<Ent>::initialise(const InfoBdd & info) {
    DoubleUniqueSql<Ent>::initialise(info);
    m_unique_3 = uniqueString(info,UniqueSet3);
    m_unique_3.squeeze();
}

/*! \ingroup groupeUniqueSql
 * \brief Classe mère des classes conditions d'unicité pour les entité possédant
 * trois clés avec exactement une non nulle et unique.
 */
template<class Ent> class TripleExactOneNotNullUniqueSql : public TripleUniqueSql<Ent>, RelationExactOneNotNullUniqueSql<Ent> {
protected:
    using DoubleUniqueSql<Ent>::m_unique;
    using DoubleUniqueSql<Ent>::m_unique_2;
    using TripleUniqueSql<Ent>::m_unique_3;
    using DoubleUniqueSql<Ent>::bindValue;
    using DoubleUniqueSql<Ent>::exec;
    using DoubleUniqueSql<Ent>::execUnique;
    using DoubleUniqueSql<Ent>::execUniqueId;
    using DoubleUniqueSql<Ent>::finish;
    using DoubleUniqueSql<Ent>::id;
    using DoubleUniqueSql<Ent>::next;
    using DoubleUniqueSql<Ent>::prepare;

public:
    enum {Id1Unique = 0, NbrUnique_1,
          Id2Unique = 0, NbrUnique_2,
          Id3Unique = 0, NbrUnique_3,
          Id1UniqueSet = TripleUniqueSql<Ent>::UniqueSet1,
          Id2UniqueSet = TripleUniqueSql<Ent>::UniqueSet2,
          Id3UniqueSet = TripleUniqueSql<Ent>::UniqueSet3
         };
    CONSTR_DEFAUT(TripleExactOneNotNullUniqueSql)

    //! Destructeur.
    ~TripleExactOneNotNullUniqueSql() override = default; //default

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    bmps::existeUni existsUnique(Ent & entity) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques
    //! et renvoie l'identifiant du premier trouver.
    std::pair<bmps::existeUni,idt> existsUniqueId(const Ent & entity) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 1.
    void bindValuesUnique(const Ent &entity) override
        {bindValue(Id1Unique,entity.id1());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bindValuesUnique_2(const Ent &entity) override
        {bindValue(Id2Unique,entity.id2());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bindValuesUnique_3(const Ent &entity) override
        {bindValue(Id3Unique,entity.id3());}
};

template<class Ent> bmps::existeUni TripleExactOneNotNullUniqueSql<Ent>::existsUnique(Ent & entity)
{
    if(entity.id3() == 0)
        return RelationExactOneNotNullUniqueSql<Ent>::existsUnique(entity);
    else if(entity.id2() == 0 && entity.id1() == 0)
    {
        prepare(m_unique_3);
        bindValuesUnique_3(entity);
        return execUnique(entity);
    }
    return bmps::Conflit;
}

template<class Ent> std::pair<bmps::existeUni,idt> TripleExactOneNotNullUniqueSql<Ent>::existsUniqueId(const Ent & entity) {
    if(entity.id3() == 0)
        return RelationExactOneNotNullUniqueSql<Ent>::existsUniqueId(entity);
    else if(entity.id2() == 0 && entity.id1() == 0) {
        prepare(m_unique_3);
        bindValuesUnique_3(entity);
        return execUniqueId(entity);
    }
    else
        return std::pair<bmps::existeUni,idt>(bmps::existeUni::Conflit,0);;
}
}
#endif // ABSTRACTUNIQUESQL_H
