/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/03/2019
 */
#ifndef TREEMODELTEMP_H
#define TREEMODELTEMP_H

#include "TreeModelReadTemp.h"

namespace modelMPS {
/*! \ingroup groupeModel
 * \brief Classe template des models de type d'arbre modifiable.
 */
template<class Ent> class TreeModelTemp : public TreeModelReadTemp<Ent>
{
public:
    //! Constructeur.
    explicit TreeModelTemp(QObject *parent = nullptr) : TreeModelReadTemp<Ent>(parent) {}

    //! Destructeur par defaut.
    ~TreeModelTemp() override = default;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
};
}
#endif // TREEMODELTEMP_H
