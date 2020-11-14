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
#include "AbstractNodeModel.h"
/*! \defgroup groupeDelegate Delegate
 * \brief Ensemble des classes des delegates.
 */
namespace widgetMPS {
class NodeView;
class ArcNodeViewWidget;
/*! \ingroup groupeWidget
 * \brief Classe mère des widgets des noeuds de l'arbre.
 */
class NodeWidget : public QFrame {
    Q_OBJECT
public:
    using NodeIndex = modelMPS::NodeIndex;
    enum {NoType = modelMPS::TreeNodeModel::NoType};
protected:
    const int m_type;           //!< Type du Noeud.
    const NodeIndex m_index;    //!< Index sur la donnée représentée par le noeud.
public:
    //! Constructeur.
    NodeWidget(const NodeIndex & index, QWidget * parent = nullptr, int tp = NoType);

    //! Connecte les éléments du noeuds au model.
    virtual void connexion() const {}

    //! Déconnecte les éléments du noeuds au model.
    virtual void deconnexion() const {}

    //! Accesseur de l'index.
    const NodeIndex & index() const noexcept
        {return m_index;}

    //! Accesseur du type de noeud.
    int type() const noexcept
        {return m_type;}
public slots:
    //! Met à jour les données du widget à partir des données du model.
    virtual void updateData() {}
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
    using NodeWidget = widgetMPS::NodeWidget;
    //! Constructeur.
    AbstractNodeDelegate(QObject * parent);
    //! Crée un widget
    virtual NodeWidget * createWidget(const NodeIndex &index, QWidget * parent = nullptr) const = 0;
};
}
namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Vue pour un modèle hérité de AbstractNodeModel.
 */
class NodeView : public QScrollArea {
    Q_OBJECT
protected:
    friend ArcNodeViewWidget;
    using NodeIndex = modelMPS::NodeIndex;
    delegateMPS::AbstractNodeDelegate * m_delegate = nullptr;       //!< Délégate de la vue.
    modelMPS::AbstractNodeModel * m_model = nullptr;                //!< Model associé à la vue.
    std::map<NodeIndex,ArcNodeViewWidget *> m_arcMap;               //!< Map des arc.
public:
    //! Constructeur.
    NodeView(QWidget * parent = nullptr);

    //! Destructeur.
    ~NodeView()
        {deleteRoot();}

    //! Accesseur du delegate.
    delegateMPS::AbstractNodeDelegate * delegate() const noexcept
        {return m_delegate;}

    //! Accesseur du model.
    modelMPS::AbstractNodeModel * model() const noexcept
        {return m_model;}

    //! Mutateur du delegate.
    void setDelegate(delegateMPS::AbstractNodeDelegate * delegate);

    //! Mutateur du model.
    void setModel(modelMPS::AbstractNodeModel * model);

public slots:
    //! Prend en compte l'insertion de nouvelle ligne.
    void insertRows(const NodeIndex & parent, int first, int last);

    //! Met à jour les donnée du NodeWidget associé à l'index.
    void updateData(const NodeIndex & index);

protected slots:
    //! Vide le map d'arc et supprime l'arbre d'arc.
    void deleteRoot();

    //! Crée la racine de l'arbre d'arc.
    void resetRoot();
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
          HSpacing = 2,
          WidthLine = 2,
          HMaxLine = 100,
          WidthCircle = 0,
          Rayon = 3,
          Ecart = 10,
          NbrCircle = 3,
          HSizeCircle = 2 * Rayon + Ecart * (NbrCircle - 1) + 2 * WidthCircle,
          VSizeCircle = 2 * Rayon + 2 * WidthCircle,
          RigthCircles = LeftNodeMargin + HSizeCircle};
    bool m_adjustSize = true;                   //!< Verrou d'ajustment de la taille.
    bool m_expanded = false;                    //!< Etat de la branche.
    bool m_leaf = true;                         //!< Le noeud est une feuille.
    const bool m_root;                          //!< Le noeud est la racine.
    NodeView * m_view;                          //!< Vue contenant le widget.
    NodeWidget * m_nodeWidget = nullptr;        //!< Widget de noeud.
    std::vector<ArcNodeViewWidget *> m_arcVec;  //!< Vecteur des arcs fils.
public:
    //! Constructeur.
    ArcNodeViewWidget(NodeWidget * node, NodeView * view, QWidget * parent = nullptr, bool root = false);

    //! Constructeur.
    ArcNodeViewWidget(const modelMPS::NodeIndex & index, NodeView * view, QWidget * parent = nullptr, bool root = false)
        : ArcNodeViewWidget(view->delegate()->createWidget(index),view,parent,root) {}

    //! Destructeur.
    ~ArcNodeViewWidget() override
        {m_view->m_arcMap.erase(m_nodeWidget->index());}

    //! Ajuste la taille.
    void adjustSize() {
        if(m_adjustSize) {
            QWidget::adjustSize();
            if(!m_root)
                static_cast<ArcNodeViewWidget *>(parentWidget())->moveNextChild(this);
        }
    }

    //! Acceseur de l'état d'expansion.
    bool expanded() const noexcept
        {return m_expanded;}

    //! Prend en compte l'insertion de nouvelles lignes.
    void insertRows(int first, int last);

    //! Accesseur de l'état de feuille.
    bool leaf() const noexcept
        {return m_leaf;}

    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override;

    //! Accesseur du widget de noeud.
    NodeWidget * nodeWidget() const noexcept
        {return m_nodeWidget;}

    //! Dessine la branche.
    void paintEvent(QPaintEvent * event) override;

    //! Accesseur de root.
    bool root() const noexcept
        {return m_root;}

    //! Mutateur de l'état d'expansion.
    void setExpanded(bool bb);

    //! Mutateur de l'état de feuille.
    void setLeaf(bool bb);

    //! Mutateur du widget de noeud.
    void setNodeWidget(NodeWidget * widget);

    //! Mutateur du widget de noeud.
    void setNodeWidget(const modelMPS::NodeIndex & index) {
        setNodeWidget(m_view->delegate()->createWidget(index,this));
        m_nodeWidget->updateData();
    }

    //! Taille souhaité du widget.
    QSize sizeHint() const override;

protected:
    //! Déplace les enfants suivants après changment de taille d'un des enfants.
    void moveNextChild(ArcNodeViewWidget * arc);
};
}
#endif // NODEVIEW_H
