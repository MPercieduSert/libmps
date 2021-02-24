/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/08/2018
 */
#ifndef ABSTRACT_UNIQUE_SQL_H
#define ABSTRACT_UNIQUE_SQL_H

#include "info_bdd.h"
#include "req_sql.h"

/*! \defgroup groupe_unique_sql Conditions d'unicités
 *\ingroup groupe_manager
 *\brief Ensemble des conditions d'unicités des entités.
 */

// Définition des conditions d'unicité des entités de type prédéfini.
//! \ingroup groupe_unique_sql
//! Macro définisant les constructeurs et destructeurs des conditions d'unicité.
#define CONSTR_DESTR_UNIQUE_SQL(ENTITY,TYPE) /*! \ingroup groupe_unique_sqlSpe */ \
    /*! \brief Conditions d'unicité de l'entité ENTITY. */ \
    class ENTITY ## _unique_sql: public TYPE<ENTITY>\
    {public:\
        /*! Construteur, transmettre en argument l'objet de requète utilisée par le manageur. */\
        ENTITY ## _unique_sql(const QString &table, const std::vector<std::map<numt,QString>> &att) : TYPE(table,att){}\
        /*! Destructeur.*/\
        ~ENTITY ## _unique_sql() {}

namespace mps {
namespace manager {
/*! \ingroup groupe_unique_sql
 *\brief Interface mère des classes conditions d'unicité.
 */
class abstract_unique_sql : public req_sql {
public:
    enum {Id_Unique = 0};
    CONSTR_DEFAUT(abstract_unique_sql)

    //! Destructeur.
    virtual ~abstract_unique_sql() = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    virtual QString creer(const info_bdd &info) const = 0;

    //! Ecrit les requètes d'unicité.
    virtual void initialise(const info_bdd &info) =0;

    //! Construit la chaine coorespondant à la requête sql d'unicité
    QString unique_string(const info_bdd &info, numt num = 0) const;
    
    //! Construit la chaine coorespondant à la requête sql de contraite d'unicité numeroté num + 1 si numero = true.
    QString unique_creer_string(const info_bdd &info, numt num = 0, bool numero = false) const;

protected:
    using req_sql::id;
    //! Accesseur de l'identifiant.
    idt id() const
        {return id(Id_Unique);}
};

/*! \ingroup groupe_unique_sql
 *\brief Interface mère des classes conditions d'unicité.
 */
template<class Ent> class abstract_unique_sql_temp : public abstract_unique_sql {
public:
    CONSTR_DEFAUT(abstract_unique_sql_temp)

    //! Destructeur.
    ~abstract_unique_sql_temp() override; //default

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    virtual b2d::existe_uni exists_unique(Ent &ent) =0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    virtual std::pair<b2d::existe_uni,idt> exists_unique_id(const Ent &ent) = 0;
};

template<class Ent> abstract_unique_sql_temp<Ent>::~abstract_unique_sql_temp() {}

/*! \ingroup groupe_unique_sql
 *\brief Classe conditions d'unicité pour les entité ne possédant pas de condition d'unicité.
 */
template<class Ent> class no_unique_sql : public abstract_unique_sql_temp<Ent> {
public:
    CONSTR_DEFAUT(no_unique_sql)

    //! Destructeur.
    ~no_unique_sql() override = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    QString creer(const info_bdd &/*info*/) const override
        {return QString();}

    //! Ecrit les requètes d'unicité.
    void initialise(const info_bdd &info) override;

    //! L'attribut ne possède pas d'ensemble de valeur unique.
    b2d::existe_uni exists_unique(Ent &/*ent*/) override
        {return b2d::existe_uni::Aucun;}

    //! L'attribut ne possède pas d'ensemble de valeur unique.
    std::pair<b2d::existe_uni,idt> exists_unique_id(const Ent &/*ent*/) override
        {return std::pair<b2d::existe_uni,idt>();}
};

template<class Ent> void no_unique_sql<Ent>::initialise(const info_bdd &/*info*/) {}

/*! \ingroup groupe_unique_sql
 *\brief Classe mère des classes condition d'unicité pour les entité possédant une condition d'unicité simple.
 */
template<class Ent> class simple_unique_sql : public abstract_unique_sql_temp<Ent> {
protected:
    QString m_unique; //!< Requete Sql sur l'existence d'un ensemble d'attribus uniques.
    using abstract_unique_sql_temp<Ent>::type_attribut_sql_string;
    using abstract_unique_sql_temp<Ent>::exec;
    using abstract_unique_sql_temp<Ent>::finish;
    using abstract_unique_sql_temp<Ent>::id;
    using abstract_unique_sql_temp<Ent>::next;
    using abstract_unique_sql_temp<Ent>::prepare;

public:
    using abstract_unique_sql_temp<Ent>::unique_string;
    using abstract_unique_sql_temp<Ent>::unique_creer_string;
    CONSTR_DEFAUT(simple_unique_sql)

    //! Destructeur.
    ~simple_unique_sql() override = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    QString creer(const info_bdd &info) const override
        {return QString(",").append(unique_creer_string(info));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a un seul ensemble de valeurs unique.
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    b2d::existe_uni exists_unique(Ent &ent) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a un seul ensemble de valeurs unique
    //! et renvoie l'identifiant du premier trouver.
    std::pair<b2d::existe_uni,idt> exists_unique_id(const Ent &ent) override;

    //! Ecrit les requètes d'unicité.
    void initialise(const info_bdd &info) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée.
    virtual void bind_values_unique(const Ent &ent) = 0;

    //! Execute la requète préparé puis teste l'identifiant de la requète avec l'entité.
    b2d::existe_uni exec_unique(Ent &ent) {
        exec();
        if(next()) {
            if(ent.is_new())
                ent.set_id(id());
            else if(ent.id() != id()) {
                finish();
                return b2d::Autre;
            }
            finish();
            return b2d::Tous;
        }
        else {
            finish();
            return b2d::Aucun;
        }
    }

    //! Execute la requète préparé puis teste l'identifiant de la requète avec l'entité.
    std::pair<b2d::existe_uni,idt> exec_unique_id(const Ent &ent) {
        std::pair<b2d::existe_uni,idt> resultat(b2d::existe_uni::Aucun,0);
        exec();
        if(next()) {
            resultat.second = id();
            if(ent.is_new() || ent.id() == resultat.second)
                resultat.first = b2d::existe_uni::Tous;
            else
                resultat.first = b2d::existe_uni::Autre;
        }
        finish();
        return resultat;
    }

    //! Retourne l'identifiant corespondant la première requête d'unicité.
    idt id_unique(const Ent &ent) {
        prepare(m_unique);
        bind_values_unique(ent);
        exec();
        return next()? id() : 0;
    }
};

template<class Ent> b2d::existe_uni simple_unique_sql<Ent>::exists_unique(Ent &ent) {
    prepare(m_unique);
    bind_values_unique(ent);
    return exec_unique(ent);
}

template<class Ent> std::pair<b2d::existe_uni,idt> simple_unique_sql<Ent>::exists_unique_id(const Ent &ent) {
    prepare(m_unique);
    bind_values_unique(ent);
    return exec_unique_id(ent);
}

template<class Ent> void simple_unique_sql<Ent>::initialise(const info_bdd &info) {
    m_unique = unique_string(info);
    m_unique.squeeze();
}

/*! \ingroup groupe_unique_sql
 *\brief Classe mère des classes conditions d'unicité pour les entité possédant deux conditions d'unicités.
 */
template<class Ent> class double_unique_sql : public simple_unique_sql<Ent> {
protected:
    QString m_unique_2; //!< Requete Sql sur l'existence de l'ensemble d'attribus uniques 2.

    using simple_unique_sql<Ent>::m_unique;
    using simple_unique_sql<Ent>::exec;
    using simple_unique_sql<Ent>::finish;
    using simple_unique_sql<Ent>::id;
    using simple_unique_sql<Ent>::id_unique;
    using simple_unique_sql<Ent>::next;
    using simple_unique_sql<Ent>::prepare;
    using simple_unique_sql<Ent>::unique_creer_string;
    using simple_unique_sql<Ent>::unique_string;

public:
    enum {Unique_Set_1, Unique_Set_2, Nbr_Unique_Set};
    CONSTR_DEFAUT(double_unique_sql)

    //! Destructeur.
    ~double_unique_sql() override = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    QString creer(const info_bdd &info) const override
        {return simple_unique_sql<Ent>::creer(info).append(",").append(unique_creer_string(info,Unique_Set_2));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    b2d::existe_uni exists_unique(Ent &ent) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques
    //! et renvoie l'identifiant du premier trouver.
    std::pair<b2d::existe_uni,idt> exists_unique_id(const Ent &ent) override;

    //! Ecrit les requètes d'unicité.
    void initialise(const info_bdd &info) override;

protected:
    //! Modifie l'identifiant de l'entité si elle est nouvelle sinon teste les identifiants.
    b2d::existe_uni aucun_autre_ou_meme(Ent &ent, idt id_uni) const {
        if(id_uni == 0)
            return b2d::Aucun;
        else {
            if(ent.is_new()) {
                ent.set_id(id_uni);
                return b2d::Tous;
            }
            else if(ent.id() != id_uni)
                return b2d::Autre;
            return b2d::Tous;
        }
    }

    //! Teste les identifiants.
    std::pair<b2d::existe_uni,idt> aucun_autre_ou_meme_id(const Ent &ent, idt id_uni) const {
        if(id_uni != 0)
            return std::pair<b2d::existe_uni,idt>((ent.is_new() || ent.id() == id_uni)?
                                                  b2d::existe_uni::Tous :
                                                  b2d::existe_uni::Autre,
                                              id_uni);
        return std::pair<b2d::existe_uni,idt>(b2d::existe_uni::Aucun,0);
    }

    //! Modifie l'identifiant de l'entité si elle est nouvelle sinon teste les identifiants.
    b2d::existe_uni autre_ou_meme(Ent &ent, idt id_uni) const {
        if(ent.is_new()) {
            ent.set_id(id_uni);
            return b2d::Meme;
        }
        else if(ent.id() == id_uni)
            return b2d::Meme;
        return b2d::Autre;
    }

    //! Teste les identifiants.
    std::pair<b2d::existe_uni,idt> autre_ou_meme_id(const Ent &ent, idt id_uni) const {
        return std::pair<b2d::existe_uni,idt>((ent.is_new() || ent.id() == id_uni)?
                                       b2d::existe_uni::Meme :
                                       b2d::existe_uni::Autre,
                                   id_uni);
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    virtual void bind_values_unique_2(const Ent &ent) = 0;

    //! Retourne l'identifiant corespondant la seconde requête d'unicité.
    idt id_2_unique(const Ent &ent) {
        prepare(m_unique_2);
        bind_values_unique_2(ent);
        exec();
        return next()? id() : 0;
    }
};

template<class Ent> b2d::existe_uni double_unique_sql<Ent>::exists_unique(Ent &ent)
{
    auto id_uni_1 = id_unique(ent);
    auto id_uni_2 = id_2_unique(ent);
    finish();

    if(id_uni_1 == id_uni_2)
        return aucun_autre_ou_meme(ent, id_uni_1);
    else {
        if(id_uni_1 == 0)
            return autre_ou_meme(ent, id_uni_2);
        else {
            if(id_uni_2 == 0)
                return autre_ou_meme(ent, id_uni_1);
            else
                return b2d::Conflit;
        }
    }
}

template<class Ent> std::pair<b2d::existe_uni,idt> double_unique_sql<Ent>::exists_unique_id(const Ent &ent)
{
    auto id_uni_1 = id_unique(ent);
    auto id_uni_2 = id_2_unique(ent);
    finish();

    if(id_uni_1 == id_uni_2)
        return aucun_autre_ou_meme_id(ent, id_uni_1);
    else {
        if(id_uni_1 == 0)
            return autre_ou_meme_id(ent, id_uni_2);
        else {
            if(id_uni_2 == 0)
                return autre_ou_meme_id(ent, id_uni_1);
            else
                return std::pair<b2d::existe_uni,idt>(b2d::Conflit,
                                                  ent.id() != id_uni_1 ? id_uni_1 : id_uni_2);
        }
    }
}

template<class Ent> void double_unique_sql<Ent>::initialise(const info_bdd &info) {
    simple_unique_sql<Ent>::initialise(info);
    m_unique_2 = unique_string(info,Unique_Set_2);
    m_unique_2.squeeze();
}

/*! \ingroup groupe_unique_sql
 *\brief Classe mère des classes conditions d'unicité pour les entité possédant
 *deux clés avec exactement une non nulle et unique.
 */
template<class Ent> class relation_exact_one_not_null_unique_sql : public virtual double_unique_sql<Ent> {
protected:
    using double_unique_sql<Ent>::m_unique;
    using double_unique_sql<Ent>::m_unique_2;
    using double_unique_sql<Ent>::bind_value;
    using double_unique_sql<Ent>::exec;
    using double_unique_sql<Ent>::exec_unique;
    using double_unique_sql<Ent>::exec_unique_id;
    using double_unique_sql<Ent>::finish;
    using double_unique_sql<Ent>::id;
    using double_unique_sql<Ent>::next;
    using double_unique_sql<Ent>::prepare;

public:
    enum {Id_1_Unique = 0,Nbr_Unique_1,Id_2_Unique = 0,Nbr_Unique_2,
          Id_1_Unique_Set = double_unique_sql<Ent>::Unique_Set_1,
          Id_2_Unique_Set = double_unique_sql<Ent>::Unique_Set_2
         };
    CONSTR_DEFAUT(relation_exact_one_not_null_unique_sql)

    //! Destructeur.
    ~relation_exact_one_not_null_unique_sql() override = default; //default

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    b2d::existe_uni exists_unique(Ent &ent) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques
    //! et renvoie l'identifiant du premier trouver.
    std::pair<b2d::existe_uni,idt> exists_unique_id(const Ent &ent) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 1.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Id_1_Unique,ent.id_1());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bind_values_unique_2(const Ent &ent) override
        {bind_value(Id_2_Unique,ent.id_2());}
};

template<class Ent> b2d::existe_uni relation_exact_one_not_null_unique_sql<Ent>::exists_unique(Ent &ent) {
    if(ent.id_2() == 0) {
        if(ent.id_1() != 0) {
            prepare(m_unique);
            bind_values_unique(ent);
            return exec_unique(ent);
        }
    }
    else if(ent.id_1() == 0) {
        prepare(m_unique_2);
        bind_values_unique_2(ent);
        return exec_unique(ent);
    }
    return b2d::Conflit;
}

template<class Ent> std::pair<b2d::existe_uni,idt> relation_exact_one_not_null_unique_sql<Ent>::exists_unique_id(const Ent &ent) {
    if(ent.id_2() == 0) {
        if(ent.id_1() != 0) {
            prepare(m_unique);
            bind_values_unique(ent);
            return exec_unique_id(ent);
        }
        else
            return std::pair<b2d::existe_uni,idt>(b2d::existe_uni::Aucun,0);
    }
    else if(ent.id_1() == 0) {
        prepare(m_unique_2);
        bind_values_unique_2(ent);
        return exec_unique_id(ent);
    }
    else
        return std::pair<b2d::existe_uni,idt>(b2d::existe_uni::Conflit,0);
}

/*! \ingroup groupe_unique_sql
 *\brief Classe mère des classes conditions d'unicité pour les entité possédant
 *deux clés avec exactement une non nulle et une condition
 *d'unicité sur la clé non nulle et d'autre attributs commums aux deux clés.
 */
template<class Ent> class atts_relation_exact_one_not_null_unique_sql : public relation_exact_one_not_null_unique_sql<Ent> {
public:
    CONSTR_DEFAUT(atts_relation_exact_one_not_null_unique_sql)

    //! Destructeur.
    ~atts_relation_exact_one_not_null_unique_sql() override; //default

protected:
    //! Transmet les valeurs des attributs uniques aux requètes SQL préparées.
    virtual void bind_values_unique_atts(const Ent &ent) = 0;

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 1.
    void bind_values_unique(const Ent &ent) override {
        relation_exact_one_not_null_unique_sql<Ent>::bind_values_unique(ent);
        bind_values_unique_atts(ent);
    }

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bind_values_unique_2(const Ent &ent) override {
        relation_exact_one_not_null_unique_sql<Ent>::bind_values_unique_2(ent);
        bind_values_unique_atts(ent);
    }
};

template<class Ent> atts_relation_exact_one_not_null_unique_sql<Ent>::~atts_relation_exact_one_not_null_unique_sql() = default;

/*! \ingroup groupe_unique_sql
 *\brief Classe mère des classes conditions d'unicité pour les entité possédant trois conditions d'unicités.
 */
template<class Ent> class triple_unique_sql : public virtual double_unique_sql<Ent> {
protected:
    QString m_unique_3; //!< Requete Sql sur l'existence de l'ensemble d'attribus uniques 2.

    using double_unique_sql<Ent>::m_unique;
    using double_unique_sql<Ent>::m_unique_2;
    using double_unique_sql<Ent>::aucun_autre_ou_meme;
    using double_unique_sql<Ent>::aucun_autre_ou_meme_id;
    using double_unique_sql<Ent>::autre_ou_meme;
    using double_unique_sql<Ent>::autre_ou_meme_id;
    using double_unique_sql<Ent>::exec;
    using double_unique_sql<Ent>::exec_unique;
    using double_unique_sql<Ent>::exec_unique_id;
    using double_unique_sql<Ent>::finish;
    using double_unique_sql<Ent>::id;
    using double_unique_sql<Ent>::id_unique;
    using double_unique_sql<Ent>::id_2_unique;
    using double_unique_sql<Ent>::next;
    using double_unique_sql<Ent>::prepare;
    using double_unique_sql<Ent>::unique_creer_string;
    using double_unique_sql<Ent>::unique_string;

public:
    enum unique_set {Unique_Set_1, Unique_Set_2, Unique_Set_3 = double_unique_sql<Ent>::Nbr_Unique_Set};
    CONSTR_DEFAUT(triple_unique_sql)

    //! Destructeur.
    ~triple_unique_sql() override = default;

    //! Construit la chaine sql de créations des conditions d'unicités.
    QString creer(const info_bdd &info) const override
        {return double_unique_sql<Ent>::creer(info).append(",").append(unique_creer_string(info,Unique_Set_3));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement trois ensembles de valeurs uniques.
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    b2d::existe_uni exists_unique(Ent &ent) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement trois ensembles de valeurs uniques
    //! et renvoie l'identifiant du premier trouver.
    std::pair<b2d::existe_uni,idt> exists_unique_id(const Ent &ent) override;

    //! Ecrit les requètes d'unicité.
    void initialise(const info_bdd &info) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    virtual void bind_values_unique_3(const Ent &ent) = 0;

    //! Retourne l'identifiant corespondant la troisième requête d'unicité.
    idt id_3_Unique(const Ent &ent) {
        prepare(m_unique_3);
        bind_values_unique_3(ent);
        exec();
        return next()? id() : 0;
    }
};

template<class Ent> b2d::existe_uni triple_unique_sql<Ent>::exists_unique(Ent &ent) {
    auto id_uni_1 = id_unique(ent);
    auto id_uni_2 = id_2_unique(ent);
    auto id_uni_3 = id_3_Unique(ent);
    finish();

    if(id_uni_1 == id_uni_2 && id_uni_1 == id_uni_3)
        return aucun_autre_ou_meme(ent, id_uni_1);
    else {
        if(id_uni_1 == 0) {
            if(id_uni_2 == 0 || id_uni_2 == id_uni_3)
                return autre_ou_meme(ent, id_uni_3);
            else if(id_uni_3 == 0)
                return autre_ou_meme(ent, id_uni_2);
            else
                return b2d::Conflit;
        }
        else {
            if((id_uni_2 == 0 &&id_uni_3 == id_uni_1)
                    || (id_uni_2 == id_uni_1 && id_uni_3 == 0))
                return autre_ou_meme(ent, id_uni_1);
            else
                return b2d::Conflit;
        }
    }
}

template<class Ent> std::pair<b2d::existe_uni,idt> triple_unique_sql<Ent>::exists_unique_id(const Ent &ent) {
    auto id_uni_1 = id_unique(ent);
    auto id_uni_2 = id_2_unique(ent);
    auto id_uni_3 = id_3_Unique(ent);
    finish();

    if(id_uni_1 == id_uni_2 &&id_uni_1 == id_uni_3)
        return aucun_autre_ou_meme_id(ent, id_uni_1);
    else {
        if(id_uni_1 == 0) {
            if(id_uni_2 == 0 || id_uni_2 == id_uni_3)
                return autre_ou_meme_id(ent, id_uni_3);
            else if(id_uni_3 == 0)
                return autre_ou_meme_id(ent, id_uni_2);
            else
                return std::pair<b2d::existe_uni,idt>(b2d::Conflit,
                                                  ent.id() != id_uni_2 ? id_uni_2 : id_uni_3);
        }
        else {
            if((id_uni_2 == 0 && id_uni_3 == id_uni_1)
                    || (id_uni_2 == id_uni_1 && id_uni_3 == 0))
                return autre_ou_meme_id(ent, id_uni_1);
            else {
                std::pair<b2d::existe_uni,idt> resultat(b2d::existe_uni::Conflit,0);
                if(ent.id() != id_uni_1)
                    resultat.second = id_uni_1;
                else if(id_uni_2 != 0)
                    resultat.second = id_uni_2;
                else
                    resultat.second = id_uni_3;
                return resultat;
            }
        }
    }
}

template<class Ent> void triple_unique_sql<Ent>::initialise(const info_bdd &info) {
    double_unique_sql<Ent>::initialise(info);
    m_unique_3 = unique_string(info,Unique_Set_3);
    m_unique_3.squeeze();
}

/*! \ingroup groupe_unique_sql
 *\brief Classe mère des classes conditions d'unicité pour les entité possédant
 *trois clés avec exactement une non nulle et unique.
 */
template<class Ent> class triple_exact_one_not_null_unique_sql
        : public triple_unique_sql<Ent>, relation_exact_one_not_null_unique_sql<Ent> {
protected:
    using double_unique_sql<Ent>::m_unique;
    using double_unique_sql<Ent>::m_unique_2;
    using triple_unique_sql<Ent>::m_unique_3;
    using double_unique_sql<Ent>::bind_value;
    using double_unique_sql<Ent>::exec;
    using double_unique_sql<Ent>::exec_unique;
    using double_unique_sql<Ent>::exec_unique_id;
    using double_unique_sql<Ent>::finish;
    using double_unique_sql<Ent>::id;
    using double_unique_sql<Ent>::next;
    using double_unique_sql<Ent>::prepare;

public:
    enum {Id_1_Unique = 0, Nbr_Unique_1,
          Id_2_Unique = 0, Nbr_Unique_2,
          Id_3_Unique = 0, Nbr_Unique_3,
          Unique_Set_1 = triple_unique_sql<Ent>::Unique_Set_1,
          Unique_Set_2 = triple_unique_sql<Ent>::Unique_Set_2,
          Unique_Set_3 = triple_unique_sql<Ent>::Unique_Set_3,
          Id_1_Unique_Set = Unique_Set_1,
          Id_2_Unique_Set = Unique_Set_2,
          Id_3_Unique_Set = Unique_Set_3
         };
    CONSTR_DEFAUT(triple_exact_one_not_null_unique_sql)

    //! Destructeur.
    ~triple_exact_one_not_null_unique_sql() override = default; //default

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques.
    //! Si le test est positif et que l'entitée est nouvelle l'identitfiant de l'entité entity est remplacé
    //! par celui l'entité en base de donnée ayant les mêmes valeurs d'attributs uniques.
    b2d::existe_uni exists_unique(Ent &ent) override;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée,
    //! dans le cas particulier où il y a exactement deux ensembles de valeurs uniques
    //! et renvoie l'identifiant du premier trouver.
    std::pair<b2d::existe_uni,idt> exists_unique_id(const Ent &ent) override;

protected:
    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 1.
    void bind_values_unique(const Ent &ent) override
        {bind_value(Id_1_Unique,ent.id_1());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bind_values_unique_2(const Ent &ent) override
        {bind_value(Id_2_Unique,ent.id_2());}

    //! Transmet les valeurs des attributs uniques à la requète SQL préparée 2.
    void bind_values_unique_3(const Ent &ent) override
        {bind_value(Id_3_Unique,ent.id_3());}
};

template<class Ent> b2d::existe_uni triple_exact_one_not_null_unique_sql<Ent>::exists_unique(Ent &ent)
{
    if(ent.id_3() == 0)
        return relation_exact_one_not_null_unique_sql<Ent>::exists_unique(ent);
    else if(ent.id_2() == 0 && ent.id_1() == 0)
    {
        prepare(m_unique_3);
        bind_values_unique_3(ent);
        return exec_unique(ent);
    }
    return b2d::Conflit;
}

template<class Ent> std::pair<b2d::existe_uni,idt> triple_exact_one_not_null_unique_sql<Ent>::exists_unique_id(const Ent &ent) {
    if(ent.id_3() == 0)
        return relation_exact_one_not_null_unique_sql<Ent>::exists_unique_id(ent);
    else if(ent.id_2() == 0 && ent.id_1() == 0) {
        prepare(m_unique_3);
        bind_values_unique_3(ent);
        return exec_unique_id(ent);
    }
    else
        return std::pair<b2d::existe_uni,idt>(b2d::existe_uni::Conflit,0);;
}
}}
#endif // ABSTRACT_UNIQUE_SQL_H
