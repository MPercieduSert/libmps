/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/10/2020
 */
#ifndef NODEVIEW_H
#define NODEVIEW_H

#include <QHBoxLayout>
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
class ArcWidget;
/*! \ingroup groupeWidget
 * \brief Vue pour un modèle hérité de AbstractNodeModel.
 */
class NodeView : public QScrollArea {
    Q_OBJECT
public:
    friend ArcWidget;
protected:
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
class ArcWidget : public QWidget {
    Q_OBJECT
protected:
    enum {NodeWidgetIndice = 0};
    bool m_expandEtat;                      //!< Etat d'expansion des fils.
    NodeView * m_view;                      //!< Vue contenant le widget.
    NodeWidget * m_nodeWidget = nullptr;    //!< Widget de noeud.
    QPushButton * m_expandButton;           //!< Bouton d'expansion des fils du noeud.
    QVBoxLayout * m_expandLayout;           //!< Calque d'expansion des fils du noeud.
    QHBoxLayout * m_mainLayout;             //!< Calque Principale.
    QVBoxLayout * m_secondLayout;           //!< Calque secondaire.
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
        {return m_expandEtat;}

    //! Accesseur du widget de noeud.
    NodeWidget * nodeWidget() const noexcept
        {return m_nodeWidget;}

    //! Mutateur du widget de noeud.
    void setNodeWidget(NodeWidget * widget);

    //! Mutateur du widget de noeud.
    void setNodeWidget(const modelMPS::NodeIndex & index) {
        setNodeWidget(m_view->delegate()->createWidget(index,this));
        m_nodeWidget->updateData();
    }

    //! Taille souhaité du widget.
    QSize sizeHint() const override;

public slots:
    //! Mutateur de l'état d'expansion des fils.
    void setExpandEtat(bool bb);
};
}
#endif // NODEVIEW_H
