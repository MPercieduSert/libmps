/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/08/2018
 */
#ifndef BDD_H
#define BDD_H

#include <functional>
#include <memory>
#include <QFont>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <stdexcept>
#include "managers_base.h"
#include "file_interface.h"
#include "xml.h"

namespace mps {
namespace b2d {
using entity = entities::entity;
/*! \ingroup groupe_file
 * \brief Classe mère du gestionnaire de base de donnée.
 *
 * Gestionnaire de la base donnée sous SQLite3. Cette classe fait le lien entre la base de donnée et les entity grâce aux Manager.
 */
class bdd : public fichier::file_interface {
protected:
    QSqlDatabase m_bdd;                                 //!< Connexion à la base de donnée.
    std::unique_ptr<manager::managers_base> m_manager;  //!< Manager des entités.
    const std::vector<int> m_version;                   //!< Version de la base de donnée requis par le programme
public:
    // Alias de type
    using vector_id_name = std::vector<std::pair<int,QString>>;
    using xml_iterator = fichier::doc_xml::const_brother_iterator;
    using xml_list_atts = std::list<std::pair<QString,QString>>;

    // Enumération
    enum {Invalide_Enum = 0x1000000,
          No_Entity
         };
    //! Catégorie des énumérations
    enum categorie : flag::flag_type {Restriction_Code = 0x1,
                                      Permission_Code = 0x2,
                                      Line_Style = 0x4,
                                      Brush_Style = 0x8,
                                      Font_Weight = 0x10,
                                      Style_Num = 0x20
                                     };
    //! Options d'exportation xml
    enum export_option : flag::flag_type {Restriction_Exportation_Xml = 0x1};

    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    bdd(const QString &bdd_type, const std::vector<int> &version, std::unique_ptr<manager::managers_base> &&manager = nullptr)
        : file_interface(QString(),"Data Base files (*.db)"),
           m_bdd(QSqlDatabase::addDatabase(bdd_type)),
           m_manager(std::move(manager)),
           m_version(version) {}


    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée, la version et le managers.
    bdd(const QString &bdd_type, const QString &name, const std::vector<int> &version,
        std::unique_ptr<manager::managers_base> &&manager = nullptr)
        : file_interface(name,"Data Base files (*.db)"),
           m_bdd(QSqlDatabase::addDatabase(bdd_type)),
           m_manager(std::move(manager)),
           m_version(version)
        {m_bdd.setDatabaseName(name);}

    //! Destructeur. Referme la base de donnée.
    ~bdd() override
        {m_bdd.close();}

    //! Ajoute des restrictions de modification pour une entité donnée.
    void add_restriction(const entity &ent, flag restrict)
        {m_manager->get(ent.id_entity()).add_restriction(ent.id(), restrict);}

    //! Renvoie le code associé à une chaine de caractère (séparateur "|") pour l'entité ident.
    flag code_from_string(const QString &str, uint id_cat, bool for_entity,  QString &controle) const;

    //! Vérifie si le fichier de chemin name existe et est un fichier de base de donnée valide, si c'est le cas,
    //! le fichier de la base de donnée est remplacé par une copie du fichier de chemin name.
    bool copy(const QString &name) override;

    //! Créer la base de donnée avec les tables et les entrées par défault.
    bool creer() final;

    //! Renvoie une réfrence sur la base de données.
    QSqlDatabase &db() noexcept
        {return m_bdd;}

    //! Supprime l'entité entity de la base de données.
    bool del(const entity &ent)
        {return del_v(ent.id(),ent.id_entity());}

    //! Supprime l'entité entity de la base de donnée et met l'identifiant de l'entité à zéro.
    bool del(entity &ent) {
        bool bb = del(static_cast<const entity &>(ent));
        if(bb)
            ent.set_id(0);
        return bb;
    }

    //! Supprime les entités d'une liste.
    template<class Ent> bool del(const vector_ptr<Ent> &liste) {
        auto it = liste.cbegin();
        while(it != liste.cend() &&del(*it))
            ++it;
        return it == liste.cend();
    }

    //! Supprime les entités d'une liste.
    template<class Ent> bool del(vector_ptr<Ent> &liste) {
        auto i = liste.begin();
        while(i != liste.end() &&del(*i))
            ++i;
        return i == liste.end();
    }

    //! Teste si le fichier de base de donnée existe.
    bool exists() const override
        {return QFile::exists(m_name);}

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    bool exists(const entity &ent);

    //! Teste s'il existe une entité de même identifiant que entity en base de donnée.
    template<class Ent> bool exists(const Ent &ent);

    //! Teste s'il existe une entité vérifiant une condition.
    template<class Ent> bool exists(typename Ent::position cle, const QVariant &value, condition cond = condition::Egal);

    //! Teste s'il existe une entité vérifiant des conditions.
    template<class Ent> bool exists(const std::map<typename Ent::position, QVariant> &value,
                    std::vector<condition> cond = std::vector<condition>({condition::Egal}));

    //! Teste s'il existe une entité ayant les mêmes valeurs qu'un des ensemble d'attributs uniques d'entity en base de donnée.
    //! De plus, si l'identifiant de entity est nul
    //! et qu'il existe en base de donnée exactement une entité possédant des ensembles
    //! d'attributs uniques avec les mêmes valeurs qu'entity,
    //! alors l'identifiant d'entity est remplacé par l'identifiant de cette entité.
    bool exists_unique(entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs qu'un des ensemble d'attributs uniques d'entity en base de donnée.
    //! De plus, si l'identifiant de entity est nul
    //! et qu'il existe en base de donnée exactement une entité possédant des ensembles
    //! d'attributs uniques avec les mêmes valeurs qu'entity,
    //! alors l'identifiant d'entity est remplacé par l'identifiant de cette entité.
    template<class Ent> bool exists_unique(Ent &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs qu'un ensemble d'attributs uniques que entity en base de donnée.
    bool exists_unique(const entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs qu'un ensemble d'attributs uniques que entity en base de donnée.
    template<class Ent> bool exists_unique(const Ent &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    existe_uni exists_unique_ensemble(entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    template<class Ent> existe_uni exists_unique_ensemble(Ent &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    existe_uni exists_unique_ensemble(const entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    template<class Ent> existe_uni exists_unique_ensemble(const Ent &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    std::pair<existe_uni,idt> exists_unique_id(const entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    template<class Ent> std::pair<existe_uni,idt> exists_unique_id(const Ent &ent);

    //! Créer un arbre doc_xml à partir d'une liste d'entités.
    fichier::doc_xml export_xml(vector_ptr<entity> &&vec, flag option);

    //! Fonction d'agrega sur l'attribut att appliquée à toutes les entités de la table.
    template<class Ent> QVariant fonction_agrega(agrega fonc, typename Ent::position att);

    //! Fonction d'agrega  sur l'attribut att appliquée à toutes les entités vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    template<class Ent> QVariant fonction_agrega(agrega fonc, typename Ent::position att, typename Ent::position cle,
                                                  const QVariant &value, condition cond = condition::Egal);

    //! Fonction d'agrega sur l'attribut att appliquée à toutes les entités vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2.
    template<class Ent> QVariant fonction_agrega(agrega fonc, typename Ent::position att, typename Ent::position cle_1,
                                                  const QVariant &value_1, typename Ent::position cle_2,  const QVariant &value_2,
                                                  condition cond_1 = condition::Egal, condition cond_2 = condition::Egal);

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant de celui d'identifiant id
    //! en commençant par les descendants.
    template<class Ent, class Fct> bool foreach_begin_child(idt id, const Fct &fct, bool ordre = true);

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant celui d'identifiant id.
    template<class Ent, class Fct> bool foreach_node(idt id, const Fct &fct, bool ordre = true);

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant le même identifiant que ent.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get(entity &ent);

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant le même identifiant que ent.
    //! Retourne True si l'opération s'est correctement déroulée.
    template<class Ent> bool get(Ent &ent);

    //! Renvoie l'arbre de toutes les entités pour une entité de type arbre.
    template<class Ent> tree<Ent> get_arbre();

    //! Renvoie l'arbre de racine d'identifiant id pour une entité de type arbre.
    template<class Ent> tree<Ent> get_arbre(idt id);

    //! Renvoie l'arbre de racine d'entité entity pour une entité de type arbre.
    template<class Ent> tree<Ent> get_arbre(const Ent &ent);

    //! Renvoie le descendant direct du parent d'identifiant id_parent de position pos dans la fratrie.
    template<class Ent> Ent get_child(idt id_parent, int num);

    //! Renvoie l'arbre des identifiants de racine d'identifiant id pour une entité de type arbre.
    template<class Ent> tree<idt> get_id_arbre(idt id);

    //! Renvoie l'identifiant du descendant direct du parent d'identifiant id_parent de position pos dans la fratrie.
    template<class Ent> idt get_id_child(idt id_parent, int num);

    //! Renvoie l'identifiant du parent (si le manager est de type arbre).
    idt get_id_parent(const entity &ent);

    //! Renvoie l'identifiant de la racine (si le manager est de type arbre).
    idt get_id_root(const entity &ent);

    //! Renvoie l'identifiant de la racine (si le manager est de type arbre).
    template<class Ent> idt get_id_root(idt id);

    //! Renvoie la liste des entités de la table des entités d'identifiant ent_id ordonnée suivant la colonne d'identifiant ordre.
    vector_ptr<entity> get_list(entidt ent_id);

    //! Renvoie la liste des entités de la table des entités Ent ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> get_list(typename Ent::position ordre = Ent::Id, bool croissant = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> get_list(typename Ent::position cle, int value, typename Ent::position ordre = Ent::Id,
                                             condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> get_list(typename Ent::position cle, idt value, typename Ent::position ordre = Ent::Id,
                                             condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> get_list(typename Ent::position cle, const QVariant &value,
                                               typename Ent::position ordre = Ent::Id,
                                               condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1 puis ordre_2.
    template<class Ent> vector_ptr<Ent> get_list(typename Ent::position cle, const QVariant &value,
                                             typename Ent::position order1, typename Ent::position order2,
                                             condition cond = condition::Egal, bool croissant1 = true, bool croissant2 = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1, ordre_2 puis ordre_3.
    template<class Ent> vector_ptr<Ent> get_list(typename Ent::position cle, const QVariant &value,
                                               typename Ent::position ordre_1, typename Ent::position ordre_2,
                                               typename Ent::position ordre_3, condition cond = condition::Egal,
                                               bool crois_1 = true, bool crois_2 = true, bool crois_3 = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> get_list(typename Ent::position cle_1, const QVariant &value_1,
                                             typename Ent::position cle_2, const QVariant &value_2,
                                             typename Ent::position ordre = Ent::Id,
                                             condition cond_1 = condition::Egal, condition cond_2 = condition::Egal,
                                             bool crois = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1, valeur de la colonne d'identifiant cle_2 = value_2,
    //! et valeur de la colonne d'identifiant cle_3 = value_3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> get_list(typename Ent::position cle_1, const QVariant &value_1,
                                             typename Ent::position cle_2, const QVariant &value_2,
                                             typename Ent::position cle_3, const QVariant &value_3,
                                             typename Ent::position ordre = Ent::Id,
                                             condition cond_1 = condition::Egal, condition cond_2 = condition::Egal,
                                             condition cond_3 = condition::Egal,
                                             bool crois = true);

    //! Renvoie la liste des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    template<class Ent> vector_ptr<Ent> get_list(const std::vector<typename Ent::position> &cle,
                         const std::vector<QVariant> &value,
                         const std::vector<typename Ent::position> &ordre = std::vector<typename Ent::position>(),
                         const std::vector<condition> &cond = std::vector<condition>(),
                         const std::vector<bool> &crois = std::vector<bool>());

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonne_table = colonne_join),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map where_map_table,
    //! valeur des colonnes de la table Join d'identifiant key = value de std::map where_map_join,
    //! ordonnée suivant les colonnes de la table Ent d'identifiants key et d'ordre value de std::map order_map_table
    //! (true -> croissant, false -> décroissant).
    template<class Ent, class Join> vector_ptr<Ent> get_list(typename Ent::position colonne_table,
                                                   typename Join::position colonne_join,
                                                   const std::map<typename Ent::position,QVariant> &where_map_table,
                                                   const std::map<typename Join::position,QVariant> &where_map_join,
                                                   const std::vector<std::pair<typename Ent::position,bool>> &order_map_table);

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (Ent.ID = join.cleJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = value_where,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    template<class Ent, class Join> vector_ptr<Ent> get_list(typename Join::position cleJoin, typename Join::position cleWhere,
                                                         const QVariant &value_where, typename Ent::position ordre = Ent::Id,
                                                         condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des entités de la table vérifiant deux conditions sur une jointure (Ent.ID = join.cleJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere1 = value_where1 et cleWhere2 = value_where2,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    template<class Ent, class Join> vector_ptr<Ent> get_list(typename Join::position cleJoin,
                                                           typename Join::position cleWhere1, const QVariant &value_where1,
                                                           typename Join::position cleWhere2, const QVariant &value_where2,
                                                           typename Ent::position ordre = Ent::Id, bool crois = true);

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure
    //! (table.ID = join_1.colonne_1, join_2.ID = join_1.colonne_2),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = value_where sur la seconde jointure,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    template<class Ent , class Join1, class Join2> vector_ptr<Ent> get_list(typename Join1::position cle_1,
                                                                            typename Join1::position cle_2,
                                                                            typename Join2::position cleWhere,
                                                                            const QVariant &value_where,
                                                                            typename Ent::position ordre = Ent::Id,
                                                                            condition cond = condition::Egal, bool crois = true);

    //! Renvoie le liste des descendant direct d'ent.
    template<class Ent> vector_ptr<Ent> get_list_childs(const Ent &ent, typename Ent::position ordre = Ent::Id);

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    template<class Ent> std::list<idt> get_list_childs_id(idt id, typename Ent::position ordre = Ent::Id);

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    template<class Ent> std::vector<std::pair<idt,bool>> get_list_childs_id_leaf(int id);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> get_list_id(typename Ent::position cle, int value, typename Ent::position ordre = Ent::Id,
                                 condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> get_list_id(typename Ent::position cle, idt value, typename Ent::position ordre = Ent::Id,
                                 condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> get_list_id(typename Ent::position cle, const QVariant &value,
                                 typename Ent::position ordre = Ent::Id,
                                 condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1 puis ordre_2.
    template<class Ent> std::list<idt> get_list_id(typename Ent::position cle, const QVariant &value,
                                 typename Ent::position ordre_1, typename Ent::position ordre_2,
                                 condition cond = condition::Egal,
                                 bool croissant1 = true, bool croissant2 = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre_1, ordre_2 puis ordre_3.
    template<class Ent> std::list<idt> get_list_id(typename Ent::position cle, const QVariant &value,
                                 typename Ent::position ordre_1, typename Ent::position ordre_2, typename Ent::position ordre_3,
                                 condition cond = condition::Egal,
                                 bool crois_1 = true, bool crois_2 = true, bool crois_3 = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> get_list_id(typename Ent::position cle_1, const QVariant &value_1,
                                 typename Ent::position cle_2, const QVariant &value_2,
                                 typename Ent::position ordre = Ent::Id,
                                 condition cond_1 = condition::Egal, condition cond_2 = condition::Egal,
                                 bool crois = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1, valeur de la colonne d'identifiant cle_2 = value_2,
    //! et valeur de la colonne d'identifiant cle_3 = value_3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> get_list_id(typename Ent::position cle_1, const QVariant &value_1,
                                 typename Ent::position cle_2, const QVariant &value_2,
                                 typename Ent::position cle_3, const QVariant &value_3,
                                 typename Ent::position ordre = Ent::Id,
                                 condition cond_1 = condition::Egal, condition cond_2 = condition::Egal,
                                 condition cond_3 = condition::Egal,
                                 bool crois = true);

    //! Renvoie la liste des identifiants de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    template<class Ent> std::list<idt> get_list_id(const std::vector<typename Ent::position> &cle,
                         const std::vector<QVariant> &value,
                         const std::vector<typename Ent::position> &ordre = std::vector<typename Ent::position>(),
                         const std::vector<condition> &cond = std::vector<condition>(),
                         const std::vector<bool> &crois = std::vector<bool>());

    //! Renvoie la map des entités de la table des entités Ent.
    template<class Ent> map_id<Ent> get_map(typename Ent::position cle_map = Ent::Id);

    //! Renvoie la map des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    template<class Ent> map_id<Ent> get_map(typename Ent::position cle, const QVariant &value,
                                           typename Ent::position cle_map = Ent::Id, condition cond = condition::Egal);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1 et valeur de la colonne d'identifiant cle_2 = value_2.
    template<class Ent> map_id<Ent> get_map(typename Ent::position cle_1, const QVariant &value_1,
                                           typename Ent::position cle_2, const QVariant &value_2,
                                           typename Ent::position cle_map = Ent::Id,
                                           condition cond_1 = condition::Egal, condition cond_2 = condition::Egal);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle_1 = value_1, valeur de la colonne d'identifiant cle_2 = value_2,
    //! et valeur de la colonne d'identifiant cle_3 = value_3.
    template<class Ent> map_id<Ent> get_map(typename Ent::position cle_1, const QVariant &value_1,
                                             typename Ent::position cle_2, const QVariant &value_2,
                                             typename Ent::position cle_3, const QVariant &value_3,
                                             typename Ent::position cle_map = Ent::Id,
                                             condition cond_1 = condition::Egal, condition cond_2 = condition::Egal,
                                             condition cond_3 = condition::Egal);

    //! Renvoie la liste des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    template<class Ent> map_id<Ent> get_map(const std::vector<typename Ent::position> &cle, const std::vector<QVariant> &value,
                         typename Ent::position cle_map = Ent::Id,
                         const std::vector<typename Ent::position> &ordre = std::vector<typename Ent::position>(),
                         const std::vector<condition> &cond = std::vector<condition>(),
                         const std::vector<bool> &crois = std::vector<bool>());

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonne_table = colonne_join),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map where_map_table,
    //! valeur des colonnes de la table Join d'identifiant key = value de std::map where_map_join.
    template<class Ent, class Join> map_id<Ent> get_map(typename Ent::position colonne_table,
                                                       typename Join::position colonne_join,
                                                       const std::map<typename Ent::position,QVariant> &where_map_table,
                                                       const std::map<typename Join::position,QVariant> &where_map_join,
                                                       typename Ent::position cle_map = Ent::Id);

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (Ent.ID = Join.cleJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = value_where.
    template<class Ent, class Join> map_id<Ent> get_map(typename Join::position cleJoin, typename Join::position cleWhere,
                                                       const QVariant &value_where, typename Ent::position cle_map = Ent::Id,
                                                       condition cond = condition::Egal);

    //! Renvoie les restrictions de modification de l'entité donnée en argument.
    flag get_restriction(const entity &ent);

    //! Renvoie les restrictions de modification de l'entité donnée en argument.
    template<class Ent> flag get_restriction(const Ent &ent);

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant les mêmes valeurs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get_unique(entity &ent);


    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant les mêmes valeurs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    template<class Ent> bool get_unique(Ent &ent);

    //! Renvoie l'identifiant de l'entité de type Ent d'Id_Prog idP fourni ou 0 si elle n'existe pas.
    template<class Ent> idt prog_to_id(idt idP) {
         Ent ent;
         ent.set_id_prog(idP);
         return exists_unique(ent) ? ent.id() : 0;
    }

    //! Importation de donnée à partir d'un xmlDoc.
    QString import_xml(const fichier::doc_xml &doc);

    //! Renvoie les informations de la base de donnée sur les entity du même type que ent.
    const manager::info_bdd &info(const entity &ent) const;

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(entity &ent, idt id_parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(entity &ent, idt id_parent, numt num = 0)
        {insert(ent,id_parent,static_cast<int>(num));}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    template<class Ent> void insert(Ent &ent, idt id_parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    template<class Ent> void insert(Ent &ent, idt id_parent, numt num = 0)
        {insert(ent,id_parent,static_cast<int>(num));}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(const entity &ent, idt id_parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(const entity &ent, idt id_parent, numt num = 0)
        {insert(ent,id_parent,static_cast<int>(num));}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    template<class Ent> void insert(const Ent &ent, idt id_parent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    template<class Ent> void insert(const Ent &ent, idt id_parent, numt num = 0)
        {insert(ent,id_parent,static_cast<int>(num));}

    //! Teste si une entité d'identifiant id est une feuille
    //! Retourne true si l'entité n'est pas de type arbre ou n'existe pas.
    template<class Ent> bool is_leaf(idt id);

    //! Teste si la base de donnée est valide.
    bool is_valid() noexcept override
        {return true;}

    //! Fabrique d'entité de classe nomée ent.
    std::unique_ptr<entity> make_entity(const QString &ent) const
        {return m_manager->make_entity(ent);}

    //! Acceseur du managers.
    const manager::managers_base &managers() const noexcept
        {return *m_manager;}

    //! Mise à jour de la base de donnée.
    void mise_a_jour_bdd();

    //! Renvoie le nombre de type d'entitée (virtuelle).
    entidt nbr_entity() const noexcept
        {return m_manager->nbr_entity();}

    //! Ouvre la base de donnée.
    bool open() override;

    //! Renvoie l'identifiant de l'entité de type Ent de référence fourni ou 0 si elle n'existe pas.
    template<class Ent> idt ref_to_id(const QString &ref) {
         Ent ent;
         ent.set_ref(ref);
         return exists_unique(ent) ? ent.id() : 0;
    }

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool same_in_bdd(const entity &ent);

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    template<class Ent> bool same_in_bdd(const Ent &ent);

    //! Enregistre l'entity dans la base de donnée.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    void save(entity &ent);

    //! Enregistre l'entity dans la base de donnée.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    template<class Ent> void save(Ent &ent);

    //! Enregistre l'entity dans la base de donnée.
    void save(const entity &ent);

    //! Enregistre l'entity dans la base de donnée.
    template<class Ent> void save(const Ent &ent);

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restrictions de modification.
    void save(entity &ent, flag restrict);

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restrictions de modification.
    template<class Ent> void save(Ent &ent, flag restrict);

    //! Enregistre l'entité entity en base de donnée ainsi que ses nouvelles restrictions de modification.
    template<class Ent> void save(const Ent &ent, flag restrict);

    //! Enregistre les entités de vector dans la base de donnée.
    template<class Ent> void save(map_id<Ent> &vector)
        {save_conteneur<Ent,map_id<Ent>>(vector);}

    //! Enregistre les entités de vector dans la base de donnée.
    template<class Ent> void save(const map_id<Ent> &vector)
        {save_conteneur<Ent,map_id<Ent>>(vector);}

    //! Enregistre l'arbre d'entités dans la base de donnée.
    template<class Ent> void save(tree<Ent> &arbre, tree_save n = tree_save::External_Change);

    //! Enregistre les entités de vector dans la base de donnée.
    void save(vector_ptr<entity> &vector);

    //! Enregistre les entités de vector dans la base de donnée.
    void save(const vector_ptr<entity> &vector);

    //! Enregistre les entités de vector dans la base de donnée.
    template<class Ent> void save(vector_ptr<Ent> &vector)
        {save_conteneur<Ent,vector_ptr<Ent>>(vector);}

    //! Enregistre les entités de vector dans la base de donnée.
    template<class Ent> void save(const vector_ptr<Ent> &vector)
        {save_conteneur<Ent,vector_ptr<Ent>>(vector);}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    void save_unique(entity &ent);

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    template<class Ent> void save_unique(Ent &ent);

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    void save_unique(const entity &ent);

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    template<class Ent> void save_unique(const Ent &ent);

    //! Renvoie le schema xml pour valider les fichiers d'importation de données.
    fichier::doc_xml schema_xml_for_import() const;

    //! Transmet la connexion à la base de donnée aux managers.
    void set_bdd();

    //! Modifie le chemin de la base de donnée.
    void set_name(const QString &name) override;

    //! Modifie les restrictions de modification pour une entité donnée.
    void set_restriction(const entity &ent, flag restrict);

    //! Modifie les restrictions de modification pour une entité donnée.
    template<class Ent> void set_restriction(const Ent &ent, flag restrict);

    //! Renvoie le nombre de frères.
    int size_brother(const entity &ent);

    //! Renvoie le nombre de frères.
    template<class Ent> int size_brother(idt id);

    //! Renvoie le nombre de descendants directs.
    int size_child(const entity &ent);

    //! Renvoie le nombre de descendants directs.
    template<class Ent> int size_child(idt id);

    //! Renvoie l'enumeration associé à str pour un groupe de catégorie.
    virtual enumt str_categorie_to_enum(const QString &str, flag categorie, QString &controle) const noexcept;

    //! Renvoie l'enumeration associé à str pour une entitée d'identifiant id_ent.
    virtual enumt str_id_to_enum(const QString &str, idt id_entity, QString &controle) const noexcept {
        controle.append("-> Enum invalide pour l'entité d'identifiant ").append(QString::number(static_cast<uint>(id_entity)))
                .append(": ").append(str);
        return Invalide_Enum;
    }

    //! Renvoie un vecteur des noms des entités correspondant à une table de la base de données
    //! (trié par ordre alphabétique).
    vector_id_name table_entity_names(bool arbre = false) const;

    //! Teste l'autorisation de modification de l'entité donnée en argument.
    bool test_autorisation(const entity &ent, flag autoris)
        {return test_autorisation_v(ent.id(),ent.id_entity(),autoris);}

protected:
    //! Renvoie un arbre avec les valeurs contenues dans le XmlDox à l'endroit pointé par iter.
    entities_base::arbre arbre_xml(xml_iterator iter, QString &controle);

    //! Cherche un attribut dans un noeud xml puis renvoie sa valeur.
    QString attribut_xml(xml_iterator iter, const QString &attribut, QString &controle) const {
        auto iter_att = iter->attributes().find(attribut);
        if(iter_att == iter->attributes().cend()) {
            controle.append("Le noeud ne possède pas d'attribut : ").append(attribut);
            return QString();
        }
        else
            return iter_att->second;
    }

    //! Enregistre les données xml associées à une entité dans la base de donnée.
    virtual void associated_xml(entity &ent, xml_iterator iter, QString &controle);

    //! Enregistre les données xml associées à une entité dans la base de donnée
    //! et complète le message d'erreur si besoin.
    void associated_controled_xml(entity &ent, xml_iterator iter, QString &controle) {
        associated_xml(ent,iter,controle);
        if(!controle.isEmpty())
            controle.prepend(QString("-> Dans l'entité associée : ").append(iter->name()).append("\n"));
    }


    //! Création de la table de l'entité en base de donnée.
    template<class Ent> void creer_table()
        {m_manager->get<Ent>().creer();}

    //! Supprime les entités d'une liste.
    template<class Ent> bool del(const std::list<idt> &liste) {
        auto it = liste.cbegin();
        while(it != liste.cend() &&del_v(*it,Ent::ID))
            ++it;
        return it == liste.cend();
    }

    //! Supprime les entités de la liste vérifiant les conditions donnée en arguments.
    template<class Ent, class... Args> bool del_list(Args... args)
        {return del<Ent>(get_list_id<Ent>(args...));}

    //! Supprime l'entité d'identifiant id de type d'identifiant id_entity de la base de données.
    virtual bool del_v(idt id, entidt id_entity)
        {return m_manager->get(id_entity).del(id);}

    //! Hydrate un attribut de l'entité par la valeur contenue dans le XmlDox à l'endroit pointé par iter.
    virtual void hydrate_attribut_xml(entity &ent, post pos, xml_iterator iter, QString &controle);

    //! Hydrate un attribut de l'entité par la valeur contenue dans le XmlDox à l'endroit pointé par iter
    //! et complète le message d'erreur si besoin.
    void hydrate_attribut_controled_xml(entity &ent, post pos, xml_iterator iter, QString &controle) {
        hydrate_attribut_xml(ent,pos,iter,controle);
        if(!controle.isEmpty())
            controle.prepend(QString("-> Dans le noeud d'attribut : ")
                             .append(iter->name()).append(" -> ").append(iter->text()).append("\n"));
    }


    //! Hydrate un attribut de l'entité entity_ass associée à entity avec le couple pair<clé,valeur>.
    virtual void hydrate_attribut_associated_xml(entity &ent_ass, const std::pair<const QString,QString> &pair,
                                               const entity &ent, QString &controle);

    //! Hydrate un attribut de l'entité entity_ass associée à entity avec le couple pair<clé,valeur>
    //! et complète le message d'erreur si besoin.
    void hydrate_attribut_associated_controled_xml(entity &ent_ass, const std::pair<const QString,QString> &pair,
                                              const entity &ent, QString &controle){
        hydrate_attribut_associated_xml(ent_ass,pair,ent,controle);
        if(!controle.isEmpty())
            controle.prepend(QString("-> Dans l'attribut : ").append(pair.first).append(" = ").append(pair.second).append("\n"));
    }

    //! Hydrate l'entité avec les valeurs contenues dans le XmlDox à l'endroit pointé par iter
    //! renvoie la liste des itérateurs sur les données associées.
    std::list<xml_iterator> hydrate_entity_xml(entity &ent, xml_iterator iter, QString &controle);

    //! Teste si un attribut d'une entité associée doit être pris en compte après l'enregistrement.
    virtual bool is_after_associated_xml(const std::pair<const QString,QString> &pair) const
        {return pair.first == "restriction";}

    //! Teste si un attribut est une entité associée.
    virtual bool is_associated_xml(xml_iterator iter, entity &ent) const;

    //! Teste si un attribut d'une entité associée est multiple.
    virtual bool is_multiple_associated_xml(const std::pair<const QString,QString> &/*pair*/) const
        {return false;}

    //! Mise à jour de la base de donnée.
    virtual void liste_mise_a_jour_bdd(int /*version*/, idt /*type*/) {}

    //! Renvoie la liste des attributs associés un attribut multiple.
    virtual xml_list_atts list_multiple_associated_xml(const std::pair<const QString,QString> &pair, QString &controle);

    //! Renvoie la liste des attributs associés un attribut multiple.
    virtual xml_list_atts list_multiple_associated_controled_xml(const std::pair<const QString,QString> &pair, QString &controle){
        auto list = list_multiple_associated_xml(pair,controle);
        if(!controle.isEmpty())
            controle.prepend(QString("-> Dans la liste d'attribut multiple : ")
                             .append(pair.first).append(" = ").append(pair.second));
        return list;
    }

    //! Ouverture de la base de donnée.
    bool open_bdd();

    //! position d'un attribut dans l'entité.
    post position_xml(const entity &ent, const QString &attribut, QString &controle) const {
        auto pos = ent.position(attribut);
        if(pos == ent.nbr_att())
            controle.append("-> L'entité de type : ").append(m_manager->info(ent.id_entity()).name())
                    .append("\nne possède pas d'attribut : ").append(attribut)
                    .append("\nL'entité : ").append(ent.affiche());
        return pos;
    }

    //! Enregistre les entités de vector dans la base de donnée.
    template<class Ent,class Conteneur> void save_conteneur(const Conteneur &vector);

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    virtual bool test_autorisation_v(idt id, entidt id_entity, flag autoris);

    //! Vérifie les autorisations des entités dont l'identifiant est dans liste.
    template<class Ent> bool test_autorisation(const std::list<idt> &liste, flag autoris) {
        auto it = liste.cbegin();
        while(it != liste.cend() &&test_autorisation_v(*it,Ent::ID, autoris))
            ++it;
        return it == liste.cend();
    }

    //! //! Vérifie les autorisations des entités de la liste vérifiant les conditions donnée en arguments.
    template<class Ent, class... Args> bool test_autorisation_list(flag autoris, Args... args)
        {return test_autorisation<Ent>(get_list_id<Ent>(args...), autoris);}
};

template<class Ent> bool bdd::exists(const Ent &ent)
    {return ent.is_new() ? false : m_manager->get<Ent>().exists(ent);}

template<class Ent> bool bdd::exists(typename Ent::position cle, const QVariant &value, condition cond)
    {return m_manager->get<Ent>().exists(cle,value,cond);}

template<class Ent> bool bdd::exists(const std::map<typename Ent::position, QVariant> &value, std::vector<condition> cond)
    {return m_manager->get<Ent>().exists(value,cond);}

template<class Ent> bool bdd::exists_unique(Ent &ent)
    {return m_manager->get<Ent>().exists_unique(ent) != Aucun;}

template<class Ent> bool bdd::exists_unique(const Ent &ent)
    {return m_manager->get<Ent>().exists_unique(ent) != Aucun;}

template<class Ent> existe_uni bdd::exists_unique_ensemble(Ent &ent)
    {return m_manager->get<Ent>().exists_unique(ent);}

template<class Ent> existe_uni bdd::exists_unique_ensemble(const Ent &ent)
    {return m_manager->get<Ent>().exists_unique(ent);}

template<class Ent> std::pair<existe_uni,idt> bdd::exists_unique_id(const Ent &ent)
    {return m_manager->get<Ent>().exists_unique_id(ent);}

template<class Ent> QVariant bdd::fonction_agrega(agrega fonc, typename Ent::position att)
    {return m_manager->get<Ent>().fonction_agrega(fonc, att);}

template<class Ent> QVariant bdd::fonction_agrega(agrega fonc, typename Ent::position att, typename Ent::position cle,
                                                   const QVariant &value, condition cond)
    {return m_manager->get<Ent>().fonction_agrega(fonc, att, cle, value, cond);}

template<class Ent> QVariant bdd::fonction_agrega(agrega fonc, typename Ent::position att,
                                                   typename Ent::position cle_1, const QVariant &value_1,
                                                   typename Ent::position cle_2, const QVariant &value_2,
                                                   condition cond_1, condition cond_2)
    {return m_manager->get<Ent>().fonction_agrega(fonc, att, cle_1, value_1, cle_2, value_2, cond_1, cond_2);}

template<class Ent, class Fct> bool bdd::foreach_begin_child(idt id, const Fct &fct, bool ordre) {
    auto childs = m_manager->get<Ent>().get_list_childs_id_leaf(id);
    if(ordre){
        auto i = childs.cbegin();
        while (i != childs.cend() &&(
                   ((*i).second &&fct((*i).first))
                   || foreach_begin_child<Ent>((*i).first,fct,ordre)
                   ))
               ++i;
        return i == childs.cend() &&fct(id);
    }
    else {
        auto i = childs.crbegin();
        while (i != childs.crend() &&(
                   ((*i).second &&fct((*i).first))
                   || foreach_begin_child<Ent>((*i).first,fct,ordre)
                   ))
               ++i;
        return i == childs.crend() &&fct(id);
    }
}

template<class Ent, class Fct> bool bdd::foreach_node(idt id, const Fct &fct, bool ordre) {
    if(fct(id)) {
        auto childs = m_manager->get<Ent>().get_list_childs_id_leaf(id);
        if(ordre) {
            auto i = childs.cbegin();
            while (i != childs.cend()  &&(
                       ((*i).second &&fct((*i).first))
                       ||foreach_node<Ent>((*i).first,fct,ordre)
                       ))
                   ++i;
            return i == childs.cend();
        }
        else {
            auto i = childs.crbegin();
            while (i != childs.crend()  &&(
                       ((*i).second &&fct((*i).first))
                       ||foreach_node<Ent>((*i).first,fct,ordre)
                       ))
                   ++i;
            return i == childs.crend();
        }
    }
    return false;
}

template<class Ent> bool bdd::get(Ent &ent)
    {return m_manager->get<Ent>().get(ent);}

template<class Ent> tree<Ent> bdd::get_arbre()
    {return m_manager->get<Ent>().get_arbre();}

template<class Ent> tree<Ent> bdd::get_arbre(idt id)
    {return m_manager->get<Ent>().get_arbre(id);}

template<class Ent> tree<Ent> bdd::get_arbre(const Ent &ent)
    {return m_manager->get<Ent>().get_arbre(ent.id());}

template<class Ent> Ent bdd::get_child(idt id_parent, int num)
    {return m_manager->get<Ent>().get_child(id_parent,num);}

template<class Ent> tree<idt> bdd::get_id_arbre(idt id)
    {return m_manager->get<Ent>().get_id_arbre(id);}

template<class Ent> idt bdd::get_id_child(idt id_parent, int num)
    {return m_manager->get<Ent>().get_id_child(id_parent,num);}

template<class Ent> idt bdd::get_id_root(idt id)
    {return m_manager->get<Ent>().get_id_root(id);}

template<class Ent> vector_ptr<Ent> bdd::get_list(typename Ent::position ordre, bool croissant)
    {return m_manager->get<Ent>().get_list(ordre, croissant);}

template<class Ent> vector_ptr<Ent> bdd::get_list(typename Ent::position cle, int value, typename Ent::position ordre,
                                              condition cond, bool crois)
    {return m_manager->get<Ent>().get_list(cle, QVariant(value), ordre, cond, crois);}

template<class Ent> vector_ptr<Ent> bdd::get_list(typename Ent::position cle, idt value, typename Ent::position ordre,
                                              condition cond, bool crois)
    {return m_manager->get<Ent>().get_list(cle, QVariant(value), ordre, cond, crois);}

template<class Ent> vector_ptr<Ent> bdd::get_list(typename Ent::position cle, const QVariant &value,
                                              typename Ent::position ordre, condition cond, bool crois)
    {return m_manager->get<Ent>().get_list(cle, value, ordre, cond, crois);}

template<class Ent> vector_ptr<Ent> bdd::get_list(typename Ent::position cle, const QVariant &value,
                                              typename Ent::position order1, typename Ent::position order2,
                                              condition cond, bool croissant1, bool croissant2)
    {return m_manager->get<Ent>().get_list(cle, value, order1, order2, cond, croissant1, croissant2);}

template<class Ent> vector_ptr<Ent> bdd::get_list(typename Ent::position cle, const QVariant &value,
                                              typename Ent::position order1, typename Ent::position order2,
                                              typename Ent::position ordre_3, condition cond,
                                              bool croissant1, bool croissant2, bool croissant3)
    {return m_manager->get<Ent>().get_list(cle, value, order1, order2, ordre_3, cond, croissant1, croissant2, croissant3);}

template<class Ent> vector_ptr<Ent> bdd::get_list(typename Ent::position cle_1, const QVariant &value_1,
                                              typename Ent::position cle_2, const QVariant &value_2,
                                              typename Ent::position order, condition cond_1, condition cond_2, bool croissant)
    {return m_manager->get<Ent>().get_list(cle_1, value_1, cle_2, value_2, order, cond_1, cond_2, croissant);}

template<class Ent> vector_ptr<Ent> bdd::get_list(typename Ent::position cle_1, const QVariant &value_1,
                                              typename Ent::position cle_2, const QVariant &value_2,
                                              typename Ent::position cle_3, const QVariant &value_3,
                                              typename Ent::position order,
                                              condition cond_1, condition cond_2, condition cond_3, bool croissant)
    {return m_manager->get<Ent>().get_list(cle_1, value_1, cle_2, value_2, cle_3, value_3,
                                           order, cond_1, cond_2, cond_3, croissant);}

template<class Ent> vector_ptr<Ent> bdd::get_list(const std::vector<typename Ent::position> &cle,
                                                const std::vector<QVariant> &value,
                                                const std::vector<typename Ent::position> &ordre,
                                                const std::vector<condition> &condition, const std::vector<bool> &crois)
    {return m_manager->get<Ent>().get_list(cle,value,ordre,condition,crois);}

template<class Ent, class Join> vector_ptr<Ent> bdd::get_list(typename Ent::position colonne_table,
                                    typename Join::position colonne_join,
                                    const std::map<typename Ent::position, QVariant> &where_map_table,
                                    const std::map<typename Join::position, QVariant> &where_map_join,
                                    const std::vector<std::pair<typename Ent::position, bool>> &order_map_table) {
    std::map<QString,QVariant> where_map_joinString;
    for(auto i = where_map_join.cbegin(); i != where_map_join.cend(); i++)
        where_map_joinString[m_manager->get<Join>().info().attribut(i->first)] = i->second;
    return m_manager->get<Ent>().get_list_join(m_manager->get<Join>().info().table(),
                                             colonne_table,
                                             m_manager->get<Join>().info().attribut(colonne_join),
                                             where_map_table, where_map_joinString, order_map_table);
}

template<class Ent, class Join> vector_ptr<Ent> bdd::get_list(typename Join::position cleJoin, typename Join::position cleWhere,
                                                            const QVariant &value_where, typename Ent::position ordre,
                                                            condition cond, bool crois) {
    return m_manager->get<Ent>().get_list_join(m_manager->get<Join>().info().table(),
                                             m_manager->get<Join>().info().attribut(cleJoin),
                                             m_manager->get<Join>().info().attribut(cleWhere),
                                             value_where, ordre, cond, crois);
}

template<class Ent, class Join> vector_ptr<Ent> bdd::get_list(typename Join::position cleJoin,
                                                       typename Join::position cleWhere1, const QVariant &value_where1,
                                                       typename Join::position cleWhere2, const QVariant &value_where2,
                                                       typename Ent::position ordre, bool crois){
    std::map<typename Join::position, QVariant> mapWhere;
    mapWhere[cleWhere1] = value_where1;
    mapWhere[cleWhere2] = value_where2;
    return get_list<Ent,Join>(Ent::Id,cleJoin,std::map<typename Ent::position, QVariant>(),mapWhere,
                             std::vector<std::pair<typename Ent::position, bool>>
                             ({std::pair<typename Ent::position, bool>(ordre,crois)}));
}

template<class Ent , class Join1, class Join2> vector_ptr<Ent> bdd::get_list(typename Join1::position cle_1,
                                                          typename Join1::position cle_2,
                                                          typename Join2::position cleWhere,
                                                          const QVariant &value_where,
                                                          typename Ent::position ordre,
                                                          condition cond, bool crois) {
    return m_manager->get<Ent>().get_list_join(m_manager->get<Join1>().info().table(),
                                             m_manager->get<Join2>().info().table(),
                                             m_manager->get<Join1>().info().attribut(cle_1),
                                             m_manager->get<Join1>().info().attribut(cle_2),
                                             m_manager->get<Join2>().info().attribut(cleWhere),
                                             value_where, ordre, cond, crois);
}

template<class Ent> vector_ptr<Ent> bdd::get_list_childs(const Ent &ent, typename Ent::position ordre)
    {return m_manager->get<Ent>().get_list_childs(ent,ordre);}

template<class Ent> std::list<idt> bdd::get_list_childs_id(idt id, typename Ent::position ordre)
    {return m_manager->get<Ent>().get_list_childs_id(id,ordre);}

template<class Ent> std::vector<std::pair<idt,bool>> bdd::get_list_childs_id_leaf(int id)
    {return m_manager->get<Ent>().get_list_childs_id_leaf(id);}

template<class Ent> std::list<idt> bdd::get_list_id(typename Ent::position cle, int value, typename Ent::position ordre,
                                              condition cond, bool crois)
    {return m_manager->get<Ent>().get_list_id(cle,value,ordre,cond,crois);}

template<class Ent> std::list<idt> bdd::get_list_id(typename Ent::position cle, idt value, typename Ent::position ordre,
                                 condition cond, bool crois)
    {return m_manager->get<Ent>().get_list_id(cle,value,ordre,cond,crois);}

template<class Ent> std::list<idt> bdd::get_list_id(typename Ent::position cle, const QVariant &value,
                                 typename Ent::position ordre, condition cond, bool crois)
    {return m_manager->get<Ent>().get_list_id(cle,value,ordre,cond,crois);}

template<class Ent> std::list<idt> bdd::get_list_id(typename Ent::position cle, const QVariant &value,
                                 typename Ent::position ordre_1, typename Ent::position ordre_2,
                                 condition cond, bool croissant1, bool croissant2)
    {return m_manager->get<Ent>().get_list_id(cle,value,ordre_1,ordre_2,cond,croissant1,croissant2);}

template<class Ent> std::list<idt> bdd::get_list_id(typename Ent::position cle, const QVariant &value,
                                 typename Ent::position ordre_1, typename Ent::position ordre_2, typename Ent::position ordre_3,
                                 condition cond, bool crois_1, bool crois_2, bool crois_3)
    {return m_manager->get<Ent>().get_list_id(cle,value,ordre_1,ordre_2,ordre_3,cond,crois_1,crois_2,crois_3);}

template<class Ent> std::list<idt> bdd::get_list_id(typename Ent::position cle_1, const QVariant &value_1,
                                 typename Ent::position cle_2, const QVariant &value_2,
                                 typename Ent::position ordre, condition cond_1, condition cond_2, bool crois)
    {return m_manager->get<Ent>().get_list_id(cle_1,value_1,cle_2,value_2,ordre,cond_1,cond_2,crois);}

template<class Ent> std::list<idt> bdd::get_list_id(typename Ent::position cle_1, const QVariant &value_1,
                                 typename Ent::position cle_2, const QVariant &value_2,
                                 typename Ent::position cle_3, const QVariant &value_3,
                                 typename Ent::position ordre,
                                 condition cond_1, condition cond_2,condition cond_3,
                                 bool crois)
    {return m_manager->get<Ent>().get_list_id(cle_1,value_1,cle_2,value_2,cle_3,value_3,ordre,cond_1,cond_2,cond_3,crois);}

template<class Ent> std::list<idt> bdd::get_list_id(const std::vector<typename Ent::position> &cle,
                         const std::vector<QVariant> &value,
                         const std::vector<typename Ent::position> &ordre,
                         const std::vector<condition> &condition,
                         const std::vector<bool> &crois)
    {return m_manager->get<Ent>().get_list_id(cle, value,ordre, condition, crois);}

template<class Ent> map_id<Ent> bdd::get_map(typename Ent::position cle_map)
    {return m_manager->get<Ent>().get_map(cle_map);}

template<class Ent> map_id<Ent> bdd::get_map(typename Ent::position cle, const QVariant &value,
                                            typename Ent::position cle_map, condition cond)
    {return m_manager->get<Ent>().get_map(cle, value, cle_map, cond);}

template<class Ent> map_id<Ent> bdd::get_map(typename Ent::position cle_1, const QVariant &value_1,
                                            typename Ent::position cle_2, const QVariant &value_2,
                                            typename Ent::position cle_map, condition cond_1, condition cond_2)
    { return m_manager->get<Ent>().get_map(cle_1, value_1, cle_2, value_2, cle_map, cond_1, cond_2);}

template<class Ent> map_id<Ent> bdd::get_map(typename Ent::position cle_1, const QVariant &value_1,
                                            typename Ent::position cle_2, const QVariant &value_2,
                                            typename Ent::position cle_3, const QVariant &value_3,
                                            typename Ent::position cle_map,
                                            condition cond_1, condition cond_2, condition cond_3)
    {return m_manager->get<Ent>().get_map(cle_1, value_1, cle_2, value_2, cle_3, value_3, cle_map, cond_1, cond_2, cond_3);}

template<class Ent> map_id<Ent> bdd::get_map(const std::vector<typename Ent::position> &cle,
                                            const std::vector<QVariant> &value,
                                            typename Ent::position cle_map,
                                            const std::vector<typename Ent::position> &ordre,
                                            const std::vector<condition> &cond,
                                            const std::vector<bool> &crois)
    {return m_manager->get<Ent>().get_map(cle,value,cle_map,ordre,cond,crois);}

template<class Ent, class Join> map_id<Ent> bdd::get_map(typename Ent::position colonne_table,
                                                        typename Join::position colonne_join,
                                                        const std::map<typename Ent::position,QVariant> &where_map_table,
                                                        const std::map<typename Join::position,QVariant> &where_map_join,
                                                        typename Ent::position cle_map) {
    std::map<QString,QVariant> where_map_joinString;
    for(auto i = where_map_join.cbegin(); i != where_map_join.cend(); i++)
        where_map_joinString[m_manager->get<Join>().attribut(i->first)] = i->second;
    return m_manager->get<Ent>().get_map_join(m_manager->get<Join>().table(),colonne_table,
                                            m_manager->get<Join>().attribut(colonne_join),
                                            where_map_table, where_map_joinString, cle_map);
}

template<class Ent, class Join> map_id<Ent> bdd::get_map(typename Join::position cleJoin,
                                                                typename Join::position cleWhere, const QVariant &value_where,
                                                                typename Ent::position cle_map, condition cond) {
    return m_manager->get<Ent>().get_map_join(m_manager->get<Join>().table(),m_manager->get<Join>().attribut(cleJoin),
                                             m_manager->get<Join>().attribut(cleWhere),
                                                      value_where, cle_map, cond);
}

template<class Ent> flag bdd::get_restriction(const Ent &ent)
    {return m_manager->get<Ent>().get_restriction(ent.id());}

template<class Ent> bool bdd::get_unique(Ent &ent)
    {return m_manager->get<Ent>().get_unique(ent);}

template<class Ent> void bdd::insert(Ent &ent, idt id_parent, int num)
    {m_manager->get<Ent>().insert(ent,id_parent,num);}

template<class Ent> void bdd::insert(const Ent &ent, idt id_parent, int num)
    {m_manager->get<Ent>().insert(ent,id_parent,num);}

template<class Ent> bool bdd::is_leaf(idt id)
    {return m_manager->get<Ent>().is_leaf(id);}

template<class Ent> bool bdd::same_in_bdd(const Ent &ent)
    {return m_manager->get<Ent>().same_in_bdd(ent);}

template<class Ent> void bdd::save(Ent &ent)
    {m_manager->get<Ent>().save(ent);}

template<class Ent> void bdd::save(const Ent &ent)
    {m_manager->get<Ent>().save(ent);}

template<class Ent> void bdd::save(Ent &ent, flag restrict)
    {m_manager->get<Ent>().save(ent, restrict);}

template<class Ent> void bdd::save(const Ent &ent, flag restrict)
    {m_manager->get<Ent>().save(ent, restrict);}

template<class Ent> void bdd::save_unique(Ent &ent)
    {m_manager->get<Ent>().save_unique(ent);}

template<class Ent> void bdd::save_unique(const Ent &ent)
    {m_manager->get<Ent>().save_unique(ent);}

template<class Ent, class Conteneur> void bdd::save_conteneur(const Conteneur &vector) {
    for(typename Conteneur::iterator i = vector.begin(); i != vector.end(); ++i)
       m_manager->get<Ent>().save(*i);
}

template<class Ent> void bdd::save(tree<Ent> &arbre, tree_save n)
    {m_manager->get<Ent>().save(arbre,n);}

template<class Ent> void bdd::set_restriction(const Ent &ent, flag restrict)
    {m_manager->get<Ent>().set_restriction(ent.id(), restrict);}

template<class Ent> int bdd::size_brother(idt id)
    {return m_manager->get<Ent>().size_brother(id);}

template<class Ent> int bdd::size_child(idt id)
    {return m_manager->get<Ent>().size_child(id);}
}}
#endif // BDD_H
