/*Auteur: PERCIE DU SERT Maxime
 *Date: 16/08/2019
 */
#ifndef MANAGERSPREDEF_H
#define MANAGERSPREDEF_H

#include "ManagerarbreModifControle.h"
#include "Managerarbre_simpleModifControle.h"
#include "ManagerPermissionModifControle.h"
#include "Managers.h"
#include "entityPredef.h"

namespace bddMPS {
    //! Ensembles prédéfinis pris en charge.
    enum ensembleFlag : flag::flag_type {
        EnsembleDisable = 0x0,
        CommentaireEnable = 0x1,
        donneeEnable = 0x2,
        EvenementEnable = 0x4,
        HistoriqueEnable = 0x8,
        mot_cleEnable = 0x10,
        RestrictionEnable = 0x20,
        TexteEnable = 0x40,
        TypeEnable = 0x80,
        UtilisationEnable = 0x100
    };

    //! Associe un numéro de cible.
    namespace cibleId  {
        //! Numéro de cible des types de base.
        enum cible_neg{Configuration = -1};

        //! Numéro de cible des entités prédéfinies.
        enum entityPredef{Commentaire,
                          commentaire_cible,
                          donnee,
                          donnee_card,
                          donnee_cible,
                          Evenement,
                          evenement_cible,
                          evenement_style,
                          Historique,
                          mot_cle,
                          mot_cle_cible,
                          mot_cle_permission,
                          mot_prog_cible,
                          mot_prog_permission,
                          Restriction,
                          Source,
                          Texte,
                          texte_cible,
                          source_texte,
                          Type,
                          type_permission,
                          Utilisation,
                          Usage,
                          NbrCibleEntPredef};
    }

    namespace natureId {
        //! Idantifiant de nature des types de base.
        enum typeBase{Vide = -1,
              Int = -2,
              String = -3,
              Bool = -4,
              Float = -5,
              Double = -6,
              Date = -7,
              Date_Time = -8,
              Variant = -9};

        //! Idantifiant de nature des entités prédéfinies.
        enum entityPredef{Commentaire,
                          commentaire_cible,
                          donnee,
                          donnee_card,
                          donnee_cible,
                          Evenement,
                          evenement_cible,
                          evenement_style,
                          Historique,
                          mot_cle,
                          mot_cle_cible,
                          mot_cle_permission,
                          mot_prog_cible,
                          mot_prog_permission,
                          Restriction,
                          Source,
                          Texte,
                          texte_cible,
                          source_texte,
                          Type,
                          type_permission,
                          Utilisation,
                          Usage,
                          NbrCibleEntPredef};
    }
}

namespace managerMPS {
namespace emps = entities;
/*! \ingroup groupeFile
 * \brief Classe mère des managers utilisant des entités prédéfinies.
 */
class ManagersPredef : public Managers {
protected:
    flag m_ensembleEnable = bmps::EnsembleDisable;      //!< Prise en charge d'ensembles.
    std::vector<int> m_cibleArray;                           //!< Tableau de correspondance ID -> Cible.
    std::vector<int> m_cibleNbr_AttArray;                     //!< Tableau de correspondance cible -> nombre d'attributs.

public:
    //! Constructeur.
    ManagersPredef(entidt Nbrentity, entidt nbrCible, const QString &versionTable,
                     std::map<entidt,std::unique_ptr<AbstractManager>> &&managers = std::map<entidt,std::unique_ptr<AbstractManager>>(),
                     const QSqlQuery &req = QSqlQuery())
        : Managers (Nbrentity, versionTable, std::move(managers), req),
          m_cibleArray(nbrentity(),bmps::natureId::Vide),
          m_cibleNbr_AttArray(nbrCible,0) {}

    //! Destructeur.
    ~ManagersPredef() = default;

    template<class Ent> int cible() const
        {return m_cibleArray[Ent::ID];}

    int cible(entidt id) const
        {return id < m_nbrentity ? m_cibleArray[id] : bmps::natureId::Vide;}

    //! Renvoie le nombre d'attributs d'une entité à partir du numéro de cible.
    int nombreAttributCible(int cible) const {
        if(cible == bmps::natureId::Vide || cible >= static_cast<int>(m_cibleNbr_AttArray.size()))
            return 0;
        if(cible < bmps::natureId::Vide)
            return 1;
        return m_cibleNbr_AttArray[static_cast<unsigned>(cible)];
    }

    //! Teste la prise en charge d'un ensemble donnée
    bool ensembleEnable(bmps::ensembleFlag flag) const noexcept
        {return m_ensembleEnable.test(flag);}

protected:
    //! Mise en place de la prise en charge des commentaires.
    void enableCommentaire(const QString &commentaire, const QString &cibleCommentaire);
    
    //! Mise en place de la prise en charge des donnees.
    void enabledonnee(const QString &donnee, const QString &arbredonnee, const QString &cibledonnee, const QString &carddonnee,
                      std::unique_ptr<AbstractGestionRestriction<emps::donnee>> && gestiondonnee
                        = std::make_unique<AbstractGestionRestriction<emps::donnee>>(),
                      std::unique_ptr<AbstractGestionRestriction<emps::donnee_cible>> && gestionCilbedonnee
                        = std::make_unique<AbstractGestionRestriction<emps::donnee_cible>>(),
                      std::unique_ptr<AbstractGestionRestriction<emps::donnee_card>> && gestionCarddonnee
                        = std::make_unique<AbstractGestionRestriction<emps::donnee_card>>());

    //! Mise en place de la prise en charge des evenements.
    void enableEvenement(const QString &evenement, const QString &evenementCible, const QString &evenementStyle);

    //! Mise en place de la prise en charge des historiques.
    void enableHistorique(const QString &historique);

    //! Mise en place de la prise en charge des mots Clé.
    void enablemot_cle(const QString &motCle, const QString &motClearbre, const QString &ciblemot_cle,
                      const QString &cibleProg, const QString &permissionmot_cle, const QString &permissionProg,
                      std::unique_ptr<AbstractGestionRestriction<emps::mot_cle>> && gestionmot_cle
                        = std::make_unique<AbstractGestionRestriction<emps::mot_cle>>(),
                      std::unique_ptr<AbstractGestionRestriction<emps::mot_cle_permission>> && gestionPermission
                        = std::make_unique<AbstractGestionRestriction<emps::mot_cle_permission>>(),
                      std::unique_ptr<AbstractGestionRestriction<emps::mot_prog_permission>> && gestionProgPermission
                        = std::make_unique<AbstractGestionRestriction<emps::mot_prog_permission>>());

    //! Mise en place de la prise en charge des restrictions de modification.
    void enableRestriction(const QString &restrictModification);

    //! Mise en place de la prise en charge des textes.
    void enableTexte(const QString &texte, const QString &cibleTexte, const QString &source, const QString &texteSource);

    //! Mise en place de la prise en charge des types.
    void enableType(const QString &type, const QString &permissionType,
                    std::unique_ptr<AbstractGestionRestriction<emps::Type>> && gestionType
                        = std::make_unique<AbstractGestionRestriction<emps::Type>>(),
                    std::unique_ptr<AbstractGestionRestriction<emps::type_permission>> && gestionPermission
                        = std::make_unique<AbstractGestionRestriction<emps::type_permission>>());

    //! Mise en place de la prise en charge des utilisations.
    void enableUtilisation(const QString &utilisation, const QString &usage, const QString &usagearbre,
                           std::unique_ptr<AbstractGestionRestriction<emps::Usage>> &&gestionUsage
                            = std::make_unique<AbstractGestionRestriction<emps::Usage>>());

    //! Mutateur du tableau de correspondance ID -> Cible.
    template<class Ent> void set_cible(int cible) {
        m_cibleArray[Ent::ID] = cible;
        m_cibleNbr_AttArray[cible] = Ent::Nbr_Att;
    }

    void setEnsembleEnable(bmps::ensembleFlag flag) noexcept
        {m_ensembleEnable |= flag;}

    //! Si les types sont pris en charge, déclare l'attribut type comme clé étrangère sur la table Type.
    template<class Ent> void set_typeForeignKey(InfoBdd &info);
};

template<> inline int ManagersPredef::cible<int>() const
    {return bmps::natureId::Int;}

template<> inline int ManagersPredef::cible<QString>() const
    {return bmps::natureId::String;}

template<> inline int ManagersPredef::cible<bool>() const
    {return bmps::natureId::Bool;}

template<> inline int ManagersPredef::cible<float>() const
    {return bmps::natureId::Float;}

template<> inline int ManagersPredef::cible<double>() const
    {return bmps::natureId::Double;}

template<> inline int ManagersPredef::cible<QDate>() const
    {return bmps::natureId::Date;}

template<> inline int ManagersPredef::cible<QDateTime>() const
    {return bmps::natureId::Date_Time;}

template<> inline int ManagersPredef::cible<QVariant>() const
    {return bmps::natureId::Variant;}

template<class Ent> void ManagersPredef::set_typeForeignKey(InfoBdd &info)
{
    if(ensembleEnable(bmps::TypeEnable))
        info.setForeignKey(Ent::Type,get(emps::Type::ID).info());
}
/*
template<> inline int ManagersPredef::cible<commentaire_cible>() const
    {return bmps::cibleId::commentaire_cible;}

template<> inline int ManagersPredef::cible<donnee_cible>() const
    {return bmps::cibleId::donnee_cible;}

template<> inline int ManagersPredef::cible<mot_cle_cible>() const
    {return bmps::cibleId::mot_cle_cible;}

template<> inline int ManagersPredef::cible<texte_cible>() const
    {return bmps::cibleId::texte_cible;}

template<> inline int ManagersPredef::cible<Commentaire>() const
    {return bmps::cibleId::Commentaire;}

template<> inline int ManagersPredef::cible<donnee>() const
    {return bmps::cibleId::donnee;}

template<> inline int ManagersPredef::cible<donnee_card>() const
    {return bmps::cibleId::donnee_card;}

template<> inline int ManagersPredef::cible<Historique>() const
    {return bmps::cibleId::Historique;}

template<> inline int ManagersPredef::cible<mot_cle>() const
    {return bmps::cibleId::mot_cle;}

template<> inline int ManagersPredef::cible<mot_cle_permission>() const
    {return bmps::cibleId::mot_cle_permission;}

template<> inline int ManagersPredef::cible<Restriction>() const
    {return bmps::cibleId::Restriction;}

template<> inline int ManagersPredef::cible<Source>() const
    {return bmps::cibleId::Source;}

template<> inline int ManagersPredef::cible<Texte>() const
    {return bmps::cibleId::Texte;}

template<> inline int ManagersPredef::cible<source_texte>() const
    {return bmps::cibleId::source_texte;}

template<> inline int ManagersPredef::cible<Type>() const
    {return bmps::cibleId::Type;}

template<> inline int ManagersPredef::cible<type_permission>() const
    {return bmps::cibleId::type_permission;}

template<> inline int ManagersPredef::cible<Utilisation>() const
    {return bmps::cibleId::Utilisation;}

template<> inline int ManagersPredef::cible<Usage>() const
    {return bmps::cibleId::Usage;}
*/
}
#endif // MANAGERSPREDEF_H
