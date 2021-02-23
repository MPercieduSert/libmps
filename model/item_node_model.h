/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef ITEM_NODE_MODEL_H
#define ITEM_NODE_MODEL_H

#include <memory>
#include <QObject>
#include <QVariant>
#include <set>
#include "bdd.h"

//! \ingroup groupe_model
//! Copie l'état du noeud.
#define NODE_COPIE(TYPE) /*! Copie l'état du noeud */ \
    node_ptr copie() const override {return std::make_unique<TYPE>(*this);}

namespace mps {
namespace model_base {
class item_node;
class item_node_model;
class tree_for_node_model;
//! Classe des noeud de l'arbre de données
using node_ptr = std::unique_ptr<item_node>;
//! Iterateur (const) sur les noeuds de l'arbre.
using node_iter = tree<node_ptr>::const_iterator;

//! Cible de donnée prédéfinies.
enum cible_data_node {
    Node_Cible = -1,
    Sub_Node_Cible = -2,
    Externe_Cible = -3
};

//! Role des données
enum role_node : flag::flag_type {
    No_Role =0x0,
    //Forme Role
    Tool_Tip_Role = 0x1,                  //!< Info bulle (QString)
    Status_Tip_Role = 0x2,                //!< Info dans la barre de tache (QString)
    What_This_Role = 0x4,                 //!< Info dans "What's this" (QString)
    Font_Role = 0x8,                     //!< Police d'affichage principale (QFont)
    Texte_Alignement_Role = 0x10,         //!< Alignement du texte principale (Qt:Alignment)
    Background_Role = 0x20,              //!< Fond du texte principale (QBrush)
    Foreground_Role = 0x40,              //!< Rendu du texte principale (QBrush)
    Form_All_Role = Tool_Tip_Role | Status_Tip_Role | What_This_Role | Font_Role | Texte_Alignement_Role
                    | Background_Role | Foreground_Role,
    //Label Role
    Label_Role = 0x80,                   //!< Titre (QString)
    Font_Label_Role = 0x100,              //!< Police du titre (QFont)
    Alignement_Label_Role = 0x200,        //!< Alignement du texte du label (Qt:Alignment)
    Background_Label_Role = 0x400,        //!< Fond du label (QBrush)
    Foreground_Label_Role = 0x800,        //!< Rendu du label (QBrush)
    Label_All_Role = Label_Role | Font_Label_Role | Alignement_Label_Role | Background_Label_Role | Foreground_Label_Role,
    //Configuration Role
    Flag_Role = 0x1000,                  //!< Code (flag)
    Orientation_Role = 0x2000,           //!< Orientation (Qt::Orientation)
    Type_Role = 0x4000,                  //!< Type des donnée (int)
    Sub_Node_Role = 0x8000,               //!< Initialisation d'un sous-noeud (QList<QVariant>(Cible,Num,Type_Sub_Node)).
    List_Of_Values = 0x10000,             //!< Liste des choix possible (QMap<QString,QVariant>)
    Config_All_Role = Flag_Role | Orientation_Role | Type_Role | List_Of_Values,
    // Donnée Principale role
    String_Role = 0x20000,               //!< Donnée principale sous forme de texte (QString)
    Check_State_Role = 0x40000,           //!< Donnée principale sous forme d'état des cases à cocher (Qt::CheckState)
    Date_Role = 0x8000,                  //!< Donnée principale sous forme de date (QDate)
    Date_Time_Role = 0x100000,            //!< Donnée principale sous forme de date et horaire (QDateTime)
    Time_Role = 0x200000,                //!< Donnée principale sous forme d'horraire (QTime)
    Int_Role = 0x400000,                 //!< Donnée principale sous forme d'un entier (int)
    Num_Role = 0x800000,                 //!< Donnée principale sous forme d'un numéro (numt)
    Variant_Role = 0x1000000,            //!< Donnée principale sous forme d'un variant (QVariant)
    Bool_Role = 0x2000000,               //!< Donnée principale sous forme d'un booléen (bool)
    Double_Role = 0x4000000,             //!< Donnée principale sous forme d'un double (double)
    Decoration_Role = 0x8000000,         //!< Donnée principale sous forme d'image (QColor,QIcon,QPixmap)
    String_List_Role = 0x10000000,        //!< Donnée principale sous forme d'une liste de texte (QStringlist)
    List_Role = 0x20000000,              //!< Donnée principale sous forme d'une liste de variant (QList<QVariant>)
    Map_Role = 0x40000000,               //!< Donnée principlae sous forme d'une map (QMap<QString,QVariant>)
    Main_All_Role = String_Role | Check_State_Role | Date_Role | Date_Time_Role | Time_Role | Int_Role | Num_Role
                    | Variant_Role | Bool_Role | Double_Role | Decoration_Role | String_List_Role | List_Role | Map_Role,
    All_Role = Form_All_Role | Label_All_Role | Config_All_Role | Main_All_Role
};

//! Types des sous-noeuds.
enum sub_node_type {
    Check_Sub_Node,
    Code_Sub_Node,
    Combo_Box_Sub_Node,
    Date_Sub_Node,
    Line_Edit_Sub_Node,
    Texte_Edit_Sub_Node
};

//! Initialisation d'un sous-noeud.
enum init_sub_node {
    Cible_Sub_Node,
    Num_Sub_Node,
    Type_Sub_Node,
    Nbr_Init_Sub_Node
};

//! flag du model
enum flag_node : flag::flag_type {
    No_Flag_Node = 0x0,
    Visible_Flag_Node = 0x1,
    Enable_Flag_Node = 0x2,
    Left_Clickable_Flag_Node = 0x4,
    Selectable_Flag_Node = 0x8,
    Expendable_FLag_Node = 0x10,
    Elder_Enable_Flag_Node = 0x20,
    Brother_Enable_Flag_Node = 0x40,
    Del_Enable_Flag_Node = 0x80,
    Default_Falg_Node = Visible_Flag_Node | Enable_Flag_Node | Left_Clickable_Flag_Node | Selectable_Flag_Node,
    Default_Root_Flag_Node = Default_Falg_Node | Expendable_FLag_Node | Elder_Enable_Flag_Node,
    Default_Flag_Node = Default_Root_Flag_Node | Brother_Enable_Flag_Node | Del_Enable_Flag_Node
};

/*! \ingroup groupe_model
 *\brief Classe mère des noeuds.
 */
class item_node {
    friend tree_for_node_model;
protected:
    node_iter m_iter = static_cast<void *>(nullptr);     //! Iterateur sur le noeud de l'arbre contenant ce noeud.
    int m_type;                                         //! type du noeud.
public:
    enum {No_Type = -1,
          No_Data = 0};
    //! Constructeur.
    item_node(int type = No_Type) : m_type(type){}

    //! Destructeur.
    virtual ~item_node();

    //! Copie l'état du noeud
    virtual node_ptr copie() const
        {return std::make_unique<item_node>(*this);}

    //! Accesseur de la donnée associé à column.
    virtual QVariant data(int cible, int role, numt /*num*/ = 0) const;

    //! Nombre de donnée associé au noeud pour une cible donnée.
    virtual numt data_count(int cible) const
        {return cible == Node_Cible ? 1 : No_Data;}

    //! Supprime les donnée du noeud.
    virtual bool del(/*b2d::bdd &*/) {return true;}

    //! Accesseur des drapeaux associés à column.
    virtual flag flags(int cible, numt /*num*/ = 0) const {
        if(cible == Node_Cible){
            if(m_iter.root())
                return Default_Root_Flag_Node;
            return Default_Flag_Node;
        }
        return Default_Falg_Node;
    }

    //! Mutateur de la donnée associé à column.
    virtual flag set_data(int /*cible*/, const QVariant &/*value*/, int /*role*/, numt /*num*/ = 0) {return No_Role;}

    //! Accesseur du type du noeud.
    int type() const {return m_type;}
};

/*! \ingroup groupe_model
 *\brief Classe d'index d'un noeuds.
 */
class node_index {
    friend item_node_model;
    friend tree_for_node_model;
protected:
    int m_cible = Node_Cible;                            //!< Cible de l'index.
    numt m_num = 0;                                     //!< Numéro.
    node_iter m_iter = static_cast<void *>(nullptr);     //!< Itérateur pointant sur un noeud du model.
    item_node_model *m_model = nullptr;                  //!< Pointeur sur le model.
    enum {root_count = 1};
public:
    using sub_index = std::pair<int,numt>;

    //! Constructeur.
    node_index() = default;

    //! Renvoie le nombre de d'enfants.
    numt child_count() const noexcept {
        if(!is_valid())
            return root_count;
        return m_iter.size_child();
    }

    //! Accesseur de la cible.
    int cible() const noexcept
        {return m_cible;}

    //! Accesseur des données associées à l'index.
    QVariant data(int role) const;

    //! Retourne un index sur le frère ainé.
    node_index first() const noexcept;

    //! Drapeaux assossiés à une donnée.
    flag flags() const;

    //! Créer un index de même model et pointeur avec la cible et le numero spécifié.
    node_index index(int cb, numt nm = 0) const noexcept {
        auto ind = *this;
        ind.set_cible(cb);
        ind.set_num(nm);
        return ind;
    }

    //! Accesseur du pointeur interne.
    void *internal_pointer() const noexcept
        {return m_iter.ptr();}

    //! Teste si l'index est la racine.
    bool is_root() const
        {return m_iter.root();}

    //! Teste si l'index est valide.
    bool is_valid() const noexcept
        {return m_iter;}

    //! Retourne un index sur le frère benjamin.
    node_index last() const noexcept;

    //! Teste si dans le model le noeud associé à l'index est une feuille.
    bool leaf() const
        {return m_iter.leaf();}

    //! Accesseur du model.
    item_node_model *model() const noexcept
        {return m_model;}

    //! Retourne un index sur le frère suivant.
    node_index next_brother() const noexcept;

    //! Accesseur du numéro.
    numt num() const noexcept
        {return m_num;}

    //! Teste l'équivalence de deux index.
    bool operator==(const node_index &index) const noexcept
        {return m_model == index.m_model &&m_iter == index.m_iter &&m_cible == index.m_cible &&m_num == index.m_num;}

    //! Retourne un index sur le parent.
    node_index parent() const noexcept;

    //! Retourne la position du noeud dans la fratrie.
    numt position() const
        {return m_iter.position() ;}

    //! Retourne un index sur le frère précédent.
    node_index prev_brother() const noexcept;

    //! Mutateur de la cible.
    void set_cible(int cb) noexcept
        {m_cible = cb;}

    //! Mutateur de la cible.
    void set_num(numt num) noexcept
        {m_num = num;}

    //! Retourne le sous index.
    sub_index sub() const noexcept
        {return {m_cible,m_num};}

protected:
    //! Accesseur (par valeur) de l'itérateur.
    node_iter iter() const noexcept
        {return m_iter;}
};

/*! \ingroup groupe_model
 *\brief arbre de noeuds pour les models à noeud.
 */
class tree_for_node_model {
protected:
    //! Poiteur constant sur les noeuds de l'arbre.
    using const_iterator = tree<node_ptr>::const_iterator;
    //! Poiteur sur les noeuds de l'arbre.
    using iterator = tree<node_ptr>::iterator;
    tree<node_ptr> m_tree;                //!< arbre de donnée.
public:
    //! Iterateur sur la racine.
    const_iterator cbegin() const noexcept
        {return m_tree.cbegin();}

    //! Iterateur sur la racine.
    tree<node_ptr>::const_reverse_iterator crbegin() const noexcept
        {return m_tree.crbegin();}

    //! Efface un noeud de l'arbre.
    void erase(const node_index &index)
        {m_tree.erase(index.m_iter);}

    //! Efface les count frère de index.
    void erase(const node_index &index, numt count)
        {m_tree.erase(index.m_iter,index.iter().to_brother_u(count));}

    //! Renvoie une référence sur le noeud coorespondant à l'index (en supposant sa validité).
    const item_node &get_node(const node_index &index) const
        {return **index.m_iter;}

    //! Renvoie une référence sur le noeud coorespondant à l'index (en supposant sa validité).
    item_node &get_node(const node_index &index)
        {return **index.m_iter;}

    //! Renvoie une référence sur le noeud de la racine.
    const item_node &get_root() const
        {return **m_tree.cbegin();}

    //! Renvoie une référence sur le noeud de la racine.
    item_node &get_root()
        {return **m_tree.begin();}

    //! Insert des noeud du model, ne vérifie pas les arguments.
    template<class Factory> std::list<node_iter> insert_nodes(const node_index &parent, numt pos, numt count, Factory factory);

    //! Remplace le noeud index par node.
    node_ptr move_node(const node_index &index, node_ptr &&node);

    //! Déplace le noeud d'index from à la position to, le noeud de position from devient invalide.
    node_ptr move_node(const node_index &from, const node_index &to);

    //! Déplace le noeud d'index from à la position to, le noeud de position from devient invalide.
    void move(const node_index &from, const node_index &to)
        {set_node(to,take_node(from));}

    //! Ajoute un fils cadet au noeud index.
    node_iter push_back(const node_index &index, node_ptr &&node)
        {return update_iter(m_tree.push_back(index.m_iter,std::move(node)));}

    //! Ajoute un fils ainé au noeud index.
    node_iter push_front(const node_index &index, node_ptr &&node)
        {return update_iter(m_tree.push_front(index.m_iter,std::move(node)));}

    //! Supprime des noeuds du model.
    bool remove_nodes(const node_index &parent, numt pos, numt count);

    //! Modifie un noeud de l'arbre.
    void set_node(const node_index &index, node_ptr &&node) {
        *static_cast<iterator>(index.m_iter) = std::move(node);
        update_iter(index.m_iter);
    }

    //! Modifie l'arbre de donnée avec F de signature node_ptr (const U &).
    template<class U, class F>  void set_tree(const tree<U> &tr, F usine);

    //! Modifie l'arbre de donnée.
    void set_root(node_ptr &&node) {
        m_tree.clear();
        *m_tree.begin() = std::move(node);
        update_iter(m_tree.cbegin());
    }

    //! Prend le noeud index, ce noeud devient invalide.
    node_ptr take_node(const node_index &index)
        {return std::move(*static_cast<iterator>(index.m_iter));}

protected:
    //! Met à jour l'itérateur du noeud pointé par l'itérateur.
    node_iter update_iter(node_iter iter) const
        {return (**iter).m_iter = iter;}
};

/*! \ingroup groupe_model
 *\brief Classe mère des models à noeuds.
 */
class item_node_model : public QObject{
    Q_OBJECT
protected:
    //! Type de modification.
    enum change_type {
        Set_Data,
        Insert_Node,
        Remove_Node
    };
    enum {Aucune_Position = 0};
    //! Information sur la position d'un noeud
    struct node_info {
        node_index parent;       //!< Index du parent des noeuds.
        numt pos;               //!< position du premier noeud.
        numt count;             //!< Nombre de noeuds concerné.
    };
    //! Information sur les modifications apporté à un noeud.
    struct node_change {
        numt type;
        numt pos;
        numt count;
        node_iter iter = static_cast<void *>(nullptr);
        node_ptr node;
    };
    std::list<node_change> m_change_list;               //!< Liste des modifications du model.
    std::forward_list<node_info> m_insert_nodes_pile;   //!< Pile des informations d'insertion de noeuds.
    std::forward_list<node_info> m_remove_nodes_pile;   //!< Pile des informations de suppression de noeuds.
    tree_for_node_model m_data;                         //!< arbre de données.

public:
    enum {Default_Type = -2};
    using QObject::parent;
    //! Constructeur.
    item_node_model(QObject *parent = nullptr) : QObject(parent) {}

    //! Teste si l'index est valide pour ce model.
    bool check_index(const node_index &index) const
        {return index.m_model == this &&index.m_iter;}

    //! Accesseur des données du model.
    virtual QVariant data(const node_index &index, int role) const;

    //! Nombre de données associées associé à un noeud pour une cible donnée.
    virtual numt data_count(const node_index &index) const;

    //! Drapeaux assossiés à une donnée.
    virtual flag flags(const node_index &index) const;

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité de l'index).
    const item_node &get_node(const node_index &index) const
        {return m_data.get_node(index);}

    //! Renvoie l'index correxpondant au noeud pos de parent.
    node_index index(const node_index &parent, numt pos, int cible = Node_Cible, numt num = 0) const;

    //! Insert count noeuds de nature type avant la position pos de parent.
    void insert_nodes(const node_index &parent, numt pos, numt count, int type = Default_Type) {
        auto list = insert(parent,pos,count,type);
        for (auto iter = list.cbegin(); iter != list.cend(); ++iter) {
            node_change change;
            change.type = Insert_Node;
            change.iter = *iter;
            m_change_list.push_back(std::move(change));
        }
    }

    //! Supprimer count noeud de la fratrie en commençant par le noeud node.
    bool remove_nodes(const node_index &index, numt count = 1)
        {return remove(index,count);}

    //! Mutateur des données du model.
    bool set_data(const node_index &index, const QVariant &value, int role) {
        node_change change;
        change.type = Set_Data;
        change.iter = index_to_iterator(index);
        change.node = get_node(index).copie();
        if(set(index,value,role)) {
            m_change_list.push_back(std::move(change));
            return true;
        }
        return false;
    }

signals:
    //! Signal le changement d'une donnée.
    void data_changed(const node_index &index, flag role);

    //! Signal de début de réinitialisation du model.
    void model_about_to_be_reset();

    //! Signal de début de réinitialisation des données de l'arbre mais conservant la structure.
    void model_about_to_reset_data();

    //! Signal de fin de réinitialisation du model.
    void model_reset();

    //! Signal de réinitialisation des données de l'arbre mais conservant la structure.
    void model_reset_data();

    //! Signal de début d'insertion de noeuds.
    void nodes_about_to_be_inserted(const node_index &parent, numt pos, numt count);

    //! Signal de début de suppression de noeuds.
    void nodes_about_to_be_removed(const node_index &parent, numt pos, numt count);

    //! Signal de fin d'insertion de noeuds.
    void nodes_inserted(const node_index &parent, numt pos, numt count);

    //! Signal de fin de suppression de noeuds.
    void nodes_removed(const node_index &parent, numt pos, numt count);

protected:
    //! Début d'insertions de noeuds.
    void begin_insert_nodes(const node_index &parent, numt pos, numt count) {
        m_insert_nodes_pile.push_front({parent,pos,count});
        emit nodes_about_to_be_inserted(parent,pos,count);
    }

    //! Début de déplacement de lignes.
    bool begin_move_nodes(const node_index &/*sourceParent*/, int /*sourceFirst*/, int /*sourceLast*/,
                       const node_index &/*destinationParent*/, int /*destinationChild*/) {return false;}

    //! Début de suppression de lignes [first,last).
    void begin_remove_nodes(const node_index &index, numt count) {
        auto pos = index.position();
        node_info info({index.parent(),pos,count});
        m_remove_nodes_pile.push_front(info);
        emit nodes_about_to_be_removed(info.parent,info.pos,info.count);
    }

    //! Début de réinitialisation des données du model.
    void begin_reset_data()
        {emit model_about_to_reset_data();}

    //! Début de réinitialisation du model.
    void begin_reset_model()
        {emit model_about_to_be_reset();}

    //! Crée un index de pointeur ptr.
    node_index create_index(node_iter iter, int cible = Node_Cible, numt num = 0) const noexcept;

    //! Fin d'insertion de lignes.
    void end_insert_nodes() {
        auto info = m_insert_nodes_pile.front();
        emit nodes_inserted(info.parent,info.pos,info.count);
        m_insert_nodes_pile.pop_front();
    }

    //! Fin de déplacement de lignes.
    void end_move_nodes() {}

    //! Fin de suppression de lignes.
    void end_remove_nodes() {
        auto info = m_remove_nodes_pile.front();
        emit nodes_removed(info.parent,info.pos,info.count);
        m_remove_nodes_pile.pop_front();
    }

    //! Fin de réinitialisation du model.
    void end_reset_model()
        {emit model_reset();}

    //! Fin de réinitialisation des données du model.
    void end_reset_data()
        {emit model_reset_data();}

    //! Renvoie une référence sur la donné coorespondant à l'index (en supposant la validité de l'index).
    item_node &get_node(const node_index &index)
        {return m_data.get_node(index);}

    //! Convertie un index en itérateur.
    node_iter index_to_iterator(const node_index &index) const
        {return index.m_iter;}

    //! Insert count noeuds de nature type avant la position pos de parent.
    virtual std::list<node_iter> insert(const node_index &parent, numt pos, numt count, int type = Default_Type);

    //! Fabrique des noeuds.
    virtual node_ptr node_factory(const node_index &/*parent*/, numt /*pos*/, int /*type*/) {return std::make_unique<item_node>();}

    //! Supprimer count noeud de la fratrie en commençant par le noeud node.
    virtual bool remove(const node_index &index, numt count = 1);

    //! Mutateur des données du model.
    virtual bool set(const node_index &index, const QVariant &value, int role);
};

/*! \ingroup groupe_model
 *\brief Classe des noeuds pour une gestion avec la base de donnée.
 */
class item_bdd_node : public item_node{
public:
    //! Constructeur.
    using item_node::item_node;

    NODE_COPIE(item_bdd_node)

    //! Enregistre les données du noeud.
    virtual void save(b2d::bdd &) {}

    //! Insert un nouveau noeud dans la base de donnée.
    virtual void insert(b2d::bdd &) {}
};

/*! \ingroup groupe_model
 *\brief Classe mère des models à noeuds avec une gestion en base de donnée.
 */
class item_node_bdd_model : public item_node_model {
    Q_OBJECT
protected:
    b2d::bdd &m_bdd;            //!< Référence à la base de donnée.
public:
    //! Constructeur.
    item_node_bdd_model(b2d::bdd &bd, QObject *parent)
        : item_node_model(parent), m_bdd(bd) {}

    //! Accesseur de la base de données.
    b2d::bdd &bdd() const
        {return m_bdd;}

public slots:
    //! Enregistre les données du model.
    virtual void save();
};

///////////////////////////////////// Definition de tree_for_node_model //////////////////////////////////
template<class Factory> std::list<node_iter> tree_for_node_model::insert_nodes(const node_index &parent, numt pos,
                                                                               numt count, Factory factory) {
    std::list<node_iter> list;
    auto iter = parent.m_iter;
    if(pos == parent.child_count())
        for(numt i = 0; i != count; ++i)
            list.push_back(update_iter(m_tree.push_back(iter,factory(parent, pos + i))));
    else {
        iter.to_child_u(pos);
        for(numt i = count; i != 0; --i)
            list.push_back(update_iter(m_tree.insert(iter,factory(parent, pos + i))));
    }
    return list;
}

template<class U, class F>  void tree_for_node_model::set_tree(const tree<U> &tr, F usine) {
    auto u_it = tr.cbegin();
    set_root(usine(*u_it));
    auto iterT = m_tree.begin();
    while (u_it) {
        if(!u_it.leaf())
            for (auto i = u_it.cbegin_child(); i; ++i)
                update_iter(m_tree.push_back(iterT, usine(*i)));
        ++u_it;
        ++iterT;
    }
}
}}
#endif // ITEM_NODE_MODEL_H
