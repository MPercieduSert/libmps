/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef NODEVIEW_H
#define NODEVIEW_H

#include <QGuiApplication>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include "node_ptrSelectionModel.h"

/*! \defgroup groupeDelegate Delegate
 *\brief Ensemble des classes des delegates.
 */

namespace widgetMPS {
using namespace type_mps;
/*! \ingroup groupeWidget
 *\brief Classe possédant un index et calque principal orienté.
 */
class IndexWidget : public QWidget {
    Q_OBJECT
protected:
    using node_ptrIndex = model_base::node_ptrIndex;
    flag m_flags;             //!< Drapeaux associé aux noeud.
    node_ptrIndex m_index;                  //!< Index associé aux noeuds.
    QBoxLayout *m_mainLayout;          //!< Calque principale du sous-noeud.
public:
    //! Constructeur.
    IndexWidget(const node_ptrIndex &index, QWidget *parent = nullptr);

    //! Acceseur de l'index.
    node_ptrIndex index() const noexcept
        {return m_index;}

    //! Accesseur du drapeaux.
    flag flags() const noexcept
        {return m_flags;}

    //! Met à jour les données à partir des données du model.
    void updateData(flag role) {
        if(role.test(model_base::Flag_Role))
            setFlags(m_index.flags());
    }

    //! Mutateur de flags.
    void setFlags(flag fl) {
        m_flags = fl;
        updateFlags();
    }

    //! Met à jour le widget après un changement de drapeaux.
    virtual void updateFlags() {
        setVisible(m_flags.test(model_base::VisibleFlagnode_ptr));
        setEnabled(m_flags.test(model_base::EnableFlagnode_ptr));
    }
 };

/*! \ingroup groupeWidget
 *\brief Classe mère des sous noeuds standard.
 */
class Subnode_ptrWidget : public IndexWidget {
    Q_OBJECT
protected:
    bool m_connexionEnable;             //!< État des connexions.
public:
    //! Constructeur.
    using IndexWidget::IndexWidget;

    //! Accesseur de l'état des connexions.
    bool connexionEnable() const noexcept
        {return m_connexionEnable;}

    //! Mutateur de l'état des connexions.
    void setConnexion(bool bb) noexcept
        {m_connexionEnable = bb;}

public slots:
    //! Met à jour les données du sous-noeud à partir des données du model.
    void updateData(flag role) {
        setConnexion(false);
        updateDataSubnode_ptr(role);
        setConnexion(true);
    }

protected:
    //! Met à jour les données du sous-noeud à partir des données du model.
    virtual void updateDataSubnode_ptr(flag role)
        {IndexWidget::updateData(role);}
};

/*! \ingroup groupeWidget
 *\brief Classe mère des widgets des noeuds de l'arbre.
 */
class node_ptrWidget : public IndexWidget {
    Q_OBJECT
public:
    using node_ptrIndex = model_base::node_ptrIndex;
    //! Etat de sélection du noeud.
    enum EtatSelection {Initial = -1,
                        NoSelected,
                        Selected,
                        Current
                       };
    /*! \ingroup groupeWidget
     *\brief Dessineur du noeud.
     */
    class node_ptrPainter {
    public:
        //! Constructeur.
        node_ptrPainter() = default;

        //! Destructeur.
        virtual ~node_ptrPainter() = default;

        //! Dessine widget.
        virtual void paint(QWidget */*widget*/) {}

        //! Mutateur de l'état de sélection.
        virtual void set_etatSelection(EtatSelection /*etat*/) {}
    };
protected:
    using SubIndex = model_base::node_ptrIndex::SubIndex;
    std::multimap<SubIndex,Subnode_ptrWidget*> m_cibleMap;      //!< Association des sous-index et des sous-noeuds.
    EtatSelection m_etatSelection = Initial;                //!< État de sélection.
    std::unique_ptr<node_ptrPainter> m_painter;                 //!< Dessineur du noeud
public:
    //! Constructeur.
    node_ptrWidget(const node_ptrIndex &index, QWidget *parent = nullptr)
        : IndexWidget(index, parent), m_painter(std::make_unique<node_ptrPainter>()) {}

    //! Destructeur.
    ~node_ptrWidget() override;

    //! Ajoute un sous-noeud.
    void addSubnode_ptrWidget(Subnode_ptrWidget *subnode_ptr);

    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override {
        if(event->button() == Qt::LeftButton &&m_flags.test(model_base::LeftClickableFlagnode_ptr))
            emit leftClicked();
    }

    //! Dessine la noeud.
    void paintEvent(QPaintEvent */*event*/) override
        {m_painter->paint(this);}

    //! Le noeud devient courant.
    virtual void set_etatSelection(EtatSelection etat) {
        if(m_etatSelection != etat) {
            m_etatSelection = etat;
            m_painter->set_etatSelection(etat);
            repaint();
        }
    }

    //! Mutateur du dessineur.
    void setPainter(std::unique_ptr<node_ptrPainter> &&painter) {
        m_painter = std::move(painter);
        repaint();
    }

public slots:
    //! Met à jour les données du widget à partir des données du model.
    virtual void updateData();

    //! Met à jour les données du widget pour l'index index à partir des données du model.
    virtual void updateData(const node_ptrIndex &index, flag role);

signals:
    //! Emit lorsque le noeud est cliqué avec le bouton gauche.
    void leftClicked();

protected:
    //! Retire un sous noeud.
    void removeSubnode_ptrWidget(QObject *subnode_ptr)
        {m_cibleMap.erase(static_cast<Subnode_ptrWidget*>(subnode_ptr)->index().subIndex());}
};
}
/*! \ingroup groupeDelegate
 *\brief Espace de noms des delegates.
 */
namespace delegateMPS {
/*! \ingroup groupeDelegate
 *\brief Delegate associé à un node_ptrView.
 */
class Abstractnode_ptrDelegate : public QObject {
    Q_OBJECT
public:
    using node_ptrIndex = model_base::node_ptrIndex;
    using node_ptrWidget = widgetMPS::node_ptrWidget;
    //! Constructeur.
    Abstractnode_ptrDelegate(QObject *parent = nullptr);
    //! Crée un noeud.
    virtual node_ptrWidget *createnode_ptr(const node_ptrIndex &index, QWidget *parent = nullptr) const = 0;
};
}
namespace widgetMPS {
class Arcnode_ptrViewWidget;
/*! \ingroup groupeWidget
 *\brief Vue pour un modèle hérité de Abstractnode_ptrModel.
 */
class node_ptrView : public QScrollArea {
    Q_OBJECT
public:
    //! Mode de sélection.
    enum SelectionMode {NoSelection,
                        SingleSelection,
                        MultiSelection,
                        ExtendedSelection,
                        ContinousSelection
     };

    //! Outils des noeuds.
    enum toolsnode_ptr {
        ExpandTool,
        ElderTool,
        BrotherTool,
        DelTool,
        EndOfTool = DelTool
    };

    /*! \ingroup groupeWidget
     *\brief Dessinant le lien entre un noeud et ses déscendant.
     */
    class ArcPainter {
    public:
        enum parametre {NoMargin = 0,
            Bottomnode_ptrMargin = 2,
            Leftnode_ptrMargin = 3,
            Rightnode_ptrMargin = 2,
            Topnode_ptrMargin = 3,
        };
        //! Constructeur.
        ArcPainter() = default;

        //! Destructeur.
        virtual ~ArcPainter() = default;

        //! Renvoie la marge au dessous du noeud.
        virtual int bottomnode_ptrMargin() const {return Bottomnode_ptrMargin;}

        //! Renvoie la marge au dessous de la zone outils.
        virtual int bottomToolsZoneMargin() const {return Bottomnode_ptrMargin;}

        //! Dessine l'arc liant les descendants.
        virtual void drawArc(Arcnode_ptrViewWidget */*arc*/) const {}

        //! Dessine la zone outils.
        virtual void drawToolZone(Arcnode_ptrViewWidget */*arc*/) const {}

        //! Renvoie la marge à gauche pour tracer l'arc.
        virtual int leftExpandedMargin() const {return NoMargin;}

        //! Renvoie la marge à gauche du noeud.
        virtual int leftnode_ptrMargin() const {return Leftnode_ptrMargin;}

        //! Renvoie la marge à droite du noeud.
        virtual int rightnode_ptrMargin() const {return Rightnode_ptrMargin;}

        //! Renvoie taille verticale de la zone outils.
        virtual int heightToolZone() const {return NoMargin;}

        //! Renvoie la marge au dessus du noeud.
        virtual int topnode_ptrMargin() const {return Topnode_ptrMargin;}

        //! Renvoie taille horizontale de la zone de demande d'expansion.
        virtual int widthToolZone(toolsnode_ptr /*num*/) const {return NoMargin;}
    };
protected:
    friend Arcnode_ptrViewWidget;
    using Delegate = delegateMPS::Abstractnode_ptrDelegate;
    using Model = model_base::item_node_model;
    using node_ptrIndex = model_base::node_ptrIndex;
    using SelectionModel = model_base::node_ptrSelectionModel;
    bool m_connexionUpdateData = true;                  //!< Etat de connexion de la mise à jour des données.
    std::unique_ptr<ArcPainter> m_arcPainter;           //!< Dessine les arc.
    Delegate *m_delegate = nullptr;                    //!< Délégate de la vue.
    Model *m_model = nullptr;                          //!< Model associé à la vue.
    SelectionMode m_selectionMode = SingleSelection;    //!< Mode de sélection.
    SelectionModel *m_selectionModel = nullptr;        //!< Model de sélection.
    std::map<void*,Arcnode_ptrViewWidget *> m_arcMap;       //!< Map des arc.
public:

    //! Constructeur.
    node_ptrView(std::unique_ptr<ArcPainter> &&arcPainter = std::make_unique<ArcPainter>(), QWidget *parent = nullptr);

    //! Destructeur.
    ~node_ptrView()
        {deleteRoot();}

    //! Gestion d'un click sur un noeud.
    void clickLeftOn(const node_ptrIndex &index);

    //! Accesseur du delegate.
    Delegate *delegate() const noexcept
        {return m_delegate;}

    //! Accesseur du model.
    Model *model() const noexcept
        {return m_model;}

    //! Accesseur du model de sélection.
    SelectionMode selectionMode() const noexcept
        {return m_selectionMode;}

    //! Accesseur du model de sélection.
    SelectionModel *selectionModel() const noexcept
        {return m_selectionModel;}

    //! Mutateur du dessinateur des arcs.
    void setArcPainter(std::unique_ptr<ArcPainter> &&arcPainter)
        {m_arcPainter = std::move(arcPainter);}

    //! Selectionne l'index courant.
    void setCurrentIndex(const node_ptrIndex &index);

    //! Mutateur du delegate.
    void setDelegate(Delegate *delegate);

    //! Mutateur du model.
    void setModel(Model *model);

    //! Mutateur du mode de sélection.
    void setSelectionModel(SelectionMode selectionMode)
        {m_selectionMode = selectionMode;}

    //! Mutateur du model de sélection.
    void setSelectionModel(SelectionModel *selectionModel);

public slots:
    //! Prend en compte l'insertion de nouveau noeud du model.
    void insert_nodes(const node_ptrIndex &parent, numt pos, numt count);

    //! Prend en compte la suppression de noeud du model.
    void removenode_ptrs(const node_ptrIndex &parent, numt pos, numt count);

    //! Met à jour toutes les données.
    void updateAllData();

    //! Met à jour les donnée du node_ptrWidget associé à l'index.
    void updateData(const node_ptrIndex &index, flag role);

protected slots:
    //! Change le noeud courant.
    void currentChanged(const node_ptrIndex &current, const node_ptrIndex &previous);

    //! Vide le map d'arc et supprime l'arbre d'arc.
    void deleteRoot();

    //! Crée la racine de l'arbre d'arc.
    void resetRoot();

    //! Change la sélection.
    void selectionChanged(const std::list<node_ptrIndex> &/*selected*/, const std::list<node_ptrIndex> &/*deselected*/) {}
};

/*! \ingroup groupeWidget
 *\brief Widget contenant un noeud et ses descendants.
 */
class Arcnode_ptrViewWidget : public QWidget {
    Q_OBJECT
protected:
    bool m_drawnode_ptr = true;                         //!< Verrou de dessin des noeuds.
    bool m_expanded = false;                        //!< Etat de la branche.
    bool m_leaf = true;                             //!< Le noeud est une feuille.
    const bool m_root;                              //!< Le noeud est la racine.
    const bool m_nodeArcVisible;                    //!< Le noeud et l'arc sont visible
    node_ptrView *m_view;                              //!< Vue contenant le widget.
    node_ptrWidget *m_nodeWidget = nullptr;            //!< Widget de noeud.
    std::vector<Arcnode_ptrViewWidget *> m_arcChild;    //!< Vecteur des arcs fils.
public:
    //! Constructeur.
    Arcnode_ptrViewWidget(node_ptrWidget *node, node_ptrView *view, QWidget *parent = nullptr, bool root = false, bool nodeArcVisible = true);

    //! Constructeur.
    Arcnode_ptrViewWidget(const model_base::node_ptrIndex &index, node_ptrView *view, QWidget *parent = nullptr, bool root = false, bool nodeArcVisible = true)
        : Arcnode_ptrViewWidget(view->delegate()->createnode_ptr(index),view,parent,root,nodeArcVisible) {}

    //! Destructeur.
    ~Arcnode_ptrViewWidget() override
        {m_view->m_arcMap.erase(m_nodeWidget->index().internal_pointer());}

    //! Accesseur du vecteur des arcs descendants directs.
    const std::vector<Arcnode_ptrViewWidget *> &arcChild() const
        {return m_arcChild;}

    //! Place le noeud  et les suivants et ajuste les tailles.
    virtual void drawnode_ptr(bool next = false);

    //! Acceseur de l'état d'expansion.
    bool expanded() const noexcept
        {return m_expanded;}

    //! Prend en compte l'insertion de noeuds enfants.
    void insert_nodes(numt pos, numt count);

    //! Accesseur de l'état de feuille.
    bool leaf() const noexcept
        {return m_leaf;}

    //! Accesseur du widget de noeud.
    node_ptrWidget *nodeWidget() const noexcept
        {return m_nodeWidget;}

    //! Suppresion de noeud enfant de first à last.
    void removenode_ptrs(numt pos, numt count);

    //! Accesseur de root.
    bool root() const noexcept
        {return m_root;}

    //! Mutateur de l'état d'expansion.
    void setExpanded(bool bb);

    //! Mutateur de l'état de feuille.
    void setLeaf(bool bb);

    //! Mutateur du widget de noeud.
    void setnode_ptrWidget(node_ptrWidget *widget);

    //! Mutateur du widget de noeud.
    void setnode_ptrWidget(const model_base::node_ptrIndex &index) {
        setnode_ptrWidget(m_view->delegate()->createnode_ptr(index,this));
        m_nodeWidget->updateData();
    }

    //! Taille souhaité du widget.
    QSize sizeHint() const override;

    //! Accesseur de la vue.
    node_ptrView *view() const noexcept
        {return m_view;}
protected:
    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override;

    //! Dessine la branche.
    void paintEvent(QPaintEvent *event) override;
};

/*! \ingroup groupeWidget
 *\brief Widget contenant un noeud et ses descendants.
 */
class Rootnode_ptrViewWidget : public Arcnode_ptrViewWidget {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    Rootnode_ptrViewWidget(const model_base::node_ptrIndex &index, node_ptrView *view)
        : Arcnode_ptrViewWidget(new node_ptrWidget(index),view,view,true,false) {
        setExpanded(true);
    }
};
}
#endif // NODEVIEW_H
