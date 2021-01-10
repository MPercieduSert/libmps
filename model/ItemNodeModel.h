/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef ITEMNODEMODEL_H
#define ITEMNODEMODEL_H

#include <memory>
#include <QObject>
#include <QVariant>
#include "Bdd.h"
#include "Tree.h"
#include "typemps.h"

namespace modelMPS {
using namespace typeMPS;
class ItemNodeModel;
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
    MainAllRole = StringRole | CheckStateRole | DateRole | DateTimeRole | TimeRole | IntRole | NumRole | VariantRole | BoolRole | DoubleRole
                    | DecorationRole | StringListRole | ListRole | MapRole,
    AllRole = FormAllRole | LabelAllRole | ConfigAllRole | MainAllRole
};

//! Types des sous-noeuds.
enum subNodeType {
    CheckSubNode,
    CodeSubNode,
    ComboBoxSubNode,
    DateSubNode,
    LineEditSubNode
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

/////////////////////////////////////////////////////// NodeIndex //////////////////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe d'index d'un noeuds.
 */
class NodeIndex {
    friend ItemNodeModel;
protected:
    int m_cible = NodeCible;                //!< Cible de l'index.
    numt m_num = 0;                         //!< Numéro.
    void * m_ptr = nullptr;                 //!< Pointeur interne sur sur la donnée du model.
    ItemNodeModel * m_model = nullptr;      //!< Pointeur sur le model.
public:
    using SubIndex = std::pair<int,numt>;

    //! Constructeur.
    NodeIndex() = default;

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
    ItemNodeModel * model() const noexcept
        {return m_model;}

    //! Retourne un index sur le frère suivant.
    NodeIndex nextBrother() const noexcept;

    //! Accesseur du numéro.
    numt num() const noexcept
        {return m_num;}

    //! Teste l'équivalence de deux index.
    bool operator==(const NodeIndex & index) const noexcept
        {return m_model == index.m_model && m_ptr == index.m_ptr && m_cible == index.m_cible && m_num == index.m_num;}

    //! Retourne un index sur le parent.
    NodeIndex parent() const;

    //! Retourne la position du noeud dans la fratrie.
    numt position() const noexcept;

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
};

//! Node abstré de l'arbre de recherche.
class ItemNode {
protected:
    int m_type;     //! type du noeud.
public:
    enum {NoType = -1,
          NoData = 0};
    //! Constructeur.
    ItemNode(int type = NoType) : m_type(type){}

    virtual ~ItemNode();

    //! Accesseur de la donnée associé à column.
    virtual QVariant data(int cible, int role, numt /*num*/ = 0) const;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    virtual numt dataCount(int cible) const
        {return cible == NodeCible ? 1 : NoData;}

    //! Supprime les donnée du noeud.
    virtual bool del() {return true;}

    //! Accesseur des drapeaux associés à column.
    virtual flag flags(int cible, numt /*num*/ = 0) const {
        if(cible == NodeCible){
//            if(isRoot)
//                return DefaultRootFlagNode;
            return DefaultNodeFlagNode;
        }
        return DefaultFalgNode;}

    //! Enregistre les données du noeud.
    virtual void save(idt /*parent*/, numt /*num*/) {}

    //! Mutateur de la donnée associé à column.
    virtual flag setData(int /*cible*/, const QVariant & /*value*/, int /*role*/, numt /*num*/ = 0) {return NoRole;}

    //! Accesseur du type du noeud.
    int type() const {return m_type;}
};

/*! \ingroup groupeModel
 * \brief Arbre de noeuds pour les models à noeud.
 */
class TreeForNodeModel {
protected:
    //! Classe des noeud de l'arbre de données
    using Node = std::unique_ptr<ItemNode>;
    //! Classe structurant les données.
    using Tree = conteneurMPS::tree<Node>;
    //! Poiteur constant sur les noeuds de l'arbre.
    using const_iterator = typename Tree::const_iterator;
    //! Poiteur sur les noeuds de l'arbre.
    using iterator = typename Tree::iterator;
    //Model * m_model;            //!< Model contenant l'arbre.
    //const bool m_racine;        //!< Racine permise.
    Tree m_tree;                //!< Arbre de donnée.
public:
    //! Renvoie une référence sur le noeud coorespondant à l'index.
    const ItemNode & getNode(const NodeIndex &index) const
        {return **getIter(index);}

    //! Renvoie une référence sur le noeud coorespondant à l'index.
    ItemNode & getNode(const NodeIndex &index)
        {return **getIter(index);}

    //! Renvoie un itérateur pointant sur le noeud désigné par index.
    const_iterator getIter(const NodeIndex &index) const
        {return index.isValid() ? getValidIter(index)
                                : m_tree.cbegin();}

    //! Renvoie un itérateur pointant sur le noeud désigné par index.
    iterator getIter(const NodeIndex &index)
        {return index.isValid() ? getValidIter(index)
                                : m_tree.begin();}

    //! Renvoie une référence sur le noeud de la racine.
    const ItemNode & getRoot() const
        {return **m_tree.cbegin();}

    //! Renvoie une référence sur le noeud de la racine.
    ItemNode & getRoot()
        {return **m_tree.begin();}

    //! Renvoie un itérateur sur la racine.
    const_iterator getRootIter() const
        {return m_tree.cbegin();}

    //! Renvoie un itérateur sur la racine.
    iterator getRootIter()
        {return m_tree.begin();}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const ItemNode & getValidNode(const NodeIndex & index) const
        {return **getValidIter(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    ItemNode & getValidNode(const NodeIndex & index)
        {return **getValidIter(index);}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    const_iterator getValidIter(const NodeIndex & index) const
        {return index.internalPointer();}

    //! Renvoie un itérateur pointant sur le noeud d'index.
    iterator getValidIter(const NodeIndex & index)
        {return index.internalPointer();}

    //! Insert des noeud du model, ne vérifie pas les arguments.
    template<class Factory> bool insertNodes(const NodeIndex &parent, numt pos, numt count, Factory factory);

    //! Supprime des noeuds du model.
    bool removeNodes(const NodeIndex & parent, numt pos, numt count);

    //! Modifie l'arbre de donnée.
    void setTree(Tree && tree)
        {m_tree = std::move(tree);}

    //! Accesseur de l'arbre.
    const Tree & tree() const
        {return m_tree;}

    //! Accesseur de l'arbre.
    Tree & tree()
        {return m_tree;}
};
///////////////////////////////////////////////// AbstractForNodeModel ///////////////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe mère des models à noeuds.
 */
class ItemNodeModel : public QObject{
    Q_OBJECT
protected:
    struct NodesInfo {
        NodeIndex parent;           //!< Index du parent des nodes insérés.
        numt pos;                    //!< Position du premier noeud insérés.
        numt count;                  //!< Position du dernier noeud insérés.
    };
    std::forward_list<NodesInfo> m_insertNodesPile;         //!< Pile des informations d'insertion de noeuds.
    std::forward_list<NodesInfo> m_removeNodesPile;         //!< Pile des informations de suppression de noeuds.
    TreeForNodeModel m_data;                                //!< Arbre de données.
    enum {RootCount = 1,
         AucunePosition = 0};
public:
    enum {DefaultType = -2};
    //! Classe des noeud de l'arbre de données
    using Node = std::unique_ptr<ItemNode>;
    //! Classe structurant les données.
    using Tree = conteneurMPS::tree<Node>;
    using QObject::parent;
    //! Constructeur.
    ItemNodeModel(QObject * parent = nullptr) : QObject(parent) {}

    //! Teste si l'index est valide pour ce model.
    bool checkIndex(const NodeIndex & index) const
        {return index.m_model == this && index.m_ptr;}

    //! Renvoie le nombre de d'enfants.
    numt childCount(const NodeIndex &parent) const {
        if(!parent.isValid())
            return RootCount;
        return m_data.getValidIter(parent).sizeChild();
    }

    //! Accesseur des données du model.
    virtual QVariant data(const NodeIndex & index, int role) const;

    //! Nombre de données associées associé à un noeud pour une cible donnée.
    virtual numt dataCount(const NodeIndex & index) const;

    //! Retourne un index sur le frère ainé.
    NodeIndex firstBrother(const NodeIndex & index) const
        {return index.isValid() ? createIndex(m_data.getValidIter(index).toFirstBrother().ptr())
                                : NodeIndex();}
    //! Drapeaux assossiés à une donnée.
    virtual flag flags(const NodeIndex & index) const;

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    const ItemNode & getNode(const NodeIndex &index) const
        {return m_data.getValidNode(index);}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité).
    ItemNode & getNode(const NodeIndex &index)
        {return m_data.getValidNode(index);}

    //! Renvoie l'index correxpondant au noeud pos de parent.
    NodeIndex index(const NodeIndex &parent, numt pos, int cible = NodeCible, numt num = 0) const;

    //! Insert count noeuds de nature type avant la position pos de parent.
    virtual bool insertNodes(const NodeIndex &parent, numt pos, numt count, int type = DefaultType);

    //! Test si l'index est la racine.
    bool isRoot(const NodeIndex & index) const {
        return index.isValid() ? m_data.getValidIter(index).root()
                               : true;}

    //! Retourne un index sur le frère benjamin.
    NodeIndex lastBrother(const NodeIndex & index) const
        {return index.isValid() ? createIndex(m_data.getValidIter(index).toLastBrother().ptr())
                           : NodeIndex();}

    //! Teste si le noeud associé à un index valide est une feuille.
    bool leaf(const NodeIndex & index) const
        {return m_data.getValidIter(index).leaf();}

    //! Retourne un index sur le frère suivant.
    NodeIndex nextBrother(const NodeIndex & index) const
        {return index.isValid() ? createIndex(m_data.getValidIter(index).toNextBrother().ptr())
                                : NodeIndex();}

    //! Renvoie l'index du parent.
    NodeIndex parent(const NodeIndex &index) const {
        if(index.isValid()){
            auto ptr = m_data.getValidIter(index).toParent().ptr();
            if(ptr)
                return createIndex(ptr);
        }
        return NodeIndex();
    }

    //! Position du noeud dans la fratrie.
    numt position(const NodeIndex & index) const {
        if(!index.isValid())
            return AucunePosition;
        return m_data.getValidIter(index).position();
    }

    //! Retourne un index sur le frère suivant.
    NodeIndex prevBrother(const NodeIndex & index) const
        {return index.isValid() ? createIndex(m_data.getValidIter(index).toPrevBrother().ptr())
                                : NodeIndex();}

    //! Supprimer count noeud de la fratrie en commençant par le noeud node.
    virtual bool removeNodes(const NodeIndex &index, numt count = 1);

    //! Mutateur des données du model.
    virtual bool setData(const NodeIndex & index, const QVariant & value, int role);

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
    NodeIndex createIndex(void * ptr = nullptr, int cible = NodeCible, numt num = 0) const noexcept;

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

    //! Fabrique des noeuds.
    virtual Node nodeFactory(const NodeIndex & /*parent*/, numt /*pos*/, int /*type*/) {return std::make_unique<ItemNode>();}
};

////////////////////////////////////////////////// TreeNodeModelWithBdd /////////////////////////////////////////////////////
/*! \ingroup groupeModel
 * \brief Classe mère des models à noeuds.
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
template<class Factory> bool TreeForNodeModel::insertNodes(const NodeIndex &parent, numt pos, numt count, Factory factory) {
    auto iter = getIter(parent);
    if(pos == iter.sizeChild())
        for(numt i = 0; i != count; ++i)
            m_tree.push_back(iter,factory(parent, pos + i));
    else {
        iter.toChildU(pos);
        for(numt i = count; i != 0; --i)
            m_tree.insert(iter,factory(parent, pos + i));
    }
    return true;
}
}
#endif // ITEMNODEMODEL_H
