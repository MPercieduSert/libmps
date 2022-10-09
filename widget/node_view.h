/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef NODE_VIEW_H
#define NODE_VIEW_H

#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include "node_selection_model.h"

/*! \defgroup groupe_delegate Delegate
 *\brief Ensemble des classes des delegates.
 */
namespace mps {
namespace widget {
/*! \ingroup groupe_Widget
 *\brief Classe possédant un index et calque principal orienté.
 */
class indexed_widget : public QWidget {
    Q_OBJECT
protected:
    using node_index = model_base::node_index;
    node_index m_index;                 //!< Index associé aux noeuds.
    QBoxLayout *m_main_layout;          //!< Calque principale du sous-noeud.
    flag m_flags;                       //!< Drapeaux associé aux noeud.
public:
    //! Constructeur.
    indexed_widget(const node_index &index, QWidget *parent = nullptr);

    //! Acceseur de l'index.
    node_index index() const noexcept
        {return m_index;}

    //! Accesseur du drapeaux.
    flag flags() const noexcept
        {return m_flags;}

    //! Met à jour les données à partir des données du model.
    void update_data(flag role) {
        if(role.test(model_base::Flag_Change_Flag))
            set_flags(m_index.flags());
    }

    //! Mutateur de flags.
    void set_flags(flag fl) {
        m_flags = fl;
        update_flags();
    }

    //! Met à jour le widget après un changement de drapeaux.
    virtual void update_flags() {
        setVisible(m_flags.test(model_base::Visible_Flag_Node));
        setEnabled(m_flags.test(model_base::Enable_Flag_Node));
    }
 };

/*! \ingroup groupe_Widget
 *\brief Classe mère des sous noeuds standard.
 */
class sub_node_widget : public indexed_widget {
    Q_OBJECT
protected:
    bool m_connexion_enable;             //!< État des connexions.
public:
    //! Constructeur.
    using indexed_widget::indexed_widget;

    //! Accesseur de l'état des connexions.
    bool connexion_enable() const noexcept
        {return m_connexion_enable;}

    //! Mutateur de l'état des connexions.
    void set_connexion(bool bb) noexcept
        {m_connexion_enable = bb;}

public slots:
    //! Met à jour les données du sous-noeud à partir des données du model.
    void update_data(mps::flag role) {
        set_connexion(false);
        update_data_sub_node(role);
        set_connexion(true);
    }

protected:
    //! Met à jour les données du sous-noeud à partir des données du model.
    virtual void update_data_sub_node(flag role)
        {indexed_widget::update_data(role);}
};

/*! \ingroup groupe_Widget
 *\brief Classe mère des widgets des noeuds de l'arbre.
 */
class node_widget : public indexed_widget {
    Q_OBJECT
public:
    using node_index = model_base::node_index;
    //! Etat de sélection du noeud.
    enum etat_selection {Initial = -1,
                        No_Selected,
                        Selected,
                        Current
                       };
    /*! \ingroup groupe_Widget
     *\brief Dessineur du noeud.
     */
    class node_painter {
    public:
        //! Constructeur.
        node_painter() = default;

        //! Destructeur.
        virtual ~node_painter();

        //! Dessine widget.
        virtual void paint(QWidget */*widget*/) {}

        //! Mutateur de l'état de sélection.
        virtual void set_etat_selection(etat_selection /*etat*/) {}
    };
protected:
    using sub_index = model_base::node_index::sub_index;
    etat_selection m_etat_selection = Initial;                  //!< État de sélection.
    std::multimap<sub_index,sub_node_widget*> m_cible_map;      //!< Association des sous-index et des sous-noeuds.
    std::unique_ptr<node_painter> m_painter;                    //!< Dessineur du noeud
public:
    //! Constructeur.
    node_widget(const node_index &index, QWidget *parent = nullptr)
        : indexed_widget(index, parent), m_painter(std::make_unique<node_painter>())
    {m_index.set_cible(model_base::Node_Cible);}

    //! Destructeur.
    ~node_widget() override;

    //! Ajoute un sous-noeud.
    void add_sub_node_widget(sub_node_widget *sub_node);

    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override {
        if(event->button() == Qt::LeftButton && m_flags.test(model_base::Left_Clickable_Flag_Node))
            emit left_clicked();
    }

    //! Dessine la noeud.
    void paintEvent(QPaintEvent */*event*/) override
        {m_painter->paint(this);}

    //! Le noeud devient courant.
    virtual void set_etat_selection(etat_selection etat) {
        if(m_etat_selection != etat) {
            m_etat_selection = etat;
            m_painter->set_etat_selection(etat);
            repaint();
        }
    }

    //! Mutateur du dessineur.
    void set_painter(std::unique_ptr<node_painter> &&painter) {
        m_painter = std::move(painter);
        repaint();
    }

public slots:
    //! Met à jour les données du widget à partir des données du model.
    virtual void update_data();

    //! Met à jour les données du widget pour l'index index à partir des données du model.
    virtual void update_data(const mps::widget::node_widget::node_index &index, mps::flag role);

signals:
    //! Emit lorsque le noeud est cliqué avec le bouton gauche.
    void left_clicked();

protected:
    //! Retire un sous noeud.
    void remove_sub_node_widget(QObject *sub_node)
        {m_cible_map.erase(static_cast<sub_node_widget*>(sub_node)->index().sub());}
};
}
/*! \ingroup groupe_delegate
 *\brief Espace de noms des delegates.
 */
namespace delegate {
/*! \ingroup groupe_delegate
 *\brief Delegate associé à un node_view.
 */
class abstract_node_delegate : public QObject {
    Q_OBJECT
public:
    using node_index = model_base::node_index;
    using node_widget = widget::node_widget;
    //! Constructeur.
    abstract_node_delegate(QObject *parent = nullptr);

    //! Crée un noeud.
    virtual node_widget *create_node(const node_index &index, QWidget *parent = nullptr) const = 0;

    //! Crée un node painter.
    virtual std::unique_ptr<node_widget::node_painter>
    node_painter(const mps::widget::node_widget::node_index &index) const = 0;
};
}
namespace widget {
class arc_node_view_widget;
/*! \ingroup groupe_Widget
 *\brief Vue pour un modèle hérité de abstract_node_model.
 */
class node_view : public QScrollArea {
    Q_OBJECT
public:
    //! Mode de sélection.
    enum mode_selection {No_Selection,
                        Single_Selection,
                        Multi_Selection,
                        Extended_Selection,
                        Continous_Selection
     };

    //! Outils des noeuds.
    enum tools_node {
        Expand_Tool,
        Elder_Tool,
        Brother_Tool,
        Del_Tool,
        End_Of_Tool = Del_Tool
    };

    /*! \ingroup groupe_Widget
     *\brief Dessinant le lien entre un noeud et ses déscendant.
     */
    class arc_painter {
    public:
        enum parametre {No_Margin = 0,
            Bottom_Node_Margin = 2,
            Left_Node_Margin = 3,
            Right_Node_Margin = 2,
            Top_Node_Margin = 3,
        };
        //! Constructeur.
        arc_painter() = default;

        //! Destructeur.
        virtual ~arc_painter() = default;

        //! Renvoie la marge au dessous du noeud.
        virtual int bottom_node_margin() const {return Bottom_Node_Margin;}

        //! Renvoie la marge au dessous de la zone outils.
        virtual int bottom_tools_zone_margin() const {return Bottom_Node_Margin;}

        //! Dessine l'arc liant les descendants.
        virtual void draw_arc(arc_node_view_widget */*arc*/) const {}

        //! Dessine la zone outils.
        virtual void draw_tool_zone(arc_node_view_widget */*arc*/) const {}

        //! Renvoie la marge à gauche pour tracer l'arc.
        virtual int left_expanded_margin() const {return No_Margin;}

        //! Renvoie la marge à gauche du noeud.
        virtual int left_node_margin() const {return Left_Node_Margin;}

        //! Renvoie la marge à droite du noeud.
        virtual int right_node_margin() const {return Right_Node_Margin;}

        //! Renvoie taille verticale de la zone outils.
        virtual int height_tool_zone() const {return No_Margin;}

        //! Renvoie la marge au dessus du noeud.
        virtual int top_node_margin() const {return Top_Node_Margin;}

        //! Renvoie taille horizontale de la zone de demande d'expansion.
        virtual int width_tool_zone(tools_node /*num*/) const {return No_Margin;}
    };
protected:
    friend arc_node_view_widget;
    using node_delegate = delegate::abstract_node_delegate;
    using node_model = model_base::item_node_model;
    using node_index = model_base::node_index;
    using select_model = model_base::node_selection_model;
    std::unique_ptr<arc_painter> m_arc_painter;             //!< Dessine les arc.
    node_delegate *m_delegate = nullptr;                    //!< Délégate de la vue.
    node_model *m_model = nullptr;                          //!< Model associé à la vue.
    select_model *m_selection_model = nullptr;              //!< Model de sélection.
    std::map<void*,arc_node_view_widget *> m_arc_map;       //!< Map des arc.
    mode_selection m_selection_mode = Single_Selection;     //!< Mode de sélection.
    bool m_connexion_update_data = true;                    //!< Etat de connexion de la mise à jour des données.
public:

    //! Constructeur.
    node_view(std::unique_ptr<arc_painter> &&painter = std::make_unique<arc_painter>(), QWidget *parent = nullptr);

    //! Destructeur.
    ~node_view() override
        {delete_root();}

    //! Gestion d'un click sur un noeud.
    void click_left_on(const node_index &index);

    //! Accesseur du delegate.
    node_delegate *delegate() const noexcept
        {return m_delegate;}

    //! Accesseur du model.
    node_model *model() const noexcept
        {return m_model;}

    //! Accesseur du model de sélection.
    mode_selection selection_mode() const noexcept
        {return m_selection_mode;}

    //! Accesseur du model de sélection.
    select_model *selection_model() const noexcept
        {return m_selection_model;}

    //! Mutateur du dessinateur des arcs.
    void set_arc_painter(std::unique_ptr<arc_painter> &&arc_painter)
        {m_arc_painter = std::move(arc_painter);}

    //! Selectionne l'index courant.
    void set_current_index(const node_index &index);

    //! Mutateur du delegate.
    void set_delegate(node_delegate *delegate);

    //! Mutateur du model.
    void set_model(node_model *model);

    //! Mutateur du mode de sélection.
    void set_selection_model(mode_selection mode)
        {m_selection_mode = mode;}

    //! Mutateur du model de sélection.
    void set_selection_model(select_model *selection_model);

public slots:
    //! Prend en compte l'insertion de nouveau noeud du model.
    void insert_nodes(const mps::widget::node_widget::node_index &parent, mps::numt pos, mps::numt count);

    //! Prend en compte la suppression de noeud du model.
    void remove_nodes(const mps::widget::node_widget::node_index &parent, mps::numt pos, mps::numt count);

    //! Met à jour toutes les données.
    void updateAllData();

    //! Met à jour les donnée du node_widget associé à l'index.
    void update_data(const mps::widget::node_widget::node_index &index, mps::flag role);

protected slots:
    //! Change le noeud courant.
    void current_changed(const mps::widget::node_widget::node_index &current,
                         const mps::widget::node_widget::node_index &previous);

    //! Vide le map d'arc et supprime l'arbre d'arc.
    void delete_root();

    //! Crée la racine de l'arbre d'arc.
    void reset_root();

    //! Change la sélection.
    void selection_changed(const std::list<node_index> &/*selected*/, const std::list<node_index> &/*deselected*/) {}
};

/*! \ingroup groupe_Widget
 *\brief Widget contenant un noeud et ses descendants.
 */
class arc_node_view_widget : public QWidget {
    Q_OBJECT
protected:
    node_view *m_view;                                  //!< Vue contenant le widget.
    node_widget *m_node = nullptr;                      //!< Widget de noeud.
    std::vector<arc_node_view_widget *> m_arc_child;    //!< Vecteur des arcs fils.
    bool m_draw_node = true;                            //!< Verrou de dessin des noeuds.
    bool m_expanded = false;                            //!< Etat de la branche.
    bool m_leaf = true;                                 //!< Le noeud est une feuille.
    const bool m_tool_zone;                             //!< Présence de la zone outils.
    const bool m_root;                                  //!< Le noeud est la racine.
    const bool m_node_arc_visible;                      //!< Le noeud et l'arc sont visible

public:
    //! Constructeur.
    arc_node_view_widget(node_widget *node, node_view *view, QWidget *parent = nullptr,
                         bool root = false, bool node_arc_visible = true, bool tool_zone = true);

    //! Constructeur.
    arc_node_view_widget(const model_base::node_index &index, node_view *view, QWidget *parent = nullptr,
                         bool root = false, bool node_arc_visible = true)
        : arc_node_view_widget(view->delegate()->create_node(index),view,parent,root,node_arc_visible,
                               index.flags().test(model_base::Tools_Zone_Flag_Node)) {}

    //! Destructeur.
    ~arc_node_view_widget() override
        {m_view->m_arc_map.erase(m_node->index().internal_pointer());}

    //! Accesseur du vecteur des arcs descendants directs.
    const std::vector<arc_node_view_widget *> &arc_child() const
        {return m_arc_child;}

    //! Place le noeud  et les suivants et ajuste les tailles.
    virtual void draw_node(bool next = false);

    //! Acceseur de l'état d'expansion.
    bool expanded() const noexcept
        {return m_expanded;}

    //! Hydrate les arc descendants directs.
    void hydrate_arc_child();

    //! Prend en compte l'insertion de noeuds enfants.
    void insert_nodes(numt pos, numt count);

    //! Accesseur de l'état de feuille.
    bool leaf() const noexcept
        {return m_leaf;}

    //! Accesseur du widget de noeud.
    node_widget *node() const noexcept
        {return m_node;}

    //! Suppresion de noeud enfant de first à last.
    void remove_nodes(numt pos, numt count);

    //! Accesseur de root.
    bool root() const noexcept
        {return m_root;}

    //! Mutateur de l'état d'expansion.
    void set_expanded(bool bb);

    //! Mutateur de l'état de feuille.
    void set_leaf(bool bb);

    //! Mutateur du widget de noeud.
    void set_node_widget(node_widget *widget);

    //! Mutateur du widget de noeud.
    void set_node_widget(const model_base::node_index &index) {
        set_node_widget(m_view->delegate()->create_node(index,this));
        m_node->update_data();
    }

    //! Taille souhaité du widget.
    QSize sizeHint() const override;

    //! Accesseur de la vue.
    node_view *view() const noexcept
        {return m_view;}
protected:
    //! Teste si le noeud doit être développer.
    bool must_be_expended() const noexcept
        {return !(m_expanded || m_arc_child.empty());}

    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override;

    //! Dessine la branche.
    void paintEvent(QPaintEvent *event) override;
};

/*! \ingroup groupe_Widget
 *\brief Widget de la racine.
 */
class root_node_view_widget : public arc_node_view_widget {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    root_node_view_widget(const model_base::node_index &index, node_view *view)
        : arc_node_view_widget(new node_widget(index),view,view,true,false) {
        set_expanded(true);
    }
};
}}
#endif // NODE_VIEW_H
