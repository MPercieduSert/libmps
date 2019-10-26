/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef TREEMODELMOTCLE_H
#define TREEMODELMOTCLE_H


#include <array>
#include "TreeModelEditEntity.h"
#include "BddPredef.h"
#include "NewMotCleDialog.h"

/*! \ingroup groupeModel
 * \brief Model de gestion des motcle.
 */
class TreeModelMotCle : public TreeModelEditEntity<MotCle>
{
public:
    /*//! Enumération des permission de motcle possible.
    enum perm {
        BaremeIndex,
        CommentaireIndex,
        ControleIndex,
        CoursIndex,
        EnonceIndex,
        ExerciceIndex,
        MotCleIndex,
        TexteIndex,
        TypeControleIndex,
        TypeUtilisationIndex,
        UtilisationIndex,
        NbrIndex
    };*/

    //! Enumeration des nombres de colonnes.
    enum {
        NomMotCleIndex,
        NcMotCleIndex,
        NbrColumnMotCle,
        NbrColumnSup = 1,
        NbrColumn = NbrColumnMotCle + NbrColumnSup// + NbrIndex
    };

protected:
    const int m_nbrEntity;                          //!< Nombre d'entités associées aux mots clés.
    BddPredef * m_bdd;                              //!< Pointeur vers la base de donnée.
    QVector<int> m_cibleList;                       //!< Liste des valeurs de cible associé à chaque index de colonnes.
    QStringList m_headerPerm;                       //!< Noms des colonnes.
    QMap<idt,QVector<codeType>> m_permission;            //!< Map des permitions indexé par l'identifiant du mot clé associé.

    using TreeModelEditEntity<MotCle>::m_tree;
    using TreeModelEditEntity<MotCle>::getItem;
    using TreeModelEditEntity<MotCle>::setDataTree;

public:
    //! Constructeur, la variable header contient le nom des entité associé à leur identifiant.
    explicit TreeModelMotCle(BddPredef * bdd, const QMap<int,QString> & header, QObject *parent = nullptr);

    //! Destructeur par defaut.
    ~TreeModelMotCle() override = default;

    //! Renvoie les autorisations de modification pour un index donné.
    bool autorisation(const QModelIndex & index, bdd::Autorisation autoris) const override;

    //! Renvoie le numero de cible correspond à l'index
    int cible(const QModelIndex & index) const
    {
        if(NbrColumnMotCle <= index.column() && index.column() < NbrColumnMotCle + m_nbrEntity)
            return m_cibleList[index.column() - NbrColumnMotCle];
        else
            return -1;
    }

    //! Renvoie le nombre de colonnes.
    int columnCount(const QModelIndex & /*parent*/) const override
        {return NbrColumn;}

    //! Renvoie la donnée correspondant à l'index et au role.
    QVariant data(const QModelIndex &index, int role) const override;

    //! Renvoie les drapeaux de l'index spécifié.
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Renvoie les labels des colonnes
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    //! Créer une nouvelle entité
    virtual bool hydrateNewEntity(MotCle &entity, int row = 0, const QModelIndex &parent = QModelIndex()) override;

    //! Renvoie l'identifiant de l'entité associée à la donnée.
    idt id(const QModelIndex & index) const;

    //! Accesseur du nombre de type.
    int nbrIndex() const;

    //! Supprime l'entité de la base de donnée si nécéssaire.
    bool removeEntity(const MotCle & entity) override;

    //! Enregistre les données.
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // TREEMODELMOTCLE_H
