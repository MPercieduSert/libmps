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
/*! \ingroup groupeWidget
 * \brief Espace de noms des widgets de la vue nodeView.
 */
namespace nodeViewWidget {
class ArcWidget;
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
}}
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
    using NodeWidget = widgetMPS::nodeViewWidget::NodeWidget;
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
    using ArcWidget = nodeViewWidget::ArcWidget;
    friend ArcWidget;
    using NodeIndex = modelMPS::NodeIndex;
    delegateMPS::AbstractNodeDelegate * m_delegate = nullptr;       //!< Délégate de la vue.
    modelMPS::AbstractNodeModel * m_model = nullptr;                //!< Model associé à la vue.
    std::map<NodeIndex,ArcWidget *> m_arcMap;                       //!< Map des arc.
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
namespace nodeViewWidget {
/*! \ingroup groupeWidget
 * \brief Widget dessinant une branche de l'arbre.
 */
class BrancheWidget : public QWidget {
    Q_OBJECT
protected:
    bool m_expanded = false;        //!< Etat de la branche.
public:
    enum {HSize = 10,
          VSize = 20,
         WidthLine = 3,
         WidthCircle = 2,
         Rayon = 2,
         Ecart = 10,
         NbrCircle = 3,
         HSizeCircle = 2 * Rayon + Ecart * (NbrCircle - 1) + WidthCircle,
         VSizeCircle = 2 * Rayon + WidthCircle};
    //! Constructeur.
    BrancheWidget(bool expandState = false, QWidget * parent = nullptr);

    //! Acceseur de l'état d'expansion.
    bool expanded() const noexcept
        {return m_expanded;}

    //! Dessine la branche.
    void paintEvent(QPaintEvent * event) override;

    //! Gestionnaire de click de souris.
    void mousePressEvent(QMouseEvent *event) override;

    //! Mutateur de l'état d'expansion.
    void setExpanded(bool bb);

    //! Taille souhaité du widget.
    QSize sizeHint() const override;

signals:
    //! Signal emit lors du changement d'état d'expansion.
    void expandStateChanged(bool bb);
};
/*! \ingroup groupeWidget
 * \brief Widget associé à un noeud.
 */
class ArcWidget : public QWidget {
    Q_OBJECT
protected:
    enum {NodeWidgetIndice = 0};
    bool m_leaf = true;                            //!< Le noeud est une feuille.
    NodeView * m_view;                      //!< Vue contenant le widget.
    NodeWidget * m_nodeWidget = nullptr;    //!< Widget de noeud.
    BrancheWidget * m_brancheWgt;            //!< Branche d'expansion des fils du noeud.
    QHBoxLayout * m_brancheLayout;           //!< Calque secondaire.
    QVBoxLayout * m_expandLayout;           //!< Calque d'expansion des fils du noeud.
    QVBoxLayout * m_mainLayout;             //!< Calque Principale.
public:
    //! Constructeur.
    ArcWidget(NodeWidget * node, NodeView * view, QWidget * parent = nullptr);

    //! Constructeur.
    ArcWidget(const modelMPS::NodeIndex & index, NodeView * view, QWidget * parent = nullptr)
        : ArcWidget(view->delegate()->createWidget(index,this),view,parent) {}

    //! Destructeur.
    ~ArcWidget() override
        {m_view->m_arcMap.erase(m_nodeWidget->index());}

    //! Accesseur de l'état d'expansion des fils.
    bool expandEtat() const noexcept
        {return m_brancheWgt->expanded();}

    //! Prend en compte l'insertion de nouvelles lignes.
    void insertRows(int first, int last);

    //! Accesseur de l'état de feuille.
    bool leaf() const noexcept
        {return m_leaf;}

    //! Accesseur du widget de noeud.
    NodeWidget * nodeWidget() const noexcept
        {return m_nodeWidget;}

    //! Mutateur de l'état d'expansion des fils.
    void setExpandEtat(bool bb);

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
};
}}
#endif // NODEVIEW_H
