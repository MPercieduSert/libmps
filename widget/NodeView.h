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
#include "NodeSelectionModel.h"

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
    using NodeIndex = modelMPS::NodeIndex;
    flag m_flags;             //!< Drapeaux associé aux noeud.
    NodeIndex m_index;                  //!< Index associé aux noeuds.
    QBoxLayout *m_mainLayout;          //!< Calque principale du sous-noeud.
public:
    //! Constructeur.
    IndexWidget(const NodeIndex &index, QWidget *parent = nullptr);

    //! Acceseur de l'index.
    NodeIndex index() const noexcept
        {return m_index;}

    //! Accesseur du drapeaux.
    flag flags() const noexcept
        {return m_flags;}

    //! Met à jour les données à partir des données du model.
    void updateData(flag role) {
        if(role.test(modelMPS::FlagRole))
            setFlags(m_index.flags());
    }

    //! Mutateur de flags.
    void setFlags(flag fl) {
        m_flags = fl;
        updateFlags();
    }

    //! Met à jour le widget après un changement de drapeaux.
    virtual void updateFlags() {
        setVisible(m_flags.test(modelMPS::VisibleFlagNode));
        setEnabled(m_flags.test(modelMPS::EnableFlagNode));
    }
 };

/*! \ingroup groupeWidget
 *\brief Classe mère des sous noeuds standard.
 */
class SubNodeWidget : public IndexWidget {
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
        updateDataSubNode(role);
        setConnexion(true);
    }

protected:
    //! Met à jour les données du sous-noeud à partir des données du model.
    virtual void updateDataSubNode(flag role)
        {IndexWidget::updateData(role);}
};

/*! \ingroup groupeWidget
 *\brief Classe mère des widgets des noeuds de l'arbre.
 */
class NodeWidget : public IndexWidget {
    Q_OBJECT
public:
    using NodeIndex = modelMPS::NodeIndex;
    //! Etat de sélection du noeud.
    enum EtatSelection {Initial = -1,
                        NoSelected,
                        Selected,
                        Current
                       };
    /*! \ingroup groupeWidget
     *\brief Dessineur du noeud.
     */
    class NodePainter {
    public:
        //! Constructeur.
        NodePainter() = default;

        //! Destructeur.
        virtual ~NodePainter() = default;

        //! Dessine widget.
        virtual void paint(QWidget */*widget*/) {}

        //! Mutateur de l'état de sélection.
        virtual void set_etatSelection(EtatSelection /*etat*/) {}
    };
protected:
    using SubIndex = modelMPS::NodeIndex::SubIndex;
    std::multimap<SubIndex,SubNodeWidget*> m_cibleMap;      //!< Association des sous-index et des sous-noeuds.
    EtatSelection m_etatSelection = Initial;                //!< État de sélection.
    std::unique_ptr<NodePainter> m_painter;                 //!< Dessineur du noeud
public:
    //! Constructeur.
    NodeWidget(const NodeIndex &index, QWidget *parent = nullptr)
        : IndexWidget(index, parent), m_painter(std::make_unique<NodePainter>()) {}

    //! Destructeur.
    ~NodeWidget() override;

    //! Ajoute un sous-noeud.
    void addSubNodeWidget(SubNodeWidget *subNode);

    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override {
        if(event->button() == Qt::LeftButton &&m_flags.test(modelMPS::LeftClickableFlagNode))
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
    void setPainter(std::unique_ptr<NodePainter> &&painter) {
        m_painter = std::move(painter);
        repaint();
    }

public slots:
    //! Met à jour les données du widget à partir des données du model.
    virtual void updateData();

    //! Met à jour les données du widget pour l'index index à partir des données du model.
    virtual void updateData(const NodeIndex &index, flag role);

signals:
    //! Emit lorsque le noeud est cliqué avec le bouton gauche.
    void leftClicked();

protected:
    //! Retire un sous noeud.
    void removeSubNodeWidget(QObject *subNode)
        {m_cibleMap.erase(static_cast<SubNodeWidget*>(subNode)->index().subIndex());}
};
}
/*! \ingroup groupeDelegate
 *\brief Espace de noms des delegates.
 */
namespace delegateMPS {
/*! \ingroup groupeDelegate
 *\brief Delegate associé à un NodeView.
 */
class AbstractNodeDelegate : public QObject {
    Q_OBJECT
public:
    using NodeIndex = modelMPS::NodeIndex;
    using NodeWidget = widgetMPS::NodeWidget;
    //! Constructeur.
    AbstractNodeDelegate(QObject *parent = nullptr);
    //! Crée un noeud.
    virtual NodeWidget *createNode(const NodeIndex &index, QWidget *parent = nullptr) const = 0;
};
}
namespace widgetMPS {
class ArcNodeViewWidget;
/*! \ingroup groupeWidget
 *\brief Vue pour un modèle hérité de AbstractNodeModel.
 */
class NodeView : public QScrollArea {
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
    enum toolsNode {
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
            BottomNodeMargin = 2,
            LeftNodeMargin = 3,
            RightNodeMargin = 2,
            TopNodeMargin = 3,
        };
        //! Constructeur.
        ArcPainter() = default;

        //! Destructeur.
        virtual ~ArcPainter() = default;

        //! Renvoie la marge au dessous du noeud.
        virtual int bottomNodeMargin() const {return BottomNodeMargin;}

        //! Renvoie la marge au dessous de la zone outils.
        virtual int bottomToolsZoneMargin() const {return BottomNodeMargin;}

        //! Dessine l'arc liant les descendants.
        virtual void drawArc(ArcNodeViewWidget */*arc*/) const {}

        //! Dessine la zone outils.
        virtual void drawToolZone(ArcNodeViewWidget */*arc*/) const {}

        //! Renvoie la marge à gauche pour tracer l'arc.
        virtual int leftExpandedMargin() const {return NoMargin;}

        //! Renvoie la marge à gauche du noeud.
        virtual int leftNodeMargin() const {return LeftNodeMargin;}

        //! Renvoie la marge à droite du noeud.
        virtual int rightNodeMargin() const {return RightNodeMargin;}

        //! Renvoie taille verticale de la zone outils.
        virtual int heightToolZone() const {return NoMargin;}

        //! Renvoie la marge au dessus du noeud.
        virtual int topNodeMargin() const {return TopNodeMargin;}

        //! Renvoie taille horizontale de la zone de demande d'expansion.
        virtual int widthToolZone(toolsNode /*num*/) const {return NoMargin;}
    };
protected:
    friend ArcNodeViewWidget;
    using Delegate = delegateMPS::AbstractNodeDelegate;
    using Model = modelMPS::ItemNodeModel;
    using NodeIndex = modelMPS::NodeIndex;
    using SelectionModel = modelMPS::NodeSelectionModel;
    bool m_connexionUpdateData = true;                  //!< Etat de connexion de la mise à jour des données.
    std::unique_ptr<ArcPainter> m_arcPainter;           //!< Dessine les arc.
    Delegate *m_delegate = nullptr;                    //!< Délégate de la vue.
    Model *m_model = nullptr;                          //!< Model associé à la vue.
    SelectionMode m_selectionMode = SingleSelection;    //!< Mode de sélection.
    SelectionModel *m_selectionModel = nullptr;        //!< Model de sélection.
    std::map<void*,ArcNodeViewWidget *> m_arcMap;       //!< Map des arc.
public:

    //! Constructeur.
    NodeView(std::unique_ptr<ArcPainter> &&arcPainter = std::make_unique<ArcPainter>(), QWidget *parent = nullptr);

    //! Destructeur.
    ~NodeView()
        {deleteRoot();}

    //! Gestion d'un click sur un noeud.
    void clickLeftOn(const NodeIndex &index);

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
    void setCurrentIndex(const NodeIndex &index);

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
    void insertNodes(const NodeIndex &parent, numt pos, numt count);

    //! Prend en compte la suppression de noeud du model.
    void removeNodes(const NodeIndex &parent, numt pos, numt count);

    //! Met à jour toutes les données.
    void updateAllData();

    //! Met à jour les donnée du NodeWidget associé à l'index.
    void updateData(const NodeIndex &index, flag role);

protected slots:
    //! Change le noeud courant.
    void currentChanged(const NodeIndex &current, const NodeIndex &previous);

    //! Vide le map d'arc et supprime l'arbre d'arc.
    void deleteRoot();

    //! Crée la racine de l'arbre d'arc.
    void resetRoot();

    //! Change la sélection.
    void selectionChanged(const std::list<NodeIndex> &/*selected*/, const std::list<NodeIndex> &/*deselected*/) {}
};

/*! \ingroup groupeWidget
 *\brief Widget contenant un noeud et ses descendants.
 */
class ArcNodeViewWidget : public QWidget {
    Q_OBJECT
protected:
    bool m_drawNode = true;                         //!< Verrou de dessin des noeuds.
    bool m_expanded = false;                        //!< Etat de la branche.
    bool m_leaf = true;                             //!< Le noeud est une feuille.
    const bool m_root;                              //!< Le noeud est la racine.
    const bool m_nodeArcVisible;                    //!< Le noeud et l'arc sont visible
    NodeView *m_view;                              //!< Vue contenant le widget.
    NodeWidget *m_nodeWidget = nullptr;            //!< Widget de noeud.
    std::vector<ArcNodeViewWidget *> m_arcChild;    //!< Vecteur des arcs fils.
public:
    //! Constructeur.
    ArcNodeViewWidget(NodeWidget *node, NodeView *view, QWidget *parent = nullptr, bool root = false, bool nodeArcVisible = true);

    //! Constructeur.
    ArcNodeViewWidget(const modelMPS::NodeIndex &index, NodeView *view, QWidget *parent = nullptr, bool root = false, bool nodeArcVisible = true)
        : ArcNodeViewWidget(view->delegate()->createNode(index),view,parent,root,nodeArcVisible) {}

    //! Destructeur.
    ~ArcNodeViewWidget() override
        {m_view->m_arcMap.erase(m_nodeWidget->index().internalPointer());}

    //! Accesseur du vecteur des arcs descendants directs.
    const std::vector<ArcNodeViewWidget *> &arcChild() const
        {return m_arcChild;}

    //! Place le noeud  et les suivants et ajuste les tailles.
    virtual void drawNode(bool next = false);

    //! Acceseur de l'état d'expansion.
    bool expanded() const noexcept
        {return m_expanded;}

    //! Prend en compte l'insertion de noeuds enfants.
    void insertNodes(numt pos, numt count);

    //! Accesseur de l'état de feuille.
    bool leaf() const noexcept
        {return m_leaf;}

    //! Accesseur du widget de noeud.
    NodeWidget *nodeWidget() const noexcept
        {return m_nodeWidget;}

    //! Suppresion de noeud enfant de first à last.
    void removeNodes(numt pos, numt count);

    //! Accesseur de root.
    bool root() const noexcept
        {return m_root;}

    //! Mutateur de l'état d'expansion.
    void setExpanded(bool bb);

    //! Mutateur de l'état de feuille.
    void setLeaf(bool bb);

    //! Mutateur du widget de noeud.
    void setNodeWidget(NodeWidget *widget);

    //! Mutateur du widget de noeud.
    void setNodeWidget(const modelMPS::NodeIndex &index) {
        setNodeWidget(m_view->delegate()->createNode(index,this));
        m_nodeWidget->updateData();
    }

    //! Taille souhaité du widget.
    QSize sizeHint() const override;

    //! Accesseur de la vue.
    NodeView *view() const noexcept
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
class RootNodeViewWidget : public ArcNodeViewWidget {
    Q_OBJECT
protected:

public:
    //! Constructeur.
    RootNodeViewWidget(const modelMPS::NodeIndex &index, NodeView *view)
        : ArcNodeViewWidget(new NodeWidget(index),view,view,true,false) {
        setExpanded(true);
    }
};
}
#endif // NODEVIEW_H
