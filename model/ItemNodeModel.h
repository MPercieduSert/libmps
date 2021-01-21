/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef ITEMNODEMODEL_H
#define ITEMNODEMODEL_H

#include <memory>
#include <QObject>
#include <QVariant>
#include <set>
#include "Bdd.h"
#include "Tree.h"
#include "typemps.h"

//! \ingroup groupeModel
//! Copie l'état du noeud.
#define NODE_COPIE(TYPE) /*! Copie l'état du noeud */ \
    Node copie() const override {return std::make_unique<TYPE>(*this);}

namespace modelMPS {
using namespace typeMPS;
class ItemNode;
class ItemNodeModel;
class TreeForNodeModel;
//! Classe des noeud de l'arbre de données
using Node = std::unique_ptr<ItemNode>;
//! Classe structurant les données.
using TreeNode = conteneurMPS::tree<Node>;
//! Iterateur (const) sur les noeuds de l'arbre.
using IterNode = TreeNode::const_iterator;

//! Cible de donnée prédéfinies.
enum cibleDataNode {
    NodeCible = -1,
    SubNodeCible = -2,
    ExterneCible = -3
};

//! Role des données
enum roleNode : flag::flag_type {
    NoRole =0x0,
    //Forme Role
    ToolTipRole = 0x1,                  //!< Info bulle (QString)
    StatusTipRole = 0x2,                //!< Info dans la barre de tache (QString)
    WhatThisRole = 0x4,                 //!< Info dans "What's this" (QString)
    FontRole = 0x8,                     //!< Police d'affichage principale (QFont)
    TexteAlignementRole = 0x10,         //!< Alignement du texte principale (Qt:Alignment)
    BackgroundRole = 0x20,              //!< Fond du texte principale (QBrush)
    ForegroundRole = 0x40,              //!< Rendu du texte principale (QBrush)
    FormAllRole = ToolTipRole | StatusTipRole | WhatThisRole | FontRole | TexteAlignementRole | BackgroundRole | ForegroundRole,
    //Label Role
    LabelRole = 0x80,                   //!< Titre (QString)
    FontLabelRole = 0x100,              //!< Police du titre (QFont)
    AlignementLabelRole = 0x200,        //!< Alignement du texte du label (Qt:Alignment)
    BackgroundLabelRole = 0x400,        //!< Fond du label (QBrush)
    ForegroundLabelRole = 0x800,        //!< Rendu du label (QBrush)
    LabelAllRole = LabelRole | FontLabelRole | AlignementLabelRole | BackgroundLabelRole | ForegroundLabelRole,
    //Configuration Role
    FlagRole = 0x1000,                  //!< Code (flag)
    OrientationRole = 0x2000,           //!< Orientation (Qt::Orientation)
    TypeRole = 0x4000,                  //!< Type des donnée (int)
    SubNodeRole = 0x8000,               //!< Initialisation d'un sous-noeud (QList<QVariant>(Cible,Num,TypeSubNode)).
    ListOfValues = 0x10000,             //!< Liste des choix possible (QMap<QString,QVariant>)
    ConfigAllRole = FlagRole | OrientationRole | TypeRole | ListOfValues,
    // Donnée Principale role
    StringRole = 0x20000,               //!< Donnée principale sous forme de texte (QString)
    CheckStateRole = 0x40000,           //!< Donnée principale sous forme d'état des cases à cocher (Qt::CheckState)
    DateRole = 0x8000,                  //!< Donnée principale sous forme de date (QDate)
    DateTimeRole = 0x100000,            //!< Donnée principale sous forme de date et horaire (QDateTime)
    TimeRole = 0x200000,                //!< Donnée principale sous forme d'horraire (QTime)
    IntRole = 0x400000,                 //!< Donnée principale sous forme d'un entier (int)
    NumRole = 0x800000,                 //!< Donnée principale sous forme d'un numéro (numt)
    VariantRole = 0x1000000,            //!< Donnée principale sous forme d'un variant (QVariant)
    BoolRole = 0x2000000,               //!< Donnée principale sous forme d'un booléen (bool)
    DoubleRole = 0x4000000,             //!< Donnée principale sous forme d'un double (double)
    DecorationRole = 0x8000000,         //!< Donnée principale sous forme d'image (QColor,QIcon,QPixmap)
    StringListRole = 0x10000000,        //!< Donnée principale sous forme d'une liste de texte (QStringlist)
    ListRole = 0x20000000,              //!< Donnée principale sous forme d'une liste de variant (QList<QVariant>)
    MapRole = 0x40000000,               //!< Donnée principlae sous forme d'une map (QMap<QString,QVariant>)
    MainAllRole = StringRole | CheckStateRole | DateRole | DateTimeRole | TimeRole | IntRole | NumRole
                    | VariantRole | BoolRole | DoubleRole
                    | DecorationRole | StringListRole | ListRole | MapRole,
    AllRole = FormAllRole | LabelAllRole | ConfigAllRole | MainAllRole
};

//! Types des sous-noeuds.
enum subNodeType {
    CheckSubNode,
    CodeSubNode,
    ComboBoxSubNode,
    DateSubNode,
    LineEditSubNode,
    TexteEditSubNode
};

//! Initialisation d'un sous-noeud.
enum initSubNode {
    CibleSubNode,
    NumSubNode,
    TypeSubNode,
    NbrInitiSubNode
};

//! flag du model
enum flagNode : flag::flag_type {
    NoFlagNode = 0x0,
    VisibleFlagNode = 0x1,
    EnableFlagNode = 0x2,
    LeftClickableFlagNode = 0x4,
    SelectableFlagNode = 0x8,
    ExpendableFLagNode = 0x10,
    ElderEnableFlagNode = 0x20,
    BrotherEnableFlagNode = 0x40,
    DelEnableFlagNode = 0x80,
    DefaultFalgNode = VisibleFlagNode | EnableFlagNode | LeftClickableFlagNode | SelectableFlagNode,
    DefaultRootFlagNode = DefaultFalgNode | ExpendableFLagNode | ElderEnableFlagNode,
    DefaultNodeFlagNode = DefaultRootFlagNode | BrotherEnableFlagNode | DelEnableFlagNode
};

/*! \ingroup groupeModel
 * \brief Classe mère des noeuds.
 */
class ItemNode {
    friend TreeForNodeModel;
protected:
    IterNode m_iter = static_cast<void *>(nullptr);     //! Iterateur sur le noeud de l'arbre contenant ce noeud.
    int m_type;                                         //! type du noeud.
public:
    enum {NoType = -1,
          NoData = 0};
    //! Constructeur.
    ItemNode(int type = NoType) : m_type(type){}

    //! Destructeur.
    virtual ~ItemNode();

    //! Copie l'état du noeud
    virtual Node copie() const
        {return std::make_unique<ItemNode>(*this);}

    //! Accesseur de la donnée associé à column.
    virtual QVariant data(int cible, int role, numt /*num*/ = 0) const;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    virtual numt dataCount(int cible) const
        {return cible == NodeCible ? 1 : NoData;}

    //! Supprime les donnée du noeud.
    virtual bool del(/*bddMPS::Bdd &*/) {return true;}

    //! Accesseur des drapeaux associés à column.
    virtual flag flags(int cible, numt /*num*/ = 0) const {
        if(cible == NodeCible){
            if(m_iter.root())
                return DefaultRootFlagNode;
            return DefaultNodeFlagNode;
        }
        return DefaultFalgNode;
    }

    //! Mutateur de la donnée associé à column.
    virtual flag setData(int /*cible*/, const QVariant & /*value*/, int /*role*/, numt /*num*/ = 0) {return NoRole;}

    //! Accesseur du type du noeud.
    int type() const {return m_type;}
};

/*! \ingroup groupeModel
 * \brief Classe d'index d'un noeuds.
 */
class NodeIndex {
    friend ItemNodeModel;
    friend TreeForNodeModel;
protected:
    int m_cible = NodeCible;                            //!< Cible de l'index.
    numt m_num = 0;                                     //!< Numéro.
    IterNode m_iter = static_cast<void *>(nullptr);     //!< Itérateur pointant sur un noeud du model.
    ItemNodeModel * m_model = nullptr;                  //!< Pointeur sur le model.
    enum {RootCount = 1};
public:
    using SubIndex = std::pair<int,numt>;

    //! Constructeur.
    NodeIndex() = default;

    //! Renvoie le nombre de d'enfants.
    numt childCount() const noexcept {
        if(!isValid())
            return RootCount;
        return m_iter.sizeChild();
    }

    //! Accesseur de la cible.
    int cible() const noexcept
        {return m_cible;}

    //! Accesseur des données associées à l'index.
    QVariant data(int role) const;

    //! Retourne un index sur le frère ainé.
    NodeIndex firstBrother() const noexcept;

    //! Drapeaux assossiés à une donnée.
    flag flags() const;

    //! Créer un index de même model et pointeur avec la cible et le numero spécifié.
    NodeIndex index(int cb, numt nm = 0) const noexcept {
        auto ind = *this;
        ind.setCible(cb);
        ind.setNum(nm);
        return ind;
    }

    //! Accesseur du pointeur interne.
    void * internalPointer() const noexcept
        {return m_iter.ptr();}

    //! Teste si l'index est la racine.
    bool isRoot() const
        {return m_iter.root();}

    //! Teste si l'index est valide.
    bool isValid() const noexcept
        {return m_iter;}

    //! Retourne un index sur le frère benjamin.
    NodeIndex lastBrother() const noexcept;

    //! Teste si dans le model le noeud associé à l'index est une feuille.
    bool leaf() const
        {return m_iter.leaf();}

    //! Accesseur du model.
    ItemNodeModel * model() const noexcept
        {return m_model;}

    //! Retourne un index sur le frère suivant.
    NodeIndex nextBrother() const noexcept;

    //! Accesseur du numéro.
    numt num() const noexcept
        {return m_num;}

    //! Teste l'équivalence de deux index.
    bool operator==(const NodeIndex & index) const noexcept
        {return m_model == index.m_model && m_iter == index.m_iter && m_cible == index.m_cible && m_num == index.m_num;}

    //! Retourne un index sur le parent.
    NodeIndex parent() const noexcept;

    //! Retourne la position du noeud dans la fratrie.
    numt position() const
        {return m_iter.position() ;}

    //! Retourne un index sur le frère précédent.
    NodeIndex prevBrother() const noexcept;

    //! Mutateur de la cible.
    void setCible(int cb) noexcept
        {m_cible = cb;}

    //! Mutateur de la cible.
    void setNum(numt num) noexcept
        {m_num = num;}

    //! Retourne le sous index.
    SubIndex subIndex() const noexcept
        {return {m_cible,m_num};}

protected:
    //! Accesseur (par valeur) de l'itérateur.
    IterNode iter() const noexcept
        {return m_iter;}
};

/*! \ingroup groupeModel
 * \brief Arbre de noeuds pour les models à noeud.
 */
class TreeForNodeModel {
protected:
    //! Poiteur constant sur les noeuds de l'arbre.
    using const_iterator = TreeNode::const_iterator;
    //! Poiteur sur les noeuds de l'arbre.
    using iterator = TreeNode::iterator;

    TreeNode m_tree;                //!< Arbre de donnée.
public:
    //! Iterateur sur la racine.
    const_iterator cbegin() const noexcept
        {return m_tree.cbegin();}

    //! Iterateur sur la racine.
    TreeNode::const_reverse_iterator crbegin() const noexcept
        {return m_tree.crbegin();}

    //! Efface un noeud de l'arbre.
    void erase(const NodeIndex & index)
        {m_tree.erase(index.m_iter);}

    //! Efface les count frère de index.
    void erase(const NodeIndex & index, numt count)
        {m_tree.erase(index.m_iter,index.iter().toBrotherU(count));}

    //! Renvoie une référence sur le noeud coorespondant à l'index (en supposant sa validité).
    const ItemNode & getNode(const NodeIndex &index) const
        {return **index.m_iter;}

    //! Renvoie une référence sur le noeud coorespondant à l'index (en supposant sa validité).
    ItemNode & getNode(const NodeIndex &index)
        {return **index.m_iter;}

    //! Renvoie une référence sur le noeud de la racine.
    const ItemNode & getRoot() const
        {return **m_tree.cbegin();}

    //! Renvoie une référence sur le noeud de la racine.
    ItemNode & getRoot()
        {return **m_tree.begin();}

    //! Insert des noeud du model, ne vérifie pas les arguments.
    template<class Factory> std::list<IterNode> insertNodes(const NodeIndex &parent, numt pos, numt count, Factory factory);

    //! Remplace le noeud index par node.
    Node moveNode(const NodeIndex & index, Node && node);

    //! Déplace le noeud d'index from à la position to, le noeud de position from devient invalide.
    Node moveNode(const NodeIndex & from, const NodeIndex & to);

    //! Déplace le noeud d'index from à la position to, le noeud de position from devient invalide.
    void move(const NodeIndex & from, const NodeIndex & to)
        {setNode(to,takeNode(from));}

    //! Ajoute un fils cadet au noeud index.
    IterNode push_back(const NodeIndex & index, Node && node)
        {return updateIter(m_tree.push_back(index.m_iter,std::move(node)));}

    //! Ajoute un fils ainé au noeud index.
    IterNode push_front(const NodeIndex & index, Node && node)
        {return updateIter(m_tree.push_front(index.m_iter,std::move(node)));}

    //! Supprime des noeuds du model.
    bool removeNodes(const NodeIndex & parent, numt pos, numt count);

    //! Modifie un noeud de l'arbre.
    void setNode(const NodeIndex & index, Node && node) {
        *static_cast<iterator>(index.m_iter) = std::move(node);
        updateIter(index.m_iter);
    }

    //! Modifie l'arbre de donnée avec F de signature Node (const U &).
    template<class U, class F>  void setTree(const conteneurMPS::tree<U> & t, F usine);

    //! Modifie l'arbre de donnée.
    void setRoot(Node && node) {
        m_tree.clear();
        *m_tree.begin() = std::move(node);
        updateIter(m_tree.cbegin());
    }

    //! Prend le noeud index, ce noeud devient invalide.
    Node takeNode(const NodeIndex & index)
        {return std::move(*static_cast<iterator>(index.m_iter));}

protected:
    //! Met à jour l'itérateur du noeud pointé par l'itérateur.
    IterNode updateIter(IterNode iter) const
        {return (**iter).m_iter = iter;}
};

/*! \ingroup groupeModel
 * \brief Classe mère des models à noeuds.
 */
class ItemNodeModel : public QObject{
    Q_OBJECT
protected:
    //! Type de modification.
    enum changeType {
        SetData,
        InsertNode,
        RemoveNode
    };
    enum {AucunePosition = 0};
    //! Information sur la position d'un noeud
    struct NodesInfo {
        NodeIndex parent;       //!< Index du parent des noeuds.
        numt pos;               //!< Position du premier noeud.
        numt count;             //!< Nombre de noeuds concerné.
    };
    //! Information sur les modifications apporté à un noeud.
    struct Change {
        numt type;
        numt pos;
        numt count;
        IterNode iter = static_cast<void *>(nullptr);
        Node node;
    };
    std::list<Change> m_changeList;                         //!< Liste des modifications du model.
    std::forward_list<NodesInfo> m_insertNodesPile;         //!< Pile des informations d'insertion de noeuds.
    std::forward_list<NodesInfo> m_removeNodesPile;         //!< Pile des informations de suppression de noeuds.
    TreeForNodeModel m_data;                                //!< Arbre de données.

public:
    enum {DefaultType = -2};
    using QObject::parent;
    //! Constructeur.
    ItemNodeModel(QObject * parent = nullptr) : QObject(parent) {}

    //! Teste si l'index est valide pour ce model.
    bool checkIndex(const NodeIndex & index) const
        {return index.m_model == this && index.m_iter;}

    //! Accesseur des données du model.
    virtual QVariant data(const NodeIndex & index, int role) const;

    //! Nombre de données associées associé à un noeud pour une cible donnée.
    virtual numt dataCount(const NodeIndex & index) const;

    //! Drapeaux assossiés à une donnée.
    virtual flag flags(const NodeIndex & index) const;

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité de l'index).
    const ItemNode & getNode(const NodeIndex &index) const
        {return m_data.getNode(index);}

    //! Renvoie l'index correxpondant au noeud pos de parent.
    NodeIndex index(const NodeIndex &parent, numt pos, int cible = NodeCible, numt num = 0) const;

    //! Insert count noeuds de nature type avant la position pos de parent.
    void insertNodes(const NodeIndex &parent, numt pos, numt count, int type = DefaultType) {
        auto list = insert(parent,pos,count,type);
        for (auto iter = list.cbegin(); iter != list.cend(); ++iter) {
            Change change;
            change.type = InsertNode;
            change.iter = *iter;
            m_changeList.push_back(std::move(change));
        }
    }

    //! Supprimer count noeud de la fratrie en commençant par le noeud node.
    bool removeNodes(const NodeIndex &index, numt count = 1)
        {return remove(index,count);}

    //! Mutateur des données du model.
    bool setData(const NodeIndex & index, const QVariant & value, int role) {
        Change change;
        change.type = SetData;
        change.iter = indexToIterator(index);
        change.node = getNode(index).copie();
        if(set(index,value,role)) {
            m_changeList.push_back(std::move(change));
            return true;
        }
        return false;
    }

signals:
    //! Signal le changement d'une donnée.
    void dataChanged(const NodeIndex & index, flag role);

    //! Signal de début de réinitialisation du model.
    void modelAboutToBeReset();

    //! Signal de début de réinitialisation des données de l'arbre mais conservant la structure.
    void modelAboutToResetData();

    //! Signal de fin de réinitialisation du model.
    void modelReset();

    //! Signal de réinitialisation des données de l'arbre mais conservant la structure.
    void modelResetData();

    //! Signal de début d'insertion de noeuds.
    void nodesAboutToBeInserted(const NodeIndex &parent, numt pos, numt count);

    //! Signal de début de suppression de noeuds.
    void nodesAboutToBeRemoved(const NodeIndex &parent, numt pos, numt count);

    //! Signal de fin d'insertion de noeuds.
    void nodesInserted(const NodeIndex &parent, numt pos, numt count);

    //! Signal de fin de suppression de noeuds.
    void nodesRemoved(const NodeIndex &parent, numt pos, numt count);

protected:
    //! Début d'insertions de noeuds.
    void beginInsertNodes(const NodeIndex & parent, numt pos, numt count) {
        m_insertNodesPile.push_front({parent,pos,count});
        emit nodesAboutToBeInserted(parent,pos,count);
    }

    //! Début de déplacement de lignes.
    bool beginMoveNodes(const NodeIndex & /*sourceParent*/, int /*sourceFirst*/, int /*sourceLast*/,
                       const NodeIndex & /*destinationParent*/, int /*destinationChild*/) {return false;}

    //! Début de suppression de lignes [first,last).
    void beginRemoveNodes(const NodeIndex & index, numt count) {
        auto pos = index.position();
        NodesInfo info({index.parent(),pos,count});
        m_removeNodesPile.push_front(info);
        emit nodesAboutToBeRemoved(info.parent,info.pos,info.count);
    }

    //! Début de réinitialisation des données du model.
    void beginResetData()
        {emit modelAboutToResetData();}

    //! Début de réinitialisation du model.
    void beginResetModel()
        {emit modelAboutToBeReset();}

    //! Crée un index de pointeur ptr.
    NodeIndex createIndex(IterNode iter, int cible = NodeCible, numt num = 0) const noexcept;

    //! Fin d'insertion de lignes.
    void endInsertNodes() {
        auto info = m_insertNodesPile.front();
        emit nodesInserted(info.parent,info.pos,info.count);
        m_insertNodesPile.pop_front();
    }

    //! Fin de déplacement de lignes.
    void endMoveNodes() {}

    //! Fin de suppression de lignes.
    void endRemoveNodes() {
        auto info = m_removeNodesPile.front();
        emit nodesRemoved(info.parent,info.pos,info.count);
        m_removeNodesPile.pop_front();
    }

    //! Fin de réinitialisation du model.
    void endResetModel()
        {emit modelReset();}

    //! Fin de réinitialisation des données du model.
    void endResetData()
        {emit modelResetData();}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité de l'index).
    ItemNode & getNode(const NodeIndex &index)
        {return m_data.getNode(index);}

    //! Convertie un index en itérateur.
    IterNode indexToIterator(const NodeIndex & index) const
        {return index.m_iter;}

    //! Insert count noeuds de nature type avant la position pos de parent.
    virtual std::list<IterNode> insert(const NodeIndex &parent, numt pos, numt count, int type = DefaultType);

    //! Fabrique des noeuds.
    virtual Node nodeFactory(const NodeIndex & /*parent*/, numt /*pos*/, int /*type*/) {return std::make_unique<ItemNode>();}

    //! Supprimer count noeud de la fratrie en commençant par le noeud node.
    virtual bool remove(const NodeIndex &index, numt count = 1);

    //! Mutateur des données du model.
    virtual bool set(const NodeIndex & index, const QVariant & value, int role);
};

/*! \ingroup groupeModel
 * \brief Classe des noeuds pour une gestion avec la base de donnée.
 */
class ItemBddNode : public ItemNode{
public:
    //! Constructeur.
    using ItemNode::ItemNode;

    NODE_COPIE(ItemBddNode)

    //! Enregistre les données du noeud.
    virtual void save(bddMPS::Bdd &) {}

    //! Insert un nouveau noeud dans la base de donnée.
    virtual void insert(bddMPS::Bdd &) {}
};

/*! \ingroup groupeModel
 * \brief Classe mère des models à noeuds avec une gestion en base de donnée.
 */
class ItemNodeBddModel : public ItemNodeModel {
    Q_OBJECT
protected:
    bddMPS::Bdd & m_bdd;            //!< Référence à la base de donnée.
public:
    //! Constructeur.
    ItemNodeBddModel(bddMPS::Bdd & bdd, QObject * parent)
        : ItemNodeModel(parent), m_bdd(bdd) {}

    //! Accesseur de la base de données.
    bddMPS::Bdd & bdd() const
        {return m_bdd;}

public slots:
    //! Enregistre les données du model.
    virtual void save();
};

///////////////////////////////////// Definition de TreeForNodeModel //////////////////////////////////
template<class Factory> std::list<IterNode> TreeForNodeModel::insertNodes(const NodeIndex &parent, numt pos, numt count, Factory factory) {
    std::list<IterNode> list;
    auto iter = parent.m_iter;
    if(pos == parent.childCount())
        for(numt i = 0; i != count; ++i)
            list.push_back(updateIter(m_tree.push_back(iter,factory(parent, pos + i))));
    else {
        iter.toChildU(pos);
        for(numt i = count; i != 0; --i)
            list.push_back(updateIter(m_tree.insert(iter,factory(parent, pos + i))));
    }
    return list;
}

template<class U, class F>  void TreeForNodeModel::setTree(const conteneurMPS::tree<U> & t, F usine) {
    auto iterU = t.cbegin();
    setRoot(usine(*iterU));
    auto iterT = m_tree.begin();
    while (iterU) {
        if(!iterU.leaf())
            for (auto i = iterU.cbeginChild(); i; ++i)
                updateIter(m_tree.push_back(iterT, usine(*i)));
        ++iterU;
        ++iterT;
    }
}
}
#endif // ITEMNODEMODEL_H
