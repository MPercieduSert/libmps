/*Auteur: PERCIE DU SERT Maxime
 *Date: 16/08/2019
 */
#ifndef MANAGERSPREDEF_H
#define MANAGERSPREDEF_H

#include "ManagerOfArbreModifControle.h"
#include "ManagerOfArbreSimpleModifControle.h"
#include "ManagerOfModifControlePermission.h"
#include "Managers.h"
#include "EntityPredef.h"
#include "EntitiesPredef.h"

namespace bdd {
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
        enum EntityPredef{CibleCommentaire,
                          CibleDonnee,
                          CibleMotCle,
                          CibleTexte,
                          Commentaire,
                          Donnee,
                          DonneeCard,
                          Historique,
                          MotCle,
                          MotClePermission,
                          Restriction,
                          Source,
                          Texte,
                          TexteSource,
                          Type,
                          TypePermission,
                          Utilisation,
                          Usage,
                          NbrCibleEntPredef};
}}

/*! \ingroup groupeFile
 *  \brief Classe mère des managers utilisant des entités prédéfinies.
 */
class ManagersPredef : public Managers
{
protected:
    bool m_commentaireEnabled = false;                   //!< Prise en charge des commentaire.
    bool m_donneeEnabled = false;                        //!< Prise en charge des donnée.
    bool m_historiqueEnabled = false;                    //!< Prise en charge des Historique.
    bool m_motCleEnabled = false;                        //!< Prise en charge des MotCle.
    bool m_restrictionModificationEnabled = false;       //!< Prise en charge des Restriction.
    bool m_texteEnabled = false;                         //!< Prise en charge des Texte.
    bool m_typeEnabled = false;                          //!< Prise en charge des Type.
    bool m_utilisationEnabled = false;                   //!< Prise en charge des Uitilisation.
    QVector<int> m_cibleArray;                           //!< Tableau de correspondance ID -> Cible.
    QVector<int> m_cibleNbrAttArray;                     //!< Tableau de correspondance cible -> nombre d'attributs.

public:
    //! Constructeur.
    ManagersPredef(const int NbrEntity, const int nbrCible, const QString & versionTable,
                     const QMap<int,AbstractManager *> & managers = QMap<int,AbstractManager *>(),
                     const QSqlQuery & req = QSqlQuery())
        : Managers (NbrEntity, versionTable, managers, req),
          m_cibleArray(nbrEntity(),bdd::cibleId::Vide),
          m_cibleNbrAttArray(nbrCible,0)
    {}

    //! Destructeur.
    ~ManagersPredef() = default;

    template<class Ent> int cible() const
        {return m_cibleArray[Ent::ID];}

    int cible(int id) const
        {return id >= 0 && id < m_nbrEntity ? m_cibleArray[id] : bdd::cibleId::Vide;}

    //! Renvoie le nombre d'attributs d'une entité à partir du numéro de cible.
    int nombreAttributCible(int cible) const
    {
        if(cible == bdd::cibleId::Vide || cible >= m_cibleNbrAttArray.size())
            return 0;
        if(cible < bdd::cibleId::Vide)
            return 1;
        return m_cibleNbrAttArray.at(cible);
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
                      AbstractGestionAutorisation<Donnee> * gestionDonnee = nullptr,
                      AbstractGestionAutorisation<CibleDonnee> * gestionCilbeDonnee = nullptr,
                      AbstractGestionAutorisation<DonneeCard> * gestionCardDonnee = nullptr);

    //! Mise en place de la prise en charge des historiques.
    void enableHistorique(const QString & historique);

    //! Mise en place de la prise en charge des mots Clé.
    void enableMotCle(const QString & motCle, const QString & motCleArbre, const QString & cibleMotCle,
                      const QString & permissionMotCle,
                      AbstractGestionAutorisation<MotCle> * gestionMotCle = nullptr,
                      AbstractGestionAutorisation<MotClePermission> * gestionPermission = nullptr);

    //! Mise en place de la prise en charge des restrictions de modification.
    void enableRestriction(const QString & restrictModification);

    //! Mise en place de la prise en charge des textes.
    void enableTexte(const QString & texte, const QString & cibleTexte, const QString & source, const QString & texteSource);

    //! Mise en place de la prise en charge des types.
    void enableType(const QString & type, const QString & permissionType,
                    AbstractGestionAutorisation<Type> * gestionType = nullptr,
                    AbstractGestionAutorisation<TypePermission> * gestionPermission = nullptr);

    //! Mise en place de la prise en charge des utilisations.
    void enableUtilisation(const QString & utilisation, const QString & usage, const QString &usageArbre,
                           AbstractGestionAutorisation<Usage> *gestionUsage = nullptr);

    //! Mutateur du tableau de correspondance ID -> Cible.
    template<class Ent> void setCible(int cible)
    {
        m_cibleArray[Ent::ID] = cible;
        m_cibleNbrAttArray[cible] = Ent::NbrAtt;
    }
};

template<> inline int ManagersPredef::cible<int>() const
    {return bdd::cibleId::Int;}

template<> inline int ManagersPredef::cible<QString>() const
    {return bdd::cibleId::String;}

template<> inline int ManagersPredef::cible<bool>() const
    {return bdd::cibleId::Bool;}

template<> inline int ManagersPredef::cible<float>() const
    {return bdd::cibleId::Float;}

template<> inline int ManagersPredef::cible<double>() const
    {return bdd::cibleId::Double;}

template<> inline int ManagersPredef::cible<QDate>() const
    {return bdd::cibleId::Date;}

template<> inline int ManagersPredef::cible<QDateTime>() const
    {return bdd::cibleId::DateTime;}

template<> inline int ManagersPredef::cible<QVariant>() const
    {return bdd::cibleId::Variant;}
/*
template<> inline int ManagersPredef::cible<CibleCommentaire>() const
    {return bdd::cibleId::CibleCommentaire;}

template<> inline int ManagersPredef::cible<CibleDonnee>() const
    {return bdd::cibleId::CibleDonnee;}

template<> inline int ManagersPredef::cible<CibleMotCle>() const
    {return bdd::cibleId::CibleMotCle;}

template<> inline int ManagersPredef::cible<CibleTexte>() const
    {return bdd::cibleId::CibleTexte;}

template<> inline int ManagersPredef::cible<Commentaire>() const
    {return bdd::cibleId::Commentaire;}

template<> inline int ManagersPredef::cible<Donnee>() const
    {return bdd::cibleId::Donnee;}

template<> inline int ManagersPredef::cible<DonneeCard>() const
    {return bdd::cibleId::DonneeCard;}

template<> inline int ManagersPredef::cible<Historique>() const
    {return bdd::cibleId::Historique;}

template<> inline int ManagersPredef::cible<MotCle>() const
    {return bdd::cibleId::MotCle;}

template<> inline int ManagersPredef::cible<MotClePermission>() const
    {return bdd::cibleId::MotClePermission;}

template<> inline int ManagersPredef::cible<Restriction>() const
    {return bdd::cibleId::Restriction;}

template<> inline int ManagersPredef::cible<Source>() const
    {return bdd::cibleId::Source;}

template<> inline int ManagersPredef::cible<Texte>() const
    {return bdd::cibleId::Texte;}

template<> inline int ManagersPredef::cible<TexteSource>() const
    {return bdd::cibleId::TexteSource;}

template<> inline int ManagersPredef::cible<Type>() const
    {return bdd::cibleId::Type;}

template<> inline int ManagersPredef::cible<TypePermission>() const
    {return bdd::cibleId::TypePermission;}

template<> inline int ManagersPredef::cible<Utilisation>() const
    {return bdd::cibleId::Utilisation;}

template<> inline int ManagersPredef::cible<Usage>() const
    {return bdd::cibleId::Usage;}
*/
#endif // MANAGERSPREDEF_H
