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
    NodeIndex index(const NodeIndex &parent, szt pos, int cible = NodeCible, szt num = 0) const override \
        {return TREE.index(parent,pos,cible,num);} \
    /*! Teste si le noeud associé à un index valide est une feuille.*/ \
    bool leaf(const NodeIndex & index) const override {return TREE.leaf(index);} \
    /*! Renvoie l'index du parent.*/ \
    NodeIndex parent(const NodeIndex &index) const override {return TREE.parent(index);} \
    /*! Renvoie le nombre de d'enfants.*/ \
    szt childCount(const NodeIndex &parent = NodeIndex()) const override {return TREE.childCount(parent);}

namespace modelMPS {
using namespace typeMPS;
class AbstractNodeModel;
//! Cible de donnée prédéfinies.
enum cibleDataNode {
    NodeCible = -1,
    SubNodeCible = -2
};

//! Role des données
enum roleDataNode {
    DataRole,
    OrientationRole,
    LabelRole
};

/*! \ingroup groupeModel
 * \brief Classe d'index d'un noeuds.
 */
class NodeIndex {
    friend AbstractNodeModel;
protected:
    int m_cible = NodeCible;                //!< Cible de l'index.
    szt m_num = 0;                              //!< Numéro.
    void * m_ptr = nullptr;                     //!< Pointeur interne sur sur la donnée du model.
    AbstractNodeModel * m_model = nullptr;      //!< Pointeur sur le model.
public:
    using SubIndex = std::pair<int,szt>;

    //! Constructeur.
    NodeIndex() = default;

    //! Accesseur de la cible.
    int cible() const noexcept
        {return m_cible;}

    //! Accesseur des données associées à l'index.
    QVariant data(int role = DataRole) const;

    //! Retourne un index sur le frère ainé.
    NodeIndex firstBrother() const noexcept;

    //! Drapeaux assossiés à une donnée.
    Qt::ItemFlags flags() const;

    //! Créer un index de même model et pointeur avec la cible et le numero spécifié.
    NodeIndex index(int cb, szt nm = 0) const noexcept {
        auto ind = *this;
        ind.setCible(cb);
        ind.setNum(nm);
        return ind;
    }

    //! Accesseur du pointeur interne.
    void * internalPointer() const noexcept
        {return m_ptr;}

    //! Teste si l'index est la racine.
    bool isRoot() const noexcept;

    //! Teste si l'index est valide.
    bool isValid() const noexcept
        {return m_model && m_ptr;}

    //! Retourne un index sur le frère benjamin.
    NodeIndex lastBrother() const noexcept;

    //! Teste si dans le model le noeud associé à l'index est une feuille.
    bool leaf() const;

    //! Accesseur du model.
    AbstractNodeModel * model() const noexcept
        {return m_model;}

    //! Retourne un index sur le frère suivant.
    NodeIndex nextBrother() const noexcept;

    //! Accesseur du numéro.
    szt num() const noexcept
        {return m_num;}

    //! Teste l'équivalence de deux index.
    bool operator==(const NodeIndex & index) const noexcept
        {return m_model == index.m_model && m_ptr == index.m_ptr & m_cible == index.m_cible && m_num == index.m_num;}

    //! Retourne un index sur le parent.
    NodeIndex parent() const;

    //! Retourne la position du noeud dans la fratrie.
    uint position() const noexcept;

    //! Retourne un index sur le frère précédent.
    NodeIndex prevBrother() const noexcept;

    //! Mutateur de la cible.
    void setCible(int cb) noexcept
        {m_cible = cb;}

    //! Mutateur de la cible.
    void setNum(szt num) noexcept
        {m_num = num;}

    //! Retourne le sous index.
    SubIndex subIndex() const noexcept
        {return {m_cible,m_num};}
};

template<class T> class TreeForNodeModel;
/*! \ingroup groupeModel
 * \brief Classe mère des models à noeuds.
 */
class AbstractNodeModel : public QObject{
    Q_OBJECT
    template<class T> friend class TreeForNodeModel;
protected:
    struct NodesInfo {
        NodeIndex parent;       //!< Index du parent des nodes insérés.
        szt first;              //!< Position du premier noeud insérés.
        szt last;               //!< Position du dernier noeud insérés.
    };
    std::forward_list<NodesInfo> m_insertNodesPile;       //!< Pile des informations d'insertion de noeuds.
    std::forward_list<NodesInfo> m_removeNodesPile;       //!< Pile des informations de suppression de noeuds.
    
public:
    using QObject::parent;
    //! Constructeur.
    AbstractNodeModel(QObject * parent = nullptr) : QObject(parent) {}

    //! Teste si l'index est valide pour ce model.
    bool checkIndex(const NodeIndex & index) const
        {return index.m_model == this && index.m_ptr;}

    //! Nombre d'enfants d'un noeud.
    virtual szt childCount(const NodeIndex & index) const = 0;

    //! Accesseur des données du model.
    virtual QVariant data(const NodeIndex & index, int role = DataRole) const = 0;

    //! Nombre de données associées associé à un noeud pour une cible donnée.
    virtual szt dataCount(const NodeIndex & index) const = 0;

    //! Retourne un index sur le frère ainé.
    virtual NodeIndex firstBrother(const NodeIndex & index) const = 0;

    //! Drapeaux assossiés à une donnée.
    virtual Qt::ItemFlags flags(const NodeIndex & index) const = 0;

    //! Index du fils de position pos de parent.
    virtual NodeIndex index(const NodeIndex & parent, szt pos, int cible = NodeCible, szt num = 0) const = 0;

    //! Insert count noeuds de nature type avant la position pos de parent.
    virtual bool insertNodes(const NodeIndex &parent, szt pos, szt count, int type) = 0;

    //! Test si l'index est la racine.
    virtual bool isRoot(const NodeIndex & index) const = 0;

    //! Retourne un index sur le frère benjamin.
    virtual NodeIndex lastBrother(const NodeIndex & index) const = 0;

    //! Teste si le noeud est une feuille.
    virtual bool leaf(const NodeIndex & index) const = 0;

    //! Retourne un index sur le frère suivant.
    virtual NodeIndex nextBrother(const NodeIndex & index) const = 0;

    //! Parent de l'index.
    virtual NodeIndex parent(const NodeIndex & index) const = 0;

    //! Position du noeud dans la fratrie.
    virtual szt position(const NodeIndex & index) const = 0;

    //! Retourne un index sur le frère suivant.
    virtual NodeIndex prevBrother(const NodeIndex & index) const = 0;

    //! Supprimer count noeud de la fratrie en commençant par le noeud node.
    virtual bool removeNodes(const NodeIndex &index, szt count = 1) = 0;

    //! Mutateur des données du model.
    virtual bool setData(const NodeIndex & index, const QVariant & value, int role = DataRole) = 0;

signals:
    //! Signal le changement d'une donnée.
    void dataChanged(const NodeIndex & index);

    //! Signal de début de réinitialisation du model.
    void modelAboutToBeReset();

    //! Signal de fin de réinitialisation du model.
    void modelReset();

    //! Signal de début d'insertion de noeuds.
    void nodesAboutToBeInserted(const NodeIndex &parent, szt first, szt last);

    //! Signal de début de suppression de noeuds.
    void nodesAboutToBeRemoved(const NodeIndex &parent, szt first, szt last);

    //! Signal de fin d'insertion de noeuds.
    void nodesInserted(const NodeIndex &parent, szt first, szt last);

    //! Signal de fin de suppression de noeuds.
    void nodesRemoved(const NodeIndex &parent, szt first, szt last);

protected:
    //! Début d'insertions de lignes
    void beginInsertNodes(const NodeIndex & parent, szt first, szt last) {
        m_insertNodesPile.push_front({parent,first,last});
        emit nodesAboutToBeInserted(parent,first,last);
    }

    //! Début de déplacement de lignes.
    bool beginMoveNodes(const NodeIndex & /*sourceParent*/, int /*sourceFirst*/, int /*sourceLast*/,
                       const NodeIndex & /*destinationParent*/, int /*destinationChild*/) {return false;}

    //! Début de suppression de lignes.
    void beginRemoveNodes(const NodeIndex & index, szt count) {
        auto pos = index.position();
        NodesInfo info({index.parent(),pos,pos + count - 1});
        m_removeNodesPile.push_front(info);
        emit nodesAboutToBeRemoved(info.parent,info.first,info.last);
    }

    //! Début de réinitialisation du model.
    void beginResetModel()
        {emit modelAboutToBeReset();}

    //! Crée un index de pointeur ptr.
    NodeIndex createIndex(void * ptr = nullptr, int cible = NodeCible, szt num = 0) const noexcept;

    //! Fin d'insertion de lignes.
    void endInsertNodes() {
        auto info = m_insertNodesPile.front();
        emit nodesInserted(info.parent,info.first,info.last);
        m_insertNodesPile.pop_front();
    }

    //! Fin de déplacement de lignes.
    void endMoveNodes() {}

    //! Fin de suppression de lignes.
    void endRemoveNodes() {
        auto info = m_removeNodesPile.front();
        emit nodesRemoved(info.parent,info.first,info.last);
        m_removeNodesPile.pop_front();
    }

    //! Fin de réinitialisation du model.
    void endResetModel() {emit modelReset();}
};

/////////////////////////////////////////// TreeForNodeModel ///////////////////////////////////////
template<class T> class TreeForNodeModel : public TempTreeForModel<T,AbstractNodeModel,NodeIndex> {
protected:
    using TFM = TempTreeForModel<T,AbstractNodeModel,NodeIndex>;
    using TFM::getValidIter;
    using TFM::m_model;
    using TFM::m_racine;
    using TFM::m_tree;
public:
    using TFM::getIter;
    //! Constructeur.
    using TempTreeForModel<T,AbstractNodeModel,NodeIndex>::TempTreeForModel;

    //! Renvoie l'index correxpondant à la ligne et colonne de parent.
    NodeIndex index(const NodeIndex &parent, szt pos, int cible = NodeCible, szt num = 0) const;

    //! Retourne un index sur le frère ainé.
    NodeIndex firstBrother(const NodeIndex & index) const
        {return index.isValid() ? m_model->createIndex(getValidIter(index).toFirstBrother().ptr())
                               : NodeIndex();}

    //! Test si l'index est la racine.
    bool isRoot(const NodeIndex & index) const
        {return index.isValid() ? getValidIter(index).root()
                       : true;}

    //! Retourne un index sur le frère ainé.
    NodeIndex lastBrother(const NodeIndex & index) const
        {return index.isValid() ? m_model->createIndex(getValidIter(index).toLastBrother().ptr())
                               : NodeIndex();}

    //! Retourne un index sur le frère précédent.
    NodeIndex nextBrother(const NodeIndex & index) const
        {return index.isValid() ? m_model->createIndex(getValidIter(index).toNextBrother().ptr())
                               : NodeIndex();}

    //! Retourne un index sur le frère suivant.
    NodeIndex prevBrother(const NodeIndex & index) const
        {return index.isValid() ? m_model->createIndex(getValidIter(index).toPrevBrother().ptr())
                               : NodeIndex();}

    //! Renvoie l'index du parent.
    NodeIndex parent(const NodeIndex & index) const;
};

//////////////////////////////////////// TreeNodeModel //////////////////////////////////////
class TreeNodeModel : public AbstractNodeModel {
    Q_OBJECT
public:
    enum {NoType = -1,
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
        virtual QVariant data(int cible, int role = DataRole, szt /*num*/ = 0) const;

        //! Nombre de donnée associé au noeud pour un type donnée.
        virtual szt dataCount(int cible) const
            {return cible == NodeCible ? 1 : NoData;}

        //! Accesseur des drapeaux associés à column.
        virtual Qt::ItemFlags flags(int /*cible*/, szt /*num*/ = 0) const {return  Qt::NoItemFlags;}

        //! Mutateur de la donnée associé à column.
        virtual bool setData(int /*cible*/, const QVariant & /*value*/, int /*role*/ = DataRole, szt /*num*/ = 0) {return true;}

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
    QVariant data(const NodeIndex &index, int role = DataRole) const override;

    //! Nombre de données associées associé à un noeud pour une cible donnée.
    szt dataCount(const NodeIndex & index) const override;

    //! Retourne un index sur le frère ainé.
    NodeIndex firstBrother(const NodeIndex & index) const override
        {return m_data.firstBrother(index);}

    //! Renvoie les drapeaux associé à un index.
    Qt::ItemFlags flags(const NodeIndex &index) const override;

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const AbstractNode & getData(const NodeIndex &index) const
        {return **m_data.getIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    AbstractNode & getData(const NodeIndex &index)
        {return **m_data.getIter(index);}

    //! Insert count noeuds de nature type avant la ligne row de parent.
    bool insertNodes(const NodeIndex &parent, szt pos, szt count, int type) override;

    //! Test si l'index est la racine.
    bool isRoot(const NodeIndex & index) const override
        {return m_data.isRoot(index);}

    //! Retourne un index sur le frère benjamin.
    NodeIndex lastBrother(const NodeIndex & index) const override
        {return m_data.lastBrother(index);}

    //! Retourne un index sur le frère suivant.
    NodeIndex nextBrother(const NodeIndex & index) const override
        {return m_data.nextBrother(index);}

    //! Position du noeud dans la fratrie.
    szt position(const NodeIndex & index) const override
        {return m_data.position(index);}

    //! Retourne un index sur le frère suivant.
    NodeIndex prevBrother(const NodeIndex & index) const override
        {return m_data.prevBrother(index);}

    //! Supprimer count ligne en commençant par la ligne index.
    bool removeNodes(const NodeIndex & index, szt count = 1) override;

    //! Mutateur la donnée associé à un couple (index,role).
    bool setData(const NodeIndex &index, const QVariant &value, int role = DataRole) override;

protected:
    //! Fabrique des noeuds.
    virtual Node nodeFactory(const NodeIndex & /*parent*/, szt /*pos*/, int /*type*/) {return std::make_unique<AbstractNode>();}
};

///////////////////////////////////// TreeForModel //////////////////////////////////
template<class T> NodeIndex TreeForNodeModel<T>::index(const NodeIndex &parent, szt pos, int cible, szt num) const {
    if(parent.isValid()) {
        auto iter = getValidIter(parent);
        iter.toChildU(pos);
        if (iter)
            return m_model->createIndex(iter.ptr(),cible,num);
    }
    else if (!m_racine || pos == 0){
        auto iter = m_tree.cbegin().toChildU(pos);
        if (iter)
            return m_model->createIndex(iter.ptr(),cible,num);
    }
    return NodeIndex();
}
template<class T> NodeIndex TreeForNodeModel<T>::parent(const NodeIndex &index) const {
    if (index.isValid()) {
        auto iter = getValidIter(index);
        iter.toParent();
        if(iter && !iter.root())
            return m_model->createIndex(iter.ptr());
    }
    return NodeIndex();
}
}
#endif // ABSTRACTNODEMODEL_H
