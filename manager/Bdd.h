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
#include "Managers.h"
#include "file_interface.h"
#include "xml.h"

/*! \ingroup groupeFile
 *\brief Corps des deux méthodes delarbre.
 */
//#define DEL_ARBRE {return foreachBeginChild(id,
//    [this,&delFonction(idt id)->bool{return delFonction(id) &&del<Ent>(id);}]
//    ,false);}

//vector_ptr<Ent> childs = getVectorChilds(entity);
//    typename vector_ptr<Ent>::iterator i = childs.begin();
//    while(i != childs.end() &&del(*i)) ++i;
//    if(i == childs.end() &&delFonction(entity)) return Bdd::del(entity);
//    else return false;}

/*! \ingroup groupeFile
 *\brief Corps de la méthodes delarbre.
 */
//#define DEL_ARBRE_MODIF_CONTROLE {return delModifControle(entity,[this,&delFonction](const Ent &ent)->bool{
//    vector_ptr<Ent> childs = getVectorChilds(entity);
//    typename vector_ptr<Ent>::iterator i = childs.begin();
//    while(i != childs.end() &&del(*i)) ++i;
//    return i == childs.end() &&delFonction(entity);});}

/*! \ingroup groupeFile
 *\brief Macro permettant définir les méthodes de suppression.
 */
//#define DEF_DEL_METHODE(CLASS)
    //template<class Ent, class U> bool Bdd::delarbre(Ent &ent, U delFonction) DEL_ARBRE
//    template<class Ent, class U> bool Bdd::delarbreModifControle(Ent &ent, U delFonction) DEL_ARBRE_MODIF_CONTROLE

namespace bddMPS {
using namespace conteneurMPS;
using entity = entities::entity;
/*! \ingroup groupeFile
 * \brief Classe mère du gestionnaire de base de donnée.
 *
 *Gestionnaire de la base donnée sous SQLite3. Cette classe fait le lien entre la base de donnée et les entity grâce aux Manager.
 */
class Bdd : public fichierMPS::file_interface
{
protected:
    QSqlDatabase m_bdd;                                 //!< Connexion à la base de donnée.
    std::unique_ptr<managerMPS::Managers> m_manager;    //!< Manager des entités.
    const std::vector<int> m_version;                   //!< Version de la base de donnée requis par le programme
public:
    // Alias de type
    using vector_id_name = std::vector<std::pair<int,QString>>;
    using xml_iterator = fichierMPS::doc_xml::const_brother_iterator;
    using xml_list_atts = std::list<std::pair<QString,QString>>;

    // Enumération
    enum {InvalideEnum = 0x1000000,
          Noentity
         };
    //! Catégorie des énumérations
    enum categorie : flag::flag_type {RestrictionCode = 0x1,
                                      PermissionCode = 0x2,
                                      LineStyle = 0x4,
                                      BrushStyle = 0x8,
                                      FontWeight = 0x10
                                     };
    //! Options d'exportation xml
    enum export_option : flag::flag_type {RestrictionExportationXml = 0x1};

    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    Bdd(const QString &dbtype, const std::vector<int> &version, std::unique_ptr<managerMPS::Managers> &&manager = nullptr)
        : file_interface(QString(),"Data Base files (*.db)"),
           m_bdd(QSqlDatabase::addDatabase(dbtype)),
           m_manager(std::move(manager)),
           m_version(version) {}


    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée, la version et le managers.
    Bdd(const QString &dbtype, const QString &name, const std::vector<int> &version,
        std::unique_ptr<managerMPS::Managers> &&manager = nullptr)
        : file_interface(name,"Data Base files (*.db)"),
           m_bdd(QSqlDatabase::addDatabase(dbtype)),
           m_manager(std::move(manager)),
           m_version(version)
        {m_bdd.set_databaseName(name);}

    //! Destructeur. Referme la base de donnée.
    ~Bdd() override
        {m_bdd.close();}

    //! Ajoute des restrictions de modification pour une entité donnée.
    void addRestriction(const entity &ent, flag restrict)
        {m_manager->get(entity.id_entity()).addRestriction(entity.id(), restrict);}

    //! Renvoie le code associé à une chaine de caractère (séparateur "|") pour l'entité identity.
    flag codeFromQString(const QString &str, uint idCat, bool forentity,  QString &controle) const;

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
        {return delP(entity.id(),entity.id_entity());}

    //! Supprime l'entité entity de la base de donnée et met l'identifiant de l'entité à zéro.
    bool del(entity &ent) {
        bool bb = del(static_cast<const entity &>(entity));
        if(bb)
            entity.set_id(0);
        return bb;
    }

    //! Supprime les entités d'une liste.
    template<class Ent> bool del(const conteneurMPS::vector_ptr<Ent> &liste) {
        auto i = liste.cbegin();
        while(i != liste.cend() &&del(*i))
            ++i;
        return i == liste.cend();
    }

    //! Supprime les entités d'une liste.
    template<class Ent> bool del(conteneurMPS::vector_ptr<Ent> &liste) {
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
    template<class Ent> bool exists(typename Ent::Position cle, const QVariant &value, condition cond = condition::Egal);

    //! Teste s'il existe une entité vérifiant des conditions.
    template<class Ent> bool exists(const std::map<typename Ent::Position, QVariant> &value,
                    std::vector<condition> cond = std::vector<condition>({condition::Egal}));

    //! Teste s'il existe une entité ayant les mêmes valeurs qu'un des ensemble d'attributs uniques d'entity en base de donnée.
    //! De plus, si l'identifiant de entity est nul et qu'il existe en base de donnée exactement une entité possédant des ensembles
    //! d'attributs uniques avec les mêmes valeurs qu'entity,
    //! alors l'identifiant d'entity est remplacé par l'identifiant de cette entité.
    bool existsUnique(entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs qu'un des ensemble d'attributs uniques d'entity en base de donnée.
    //! De plus, si l'identifiant de entity est nul et qu'il existe en base de donnée exactement une entité possédant des ensembles
    //! d'attributs uniques avec les mêmes valeurs qu'entity,
    //! alors l'identifiant d'entity est remplacé par l'identifiant de cette entité.
    template<class Ent> bool existsUnique(Ent &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs qu'un ensemble d'attributs uniques que entity en base de donnée.
    bool existsUnique(const entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs qu'un ensemble d'attributs uniques que entity en base de donnée.
    template<class Ent> bool existsUnique(const Ent &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    existeUni existsUniqueEnsemble(entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    template<class Ent> existeUni existsUniqueEnsemble(Ent &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    existeUni existsUniqueEnsemble(const entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que entity en base de donnée.
    template<class Ent> existeUni existsUniqueEnsemble(const Ent &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    std::pair<existeUni,idt> existsUniqueId(const entity &ent);

    //! Teste s'il existe une entité ayant les mêmes valeurs d'attributs uniques que l'entité entity en base de donnée
    //! et renvoie l'identifiant du premier trouver.
    template<class Ent> std::pair<existeUni,idt> existsUniqueId(const Ent &ent);

    //! Créer un arbre doc_xml à partir d'une liste d'entités.
    fichierMPS::doc_xml exportXml(vector_ptr<entity> &&vec, flag option);

    //! Fonction d'agrega sur l'attribut att appliquée à toutes les entités de la table.
    template<class Ent> QVariant fonctionAgrega(agrega fonc, typename Ent::Position att);

    //! Fonction d'agrega  sur l'attribut att appliquée à toutes les entités vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    template<class Ent> QVariant fonctionAgrega(agrega fonc, typename Ent::Position att, typename Ent::Position cle,
                                                  const QVariant &value, condition cond = condition::Egal);

    //! Fonction d'agrega sur l'attribut att appliquée à toutes les entités vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    template<class Ent> QVariant fonctionAgrega(agrega fonc, typename Ent::Position att, typename Ent::Position cle1,
                                                  const QVariant &value1, typename Ent::Position cle2,  const QVariant &value2,
                                                  condition cond1 = condition::Egal, condition cond2 = condition::Egal);

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant de celui d'identifiant id
    //! en commençant par les descendants.
    template<class Ent, class Fct> bool foreachBeginChild(idt id, const Fct &fct, bool ordre = true);

    //! Applique la fonction fonction bool fct(idt id) à chaque noeud descendant celui d'identifiant id.
    template<class Ent, class Fct> bool foreachNode(idt id, const Fct &fct, bool ordre = true);

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool get(entity &ent);

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant le même identifiant que entity.
    //! Retourne True si l'opération s'est correctement déroulée.
    template<class Ent> bool get(Ent &ent);

    //! Renvoie l'arbre de toutes les entités pour une entité de type arbre.
    template<class Ent> tree<Ent> getarbre();

    //! Renvoie l'arbre de racine d'identifiant id pour une entité de type arbre.
    template<class Ent> tree<Ent> getarbre(idt id);

    //! Renvoie l'arbre de racine d'entité entity pour une entité de type arbre.
    template<class Ent> tree<Ent> getarbre(const Ent &ent);

    //! Renvoie l'identifiant du parent (si le manager est de type arbre).
    idt getIdParent(const entity &ent);

    //! Renvoie la liste des entités de la table des entités d'identifiant ent_id ordonnée suivant la colonne d'identifiant ordre.
    vector_ptr<entity> getList(entidt ent_id);

    //! Renvoie la liste des entités de la table des entités Ent ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> getList(typename Ent::Position ordre = Ent::Id, bool croissant = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> getList(typename Ent::Position cle, int value, typename Ent::Position ordre = Ent::Id,
                                             condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> getList(typename Ent::Position cle, idt value, typename Ent::Position ordre = Ent::Id,
                                             condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> getList(typename Ent::Position cle, const QVariant &value,
                                               typename Ent::Position ordre = Ent::Id,
                                               condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    template<class Ent> vector_ptr<Ent> getList(typename Ent::Position cle, const QVariant &value,
                                             typename Ent::Position order1, typename Ent::Position order2,
                                             condition cond = condition::Egal, bool croissant1 = true, bool croissant2 = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1, ordre2 puis ordre3.
    template<class Ent> vector_ptr<Ent> getList(typename Ent::Position cle, const QVariant &value,
                                               typename Ent::Position ordre1, typename Ent::Position ordre2,
                                               typename Ent::Position ordre3, condition cond = condition::Egal,
                                               bool crois1 = true, bool crois2 = true, bool crois3 = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> getList(typename Ent::Position cle1, const QVariant &value1,
                                             typename Ent::Position cle2, const QVariant &value2,
                                             typename Ent::Position ordre = Ent::Id,
                                             condition cond1 = condition::Egal, condition cond2 = condition::Egal,
                                             bool crois = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> vector_ptr<Ent> getList(typename Ent::Position cle1, const QVariant &value1,
                                             typename Ent::Position cle2, const QVariant &value2,
                                             typename Ent::Position cle3, const QVariant &value3,
                                             typename Ent::Position ordre = Ent::Id,
                                             condition cond1 = condition::Egal, condition cond2 = condition::Egal,
                                             condition cond3 = condition::Egal,
                                             bool crois = true);

    //! Renvoie la liste des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre,
    //! croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    template<class Ent> vector_ptr<Ent> getList(const std::vector<typename Ent::Position> &cle, const std::vector<QVariant> &value,
                         const std::vector<typename Ent::Position> &ordre = std::vector<typename Ent::Position>(),
                         const std::vector<condition> &cond = std::vector<condition>(),
                         const std::vector<bool> &crois = std::vector<bool>());

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map whereMapTable,
    //! valeur des colonnes de la table Join d'identifiant key = value de std::map whereMapJoin,
    //! ordonnée suivant les colonnes de la table Ent d'identifiants key et d'ordre value de std::map orderMapTable
    //! (true -> croissant, false -> décroissant).
    template<class Ent, class Join> vector_ptr<Ent> getList(typename Ent::Position colonneTable,
                                                   typename Join::Position colonneJoin,
                                                   const std::map<typename Ent::Position,QVariant> &whereMapTable,
                                                   const std::map<typename Join::Position,QVariant> &whereMapJoin,
                                                   const std::vector<std::pair<typename Ent::Position,bool>> &orderMapTable);

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (Ent.ID = join.cleJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    template<class Ent, class Join> vector_ptr<Ent> getList(typename Join::Position cleJoin, typename Join::Position cleWhere,
                                                         const QVariant &valueWhere, typename Ent::Position ordre = Ent::Id,
                                                         condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des entités de la table vérifiant deux conditions sur une jointure (Ent.ID = join.cleJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere1 = valueWhere1 et cleWhere2 = valueWhere2,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    template<class Ent, class Join> vector_ptr<Ent> getList(typename Join::Position cleJoin,
                                                           typename Join::Position cleWhere1, const QVariant &valueWhere1,
                                                           typename Join::Position cleWhere2, const QVariant &valueWhere2,
                                                           typename Ent::Position ordre = Ent::Id, bool crois = true);

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure
    //! (table.ID = join1.colonne1, join2.ID = join1.colonne2),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere sur la seconde jointure,
    //! ordonnée suivant la colonne de l'entité d'identifiant ordre.
    template<class Ent , class Join1, class Join2> vector_ptr<Ent> getList(typename Join1::Position cle1,
                                                                              typename Join1::Position cle2,
                                                                              typename Join2::Position cleWhere,
                                                                              const QVariant &valueWhere,
                                                                              typename Ent::Position ordre = Ent::Id,
                                                                              condition cond = condition::Egal, bool crois = true);

    //! Renvoie le liste des descendant direct d'entity.
    template<class Ent> vector_ptr<Ent> getListChilds(const Ent &ent);

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id.
    template<class Ent> std::list<idt> getListChildsId(idt id);

    //! Renvoie le liste des identifiants des descendant direct de l'entité d'identifiant id
    //! ainsi que si ce descendant est une feuille ou non.
    template<class Ent> std::vector<std::pair<idt,bool>> getListChildsIdLeaf(int id);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> getListId(typename Ent::Position cle, int value, typename Ent::Position ordre = Ent::Id,
                                 condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> getListId(typename Ent::Position cle, idt value, typename Ent::Position ordre = Ent::Id,
                                 condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> getListId(typename Ent::Position cle, const QVariant &value,
                                 typename Ent::Position ordre = Ent::Id,
                                 condition cond = condition::Egal, bool crois = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1 puis ordre2.
    template<class Ent> std::list<idt> getListId(typename Ent::Position cle, const QVariant &value,
                                 typename Ent::Position ordre1, typename Ent::Position ordre2,
                                 condition cond = condition::Egal,
                                 bool croissant1 = true, bool croissant2 = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value, ordonnée suivant les colonnes d'identifiant ordre1, ordre2 puis ordre3.
    template<class Ent> std::list<idt> getListId(typename Ent::Position cle, const QVariant &value,
                                 typename Ent::Position ordre1, typename Ent::Position ordre2, typename Ent::Position ordre3,
                                 condition cond = condition::Egal,
                                 bool crois1 = true, bool crois2 = true, bool crois3 = true);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2,
    //! ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> getListId(typename Ent::Position cle1, const QVariant &value1,
                                 typename Ent::Position cle2, const QVariant &value2,
                                 typename Ent::Position ordre = Ent::Id,
                                 condition cond1 = condition::Egal, condition cond2 = condition::Egal,
                                 bool crois = true);

    //! Renvoie la liste des identifiants de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3,
    //! ordonnée suivant la colonne d'identifiant ordre.
    template<class Ent> std::list<idt> getListId(typename Ent::Position cle1, const QVariant &value1,
                                 typename Ent::Position cle2, const QVariant &value2,
                                 typename Ent::Position cle3, const QVariant &value3,
                                 typename Ent::Position ordre = Ent::Id,
                                 condition cond1 = condition::Egal, condition cond2 = condition::Egal,
                                 condition cond3 = condition::Egal,
                                 bool crois = true);

    //! Renvoie la liste des identifiants de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre, croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    template<class Ent> std::list<idt> getListId(const std::vector<typename Ent::Position> &cle, const std::vector<QVariant> &value,
                         const std::vector<typename Ent::Position> &ordre = std::vector<typename Ent::Position>(),
                         const std::vector<condition> &cond = std::vector<condition>(),
                         const std::vector<bool> &crois = std::vector<bool>());

    //! Renvoie la map des entités de la table des entités Ent.
    template<class Ent> map_id<Ent> getMap(typename Ent::Position cleMap = Ent::Id);

    //! Renvoie la map des entités de la table des entités Ent vérifiant la condition,
    //! valeur de la colonne d'identifiant cle = value.
    template<class Ent> map_id<Ent> getMap(typename Ent::Position cle, const QVariant &value,
                                           typename Ent::Position cleMap = Ent::Id, condition cond = condition::Egal);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1 et valeur de la colonne d'identifiant cle2 = value2.
    template<class Ent> map_id<Ent> getMap(typename Ent::Position cle1, const QVariant &value1,
                                           typename Ent::Position cle2, const QVariant &value2,
                                           typename Ent::Position cleMap = Ent::Id,
                                           condition cond1 = condition::Egal, condition cond2 = condition::Egal);

    //! Renvoie la liste des entités de la table des entités Ent vérifiant les deux conditions,
    //! valeur de la colonne d'identifiant cle1 = value1, valeur de la colonne d'identifiant cle2 = value2,
    //! et valeur de la colonne d'identifiant cle3 = value3.
    template<class Ent> map_id<Ent> getMap(typename Ent::Position cle1, const QVariant &value1,
                                             typename Ent::Position cle2, const QVariant &value2,
                                             typename Ent::Position cle3, const QVariant &value3,
                                             typename Ent::Position cleMap = Ent::Id,
                                             condition cond1 = condition::Egal, condition cond2 = condition::Egal,
                                             condition cond3 = condition::Egal);

    //! Renvoie la liste des entités de la table vérifiant les conditions, pour tout i,
    //! valeur de la colonne d'identifiant cle[i] condition[i] value[i],
    //! ordonnée suivant les colonnes d'identifiant contenue dans ordre, croissante (crois[i]=true) ou décroissante (croiss[i]=false).
    template<class Ent> map_id<Ent> getMap(const std::vector<typename Ent::Position> &cle, const std::vector<QVariant> &value,
                         typename Ent::Position cleMap = Ent::Id,
                         const std::vector<typename Ent::Position> &ordre = std::vector<typename Ent::Position>(),
                         const std::vector<condition> &cond = std::vector<condition>(),
                         const std::vector<bool> &crois = std::vector<bool>());

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (colonneTable = colonneJoin),
    //! valeur des colonnes de la table Ent d'identifiant key = value de std::map whereMapTable,
    //! valeur des colonnes de la table Join d'identifiant key = value de std::map whereMapJoin.
    template<class Ent, class Join> map_id<Ent> getMap(typename Ent::Position colonneTable,
                                                       typename Join::Position colonneJoin,
                                                       const std::map<typename Ent::Position,QVariant> &whereMapTable,
                                                       const std::map<typename Join::Position,QVariant> &whereMapJoin,
                                                       typename Ent::Position cleMap = Ent::Id);

    //! Renvoie la liste des entités de la table vérifiant une condition sur une jointure (Ent.ID = Join.cleJoin),
    //! valeur de la colonne de la jointure d'identifiant cleWhere = valueWhere.
    template<class Ent, class Join> map_id<Ent> getMap(typename Join::Position cleJoin, typename Join::Position cleWhere,
                                                       const QVariant &valueWhere, typename Ent::Position cleMap = Ent::Id,
                                                       condition cond = condition::Egal);

    //! Renvoie les restrictions de modification de l'entité donnée en argument.
    flag getRestriction(const entity &ent);

    //! Renvoie les restrictions de modification de l'entité donnée en argument.
    template<class Ent> flag getRestriction(const Ent &ent);

    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant les mêmes valeurs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    bool getUnique(entity &ent);


    //! Hydrate l'entité entity avec les valeurs des attributs de l'entité enregistrées en base de donnée
    //! ayant les mêmes valeurs uniques.
    //! Retourne True si l'opération s'est correctement déroulée.
    template<class Ent> bool getUnique(Ent &ent);

    //! Renvoie l'identifiant de l'entité de type Ent d'Id_Prog idP fourni ou 0 si elle n'existe pas.
    template<class Ent> idt Id_ProgToId(idt idP) {
         Ent entity;
         entity.set_id_prog(idP);
         return existsUnique(entity) ? entity.id() : 0;
    }

    //! Importation de donnée à partir d'un xmlDoc.
    QString importXml(const fichierMPS::doc_xml &doc);

    //! Renvoie les informations de la base de donnée sur les entity du même type que entity.
    const managerMPS::InfoBdd &info(const entity &ent) const;

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(entity &ent, idt idParent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(entity &ent, idt idParent, numt num = 0)
        {insert(entity,idParent,static_cast<int>(num));}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    template<class Ent> void insert(Ent &ent, idt idParent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    template<class Ent> void insert(Ent &ent, idt idParent, numt num = 0)
        {insert(entity,idParent,static_cast<int>(num));}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(const entity &ent, idt idParent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    void insert(const entity &ent, idt idParent, numt num = 0)
        {insert(entity,idParent,static_cast<int>(num));}

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    template<class Ent> void insert(const Ent &ent, idt idParent, int num = 0);

    //! Enregistre l'entité entity en base avec le parent et la position spécifiés.
    template<class Ent> void insert(const Ent &ent, idt idParent, numt num = 0)
        {insert(entity,idParent,static_cast<int>(num));}

    //! Teste si la base de donnée est valide.
    bool is_valid() noexcept override
        {return true;}

    //! Fabrique d'entité de classe nomée entity.
    std::unique_ptr<entity> makeentity(const QString &ent) const
        {return m_manager->makeentity(entity);}

    //! Acceseur du managers.
    const managerMPS::Managers &managers() const noexcept
        {return *m_manager;}

    //! Mise à jour de la base de donnée.
    void miseAJourBdd();

    //! Renvoie le nombre de type d'entitée (virtuelle).
    entidt nbrentity() const noexcept
        {return m_manager->nbrentity();}

    //! Ouvre la base de donnée.
    bool open() override;

    //! Renvoie l'identifiant de l'entité de type Ent de référence fourni ou 0 si elle n'existe pas.
    template<class Ent> idt refToId(const QString &ref) {
         Ent entity;
         entity.set_ref(ref);
         return existsUnique(entity) ? entity.id() : 0;
    }

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    bool sameInBdd(const entity &ent);

    //! Teste s'il y a dans la base de donnée une entité ayant exactement les mêmes attributs (identifiant compris).
    template<class Ent> bool sameInBdd(const Ent &ent);

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
        {saveConteneur<Ent,map_id<Ent>>(vector);}

    //! Enregistre les entités de vector dans la base de donnée.
    template<class Ent> void save(const map_id<Ent> &vector)
        {saveConteneur<Ent,map_id<Ent>>(vector);}

    //! Enregistre l'arbre d'entités dans la base de donnée.
    template<class Ent> void save(tree<Ent> &arbre, treeSave n = treeSave::ExternalChange);

    //! Enregistre les entités de vector dans la base de donnée.
    void save(vector_ptr<entity> &vector);

    //! Enregistre les entités de vector dans la base de donnée.
    void save(const vector_ptr<entity> &vector);

    //! Enregistre les entités de vector dans la base de donnée.
    template<class Ent> void save(vector_ptr<Ent> &vector)
        {saveConteneur<Ent,vector_ptr<Ent>>(vector);}

    //! Enregistre les entités de vector dans la base de donnée.
    template<class Ent> void save(const vector_ptr<Ent> &vector)
        {saveConteneur<Ent,vector_ptr<Ent>>(vector);}

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    void saveUnique(entity &ent);

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est mise à jour
    //! et l'identifiant de entity devient idU,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    //! Si l'entité est nouvelle en base de donnée l'identifiant de entity est mise-à-jour.
    template<class Ent> void saveUnique(Ent &ent);

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    void saveUnique(const entity &ent);

    //! Enregistre l'entity dans la base de donnée, s'il existe en base de donnée une entité d'identifiant idU
    //! ayant les mêmes attributs unique,
    //! deux cas se présentent, soit entity à un identifiant nul alors l'entité d'identifiant idU est seulement mise à jour,
    //! soit entity à un identifiant idE non nul alors l'entité d'identifiant idU est mise à jour
    //! et l'entité d'identifiant idE est supprimé.
    template<class Ent> void saveUnique(const Ent &ent);

    //! Renvoie le schema xml pour valider les fichiers d'importation de données.
    fichierMPS::doc_xml schemaXmlForImport() const;

    //! Transmet la connexion à la base de donnée aux managers.
    void setBdd();

    //! Modifie le chemin de la base de donnée.
    void set_name(const QString &name) override;

    //! Modifie les restrictions de modification pour une entité donnée.
    void setRestriction(const entity &ent, flag restrict);

    //! Modifie les restrictions de modification pour une entité donnée.
    template<class Ent> void setRestriction(const Ent &ent, flag restrict);

    //! Renvoie le nombre de descendants directs.
    int size_child(const entity &ent)
        {return m_manager->get(entity.id()).size_child(entity);}

    //! Renvoie l'enumeration associé à str pour un groupe de catégorie.
    virtual enumt strCategorieToEnum(const QString &str, flag categorie, QString &controle) const noexcept;

    //! Renvoie l'enumeration associé à str pour une entitée d'identifiant id_entity.
    virtual enumt strIdToEnum(const QString &str, idt id_entity, QString &controle) const noexcept {
        controle.append("-> Enum invalide pour l'entité d'identifiant ").append(QString::number(static_cast<uint>(id_entity)))
                .append(": ").append(str);
        return InvalideEnum;
    }

    //! Renvoie un vecteur des noms des entités correspondant à une table de la base de données
    //! (trié par ordre alphabétique).
    vector_id_name table_entity_names(bool arbre = false) const;

    //! Teste l'autorisation de modification de l'entité donnée en argument.
    bool testAutorisation(const entity &entity, flag autoris)
        {return testAutorisationP(entity.id(),entity.id_entity(),autoris);}

protected:
    //! Renvoie un arbre avec les valeurs contenues dans le XmlDox à l'endroit pointé par iter.
    entities_base::arbre arbreXml(xml_iterator iter, QString &controle);

    //! Cherche un attribut dans un noeud xml puis renvoie sa valeur.
    QString attributXml(xml_iterator iter, const QString &attribut, QString &controle) const {
        auto iter_att = iter->attributes().find(attribut);
        if(iter_att == iter->attributes().cend()) {
            controle.append("Le noeud ne possède pas d'attribut : ").append(attribut);
            return QString();
        }
        else
            return iter_att->second;
    }

    //! Enregistre les données xml associées à une entité dans la base de donnée.
    virtual void associatedXml(entity &ent, xml_iterator iter, QString &controle);

    //! Enregistre les données xml associées à une entité dans la base de donnée
    //! et complète le message d'erreur si besoin.
    void associatedControledXml(entity &ent, xml_iterator iter, QString &controle) {
        associatedXml(entity,iter,controle);
        if(!controle.isEmpty())
            controle.prepend(QString("-> Dans l'entité associée : ").append(iter->name()).append("\n"));
    }


    //! Création de la table de l'entité en base de donnée.
    template<class Ent> void creerTable()
        {m_manager->get<Ent>().creer();}

    //! Supprime les entités d'une liste.
    template<class Ent> bool del(const std::list<idt> &liste) {
        auto i = liste.cbegin();
        while(i != liste.cend() &&delP(*i,Ent::ID))
            ++i;
        return i == liste.cend();
    }

    //! Supprime les entités de la liste vérifiant les conditions donnée en arguments.
    template<class Ent, class... Args> bool delList(Args... args)
        {return del<Ent>(getListId<Ent>(args...));}

    //! Supprime l'entité d'identifiant id de type d'identifiant id_entity de la base de données.
    virtual bool delP(idt id, entidt id_entity)
        {return m_manager->get(id_entity).del(id);}

    //! Hydrate un attribut de l'entité par la valeur contenue dans le XmlDox à l'endroit pointé par iter.
    virtual void hydrateAttributXml(entity &ent, post pos, xml_iterator iter, QString &controle);

    //! Hydrate un attribut de l'entité par la valeur contenue dans le XmlDox à l'endroit pointé par iter
    //! et complète le message d'erreur si besoin.
    void hydrateAttributControledXml(entity &ent, post pos, xml_iterator iter, QString &controle) {
        hydrateAttributXml(entity,pos,iter,controle);
        if(!controle.isEmpty())
            controle.prepend(QString("-> Dans le noeud d'attribut : ")
                             .append(iter->name()).append(" -> ").append(iter->text()).append("\n"));
    }


    //! Hydrate un attribut de l'entité entity_ass associée à entity avec le couple pair<clé,valeur>.
    virtual void hydrateAttributAssociatedXml(entity &entity_ass, const std::pair<const QString,QString> &pair,
                                               const entity &entity, QString &controle);

    //! Hydrate un attribut de l'entité entity_ass associée à entity avec le couple pair<clé,valeur>
    //! et complète le message d'erreur si besoin.
    void hydrateAttributAssociatedControledXml(entity &entity_ass, const std::pair<const QString,QString> &pair,
                                              const entity &entity, QString &controle){
        hydrateAttributAssociatedXml(entity_ass,pair,entity,controle);
        if(!controle.isEmpty())
            controle.prepend(QString("-> Dans l'attribut : ").append(pair.first).append(" = ").append(pair.second).append("\n"));
    }

    //! Hydrate l'entité avec les valeurs contenues dans le XmlDox à l'endroit pointé par iter
    //! renvoie la liste des itérateurs sur les données associées.
    std::list<xml_iterator> hydrateentityXml(entity &entity, xml_iterator iter, QString &controle);

    //! Teste si un attribut d'une entité associée doit être pris en compte après l'enregistrement.
    virtual bool isAfterAssociatedXml(const std::pair<const QString,QString> &pair) const
        {return pair.first == "restriction";}

    //! Teste si un attribut est une entité associée.
    virtual bool isAssociatedXml(xml_iterator iter, entity &entity) const;

    //! Teste si un attribut d'une entité associée est multiple.
    virtual bool isMultipleAssociatedXml(const std::pair<const QString,QString> &/*pair*/) const
        {return false;}

    //! Mise à jour de la base de donnée.
    virtual void listeMiseAJourBdd(int /*version*/, idt /*type*/) {}

    //! Renvoie la liste des attributs associés un attribut multiple.
    virtual xml_list_atts listMultipleAssociatedXml(const std::pair<const QString,QString> &pair, QString &controle);

    //! Renvoie la liste des attributs associés un attribut multiple.
    virtual xml_list_atts listMultipleAssociatedControledXml(const std::pair<const QString,QString> &pair, QString &controle){
        auto list = listMultipleAssociatedXml(pair,controle);
        if(!controle.isEmpty())
            controle.prepend(QString("-> Dans la liste d'attribut multiple : ")
                             .append(pair.first).append(" = ").append(pair.second));
        return list;
    }

    //! Ouverture de la base de donnée.
    bool openBdd();

    //! position d'un attribut dans l'entité.
    post positionXml(const entity &ent, const QString &attribut, QString &controle) const {
        auto pos = entity.position(attribut);
        if(pos == entity.nbr_att())
            controle.append("-> L'entité de type : ").append(m_manager->info(entity.id_entity()).name())
                    .append("\nne possède pas d'attribut : ").append(attribut)
                    .append("\nL'entité : ").append(entity.affiche());
        return pos;
    }

    //! Enregistre les entités de vector dans la base de donnée.
    template<class Ent,class Conteneur> void saveConteneur(const Conteneur &vector);

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    virtual bool testAutorisationP(idt id, entidt id_entity, flag autoris);

    //! Vérifie les autorisations des entités dont l'identifiant est dans liste.
    template<class Ent> bool testAutorisation(const std::list<idt> &liste, flag autoris) {
        auto i = liste.cbegin();
        while(i != liste.cend() &&testAutorisationP(*i,Ent::ID, autoris))
            ++i;
        return i == liste.cend();
    }

    //! //! Vérifie les autorisations des entités de la liste vérifiant les conditions donnée en arguments.
    template<class Ent, class... Args> bool testAutorisationList(flag autoris, Args... args)
        {return testAutorisation<Ent>(getListId<Ent>(args...), autoris);}
};

template<class Ent> bool Bdd::exists(const Ent &ent)
    {return entity.is_new() ? false : m_manager->get<Ent>().exists(entity);}

template<class Ent> bool Bdd::exists(typename Ent::Position cle, const QVariant &value, condition cond)
    {return m_manager->get<Ent>().exists(cle,value,cond);}

template<class Ent> bool Bdd::exists(const std::map<typename Ent::Position, QVariant> &value, std::vector<condition> cond)
    {return m_manager->get<Ent>().exists(value,cond);}

template<class Ent> bool Bdd::existsUnique(Ent &ent)
    {return m_manager->get<Ent>().existsUnique(entity) != Aucun;}

template<class Ent> bool Bdd::existsUnique(const Ent &ent)
    {return m_manager->get<Ent>().existsUnique(entity) != Aucun;}

template<class Ent> existeUni Bdd::existsUniqueEnsemble(Ent &ent)
    {return m_manager->get<Ent>().existsUnique(entity);}

template<class Ent> existeUni Bdd::existsUniqueEnsemble(const Ent &ent)
    {return m_manager->get<Ent>().existsUnique(entity);}

template<class Ent> std::pair<existeUni,idt> Bdd::existsUniqueId(const Ent &ent)
    {return m_manager->get<Ent>().existsUniqueId(entity);}

template<class Ent> QVariant Bdd::fonctionAgrega(agrega fonc, typename Ent::Position att)
    {return m_manager->get<Ent>().fonctionAgrega(fonc, att);}

template<class Ent> QVariant Bdd::fonctionAgrega(agrega fonc, typename Ent::Position att, typename Ent::Position cle,
                                                   const QVariant &value, condition cond)
    {return m_manager->get<Ent>().fonctionAgrega(fonc, att, cle, value, cond);}

template<class Ent> QVariant Bdd::fonctionAgrega(agrega fonc, typename Ent::Position att,
                                                   typename Ent::Position cle1, const QVariant &value1,
                                                   typename Ent::Position cle2, const QVariant &value2,
                                                   condition cond1, condition cond2)
    {return m_manager->get<Ent>().fonctionAgrega(fonc, att, cle1, value1, cle2, value2, cond1, cond2);}

template<class Ent, class Fct> bool Bdd::foreachBeginChild(idt id, const Fct &fct, bool ordre) {
    auto childs = m_manager->get<Ent>().getListChildsIdLeaf(id);
    if(ordre){
        auto i = childs.cbegin();
        while (i != childs.cend() &&(
                   ((*i).second &&fct((*i).first))
                   || foreachBeginChild<Ent>((*i).first,fct,ordre)
                   ))
               ++i;
        return i == childs.cend() &&fct(id);
    }
    else {
        auto i = childs.crbegin();
        while (i != childs.crend() &&(
                   ((*i).second &&fct((*i).first))
                   || foreachBeginChild<Ent>((*i).first,fct,ordre)
                   ))
               ++i;
        return i == childs.crend() &&fct(id);
    }
}

template<class Ent, class Fct> bool Bdd::foreachNode(idt id, const Fct &fct, bool ordre) {
    if(fct(id)) {
        auto childs = m_manager->get<Ent>().getListChildsIdLeaf(id);
        if(ordre) {
            auto i = childs.cbegin();
            while (i != childs.cend()  &&(
                       ((*i).second &&fct((*i).first))
                       ||foreachNode<Ent>((*i).first,fct,ordre)
                       ))
                   ++i;
            return i == childs.cend();
        }
        else {
            auto i = childs.crbegin();
            while (i != childs.crend()  &&(
                       ((*i).second &&fct((*i).first))
                       ||foreachNode<Ent>((*i).first,fct,ordre)
                       ))
                   ++i;
            return i == childs.crend();
        }
    }
    return false;
}

template<class Ent> bool Bdd::get(Ent &ent)
    {return m_manager->get<Ent>().get(entity);}

template<class Ent> tree<Ent> Bdd::getarbre()
    {return m_manager->get<Ent>().getarbre();}

template<class Ent> tree<Ent> Bdd::getarbre(idt id)
    {return m_manager->get<Ent>().getarbre(id);}

template<class Ent> tree<Ent> Bdd::getarbre(const Ent &ent)
    {return m_manager->get<Ent>().getarbre(entity.id());}

template<class Ent> vector_ptr<Ent> Bdd::getList(typename Ent::Position ordre, bool croissant)
    {return m_manager->get<Ent>().getList(ordre, croissant);}

template<class Ent> vector_ptr<Ent> Bdd::getList(typename Ent::Position cle, int value, typename Ent::Position ordre,
                                              condition cond, bool crois)
    {return m_manager->get<Ent>().getList(cle, QVariant(value), ordre, cond, crois);}

template<class Ent> vector_ptr<Ent> Bdd::getList(typename Ent::Position cle, idt value, typename Ent::Position ordre,
                                              condition cond, bool crois)
    {return m_manager->get<Ent>().getList(cle, QVariant(value), ordre, cond, crois);}

template<class Ent> vector_ptr<Ent> Bdd::getList(typename Ent::Position cle, const QVariant &value,
                                              typename Ent::Position ordre, condition cond, bool crois)
    {return m_manager->get<Ent>().getList(cle, value, ordre, cond, crois);}

template<class Ent> vector_ptr<Ent> Bdd::getList(typename Ent::Position cle, const QVariant &value,
                                              typename Ent::Position order1, typename Ent::Position order2,
                                              condition cond, bool croissant1, bool croissant2)
    {return m_manager->get<Ent>().getList(cle, value, order1, order2, cond, croissant1, croissant2);}

template<class Ent> vector_ptr<Ent> Bdd::getList(typename Ent::Position cle, const QVariant &value,
                                              typename Ent::Position order1, typename Ent::Position order2,
                                              typename Ent::Position ordre3, condition cond,
                                              bool croissant1, bool croissant2, bool croissant3)
    {return m_manager->get<Ent>().getList(cle, value, order1, order2, ordre3, cond, croissant1, croissant2, croissant3);}

template<class Ent> vector_ptr<Ent> Bdd::getList(typename Ent::Position cle1, const QVariant &value1,
                                              typename Ent::Position cle2, const QVariant &value2,
                                              typename Ent::Position order, condition cond1, condition cond2, bool croissant)
    {return m_manager->get<Ent>().getList(cle1, value1, cle2, value2, order, cond1, cond2, croissant);}

template<class Ent> vector_ptr<Ent> Bdd::getList(typename Ent::Position cle1, const QVariant &value1,
                                              typename Ent::Position cle2, const QVariant &value2,
                                              typename Ent::Position cle3, const QVariant &value3,
                                              typename Ent::Position order,
                                              condition cond1, condition cond2, condition cond3, bool croissant)
    {return m_manager->get<Ent>().getList(cle1, value1, cle2, value2, cle3, value3, order, cond1, cond2, cond3, croissant);}

template<class Ent> vector_ptr<Ent> Bdd::getList(const std::vector<typename Ent::Position> &cle,
                                                const std::vector<QVariant> &value,
                                                const std::vector<typename Ent::Position> &ordre,
                                                const std::vector<condition> &condition, const std::vector<bool> &crois)
    {return m_manager->get<Ent>().getList(cle,value,ordre,condition,crois);}

template<class Ent, class Join> vector_ptr<Ent> Bdd::getList(typename Ent::Position colonneTable,
                                                            typename Join::Position colonneJoin,
                                                            const std::map<typename Ent::Position, QVariant> &whereMapTable,
                                                            const std::map<typename Join::Position, QVariant> &whereMapJoin,
                                                            const std::vector<std::pair<typename Ent::Position, bool>> &orderMapTable) {
    std::map<QString,QVariant> whereMapJoinString;
    for(auto i = whereMapJoin.cbegin(); i != whereMapJoin.cend(); i++)
        whereMapJoinString[m_manager->get<Join>().info().attribut(i->first)] = i->second;
    return m_manager->get<Ent>().getListJoin(m_manager->get<Join>().info().table(),
                                             colonneTable,
                                             m_manager->get<Join>().info().attribut(colonneJoin),
                                             whereMapTable, whereMapJoinString, orderMapTable);
}

template<class Ent, class Join> vector_ptr<Ent> Bdd::getList(typename Join::Position cleJoin, typename Join::Position cleWhere,
                                                            const QVariant &valueWhere, typename Ent::Position ordre,
                                                            condition cond, bool crois) {
    return m_manager->get<Ent>().getListJoin(m_manager->get<Join>().info().table(),
                                             m_manager->get<Join>().info().attribut(cleJoin),
                                             m_manager->get<Join>().info().attribut(cleWhere),
                                             valueWhere, ordre, cond, crois);
}

template<class Ent, class Join> vector_ptr<Ent> Bdd::getList(typename Join::Position cleJoin,
                                                       typename Join::Position cleWhere1, const QVariant &valueWhere1,
                                                       typename Join::Position cleWhere2, const QVariant &valueWhere2,
                                                       typename Ent::Position ordre, bool crois){
    std::map<typename Join::Position, QVariant> mapWhere;
    mapWhere[cleWhere1] = valueWhere1;
    mapWhere[cleWhere2] = valueWhere2;
    return getList<Ent,Join>(Ent::Id,cleJoin,std::map<typename Ent::Position, QVariant>(),mapWhere,
                             std::vector<std::pair<typename Ent::Position, bool>>
                             ({std::pair<typename Ent::Position, bool>(ordre,crois)}));
}

template<class Ent , class Join1, class Join2> vector_ptr<Ent> Bdd::getList(typename Join1::Position cle1,
                                                          typename Join1::Position cle2,
                                                          typename Join2::Position cleWhere,
                                                          const QVariant &valueWhere,
                                                          typename Ent::Position ordre,
                                                          condition cond, bool crois) {
    return m_manager->get<Ent>().getListJoin(m_manager->get<Join1>().info().table(),
                                             m_manager->get<Join2>().info().table(),
                                             m_manager->get<Join1>().info().attribut(cle1),
                                             m_manager->get<Join1>().info().attribut(cle2),
                                             m_manager->get<Join2>().info().attribut(cleWhere),
                                             valueWhere, ordre, cond, crois);
}

template<class Ent> vector_ptr<Ent> Bdd::getListChilds(const Ent &ent)
    {return m_manager->get<Ent>().getListChilds(entity);}

template<class Ent> std::list<idt> Bdd::getListChildsId(idt id)
    {return m_manager->get<Ent>().getListChildsId(id);}

template<class Ent> std::vector<std::pair<idt,bool>> Bdd::getListChildsIdLeaf(int id)
    {return m_manager->get<Ent>().getListChildsIdLeaf(id);}

template<class Ent> std::list<idt> Bdd::getListId(typename Ent::Position cle, int value, typename Ent::Position ordre,
                                              condition cond, bool crois)
    {return m_manager->get<Ent>().getListId(cle,value,ordre,cond,crois);}

template<class Ent> std::list<idt> Bdd::getListId(typename Ent::Position cle, idt value, typename Ent::Position ordre,
                                 condition cond, bool crois)
    {return m_manager->get<Ent>().getListId(cle,value,ordre,cond,crois);}

template<class Ent> std::list<idt> Bdd::getListId(typename Ent::Position cle, const QVariant &value,
                                 typename Ent::Position ordre, condition cond, bool crois)
    {return m_manager->get<Ent>().getListId(cle,value,ordre,cond,crois);}

template<class Ent> std::list<idt> Bdd::getListId(typename Ent::Position cle, const QVariant &value,
                                 typename Ent::Position ordre1, typename Ent::Position ordre2,
                                 condition cond, bool croissant1, bool croissant2)
    {return m_manager->get<Ent>().getListId(cle,value,ordre1,ordre2,cond,croissant1,croissant2);}

template<class Ent> std::list<idt> Bdd::getListId(typename Ent::Position cle, const QVariant &value,
                                 typename Ent::Position ordre1, typename Ent::Position ordre2, typename Ent::Position ordre3,
                                 condition cond, bool crois1, bool crois2, bool crois3)
    {return m_manager->get<Ent>().getListId(cle,value,ordre1,ordre2,ordre3,cond,crois1,crois2,crois3);}

template<class Ent> std::list<idt> Bdd::getListId(typename Ent::Position cle1, const QVariant &value1,
                                 typename Ent::Position cle2, const QVariant &value2,
                                 typename Ent::Position ordre, condition cond1, condition cond2, bool crois)
    {return m_manager->get<Ent>().getListId(cle1,value1,cle2,value2,ordre,cond1,cond2,crois);}

template<class Ent> std::list<idt> Bdd::getListId(typename Ent::Position cle1, const QVariant &value1,
                                 typename Ent::Position cle2, const QVariant &value2,
                                 typename Ent::Position cle3, const QVariant &value3,
                                 typename Ent::Position ordre,
                                 condition cond1, condition cond2,condition cond3,
                                 bool crois)
    {return m_manager->get<Ent>().getListId(cle1,value1,cle2,value2,cle3,value3,ordre,cond1,cond2,cond3,crois);}

template<class Ent> std::list<idt> Bdd::getListId(const std::vector<typename Ent::Position> &cle,
                         const std::vector<QVariant> &value,
                         const std::vector<typename Ent::Position> &ordre,
                         const std::vector<condition> &condition,
                         const std::vector<bool> &crois)
    {return m_manager->get<Ent>().getListId(cle, value,ordre, condition, crois);}

template<class Ent> map_id<Ent> Bdd::getMap(typename Ent::Position cleMap)
    {return m_manager->get<Ent>().getMap(cleMap);}

template<class Ent> map_id<Ent> Bdd::getMap(typename Ent::Position cle, const QVariant &value,
                                            typename Ent::Position cleMap, condition cond)
    {return m_manager->get<Ent>().getMap(cle, value, cleMap, cond);}

template<class Ent> map_id<Ent> Bdd::getMap(typename Ent::Position cle1, const QVariant &value1,
                                            typename Ent::Position cle2, const QVariant &value2,
                                            typename Ent::Position cleMap, condition cond1, condition cond2)
    { return m_manager->get<Ent>().getMap(cle1, value1, cle2, value2, cleMap, cond1, cond2);}

template<class Ent> map_id<Ent> Bdd::getMap(typename Ent::Position cle1, const QVariant &value1,
                                            typename Ent::Position cle2, const QVariant &value2,
                                            typename Ent::Position cle3, const QVariant &value3,
                                            typename Ent::Position cleMap,
                                            condition cond1, condition cond2, condition cond3)
    {return m_manager->get<Ent>().getMap(cle1, value1, cle2, value2, cle3, value3, cleMap, cond1, cond2, cond3);}

template<class Ent> map_id<Ent> Bdd::getMap(const std::vector<typename Ent::Position> &cle,
                                            const std::vector<QVariant> &value,
                                            typename Ent::Position cleMap,
                                            const std::vector<typename Ent::Position> &ordre,
                                            const std::vector<condition> &cond,
                                            const std::vector<bool> &crois)
    {return m_manager->get<Ent>().getMap(cle,value,cleMap,ordre,cond,crois);}

template<class Ent, class Join> map_id<Ent> Bdd::getMap(typename Ent::Position colonneTable,
                                                        typename Join::Position colonneJoin,
                                                        const std::map<typename Ent::Position,QVariant> &whereMapTable,
                                                        const std::map<typename Join::Position,QVariant> &whereMapJoin,
                                                        typename Ent::Position cleMap) {
    std::map<QString,QVariant> whereMapJoinString;
    for(auto i = whereMapJoin.cbegin(); i != whereMapJoin.cend(); i++)
        whereMapJoinString[m_manager->get<Join>().attribut(i->first)] = i->second;
    return m_manager->get<Ent>().getMapJoin(m_manager->get<Join>().table(),colonneTable,
                                            m_manager->get<Join>().attribut(colonneJoin),
                                            whereMapTable, whereMapJoinString, cleMap);
}

template<class Ent, class Join> map_id<Ent> Bdd::getMap(typename Join::Position cleJoin,
                                                                typename Join::Position cleWhere, const QVariant &valueWhere,
                                                                typename Ent::Position cleMap, condition cond) {
    return m_manager->get<Ent>().getMapJoin(m_manager->get<Join>().table(),m_manager->get<Join>().attribut(cleJoin),
                                             m_manager->get<Join>().attribut(cleWhere),
                                                      valueWhere, cleMap, cond);
}

template<class Ent> flag Bdd::getRestriction(const Ent &ent)
    {return m_manager->get<Ent>().getRestriction(entity.id());}

template<class Ent> bool Bdd::getUnique(Ent &ent)
    {return m_manager->get<Ent>().getUnique(entity);}

template<class Ent> void Bdd::insert(Ent &ent, idt idParent, int num)
    {m_manager->get<Ent>().insert(entity,idParent,num);}

template<class Ent> void Bdd::insert(const Ent &ent, idt idParent, int num)
    {m_manager->get<Ent>().insert(entity,idParent,num);}

template<class Ent> bool Bdd::sameInBdd(const Ent &ent)
    {return m_manager->get<Ent>().sameInBdd(entity);}

template<class Ent> void Bdd::save(Ent &ent)
    {m_manager->get<Ent>().save(entity);}

template<class Ent> void Bdd::save(const Ent &ent)
    {m_manager->get<Ent>().save(entity);}

template<class Ent> void Bdd::save(Ent &ent, flag restrict)
    {m_manager->get<Ent>().save(entity, restrict);}

template<class Ent> void Bdd::save(const Ent &ent, flag restrict)
    {m_manager->get<Ent>().save(entity, restrict);}

template<class Ent> void Bdd::saveUnique(Ent &ent)
    {m_manager->get<Ent>().saveUnique(entity);}

template<class Ent> void Bdd::saveUnique(const Ent &ent)
    {m_manager->get<Ent>().saveUnique(entity);}

template<class Ent, class Conteneur> void Bdd::saveConteneur(const Conteneur &vector) {
    for(typename Conteneur::iterator i = vector.begin(); i != vector.end(); ++i)
       m_manager->get<Ent>().save(*i);
}

template<class Ent> void Bdd::save(tree<Ent> &arbre, treeSave n)
    {m_manager->get<Ent>().save(arbre,n);}

template<class Ent> void Bdd::setRestriction(const Ent &ent, flag restrict)
    {m_manager->get<Ent>().setRestriction(entity.id(), restrict);}
}
#endif // BDD_H
