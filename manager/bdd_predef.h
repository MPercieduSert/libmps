/*Auteur: PERCIE DU SERT Maxime
 *Date: 19/02/2016
 */
#ifndef BDD_PREDEF_H
#define BDD_PREDEF_H

#include <QFont>
#include "bdd.h"
#include "managers_predef.h"
#include "entity_predef.h"

namespace mps {
namespace b2d {
    namespace bdd_version {
        enum creation_bdd_predef {Creation_Predef = Nbr_Version,
                        Nbr_Version_Predef};}

    namespace code {
        enum permission : flag::flag_type {Interdit = 0,
                              Visible = 0x1,
                              Attribuable = 0x2};
    }

/*! \ingroup groupe_file
 *\brief Gestionnaire de la base de donnée pour les entités prédéfinie.
 *
 *Gestionnaire de la base de donnée pour les entités prédéfinie:
 *     - Commentaire,
 *     - commentaire_cible,
 *     - donnee,
 *     - donnee_card,
 *     - donnee_cible,
 *     - Evenement,
 *     - evenement_cible,
 *     - evenement_style,
 *     - Historique,
 *     - mot_cle,
 *     - mot_cle_cible,
 *     - mot_cle_permission,
 *     - Restriction,
 *     - Source,
 *     - source_texte,
 *     - Texte,
 *     - texte_cible,
 *     - Type,
 *     - type_permission,
 *     - Utilisation
 */
class bdd_predef : public bdd {
public:
    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    bdd_predef(const QString &bdd_type, const std::vector<int> &version, std::unique_ptr<manager::managers_predef> &&manager)
        : bdd(bdd_type,version,std::move(manager)) {}

    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    bdd_predef(const QString &bdd_type, const QString &name, const std::vector<int> &version,
              std::unique_ptr<manager::managers_predef> &&manager)
        : bdd(bdd_type,name,version,std::move(manager)) {}

    //! Destructeur. Referme la base de donnée.
    ~bdd_predef() override = default;

    //! Retourne le numéro de cible associé à l'identifiant d'une entité.
    int cible(entidt id) const
        {return managers().cible(id);}

    //! Retourne le numéro de cible associé à un type d'entité.
    template<class Ent> int cible() const
        {return managers().cible<Ent>();}

    //! Suppresseur d'une entité enregistrée comme donnée.
    void del_entity_in_donnee(idt id_cible, int cible, int num = 0);

    //! Accesseur d'une entité enregistrée comme donnée.
    template<class Ent> Ent get_entity_in_donnee(idt id_cible, int cible, int num = 0);

    //! Acceseur du manageur.
    const manager::managers_predef &managers() const
        {return static_cast<const manager::managers_predef &>(*m_manager);}

    //! Acceseur du manageur.
    manager::managers_predef &managers()
        {return static_cast<manager::managers_predef &>(*m_manager);}

    //! Renvoie le nombre des entités liées à la cible.
    virtual int nbr_entity_cible(int /*cible*/) const
        {return 0;}//à finir

    //! Renvoie le nom des entités liées à la cible.
    virtual std::map<int,QString> noms_entity_cible(int /*cible*/) const
        {return std::map<int,QString>();}//à finir

    //! Mutateur d'une entité enregistrée comme donnée.
    template<class Ent> void set_entity_in_donnee(const Ent &ent, idt id_cible, int cible, int num = 0);

    //! Renvoie l'enumeration associé à str pour un groupe de catégorie.
    enumt str_categorie_to_enum(const QString &str, flag categorie, QString &controle) const noexcept override;

    //! Renvoie l'enumeration associé à str pour une entitée de type id_ent.
    enumt str_id_to_enum(const QString &str, idt id_entity, QString &controle) const noexcept override;

protected:
    //! Supprime l'entité d'identifiant id de type d'identifiant id_entity de la base de données.
    bool del_v(idt id, entidt id_entity) override;

    //! Hydrate un attribut de l'entité par la valeur contenue dans le XmlDox à l'endroit pointé par iter.
    void hydrate_attribut_xml(entities::entity &ent, post pos, xml_iterator iter, QString &controle) override;

    //! Hydrate un attribut de l'entité entity_ass associée à entity avec le couple pair<clé,valeur>.
    void hydrate_attribut_associated_xml(entity &ent_ass, const std::pair<const QString,QString> &pair,
                                      const entity &ent, QString &controle) override;

    //! Renvoie le numero de début d'enregistrement d'une entité.
    std::pair<int, int> interval_entity_in_donnee(idt id_cible, int cible, int num);

    //! Teste si un attribut d'une entité associée est multiple.
    bool is_multiple_associated_xml(const std::pair<const QString,QString> &pair) const override;

    //! Mise à jour de la base de donnée.
    void liste_mise_a_jour_bdd(int version, idt type) override;

    //! Renvoie la liste des attributs associés un attribut multiple.
    xml_list_atts list_multiple_associated_xml(const std::pair<const QString,QString> &pair, QString &controle) override;

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    bool test_autorisation_v(idt id, entidt id_entity, flag autoris) override;
};

template<class Ent> Ent bdd_predef::get_entity_in_donnee(idt id_cible, int cb, int num)
{
    using namespace entities;
    donnee_cible nature(donnee::Entity_Nature_IdND,id_cible,cb,num);
    if(!get(nature) || nature.valeur().toInt() == nature_id::Vide || nature.valeur().toInt() != cible<Ent>())
        return Ent();

    std::pair<int,int> interval(interval_entity_in_donnee(id_cible,cb,num));
    auto liste = get_list<donnee_cible>(std::vector<donnee_cible::position>({
                                                     donnee_cible::Num,
                                                     donnee_cible::Num,
                                                     donnee_cible::Id_Donnee,
                                                     donnee_cible::Id_Cible,
                                                     donnee_cible::Cible}),
                                                     std::vector<QVariant>({interval.first,
                                                     interval.second,
                                                     donnee::Entity_IdND,
                                                     id_cible,
                                                     cb}),
                                                     std::vector<donnee_cible::position>({donnee_cible::Num}),
                                                     std::vector<condition>({Sup_Egal, Inf}));
    Ent ent;
    auto j = liste.begin();
    for(post i = 0; i != Ent::Nbr_Att; ++i, ++j)
        ent.set_data(i,j->valeur());
    return ent;
}

template<class Ent> void bdd_predef::set_entity_in_donnee(const Ent &ent, idt id_cible, int cb, int num) {
    using namespace entities;

    if(num < 0)
        throw std::invalid_argument("L'argument num doit être positif");

    donnee_cible nature(donnee::Entity_Nature_IdND, id_cible, cb, num);
    if(get(nature)) {
        if(nature.valeur().toInt() != cible<Ent>()) {
            std::pair<int, int> interval_old(interval_entity_in_donnee(id_cible,cb,num));
            auto  offset = Ent::Nbr_Att - (interval_old.second - interval_old.first);
            if(offset != 0) {
                auto liste  = get_list<donnee_cible>(std::vector<donnee_cible::position>({donnee_cible::Num,
                                                                 donnee_cible::Id_Donnee,
                                                                 donnee_cible::Id_Cible,
                                                                 donnee_cible::Cible}),
                                                                 std::vector<QVariant>({interval_old.second,
                                                                 donnee::Entity_IdND,
                                                                 id_cible,
                                                                 cb}),
                                                                 std::vector<donnee_cible::position>({donnee_cible::Num}),
                                                                 std::vector<condition>({Sup_Egal}),
                                                                 std::vector<bool>({offset < 0}));
                for(auto i = liste.begin(); i != liste.end(); ++i) {
                    i->set_num(i->num() + offset);
                    save(*i);
                }
            }
            nature.set_valeur(cible<Ent>());
            save(nature);
        }
    }
    else {
        if(num != 0) {
            donnee_cible nature_num_prec(donnee::Entity_Nature_IdND, id_cible, cb, num - 1);
            nature_num_prec.set_valeur(nature_id::Vide);
            while(nature_num_prec.num() >= 0 &&!exists_unique(nature_num_prec)) {
                save(nature_num_prec);
                nature_num_prec.set_num(nature_num_prec.num() - 1);
            }
        }
        nature.set_valeur(cible<Ent>());
        save(nature);
    }

    std::pair<int, int> interval(interval_entity_in_donnee(id_cible,cb,num));
    donnee_cible c_dn;
    c_dn.set_id_1(donnee::Entity_IdND);
    c_dn.set_id_cible(id_cible);
    c_dn.set_cible(cb);
    for(int i = 0; i != Ent::Nbr_Att; ++i) {
        c_dn.set_id(0);
        c_dn.set_num(interval.first + i);
        c_dn.set_valeur(ent.data(i));
        save_unique(c_dn);
    }
}
}}
#endif // BDD_PREDEF_H
