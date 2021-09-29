/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2016
 */
#ifndef MANAGER_SQL_H
#define MANAGER_SQL_H

#include <memory>
#include "abstract_link_sql.h"
#include "abstract_manager.h"
#include "abstract_unique_sql.h"

// Macro pour manageur.
//! \ingroup groupe_manager
//! Coprs des deux methodes save_valide.
#define SAVE_VALIDE auto controle = b2d::None;\
    if(ent.is_new()) {\
        controle = exists_unique(ent); \
        if(controle == b2d::Aucun) {\
            add(ent); \
            controle = b2d::None;}} \
    else {\
        if(!same_in_bdd(ent)){ \
            controle = exists_unique(ent); \
            if(exists_unique(ent) <= b2d::Meme) { \
                modify(ent); \
                controle = b2d::None;}}} \
    if(controle != b2d::None){ \
        if(controle != b2d::Conflit) { \
            Ent ent_bdd(ent); \
            ent_bdd.set_id(0); \
            get_unique(ent_bdd); \
            throw unique_entity_exception(QString("void manager_sql<").append(name()).append(">::save_valide(") \
                                                       .append(name()).append("&)"),ent,ent_bdd);} \
        else throw unique_entity_exception(QString("void manager_sql<").append(name()).append(">::save_valide(") \
                                        .append(name()).append("&)"),ent);}

// Macro pour manageur.
//! \ingroup groupe_manager
//! Coprs des deux methodes save.
#define SAVE if(ent.is_valid()) \
    save_valide(ent); \
else \
    throw entities::invalide_entity_exception(QString("void manager_sql<").append(name()).append(">::save(") \
                                                        .append(name()).append("&)"),ent);

namespace mps {
namespace manager {
/*! \ingroup groupe_manager
 *\brief Classe template mère des différents manageurs de type SQL.
 */
class abstract_manager_sql :  public req_sql {
public:
    static const std::vector<QString> m_Fonction_agrega_string;
    static const std::vector<QString> m_Condition_string;
    CONSTR_DEFAUT(abstract_manager_sql)
    DESTR_VIDE(abstract_manager_sql)
};

/*! \ingroup groupe_manager
 *\brief Classe template mère des différents manageurs de type SQL.
 *
 *Classe template permettant d'instancier les différents manageurs.
 *Cette classe implémente les méthodes virtuelles pures de sa classe mère.
 *La classe Ent est la classe d'entité de programmation
 *et le pointeur unique permet de faire les tests d'unicité.
 */
template<class Ent> class manager_sql : public abstract_manager_temp<Ent>, public abstract_manager_sql {
protected:
    link_sql<Ent> m_link;            //!< Lien.
    info_bdd m_info;                 //!< Information sur la table dans la base de donnée.
    std::unique_ptr<abstract_unique_sql_temp<Ent>> m_unique;        //! Condition d'unicité.
    // Requête sql
    QString m_add;       //!< Requéte sql d'insertion d'une ligne.
    QString m_delete;    //!< Requéte sql de suppression d'une ligne
    QString m_exists;    //!< Requéte sql d'existence d'une ligne d'identifiant donné.
    QString m_exists_where;    //!< Requéte sql d'existense d'une ligne vérifiant une condition.
    QString m_fonction_agrega;    //!< Requéte sql appliquant une fonction d'agrega à toutes les lignes de la table.
    QString m_fonction_agrega_where_1;      //!< Requéte sql appliquant une fonction d'agrega aux lignes de la table vérifiant une condition.
    QString m_fonction_agrega_where_2;      //!< Requéte sql appliquant une fonction d'agrega aux lignes de la table vérifiant deux conditions.
    QString m_get;       //!< Requéte sql de lecture d'une ligne d'identifiant donné.
    QString m_get_list;   //!< Requéte sql de lecture de toutes les lignes de la table triées suivant une colonne.
    QString m_get_list_where;      //!< Requéte sql de lecture des lignes de la table vérifiant une condition.
    QString m_get_list_where_order;      //!< Requéte sql de lecture des lignes de la table vérifiant une condition et un ordre.
    QString m_get_list_id_where_order;      //!< Requéte sql de lecture des identifiants des lignes de la table vérifiant une condition et un ordre.
    QString m_get_list_1_where_1;    //!< Requéte sql de lecture des lignes de la table vérifiant une condition triées suivant une colonne.
    QString m_get_list_id_1_where_1;    //!< Requéte sql de lecture des identifiants de la table vérifiant une condition triées suivant une colonne.
    QString m_get_list_1_where_2;    //!< Requéte sql de lecture des lignes de la table vérifiant une condition triées suivant deux colonnes.
    QString m_get_list_id_1_where_2;    //!< Requéte sql de lecture des identifiants de la table vérifiant une condition triées suivant deux colonnes.
    QString m_get_list_1_where_3;    //!< Requéte sql de lecture des lignes de la table vérifiant une condition triées suivant trois colonnes.
    QString m_get_list_id_1_where_3;    //!< Requéte sql de lecture des identifiants de la table vérifiant une condition triées suivant trois colonnes.
    QString m_get_list_2_where_1;    //!< Requéte sql de lecture des lignes de la table vérifiant deux conditions triées suivant une colonne.
    QString m_get_list_id_2_where_1;    //!< Requéte sql de lecture des identifiants de la table vérifiant deux conditions triées suivant une colonne.
    QString m_get_list_3_where_1;    //!< Requéte sql de lecture des lignes de la table vérifiant deux conditions triées suivant une colonne.
    QString m_get_list_id_3_where_1;    //!< Requéte sql de lecture des identifiants de la table vérifiant deux conditions triées suivant une colonne.
    QString m_get_list_join;        //!< Requéte sql de lecture des lignes de la table vérifiant une condition avec une jointure triées jusqu'à trois colonnes.
    QString m_get_list_join_1_where_1;        //!< Requéte sql de lecture des lignes de la table vérifiant une condition avec une jointure triées suivant une colonne.
    QString m_last_id;    //!< Requéte sql de lecture de l'identifiant de la dernière ligne insérée dans la table.
    QString m_modify;    //!< Requéte sql de mis à jour d'une ligne de la table.

    using abstract_manager_sql::value;

public:
    using abstract_manager_temp<Ent>::exists;
    using abstract_manager_temp<Ent>::exists_unique;
    using abstract_manager_temp<Ent>::del;
    using abstract_manager_temp<Ent>::get;
    using abstract_manager_temp<Ent>::get_list;
    using abstract_manager_temp<Ent>::get_unique;
    using abstract_manager_temp<Ent>::name;
    using abstract_manager_temp<Ent>::same_in_bdd;
    using abstract_manager_temp<Ent>::save;

    //! Contructeur, avec les noms des attributs de la table et un po.
    manager_sql(const info_bdd &info = info_bdd(),
               std::unique_ptr<abstract_unique_sql_temp<Ent>> &&unique =  std::make_unique<no_unique_sql<Ent>>());

    //! Destructeur.
    ~manager_sql() override = default;

    //! Renvoie le nom en base de donnée du i-ème attribut.
    const QString &attribut(post pos) const
        {return m_info.attribut(pos);}

    //! Crée dans la base de donnée la table associée l'entité du manageur.
    void creer() override;


    //! Teste si la table est vide.
    bool empty() override {
        exec(m_exists_where.arg("true"));
        return !next();
    }

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    bool exists(const entity &ent) override {
        if(Ent::Verif_entity(ent)) {
            prepare(m_exists);
            m_link.set_id(ent);
            exec();
            bool bb = next();
            finish();
            return bb;
        }
        else
            throw entities::invalide_entity_exception(QString("bool manager_sql<")
                                                      .append(name()).append(">::exist(const entity &)"),ent);
    }

    //! Teste s'il existe une entité d'identifiant id en base de donnée.
    bool exists(idt id) override {
        prepare(m_exists);
        bind_value(0,id);
        exec();
        bool bb = next();
        finish();
        return bb;
    }

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    bool exists(const Ent &ent) override
        {return exists(ent.id());}

    //! Teste s'il existe une entité vérifiant une condition.
    bool exists(typename Ent::position cle, const QVariant &value, b2d::condition cond = b2d::condition::Egal) override {
        prepare(m_exists_where.arg(QString(attribut(cle)).append(m_Condition_string[cond]).append("?")));
        bind_value(0,value);
        exec();
        bool bb = next();
        finish();
        return bb;
    }

    //! Teste s'il existe une entité vérifiant des conditions.
    bool exists(const std::map<typename Ent::position, QVariant> &value,
                std::vector<b2d::condition> cond = std::vector<b2d::condition>({b2d::condition::Egal})) override {
        if(value.size() > cond.size())
            cond.resize(value.size(),b2d::condition::Egal);
        QString str;
        auto it_cond = cond.cbegin();
        for (auto it_value = value.cbegin(); it_value != value.cend(); ++it_value, ++it_cond)
            str.append(attribut(it_value->first)).append(m_Condition_string[*it_cond]).append("? AND ");
        str.chop(5);
        prepare(m_exists_where.arg(str));
        auto i = 0;
        for(auto it_value = value.cbegin(); it_value != value.cend(); ++it_value, ++i)
            bind_value(i,it_value->second);
        exec();
        bool bb = next();
        finish();
        return bb;
    }

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui de l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    b2d::existe_uni exists_unique(Ent &ent) override
        {return m_unique->exists_unique(ent);}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    b2d::existe_uni exists_unique(const Ent &ent) override
        {return exists_unique_id(ent).first;}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    std::pair<b2d::existe_uni,idt> exists_unique_id(const Ent &ent) override
        {return m_unique->exists_unique_id(ent);}


    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override {
        prepare(m_delete);
        bind_value(0,id);
        exec_finish();
        return true;
    }

    //! Fonction d'agrega sur l'attribut att appliquée à toutes les entités de la table.
    QVariant fonction_agrega(b2d::agrega fonc, typename Ent::position att) override {
        exec(m_fonction_agrega.arg(m_Fonction_agrega_string[fonc],
                                     attribut(att)));
        next();
        return value<QVariant>();
    }

    //! Fonction d'agrega  sur l'attribut att appliquée à toutes les entités vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    QVariant fonction_agrega(b2d::agrega fonc, typename Ent::position att,
                                       typename Ent::position cle, const QVariant &value,
                                       b2d::condition cond = b2d::condition::Egal) override {
        prepare(m_fonction_agrega_where_1.arg(m_Fonction_agrega_string[fonc],
                                              attribut(att),
                                              attribut(cle),
                                              m_Condition_string[cond]));
        bind_value(0,value);
        exec();
        next();
        return abstract_manager_sql::value<QVariant>();
    }

    //! Fonction d'agrega sur l'attribut att appliquée à toutes les entités vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2.
    QVariant fonction_agrega(b2d::agrega fonc, typename Ent::position att,
                                       typename Ent::position cle_1, const QVariant &value_1,
                                       typename Ent::position cle_2, const QVariant &value_2,
                                       b2d::condition cond_1 = b2d::condition::Egal,
                                       b2d::condition cond_2 = b2d::condition::Egal) override {
        prepare(m_fonction_agrega_where_2.arg(m_Fonction_agrega_string[fonc],
                                              attribut(att),
                                              attribut(cle_1),
                                              m_Condition_string[cond_1],
                                              attribut(cle_2),
                                              m_Condition_string[cond_2]));
        bind_value(0,value_1);
        bind_value(1,value_2);
        exec();
        next();
        return value<QVariant>();
    }

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que ent.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get(Ent &ent) override {
        prepare(m_get);
        m_link.set_id(ent);
        exec();
        if(next()) {
            m_link.from_requete(ent);
            finish();
            return true;
        }
        else {
            finish();
            return false;
        }
    }

    //! Renvoie la liste des entités de la table vérifiant la condition.
    list_ptr<Ent> get_list(const QString &condition) override {
        prepare(m_get_list_where.arg(condition));
        return list_from_requete();
    }

    //! Renvoie la liste des entités de la table ordonnée suivant la colonne d'identifiant ordre.
    list_ptr<Ent> get_list(typename Ent::position ordre = Ent::Id, bool crois = true) override {
        prepare(m_get_list.arg(attribut(ordre),croissant(crois)));
        return list_from_requete();
    }

    //! Renvoie la liste des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    list_ptr<Ent> get_list(typename Ent::position cle, const QVariant &value,
                           typename Ent::position ordre = Ent::Id,
                           b2d::condition cond = b2d::condition::Egal, bool crois = true) override {
        prepare(m_get_list_1_where_1.arg(attribut(cle), m_Condition_string[cond],
                                        attribut(ordre), croissant(crois)));
        bind_value(0,value);
        return list_from_requete();
    }

    //! Renvoie la liste des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1 puis ordre_2.
    list_ptr<Ent> get_list(typename Ent::position cle, const QVariant &value,
                         typename Ent::position ordre_1, typename Ent::position ordre_2,
                         b2d::condition cond = b2d::condition::Egal, bool crois_1 = true, bool crois_2 = true) override {
        prepare(m_get_list_1_where_2.arg(attribut(cle), m_Condition_string[cond],
                                        attribut(ordre_1), croissant(crois_1),
                                        attribut(ordre_2), croissant(crois_2)));
        bind_value(0,value);
        return list_from_requete();
    }

    //! Renvoie la liste des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1, ordre_2 puis ordre_3.
    list_ptr<Ent> get_list(typename Ent::position cle, const QVariant &value,
                         typename Ent::position ordre_1, typename Ent::position ordre_2, typename Ent::position ordre_3,
                         b2d::condition cond = b2d::condition::Egal,
                         bool crois_1 = true, bool crois_2 = true, bool crois_3 = true) override {
        prepare(m_get_list_1_where_3.arg(attribut(cle), m_Condition_string[cond],
                                        attribut(ordre_1), croissant(crois_1),
                                        attribut(ordre_2), croissant(crois_2),
                                        attribut(ordre_3), croissant(crois_3)));
        bind_value(0,value);
        return list_from_requete();
    }

    //! Renvoie la liste des entités de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    list_ptr<Ent> get_list(typename Ent::position cle_1, const QVariant &value_1,
                         typename Ent::position cle_2,  const QVariant &value_2,
                         typename Ent::position ordre = Ent::Id,
                         b2d::condition cond_1 = b2d::condition::Egal, b2d::condition cond_2 = b2d::condition::Egal,
                         bool crois = true) override {
        prepare(m_get_list_2_where_1.arg(attribut(cle_1), m_Condition_string[cond_1],
                                        attribut(cle_2), m_Condition_string[cond_2],
                                        attribut(ordre), croissant(crois)));
        bind_value(0,value_1);
        bind_value(1,value_2);
        return list_from_requete();
    }

    //! Renvoie la liste des entités de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1, valeur de la colonne d'identifiant cle_2 = value_2
    //! et valeur de la colonne d'identifiant cle_3 = value_3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    list_ptr<Ent> get_list(typename Ent::position cle_1, const QVariant &value_1,
                           typename Ent::position cle_2, const QVariant &value_2,
                           typename Ent::position cle_3, const QVariant &value_3,
                           typename Ent::position ordre = Ent::Id,
                           b2d::condition cond_1 = b2d::condition::Egal,
                           b2d::condition cond_2 = b2d::condition::Egal,
                           b2d::condition cond_3 = b2d::condition::Egal,
                           bool crois = true) override {
        prepare(m_get_list_3_where_1.arg(attribut(cle_1), m_Condition_string[cond_1],
                                        attribut(cle_2), m_Condition_string[cond_2],
                                        attribut(cle_3), m_Condition_string[cond_3],
                                        attribut(ordre), croissant(crois)));
        bind_value(0,value_1);
        bind_value(1,value_2);
        bind_value(2,value_3);
        return list_from_requete();
    }

    //! Renvoie la liste des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    list_ptr<Ent> get_list(const std::vector<typename Ent::position> &cle, const std::vector<QVariant> &value,
                         const std::vector<typename Ent::position> &ordre = std::vector<typename Ent::position>(),
                         const std::vector<b2d::condition> &condition = std::vector<b2d::condition>(),
                         const std::vector<bool> &crois = std::vector<bool>()) override {
        if(cle.size() != value.size())
            throw std::invalid_argument("get_list le nombre de clés doit être égale au nombre de valeurs");
        QString sql_w;

        for(szt i = 0; i != cle.size(); ++i) {
            sql_w.append(attribut(cle.at(i)));
            if(i < condition.size())
                sql_w.append(m_Condition_string[condition[i]]);
            else
                sql_w.append(m_Condition_string[b2d::condition::Egal]);
            sql_w.append("? AND ");
        }
        sql_w.chop(4);
        QString sqlO;
        for(szt i = 0; i != ordre.size(); ++i) {
            sqlO.append(m_Condition_string[condition[i]]);
            if(i < crois.size())
                sqlO.append(" ").append(croissant(crois[i]));
            sqlO.append(",");
        }
        sqlO.chop(1);

        prepare(m_get_list_where_order.arg(sql_w,sql_w));
        for(numt i = 0; i != value.size(); ++i)
            bind_value(i,value[i]);

        return list_from_requete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    std::list<idt> get_list_id(typename Ent::position cle, const QVariant &value,
                         typename Ent::position ordre = Ent::Id,
                         b2d::condition cond = b2d::condition::Egal, bool crois = true) override {
        prepare(m_get_list_id_1_where_1.arg(attribut(cle),m_Condition_string[cond],
                                        attribut(ordre),croissant(crois)));
        bind_value(0,value);
        return list_id_from_requete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1 puis ordre_2.
    std::list<idt> get_list_id(typename Ent::position cle, const QVariant &value,
                         typename Ent::position ordre_1, typename Ent::position ordre_2,
                         b2d::condition cond = b2d::condition::Egal,
                         bool crois_1 = true, bool crois_2 = true) override {
        prepare(m_get_list_id_1_where_2.arg(attribut(cle), m_Condition_string[cond],
                                        attribut(ordre_1), croissant(crois_1),
                                        attribut(ordre_2), croissant(crois_2)));
        bind_value(0,value);
        return list_id_from_requete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1, ordre_2 puis ordre_3.
    std::list<idt> get_list_id(typename Ent::position cle, const QVariant &value,
                           typename Ent::position ordre_1, typename Ent::position ordre_2, typename Ent::position ordre_3,
                           b2d::condition cond = b2d::condition::Egal,
                           bool crois_1 = true, bool crois_2 = true, bool crois_3 = true) override {
        prepare(m_get_list_id_1_where_3.arg(attribut(cle), m_Condition_string[cond],
                                        attribut(ordre_1), croissant(crois_1),
                                        attribut(ordre_2), croissant(crois_2),
                                        attribut(ordre_3), croissant(crois_3)));
        bind_value(0,value);
        return list_id_from_requete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    std::list<idt> get_list_id(typename Ent::position cle_1, const QVariant &value_1,
                           typename Ent::position cle_2,  const QVariant &value_2,
                           typename Ent::position ordre = Ent::Id,
                           b2d::condition cond_1 = b2d::condition::Egal, b2d::condition cond_2 = b2d::condition::Egal,
                           bool crois = true) override {
        prepare(m_get_list_id_2_where_1.arg(attribut(cle_1), m_Condition_string[cond_1],
                                        attribut(cle_2), m_Condition_string[cond_2],
                                        attribut(ordre), croissant(crois)));
        bind_value(0,value_1);
        bind_value(1,value_2);
        return list_id_from_requete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1, valeur de la colonne d'identifiant cle_2 = value_2
    //! et valeur de la colonne d'identifiant cle_3 = value_3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    std::list<idt> get_list_id(typename Ent::position cle_1, const QVariant &value_1,
                         typename Ent::position cle_2, const QVariant &value_2,
                         typename Ent::position cle_3, const QVariant &value_3,
                         typename Ent::position ordre = Ent::Id,
                         b2d::condition cond_1 = b2d::condition::Egal, b2d::condition cond_2 = b2d::condition::Egal,
                         b2d::condition cond_3 = b2d::condition::Egal,
                         bool crois = true) override {
        prepare(m_get_list_id_3_where_1.arg(attribut(cle_1), m_Condition_string[cond_1],
                                        attribut(cle_2), m_Condition_string[cond_2],
                                        attribut(cle_3), m_Condition_string[cond_3],
                                        attribut(ordre), croissant(crois)));
        bind_value(0,value_1);
        bind_value(1,value_2);
        bind_value(2,value_3);
        return list_id_from_requete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    std::list<idt> get_list_id(const std::vector<typename Ent::position> &cle, const std::vector<QVariant> &value,
                           const std::vector<typename Ent::position> &ordre = std::vector<typename Ent::position>(),
                           const std::vector<b2d::condition> &condition = std::vector<b2d::condition>(),
                           const std::vector<bool> &crois = std::vector<bool>()) override {
        if(cle.size() != value.size())
            throw std::invalid_argument("get_list le nombre de clés doit être égale au nombre de valeurs");
        QString sql_w;

        for(szt i = 0; i != cle.size(); ++i) {
            sql_w.append(attribut(cle.at(i)));
            if(i < condition.size())
                sql_w.append(m_Condition_string[condition[i]]);
            else
                sql_w.append(m_Condition_string[b2d::condition::Egal]);
            sql_w.append("? AND ");
        }
        sql_w.chop(4);
        QString sqlO;
        for(szt i = 0; i != ordre.size(); ++i) {
            sqlO.append(attribut(ordre[i]));
            if(i < crois.size())
                sqlO.append(" ").append(croissant(crois[i]));
            sqlO.append(",");
        }
        sqlO.chop(1);

        prepare(m_get_list_id_where_order.arg(sql_w,sqlO));
        for(numt i = 0; i != value.size(); ++i)
            bind_value(i,value[i]);

        return list_id_from_requete();
    }

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonne_table = colonne_join),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map where_map_table,
    //! valeur des colonnes de la table Join key = value de std::map where_map_join,
    //! ordonnée suivant les colonnes de la table Ent d'identifiants key
    //! et d'ordre value de std::map order_map_table (true -> croissant, false -> décroissant).
    list_ptr<Ent> get_list_join(const QString &table_join, post colonne_table,
                                                   const QString &colonne_join,
                                                   const std::map<typename Ent::position,QVariant> &where_map_table,
                                                   const std::map<QString,QVariant> &where_map_join,
                                                   const std::vector<std::pair<typename Ent::position,bool>> &order_map_table) override {
        QString sql_where;
        for(auto i = where_map_table.cbegin(); i != where_map_table.cend(); ++i)
            sql_where.append("T.").append(attribut(i->first)).append("=? AND ");
        for(auto i = where_map_join.cbegin(); i != where_map_join.cend(); ++i)
            sql_where.append("J.").append(i->first).append("=? AND ");
        sql_where.chop(5);
        QString sql_order;
        for(auto i = order_map_table.cbegin(); i != order_map_table.cend(); ++i)
            sql_order.append(" T.").append(attribut(i->first)).append(" ").append(croissant(i->second)).append(",");
        sql_order.chop(1);
        prepare(m_get_list_join.arg(table_join,
                                     attribut(colonne_table),
                                     colonne_join,
                                     sql_where,
                                     sql_order));
        auto j = 0;
        for(auto i = where_map_table.cbegin(); i != where_map_table.cend(); ++i, ++j)
            bind_value(j,i->second);
        for(auto i = where_map_join.cbegin(); i != where_map_join.cend(); ++i, ++j)
            bind_value(j,i->second);
        return list_from_requete();
    }

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonne_join),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = value_where,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    list_ptr<Ent> get_list_join(const QString &table_join,
                             const QString &colonne_join,
                             const QString &where_join,
                             const QVariant &value_where,
                             typename Ent::position ordre = Ent::Id,
                             b2d::condition cond = b2d::condition::Egal, bool crois = true) override {
        prepare(m_get_list_join_1_where_1.arg(table_join,
                                            colonne_join,
                                            where_join, m_Condition_string[cond],
                                            attribut(ordre), croissant(crois)));
        bind_value(0,value_where);
        return list_from_requete();
    }

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure
    //! (table.ID = join_1.colonne_1, join_2.ID = join_1.colonne_2),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = value_where sur la seconde jointure,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    list_ptr<Ent> get_list_join(const QString &table_join_1,
                             const QString &table_join_2,
                             const QString &colonne_1,
                             const QString &colonne_2,
                             const QString &where_join_2,
                             const QVariant &value_where,
                             typename Ent::position ordre = Ent::Id,
                             b2d::condition cond = b2d::condition::Egal, bool crois = true) override {
        prepare(m_get_list_join.arg(table_join_1,"ID",
                                     QString(colonne_1).append(" JOIN ").append(table_join_2).append(" K ON K.ID=J.").append(colonne_2),
                                     QString("K.").append(where_join_2).append(m_Condition_string[cond]).append("?"),
                                     QString("T.").append(attribut(ordre)).append(" ").append(croissant(crois))));
        bind_value(0,value_where);
        return list_from_requete();
    }

    //! Renvoie la map des entités de la table vérifiant la condition.
    map_id<Ent> get_map(const QString &condition, typename Ent::position cle_map = Ent::Id) {
        prepare(m_get_list_where.arg(condition));
        return map_from_requete(cle_map);
    }

    //! Renvoie la map des entités de la table.
    map_id<Ent> get_map(typename Ent::position cle_map = Ent::Id) {
        prepare(m_get_list.arg(attribut(cle_map),QString()));
        return map_from_requete(cle_map);
    }

    //! Renvoie la map des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    map_id<Ent> get_map(typename Ent::position cle, const QVariant &value,
                       typename Ent::position cle_map = Ent::Id, b2d::condition cond = b2d::condition::Egal) override {
        prepare(m_get_list_1_where_1.arg(attribut(cle), m_Condition_string[cond],
                                        attribut(cle_map), QString()));
        bind_value(0,value);
        return map_from_requete(cle_map);
    }

    //! Renvoie la map des entités de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2.
    map_id<Ent> get_map(typename Ent::position cle_1, const QVariant &value_1,
                       typename Ent::position cle_2,  const QVariant &value_2,
                       typename Ent::position cle_map = Ent::Id,
                       b2d::condition cond_1 = b2d::condition::Egal, b2d::condition cond_2 = b2d::condition::Egal) override {
        prepare(m_get_list_2_where_1.arg(attribut(cle_1), m_Condition_string[cond_1],
                                        attribut(cle_2), m_Condition_string[cond_2],
                                        attribut(cle_map), QString()));
        bind_value(0,value_1);
        bind_value(1,value_2);
        return map_from_requete(cle_map);
    }

    //! Renvoie la map des entités de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1, valeur de la colonne d'identifiant cle_2 = value_2
    //! et valeur de la colonne d'identifiant cle_3 = value_3.
    map_id<Ent> get_map(typename Ent::position cle_1, const QVariant &value_1,
                         typename Ent::position cle_2, const QVariant &value_2,
                         typename Ent::position cle_3, const QVariant &value_3,
                         typename Ent::position cle_map = Ent::Id,
                         b2d::condition cond_1 = b2d::condition::Egal, b2d::condition cond_2 = b2d::condition::Egal,
                         b2d::condition cond_3 = b2d::condition::Egal) override {
        prepare(m_get_list_3_where_1.arg(attribut(cle_1), m_Condition_string[cond_1],
                                        attribut(cle_2), m_Condition_string[cond_2],
                                        attribut(cle_3), m_Condition_string[cond_3],
                                        attribut(cle_map), QString()));
        bind_value(0,value_1);
        bind_value(1,value_2);
        bind_value(2,value_3);
        return map_from_requete(cle_map);
    }

    //! Renvoie la map des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    map_id<Ent> get_map(const std::vector<typename Ent::position> &cle, const std::vector<QVariant> &value,
                       typename Ent::position cle_map = Ent::Id,
                       const std::vector<typename Ent::position> &ordre = std::vector<typename Ent::position>(),
                       const std::vector<b2d::condition> &condition = std::vector<b2d::condition>(),
                       const std::vector<bool> &crois = std::vector<bool>()) override {
        if(cle.size() != value.size())
            throw std::invalid_argument("get_list le nombre de clés doit être égale au nombre de valeurs");
        QString sql;

        for(szt i = 0; i != cle.size(); ++i) {
            sql.append(attribut(cle.at(i)));
            if(i < condition.size())
                sql.append(m_Condition_string[condition[i]]);
            else
                sql.append(m_Condition_string[b2d::condition::Egal]);
            sql.append("? AND ");
        }
        sql.chop(4);
        sql.append("ORDER BY ");
        for(szt i =0; i != ordre.size(); ++i) {
            sql.append(attribut(ordre[i]));
            if(i < crois.size())
                sql.append(" ").append(croissant(crois[i]));
            sql.append(",");
        }
        sql.chop(1);

        prepare(m_get_list_where_order.arg(sql));
        for(numt i = 0; i != value.size(); ++i)
            bind_value(i,value[i]);

        return map_from_requete(cle_map);
    }

    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (colonne_table = colonne_join),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map where_map_table,
    //! valeur des colonnes de la table Join key = value de std::map where_map_join.
    map_id<Ent> get_map_join(const QString &table_join, post colonne_table,
                           const QString &colonne_join,
                           const std::map<typename Ent::position,QVariant> &where_map_table,
                           const std::map<QString,QVariant> &where_map_join,
                           typename Ent::position cle_map = Ent::Id) override {
        QString sql_where;
        for(auto i = where_map_table.cbegin(); i != where_map_table.cend(); ++i)
            sql_where.append("T.").append(attribut(i->first)).append("=? AND ");
        for(auto i = where_map_join.cbegin(); i != where_map_join.cend(); ++i)
            sql_where.append("J.").append(i->first).append("=? AND ");
        sql_where.chop(5);
        QString sql_order(" T.");
        sql_order.append(attribut(cle_map));
        prepare(m_get_list_join.arg(table_join,
                                     attribut(colonne_table),
                                     colonne_join,
                                     sql_where,
                                     sql_order));
        auto j = 0;
        for(auto i = where_map_table.cbegin(); i != where_map_table.cend(); ++i, ++j)
            bind_value(j,i->second);
        for(auto i = where_map_join.cbegin(); i != where_map_join.cend(); ++i, ++j)
            bind_value(j,i->second);
        return map_from_requete(cle_map);
    }

    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonne_join),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = value_where.
    map_id<Ent> get_map_join(const QString &table_join,
                           const QString &colonne_join,
                           const QString &where_join,
                           const QVariant &value_where,
                           typename Ent::position cle_map = Ent::Id,
                           b2d::condition cond = b2d::condition::Egal) override {
        prepare(m_get_list_join_1_where_1.arg(table_join,
                                            colonne_join,
                                            where_join, m_Condition_string[cond],
                                            attribut(cle_map),QString()));
        bind_value(0,value_where);
        return map_from_requete(cle_map);
    }

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant les mêmes valeurs pour au moins un ensemble des attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get_unique(Ent &ent) override {
        b2d::existe_uni n = exists_unique(ent);
        return (n == b2d::Tous || n == b2d::Meme) ? get(ent) : false;
    }

    //! Renvoie les info de la table associée au manager.
    const info_bdd &info() const
        {return m_info;}

    //! Renvoie le nombre d'attribut de l'entité dans la base de donnée.
    post nbr_att() const
        {return m_info.nbr_att();}

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool same_in_bdd(const Ent &c_ent) override {
        Ent ent(c_ent.id());
        return get(ent) ? ent == c_ent : false;
    }

    //! Teste s'il y a dans la base de donnée une entité d'identifiant id ayant exactement les mêmes attributs que ent.
    bool same_in_bdd(const Ent &c_ent, idt id) override {
        Ent ent(id);
        if(!get(ent))
            return false;
        ent.set_id(c_ent.id());
        return ent == c_ent;
    }

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à ent.
    void save(Ent &ent) override
        {SAVE}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent &ent) override
        {SAVE}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    void save_unique(Ent &ent) override {
        if(ent.is_valid()) {
            auto id = ent.id();
            b2d::existe_uni n = exists_unique(ent);
            if(n <= b2d::existe_uni::Meme) {
                if(ent.is_new())
                    add(ent);
                else if(!same_in_bdd(ent))
                    modify(ent);

                if(id != 0 &&id != ent.id())
                    del(id);
            }
            else
                throw unique_entity_exception(QString("void manager_sql<").append(name()).append(">::save_unique(") \
                                            .append(name()).append("&)"), ent);
        }
        else
            throw entities::invalide_entity_exception(QString("void manager_sql<").append(name()).append(">::save_unique(") \
                                                                                       .append(name()).append("&)"),ent);
    }

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    void save_unique(const Ent &ent) override {
        if(ent.is_valid())
        {
            std::pair<b2d::existe_uni,idt> pairExists = exists_unique_id(ent);
            if(pairExists.first <= b2d::existe_uni::Meme) {
                if(ent.is_new()) {
                    if(pairExists.second == 0)
                        add(ent);
                    else {
                        if(!same_in_bdd(ent,pairExists.second))
                            modify(ent,pairExists.second);
                    }
                }
                else if(!same_in_bdd(ent))
                    modify(ent);

                if(!ent.is_new() &&pairExists.second != ent.id())
                    del(ent.id());
            }
            else
                throw unique_entity_exception(QString("void manager_sql<").append(name()).append(">::save_unique(const ") \
                                            .append(name()).append("&)"),ent);
        }
        else
            throw entities::invalide_entity_exception(QString("void manager_sql<").append(name()).append(">::save_unique(const ") \
                                                                                       .append(name()).append("&)"),ent);
    }

    //! Renvoie le nom de la table de l'entité dans la base de donnée.
    const QString &table() const
        {return m_info.table();}

protected:
    /*//! Constructeur sans argument pour en faire une classe virtuelle.
    manager_sql();*/

    //! Insert la nouvelle entité entity dans la base de donnée
    //! et assigne l'identifiant de l'entité insérée en base de donnée à ent.
    virtual void add(Ent &ent) {
        prepare(m_add);
        m_link.bind_values(ent);
        exec();
        exec(m_last_id);
        next();
        ent.set_id(value<idt>());
        finish();
    }

    //! Insert la nouvelle entité entity dans la base de donnée.
    virtual void add(const Ent &ent) {
        prepare(m_add);
        m_link.bind_values(ent);
        exec_finish();
    }

    //! Renvoit la chaine de caractères "ACS" ou "DESC"
    QString croissant(bool crois)
        {return crois ? QString(): " DESC";}

    //! Construit la liste des entités correspondant une requète de type sqlGetList.
    list_ptr<Ent> list_from_requete() {
        exec();
        list_ptr<Ent> liste;
        while(next())
            liste.push_front(m_link.new_from_requete());
        finish();
        return liste;
    }

    //! Construit la liste des entités correspondant une requète de type sqlGetList.
    std::list<idt> list_id_from_requete() {
        exec();
        std::list<idt> liste;
        while(next())
            liste.push_back(value<idt>());
        finish();
        return liste;
    }

    //! Construit la map des entités correspondant une requète de type sqlGetList.
    map_id<Ent> map_from_requete(typename Ent::position cle) {
        exec();
        map_id<Ent> map;
        while(next())
            m_link.from_requete(map[m_link.id(cle)]);
        finish();
        return map;
    }

    //! Message d'erreurs si l'entité entity n'est pas valide.
    QString message_erreurs(const entity &ent) const override;

    //! Message d'erreurs s'il existe déjà en base de donnée une entité ayant les mêmes valeurs d'attributs uniques que ent.
    //QString message_erreursUnique(const entity &ent) const override;

    //! Met à jour l'entité entity en base de donnée.
    virtual void modify(const Ent &ent) {
        prepare(m_modify);
        m_link.bind_values(ent);
        m_link.set_id(ent,m_info.nbr_att()-1);
        exec_finish();
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'ent.
    virtual void modify(const Ent &ent, idt id) {
        prepare(m_modify);
        m_link.bind_values(ent);
        bind_value(m_info.nbr_att()-1,id);
        exec_finish();
    }

    //! Enregistre l'entité valide entity en base de donnée et assigne l'identifiant de l'entité
    //! insérée en base de donnée à ent.
    virtual void save_valide(Ent &ent)
        {SAVE_VALIDE}

    //! Enregistre l'entité valide entity en base de donnée.
    virtual void save_valide(const Ent &ent)
        {SAVE_VALIDE}

    //! Ecrit les chaines des requètes sql.
    void write_string_sql();
};

template<class Ent> manager_sql<Ent>::manager_sql(const info_bdd &info, std::unique_ptr<abstract_unique_sql_temp<Ent>> &&unique)
    : m_info(info),
      m_unique(std::move(unique)) {
    write_string_sql();
    m_unique->initialise(m_info);
}

template<class Ent> void manager_sql<Ent>::creer() {
        QString sql(word_sql_string(b2d::word_sql::Create));
        sql.append(" ").append(table()).append("(").append(attribut(entity::Id)).append(" ")
                .append(type_attribut_sql_string(b2d::type_attribut_bdd::Primary));
        for(post i = 1; i != m_info.nbr_att(); ++i) {
            sql.append(",").append(attribut(i)).append(" ").append(type_attribut_sql_string(m_info.creer_attribut(i).first));
            if(m_info.creer_attribut(i).second)
                sql.append(" ").append(word_sql_string(b2d::word_sql::Not_null));
        }
        for(auto i = m_info.foreign_key_table().cbegin(); i != m_info.foreign_key_table().cend(); ++i)
            if(!i->second.isEmpty())
                sql.append(",").append(word_sql_string(b2d::word_sql::Foreign)).append("(").append(attribut(i->first)).append(") ")
                    .append(word_sql_string(b2d::word_sql::Ref)).append(" ").append(i->second);
        sql.append(m_unique->creer(m_info));
        sql.append(")");
        exec(sql);
}

template<class Ent> QString manager_sql<Ent>::message_erreurs(const entity &ent) const {
    QString message("Entité invalide:");
    message.append(Ent::Name()).append(" d'identifiant:").append(QString::number(ent.id())).append("\n")
            .append(ent.affiche());
    return message;
}

template<class Ent> void manager_sql<Ent>::write_string_sql() {
    // Liste des colonnes.
    QString colonnesId;
    for(post i = 0; i != m_info.nbr_att(); ++i)
        colonnesId.append(attribut(i)).append(",");
    colonnesId.chop(1);

    QString colonnes;
    for(post i = 1; i != m_info.nbr_att(); ++i)
        colonnes.append(attribut(i)).append(",");
    colonnes.chop(1);

    QString idEgal(attribut(Ent::Id));
    idEgal.append("=?");

    // Select where
    m_get_list_where.append("SELECT ");
    m_get_list_where.append(colonnesId).append(" FROM ").append(table()).append(" WHERE %1");
    m_get_list_where.squeeze();

    // Select Join
    QString selectJoin("SELECT ");
    for(post i = 0; i != m_info.nbr_att(); ++i)
        selectJoin.append("T.").append(attribut(i)).append(",");
    selectJoin.chop(1);
    selectJoin.append(" FROM ").append(table()).append(" T JOIN %1 J ON ");

    // Add
    m_add.append("INSERT INTO ");
    m_add.append(table()).append("(").append(colonnes).append(") VALUES(");
    for(post i = 1; i != m_info.nbr_att(); ++i)
        m_add.append("?,");
    m_add.chop(1);
    m_add.append(")");
    m_add.squeeze();

    // Delete
    m_delete.append("DELETE FROM ");
    m_delete.append(table()).append(" WHERE ").append(idEgal);
    m_delete.squeeze();

    // ExisteWhere
    m_exists_where.append("SELECT 1 FROM ");
    m_exists_where.append(table()).append(" WHERE %1 LIMIT 1");
    m_exists_where.squeeze();

    // Existe
    m_exists.append(m_exists_where.arg(idEgal));
    m_exists.squeeze();

    // Agrega
    m_fonction_agrega.append("SELECT %1(%2) FROM ");
    m_fonction_agrega.append(table());
    m_fonction_agrega.squeeze();

    // Agrega where 1
    m_fonction_agrega_where_1.append(m_fonction_agrega);
    m_fonction_agrega_where_1.append(" WHERE %3%4?");
    m_fonction_agrega_where_1.squeeze();

    // Agrega where 2
    m_fonction_agrega_where_2.append(m_fonction_agrega_where_1);
    m_fonction_agrega_where_2.append(" AND %5%6?");
    m_fonction_agrega_where_2.squeeze();

    // Get
    m_get.append(m_get_list_where.arg(idEgal));
    m_get.squeeze();

    // Get list
    m_get_list.append("SELECT ");
    m_get_list.append(colonnesId).append(" FROM ").append(table()).append(" ORDER BY %1%2");
    m_get_list.squeeze();

    // Get list where ordre
    m_get_list_where_order.append(m_get_list_where.arg("%1? ORDER BY %2"));
    m_get_list_where_order.squeeze();

    // Get list id where  ordre
    m_get_list_id_where_order.append("SELECT ID FROM ").append(table()).append(" WHERE %1? ORDER BY %2");
    m_get_list_id_where_order.squeeze();

    // Get list 1 where 1 ordre
    m_get_list_1_where_1.append(m_get_list_where_order.arg("%1%2","%3%4"));
    m_get_list_1_where_1.squeeze();

    // Get list id 1 where 1 ordre
    m_get_list_id_1_where_1.append(m_get_list_id_where_order.arg("%1%2","%3%4"));
    m_get_list_id_1_where_1.squeeze();

    // Get list 1 where 2 ordre
    m_get_list_1_where_2.append(m_get_list_1_where_1);
    m_get_list_1_where_2.append(",%5%6");
    m_get_list_1_where_2.squeeze();

    // Get list id 1 where 2 ordre
    m_get_list_id_1_where_2.append(m_get_list_id_1_where_1);
    m_get_list_id_1_where_2.append(",%5%6");
    m_get_list_id_1_where_2.squeeze();

    // Get list 1 where 3 ordre
    m_get_list_1_where_3.append(m_get_list_1_where_2);
    m_get_list_1_where_3.append(",%7%8");
    m_get_list_1_where_3.squeeze();

    // Get list id 1 where 3 ordre
    m_get_list_id_1_where_3.append(m_get_list_id_1_where_2);
    m_get_list_id_1_where_3.append(",%7%8");
    m_get_list_id_1_where_3.squeeze();

    // Get list 2 where 1 ordre
    m_get_list_2_where_1.append(m_get_list_where_order.arg("%1%2? AND %3%4","%5%6"));
    m_get_list_2_where_1.squeeze();

    // Get list id 2 where 1 ordre
    m_get_list_id_2_where_1.append(m_get_list_id_where_order.arg("%1%2? AND %3%4","%5%6"));
    m_get_list_id_2_where_1.squeeze();

    // Get list 3 where 1 ordre
    m_get_list_3_where_1.append(m_get_list_where_order.arg("%1%2? AND %3%4? AND %5%6","%7%8"));
    m_get_list_3_where_1.squeeze();

    // Get list 3 where 1 ordre
    m_get_list_id_3_where_1.append(m_get_list_id_where_order.arg("%1%2? AND %3%4? AND %5%6","%7%8"));
    m_get_list_id_3_where_1.squeeze();

    // Get list join
    m_get_list_join.append(selectJoin);
    m_get_list_join.append("T.%2=J.%3 WHERE %4 ORDER BY %5");
    m_get_list_join.squeeze();

    // Get list join 1 where 1
    m_get_list_join_1_where_1.append(selectJoin);
    m_get_list_join_1_where_1.append("T.ID=J.%2 WHERE J.%3%4? ORDER BY T.%5%6");
    m_get_list_join_1_where_1.squeeze();

    // Id last query
    m_last_id.append("SELECT last_insert_rowid() FROM ");
    m_last_id.append(table());
    m_last_id.squeeze();

    // Modify
    m_modify.append("UPDATE ");
    m_modify.append(table()).append(" SET ");
    for(post i = 1; i != m_info.nbr_att(); ++i)
        m_modify.append(attribut(i)).append("=?,");
    m_modify.chop(1);
    m_modify.append(" WHERE ").append(idEgal);
    m_modify.squeeze();
}
}}
#endif // MANAGER_SQL_H
