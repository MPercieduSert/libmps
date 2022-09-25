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
class node_index;
class tree_for_node_model;
//! Classe des noeud de l'arbre de données
using node_ptr = std::unique_ptr<item_node>;
//! Iterateur (const) sur les noeuds de l'arbre.
using node_iter = tree<node_ptr>::const_iterator;

//! Cible de donnée prédéfinies.
enum cible_data_node {
    Node_Cible = -1,
    No_Cible = -2,
    Externe_Cible = -3
};

//! Role des données
enum role_node {
    No_Role = 0x0,
    //Forme Role
    Tool_Tip_Role,              //!< Info bulle (QString)
    Status_Tip_Role,            //!< Info dans la barre de tache (QString)
    What_This_Role,             //!< Info dans "What's this" (QString)
    Font_Role,                  //!< Police d'affichage principale (QFont)
    Texte_Alignement_Role,      //!< Alignement du texte principale (Qt:Alignment)
    Background_Role,            //!< Fond du texte principale (QBrush)
    Foreground_Role,            //!< Rendu du texte principale (QBrush),
    //Label Role
    Label_Role,                 //!< Titre (QString)
    Font_Label_Role,            //!< Police du titre (QFont)
    Alignement_Label_Role,      //!< Alignement du texte du label (Qt:Alignment)
    Background_Label_Role,      //!< Fond du label (QBrush)
    Foreground_Label_Role ,     //!< Rendu du label (QBrush)
    //Configuration Role
    Cibles_Role,                //!< Cibles (QList<QVariant> = QList<QPair<cible,num>>).
    Flag_Role,                  //!< Drapeaux (flag)
    Orientation_Role,           //!< Orientation (Qt::Orientation)
    Type_Role,                  //!< Type des donnée (int)
    Nombre_Role,                //!< Nombre de composants (szt)
    Min_role,                   //!< Minimum (int)
    Max_role,                   //!< Maximum (int)
    // Donnée Principale role
    String_Role,                //!< Donnée principale sous forme de texte (QString)
    Check_State_Role,           //!< Donnée principale sous forme d'état des cases à cocher (Qt::CheckState)
    Date_Role,                  //!< Donnée principale sous forme de date (QDate)
    Date_Time_Role,             //!< Donnée principale sous forme de date et horaire (QDateTime)
    Time_Role,                  //!< Donnée principale sous forme d'horraire (QTime)
    Id_Role,                    //!< Donnée principale sous forme d'identifiant (uint)
    Int_Role,                   //!< Donnée principale sous forme d'un entier (int)
    Num_Role,                   //!< Donnée principale sous forme d'un numéro (uint)
    Variant_Role,               //!< Donnée principale sous forme d'un variant (QVariant)
    Bool_Role ,                 //!< Donnée principale sous forme d'un booléen (bool)
    Double_Role,                //!< Donnée principale sous forme d'un double (double)
    Decoration_Role,            //!< Donnée principale sous forme d'image (QColor,QIcon,QPixmap)
    String_List_Role,           //!< Donnée principale sous forme d'une liste de texte (QStringlist)
    List_Role,                  //!< Donnée principale sous forme d'une liste de variant (QList<QVariant>)
    Map_Role ,                  //!< Donnée principlae sous forme d'une map (QMap<QString,QVariant>)
};

//! Drapeaux des signaux de modifications
enum change_flag : flag::flag_type {
    No_Change_Flag = 0x0,
    Same_Change_Flag = 0x1,
    Main_Data_Change_Flag = 0x2,
    Main_Same_Change_Flag = Same_Change_Flag | Main_Data_Change_Flag,
    Label_Change_Flag = 0x4,
    Flag_Change_Flag = 0x8,
    Setting_Change_Flag = 0x10,
    Type_Change_Flag = 0x20,
    All_Data_Change_Flag = Main_Same_Change_Flag | Label_Change_Flag | Flag_Change_Flag | Setting_Change_Flag

};

//! Types des sous-noeuds.
enum sub_node_type {
    Check_Sub_Node,
    Code_Sub_Node,
    Combo_Box_Sub_Node,
    Date_Sub_Node,
    Label_Sub_Node,
    Line_Edit_Sub_Node,
    Spin_Box_Sub_Node,
    Texte_Edit_Sub_Node
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
    Auto_Expend_Flag_Node = 0x100,
    Default_Flag_Node = Visible_Flag_Node | Enable_Flag_Node | Left_Clickable_Flag_Node | Selectable_Flag_Node,
    Default_Root_Flag_Node = Default_Flag_Node | Expendable_FLag_Node | Elder_Enable_Flag_Node | Auto_Expend_Flag_Node,
    Default_Node_Flag_Node = Default_Root_Flag_Node | Brother_Enable_Flag_Node | Del_Enable_Flag_Node
};

//! Numéro du noeud et des sous-noeuds.
enum num_node {
    Node_Num,
    Off_Set_Num_Sub_Node
};

/*! \ingroup groupe_model
 *\brief Classe mère des noeuds.
 */
class item_node {
    friend item_node_model;
    friend tree_for_node_model;
protected:
    node_iter m_iter = static_cast<void *>(nullptr);    //!< Iterateur sur le noeud de l'arbre contenant ce noeud.
    int m_type;                                         //!< type du noeud.
public:
    enum {No_Type = -1,
          No_Data = 0};
    //! Constructeur.
    item_node(int type = No_Type) : m_type(type){}

    CONSTR_AFFECT_DEFAUT(item_node)

    //! Destructeur.
    virtual ~item_node();

    //! Copie l'état du noeud
    virtual node_ptr copie() const
        {return std::make_unique<item_node>(*this);}

    //! Accesseur de la donnée associé à column.
    virtual QVariant data(int cible, int role, numt num = 0) const;

    //! Supprime les donnée du noeud.
    virtual bool del(/*b2d::bdd &*/) {return true;}

    //! Accesseur des drapeaux associés.
    virtual flag flags(int cible, numt /*num*/ = 0) const {
        if(cible == Node_Cible){
            if(m_iter.root())
                return Default_Root_Flag_Node;
            return Default_Node_Flag_Node;
        }
        return Default_Flag_Node;
    }

    //! Mutateur de la donnée associé.
    virtual flag set_data(int /*cible*/, const QVariant &/*value*/, int /*role*/, numt /*num*/ = 0) {return No_Change_Flag;}

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
    numt m_num = 0;                                      //!< Numéro.
    node_iter m_iter = static_cast<void *>(nullptr);     //!< Itérateur pointant sur un noeud du model.
    item_node_model *m_model = nullptr;                  //!< Pointeur sur le model.
    QObject *m_source;
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

    //! Retourne un index sur le fils ainé.
    node_index first() const noexcept;

    //! Retourne un index sur le frère ainé.
    node_index first_brother() const noexcept;

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

    //! Teste si un QObject est la source.
    bool is_source(QObject *ptr) const noexcept
        {return m_source && m_source == ptr;}

    //! Teste si l'index est valide.
    bool is_valid() const noexcept
        {return m_iter;}

    //! Retourne un index sur le fils benjamin.
    node_index last() const noexcept;

    //! Retourne un index sur le frère benjamin.
    node_index last_brother() const noexcept;

    //! Teste si dans le model le noeud associé à l'index est une feuille.
    bool leaf() const
        {return m_iter.leaf();}

    //! Accesseur du model.
    item_node_model *model() const noexcept
        {return m_model;}

    //! Retourne un index sur le frère suivant.
    node_index next() const noexcept;

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
    node_index prev() const noexcept;

    //! Mutateur de la cible.
    void set_cible(int cb) noexcept
        {m_cible = cb;}

    //! Mutateur de la cible.
    void set_num(numt num) noexcept
        {m_num = num;}

    //! Mutateur de la source.
    void set_source(QObject *src) noexcept
        {m_source = src;}

    //! Retourne le sous index.
    sub_index sub() const noexcept
        {return {m_cible,m_num};}

    //! Déplace l'index sur le frère suivant.
    node_index & to_next() noexcept {
        m_iter.to_next();
        return *this;
    }

    //! Déplace l'index sur le parent.
    node_index & to_parent() noexcept {
        m_iter.to_parent();
        return *this;
    }

    //! Déplace l'index sur le frère précédent.
    node_index & to_prev() noexcept {
        m_iter.to_prev();
        return *this;
    }

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
    node_iter push_back(node_iter it, node_ptr &&node)
        {return update_iter(m_tree.push_back(it,std::move(node)));}

    //! Ajoute un fils cadet au noeud index.
    node_iter push_back(const node_index &index, node_ptr &&node)
        {return push_back(index.m_iter,std::move(node));}

    //! Ajoute un fils ainé au noeud index.
    node_iter push_front(node_iter it, node_ptr &&node)
        {return update_iter(m_tree.push_front(it,std::move(node)));}

    //! Ajoute un fils ainé au noeud index.
    node_iter push_front(const node_index &index, node_ptr &&node)
        {return push_front(index.m_iter,std::move(node));}

    //! Supprime des noeuds du model.
    bool remove_nodes(const node_index &parent, numt pos, numt count);

    //! Modifie un noeud de l'arbre.
    void set_node(const node_index &index, node_ptr &&node) {
        *static_cast<iterator>(index.m_iter) = std::move(node);
        update_iter(index.m_iter);
    }

    //! Modifie l'arbre de donnée avec F de signature node_ptr (tree<U>::const_abstrac_iterator).
    template<class U, class F>  void set_tree(const tree<U> &tr, F usine);

    template<class U, class F>  void set_tree_ref(const tree<U> &tr, F usine)
        {set_tree(tr,[&usine](typename tree<U>::const_abstract_iterator iter)->node_ptr{return usine(*iter);});}

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
        node_iter iter = static_cast<void *>(nullptr);
        node_ptr node = nullptr;
        numt type = Set_Data;
        numt pos = 0;
        numt count = 0;
    };
    std::list<node_change> m_change_list;               //!< Liste des modifications du model.
    std::forward_list<node_info> m_insert_nodes_pile;   //!< Pile des informations d'insertion de noeuds.
    std::forward_list<node_info> m_remove_nodes_pile;   //!< Pile des informations de suppression de noeuds.
    tree_for_node_model m_data;                         //!< arbre de données.

public:
    using node_index = model_base::node_index;
    using node_iter = model_base::node_iter;
    enum {Default_Type = -2};
    using QObject::parent;
    //! Constructeur.
    item_node_model(QObject *parent = nullptr) : QObject(parent) {}

    //! Teste si l'index est valide pour ce model.
    bool check_index(const node_index &index) const
        {return index.m_model == this &&index.m_iter;}

    //! Accesseur des données du model.
    virtual QVariant data(const node_index &index, int role) const;

    //! Emet un signal de changement de données.
    void emit_data_changed(const item_node &node, int cible, flag role, numt num = 0)
        {emit data_changed(create_index(node.m_iter,cible,num),role);}

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

    //! Mutateur des données du model (dans le système de suivie des modifications).
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
    void data_changed(const mps::model_base::item_node_model::node_index &index, mps::flag role);

    //! Signal de début de réinitialisation du model.
    void model_about_to_be_reset();

    //! Signal de début de réinitialisation des données de l'arbre mais conservant la structure.
    void model_about_to_reset_data();

    //! Signal de fin de réinitialisation du model.
    void model_reset();

    //! Signal de réinitialisation des données de l'arbre mais conservant la structure.
    void model_reset_data();

    //! Signal de début d'insertion de noeuds.
    void nodes_about_to_be_inserted(const mps::model_base::item_node_model::node_index &parent, mps::numt pos, mps::numt count);

    //! Signal de début de suppression de noeuds.
    void nodes_about_to_be_removed(const mps::model_base::item_node_model::node_index &parent, mps::numt pos, mps::numt count);

    //! Signal de fin d'insertion de noeuds.
    void nodes_inserted(const mps::model_base::item_node_model::node_index &parent, mps::numt pos, mps::numt count);

    //! Signal de fin de suppression de noeuds.
    void nodes_removed(const mps::model_base::item_node_model::node_index &parent, mps::numt pos, mps::numt count);

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

    //! Mutateur des données du model (vérification de l'index et emission de la donnée modifié).
    virtual bool set(const node_index &index, const QVariant &value, int role);
};

/*! \ingroup groupe_model
 *\brief Classe des noeuds pour une gestion avec la base de donnée.
 */
class item_bdd_node : public item_node{
public:
    //! Constructeur.
    using item_node::item_node;

    CONSTR_AFFECT_DEFAUT(item_bdd_node)

    //! Destructeur.
    ~item_bdd_node() override;

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

/*! \ingroup groupe_model
 *\brief Classe mère interface de models à noeuds, ne contient pas de donnée mais un lien vers un model la contenant.
 */
class interface_node_model : public item_node_model {
    Q_OBJECT
protected:
    item_node_model *m_model;           //!< Lien vers les données.
public:
    //! Constructeur.
    interface_node_model(item_node_model *model, QObject *parent = nullptr) : item_node_model(parent), m_model(model) {}

    //! Accesseur du model.
    virtual item_node_model *model() const
        {return m_model;}
};

/*! \ingroup groupe_model
 *\brief Classe mère interface de models à noeuds doté d'un noeud courant pointant sur le modèle interfacé.
 */
class current_index_interface_node_model : public interface_node_model {
    Q_OBJECT
protected:
    node_index m_current;           //!< Indexe sut le noeud courant du model interfacé.
public:
    //! Constructeur.
    using interface_node_model::interface_node_model;

    const node_index &current() const
        {return m_current;}

public slots:
    //! Mutateur de l'index.
    virtual void set_current(const model_base::node_index &current, const model_base::node_index &/*previous*/ = node_index())
        {m_current = current;
        emit model_reset();}
};

///////////////////////////////////// Definition de tree_for_node_model //////////////////////////////////
template<class Factory> std::list<node_iter> tree_for_node_model::insert_nodes(const node_index &parent, numt pos,
                                                                               numt count, Factory factory) {
    std::list<node_iter> list;
    auto iter = parent.m_iter;
    if(pos == parent.child_count())
        for(numt i = 0; i != count; ++i)
            list.push_back(push_back(iter,factory(parent, pos + i)));
    else {
        iter.to_child_u(pos);
        for(numt i = count; i != 0; --i)
            list.push_back(update_iter(m_tree.insert(iter,factory(parent, pos + i))));
    }
    return list;
}

template<class U, class F>  void tree_for_node_model::set_tree(const tree<U> &tr, F usine) {
    auto u_it = tr.cbegin();
    set_root(usine(u_it));
    auto iterT = m_tree.begin();
    while (u_it) {
        if(!u_it.leaf())
            for (auto i = u_it.cbegin_child(); i; ++i)
                update_iter(m_tree.push_back(iterT, usine(i)));
        ++u_it;
        ++iterT;
    }
}
}}
#endif // ITEM_NODE_MODEL_H
