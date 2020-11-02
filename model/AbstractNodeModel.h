/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef ABSTRACTNODEMODEL_H
#define ABSTRACTNODEMODEL_H

#include <memory>
#include <QObject>
#include <QVariant>
#include "AbstractModel.h"

//! Macro d'inclusion des membres virtual dans le model à node.
#define TREE_FOR_NODE_MODEL_INDEX_PARENT_ROWCOUNT(TREE) /*! Renvoie l'index correxpondant à la ligne et colonne de parent.*/ \
    NodeIndex index(int row, const NodeIndex &parent = NodeIndex()) const override \
        {return TREE.index(row,parent);} \
    /*! Renvoie l'index du parent.*/ \
    NodeIndex parent(const NodeIndex &index) const override {return TREE.parent(index);} \
    /*! Renvoie le nombre de d'enfants.*/ \
    int rowCount(const NodeIndex &parent = NodeIndex()) const override {return TREE.rowCount(parent);}

namespace modelMPS {
using namespace typeMPS;
class AbstractNodeModel;
//! Donnée types prédéfinies.
enum typeDataNode {
    NodeType = -1
};

//! Role des données
enum roleDataNode {
    progRole = -1
};


/*! \ingroup groupeModel
 * \brief Classe d'index d'un noeuds.
 */
class NodeIndex {
    friend AbstractNodeModel;
protected:
    void * m_ptr = nullptr;                     //!< Pointeur interne sur sur la donnée du model.
    int m_row = -1;                             //!< Position dans la fratrie.
    AbstractNodeModel * m_model = nullptr;      //!< Pointeur sur le model.
public:
    //! Constructeur.
    NodeIndex() = default;

    //! Accesseur des données associées à l'index.
    QVariant data(int type, int role = Qt::DisplayRole, szt num = 0) const;

    //! Drapeaux assossiés à une donnée.
    Qt::ItemFlags flags(int type, szt num = 0) const;

    //! Accesseur du pointeur interne.
    void * internalPointer() const noexcept
        {return m_ptr;}

    //! Teste si l'index est valide.
    bool isValid() const noexcept
        {return m_model && m_row >= 0;}

    //! Accesseur du model.
    AbstractNodeModel * model() const noexcept
        {return m_model;}

    //! Retourne un index sur le parent.
    NodeIndex parent() const;
};

template<class T> class TreeForNodeModel;
/*! \ingroup groupeModel
 * \brief Classe mère des models à noeuds.
 */
class AbstractNodeModel : public QObject{
    Q_OBJECT
    template<class T> friend class TreeForNodeModel;
public:
    using QObject::parent;
    //! Constructeur.
    AbstractNodeModel(QObject * parent = nullptr) : QObject(parent) {}

    //! Teste si l'index est valide pour ce model.
    bool checkIndex(const NodeIndex & index) const
        {return index.m_model == this && index.m_ptr && index.m_row >= 0 && index.m_row < rowCount(index);}

    //! Accesseur des données du model.
    virtual QVariant data(const NodeIndex & index, int type, int role = Qt::DisplayRole, szt num = 0) const = 0;

    //! Nombre de donnée d'un noeud et d'un type.
    virtual szt dataCount(const NodeIndex & index, int type) const = 0;

    //! Drapeaux assossiés à une donnée.
    virtual Qt::ItemFlags flags(const NodeIndex & index, int type, szt num = 0) const = 0;

    //! Index du fils numéro num de parent.
    virtual NodeIndex index(int row, const NodeIndex & parent = NodeIndex()) const = 0;

    //! Insert count noeuds de nature type avant la ligne row de parent.
    virtual bool insertNodes(int type, int row, int count, const NodeIndex &parent = NodeIndex()) = 0;

    //! Parent de l'index.
    virtual NodeIndex parent(const NodeIndex & index) const = 0;

    //! Supprimer count ligne en commençant par la ligne row.
    virtual bool removeRows(int row, int count, const NodeIndex &parent = NodeIndex()) = 0;

    //! Nombre d'enfants d'un noeud.
    virtual int rowCount(const NodeIndex & index) const = 0;

    //! Mutateur des données du model.
    virtual bool setData(const NodeIndex & index, int type, const QVariant & value, int role = Qt::EditRole, szt num = 0) = 0;

signals:
    //! Signal le changement d'une donnée.
    void dataChanged(const NodeIndex & index, int type, szt num);

    //! Signal le changement d'une donnée.
    void dataChanged(const NodeIndex & index);

    //! Signal la réinitialisation du model.
    void modelReset();

protected:
    //! Début d'insertions de lignes
    void beginInsertRows(const NodeIndex & /*parent*/, int /*first*/, int /*last*/) {}

    //! Début de déplacement de lignes.
    bool beginMoveRows(const NodeIndex & /*sourceParent*/, int /*sourceFirst*/, int /*sourceLast*/,
                       const NodeIndex & /*destinationParent*/, int /*destinationChild*/) {return false;}

    //! Début de suppression de lignes.
    void beginRemoveRows(const NodeIndex & /*parent*/, int /*first*/, int /*last*/) {}

    //! Début de réinitialisation du model.
    void beginResetModel() {}

    //! Crée un index sur le fils numéro row de parent.
    NodeIndex createIndex(int row, void * ptr = nullptr) const;

    //! Fin d'insertion de lignes.
    void endInsertRows() {}

    //! Fin de déplacement de lignes.
    void endMoveRows() {}

    //! Fin de suppression de lignes.
    void endRemoveRows() {}

    //! Fin de réinitialisation du model.
    void endResetModel() {}
};

/////////////////////////////////////////// TreeForNodeModel ///////////////////////////////////////
template<class T> class TreeForNodeModel : public TempTreeForModel<T,AbstractNodeModel,NodeIndex> {
protected:
    using TFM = TempTreeForModel<T,AbstractNodeModel,NodeIndex>;
    using TFM::m_model;
    using TFM::m_racine;
    using TFM::m_tree;
public:
    using TFM::getIter;
    //! Constructeur.
    using TempTreeForModel<T,AbstractNodeModel,NodeIndex>::TempTreeForModel;

    //! Renvoie l'index correxpondant à la ligne et colonne de parent.
    NodeIndex index(int row, const NodeIndex &parent = NodeIndex()) const;
};

//////////////////////////////////////// TreeNodeModel //////////////////////////////////////
class TreeNodeModel : public AbstractNodeModel {
    Q_OBJECT
public:
    enum {NoType = -1,
          NodeType = -1,
          NoData = 0
         };

    //! Node abstré de l'arbre de recherche.
    class AbstractNode {
    protected:
        int m_type;     //! type du noeud.
    public:
        //! Constructeur.
        AbstractNode(int type = NoType) : m_type(type){}

        virtual ~AbstractNode();

        //! Accesseur de la donnée associé à column.
        virtual QVariant data(int tp, int role = Qt::DisplayRole, szt /*num*/ = 0) const {
            if(tp == NodeType && role == progRole)
                return type();
            return QVariant();}

        //! Nombre de donnée associé au noeud pour un type donnée.
        virtual szt dataCount(int tp) const
            {return tp == NodeType ? 1 : NoData;}

        //! Accesseur des drapeaux associés à column.
        virtual Qt::ItemFlags flags(int /*tp*/, szt /*num*/ = 0) const {return  Qt::NoItemFlags;}

        //! Mutateur de la donnée associé à column.
        virtual bool setData(int /*tp*/, const QVariant & /*value*/, int /*role*/ = Qt::EditRole, szt /*num*/ = 0) {return true;}

        //! Accesseur du type du noeud.
        int type() const {return m_type;}
    };
    //! Classe des noeud de l'arbre de données
    using Node = std::unique_ptr<AbstractNode>;
    //! Classe structurant les données.
    using Tree = conteneurMPS::tree<Node>;
    //! Classe du protomodel.
    using Data = TreeForNodeModel<Node>;
protected:
    Data m_data;                            //!< Arbre de données du model.
public:
    TREE_FOR_NODE_MODEL_INDEX_PARENT_ROWCOUNT(m_data)    // Implémentation des méthodes virtuelles index, parent, rowCount.

    //!Constructeur.
    TreeNodeModel(bool racine, QObject * parent);

    //! Accesseur la donnée associé à un couple (index,role).
    QVariant data(const NodeIndex &index, int type, int role = Qt::DisplayRole, szt num = 0) const override;

    //! Nombre de données associées associé à un noeud pour un type donnée.
    szt dataCount(const NodeIndex & index, int type) const override;

    //! Renvoie les drapeaux associé à un index.
    Qt::ItemFlags flags(const NodeIndex &index, int type, szt num = 0) const override;

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const AbstractNode & getData(const NodeIndex &index) const
        {return **m_data.getIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    AbstractNode & getData(const NodeIndex &index)
        {return **m_data.getIter(index);}

    //! Insert count noeuds de nature type avant la ligne row de parent.
    bool insertNodes(int type, int row, int count, const NodeIndex &parent = NodeIndex()) override;

    //! Supprimer count ligne en commençant par la ligne row.
    bool removeRows(int row, int count, const NodeIndex &parent = NodeIndex()) override;

    //! Mutateur la donnée associé à un couple (index,role).
    bool setData(const NodeIndex &index, int type, const QVariant &value, int role = Qt::EditRole, szt num = 0) override;

protected:
    //! Fabrique des noeuds.
    virtual Node nodeFactory(int /*type*/, int /*row*/, const NodeIndex & /*parent*/) {return std::make_unique<AbstractNode>();}
};

///////////////////////////////////// TreeForModel //////////////////////////////////
template<class T> NodeIndex TreeForNodeModel<T>::index(int row, const NodeIndex &parent) const {
    if(parent.isValid()) {
        auto iter = getIter(parent);
        iter.toChild(row);
        if (iter)
            return m_model->createIndex(row, iter.ptr());
    }
    else if (!m_racine || row == 0){
        auto iter = m_tree.cbegin().toChild(row);
        if (iter)
            return m_model->createIndex(row, iter.ptr());
    }
    return NodeIndex();
}
}
#endif // ABSTRACTNODEMODEL_H
