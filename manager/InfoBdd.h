/*Auteur: PERCIE DU SERT Maxime
 *Date: 09/09/2018
 */
#ifndef INFOBDD_H
#define INFOBDD_H

#include<QMap>
#include<QString>
#include<QVector>
#include"AbstractManager.h"

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
