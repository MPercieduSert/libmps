/*Auteur: PERCIE DU SERT Maxime
 *Date: 09/09/2018
 */
#ifndef INFO_BDD_H
#define INFO_BDD_H

#include <map>
#include <QString>
#include <vector>
#include "entity.h"

/*! \defgroup groupe_manager Manageurs
 *\brief Ensemble de classes représentant les managers des entités de la base de donnée.
 */

namespace mps {
/*! \ingroup groupe_manager
 *\brief Espace de nom pour la base de donnée.
 */
namespace b2d {
    //! Restriction de modification d'une entité.
    enum restriction_code : flag::flag_type {Aucune = 0x0,
                                        Modif = 0x1,
                                        Suppr = 0x2,
                                        Modif_Suppr = Modif | Suppr};

    //! Identifiant pour les fonctions d'agrega.
    enum agrega {Max,
                Min,
                Moyenne,
                Nombre,
                Somme,
                Nbr_Agrega};

    //! Les différents types de conditions
    enum condition {Egal,
                   Is,
                   Inf,
                   Sup,
                   Inf_Egal,
                   Sup_Egal,
                   Different,
                   Is_Not};

    //! Type de donnée pour la base de donnée
    enum type_attribut_bdd{Blob,
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
                   Nbr_Type};

    //! Type de Manager
    enum type_manager_code : flag::flag_type {
                    Normal_Type_Manager = 0,
                    Arbre_Type_Manager = 0x1,
                    Arbre_Simple_Type_Manager = 0x2,
                    Modif_Controle_Type_Manager = 0x4,
                    Permission_Type_Manager = 0x8};

    /*! \brief Les différents cas des résultats des tests d'existence d'unicité.
     *(concordance d'indentifiant: ou bien l'identifiant en base de donnée et le même celui de l'entité testée ou bien ce dernier est nul.)
     *
     */
    enum existe_uni {
            None = -1,   //!< Indentifiant de programmation.
            Aucun = 0,  //!< Aucun des ensembles d'unicité n'existent en base de donnée.
            Tous = 1,   //!< Tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Meme = 2,   //!< Certains mais pas tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Autre = 3,  //!< Certains des ensembles d'unicité existent en base de donnée dans la même entité avec un autre identifiant.
            Conflit = 4 //!< Certains des ensembles d'unicité existent en base de donnée dans des entités différentes.
            };

    //! Enumeration des différent type de sauvegarde d'un arbre.
    enum tree_save {Entity_Only,
                   Add_Leaf,
                   Without_Delete,
                   Internal_Change,
                   External_Change,
                   Entity_Only_Whitout_Root,
                   Add_Leaf_Whitout_Root,
                   Without_Delete_Whitout_Root,
                   Internal_Change_Whitout_Root,
                   External_Change_Whitout_Root};

/*! \ingroup groupe_manager
 *\brief Classe des informations d'une entité sur sa table dans la base de données.
 */
class info_bdd {
public:
using caract = std::pair<type_attribut_bdd, bool>;

protected:
    std::vector<QString> m_att_sql;                  //!< Vecteur des noms des attribut dans la base de donnée.
    std::vector<caract> m_att_caract;                //!< Vecteur des propriétés des attributs pour la création de la table.
    std::vector<std::vector<post>> m_att_unique;     //!< Tableau 2d des indices des attributs uniques.
    std::map<post,QString> m_foreign_key_name;         //!< Map des clés étrangères.
    std::map<post,QString> m_foreign_key_table;        //!< Map des clés étrangères.
    QString m_name;                                 //!< Nom de l'entité géré par le manager.
    QString m_table;                                //!< Nom de la table dans la base de donnée.

public:
    //! Constructeur.
    info_bdd() = default;

    //! Constructeur.
    info_bdd(const QString &name, const QString &table, const std::map<post,QString> &att,
            const std::vector<caract> &att_caract,
            const std::vector<std::vector<post>> &att_unique = std::vector<std::vector<post>>(),
            const std::map<post,QString> &foreign_key_name = std::map<post,QString>(),
            const std::map<post,QString> &foreign_key_table = std::map<post,QString>());

    //! Constructeur. Information à completer sur les attributs.
    info_bdd(const QString &name, const QString &table, post nbr_att, const std::vector<post> &nbr_att_unique = std::vector<post>());

    //! Constructeur de copie.
    info_bdd(const info_bdd &) = default;

    //! Destructeur.
    ~info_bdd() = default;

    //! Retourne le nom en base de donnee du n-ième attribut.
    const QString &attribut(post n) const
        {return m_att_sql[n];}

    //! Liste des indices des attributs uniques.
    const std::vector<post> &attribut_unique(numt num = 0) const
        {return m_att_unique[num];}

    //! Liste des propriétés des attributs lors de la création de la table.
    const caract &creer_attribut(post num) const
        {return m_att_caract[num-1];}

    //! Liste des clés étrangères avec le nom des entités.
    const std::map<post,QString> &foreign_key_name() const
        {return m_foreign_key_name;}

    //! Liste des clés étrangères avec le nom de tables.
    const std::map<post,QString> &foreign_key_table() const
        {return m_foreign_key_table;}

    //! Renvoie le nom de l'entité géré par le manager.
    const QString &name() const
        {return m_name;}

    //! Nombre d'attribut de la table.
    post nbr_att() const
        {return static_cast<post>(m_att_sql.size());}

    //! Nombre de jeu d'attribut unique de la table.
    numt nbr_set_unique() const
        {return static_cast<numt>(m_att_unique.size());}


    //! Mutateur d'un attribut.
    void set_attribut(post num, const QString &name, type_attribut_bdd type_att = type_attribut_bdd::Integer, bool not_null = true) {
        m_att_sql[num] = name;
        m_att_caract[num-1] = caract(type_att,not_null);
    }

    //! Mutateur d'une clé étrangère.
    void set_foreign_key(post numAtt, const info_bdd &info) {
        m_foreign_key_name[numAtt] = info.name();
        m_foreign_key_table[numAtt] = info.table();
    }

    //! Mutateur de l'entité géré par le manager.
    void set_name(const QString &name)
        {m_name = name;}

    //! Mutateur d'un attribut unique.
    void set_unique(post numAtt, numt numUnique, numt numSet = 0)
        {m_att_unique[numSet][numUnique] = numAtt;}

    //! Nom de la table.
    const QString &table() const
        {return m_table;}
};
}}
#endif // INFOBDD_H
