/*Auteur: PERCIE DU SERT Maxime
 *Date: 19/02/2016
 */
#ifndef BDDPREDEF_H
#define BDDPREDEF_H

#include <QFont>
#include "Bdd.h"
#include "ManagersPredef.h"
#include "entityPredef.h"

namespace bddMPS {
    namespace bddVersion {
        enum creationBddPredef {CreationPredef = NbrVersion,
                        NbrVersionPredef};}

    namespace code {
        enum permission : flag::flag_type {Interdit = 0,
                              Visible = 0x1,
                              Attribuable = 0x2};
    }

/*! \ingroup groupeFile
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
class BddPredef : public Bdd {
public:
    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    BddPredef(const QString &dbtype, const std::vector<int> &version, std::unique_ptr<managerMPS::ManagersPredef> &&manager)
        : Bdd(dbtype,version,std::move(manager)) {}

    //! Constructeur. Donner en argument le type ainsi que le chemin de la base de donnée.
    BddPredef(const QString &dbtype, const QString &name, const std::vector<int> &version,
              std::unique_ptr<managerMPS::ManagersPredef> &&manager)
        : Bdd(dbtype,name,version,std::move(manager)) {}

    //! Destructeur. Referme la base de donnée.
    ~BddPredef() override = default;

    //! Retourne le numéro de cible associé à l'identifiant d'une entité.
    int cible(entidt id) const
        {return managers().cible(id);}

    //! Retourne le numéro de cible associé à un type d'entité.
    template<class Ent> int cible() const
        {return managers().cible<Ent>();}

    //! Suppresseur d'une entité enregistrée comme donnée.
    void delentityIndonnee(idt id_cible, int cible, int num = 0);

    //! Accesseur d'une entité enregistrée comme donnée.
    template<class Ent> Ent getentityIndonnee(idt id_cible, int cible, int num = 0);

    //! Acceseur du manageur.
    const managerMPS::ManagersPredef &managers() const
        {return static_cast<const managerMPS::ManagersPredef &>(*m_manager);}

    //! Acceseur du manageur.
    managerMPS::ManagersPredef &managers()
        {return static_cast<managerMPS::ManagersPredef &>(*m_manager);}

    //! Renvoie le nombre des entités liées à la cible.
    virtual int nbrentityCible(int /*cible*/) const
        {return 0;}//à finir

    //! Renvoie le nom des entités liées à la cible.
    virtual std::map<int,QString> nomsentityCible(int /*cible*/) const
        {return std::map<int,QString>();}//à finir

    //! Mutateur d'une entité enregistrée comme donnée.
    template<class Ent> void setentityIndonnee(const Ent &ent, idt id_cible, int cible, int num = 0);

    //! Renvoie l'enumeration associé à str pour un groupe de catégorie.
    enumt strCategorieToEnum(const QString &str, flag categorie, QString &controle) const noexcept override;

    //! Renvoie l'enumeration associé à str pour une entitée de type id_entity.
    enumt strIdToEnum(const QString &str, idt id_entity, QString &controle) const noexcept override;

protected:
    //! Supprime l'entité d'identifiant id de type d'identifiant id_entity de la base de données.
    bool delP(idt id, entidt id_entity) override;

    //! Hydrate un attribut de l'entité par la valeur contenue dans le XmlDox à l'endroit pointé par iter.
    void hydrateAttributXml(entities::entity &ent, post pos, xml_iterator iter, QString &controle) override;

    //! Hydrate un attribut de l'entité entity_ass associée à entity avec le couple pair<clé,valeur>.
    void hydrateAttributAssociatedXml(entity &ent_ass, const std::pair<const QString,QString> &pair,
                                      const entity &ent, QString &controle) override;

    //! Renvoie le numero de début d'enregistrement d'une entité.
    std::pair<int, int> intervalentityIndonnee(idt id_cible, int cible, int num);

    //! Teste si un attribut d'une entité associée est multiple.
    bool isMultipleAssociatedXml(const std::pair<const QString,QString> &pair) const override;

    //! Mise à jour de la base de donnée.
    void listeMiseAJourBdd(int version, idt type) override;

    //! Renvoie la liste des attributs associés un attribut multiple.
    xml_list_atts listMultipleAssociatedXml(const std::pair<const QString,QString> &pair, QString &controle) override;

    //! Renvoie l'autorisation de modification de l'entité donnée en argument.
    bool testAutorisationP(idt id, entidt id_entity, flag autoris) override;
};

template<class Ent> Ent BddPredef::getentityIndonnee(idt id_cible, int Cible, int num)
{
    using namespace entities;
    donnee_cible nature(donnee::Entity_Nature_IdND,id_cible,Cible,num);
    if(!get(nature) || nature.valeur().toInt() == natureId::Vide || nature.valeur().toInt() != cible<Ent>())
        return Ent();

    std::pair<int,int> interval(intervalentityIndonnee(id_cible,Cible,num));
    auto liste = getList<donnee_cible>(std::vector<donnee_cible::Position>({donnee_cible::Num,
                                                     donnee_cible::Num,
                                                     donnee_cible::Iddonnee,
                                                     donnee_cible::Id_Cible,
                                                     donnee_cible::Cible}),
                                                     std::vector<QVariant>({interval.first,
                                                     interval.second,
                                                     donnee::Entity_IdND,
                                                     id_cible,
                                                     Cible}),
                                                     std::vector<donnee_cible::Position>({donnee_cible::Num}),
                                                     std::vector<condition>({SupEgal, Inf}));
    Ent entity;
    auto j = liste.begin();
    for(post i = 0; i != Ent::Nbr_Att; ++i, ++j)
        entity.set_data(i,j->valeur());
    return entity;
}

template<class Ent> void BddPredef::setentityIndonnee(const Ent &ent, idt id_cible, int Cible, int num) {
    using namespace entities;

    if(num < 0)
        throw std::invalid_argument("L'argument num doit être positif");

    donnee_cible nature(donnee::Entity_Nature_IdND, id_cible, Cible, num);
    if(get(nature)) {
        if(nature.valeur().toInt() != cible<Ent>()) {
            std::pair<int, int> intervalOld(intervalentityIndonnee(id_cible,Cible,num));
            auto  offset = Ent::Nbr_Att - (intervalOld.second - intervalOld.first);
            if(offset != 0) {
                auto liste  = getList<donnee_cible>(std::vector<donnee_cible::Position>({donnee_cible::Num,
                                                                 donnee_cible::Iddonnee,
                                                                 donnee_cible::Id_Cible,
                                                                 donnee_cible::Cible}),
                                                                 std::vector<QVariant>({intervalOld.second,
                                                                 donnee::Entity_IdND,
                                                                 id_cible,
                                                                 Cible}),
                                                                 std::vector<donnee_cible::Position>({donnee_cible::Num}),
                                                                 std::vector<condition>({SupEgal}),
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
            donnee_cible natureNumPrec(donnee::Entity_Nature_IdND, id_cible, Cible, num - 1);
            natureNumPrec.set_valeur(natureId::Vide);
            while(natureNumPrec.num() >= 0 &&!existsUnique(natureNumPrec)) {
                save(natureNumPrec);
                natureNumPrec.set_num(natureNumPrec.num() - 1);
            }
        }
        nature.set_valeur(cible<Ent>());
        save(nature);
    }

    std::pair<int, int> interval(intervalentityIndonnee(id_cible,Cible,num));
    donnee_cible cd;
    cd.set_id_1(donnee::Entity_IdND);
    cd.set_id_cible(id_cible);
    cd.set_cible(Cible);
    for(int i = 0; i != Ent::Nbr_Att; ++i) {
        cd.set_id(0);
        cd.set_num(interval.first + i);
        cd.set_valeur(entity.data(i));
        saveUnique(cd);
    }
}
}
#endif // BDDPREDEF_H
