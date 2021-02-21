/*Auteur: PERCIE DU SERT Maxime
 *Date: 09/09/2018
 */
#ifndef INFOBDD_H
#define INFOBDD_H

#include<map>
#include<QString>
#include<vector>
#include "entity.h"

/*! \defgroup groupeManager Manageurs
 *\brief Ensemble de classes représentant les managers des entités de la base de donnée.
 */

/*! \ingroup groupeManager
 *\brief Espace de nom pour la base de donnée.
 */
namespace bddMPS {
using namespace type_mps;
namespace emps = entities;

    //! Restriction de modification d'une entité.
    enum restriction : flag::flag_type {Aucune = 0x0,
                      Modif = 0x1,
                      Suppr = 0x2,
                      ModifSupr = 0x3};

    //! Identifiant pour les fonctions d'agrega.
    enum agrega {Max,
                Min,
                Moyenne,
                Nombre,
                Somme,
                NbrAgrega};

    //! Les différents types de conditions
    enum condition {Egal,
                   Is,
                   Inf,
                   Sup,
                   InfEgal,
                   SupEgal,
                   Different,
                   IsNot};

    //! Type de donnée pour la base de donnée
    enum typeAttributBdd{Blob,
                   Bool,
                   Date,
                   Date_Time,
                   Double,
                   Integer,
                   Numeric,
                   Primary,
                   Real,
                   Text,
                   Variant,
                   NbrType};

    //! Type de Manager
    enum typeManager : flag::flag_type {
        NormalTypeManager = 0,
        arbreTypeManager = 0x1,
        arbre_simpleTypeManager = 0x2,
        ModifControleTypeManager = 0x4,
        PermissionTypeManager = 0x8};

    /*! \brief Les différents cas des résultats des tests d'existence d'unicité.
     *(concordance d'indentifiant: ou bien l'identifiant en base de donnée et le même celui de l'entité testée ou bien ce dernier est nul.)
     *
     */
    enum existeUni {
            None = -1,   //!< Indentifiant de programmation.
            Aucun = 0,  //!< Aucun des ensembles d'unicité n'existent en base de donnée.
            Tous = 1,   //!< Tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Meme = 2,   //!< Certains mais pas tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Autre = 3,  //!< Certains des ensembles d'unicité existent en base de donnée dans la même entité avec un autre identifiant.
            Conflit = 4 //!< Certains des ensembles d'unicité existent en base de donnée dans des entités différentes.
            };

    //! Enumeration des différent type de sauvegarde d'un arbre.
    enum treeSave {entityOnly,
                   AddLeaf,
                   WithoutDelete,
                   InternalChange,
                   ExternalChange,
                   entityOnlyWhitoutRoot,
                   AddLeafWhitoutRoot,
                   WithoutDeleteWhitoutRoot,
                   InternalChangeWhitoutRoot,
                   ExternalChangeWhitoutRoot};

/*! \ingroup groupeManager
 *\brief Classe des informations d'une entité sur sa table dans la base de données.
 */
class InfoBdd {
public:
using Caract = std::pair<typeAttributBdd, bool>;

protected:
    std::vector<QString> m_attSql;                  //!< Vecteur des noms des attribut dans la base de donnée.
    std::vector<Caract> m_attCaract;                //!< Vecteur des propriétés des attributs pour la création de la table.
    std::vector<std::vector<post>> m_attUnique;     //!< Tableau 2d des indices des attributs uniques.
    std::map<post,QString> m_foreignKeyName;         //!< Map des clés étrangères.
    std::map<post,QString> m_foreignKeyTable;        //!< Map des clés étrangères.
    QString m_name;                                 //!< Nom de l'entité géré par le manager.
    QString m_table;                                //!< Nom de la table dans la base de donnée.

public:
    //! Constructeur.
    InfoBdd() = default;

    //! Constructeur.
    InfoBdd(const QString &name, const QString &table, const std::map<post,QString> &att,
            const std::vector<Caract> &attCaract,
            const std::vector<std::vector<post>> &attUnique = std::vector<std::vector<post>>(),
            const std::map<post,QString> &foreignKeyName = std::map<post,QString>(),
            const std::map<post,QString> &foreignKeyTable = std::map<post,QString>());

    //! Constructeur. Information à completer sur les attributs.
    InfoBdd(const QString &name, const QString &table, post nbr_att, const std::vector<post> &nbr_attUnique = std::vector<post>());

    //! Constructeur de copie.
    InfoBdd(const InfoBdd &) = default;

    //! Destructeur.
    ~InfoBdd() = default;

    //! Retourne le nom en base de donnee du n-ième attribut.
    const QString &attribut(post n) const
        {return m_attSql[n];}

    //! Liste des indices des attributs uniques.
    const std::vector<post> &attributUnique(numt num = 0) const
        {return m_attUnique[num];}

    //! Liste des propriétés des attributs lors de la création de la table.
    const Caract &creerAttribut(post num) const
        {return m_attCaract[num-1];}

    //! Liste des clés étrangères avec le nom des entités.
    const std::map<post,QString> &foreignKeyName() const
        {return m_foreignKeyName;}

    //! Liste des clés étrangères avec le nom de tables.
    const std::map<post,QString> &foreignKeyTable() const
        {return m_foreignKeyTable;}

    //! Renvoie le nom de l'entité géré par le manager.
    const QString &name() const
        {return m_name;}

    //! Nombre d'attribut de la table.
    post nbr_att() const
        {return static_cast<post>(m_attSql.size());}

    //! Nombre de jeu d'attribut unique de la table.
    numt nbrSetUnique() const
        {return static_cast<numt>(m_attUnique.size());}


    //! Mutateur d'un attribut.
    void set_attribut(post num, const QString &name, typeAttributBdd typeAtt = typeAttributBdd::Integer, bool not_null = true) {
        m_attSql[num] = name;
        m_attCaract[num-1] = Caract(typeAtt,not_null);
    }

    //! Mutateur d'une clé étrangère.
    void setForeignKey(post numAtt, const InfoBdd &info) {
        m_foreignKeyName[numAtt] = info.name();
        m_foreignKeyTable[numAtt] = info.table();
    }

    //! Mutateur de l'entité géré par le manager.
    void set_name(const QString &name)
        {m_name = name;}

    //! Mutateur d'un attribut unique.
    void setUnique(post numAtt, numt numUnique, numt numSet = 0)
        {m_attUnique[numSet][numUnique] = numAtt;}

    //! Nom de la table.
    const QString &table() const
        {return m_table;}
};
}
#endif // INFOBDD_H
