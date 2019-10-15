/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

/*! \defgroup groupeManager Manageurs
 * \brief Ensemble de classes représentant les managers des entités de la base de donnée.
 */

#include <exception>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <QVector>
#include "MapPtr.h"
#include "Tree.h"
#include "VectorPtr.h"
#include "Entity.h"


/*! \ingroup groupeManager
 * \brief Espace de nom pour la base de donnée.
 */
namespace bdd {
    //! Autorisation de modification d'une entité.
    enum Autorisation{Toute = 1,
                      Modif = 2,
                      Suppr = 3};
    //! Identifiant pour les fonctions d'agrega.
    enum Agrega {Max,
                Min,
                Moyenne,
                Nombre,
                Somme,
                NbrAgrega};

    //! Les différents types de conditions
    enum Condition {Egal,
                   Is,
                   Inf,
                   Sup,
                   InfEgal,
                   SupEgal,
                   Different,
                   IsNot};

    //! Type de donnée pour la base de donnée
    enum TypeAttributBdd{Blob,
                   Bool,
                   Date,
                   DateTime,
                   Double,
                   Integer,
                   Numeric,
                   Primary,
                   Real,
                   Text,
                   Variant,
                   NbrType};

    /*! \brief Les différents cas des résultats des tests d'existence d'unicité. 
     * (concordance d'indentifiant: ou bien l'identifiant en base de donnée et le même celui de l'entité testée ou bien ce dernier est nul.)
     * 
     */ 
    enum ExisteUni {Aucun = 0,  //!< Aucun des ensembles d'unicité n'existent en base de donnée.
            Tous = 1,   //!< Tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Meme = 2,   //!< Certains mais pas tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Autre = 3,  //!< Certains des ensembles d'unicité existent en base de donnée dans la même entité avec un autre identifiant.
            Conflit = 4 //!< Certains des ensembles d'unicité existent en base de donnée dans des entités différentes.
            };

    //! Enumeration des différent type de sauvegarde d'un arbre.
    enum TreeSave {EntityOnly,
                   AddLeaf,
                   WithoutDelete,
                   InternalChange,
                   ExternalChange,
                   EntityOnlyWhitoutRoot,
                   AddLeafWhitoutRoot,
                   WithoutDeleteWhitoutRoot,
                   InternalChangeWhitoutRoot,
                   ExternalChangeWhitoutRoot};
}

/*! \ingroup groupeManager
 * \brief Classe abstraite de base des manageurs.
 *
 * Classe abstraite de base des manageurs.
 * Un manageur est un gestionnaire associé à un type d'entité,
 * permettant de faire le lien entre ce type d'entité et la table correspondante en base donnée.
 * Cette classe joue le rôle d'interface pour les différents manageurs.
 */
class AbstractManager
{
public:   
    //! Constructeur.
    AbstractManager() = default;

    //! Destructeur.
    virtual ~AbstractManager(); //default

    //! Crée dans la base de donnée la table associée à l'entité du manageur.
    virtual void creer() = 0;

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    virtual bool exists(const Entity & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui de l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    virtual bdd::ExisteUni existsUnique(Entity & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    virtual bdd::ExisteUni existsUnique(const Entity & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    virtual QPair<bdd::ExisteUni,int> existsUniqueId(const Entity & entity) = 0;

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    virtual bool del(int id) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get(Entity & entity) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant les mêmes valeurs pour les attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool getUnique(Entity & entity) = 0;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes valeurs d'attributs (identifiant compris).
    virtual bool sameInBdd(const Entity & entity) = 0;

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    virtual void save(Entity & entity) = 0;

    //! Enregistre l'entité entity en base de donnée.
    virtual void save(const Entity & entity) = 0;

protected:
    //! Message d'erreurs si l'entité entity n'est pas valide.
    virtual QString messageErreurs(const Entity & entity) const = 0;

    //! Message d'erreurs s'il existe déjà en base de donnée une entité ayant les mêmes valeurs d'attributs uniques que entity.
    virtual QString messageErreursUnique(const Entity & entity) const = 0;
};

/*! \ingroup groupeManager
 * \brief Classe abstraite de base des manageurs associée à l'entité Ent.
 * Regroupe les méthodes virtuelles nécessaire à la base de donnée propre à l'entité Ent.
 */
template<class Ent> class AbstractManagerTemp : public AbstractManager
{
public:
    //! Constructeur.
    AbstractManagerTemp() = default;

    //! Destructeur.
    ~AbstractManagerTemp() override = default;

    // Méthode d'existence
    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    virtual bool exists(const Ent & entity) = 0;

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    bool exists(const Entity & entity) override
        {return exists(Ent::convert(entity));}

    //! Teste s'il existe une entité vérifiant une condition en base de donnée.
    virtual bool exists(typename Ent::Position cle, const QVariant & value, bdd::Condition cond = bdd::Condition::Egal) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques qu'entity en base de donnée.
    //! De plus, si l'identifiant de entity est nul et qu'il existe en base de donnée exactement une entité
    //! possédant des ensembles d'attributs uniques avec les mêmes valeurs qu'entity, alors l'identifiant d'entity
    //! est remplacé par l'identifiant de cette entité.
    virtual bdd::ExisteUni existsUnique(Ent & entity) = 0;

    //! Idem bdd::ExisteUni existsUnique(Ent & entity) avec une conversion de référence.
    bdd::ExisteUni existsUnique(Entity & entity) override
        {return existsUnique(Ent::convert(entity));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques qu'entity en base de donnée.
    virtual bdd::ExisteUni existsUnique(const Ent & entity) = 0;

    //! Idem bdd::ExisteUni existsUnique(const Ent & entity) avec une conversion de référence.
    bdd::ExisteUni existsUnique(const Entity & entity) override
        {return existsUnique(Ent::convert(entity));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    virtual QPair<bdd::ExisteUni,int> existsUniqueId(const Ent & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    QPair<bdd::ExisteUni,int> existsUniqueId(const Entity & entity) override
        {return existsUniqueId(Ent::convert(entity));}

    // Méthode d'aggrégation
    //! Fonction d'agrega de valeur de type QVariant sur l'attribut att appliquée à toutes les entités de la table.
    virtual QVariant fonctionAgrega(bdd::Agrega fonc, typename Ent::Position att) = 0;

    //! Fonction d'agrega de valeur de type T sur l'attribut att appliquée à toutes les entités vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    virtual QVariant fonctionAgrega(bdd::Agrega /*fonc*/, typename Ent::Position /*att*/, typename Ent::Position /*cle*/,
                                       const QVariant & /*value*/, bdd::Condition /*cond*/ = bdd::Condition::Egal) = 0;

    //! Fonction d'agrega de valeur de type T sur l'attribut att appliquée à toutes les entités vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    virtual QVariant fonctionAgrega(bdd::Agrega /*fonc*/, typename Ent::Position /*att*/, typename Ent::Position /*cle1*/,
                                       const QVariant & /*value1*/, typename Ent::Position /*cle2*/,  const QVariant & /*value2*/,
                                       bdd::Condition /*cond1*/ = bdd::Condition::Egal,
                                       bdd::Condition /*cond2*/ = bdd::Condition::Egal) = 0;

    // Méthodes de demande
    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get(Ent & entity) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get(Entity & entity) override
        {return get(Ent::convert(entity));}

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    virtual bool getAutorisation(const Ent & /*entity*/, bdd::Autorisation /*autorisation*/)
        {return true;}

    // Obtention d'un arbre.
    //! Renvoie l'arbre de toutes les entités.
    virtual Tree<Ent> getArbre();

    //! Renvoie l'arbre de racine entity.
    virtual Tree<Ent> getArbre(const Ent & entity);

    //! Renvoie l'arbre de racine d'identifiant id pour une entité de type arbre.
    virtual Tree<Ent> getArbre(int id);

    // Obtention d'une liste.
    //! Renvoie la liste des entités de la table des entités Ent ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position ordre = Ent::Id, bool croissant = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, int value, typename Ent::Position ordre = Ent::Id,
                                 bdd::Condition cond = bdd::Condition::Egal, bool crois = true)
        {return getList(cle,QVariant(value),ordre,cond,crois);}

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value, typename Ent::Position ordre = Ent::Id,
                                 bdd::Condition cond = bdd::Condition::Egal, bool crois = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position order1, typename Ent::Position order2,
                                 bdd::Condition cond = bdd::Condition::Egal,
                                 bool croissant1 = true, bool croissant2 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1, ordre2 puis ordre3.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position ordre1, typename Ent::Position ordre2, typename Ent::Position ordre3,
                                 bdd::Condition cond = bdd::Condition::Egal,
                                 bool crois1 = true, bool crois2 = true, bool crois3 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle1, const QVariant & value1,
                                             typename Ent::Position cle2, const QVariant & value2,
                                             typename Ent::Position ordre = Ent::Id,
                                             bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal,
                                             bool crois = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle1, const QVariant & value1,
                                             typename Ent::Position cle2, const QVariant & value2,
                                             typename Ent::Position cle3, const QVariant & value3,
                                             typename Ent::Position ordre = Ent::Id,
                                             bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal,
                                             bdd::Condition cond3 = bdd::Condition::Egal,
                                             bool crois = true) = 0;

    //! Renvoie la liste des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre, croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    virtual ListPtr<Ent> getList(const QList<typename Ent::Position> & cle, const QList<QVariant> & value,
                         const QList<typename Ent::Position> & ordre = QList<typename Ent::Position>(),
                         const QList<bdd::Condition> & condition = QList<bdd::Condition>(), const QList<bool> & crois = QList<bool>()) = 0;

    //! Renvoie la liste des entités de la table vérifiant la condition.
    virtual ListPtr<Ent> getList(const QString & condition) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual QList<int> getListId(typename Ent::Position cle, int value, typename Ent::Position ordre = Ent::Id,
                                 bdd::Condition cond = bdd::Condition::Egal, bool crois = true)
    {return getListId(cle, QVariant(value), ordre, cond, crois);}

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual QList<int> getListId(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position ordre = Ent::Id,
                                 bdd::Condition cond = bdd::Condition::Egal, bool crois = true) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    virtual QList<int> getListId(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position order1, typename Ent::Position order2,
                                 bdd::Condition cond = bdd::Condition::Egal,
                                 bool croissant1 = true, bool croissant2 = true) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1, ordre2 puis ordre3.
    virtual QList<int> getListId(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position ordre1, typename Ent::Position ordre2, typename Ent::Position ordre3,
                                 bdd::Condition cond = bdd::Condition::Egal,
                                 bool crois1 = true, bool crois2 = true, bool crois3 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual QList<int> getListId(typename Ent::Position cle1, const QVariant & value1,
                                 typename Ent::Position cle2, const QVariant & value2,
                                 typename Ent::Position ordre = Ent::Id,
                                 bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal,
                                 bool crois = true) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual QList<int> getListId(typename Ent::Position cle1, const QVariant & value1,
                                 typename Ent::Position cle2, const QVariant & value2,
                                 typename Ent::Position cle3, const QVariant & value3,
                                 typename Ent::Position ordre = Ent::Id,
                                 bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal,
                                 bdd::Condition cond3 = bdd::Condition::Egal,
                                 bool crois = true) = 0;

    //! Renvoie la liste des identifiants de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre, croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    virtual QList<int> getListId(const QList<typename Ent::Position> & cle, const QList<QVariant> & value,
                         const QList<typename Ent::Position> & ordre = QList<typename Ent::Position>(),
                         const QList<bdd::Condition> & condition = QList<bdd::Condition>(),
                         const QList<bool> & crois = QList<bool>()) = 0;

    //! Renvoie le liste des descendant direct d'entity.
    virtual ListPtr<Ent> getListChilds(const Ent & /*entity*/)
        {return ListPtr<Ent>();}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    virtual QList<int> getListChildsId(int /*id*/)
        {return QList<int>();}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    virtual QList<QPair<int,bool>> getListChildsIdLeaf(int /*id*/, bool /*ordre*/ = true)
        {return QList<QPair<int,bool>>();}

    // Liste de Jointure
    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de QMap whereMapTable,
    //! valeur des colonnes de la table Join key = value de QMap whereMapJoin,
    //! ordonnée suivant les colonnes de la table Ent d'identifiants key et d'ordre value de QMap orderMapTable
    //! (true -> croissant, false -> décroissant).
    virtual ListPtr<Ent> getListJoin(const QString & tableJoin, int colonneTable,
                                     const QString & colonneJoin,
                                     const QMap<int,QVariant> & whereMapTable,
                                     const QMap<QString,QVariant> & whereMapJoin,
                                     const QList<QPair<int,bool>> & orderMapTable) = 0;

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonneJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    virtual ListPtr<Ent> getListJoin(const QString & tableJoin,
                                     const QString & colonneJoin,
                                     const QString & whereJoin,
                                     const QVariant & valueWhere,
                                     typename Ent::Position ordre = Ent::Id,
                                     bdd::Condition cond = bdd::Condition::Egal, bool crois = true) = 0;

    // Map
    //! Renvoie la map des entités de la table des entités Ent.
    virtual MapPtr<Ent> getMap(typename Ent::Position cleMap = Ent::Id) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    virtual MapPtr<Ent> getMap(typename Ent::Position cle, const QVariant & value,
                               typename Ent::Position cleMap = Ent::Id, bdd::Condition cond = bdd::Condition::Egal) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    virtual MapPtr<Ent> getMap(typename Ent::Position cle1, const QVariant & value1,
                               typename Ent::Position cle2, const QVariant & value2,
                               typename Ent::Position cleMap = Ent::Id,
                               bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3.
    virtual MapPtr<Ent> getMap(typename Ent::Position cle1, const QVariant & value1,
                               typename Ent::Position cle2, const QVariant & value2,
                               typename Ent::Position cle3, const QVariant & value3,
                               typename Ent::Position cleMap = Ent::Id,
                               bdd::Condition cond1 = bdd::Condition::Egal, bdd::Condition cond2 = bdd::Condition::Egal,
                               bdd::Condition cond3 = bdd::Condition::Egal) = 0;

    //! Renvoie la map des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre, croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    virtual MapPtr<Ent> getMap(const QList<typename Ent::Position> & cle, const QList<QVariant> & value,
                         typename Ent::Position cleMap = Ent::Id,
                         const QList<typename Ent::Position> & ordre = QList<typename Ent::Position>(),
                         const QList<bdd::Condition> & condition = QList<bdd::Condition>(),
                         const QList<bool> & crois = QList<bool>()) = 0;

    // Map de Jointure
    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de QMap whereMapTable,
    //! valeur des colonnes de la table Join key = value de QMap whereMapJoin.
    virtual MapPtr<Ent> getMapJoin(const QString & tableJoin, int colonneTable,
                                   const QString & colonneJoin,
                                   const QMap<int,QVariant> & whereMapTable,
                                   const QMap<QString,QVariant> & whereMapJoin,
                                   typename Ent::Position cleMap = Ent::Id) = 0;

    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonneJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere.
    virtual MapPtr<Ent> getMapJoin(const QString & tableJoin,
                                   const QString & colonneJoin,
                                   const QString & whereJoin,
                                   const QVariant & valueWhere,
                                   typename Ent::Position cleMap = Ent::Id,
                                   bdd::Condition cond = bdd::Condition::Egal) = 0;

    //! Renvoie la liste des restrictions de modification de l'entité donnée en argument.
    virtual QList<int> getRestriction(const Ent & /*entity*/)
        {return QList<int>();}

    //! Renvoie le vecteur des descendant direct d'entity.
    virtual VectorPtr<Ent> getVectorChilds(const Ent & /*entity*/)
        {return VectorPtr<Ent>();}

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant les mêmes valeurs pour au moins un ensemble des attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool getUnique(Ent & entity) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant les mêmes valeurs pour les attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool getUnique(Entity & entity) override
        {return getUnique(Ent::convert(entity));}

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    virtual bool sameInBdd(const Ent & entity) = 0;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool sameInBdd(const Entity &entity) override
        {return sameInBdd(Ent::convert(entity));}

    //! Teste s'il y a dans la base de donnée une entité d'identifiant id ayant exactement les mêmes attributs que entity.
    virtual bool sameInBdd(const Ent & entity, int id) = 0;

    // Save
    //! Enregistre l'entity dans la base de donnée.
    virtual void save(Ent & entity) = 0;

    //! Enregistre l'entity dans la base de donnée.
    virtual void save(const Ent & entity) = 0;

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    void save(Entity & entity) override
        {save(Ent::convert(entity));}

    //! Enregistre l'entité entity en base de donnée.
    void save(const Entity & entity) override
        {save(Ent::convert(entity));}

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
    virtual void save(Ent & entity, bdd::Autorisation autorisation, bool bb = false)
    {
        save(entity);
        setAutorisation(entity, autorisation, bb);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle autorisation de modification.
    virtual void save(const Ent & entity, bdd::Autorisation autorisation, bool bb = false)
    {
        save(entity);
        setAutorisation(entity, autorisation, bb);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisations de modification.
    virtual void save(Ent & entity, const QMap<bdd::Autorisation,bool> & autorisations)
    {
        save(entity);
        setAutorisation(entity, autorisations);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles autorisations de modification.
    virtual void save(const Ent & entity, const QMap<bdd::Autorisation,bool> & autorisations)
    {
        save(entity);
        setAutorisation(entity, autorisations);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restriction de modification.
    virtual void save(Ent & entity, const QList<bdd::Autorisation> restriction)
    {
        save(entity);
        setRestriction(entity, restriction);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelle restriction de modification.
    virtual void save(const Ent & entity, const QList<bdd::Autorisation> restriction)
    {
        save(entity);
        setRestriction(entity, restriction);
    }

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(Ent & entity, const Ent & parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(const Ent & entity, const Ent & parent, int num = 0);

    //! Enregistre l'arbre d'entités dans la base de donnée pour les entités de type arbre.
    virtual void save(Tree<Ent> & arbre, bdd::TreeSave n = bdd::TreeSave::ExternalChange);

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    virtual void saveUnique(Ent & entity) = 0;

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    virtual void saveUnique(const Ent & entity) = 0;

    //! Modifie une autorisation de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, bdd::Autorisation /*autorisation*/, bool bb = false)
    {
        if(!bb)
            throw std::invalid_argument(QString("Les entités du type de l'entité suivante ne peuvent avoir "
                                                "de restriction de modification.\n")
                                        .append(entity.affiche()).append(".").toStdString());
    }

    //! Modifie les autorisations de modification pour une entité donnée.
    virtual void setAutorisation(const Ent & entity, const QMap<bdd::Autorisation,bool> & autorisations)
    {
        QMap<bdd::Autorisation,bool>::const_iterator i = autorisations.cbegin();
        while(i != autorisations.cend() && i.value()) i++;
        if(i != autorisations.cend())
            throw std::invalid_argument(QString("Les entités du type de l'entité suivante ne peuvent avoir "
                                                "de restriction de modification.\n")
                                        .append(entity.affiche()).append(".").toStdString());
    }

    //! Ajoute des restrictions de modification pour une entité donnée.
    virtual void setRestriction(const Ent & entity, const QList<bdd::Autorisation> restriction)
    {
        if(!restriction.isEmpty())
            throw std::invalid_argument(QString("Les entités du type de l'entité suivante ne peuvent avoir "
                                                "de restriction de modification.\n")
                                        .append(entity.affiche()).append(".").toStdString());
    }
};

template<class Ent> Tree<Ent> AbstractManagerTemp<Ent>::getArbre()
    {throw std::invalid_argument(QString("La méthode 'getArbre' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> Tree<Ent> AbstractManagerTemp<Ent>::getArbre(const Ent & /*entity*/)
    {throw std::invalid_argument(QString("La méthode 'getArbre' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> Tree<Ent> AbstractManagerTemp<Ent>::getArbre(int /*id*/)
    {throw std::invalid_argument(QString("La méthode 'getArbre' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

/*template<class Ent> VectorPtr<Ent> AbstractManagerTemp<Ent>::getVectorChilds(const Ent & entity)
    {throw std::invalid_argument(QString("La méthode 'ListPtr<Ent> getListChilds(const Ent & entity)' "
                                         "n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}*/

template<class Ent> void AbstractManagerTemp<Ent>::save(Ent & /*entity*/, const Ent & /*parent*/, int /*num*/)
    {throw std::invalid_argument(QString("La méthode 'save(Ent & entity, const Ent & parent, int num)' "
                                         "n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> void AbstractManagerTemp<Ent>::save(const Ent & /*entity*/, const Ent & /*parent*/, int /*num*/)
{throw std::invalid_argument(QString("La méthode 'save(const Ent & entity, const Ent & parent, int num)' "
                                     "n'est pas définie pour le manager des : ")
                             .append(Ent::Name()).append(".").toStdString());}

template<class Ent> void AbstractManagerTemp<Ent>::save(Tree<Ent> & /*arbre*/, bdd::TreeSave /*n*/)
    {throw std::invalid_argument(QString("La méthode 'save(Tree<Ent>,...)' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}
#endif // ABSTRACTMANAGER_H
