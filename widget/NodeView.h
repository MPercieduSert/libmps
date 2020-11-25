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
 * \brief Ensemble des classes des delegates.
 */

namespace widgetMPS {
using namespace typeMPS;
class NodeView;
class ArcNodeViewWidget;
/*! \ingroup groupeWidget
 * \brief Classe mère des widgets des noeuds de l'arbre.
 */
class AbstractNodeWidget : public QWidget {
    Q_OBJECT
public:
    using NodeIndex = modelMPS::NodeIndex;
    enum {NoType = modelMPS::TreeNodeModel::NoType,
         Rayon = 10,
         NoSelectedWidth = 2,
         SelectedWidth = NoSelectedWidth,
         CurrentWidth = 2 * SelectedWidth};
    //! Etat de sélection du noeud.
    enum EtatSelection {Initial = -1,
                        NoSelected,
                        Selected,
                        Current
                       };
protected:
    EtatSelection m_etatSelection = Initial;        //!< État de sélection.
    int m_widthLine = NoSelectedWidth;              //!< Épaisseur du trait.
    QColor m_colorLine;                             //!< Couleur de la ligne.
    const int m_type;                               //!< Type du Noeud.
    const NodeIndex m_index;                        //!< Index sur la donnée représentée par le noeud.
public:
    //! Constructeur.
    AbstractNodeWidget(const NodeIndex & index, ArcNodeViewWidget * parent = nullptr, int tp = NoType);

    //! Connecte les éléments du noeuds au model.
    virtual void connexion() const {}

    //! Déconnecte les éléments du noeuds au model.
    virtual void deconnexion() const {}

    //! Accesseur de l'index.
    const NodeIndex & index() const noexcept
        {return m_index;}

    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override;

    //! Dessine la noeud.
    void paintEvent(QPaintEvent * event) override;

    //! Le noeud devient courant.
    void setEtatSelection(EtatSelection etat);

    //! Accesseur du type de noeud.
    int type() const noexcept
        {return m_type;}
public slots:
    //! Met à jour les données du widget à partir des données du model.
    virtual void updateData() = 0;

    //! Met à jour les données du widget pour l'index index à partir des données du model.
    virtual void updateData(const NodeIndex & /*index*/) = 0;
};
}
/*! \ingroup groupeDelegate
 * \brief Espace de noms des delegates.
 */
namespace delegateMPS {
/*! \ingroup groupeDelegate
 * \brief Delegate associé à un NodeView.
 */
class AbstractNodeDelegate : public QObject {
public:
    using NodeIndex = modelMPS::NodeIndex;
    using AbstractNodeWidget = widgetMPS::AbstractNodeWidget;
    //! Constructeur.
    AbstractNodeDelegate(QObject * parent);
    //! Crée un widget
    virtual AbstractNodeWidget * createWidget(const NodeIndex &index, widgetMPS::ArcNodeViewWidget * parent = nullptr) const = 0;
};
}
namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Vue pour un modèle hérité de AbstractNodeModel.
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
protected:
    friend ArcNodeViewWidget;
    friend AbstractNodeWidget;
    using Delegate = delegateMPS::AbstractNodeDelegate;
    using Model = modelMPS::AbstractNodeModel;
    using NodeIndex = modelMPS::NodeIndex;
    using SelectionModel = modelMPS::NodeSelectionModel;
    Delegate * m_delegate = nullptr;                    //!< Délégate de la vue.
    Model * m_model = nullptr;                          //!< Model associé à la vue.
    SelectionMode m_selectionMode = SingleSelection;    //!< Mode de sélection.
    SelectionModel * m_selectionModel = nullptr;        //!< Model de sélection.
    std::map<void*,ArcNodeViewWidget *> m_arcMap;   //!< Map des arc.
public:
    //! Constructeur.
    NodeView(QWidget * parent = nullptr);

    //! Destructeur.
    ~NodeView()
        {deleteRoot();}

    //! Accesseur du delegate.
    Delegate * delegate() const noexcept
        {return m_delegate;}

    //! Accesseur du model.
    Model * model() const noexcept
        {return m_model;}

    //! Accesseur du model de sélection.
    SelectionMode selectionMode() const noexcept
        {return m_selectionMode;}

    //! Accesseur du model de sélection.
    SelectionModel * selectionModel() const noexcept
        {return m_selectionModel;}

    //! Selectionne l'index courant.
    void setCurrentIndex(const NodeIndex & index);

    //! Mutateur du delegate.
    void setDelegate(Delegate * delegate);

    //! Mutateur du model.
    void setModel(Model * model);

    //! Mutateur du mode de sélection.
    void setSelectionModel(SelectionMode selectionMode)
        {m_selectionMode = selectionMode;}

    //! Mutateur du model de sélection.
    void setSelectionModel(SelectionModel * selectionModel);

public slots:
    //! Prend en compte l'insertion de nouveau noeud du model.
    void insertNodes(const NodeIndex & parent, szt first, szt last);

    //! Prend en compte la suppression de noeud du model.
    void removeNodes(const NodeIndex & parent, szt first, szt last);

    //! Met à jour les donnée du AbstractNodeWidget associé à l'index.
    void updateData(const NodeIndex & index);

protected slots:
    //! Change le noeud courant.
    void currentChanged(const NodeIndex & current, const NodeIndex & previous);

    //! Vide le map d'arc et supprime l'arbre d'arc.
    void deleteRoot();

    //! Crée la racine de l'arbre d'arc.
    void resetRoot();

    //! Change la sélection.
    void selectionChanged(const std::list<NodeIndex> & selected, const std::list<NodeIndex> & deselected) {}
protected:
    //! Gestion d'un click sur un noeud.
    void clickLeftOn(const NodeIndex & index);
};

/*! \ingroup groupeWidget
 * \brief Widget associé à un noeud.
 */
class ArcNodeViewWidget : public QWidget {
    Q_OBJECT
protected:
    enum {BottomMargin = 2,
          LeftNodeMargin = 3,
          LeftExpandMargin = LeftNodeMargin + 10,
          RightMargin = 2,
          TopNodeMargin = 3,
          VSpacing = 3,
          HSpacing = 3,
          WidthLine = 2,
          HMaxLine = 100,
          WidthCircle = 0,
          Rayon = 3,
          Ecart = Rayon * 4,
          NbrCircle = 3,
          HSizeCircle = 2 * Rayon + Ecart * (NbrCircle - 1) + 2 * WidthCircle,
          VSizeCircle = 2 * Rayon + 2 * WidthCircle,
          RigthCircles = LeftNodeMargin + HSizeCircle
         };

    bool m_drawNode = true;                     //!< Verrou de dessin des noeuds.
    bool m_expanded = false;                    //!< Etat de la branche.
    bool m_leaf = true;                         //!< Le noeud est une feuille.
    const bool m_root;                          //!< Le noeud est la racine.
    NodeView * m_view;                          //!< Vue contenant le widget.
    AbstractNodeWidget * m_nodeWidget = nullptr;        //!< Widget de noeud.
    std::vector<ArcNodeViewWidget *> m_arcVec;  //!< Vecteur des arcs fils.
public:
    //! Constructeur.
    ArcNodeViewWidget(AbstractNodeWidget * node, NodeView * view, QWidget * parent = nullptr, bool root = false);

    //! Constructeur.
    ArcNodeViewWidget(const modelMPS::NodeIndex & index, NodeView * view, QWidget * parent = nullptr, bool root = false)
        : ArcNodeViewWidget(view->delegate()->createWidget(index),view,parent,root) {}

    //! Destructeur.
    ~ArcNodeViewWidget() override
        {m_view->m_arcMap.erase(m_nodeWidget->index().internalPointer());}

    //! Place le noeud  et les suivants et ajuste les tailles.
    void drawNode(bool next = false);

    //! Acceseur de l'état d'expansion.
    bool expanded() const noexcept
        {return m_expanded;}

    //! Prend en compte l'insertion de noeuds enfants.
    void insertNodes(szt first, szt last);

    //! Accesseur de l'état de feuille.
    bool leaf() const noexcept
        {return m_leaf;}

    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override;

    //! Accesseur du widget de noeud.
    AbstractNodeWidget * nodeWidget() const noexcept
        {return m_nodeWidget;}

    //! Dessine la branche.
    void paintEvent(QPaintEvent * event) override;

    //! Suppresion de noeud enfant de first à last.
    void removeNodes(szt first, szt last);

    //! Accesseur de root.
    bool root() const noexcept
        {return m_root;}

    //! Mutateur de l'état d'expansion.
    void setExpanded(bool bb);

    //! Mutateur de l'état de feuille.
    void setLeaf(bool bb);

    //! Mutateur du widget de noeud.
    void setNodeWidget(AbstractNodeWidget * widget);

    //! Mutateur du widget de noeud.
    void setNodeWidget(const modelMPS::NodeIndex & index) {
        setNodeWidget(m_view->delegate()->createWidget(index,this));
        m_nodeWidget->updateData();
    }

    //! Taille souhaité du widget.
    QSize sizeHint() const override;

    //! Accesseur de la vue.
    NodeView * view() const noexcept
        {return m_view;}
};
}
#endif // NODEVIEW_H
