/*Auteur: PERCIE DU SERT Maxime
 *Date: 05/05/2020
 */
#ifndef TREENODEMODEL_H
#define TREENODEMODEL_H

#include <memory>
#include "AbstractModel.h"

namespace modelMPS {
using namespace typeMPS;

class TreeNodeModel : public AbstractModel {
    Q_OBJECT
public:
    enum {NoType = -1};

    //! Noeud abstré de l'arbre de recherche.
    class AbstractNode {
    protected:
        int m_type;     //! type du noeud.
    public:
        //! Constructeur.
        AbstractNode(int type = NoType) : m_type(type){}

        virtual ~AbstractNode();

        //! Nombre de colonne des descendants.
        virtual int childColumnCount() const {return 0;}

        //! Accesseur de la donnée associé à column.
        virtual QVariant data(int /*column*/, int /*role*/ = Qt::DisplayRole) const {return QVariant();}

        //! Accesseur des drapeaux associés à column.
        virtual Qt::ItemFlags flags(int /*column*/) const {return  Qt::NoItemFlags;}

        //! Mutateur de la donnée associé à column.
        virtual bool setData(int /*column*/, const QVariant & /*value*/, int /*role*/ = Qt::EditRole) {return true;}

        //! Accesseur du type du noeud.
        int type() const {return m_type;}
    };
    //! Classe des noeud de l'arbre de données
    using Node = std::unique_ptr<AbstractNode>;
    //! Classe structurant les données.
    using Tree = conteneurMPS::tree<Node>;
    //! Classe du protomodel.
    using Data = TreeForModel<Node>;
protected:
    Data m_data;                            //!< Arbre de données du model.
    std::vector<QString> m_header;          //!< Entête des colonnes.      
public:
    TREE_FOR_MODEL_INDEX_PARENT_ROWCOUNT(m_data)    // Implémentation des méthodes virtuelles index, parent, rowCount.

    //!Constructeur.
    TreeNodeModel(bool racine, QObject * parent);

    //! Accesseur du nombre de colonne des descendant.
    int columnCount(const QModelIndex & parent) const override;

    //! Accesseur la donnée associé à un couple (index,role).
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    //! Renvoie les drapeaux associé à un index.
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const AbstractNode & getData(const QModelIndex &index) const
        {return **m_data.getIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    AbstractNode & getData(const QModelIndex &index)
        {return **m_data.getIter(index);}

    //! Accesseur des entêtes des colonnes.
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        if(orientation == Qt::Horizontal && role == Qt::DisplayRole
                && 0 <= section  && section < static_cast<int>(m_header.size()))
            return m_header[static_cast<szt>(section)];
        return QVariant();
    }

    bool insertNodes(int type, int row, int count, const QModelIndex &parent = QModelIndex());

    //! Mutateur la donnée associé à un couple (index,role).
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

protected:
    //! Fabrique des noeuds.
    virtual Node nodeFactory(int /*type*/, int /*row*/, const QModelIndex & /*parent*/) {return std::make_unique<AbstractNode>();}
};
}
#endif // TREENODEMODEL_H
