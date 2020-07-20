/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACTMANAGER_H
#define ABSTRACTMANAGER_H

#include <exception>
#include <map>
#include <memory>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <vector>
#include "ConteneurPtr.h"
#include "InfoBdd.h"
#include "Tree.h"

/*! \ingroup groupeManager
 * \brief Espace de nom des managers.
 */
namespace managerMPS {
using namespace typeMPS;
using namespace conteneurMPS;
namespace bmps = bddMPS;
using Entity = entityMPS::Entity;
using InfoBdd = bmps::InfoBdd;

/*! \ingroup groupeManager
 * \brief Exception lancé par une condition d'unicité mis en défaut.
 */
class UniqueEntityException : public entityMPS::EntityException {
public:
    //! Constructeur par defaut.
    UniqueEntityException() = default;

    //! Constructeur.
    UniqueEntityException(const QString & txt,const Entity & entity) {
        m_txt = txt;
        m_txt.append("\nErreur, conflit sur une condition d'unicité de la base de donnée:\n");
        m_txt.append("il existe déjà dans la base de donnée une entrée "
                     "ayant les mêmes valeurs d'attributs unique que cette nouvelle entrée:\n");
        m_txt.append(entity.affiche());
    }

    //! Constructeur.
    UniqueEntityException(const QString & txt,const Entity & entity, const Entity & entBdd)
        : UniqueEntityException(txt,entity) {
        m_txt.append("\nL'entrée suivante est déjà présente dans la base de donnée:\n");
        m_txt.append(entBdd.affiche());
    }

    //! Destructeur.
    ~UniqueEntityException();
};


/*! \ingroup groupeManager
 * \brief Classe abstraite de base des manageurs.
 *
 * Classe abstraite de base des manageurs.
 * Un manageur est un gestionnaire associé à un type d'entité,
 * permettant de faire le lien entre ce type d'entité et la table correspondante en base donnée.
 * Cette classe joue le rôle d'interface pour les différents manageurs.
 */
class AbstractManager {
public:   
    //! Constructeur.
    AbstractManager() = default;

    //! Destructeur.
    virtual ~AbstractManager(); //default

    //! Ajoute une restriction de modification pour l'entité d'identifiant id.
    virtual void addRestriction(idt /*id*/, flag restrict) {
        if(restrict)
            throw std::invalid_argument(QString("void AbstractManager::addRestriction(idt, flag) :\n"
                                                "Les entités du type ").append(name())
                                        .append(" ne peuvent avoir de restriction de modification.\n").toStdString());
    }

    //! Crée dans la base de donnée la table associée à l'entité du manageur.
    virtual void creer() = 0;

    //! Accesseur des restrictions gérées.
    virtual flag enableRestriction() const
        {return bmps::Aucune;}

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    virtual bool exists(const Entity & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui de l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    virtual bmps::existeUni existsUnique(Entity & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    virtual bmps::existeUni existsUnique(const Entity & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    virtual std::pair<bmps::existeUni,idt> existsUniqueId(const Entity & entity) = 0;

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    virtual bool del(idt id) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get(Entity & entity) = 0;

    //! Renvoie la liste des restrictions de modification de l'entité d'identifiant id.
    virtual flag getRestriction(idt /*id*/)
         {return bddMPS::Aucune;}

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant les mêmes valeurs pour les attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool getUnique(Entity & entity) = 0;

    //! Renvoie les informations de la table associée au manager.
    virtual const InfoBdd & info() const = 0;

    //! Renvoie les informations de la table arbre associée au manager.
    virtual InfoBdd infoArbre() const
        {return InfoBdd();}

    //! Renvoie un pointeur sur une entité du type géré par le manager.
    virtual std::unique_ptr<Entity> makeEntity() const = 0;

    //! Retourne le nom de l'entité associée au manager.
    QString name() const
        {return info().name();}

    //! Retourne un vecteur contenant les noms des attributs.
    virtual std::vector<QString> namesAttributs() const = 0;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes valeurs d'attributs (identifiant compris).
    virtual bool sameInBdd(const Entity & entity) = 0;

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    virtual void save(Entity & entity) = 0;

    //! Enregistre l'entité entity en base de donnée.
    virtual void save(const Entity & entity) = 0;

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle restriction de modification.
    virtual void save(Entity & entity, flag restict) {
        saveByPass(entity);
        setRestriction(entity.id(), restict);
    }

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    virtual void save(Entity & entity, const Entity & parent, int num = 0) = 0;

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    virtual void save(const Entity & entity, const Entity & parent, int num = 0) = 0;

    //! Sauve l'entité en ignorant les restrictions.
    virtual void saveByPass(Entity & entity)
        {save(entity);}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    virtual void saveUnique(Entity & entity) = 0;

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    virtual void saveUnique(const Entity & entity) = 0;

    //! Modifie la restriction de modification pour l'entité d'identifiant id.
    virtual void setRestriction(idt /*id*/, flag restrict) {
        if(restrict)
            throw std::invalid_argument(QString("void AbstractManager::setRestriction(idt, flag) :\n"
                                                "Les entités du type ").append(name())
                                        .append(" ne peuvent avoir de restriction de modification.\n").toStdString());
    }

    //! Test la restriction de modification de l'entité d'identifiant id.
    virtual bool testRestriction(idt /*id*/, flag /*restrict*/)
        {return false;}

    //! Retourne le type du manager.
    virtual flag typeManager() const
        {return bmps::NormalTypeManager;}

protected:
    //! Message d'erreurs si l'entité entity n'est pas valide.
    virtual QString messageErreurs(const Entity & entity) const = 0;

    //! Message d'erreurs s'il existe déjà en base de donnée une entité ayant les mêmes valeurs d'attributs uniques que entity.
    //virtual QString messageErreursUnique(const Entity & entity) const = 0;
};

/*! \ingroup groupeManager
 * \brief Classe abstraite de base des manageurs associée à l'entité Ent.
 * Regroupe les méthodes virtuelles nécessaire à la base de donnée propre à l'entité Ent.
 */
template<class Ent> class AbstractManagerTemp : public AbstractManager {
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
        {return exists(Ent::Convert(entity));}

    //! Teste s'il existe une entité vérifiant une condition en base de donnée.
    virtual bool exists(typename Ent::Position cle, const QVariant & value, bmps::condition cond = bmps::condition::Egal) = 0;

    //! Teste s'il existe une entité vérifiant des conditions.
    virtual bool exists(const std::map<typename Ent::Position, QVariant> & value,
                std::vector<bmps::condition> cond = std::vector<bmps::condition>({bmps::condition::Egal})) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques qu'entity en base de donnée.
    //! De plus, si l'identifiant de entity est nul et qu'il existe en base de donnée exactement une entité
    //! possédant des ensembles d'attributs uniques avec les mêmes valeurs qu'entity, alors l'identifiant d'entity
    //! est remplacé par l'identifiant de cette entité.
    virtual bmps::existeUni existsUnique(Ent & entity) = 0;

    //! Idem bmps::existeUni existsUnique(Ent & entity) avec une conversion de référence.
    bmps::existeUni existsUnique(Entity & entity) override
        {return existsUnique(Ent::Convert(entity));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques qu'entity en base de donnée.
    virtual bmps::existeUni existsUnique(const Ent & entity) = 0;

    //! Idem bmps::existeUni existsUnique(const Ent & entity) avec une conversion de référence.
    bmps::existeUni existsUnique(const Entity & entity) override
        {return existsUnique(Ent::Convert(entity));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    virtual std::pair<bmps::existeUni,idt> existsUniqueId(const Ent & entity) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    std::pair<bmps::existeUni,idt> existsUniqueId(const Entity & entity) override
        {return existsUniqueId(Ent::Convert(entity));}

    // Méthode d'aggrégation
    //! Fonction d'agrega de valeur de type QVariant sur l'attribut att appliquée à toutes les entités de la table.
    virtual QVariant fonctionAgrega(bmps::agrega fonc, typename Ent::Position att) = 0;

    //! Fonction d'agrega de valeur de type T sur l'attribut att appliquée à toutes les entités vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    virtual QVariant fonctionAgrega(bmps::agrega /*fonc*/, typename Ent::Position /*att*/, typename Ent::Position /*cle*/,
                                       const QVariant & /*value*/, bmps::condition /*cond*/ = bmps::condition::Egal) = 0;

    //! Fonction d'agrega de valeur de type T sur l'attribut att appliquée à toutes les entités vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    virtual QVariant fonctionAgrega(bmps::agrega /*fonc*/, typename Ent::Position /*att*/, typename Ent::Position /*cle1*/,
                                       const QVariant & /*value1*/, typename Ent::Position /*cle2*/,  const QVariant & /*value2*/,
                                       bmps::condition /*cond1*/ = bmps::condition::Egal,
                                       bmps::condition /*cond2*/ = bmps::condition::Egal) = 0;

    // Méthodes de demande
    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get(Ent & entity) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get(Entity & entity) override
        {return get(Ent::Convert(entity));}

    // Obtention d'un arbre.
    //! Renvoie l'arbre de toutes les entités.
    virtual tree<Ent> getArbre();

    //! Renvoie l'arbre de racine entity.
    virtual tree<Ent> getArbre(const Ent & entity);

    //! Renvoie l'arbre de racine d'identifiant id pour une entité de type arbre.
    virtual tree<Ent> getArbre(idt id);

    // Obtention d'une liste.
    //! Renvoie la liste des entités de la table des entités Ent ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position ordre = Ent::Id, bool croissant = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, int value, typename Ent::Position ordre = Ent::Id,
                                 bmps::condition cond = bmps::condition::Egal, bool crois = true)
        {return getList(cle,QVariant(value),ordre,cond,crois);}

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, idt value, typename Ent::Position ordre = Ent::Id,
                                 bmps::condition cond = bmps::condition::Egal, bool crois = true)
        {return getList(cle,QVariant(value),ordre,cond,crois);}

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value, typename Ent::Position ordre = Ent::Id,
                                 bmps::condition cond = bmps::condition::Egal, bool crois = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position order1, typename Ent::Position order2,
                                 bmps::condition cond = bmps::condition::Egal,
                                 bool croissant1 = true, bool croissant2 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1, ordre2 puis ordre3.
    virtual ListPtr<Ent> getList(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position ordre1, typename Ent::Position ordre2, typename Ent::Position ordre3,
                                 bmps::condition cond = bmps::condition::Egal,
                                 bool crois1 = true, bool crois2 = true, bool crois3 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle1, const QVariant & value1,
                                             typename Ent::Position cle2, const QVariant & value2,
                                             typename Ent::Position ordre = Ent::Id,
                                             bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                                             bool crois = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual ListPtr<Ent> getList(typename Ent::Position cle1, const QVariant & value1,
                                             typename Ent::Position cle2, const QVariant & value2,
                                             typename Ent::Position cle3, const QVariant & value3,
                                             typename Ent::Position ordre = Ent::Id,
                                             bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                                             bmps::condition cond3 = bmps::condition::Egal,
                                             bool crois = true) = 0;

    //! Renvoie la liste des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre, croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    virtual ListPtr<Ent> getList(const std::vector<typename Ent::Position> & cle, const std::vector<QVariant> & value,
                         const std::vector<typename Ent::Position> & ordre = std::vector<typename Ent::Position>(),
                         const std::vector<bmps::condition> & condition = std::vector<bmps::condition>(),
                                 const std::vector<bool> & crois = std::vector<bool>()) = 0;

    //! Renvoie la liste des entités de la table vérifiant la condition.
    virtual ListPtr<Ent> getList(const QString & condition) = 0;

    //! Renvoie le liste des descendant direct d'entity.
    virtual VectorPtr<Ent> getListChilds(const Ent & /*entity*/)
        {return VectorPtr<Ent>();}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    virtual std::list<idt> getListChildsId(idt /*id*/)
        {return std::list<idt>();}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    virtual std::vector<std::pair<idt,bool>> getListChildsIdLeaf(idt /*id*/)
        {return std::vector<std::pair<idt,bool>>();}

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> getListId(typename Ent::Position cle, int value, typename Ent::Position ordre = Ent::Id,
                                 bmps::condition cond = bmps::condition::Egal, bool crois = true)
        {return getListId(cle, QVariant(value), ordre, cond, crois);}

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> getListId(typename Ent::Position cle, idt value, typename Ent::Position ordre = Ent::Id,
                                 bmps::condition cond = bmps::condition::Egal, bool crois = true)
        {return getListId(cle, QVariant(value), ordre, cond, crois);}

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> getListId(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position ordre = Ent::Id,
                                 bmps::condition cond = bmps::condition::Egal, bool crois = true) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    virtual std::list<idt> getListId(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position order1, typename Ent::Position order2,
                                 bmps::condition cond = bmps::condition::Egal,
                                 bool croissant1 = true, bool croissant2 = true) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1, ordre2 puis ordre3.
    virtual std::list<idt> getListId(typename Ent::Position cle, const QVariant & value,
                                 typename Ent::Position ordre1, typename Ent::Position ordre2, typename Ent::Position ordre3,
                                 bmps::condition cond = bmps::condition::Egal,
                                 bool crois1 = true, bool crois2 = true, bool crois3 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> getListId(typename Ent::Position cle1, const QVariant & value1,
                                 typename Ent::Position cle2, const QVariant & value2,
                                 typename Ent::Position ordre = Ent::Id,
                                 bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                                 bool crois = true) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> getListId(typename Ent::Position cle1, const QVariant & value1,
                                 typename Ent::Position cle2, const QVariant & value2,
                                 typename Ent::Position cle3, const QVariant & value3,
                                 typename Ent::Position ordre = Ent::Id,
                                 bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                                 bmps::condition cond3 = bmps::condition::Egal,
                                 bool crois = true) = 0;

    //! Renvoie la liste des identifiants de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre, croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    virtual std::list<idt> getListId(const std::vector<typename Ent::Position> & cle, const std::vector<QVariant> & value,
                         const std::vector<typename Ent::Position> & ordre = std::vector<typename Ent::Position>(),
                         const std::vector<bmps::condition> & condition = std::vector<bmps::condition>(),
                         const std::vector<bool> & crois = std::vector<bool>()) = 0;

    // Liste de Jointure
    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map whereMapTable,
    //! valeur des colonnes de la table Join key = value de std::map whereMapJoin,
    //! ordonnée suivant les colonnes de la table Ent d'identifiants key et d'ordre value de std::map orderMapTable
    //! (true -> croissant, false -> décroissant).
    virtual ListPtr<Ent> getListJoin(const QString & tableJoin, szt colonneTable,
                                     const QString & colonneJoin,
                                     const std::map<typename Ent::Position,QVariant> & whereMapTable,
                                     const std::map<QString,QVariant> & whereMapJoin,
                                     const std::vector<std::pair<typename Ent::Position,bool>> & orderMapTable) = 0;

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonneJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    virtual ListPtr<Ent> getListJoin(const QString & tableJoin,
                                     const QString & colonneJoin,
                                     const QString & whereJoin,
                                     const QVariant & valueWhere,
                                     typename Ent::Position ordre = Ent::Id,
                                     bmps::condition cond = bmps::condition::Egal, bool crois = true) = 0;

    // Map
    //! Renvoie la map des entités de la table des entités Ent.
    virtual mapIdt<Ent> getMap(typename Ent::Position cleMap = Ent::Id) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    virtual mapIdt<Ent> getMap(typename Ent::Position cle, const QVariant & value,
                               typename Ent::Position cleMap = Ent::Id, bmps::condition cond = bmps::condition::Egal) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    virtual mapIdt<Ent> getMap(typename Ent::Position cle1, const QVariant & value1,
                               typename Ent::Position cle2, const QVariant & value2,
                               typename Ent::Position cleMap = Ent::Id,
                               bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3.
    virtual mapIdt<Ent> getMap(typename Ent::Position cle1, const QVariant & value1,
                               typename Ent::Position cle2, const QVariant & value2,
                               typename Ent::Position cle3, const QVariant & value3,
                               typename Ent::Position cleMap = Ent::Id,
                               bmps::condition cond1 = bmps::condition::Egal, bmps::condition cond2 = bmps::condition::Egal,
                               bmps::condition cond3 = bmps::condition::Egal) = 0;

    //! Renvoie la map des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre, croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    virtual mapIdt<Ent> getMap(const std::vector<typename Ent::Position> & cle, const std::vector<QVariant> & value,
                         typename Ent::Position cleMap = Ent::Id,
                         const std::vector<typename Ent::Position> & ordre = std::vector<typename Ent::Position>(),
                         const std::vector<bmps::condition> & condition = std::vector<bmps::condition>(),
                         const std::vector<bool> & crois = std::vector<bool>()) = 0;

    // Map de Jointure
    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map whereMapTable,
    //! valeur des colonnes de la table Join key = value de std::map whereMapJoin.
    virtual mapIdt<Ent> getMapJoin(const QString & tableJoin, szt colonneTable,
                                   const QString & colonneJoin,
                                   const std::map<typename Ent::Position,QVariant> & whereMapTable,
                                   const std::map<QString,QVariant> & whereMapJoin,
                                   typename Ent::Position cleMap = Ent::Id) = 0;

    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonneJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere.
    virtual mapIdt<Ent> getMapJoin(const QString & tableJoin,
                                   const QString & colonneJoin,
                                   const QString & whereJoin,
                                   const QVariant & valueWhere,
                                   typename Ent::Position cleMap = Ent::Id,
                                   bmps::condition cond = bmps::condition::Egal) = 0;

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
        {return getUnique(Ent::Convert(entity));}

    //! Renvoie un pointeur sur une entité du type géré par le manager.
    std::unique_ptr<Entity> makeEntity() const override
        {return std::make_unique<Ent>();}

//    //! Retourne le nom de l'entité associée au manager.
//    QString name() const override
//        {return Ent::Name();}

    //! Retourne un vecteur contenant les noms des attributs.
    std::vector<QString> namesAttributs() const override
        {return Ent::NamesAttributs();}

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    virtual bool sameInBdd(const Ent & entity) = 0;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool sameInBdd(const Entity &entity) override
        {return sameInBdd(Ent::Convert(entity));}

    //! Teste s'il y a dans la base de donnée une entité d'identifiant id ayant exactement les mêmes attributs que entity.
    virtual bool sameInBdd(const Ent & entity, idt id) = 0;

    // Save
    //! Enregistre l'entity dans la base de donnée.
    virtual void save(Ent & entity) = 0;

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à entity.
    void save(Entity & entity) override
        {save(Ent::Convert(entity));}

    //! Enregistre l'entity dans la base de donnée.
    virtual void save(const Ent & entity) = 0;

    //! Enregistre l'entité entity en base de donnée.
    void save(const Entity & entity) override
        {save(Ent::Convert(entity));}

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle restriction de modification.
    virtual void save(Ent & entity, flag restrict) {
        saveByPass(entity);
        setRestriction(entity.id(), restrict);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle restriction de modification.
    virtual void save(const Ent & entity, flag restrict) {
        Ent ent(entity);
        saveByPass(ent);
        setRestriction(entity.id(), restrict);
    }

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    virtual void save(Ent & entity, const Ent & parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(Entity & entity, const Entity & parent, int num = 0) override
        {save(Ent::Convert(entity), Ent::Convert(parent), num);}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    virtual void save(const Ent & entity, const Ent & parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void save(const Entity & entity, const Entity & parent, int num = 0) override
        {save(Ent::Convert(entity), Ent::Convert(parent), num);}

    //! Enregistre l'arbre d'entités dans la base de donnée pour les entités de type arbre.
    virtual void save(tree<Ent> & arbre, bmps::treeSave n = bmps::treeSave::ExternalChange);

    //! Sauve l'entité en ignorant les restrictions.
    virtual void saveByPass(Ent & entity)
        {save(entity);}

//    //! Sauve l'entité en ignorant les restrictions.
//    void saveByPass(Entity & entity)
//        {save(Ent::Convert(entity));}

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
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    void saveUnique(Entity & entity) override
        {saveUnique(Ent::Convert(entity));}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    virtual void saveUnique(const Ent & entity) = 0;

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    void saveUnique(const Entity & entity) override
        {saveUnique(Ent::Convert(entity));}

    //! Teste la restriction de modification pour une entité d'identifiant id avec les valeurs de entity.
    virtual bool testRestriction(idt /*id*/, flag /*restrict*/, const Ent & /*entity*/)
        {return true;}
};

template<class Ent> tree<Ent> AbstractManagerTemp<Ent>::getArbre()
    {throw std::invalid_argument(QString("La méthode 'getArbre' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> tree<Ent> AbstractManagerTemp<Ent>::getArbre(const Ent & /*entity*/)
    {throw std::invalid_argument(QString("La méthode 'getArbre' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> tree<Ent> AbstractManagerTemp<Ent>::getArbre(idt /*id*/)
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

template<class Ent> void AbstractManagerTemp<Ent>::save(tree<Ent> & /*arbre*/, bmps::treeSave /*n*/)
    {throw std::invalid_argument(QString("La méthode 'save(tree<Ent>,...)' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}
}
#endif // ABSTRACTMANAGER_H
