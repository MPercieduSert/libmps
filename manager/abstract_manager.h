/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef ABSTRACT_MANAGER_H
#define ABSTRACT_MANAGER_H

#include <exception>
#include <map>
#include <memory>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QString>
#include <vector>
#include "conteneur_ptr.h"
#include "info_bdd.h"
#include "tree.h"

namespace mps {
/*! \ingroup groupe_manager
 *\brief Espace de nom des managers.
 */
namespace manager {
using entity = entities::entity;
using info_bdd = b2d::info_bdd;

/*! \ingroup groupe_manager
 *\brief Exception lancé par une condition d'unicité mis en défaut.
 */
class unique_entity_exception : public entities::entity_exception {
public:
    //! Constructeur par defaut.
    unique_entity_exception() = default;

    //! Constructeur.
    unique_entity_exception(const QString &txt,const entity &ent) {
        m_txt = txt;
        m_txt.append("\nErreur, conflit sur une condition d'unicité de la base de donnée:\n");
        m_txt.append("il existe déjà dans la base de donnée une entrée "
                     "ayant les mêmes valeurs d'attributs unique que cette nouvelle entrée:\n");
        m_txt.append(ent.affiche());
    }

    //! Constructeur.
    unique_entity_exception(const QString &txt,const entity &ent, const entity &ent_bdd)
        : unique_entity_exception(txt,ent) {
        m_txt.append("\nL'entrée suivante est déjà présente dans la base de donnée:\n");
        m_txt.append(ent_bdd.affiche());
    }

    //! Destructeur.
    ~unique_entity_exception();
};


/*! \ingroup groupe_manager
 *\brief Classe abstraite de base des manageurs.
 *
 *Classe abstraite de base des manageurs.
 *Un manageur est un gestionnaire associé à un type d'entité,
 *permettant de faire le lien entre ce type d'entité et la table correspondante en base donnée.
 *Cette classe joue le rôle d'interface pour les différents manageurs.
 */
class abstract_manager {
protected:
    enum {No_Child = 0};
public:   
    //! Constructeur.
    abstract_manager() = default;

    //! Destructeur.
    virtual ~abstract_manager(); //default

    //! Ajoute une restriction de modification pour l'entité d'identifiant id.
    virtual void add_restriction(idt /*id*/, flag restrict) {
        if(restrict)
            throw std::invalid_argument(QString("void abstract_manager::add_restriction(idt, flag) :\n"
                                                "Les entités du type ").append(name())
                                        .append(" ne peuvent avoir de restriction de modification.\n").toStdString());
    }

    //! Crée dans la base de donnée la table associée à l'entité du manageur.
    virtual void creer() = 0;

    //! Accesseur des restrictions gérées.
    virtual flag enable_restriction() const
        {return b2d::Aucune;}

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    virtual bool exists(const entity &ent) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    //! Si le test est positif, l'identitfiant de l'entité entity est remplacé par celui de l'entité en base de donnée
    //! ayant les mêmes valeurs d'attributs uniques.
    virtual b2d::existe_uni exists_unique(entity &ent) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée.
    virtual b2d::existe_uni exists_unique(const entity &ent) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    virtual std::pair<b2d::existe_uni,idt> exists_unique_id(const entity &ent) = 0;

    //! Supprime de la table en base de donnée l'entité d'identifiant id.
    virtual bool del(idt id) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que ent.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get(entity &ent) = 0;

    //! Renvoie l'identifiant du descendant direct du parent d'identifiant id_parent de position pos dans la fratrie.
    virtual idt get_id_child(idt id_parent, int num) = 0;

    //! Renvoie l'identifiant du parent (si le manager est de type arbre).
    virtual idt get_id_parent(idt id) = 0;

    //! Renvoie la référence du parent (si le manager est de type arbre).
    virtual QString get_ref_parent(idt id) = 0;

    //! Renvoie la liste des restrictions de modification de l'entité d'identifiant id.
    virtual flag get_restriction(idt /*id*/)
         {return b2d::Aucune;}

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant les mêmes valeurs pour les attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get_unique(entity &ent) = 0;

    //! Renvoie la liste des entités de la table ordonnée suivant les identifiants croissants.
    virtual vector_ptr<entity> get_vector_entity() = 0;

    //! Renvoie les informations de la table associée au manager.
    virtual const info_bdd &info() const = 0;

    //! Renvoie les informations de la table arbre associée au manager.
    virtual info_bdd info_arbre() const
        {return info_bdd();}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    virtual void insert(entity &ent, idt id_parent, int num = 0) = 0;

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    virtual void insert(const entity &ent, idt id_parent, int num = 0) = 0;

    //! Teste si une entité d'identifiant id est une feuille
    //! Retourne true si l'entité n'est pas de type arbre ou n'existe pas.
    virtual bool is_leaf(idt /*id*/)
        {return true;}

    //! Renvoie un pointeur sur une entité du type géré par le manager.
    virtual std::unique_ptr<entity> make_entity() const = 0;

    //! Retourne le nom de l'entité associée au manager.
    QString name() const
        {return info().name();}

    //! Retourne un vecteur contenant les noms des attributs.
    virtual std::vector<QString> attribut_names() const = 0;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes valeurs d'attributs (identifiant compris).
    virtual bool same_in_bdd(const entity &ent) = 0;

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à ent.
    virtual void save(entity &ent) = 0;

    //! Enregistre l'entité entity en base de donnée.
    virtual void save(const entity &ent) = 0;

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle restriction de modification.
    virtual void save(entity &ent, flag restict) {
        save_by_pass(ent);
        set_restriction(ent.id(),restict);
    }

    //! Sauve l'entité en ignorant les restrictions.
    virtual void save_by_pass(entity &ent)
        {save(ent);}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    virtual void save_unique(entity &ent) = 0;

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    virtual void save_unique(const entity &ent) = 0;

    //! Modifie la restriction de modification pour l'entité d'identifiant id.
    virtual void set_restriction(idt /*id*/, flag restrict) {
        if(restrict)
            throw std::invalid_argument(QString("void abstract_manager::set_restriction(idt, flag) :\n"
                                                "Les entités du type ").append(name())
                                        .append(" ne peuvent avoir de restriction de modification.\n").toStdString());
    }

    //! Renvoie le nombre de descendants directs.
    virtual int size_child(const entity &/*ent*/)
        {return No_Child;}

    //! Test la restriction de modification de l'entité d'identifiant id.
    virtual bool test_restriction(idt /*id*/, flag /*restrict*/)
        {return false;}

    //! Retourne le type du manager.
    virtual flag type_manager() const
        {return b2d::Normal_Type_Manager;}

protected:
    //! Message d'erreurs si l'entité entity n'est pas valide.
    virtual QString message_erreurs(const entity &ent) const = 0;
};

/*! \ingroup groupe_manager
 *\brief Classe abstraite de base des manageurs associée à l'entité Ent.
 *Regroupe les méthodes virtuelles nécessaire à la base de donnée propre à l'entité Ent.
 */
template<class Ent> class abstract_manager_temp : public abstract_manager {
public:
    //! Constructeur.
    abstract_manager_temp() = default;

    //! Destructeur.
    ~abstract_manager_temp() override = default;

    // Méthode d'existence
    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    virtual bool exists(const Ent &ent) = 0;

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    bool exists(const entity &ent) override
        {return exists(Ent::Convert(ent));}

    //! Teste s'il existe une entité vérifiant une condition en base de donnée.
    virtual bool exists(typename Ent::position cle, const QVariant &value, b2d::condition cond = b2d::condition::Egal) = 0;

    //! Teste s'il existe une entité vérifiant des conditions.
    virtual bool exists(const std::map<typename Ent::position, QVariant> &value,
                std::vector<b2d::condition> cond = std::vector<b2d::condition>({b2d::condition::Egal})) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques qu'entity en base de donnée.
    //! De plus, si l'identifiant de entity est nul et qu'il existe en base de donnée exactement une entité
    //! possédant des ensembles d'attributs uniques avec les mêmes valeurs qu'entity, alors l'identifiant d'entity
    //! est remplacé par l'identifiant de cette entité.
    virtual b2d::existe_uni exists_unique(Ent &ent) = 0;

    //! Idem b2d::existe_uni exists_unique(Ent &ent) avec une conversion de référence.
    b2d::existe_uni exists_unique(entity &ent) override
        {return exists_unique(Ent::Convert(ent));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques qu'entity en base de donnée.
    virtual b2d::existe_uni exists_unique(const Ent &ent) = 0;

    //! Idem b2d::existe_uni exists_unique(const Ent &ent) avec une conversion de référence.
    b2d::existe_uni exists_unique(const entity &ent) override
        {return exists_unique(Ent::Convert(ent));}

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    virtual std::pair<b2d::existe_uni,idt> exists_unique_id(const Ent &ent) = 0;

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    std::pair<b2d::existe_uni,idt> exists_unique_id(const entity &ent) override
        {return exists_unique_id(Ent::Convert(ent));}

    // Méthode d'aggrégation
    //! Fonction d'agrega de valeur de type QVariant sur l'attribut att appliquée à toutes les entités de la table.
    virtual QVariant fonction_agrega(b2d::agrega fonc, typename Ent::position att) = 0;

    //! Fonction d'agrega de valeur de type T sur l'attribut att appliquée à toutes les entités vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    virtual QVariant fonction_agrega(b2d::agrega /*fonc*/, typename Ent::position /*att*/, typename Ent::position /*cle*/,
                                       const QVariant &/*value*/, b2d::condition /*cond*/ = b2d::condition::Egal) = 0;

    //! Fonction d'agrega de valeur de type T sur l'attribut att appliquée à toutes les entités vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2.
    virtual QVariant fonction_agrega(b2d::agrega /*fonc*/, typename Ent::position /*att*/, typename Ent::position /*cle_1*/,
                                       const QVariant &/*value_1*/, typename Ent::position /*cle_2*/,  const QVariant &/*value_2*/,
                                       b2d::condition /*cond_1*/ = b2d::condition::Egal,
                                       b2d::condition /*cond_2*/ = b2d::condition::Egal) = 0;

    // Méthodes de demande
    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant le même identifiant que ent.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get(Ent &ent) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant le même identifiant que ent.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get(entity &ent) override
        {return get(Ent::Convert(ent));}

    // Obtention d'un arbre.
    //! Renvoie l'arbre de toutes les entités.
    virtual tree<Ent> get_arbre();

    //! Renvoie l'arbre de racine ent.
    virtual tree<Ent> get_arbre(const Ent &ent);

    //! Renvoie l'arbre de racine d'identifiant id pour une entité de type arbre.
    virtual tree<Ent> get_arbre(idt id);

    //! Renvoie le descendant direct du parent d'identifiant id_parent de position pos dans la fratrie.
    virtual Ent get_child(idt id_parent, int num);

    //! Renvoie l'identifiant du descendant direct du parent d'identifiant id_parent de position pos dans la fratrie.
    idt get_id_child(idt id_parent, int num) override;

    //! Renvoie l'identifiant du parent (si le manager est de type arbre).
    idt get_id_parent(idt id) override;

    // Obtention d'une liste.
    //! Renvoie la liste des entités de la table des entités Ent ordonnée suivant la colonne d'identifiant ordre.
    virtual list_ptr<Ent> get_list(typename Ent::position ordre = Ent::Id, bool croissant = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual list_ptr<Ent> get_list(typename Ent::position cle, int value, typename Ent::position ordre = Ent::Id,
                                 b2d::condition cond = b2d::condition::Egal, bool crois = true)
        {return get_list(cle,QVariant(value),ordre,cond,crois);}

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual list_ptr<Ent> get_list(typename Ent::position cle, idt value, typename Ent::position ordre = Ent::Id,
                                 b2d::condition cond = b2d::condition::Egal, bool crois = true)
        {return get_list(cle,QVariant(value),ordre,cond,crois);}

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual list_ptr<Ent> get_list(typename Ent::position cle, const QVariant &value, typename Ent::position ordre = Ent::Id,
                                 b2d::condition cond = b2d::condition::Egal, bool crois = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1 puis ordre_2.
    virtual list_ptr<Ent> get_list(typename Ent::position cle, const QVariant &value,
                                   typename Ent::position order1, typename Ent::position order2,
                                   b2d::condition cond = b2d::condition::Egal,
                                   bool croissant1 = true, bool croissant2 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1, ordre_2 puis ordre_3.
    virtual list_ptr<Ent> get_list(typename Ent::position cle, const QVariant &value,
                                   typename Ent::position ordre_1, typename Ent::position ordre_2, typename Ent::position ordre_3,
                                   b2d::condition cond = b2d::condition::Egal,
                                   bool crois_1 = true, bool crois_2 = true, bool crois_3 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual list_ptr<Ent> get_list(typename Ent::position cle_1, const QVariant &value_1,
                                   typename Ent::position cle_2, const QVariant &value_2,
                                   typename Ent::position ordre = Ent::Id,
                                   b2d::condition cond_1 = b2d::condition::Egal,
                                   b2d::condition cond_2 = b2d::condition::Egal,
                                   bool crois = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1, valeur de la colonne d'identifiant cle_2 = value_2,
    //! et valeur de la colonne d'identifiant cle_3 = value_3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual list_ptr<Ent> get_list(typename Ent::position cle_1, const QVariant &value_1,
                                   typename Ent::position cle_2, const QVariant &value_2,
                                   typename Ent::position cle_3, const QVariant &value_3,
                                   typename Ent::position ordre = Ent::Id,
                                   b2d::condition cond_1 = b2d::condition::Egal,
                                   b2d::condition cond_2 = b2d::condition::Egal,
                                   b2d::condition cond_3 = b2d::condition::Egal,
                                   bool crois = true) = 0;

    //! Renvoie la liste des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    virtual list_ptr<Ent> get_list(const std::vector<typename Ent::position> &cle, const std::vector<QVariant> &value,
                                   const std::vector<typename Ent::position> &ordre = std::vector<typename Ent::position>(),
                                   const std::vector<b2d::condition> &condition = std::vector<b2d::condition>(),
                                   const std::vector<bool> &crois = std::vector<bool>()) = 0;

    //! Renvoie la liste des entités de la table vérifiant la condition.
    virtual list_ptr<Ent> get_list(const QString &condition) = 0;

    //! Renvoie le liste des descendant direct d'ent.
    virtual vector_ptr<Ent> get_list_childs(const Ent &/*ent*/, typename Ent::position /*ordre*/ = Ent::Id)
        {return vector_ptr<Ent>();}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    virtual std::list<idt> get_list_childs_id(idt /*id*/, typename Ent::position /*ordre*/ = Ent::Id)
        {return std::list<idt>();}

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    virtual std::vector<std::pair<idt,bool>> get_list_childs_id_leaf(idt /*id*/)
        {return std::vector<std::pair<idt,bool>>();}

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> get_list_id(typename Ent::position cle, int value, typename Ent::position ordre = Ent::Id,
                                       b2d::condition cond = b2d::condition::Egal, bool crois = true)
        {return get_list_id(cle, QVariant(value), ordre, cond, crois);}

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> get_list_id(typename Ent::position cle, idt value, typename Ent::position ordre = Ent::Id,
                                 b2d::condition cond = b2d::condition::Egal, bool crois = true)
        {return get_list_id(cle, QVariant(value), ordre, cond, crois);}

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> get_list_id(typename Ent::position cle, const QVariant &value,
                                 typename Ent::position ordre = Ent::Id,
                                 b2d::condition cond = b2d::condition::Egal, bool crois = true) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1 puis ordre_2.
    virtual std::list<idt> get_list_id(typename Ent::position cle, const QVariant &value,
                                 typename Ent::position order1, typename Ent::position order2,
                                 b2d::condition cond = b2d::condition::Egal,
                                 bool croissant1 = true, bool croissant2 = true) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1, ordre_2 puis ordre_3.
    virtual std::list<idt> get_list_id(typename Ent::position cle, const QVariant &value,
                                 typename Ent::position ordre_1, typename Ent::position ordre_2, typename Ent::position ordre_3,
                                 b2d::condition cond = b2d::condition::Egal,
                                 bool crois_1 = true, bool crois_2 = true, bool crois_3 = true) = 0;

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> get_list_id(typename Ent::position cle_1, const QVariant &value_1,
                                 typename Ent::position cle_2, const QVariant &value_2,
                                 typename Ent::position ordre = Ent::Id,
                                 b2d::condition cond_1 = b2d::condition::Egal, b2d::condition cond_2 = b2d::condition::Egal,
                                 bool crois = true) = 0;

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1, valeur de la colonne d'identifiant cle_2 = value_2,
    //! et valeur de la colonne d'identifiant cle_3 = value_3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    virtual std::list<idt> get_list_id(typename Ent::position cle_1, const QVariant &value_1,
                                 typename Ent::position cle_2, const QVariant &value_2,
                                 typename Ent::position cle_3, const QVariant &value_3,
                                 typename Ent::position ordre = Ent::Id,
                                 b2d::condition cond_1 = b2d::condition::Egal, b2d::condition cond_2 = b2d::condition::Egal,
                                 b2d::condition cond_3 = b2d::condition::Egal,
                                 bool crois = true) = 0;

    //! Renvoie la liste des identifiants de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    virtual std::list<idt> get_list_id(const std::vector<typename Ent::position> &cle, const std::vector<QVariant> &value,
                         const std::vector<typename Ent::position> &ordre = std::vector<typename Ent::position>(),
                         const std::vector<b2d::condition> &condition = std::vector<b2d::condition>(),
                         const std::vector<bool> &crois = std::vector<bool>()) = 0;

    // Liste de Jointure
    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonne_table = colonne_join),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map where_map_table,
    //! valeur des colonnes de la table Join key = value de std::map where_map_join,
    //! ordonnée suivant les colonnes de la table Ent d'identifiants key et d'ordre value de std::map order_map_table
    //! (true -> croissant, false -> décroissant).
    virtual list_ptr<Ent> get_list_join(const QString &table_join, post colonne_table,
                                     const QString &colonne_join,
                                     const std::map<typename Ent::position,QVariant> &where_map_table,
                                     const std::map<QString,QVariant> &where_map_join,
                                     const std::vector<std::pair<typename Ent::position,bool>> &order_map_table) = 0;

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonne_join),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = value_where,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    virtual list_ptr<Ent> get_list_join(const QString &table_join,
                                     const QString &colonne_join,
                                     const QString &where_join,
                                     const QVariant &value_where,
                                     typename Ent::position ordre = Ent::Id,
                                     b2d::condition cond = b2d::condition::Egal, bool crois = true) = 0;

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure
    //! (table.ID = join_1.colonne_1, join_2.ID = join_1.colonne_2),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = value_where sur la seconde jointure,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    virtual list_ptr<Ent> get_list_join(const QString &table_join_1,
                             const QString &table_join_2,
                             const QString &colonne_1,
                             const QString &colonne_2,
                             const QString &where_join_2,
                             const QVariant &value_where,
                             typename Ent::position ordre = Ent::Id,
                             b2d::condition cond = b2d::condition::Egal, bool crois = true) = 0;

    // Map
    //! Renvoie la map des entités de la table des entités Ent.
    virtual map_id<Ent> get_map(typename Ent::position cle_map = Ent::Id) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    virtual map_id<Ent> get_map(typename Ent::position cle, const QVariant &value,
                               typename Ent::position cle_map = Ent::Id, b2d::condition cond = b2d::condition::Egal) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2.
    virtual map_id<Ent> get_map(typename Ent::position cle_1, const QVariant &value_1,
                               typename Ent::position cle_2, const QVariant &value_2,
                               typename Ent::position cle_map = Ent::Id,
                               b2d::condition cond_1 = b2d::condition::Egal, b2d::condition cond_2 = b2d::condition::Egal) = 0;

    //! Renvoie la map des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1, valeur de la colonne d'identifiant cle_2 = value_2,
    //! et valeur de la colonne d'identifiant cle_3 = value_3.
    virtual map_id<Ent> get_map(typename Ent::position cle_1, const QVariant &value_1,
                               typename Ent::position cle_2, const QVariant &value_2,
                               typename Ent::position cle_3, const QVariant &value_3,
                               typename Ent::position cle_map = Ent::Id,
                               b2d::condition cond_1 = b2d::condition::Egal, b2d::condition cond_2 = b2d::condition::Egal,
                               b2d::condition cond_3 = b2d::condition::Egal) = 0;

    //! Renvoie la map des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    virtual map_id<Ent> get_map(const std::vector<typename Ent::position> &cle, const std::vector<QVariant> &value,
                         typename Ent::position cle_map = Ent::Id,
                         const std::vector<typename Ent::position> &ordre = std::vector<typename Ent::position>(),
                         const std::vector<b2d::condition> &condition = std::vector<b2d::condition>(),
                         const std::vector<bool> &crois = std::vector<bool>()) = 0;

    // Map de Jointure
    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (colonne_table = colonne_join),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map where_map_table,
    //! valeur des colonnes de la table Join key = value de std::map where_map_join.
    virtual map_id<Ent> get_map_join(const QString &table_join, post colonne_table,
                                   const QString &colonne_join,
                                   const std::map<typename Ent::position,QVariant> &where_map_table,
                                   const std::map<QString,QVariant> &where_map_join,
                                   typename Ent::position cle_map = Ent::Id) = 0;

    //! Renvoie la map des entités de la table vérifiant une condition sur une jointure (table.ID = join.colonne_join),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = value_where.
    virtual map_id<Ent> get_map_join(const QString &table_join,
                                   const QString &colonne_join,
                                   const QString &where_join,
                                   const QVariant &value_where,
                                   typename Ent::position cle_map = Ent::Id,
                                   b2d::condition cond = b2d::condition::Egal) = 0;

    //! Renvoie la référence du parent (si le manager est de type arbre).
    QString get_ref_parent(idt id) override;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant les mêmes valeurs pour au moins un ensemble des attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    virtual bool get_unique(Ent &ent) = 0;

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrée en base de donnée
    //! ayant les mêmes valeurs pour les attributs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get_unique(entity &ent) override
        {return get_unique(Ent::Convert(ent));}

    //! Renvoie le vecteur des descendant direct d'ent.
    virtual vector_ptr<Ent> get_vector_childs(const Ent &/*ent*/)
        {return vector_ptr<Ent>();}

    //! Renvoie la liste des entités de la table ordonnée suivant les identifiants croissants.
    vector_ptr<entity> get_vector_entity() override
        {return get_list();}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    virtual void insert(Ent &ent, idt id_parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(entity &ent, idt id_parent, int num = 0) override
        {insert(Ent::Convert(ent), id_parent, num);}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    virtual void insert(const Ent &ent, idt id_parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(const entity &ent, idt id_parent, int num = 0) override
        {insert(Ent::Convert(ent), id_parent, num);}

    //! Renvoie un pointeur sur une entité du type géré par le manager.
    std::unique_ptr<entity> make_entity() const override
        {return std::make_unique<Ent>();}

    //! Retourne un vecteur contenant les noms des attributs.
    std::vector<QString> attribut_names() const override
        {return Ent::Names();}

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    virtual bool same_in_bdd(const Ent &ent) = 0;

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool same_in_bdd(const entity &ent) override
        {return same_in_bdd(Ent::Convert(ent));}

    //! Teste s'il y a dans la base de donnée une entité d'identifiant id ayant exactement les mêmes attributs que ent.
    virtual bool same_in_bdd(const Ent &ent, idt id) = 0;

    // Save
    //! Enregistre l'entity dans la base de donnée.
    virtual void save(Ent &ent) = 0;

    //! Enregistre l'entité entity en base de donnée et assigne l'identifiant de l'entité insérée en base de donnée à ent.
    void save(entity &ent) override
        {save(Ent::Convert(ent));}

    //! Enregistre l'entity dans la base de donnée.
    virtual void save(const Ent &ent) = 0;

    //! Enregistre l'entité entity en base de donnée.
    void save(const entity &ent) override
        {save(Ent::Convert(ent));}

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle restriction de modification.
    virtual void save(Ent &ent, flag restrict) {
        save_by_pass(ent);
        set_restriction(ent.id(), restrict);
    }

    //! Enregistre l'entité entity en base de donnée ainsi que sa nouvelle restriction de modification.
    virtual void save(const Ent &c_ent, flag restrict) {
        Ent ent(c_ent);
        save_by_pass(ent);
        set_restriction(ent.id(), restrict);
    }

    //! Enregistre l'arbre d'entités dans la base de donnée pour les entités de type arbre.
    virtual void save(tree<Ent> &arbre, b2d::tree_save n = b2d::tree_save::External_Change);

    //! Sauve l'entité en ignorant les restrictions.
    virtual void save_by_pass(Ent &ent)
        {save(ent);}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    virtual void save_unique(Ent &ent) = 0;

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    void save_unique(entity &ent) override
        {save_unique(Ent::Convert(ent));}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    virtual void save_unique(const Ent &ent) = 0;

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    void save_unique(const entity &ent) override
        {save_unique(Ent::Convert(ent));}

    //! Teste la restriction de modification pour une entité d'identifiant id avec les valeurs de ent.
    virtual bool test_restriction(idt /*id*/, flag /*restrict*/, const Ent &/*ent*/)
        {return true;}
};

template<class Ent> tree<Ent> abstract_manager_temp<Ent>::get_arbre()
    {throw std::invalid_argument(QString("La méthode 'get_arbre' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> tree<Ent> abstract_manager_temp<Ent>::get_arbre(const Ent &/*ent*/)
    {throw std::invalid_argument(QString("La méthode 'get_arbre' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> tree<Ent> abstract_manager_temp<Ent>::get_arbre(idt /*id*/)
    {throw std::invalid_argument(QString("La méthode 'get_arbre' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> Ent abstract_manager_temp<Ent>::get_child(idt /*id_parent*/, int /*num*/)
    {throw std::invalid_argument(QString("La méthode 'get_child' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> idt abstract_manager_temp<Ent>::get_id_child(idt /*id_parent*/, int /*num*/)
    {throw std::invalid_argument(QString("La méthode 'get_id_child' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> idt abstract_manager_temp<Ent>::get_id_parent(idt /*id*/)
    {throw std::invalid_argument(QString("La méthode 'get_id_parent' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> QString abstract_manager_temp<Ent>::get_ref_parent(idt /*id*/)
    {throw std::invalid_argument(QString("La méthode 'get_ref_parent' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> void abstract_manager_temp<Ent>::insert(Ent &/*ent*/, idt /*id_parent*/, int /*num*/)
    {throw std::invalid_argument(QString("La méthode 'save(Ent &ent, const Ent &parent, int num)' "
                                         "n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}

template<class Ent> void abstract_manager_temp<Ent>::insert(const Ent &/*ent*/, idt /*id_parent*/, int /*num*/)
{throw std::invalid_argument(QString("La méthode 'insert(const Ent &ent, const Ent &parent, int num)' "
                                     "n'est pas définie pour le manager des : ")
                             .append(Ent::Name()).append(".").toStdString());}

template<class Ent> void abstract_manager_temp<Ent>::save(tree<Ent> &/*arbre*/, b2d::tree_save /*n*/)
    {throw std::invalid_argument(QString("La méthode 'save(tree<Ent>,...)' n'est pas définie pour le manager des : ")
                                 .append(Ent::Name()).append(".").toStdString());}
}}
#endif // ABSTRACT_MANAGER_H
