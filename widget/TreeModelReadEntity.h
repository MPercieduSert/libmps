/*Auteur: PERCIE DU SERT Maxime
 *Date: 18/09/2018
 */
#ifndef TREEMODELREADENTITY_H
#define TREEMODELREADENTITY_H

#include "AbstractManager.h"
#include "Entity.h"
#include "TreeModelReadTemp.h"

namespace modelMPS {
using namespace typeMPS;
namespace bmps = bddMPS;
namespace emps = entityMPS;
/*! \ingroup groupeModel
 * \brief Classe template des models de type arbre non-modifiable pour les entités.
 */
template<class Ent> class TreeModelReadEntity: public TreeModelReadTemp<Ent> {
protected:
    std::vector<typename Ent::Position> m_atts;   //!< Liste des attributs à afficher.
    using TreeModelReadTemp<Ent>::m_tree;
    using TreeModelReadTemp<Ent>::getIter;

public:
    using TreeModelReadTemp<Ent>::createIndex;

    explicit TreeModelReadEntity(const std::vector<typename Ent::Position> & atts, QObject *parent = nullptr)
        : TreeModelReadTemp<Ent>(parent), m_atts(atts) {}

    //! Destructeur par defaut.
    ~TreeModelReadEntity() override = default;

    //! Renvoie le nombre de colonnes.
    int columnCount(const QModelIndex & /*parent*/) const override
        {return m_atts.empty() ? Ent::NbrAtt : m_atts.size();}

    //! Renvoie la donnée correspondant à l'index et au role.
    QVariant data(const QModelIndex &index, int role) const override;

    //! Renvoie un index sur le premier (dans le sens de parcourt) noeud de l'arbre ayant pour donnée un entité d'identifiant id.
    QItemSelection foundId(idt id) const;

    //! Renvoie les labels des colonnes
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

template<class Ent> QVariant TreeModelReadEntity<Ent>::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    auto iter = getIter(index);
    return m_atts.empty() ? iter->data(index.column()) : iter->data(m_atts.at(index.column()));
}

template<class Ent> QItemSelection TreeModelReadEntity<Ent>::foundId(idt id) const {
    auto i = m_tree.cbegin();
    while(i && i->id() != id )
        ++i;
    if(i != m_tree.end())
        return QItemSelection(createIndex(i.indexBrother(),0,*i),createIndex(i.indexBrother(),m_atts.size()-1,*i));
    else
        return QItemSelection();
}

template<class Ent> QVariant TreeModelReadEntity<Ent>::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return m_atts.empty() ? Ent::NameAttribut(section) : Ent::NameAttribut(m_atts.at(section));
    return QVariant();
}
}
#endif // TREEMODELREADENTITY_H
