/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef MOTCLEMODEL_H
#define MOTCLEMODEL_H

#include <QSet>
#include "TreeModelReadTemp.h"
#include "BddPredef.h"

/*! \ingroup groupeModel
 * \brief Model de gestion des motcle.
 */
class MotCleModel : public TreeModelReadTemp<QPair<MotCle,int>>
{
    Q_OBJECT
protected:
    enum {PasTous,TousJusquaPresent,Tous};
    BddPredef * m_bdd;          //!< Pointeur vers la base de donnée.
    const int m_cible;          //!< Numero de cible du type d'entité pour les quels on souhaite gérer les mots clés.
    QSet<int> m_idSet;          //!< Ensemble des identifiants des entités dont les mots clés sont affichés.
    QMap<int,bool> m_idMotCle;  //!< Map dont les clés sont les identifiants des mots clés associés aux entités d'identifiant contenus dans m_idSet et de valeur true si le mot clé est associé à toutes les entités dont les identifiants sont contenus dans m_idSet et false sinon..

public:
    enum ColumnIndex{CheckColumn, NomColumn, NbrColumn, Aucun};

    //! Constructeur.
    explicit MotCleModel(BddPredef *bd, int cible, QObject * parent);

    //! Destructeur par defaut.
    ~MotCleModel() override = default;

    //! Renvoie le nombre de colonnes.
    int columnCount(const QModelIndex & /*parent*/) const override
        {return NbrColumn;}

    //! Renvoie la donnée correspondant à l'index et au role.
    QVariant data(const QModelIndex &index, int role) const override;

    //! Renvoie la liste des mots clés associés aux entités dont d'identifiant est contenu dans m_idSet.
    QString dataListeNomMotCle() const;

    //! Renvoie les labels des colonnes
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //! Mes à jour l'arbre des mots clès.
    void setTreeMotCle();

signals:
    void changedIdSet(QSet<int> IdSet);

public slots:
    //!< Mutateur de m_idEntityList, l'identifiant de l'entité pour laquelle sont affiché les mots clés.
    void setIdSet(const QSet<int> & idSet);
};

#endif // MOTCLEMODEL_H
