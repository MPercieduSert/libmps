/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2016
 */
#ifndef MANAGERSQL_H
#define MANAGERSQL_H

#include <memory>
#include "AbstractLinkSql.h"
#include "AbstractManager.h"
#include "AbstractUniqueSql.h"


// Macro pour manageur.
//! \ingroup groupeManager
//! Coprs des deux methodes saveValide.
#define SAVE_VALIDE auto controle = bmps::None;\
    if(entity.isNew()) {\
        controle = existsUnique(entity); \
        if(controle == bmps::Aucun) {\
            add(entity); \
            controle = bmps::None;}} \
    else {\
        if(!sameInBdd(entity)){ \
            controle = existsUnique(entity); \
            if(existsUnique(entity) <= bmps::Meme) { \
                modify(entity); \
                controle = bmps::None;}}} \
    if(controle != bmps::None){ \
        if(controle != bmps::Conflit) { \
            Ent entBdd(entity); \
            entBdd.setId(0); \
            getUnique(entBdd); \
            throw UniqueEntityException(QString("void ManagerSql<").append(name()).append(">::saveValide(") \
                                                       .append(name()).append("&)"),entity,entBdd);} \
        else throw UniqueEntityException(QString("void ManagerSql<").append(name()).append(">::saveValide(") \
                                        .append(name()).append("&)"),entity);}

// Macro pour manageur.
//! \ingroup groupeManager
//! Coprs des deux methodes save.
#define SAVE if(entity.isValid()) \
    saveValide(entity); \
else \
    throw entityMPS::InvalideEntityException(QString("void ManagerSql<").append(name()).append(">::save(") \
                                                        .append(name()).append("&)"),entity);

namespace managerMPS {
/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs de type SQL.
 */
class AbstractManagerSql :  public ReqSql {
public:
    static const std::vector<QString> m_fonctionAgregaString;
    static const std::vector<QString> m_conditionString;
    //static const std::vector<QString> m_conditionNullString;
    //static const std::array<const char *, bmps::condition> m_fonctionConditionString;

    CONSTR_DEFAUT(AbstractManagerSql)
    DESTR_VIDE(AbstractManagerSql)
};

/*! \ingroup groupeManager
 * \brief Classe template mère des différents manageurs de type SQL.
 *
 * Classe template permettant d'instancier les différents manageurs.
 * Cette classe implémente les méthodes virtuelles pures de sa classe mère.
 * La classe Ent est la classe d'entité de programmation
 * et le pointeur unique permet de faire les tests d'unicité.
 */
template<class Ent> class ManagerSql : public AbstractManagerTemp<Ent>, public AbstractManagerSql {
protected:
    LinkSql<Ent> m_link;            //!< Lien.
    InfoBdd m_info;                 //!< Information sur la table dans la base de donnée.
    std::unique_ptr<AbstractUniqueSqlTemp<Ent>> m_unique;        //! Condition d'unicité.
    // Requête sql
    QString m_sqlAdd;       //!< Requéte sql d'insertion d'une ligne.
    QString m_sqlDelete;    //!< Requéte sql de suppression d'une ligne
    QString m_sqlExists;    //!< Requéte sql d'existence d'une ligne d'identifiant donné.
    QString m_sqlExistsWhere;    //!< Requéte sql d'existense d'une ligne vérifiant une condition.
    QString m_sqlFonctionAgrega;    //!< Requéte sql appliquant une fonction d'agrega à toutes les lignes de la table.
    QString m_sqlFonctionAgregaWhere1;      //!< Requéte sql appliquant une fonction d'agrega aux lignes de la table vérifiant une condition.
    QString m_sqlFonctionAgregaWhere2;      //!< Requéte sql appliquant une fonction d'agrega aux lignes de la table vérifiant deux conditions.
    QString m_sqlGet;       //!< Requéte sql de lecture d'une ligne d'identifiant donné.
    QString m_sqlGetList;   //!< Requéte sql de lecture de toutes les lignes de la table triées suivant une colonne.
    QString m_sqlGetListWhere;      //!< Requéte sql de lecture des lignes de la table vérifiant une condition.
    QString m_sqlGetListWhereOrderby;      //!< Requéte sql de lecture des lignes de la table vérifiant une condition et un ordre.
    QString m_sqlGetListIdWhereOrderby;      //!< Requéte sql de lecture des identifiants des lignes de la table vérifiant une condition et un ordre.
    QString m_sqlGetList1Where1;    //!< Requéte sql de lecture des lignes de la table vérifiant une condition triées suivant une colonne.
    QString m_sqlGetListId1Where1;    //!< Requéte sql de lecture des identifiants de la table vérifiant une condition triées suivant une colonne.
    QString m_sqlGetList1Where2;    //!< Requéte sql de lecture des lignes de la table vérifiant une condition triées suivant deux colonnes.
    QString m_sqlGetListId1Where2;    //!< Requéte sql de lecture des identifiants de la table vérifiant une condition triées suivant deux colonnes.
    QString m_sqlGetList1Where3;    //!< Requéte sql de lecture des lignes de la table vérifiant une condition triées suivant trois colonnes.
    QString m_sqlGetListId1Where3;    //!< Requéte sql de lecture des identifiants de la table vérifiant une condition triées suivant trois colonnes.
    QString m_sqlGetList2Where1;    //!< Requéte sql de lecture des lignes de la table vérifiant deux conditions triées suivant une colonne.
    QString m_sqlGetListId2Where1;    //!< Requéte sql de lecture des identifiants de la table vérifiant deux conditions triées suivant une colonne.
    QString m_sqlGetList3Where1;    //!< Requéte sql de lecture des lignes de la table vérifiant deux conditions triées suivant une colonne.
    QString m_sqlGetListId3Where1;    //!< Requéte sql de lecture des identifiants de la table vérifiant deux conditions triées suivant une colonne.
    QString m_sqlGetListJoin;        //!< Requéte sql de lecture des lignes de la table vérifiant une condition avec une jointure triées jusqu'à trois colonnes.
    QString m_sqlGetListJoin1Where1;        //!< Requéte sql de lecture des lignes de la table vérifiant une condition avec une jointure triées suivant une colonne.
    QString m_sqllastId;    //!< Requéte sql de lecture de l'identifiant de la dernière ligne insérée dans la table.
    QString m_sqlModify;    //!< Requéte sql de mis à jour d'une ligne de la table.

    using AbstractManagerSql::value;

public:
    using AbstractManagerTemp<Ent>::exists;
    using AbstractManagerTemp<Ent>::existsUnique;
    using AbstractManagerTemp<Ent>::del;
    using AbstractManagerTemp<Ent>::get;
    using AbstractManagerTemp<Ent>::getList;
    using AbstractManagerTemp<Ent>::getUnique;
    using AbstractManagerTemp<Ent>::name;
    using AbstractManagerTemp<Ent>::sameInBdd;
    using AbstractManagerTemp<Ent>::save;

    //! Contructeur, avec les noms des attributs de la table et un po.
    ManagerSql(const InfoBdd & info = InfoBdd(),
               std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique =  std::make_unique<NoUniqueSql<Ent>>());

    //! Destructeur.
    ~ManagerSql() override = default;

    //! Renvoie le nom en base de donnée du i-ème attribut.
    const QString & attribut(szt pos) const
        {return m_info.attribut(pos);}

    //! Crée dans la base de donnée la table associée l'entité du manageur.
    void creer() override;

    /*//! Renvoie le nom en base de donnée du i-ème attribut unique.
    const QString & attributUnique(szt i) const
        {return m_link.attribut(m_unique.attributUnique(i));}*/

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    bool exists(const Entity & entity) override {
        if(Ent::VerifEntity(entity)) {
            prepare(m_sqlExists);
            m_link.setId(entity);
            exec();
            bool bb = next();
            finish();
            return bb;
        }
        else
            throw entityMPS::InvalideEntityException(QString("bool ManagerSql<").append(name()).append(">::exist(const Entity &)"),entity);
    }

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    bool exists(const Ent & entity) override {
        prepare(m_sqlExists);
        m_link.setId(entity);
        exec();
        bool bb = next();
        finish();
        return bb;
    }

    //! Teste s'il existe une entité vérifiant une condition.
    bool exists(typename Ent::Position cle, const QVariant & value, bmps::condition cond = bmps::condition::Egal) override {
        prepare(m_sqlExistsWhere.arg(QString(attribut(cle)).append(m_conditionString[cond]).append("?")));
        bindValue(0,value);
        exec();
        bool bb = next();
        finish();
        return bb;
    }

    //! Teste s'il existe une entité vérifiant des conditions.
    bool exists(const std::map<typename Ent::Position, QVariant> & value,
                std::vector<bmps::condition> cond = std::vector<bmps::condition>({bmps::condition::Egal})) override {
        if(value.size() > cond.size())
            cond.resize(value.size(),bmps::condition::Egal);
        QString str;
        auto iterCond = cond.cbegin();
        for (auto iterValue = value.cbegin(); iterValue != value.cend(); ++iterValue, ++iterCond)
            str.append(attribut(iterValue->first)).append(m_conditionString[*iterCond]).append("? AND ");
        str.chop(5);
        prepare(m_sqlExistsWhere.arg(str));
        auto i = 0;
        for(auto iterValue = value.cbegin(); iterValue != value.cend(); ++iterValue, ++i)
            bindValue(i,iterValue->second);
        exec();
        bool bb = next();
        finish();
        return bb;
    }

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui de l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    bmps::existeUni existsUnique(Ent & entity) override
        {return m_unique->existsUnique(entity);}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    bmps::existeUni existsUnique(const Ent & entity) override
        {return existsUniqueId(entity).first;}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    std::pair<bmps::existeUni,idt> existsUniqueId(const Ent & entity) override
        {return m_unique->existsUniqueId(entity);}


    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    bool del(idt id) override {
        prepare(m_sqlDelete);
        bindValue(0,id);
        execFinish();
        return true;
    }

    //! Fonction d'agrega sur l'attribut att appliquée à toutes les entités de la table.
    QVariant fonctionAgrega(bmps::agrega fonc, typename Ent::Position att) override {
        exec(m_sqlFonctionAgrega.arg(m_fonctionAgregaString[fonc],
                                     attribut(att)));
        next();
        return value<QVariant>();
    }

    //! Fonction d'agrega  sur l'attribut att appliquée à toutes les entités vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    QVariant fonctionAgrega(bmps::agrega fonc, typename Ent::Position att,
                                       typename Ent::Position cle, const QVariant & value,
                                       bmps::condition cond = bmps::condition::Egal) override {
        prepare(m_sqlFonctionAgregaWhere1.arg(m_fonctionAgregaString[fonc],
                                              attribut(att),
                                              attribut(cle),
                                              m_conditionString[cond]));
        bindValue(0,value);
        exec();
        next();
        return AbstractManagerSql::value<QVariant>();
    }

    //! Fonction d'agrega sur l'attribut att appliquée à toutes les entités vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    QVariant fonctionAgrega(bmps::agrega fonc, typename Ent::Position att,
                                       typename Ent::Position cle1, const QVariant & value1,
                                       typename Ent::Position cle2, const QVariant & value2,
                                       bmps::condition cond1 = bmps::condition::Egal,
                                       bmps::condition cond2 = bmps::condition::Egal) override {
        prepare(m_sqlFonctionAgregaWhere2.arg(m_fonctionAgregaString[fonc],
                                              attribut(att),
                                              attribut(cle1),
                                              m_conditionString[cond1],
                                              attribut(cle2),
                                              m_conditionString[cond2]));
        bindValue(0,value1);
        bindValue(1,value2);
        exec();
        next();
        return value<QVariant>();
    }

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get(Ent & entity) override {
        prepare(m_sqlGet);
        m_link.setId(entity);
        exec();
        if(next()) {
            m_link.fromRequete(entity);
            finish();
            return true;
        }
        else {
            finish();
            return false;
        }
    }

    //! Renvoie la liste des entités de la table vérifiant la condition.
    ListPtr<Ent> getList(const QString & condition) override {
        prepare(m_sqlGetListWhere.arg(condition));
        return listFromRequete();
    }

    //! Renvoie la liste des entités de la table ordonnée suivant la colonne d'identifiant ordre.
    ListPtr<Ent> getList(typename Ent::Position ordre = Ent::Id, bool crois = true) override {
        prepare(m_sqlGetList.arg(attribut(ordre),croissant(crois)));
        return listFromRequete();
    }

    //! Renvoie la liste des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value,
                         typename Ent::Position ordre = Ent::Id,
                         bmps::condition cond = bmps::condition::Egal, bool crois = true) override {
        prepare(m_sqlGetList1Where1.arg(attribut(cle), m_conditionString[cond],
                                        attribut(ordre), croissant(crois)));
        bindValue(0,value);
        return listFromRequete();
    }

    //! Renvoie la liste des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value,
                         typename Ent::Position ordre1, typename Ent::Position ordre2,
                         bmps::condition cond = bmps::condition::Egal, bool crois1 = true, bool crois2 = true) override {
        prepare(m_sqlGetList1Where2.arg(attribut(cle), m_conditionString[cond],
                                        attribut(ordre1), croissant(crois1),
                                        attribut(ordre2), croissant(crois2)));
        bindValue(0,value);
        return listFromRequete();
    }

    //! Renvoie la liste des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1, ordre2 puis ordre3.
    ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value,
                         typename Ent::Position ordre1, typename Ent::Position ordre2, typename Ent::Position ordre3,
                         bmps::condition cond = bmps::condition::Egal,
                         bool crois1 = true, bool crois2 = true, bool crois3 = true) override {
        prepare(m_sqlGetList1Where3.arg(attribut(cle), m_conditionString[cond],
                                        attribut(ordre1), croissant(crois1),
                                        attribut(ordre2), croissant(crois2),
                                        attribut(ordre3), croissant(crois3)));
        bindValue(0,value);
        return listFromRequete();
    }

    //! Renvoie la liste des entités de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    ListPtr<Ent> getList(typename Ent::Position cle1, const QVariant & value1,
                         typename Ent::Position cle2,  const QVariant & value2,
                         typename Ent::Position ordre = Ent::Id,
                         bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                         bool crois = true) override {
        prepare(m_sqlGetList2Where1.arg(attribut(cle1), m_conditionString[cond1],
                                        attribut(cle2), m_conditionString[cond2],
                                        attribut(ordre), croissant(crois)));
        bindValue(0,value1);
        bindValue(1,value2);
        return listFromRequete();
    }

    //! Renvoie la liste des entités de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2
    //! et valeur de la colonne d'identifiant cle3 = value3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    ListPtr<Ent> getList(typename Ent::Position cle1, const QVariant & value1,
                         typename Ent::Position cle2, const QVariant & value2,
                         typename Ent::Position cle3, const QVariant & value3,
                         typename Ent::Position ordre = Ent::Id,
                         bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                         bmps::condition cond3 = bmps::condition::Egal,
                         bool crois = true) override {
        prepare(m_sqlGetList3Where1.arg(attribut(cle1), m_conditionString[cond1],
                                        attribut(cle2), m_conditionString[cond2],
                                        attribut(cle3), m_conditionString[cond3],
                                        attribut(ordre), croissant(crois)));
        bindValue(0,value1);
        bindValue(1,value2);
        bindValue(1,value3);
        return listFromRequete();
    }

    //! Renvoie la liste des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    ListPtr<Ent> getList(const std::vector<typename Ent::Position> & cle, const std::vector<QVariant> & value,
                         const std::vector<typename Ent::Position> & ordre = std::vector<typename Ent::Position>(),
                         const std::vector<bmps::condition> & condition = std::vector<bmps::condition>(),
                         const std::vector<bool> & crois = std::vector<bool>()) override {
        if(cle.size() != value.size())
            throw std::invalid_argument("getList le nombre de clés doit être égale au nombre de valeurs");
        QString sqlW;

        for(unsigned i = 0; i != cle.size(); ++i) {
            sqlW.append(attribut(i));
            if(i < condition.size())
                sqlW.append(m_conditionString[condition[i]]);
            else
                sqlW.append(m_conditionString[bmps::condition::Egal]);
            sqlW.append("? AND ");
        }
        sqlW.chop(4);
        QString sqlO;
        for(unsigned i =0; i != ordre.size(); ++i) {
            sqlO.append(m_conditionString[condition[i]]);
            if(i < crois.size())
                sqlO.append(" ").append(croissant(crois[i]));
            sqlO.append(",");
        }
        sqlO.chop(1);

        prepare(m_sqlGetListWhereOrderby.arg(sqlW,sqlW));
        for(unsigned i = 0; i != value.size(); ++i)
            bindValue(i,value[i]);

        return listFromRequete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    std::list<idt> getListId(typename Ent::Position cle, const QVariant & value,
                         typename Ent::Position ordre = Ent::Id,
                         bmps::condition cond = bmps::condition::Egal, bool crois = true) override {
        prepare(m_sqlGetListId1Where1.arg(attribut(cle),m_conditionString[cond],
                                        attribut(ordre),croissant(crois)));
        bindValue(0,value);
        return listIdFromRequete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    std::list<idt> getListId(typename Ent::Position cle, const QVariant & value,
                         typename Ent::Position ordre1, typename Ent::Position ordre2,
                         bmps::condition cond = bmps::condition::Egal,
                         bool crois1 = true, bool crois2 = true) override {
        prepare(m_sqlGetListId1Where2.arg(attribut(cle), m_conditionString[cond],
                                        attribut(ordre1), croissant(crois1),
                                        attribut(ordre2), croissant(crois2)));
        bindValue(0,value);
        return listIdFromRequete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1, ordre2 puis ordre3.
    std::list<idt> getListId(typename Ent::Position cle, const QVariant & value,
                           typename Ent::Position ordre1, typename Ent::Position ordre2, typename Ent::Position ordre3,
                           bmps::condition cond = bmps::condition::Egal,
                           bool crois1 = true, bool crois2 = true, bool crois3 = true) override {
        prepare(m_sqlGetListId1Where3.arg(attribut(cle), m_conditionString[cond],
                                        attribut(ordre1), croissant(crois1),
                                        attribut(ordre2), croissant(crois2),
                                        attribut(ordre3), croissant(crois3)));
        bindValue(0,value);
        return listIdFromRequete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    std::list<idt> getListId(typename Ent::Position cle1, const QVariant & value1,
                           typename Ent::Position cle2,  const QVariant & value2,
                           typename Ent::Position ordre = Ent::Id,
                           bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                           bool crois = true) override {
        prepare(m_sqlGetListId2Where1.arg(attribut(cle1), m_conditionString[cond1],
                                        attribut(cle2), m_conditionString[cond2],
                                        attribut(ordre), croissant(crois)));
        bindValue(0,value1);
        bindValue(1,value2);
        return listIdFromRequete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2
    //! et valeur de la colonne d'identifiant cle3 = value3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    std::list<idt> getListId(typename Ent::Position cle1, const QVariant & value1,
                         typename Ent::Position cle2, const QVariant & value2,
                         typename Ent::Position cle3, const QVariant & value3,
                         typename Ent::Position ordre = Ent::Id,
                         bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                         bmps::condition cond3 = bmps::condition::Egal,
                         bool crois = true) override {
        prepare(m_sqlGetListId3Where1.arg(attribut(cle1), m_conditionString[cond1],
                                        attribut(cle2), m_conditionString[cond2],
                                        attribut(cle3), m_conditionString[cond3],
                                        attribut(ordre), croissant(crois)));
        bindValue(0,value1);
        bindValue(1,value2);
        bindValue(2,value3);
        return listIdFromRequete();
    }

    //! Renvoie la liste des identifiants de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    std::list<idt> getListId(const std::vector<typename Ent::Position> & cle, const std::vector<QVariant> & value,
                           const std::vector<typename Ent::Position> & ordre = std::vector<typename Ent::Position>(),
                           const std::vector<bmps::condition> & condition = std::vector<bmps::condition>(),
                           const std::vector<bool> & crois = std::vector<bool>()) override {
        if(cle.size() != value.size())
            throw std::invalid_argument("getList le nombre de clés doit être égale au nombre de valeurs");
        QString sqlW;

        for(unsigned i = 0; i != cle.size(); ++i) {
            sqlW.append(attribut(i));
            if(i < condition.size())
                sqlW.append(m_conditionString[condition[i]]);
            else
                sqlW.append(m_conditionString[bmps::condition::Egal]);
            sqlW.append("? AND ");
        }
        sqlW.chop(4);
        QString sqlO;
        for(unsigned i =0; i != ordre.size(); ++i) {
            sqlO.append(ordre[i]);
            if(i < crois.size())
                sqlO.append(" ").append(croissant(crois[i]));
            sqlO.append(",");
        }
        sqlO.chop(1);

        prepare(m_sqlGetListIdWhereOrderby.arg(sqlW,sqlO));
        for(szt i = 0; i != value.size(); ++i)
            bindValue(i,value[i]);

        return listIdFromRequete();
    }

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map whereMapTable,
    //! valeur des colonnes de la table Join key = value de std::map whereMapJoin,
    //! ordonnée suivant les colonnes de la table Ent d'identifiants key
    //! et d'ordre value de std::map orderMapTable (true -> croissant, false -> décroissant).
    ListPtr<Ent> getListJoin(const QString & tableJoin, szt colonneTable,
                                                   const QString & colonneJoin,
                                                   const std::map<typename Ent::Position,QVariant> & whereMapTable,
                                                   const std::map<QString,QVariant> & whereMapJoin,
                                                   const std::vector<std::pair<typename Ent::Position,bool>> & orderMapTable) override {
        QString sqlWhere;
        for(auto i = whereMapTable.cbegin(); i != whereMapTable.cend(); ++i)
            sqlWhere.append("T.").append(attribut(i->first)).append("=? AND ");
        for(auto i = whereMapJoin.cbegin(); i != whereMapJoin.cend(); ++i)
            sqlWhere.append("J.").append(i->first).append("=? AND ");
        sqlWhere.chop(5);
        QString sqlOrder;
        for(auto i = orderMapTable.cbegin(); i != orderMapTable.cend(); ++i)
            sqlOrder.append(" T.").append(attribut(i->first)).append(" ").append(croissant(i->second)).append(",");
        sqlOrder.chop(1);
        prepare(m_sqlGetListJoin.arg(tableJoin,
                                     attribut(colonneTable),
                                     colonneJoin,
                                     sqlWhere,
                                     sqlOrder));
        szt j = 0;
        for(auto i = whereMapTable.cbegin(); i != whereMapTable.cend(); ++i, ++j)
            bindValue(j,i->second);
        for(auto i = whereMapJoin.cbegin(); i != whereMapJoin.cend(); ++i, ++j)
            bindValue(j,i->second);
        return listFromRequete();
    }

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonneJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    ListPtr<Ent> getListJoin(const QString & tableJoin,
                             const QString & colonneJoin,
                             const QString & whereJoin,
                             const QVariant & valueWhere,
                             typename Ent::Position ordre = Ent::Id,
                             bmps::condition cond = bmps::condition::Egal, bool crois = true) override {
        prepare(m_sqlGetListJoin1Where1.arg(tableJoin,
                                            colonneJoin,
                                            whereJoin, m_conditionString[cond],
                                            attribut(ordre), croissant(crois)));
        bindValue(0,valueWhere);
        return listFromRequete();
    }

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure
    //! (table.ID = join1.colonne1, join2.ID = join1.colonne2),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere sur la seconde jointure,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    ListPtr<Ent> getListJoin(const QString & tableJoin1,
                             const QString & tableJoin2,
                             const QString & colonne1,
                             const QString & colonne2,
                             const QString & whereJoin2,
                             const QVariant & valueWhere,
                             typename Ent::Position ordre = Ent::Id,
                             bmps::condition cond = bmps::condition::Egal, bool crois = true) override {
        prepare(m_sqlGetListJoin.arg(tableJoin1,"ID",
                                     QString(colonne1).append(" JOIN ").append(tableJoin2).append(" K ON K.ID=J.").append(colonne2),
                                     QString("K.").append(whereJoin2).append(m_conditionString[cond]).append("?"),
                                     QString("T.").append(attribut(ordre)).append(" ").append(croissant(crois))));
        bindValue(0,valueWhere);
        return listFromRequete();
    }

    //! Renvoie la map des entités de la table vérifiant la condition.
    mapIdt<Ent> getMap(const QString & condition, typename Ent::Position cleMap = Ent::Id) {
        prepare(m_sqlGetListWhere.arg(condition));
        return mapFromRequete(cleMap);
    }

    //! Renvoie la map des entités de la table.
    mapIdt<Ent> getMap(typename Ent::Position cleMap = Ent::Id) {
        prepare(m_sqlGetList.arg(attribut(cleMap),QString()));
        return mapFromRequete(cleMap);
    }

    //! Renvoie la map des entités de la table vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    mapIdt<Ent> getMap(typename Ent::Position cle, const QVariant & value,
                       typename Ent::Position cleMap = Ent::Id, bmps::condition cond = bmps::condition::Egal) override {
        prepare(m_sqlGetList1Where1.arg(attribut(cle), m_conditionString[cond],
                                        attribut(cleMap), QString()));
        bindValue(0,value);
        return mapFromRequete(cleMap);
    }

    //! Renvoie la map des entités de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    mapIdt<Ent> getMap(typename Ent::Position cle1, const QVariant & value1,
                       typename Ent::Position cle2,  const QVariant & value2,
                       typename Ent::Position cleMap = Ent::Id,
                       bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal) override {
        prepare(m_sqlGetList2Where1.arg(attribut(cle1), m_conditionString[cond1],
                                        attribut(cle2), m_conditionString[cond2],
                                        attribut(cleMap), QString()));
        bindValue(0,value1);
        bindValue(1,value2);
        return mapFromRequete(cleMap);
    }

    //! Renvoie la map des entités de la table vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2
    //! et valeur de la colonne d'identifiant cle3 = value3.
    mapIdt<Ent> getMap(typename Ent::Position cle1, const QVariant & value1,
                         typename Ent::Position cle2, const QVariant & value2,
                         typename Ent::Position cle3, const QVariant & value3,
                         typename Ent::Position cleMap = Ent::Id,
                         bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                         bmps::condition cond3 = bmps::condition::Egal) override {
        prepare(m_sqlGetList3Where1.arg(attribut(cle1), m_conditionString[cond1],
                                        attribut(cle2), m_conditionString[cond2],
                                        attribut(cle3), m_conditionString[cond3],
                                        attribut(cleMap), QString()));
        bindValue(0,value1);
        bindValue(1,value2);
        bindValue(3,value3);
        return mapFromRequete(cleMap);
    }

    //! Renvoie la map des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    mapIdt<Ent> getMap(const std::vector<typename Ent::Position> & cle, const std::vector<QVariant> & value,
                       typename Ent::Position cleMap = Ent::Id,
                       const std::vector<typename Ent::Position> & ordre = std::vector<typename Ent::Position>(),
                       const std::vector<bmps::condition> & condition = std::vector<bmps::condition>(),
                       const std::vector<bool> & crois = std::vector<bool>()) override {
        if(cle.size() != value.size())
            throw std::invalid_argument("getList le nombre de clés doit être égale au nombre de valeurs");
        QString sql;

        for(unsigned i = 0; i != cle.size(); ++i) {
            sql.append(attribut(i));
            if(i < condition.size())
                sql.append(m_conditionString[condition[i]]);
            else
                sql.append(m_conditionString[bmps::condition::Egal]);
            sql.append("? AND ");
        }
        sql.chop(4);
        sql.append("ORDER BY ");
        for(unsigned i =0; i != ordre.size(); ++i) {
            sql.append(ordre[i]);
            if(i < crois.size())
                sql.append(" ").append(croissant(crois[i]));
            sql.append(",");
        }
        sql.chop(1);

        prepare(m_sqlGetListWhereOrderby.arg(sql));
        for(unsigned i = 0; i != value.size(); ++i)
            bindValue(i,value[i]);

        return mapFromRequete(cleMap);
    }

    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map whereMapTable,
    //! valeur des colonnes de la table Join key = value de std::map whereMapJoin.
    mapIdt<Ent> getMapJoin(const QString & tableJoin, szt colonneTable,
                           const QString & colonneJoin,
                           const std::map<typename Ent::Position,QVariant> & whereMapTable,
                           const std::map<QString,QVariant> & whereMapJoin,
                           typename Ent::Position cleMap = Ent::Id) override {
        QString sqlWhere;
        for(auto i = whereMapTable.cbegin(); i != whereMapTable.cend(); ++i)
            sqlWhere.append("T.").append(attribut(i->first)).append("=? AND ");
        for(auto i = whereMapJoin.cbegin(); i != whereMapJoin.cend(); ++i)
            sqlWhere.append("J.").append(i->first).append("=? AND ");
        sqlWhere.chop(5);
        QString sqlOrder(" T.");
        sqlOrder.append(attribut(cleMap));
        prepare(m_sqlGetListJoin.arg(tableJoin,
                                     attribut(colonneTable),
                                     colonneJoin,
                                     sqlWhere,
                                     sqlOrder));
        szt j = 0;
        for(auto i = whereMapTable.cbegin(); i != whereMapTable.cend(); ++i, ++j)
            bindValue(j,i->second);
        for(auto i = whereMapJoin.cbegin(); i != whereMapJoin.cend(); ++i, ++j)
            bindValue(j,i->second);
        return mapFromRequete(cleMap);
    }

    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonneJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere.
    mapIdt<Ent> getMapJoin(const QString & tableJoin,
                           const QString & colonneJoin,
                           const QString & whereJoin,
                           const QVariant & valueWhere,
                           typename Ent::Position cleMap = Ent::Id,
                           bmps::condition cond = bmps::condition::Egal) override {
        prepare(m_sqlGetListJoin1Where1.arg(tableJoin,
                                            colonneJoin,
                                            whereJoin, m_conditionString[cond],
                                            attribut(cleMap),QString()));
        bindValue(0,valueWhere);
        return mapFromRequete(cleMap);
    }

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant les mêmes valeurs pour au moins un ensemble des attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool getUnique(Ent & entity) override {
        bmps::existeUni n = existsUnique(entity);
        return (n == bmps::Tous || n == bmps::Meme) ? get(entity) : false;
    }

    //! Renvoie les info de la table associée au manager.
    const InfoBdd & info() const
        {return m_info;}

    //! Renvoie le nombre d'attribut de l'entité dans la base de donnée.
    szt nbrAtt() const
        {return m_info.nbrAtt();}

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool sameInBdd(const Ent & entity) override {
        Ent entityT(entity.id());
        return get(entityT) ? entityT == entity : false;
    }

    //! Teste s'il y a dans la base de donnée une entité d'identifiant id ayant exactement les mêmes attributs que entity.
    bool sameInBdd(const Ent & entity, idt id) override {
        Ent entityT(id);
        if(!get(entityT))
            return false;
        entityT.setId(entity.id());
        return entityT == entity;
    }

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    void save(Ent & entity) override
        {SAVE}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Ent & entity) override
        {SAVE}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    void saveUnique(Ent & entity) override {
        if(entity.isValid()) {
            auto id = entity.id();
            bmps::existeUni n = existsUnique(entity);
            if(n <= bmps::existeUni::Meme) {
                if(entity.isNew())
                    add(entity);
                else if(!sameInBdd(entity))
                    modify(entity);

                if(id != 0 && id != entity.id())
                    del(id);
            }
            else
                throw UniqueEntityException(QString("void ManagerSql<").append(name()).append(">::saveUnique(") \
                                            .append(name()).append("&)"), entity);
        }
        else
            throw entityMPS::InvalideEntityException(QString("void ManagerSql<").append(name()).append(">::saveUnique(") \
                                                                                       .append(name()).append("&)"),entity);
    }

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    void saveUnique(const Ent & entity) override {
        if(entity.isValid())
        {
            std::pair<bmps::existeUni,idt> pairExists = existsUniqueId(entity);
            if(pairExists.first <= bmps::existeUni::Meme) {
                if(entity.isNew()) {
                    if(pairExists.second == 0)
                        add(entity);
                    else {
                        if(!sameInBdd(entity,pairExists.second))
                            modify(entity,pairExists.second);
                    }
                }
                else if(!sameInBdd(entity))
                    modify(entity);

                if(!entity.isNew() && pairExists.second != entity.id())
                    del(entity.id());
            }
            else
                throw UniqueEntityException(QString("void ManagerSql<").append(name()).append(">::saveUnique(const ") \
                                            .append(name()).append("&)"),entity);
        }
        else
            throw entityMPS::InvalideEntityException(QString("void ManagerSql<").append(name()).append(">::saveUnique(const ") \
                                                                                       .append(name()).append("&)"),entity);
    }

    //! Renvoie le nom de la table de l'entité dans la base de donnée.
    const QString & table() const
        {return m_info.table();}

protected:
    /*//! Constructeur sans argument pour en faire une classe virtuelle.
    ManagerSql();*/

    //! Insert la nouvelle entité entity dans la base de donnée
    //! et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    virtual void add(Ent & entity) {
        prepare(m_sqlAdd);
        m_link.bindValues(entity);
        exec();
        exec(m_sqllastId);
        next();
        entity.setId(value<szt>());
        finish();
    }

    //! Insert la nouvelle entité entity dans la base de donnée.
    virtual void add(const Ent & entity) {
        prepare(m_sqlAdd);
        m_link.bindValues(entity);
        execFinish();
    }

    //! Renvoit la chaine de caractères "ACS" ou "DESC"
    QString croissant(bool crois)
        {return crois ? QString(): " DESC";}

    //! Construit la liste des entités correspondant une requète de type sqlGetList.
    ListPtr<Ent> listFromRequete() {
        exec();
        ListPtr<Ent> liste;
        while(next())
            liste.push_front(m_link.newFromRequete());
        finish();
        return liste;
    }

    //! Construit la liste des entités correspondant une requète de type sqlGetList.
    std::list<idt> listIdFromRequete() {
        exec();
        std::list<idt> liste;
        while(next())
            liste.push_back(value<idt>());
        finish();
        return liste;
    }

    //! Construit la map des entités correspondant une requète de type sqlGetList.
    mapIdt<Ent> mapFromRequete(typename Ent::Position cle) {
        exec();
        mapIdt<Ent> map;
        while(next())
            m_link.fromRequete(map[m_link.id(cle)]);
        finish();
        return map;
    }

    //! Message d'erreurs si l'entité entity n'est pas valide.
    QString messageErreurs(const Entity & entity) const override;

    //! Message d'erreurs s'il existe déjà en base de donnée une entité ayant les mêmes valeurs d'attributs uniques que entity.
    //QString messageErreursUnique(const Entity & entity) const override;

    //! Met à jour l'entité entity en base de donnée.
    virtual void modify(const Ent & entity) {
        prepare(m_sqlModify);
        m_link.bindValues(entity);
        m_link.setId(entity,m_info.nbrAtt()-1);
        execFinish();
    }

    //! Met à jour l'entité entity en base de donnée d'identifiant id avec les valeurs d'entity.
    virtual void modify(const Ent & entity, idt id) {
        prepare(m_sqlModify);
        m_link.bindValues(entity);
        bindValue(m_info.nbrAtt()-1,id);
        execFinish();
    }

    //! Enregistre l'entité valide entity en base de donnée et assigne l'identifiant de l'entité
    //! insérée en base de donnée à entity.
    virtual void saveValide(Ent & entity)
        {SAVE_VALIDE}

    //! Enregistre l'entité valide entity en base de donnée.
    virtual void saveValide(const Ent & entity)
        {SAVE_VALIDE}

    //! Ecrit les chaines des requètes sql.
    void writeStringSql();
};

template<class Ent> ManagerSql<Ent>::ManagerSql(const InfoBdd &info, std::unique_ptr<AbstractUniqueSqlTemp<Ent>> && unique)
    : m_info(info),
      m_unique(std::move(unique)) {
    writeStringSql();
    m_unique->initialise(m_info);
}

template<class Ent> void ManagerSql<Ent>::creer() {
    //try
    //{
        QString sql(wordSqlString(bmps::wordSql::Create));
        sql.append(" ").append(table()).append("(").append(attribut(Entity::Id)).append(" ")
                .append(typeAttributSqlString(bmps::typeAttributBdd::Primary));
        for(szt i = 1; i != m_info.nbrAtt(); ++i) {
            sql.append(",").append(attribut(i)).append(" ").append(typeAttributSqlString(m_info.creerAttribut(i).first));
            if(m_info.creerAttribut(i).second)
                sql.append(" ").append(wordSqlString(bmps::wordSql::NotNull));
        }
        for(auto i = m_info.foreignKeyTable().cbegin(); i != m_info.foreignKeyTable().cend(); ++i)
            if(!i->second.isEmpty())
                sql.append(",").append(wordSqlString(bmps::wordSql::Foreign)).append("(").append(attribut(i->first)).append(") ")
                    .append(wordSqlString(bmps::wordSql::Ref)).append(" ").append(i->second);
        sql.append(m_unique->creer(m_info));
        sql.append(")");
        exec(sql);
//    }
//    catch(std::exception & e)
//    {throw std::runtime_error(QString("void ManagerSql<Ent>::creer() avec Ent=").append(Ent::Name()).append("/n")
//                              .append(e.what()).toStdString());}
}

template<class Ent> QString ManagerSql<Ent>::messageErreurs(const Entity & entity) const {
    QString message("Entité invalide:");
    message.append(Ent::Name()).append(" d'identifiant:").append(QString::number(entity.id())).append("\n")
            .append(entity.affiche());
    return message;
}

template<class Ent> void ManagerSql<Ent>::writeStringSql() {
    // Liste des colonnes.
    QString colonnesId;
    for(szt i = 0; i != m_info.nbrAtt(); ++i)
        colonnesId.append(attribut(i)).append(",");
    colonnesId.chop(1);

    QString colonnes;
    for(szt i = 1; i != m_info.nbrAtt(); ++i)
        colonnes.append(attribut(i)).append(",");
    colonnes.chop(1);

    QString idEgal(attribut(Ent::Id));
    idEgal.append("=?");

    // Select where
    m_sqlGetListWhere.append("SELECT ");
    m_sqlGetListWhere.append(colonnesId).append(" FROM ").append(table()).append(" WHERE %1");
    m_sqlGetListWhere.squeeze();

    // Select Join
    QString selectJoin("SELECT ");
    for(szt i = 0; i != m_info.nbrAtt(); ++i)
        selectJoin.append("T.").append(attribut(i)).append(",");
    selectJoin.chop(1);
    selectJoin.append(" FROM ").append(table()).append(" T JOIN %1 J ON ");

    // Add
    m_sqlAdd.append("INSERT INTO ");
    m_sqlAdd.append(table()).append("(").append(colonnes).append(") VALUES(");
    for(szt i = 1; i != m_info.nbrAtt(); ++i)
        m_sqlAdd.append("?,");
    m_sqlAdd.chop(1);
    m_sqlAdd.append(")");
    m_sqlAdd.squeeze();

    // Delete
    m_sqlDelete.append("DELETE FROM ");
    m_sqlDelete.append(table()).append(" WHERE ").append(idEgal);
    m_sqlDelete.squeeze();

    // ExisteWhere
    m_sqlExistsWhere.append("SELECT 1 FROM ");
    m_sqlExistsWhere.append(table()).append(" WHERE %1 LIMIT 1");
    m_sqlExists.squeeze();

    // Existe
    m_sqlExists.append(m_sqlExistsWhere.arg(idEgal));
    m_sqlExists.squeeze();

    // Agrega
    m_sqlFonctionAgrega.append("SELECT %1(%2) FROM ");
    m_sqlFonctionAgrega.append(table());
    m_sqlFonctionAgrega.squeeze();

    // Agrega where 1
    m_sqlFonctionAgregaWhere1.append(m_sqlFonctionAgrega);
    m_sqlFonctionAgregaWhere1.append(" WHERE %3%4?");
    m_sqlFonctionAgregaWhere1.squeeze();

    // Agrega where 2
    m_sqlFonctionAgregaWhere2.append(m_sqlFonctionAgregaWhere1);
    m_sqlFonctionAgregaWhere2.append(" AND %5%6?");
    m_sqlFonctionAgregaWhere2.squeeze();

    // Get
    m_sqlGet.append(m_sqlGetListWhere.arg(idEgal));
    m_sqlGet.squeeze();

    // Get list
    m_sqlGetList.append("SELECT ");
    m_sqlGetList.append(colonnesId).append(" FROM ").append(table()).append(" ORDER BY %1%2");
    m_sqlGetList.squeeze();

    // Get list where ordre
    m_sqlGetListWhereOrderby.append(m_sqlGetListWhere.arg("%1? ORDER BY %2"));
    m_sqlGetListWhereOrderby.squeeze();

    // Get list id where  ordre
    m_sqlGetListIdWhereOrderby.append("SELECT ID FROM ").append(table()).append(" WHERE %1? ORDER BY %2");
    m_sqlGetListIdWhereOrderby.squeeze();

    // Get list 1 where 1 ordre
    m_sqlGetList1Where1.append(m_sqlGetListWhereOrderby.arg("%1%2","%3%4"));
    m_sqlGetList1Where1.squeeze();

    // Get list id 1 where 1 ordre
    m_sqlGetListId1Where1.append(m_sqlGetListIdWhereOrderby.arg("%1%2","%3%4"));
    m_sqlGetListId1Where1.squeeze();

    // Get list 1 where 2 ordre
    m_sqlGetList1Where2.append(m_sqlGetList1Where1);
    m_sqlGetList1Where2.append(",%5%6");
    m_sqlGetList1Where2.squeeze();

    // Get list id 1 where 2 ordre
    m_sqlGetListId1Where2.append(m_sqlGetListId1Where1);
    m_sqlGetListId1Where2.append(",%5%6");
    m_sqlGetListId1Where2.squeeze();

    // Get list 1 where 3 ordre
    m_sqlGetList1Where3.append(m_sqlGetList1Where2);
    m_sqlGetList1Where3.append(",%7%8");
    m_sqlGetList1Where3.squeeze();

    // Get list id 1 where 3 ordre
    m_sqlGetListId1Where3.append(m_sqlGetListId1Where2);
    m_sqlGetListId1Where3.append(",%7%8");
    m_sqlGetListId1Where3.squeeze();

    // Get list 2 where 1 ordre
    m_sqlGetList2Where1.append(m_sqlGetListWhereOrderby.arg("%1%2? AND %3%4","%5%6"));
    m_sqlGetList2Where1.squeeze();

    // Get list id 2 where 1 ordre
    m_sqlGetListId2Where1.append(m_sqlGetListIdWhereOrderby.arg("%1%2? AND %3%4","%5%6"));
    m_sqlGetListId2Where1.squeeze();

    // Get list 3 where 1 ordre
    m_sqlGetList3Where1.append(m_sqlGetListWhereOrderby.arg("%1%2? AND %3%4? AND %5%6","%7%8"));
    m_sqlGetList3Where1.squeeze();

    // Get list 3 where 1 ordre
    m_sqlGetListId3Where1.append(m_sqlGetListIdWhereOrderby.arg("%1%2? AND %3%4? AND %5%6","%7%8"));
    m_sqlGetListId3Where1.squeeze();

    // Get list join
    m_sqlGetListJoin.append(selectJoin);
    m_sqlGetListJoin.append("T.%2=J.%3 WHERE %4 ORDER BY %5");
    m_sqlGetListJoin.squeeze();

    // Get list join 1 where 1
    m_sqlGetListJoin1Where1.append(selectJoin);
    m_sqlGetListJoin1Where1.append("T.ID=J.%2 WHERE J.%3%4? ORDER BY T.%5%6");
    m_sqlGetListJoin1Where1.squeeze();

    // Id last query
    m_sqllastId.append("SELECT last_insert_rowid() FROM ");
    m_sqllastId.append(table());
    m_sqllastId.squeeze();

    // Modify
    m_sqlModify.append("UPDATE ");
    m_sqlModify.append(table()).append(" SET ");
    for(szt i = 1; i != m_info.nbrAtt(); ++i)
        m_sqlModify.append(attribut(i)).append("=?,");
    m_sqlModify.chop(1);
    m_sqlModify.append(" WHERE ").append(idEgal);
    m_sqlModify.squeeze();
}
}
#endif // MANAGERSQL_H
