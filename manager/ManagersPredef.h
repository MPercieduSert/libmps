/*Auteur: PERCIE DU SERT Maxime
 *Date: 16/08/2019
 */
#ifndef MANAGERSPREDEF_H
#define MANAGERSPREDEF_H

#include "ManagerArbreModifControle.h"
#include "ManagerArbreSimpleModifControle.h"
#include "ManagerPermissionModifControle.h"
#include "Managers.h"
#include "EntityPredef.h"

namespace bddMPS {
    //! Associe un numéro de cible.
    namespace cibleId  {
        //! Numéro de cible des types de base.
        enum typeBase{Vide = -1,
              Int = -2,
              String = -3,
              Bool = -4,
              Float = -5,
              Double = -6,
              Date = -7,
              DateTime = -8,
              Variant = -9};

        //! Numéro de cible des entités prédéfinies.
        enum EntityPredef{Commentaire,
                          CommentaireCible,
                          Donnee,
                          DonneeCard,
                          DonneeCible,
                          Historique,
                          MotCle,
                          MotCleCible,
                          MotClePermission,
                          MotProgCible,
                          MotProgPermission,
                          Restriction,
                          Source,
                          Texte,
                          TexteCible,
                          SourceTexte,
                          Type,
                          TypePermission,
                          Utilisation,
                          Usage,
                          NbrCibleEntPredef};
}}

namespace managerMPS {
namespace emps = entityMPS;
/*! \ingroup groupeFile
 *  \brief Classe mère des managers utilisant des entités prédéfinies.
 */
class ManagersPredef : public Managers {
protected:
    bool m_commentaireEnabled = false;                   //!< Prise en charge des commentaire.
    bool m_donneeEnabled = false;                        //!< Prise en charge des donnée.
    bool m_historiqueEnabled = false;                    //!< Prise en charge des Historique.
    bool m_motCleEnabled = false;                        //!< Prise en charge des MotCle.
    bool m_restrictionModificationEnabled = false;       //!< Prise en charge des Restriction.
    bool m_texteEnabled = false;                         //!< Prise en charge des Texte.
    bool m_typeEnabled = false;                          //!< Prise en charge des Type.
    bool m_utilisationEnabled = false;                   //!< Prise en charge des Uitilisation.
    std::vector<int> m_cibleArray;                           //!< Tableau de correspondance ID -> Cible.
    std::vector<int> m_cibleNbrAttArray;                     //!< Tableau de correspondance cible -> nombre d'attributs.

public:
    //! Constructeur.
    ManagersPredef(szt NbrEntity, szt nbrCible, const QString & versionTable,
                     std::map<szt,std::unique_ptr<AbstractManager>> && managers = std::map<szt,std::unique_ptr<AbstractManager>>(),
                     const QSqlQuery & req = QSqlQuery())
        : Managers (NbrEntity, versionTable, std::move(managers), req),
          m_cibleArray(nbrEntity(),bmps::cibleId::Vide),
          m_cibleNbrAttArray(nbrCible,0) {}

    //! Destructeur.
    ~ManagersPredef() = default;

    template<class Ent> int cible() const
        {return m_cibleArray[Ent::ID];}

    int cible(int id) const
        {return id >= 0 && id < m_nbrEntity ? m_cibleArray[id] : bmps::cibleId::Vide;}

    //! Renvoie le nombre d'attributs d'une entité à partir du numéro de cible.
    int nombreAttributCible(int cible) const {
        if(cible == bmps::cibleId::Vide || cible >= m_cibleNbrAttArray.size())
            return 0;
        if(cible < bmps::cibleId::Vide)
            return 1;
        return m_cibleNbrAttArray[(cible)];
    }

    //! Prise en charge des commentaires?
    bool commentaireIsEnabled() const
        {return m_commentaireEnabled;}

    //! Prise en charge des données?
    bool donneeIsEnabled() const
        {return m_donneeEnabled;}

    //! Prise en charge des historiques?
    bool historiqueIsEnabled() const
        {return m_historiqueEnabled;}

    //! Prise en charge des mots clé?
    bool motCleIsEnabled() const
        {return m_motCleEnabled;}

    //! Prise en charge des restrictions de modification?
    bool restrictionModificationIsEnabled() const
        {return m_restrictionModificationEnabled;}

    //! Prise en charge des textes?
    bool texteIsEnabled() const
        {return m_texteEnabled;}

    //! Prise en charge des type?
    bool typeIsEnabled() const
        {return m_typeEnabled;}

    //! Prise en charge des utilisations?
    bool utilisationIsEnabled() const
        {return m_utilisationEnabled;}

protected:
    //! Mise en place de la prise en charge des commentaires.
    void enableCommentaire(const QString & commentaire, const QString & cibleCommentaire);
    
    //! Mise en place de la prise en charge des donnees.
    void enableDonnee(const QString & donnee, const QString & arbreDonnee, const QString & cibleDonnee, const QString & cardDonnee,
                      std::unique_ptr<AbstractGestionAutorisation<emps::Donnee>> &&  gestionDonnee
                        = std::make_unique<GestionAutorisationNoRestrictif<emps::Donnee>>(),
                      std::unique_ptr<AbstractGestionAutorisation<emps::DonneeCible>> &&  gestionCilbeDonnee
                        = std::make_unique<GestionAutorisationNoRestrictif<emps::DonneeCible>>(),
                      std::unique_ptr<AbstractGestionAutorisation<emps::DonneeCard>> &&  gestionCardDonnee
                        = std::make_unique<GestionAutorisationNoRestrictif<emps::DonneeCard>>());

    //! Mise en place de la prise en charge des historiques.
    void enableHistorique(const QString & historique);

    //! Mise en place de la prise en charge des mots Clé.
    void enableMotCle(const QString & motCle, const QString & motCleArbre, const QString & cibleMotCle,
                      const QString & cibleProg, const QString & permissionMotCle, const QString & permissionProg,
                      std::unique_ptr<AbstractGestionAutorisation<emps::MotCle>> &&  gestionMotCle
                        = std::make_unique<GestionAutorisationNoRestrictif<emps::MotCle>>(),
                      std::unique_ptr<AbstractGestionAutorisation<emps::MotClePermission>> &&  gestionPermission
                        = std::make_unique<GestionAutorisationNoRestrictif<emps::MotClePermission>>(),
                      std::unique_ptr<AbstractGestionAutorisation<emps::MotProgPermission>> &&  gestionProgPermission
                        = std::make_unique<GestionAutorisationNoRestrictif<emps::MotProgPermission>>());

    //! Mise en place de la prise en charge des restrictions de modification.
    void enableRestriction(const QString & restrictModification);

    //! Mise en place de la prise en charge des textes.
    void enableTexte(const QString & texte, const QString & cibleTexte, const QString & source, const QString & texteSource);

    //! Mise en place de la prise en charge des types.
    void enableType(const QString & type, const QString & permissionType,
                    std::unique_ptr<AbstractGestionAutorisation<emps::Type>> &&  gestionType
                        = std::make_unique<GestionAutorisationNoRestrictif<emps::Type>>(),
                    std::unique_ptr<AbstractGestionAutorisation<emps::TypePermission>> &&  gestionPermission
                        = std::make_unique<GestionAutorisationNoRestrictif<emps::TypePermission>>());

    //! Mise en place de la prise en charge des utilisations.
    void enableUtilisation(const QString & utilisation, const QString & usage, const QString &usageArbre,
                           std::unique_ptr<AbstractGestionAutorisation<emps::Usage>> && gestionUsage
                            = std::make_unique<GestionAutorisationNoRestrictif<emps::Usage>>());

    //! Mutateur du tableau de correspondance ID -> Cible.
    template<class Ent> void setCible(int cible) {
        m_cibleArray[Ent::ID] = cible;
        m_cibleNbrAttArray[cible] = Ent::NbrAtt;
    }

    //! Si les types sont pris en charge, déclare l'attribut type comme clé étrangère sur la table Type.
    template<class Ent> void setTypeForeignKey(InfoBdd & info);
};

template<> inline int ManagersPredef::cible<int>() const
    {return bmps::cibleId::Int;}

template<> inline int ManagersPredef::cible<QString>() const
    {return bmps::cibleId::String;}

template<> inline int ManagersPredef::cible<bool>() const
    {return bmps::cibleId::Bool;}

template<> inline int ManagersPredef::cible<float>() const
    {return bmps::cibleId::Float;}

template<> inline int ManagersPredef::cible<double>() const
    {return bmps::cibleId::Double;}

template<> inline int ManagersPredef::cible<QDate>() const
    {return bmps::cibleId::Date;}

template<> inline int ManagersPredef::cible<QDateTime>() const
    {return bmps::cibleId::DateTime;}

template<> inline int ManagersPredef::cible<QVariant>() const
    {return bmps::cibleId::Variant;}

template<class Ent> void ManagersPredef::setTypeForeignKey(InfoBdd & info)
{
    if(typeIsEnabled())
        info.setForeignKey(Ent::Type,get(emps::Type::ID).info());
}
/*
template<> inline int ManagersPredef::cible<CommentaireCible>() const
    {return bmps::cibleId::CommentaireCible;}

template<> inline int ManagersPredef::cible<DonneeCible>() const
    {return bmps::cibleId::DonneeCible;}

template<> inline int ManagersPredef::cible<MotCleCible>() const
    {return bmps::cibleId::MotCleCible;}

template<> inline int ManagersPredef::cible<TexteCible>() const
    {return bmps::cibleId::TexteCible;}

template<> inline int ManagersPredef::cible<Commentaire>() const
    {return bmps::cibleId::Commentaire;}

template<> inline int ManagersPredef::cible<Donnee>() const
    {return bmps::cibleId::Donnee;}

template<> inline int ManagersPredef::cible<DonneeCard>() const
    {return bmps::cibleId::DonneeCard;}

template<> inline int ManagersPredef::cible<Historique>() const
    {return bmps::cibleId::Historique;}

template<> inline int ManagersPredef::cible<MotCle>() const
    {return bmps::cibleId::MotCle;}

template<> inline int ManagersPredef::cible<MotClePermission>() const
    {return bmps::cibleId::MotClePermission;}

template<> inline int ManagersPredef::cible<Restriction>() const
    {return bmps::cibleId::Restriction;}

template<> inline int ManagersPredef::cible<Source>() const
    {return bmps::cibleId::Source;}

template<> inline int ManagersPredef::cible<Texte>() const
    {return bmps::cibleId::Texte;}

template<> inline int ManagersPredef::cible<SourceTexte>() const
    {return bmps::cibleId::SourceTexte;}

template<> inline int ManagersPredef::cible<Type>() const
    {return bmps::cibleId::Type;}

template<> inline int ManagersPredef::cible<TypePermission>() const
    {return bmps::cibleId::TypePermission;}

template<> inline int ManagersPredef::cible<Utilisation>() const
    {return bmps::cibleId::Utilisation;}

template<> inline int ManagersPredef::cible<Usage>() const
    {return bmps::cibleId::Usage;}
*/
}
#endif // MANAGERSPREDEF_H
