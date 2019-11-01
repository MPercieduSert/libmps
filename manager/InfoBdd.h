/*Auteur: PERCIE DU SERT Maxime
 *Date: 09/09/2018
 */
#ifndef INFOBDD_H
#define INFOBDD_H

#include<QMap>
#include<QString>
#include<QVector>
#include "Entity.h"

/*! \defgroup groupeManager Manageurs
 * \brief Ensemble de classes représentant les managers des entités de la base de donnée.
 */

/*! \ingroup groupeManager
 * \brief Espace de nom pour la base de donnée.
 */
namespace bdd {
    //! Autorisation de modification d'une entité.
    enum Autorisation{Toute = 1,
                      Modif = 2,
                      Suppr = 3};
    //! Identifiant pour les fonctions d'agrega.
    enum Agrega {Max,
                Min,
                Moyenne,
                Nombre,
                Somme,
                NbrAgrega};

    //! Les différents types de conditions
    enum Condition {Egal,
                   Is,
                   Inf,
                   Sup,
                   InfEgal,
                   SupEgal,
                   Different,
                   IsNot};

    //! Type de donnée pour la base de donnée
    enum TypeAttributBdd{Blob,
                   Bool,
                   Date,
                   DateTime,
                   Double,
                   Integer,
                   Numeric,
                   Primary,
                   Real,
                   Text,
                   Variant,
                   NbrType};

    /*! \brief Les différents cas des résultats des tests d'existence d'unicité.
     * (concordance d'indentifiant: ou bien l'identifiant en base de donnée et le même celui de l'entité testée ou bien ce dernier est nul.)
     *
     */
    enum ExisteUni {Aucun = 0,  //!< Aucun des ensembles d'unicité n'existent en base de donnée.
            Tous = 1,   //!< Tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Meme = 2,   //!< Certains mais pas tous les ensembles d'unicité existent en base de donnée dans la même entité avec concordance d'identifiant.
            Autre = 3,  //!< Certains des ensembles d'unicité existent en base de donnée dans la même entité avec un autre identifiant.
            Conflit = 4 //!< Certains des ensembles d'unicité existent en base de donnée dans des entités différentes.
            };

    //! Enumeration des différent type de sauvegarde d'un arbre.
    enum TreeSave {EntityOnly,
                   AddLeaf,
                   WithoutDelete,
                   InternalChange,
                   ExternalChange,
                   EntityOnlyWhitoutRoot,
                   AddLeafWhitoutRoot,
                   WithoutDeleteWhitoutRoot,
                   InternalChangeWhitoutRoot,
                   ExternalChangeWhitoutRoot};
}

/*! \ingroup groupeManager
 * \brief Classe des informations d'une entité sur sa table dans la base de données.
 */
class InfoBdd
{
public:
using Caract = QPair<bdd::TypeAttributBdd, bool>;

protected:
    QVector<QString> m_attSql;      //!< Vecteur des noms des attribut dans la base de donnée.
    QVector<Caract> m_attCaract;     //!< Vecteur des propriétés des attributs pour la création de la table.
    QVector<QVector<int>> m_attUnique;       //!< Tableau 2d des indices des attributs uniques.
    QMap<int,QString> m_foreignKey;         //!< Map des clés étrangères.
    QString m_table;        //!< Nom de la table dans la base de donnée.
public:
    //! Constructeur.
    InfoBdd(const QString & table, const QMap<int,QString> & att,
            const QVector<Caract> & attCaract,
            const QVector<QVector<int>> & attUnique = QVector<QVector<int>>(),
            const QMap<int,QString> & foreignKey = QMap<int,QString>());

    //! Constructeur. Information sur à completer sur les attributs.
    InfoBdd(const QString & table, int nbrAtt, const QVector<int> & nbrAttUnique = QVector<int>());

    //! Constructeur de copie.
    InfoBdd(const InfoBdd & ) = default;

    //! Destructeur.
    ~InfoBdd() = default;

    //! Retourne le nom en base de donnee du n-ième attribut.
    const QString & attribut(int n) const
        {return m_attSql.at(n);}

    //! Liste des indices des attributs uniques.
    const QVector<int> & attributUnique(int num = 0) const
        {return m_attUnique.at(num);}

    //! Liste des propriétés des attributs lors de la création de la table.
    const Caract & creerAttribut(int num) const
        {return m_attCaract.at(num-1);}

    //! Liste des clés étrangères.
    const QMap<int,QString> & foreignKey() const
        {return m_foreignKey;}

    //! Nombre d'attribut de la table.
    int nbrAtt() const
        {return m_attSql.count();}

    //! Mutateur d'un attribut.
    void setAttribut(int num, const QString & name, bdd::TypeAttributBdd typeAtt = bdd::TypeAttributBdd::Integer, bool notNull = true)
    {
        m_attSql[num] = name;
        m_attCaract[num-1] = Caract(typeAtt,notNull);
    }

    //! Mutateur d'une clé étrangère.
    void setForeignKey(int numAtt, const InfoBdd & info)
        {m_foreignKey[numAtt] = info.table();}

    //! Mutateur d'un attribut unique.
    void setUnique(int numAtt, int numUnique, int numSet = 0)
        {m_attUnique[numSet][numUnique] = numAtt;}

    //! Nom de la table.
    const QString & table() const
        {return m_table;}
};

#endif // INFOBDD_H
