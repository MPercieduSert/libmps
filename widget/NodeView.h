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
/*! \ingroup groupeWidget
 * \brief Classe mère des widgets des noeuds de l'arbre.
 */
class NodeWidget : public QWidget {
    Q_OBJECT
public:
    using NodeIndex = modelMPS::NodeIndex;
protected:
    NodeIndex m_index;      //! Index sur la donnée représentée par le noeud.
public:
    //! Constructeur.
    NodeWidget(const NodeIndex & index, QWidget * parent = nullptr);

public slots:
    //! Met à jour les données du widget à partir des données du model.
    virtual void updateData() {}
};
}
/*! \ingroup groupeDelegate
 * \brief Espace de noms des delegates.
 */
namespace delegateMPS {
/*! \ingroup groupeWidget
 * \brief Delegate associé à un NodeView.
 */
class AbstractNodeDelegate : public QObject {
public:
    using NodeIndex = modelMPS::NodeIndex;
    using NodeWidget = widgetMPS::NodeWidget;
    //! Constructeur.
    AbstractNodeDelegate(QObject * parent);
    //! Crée un widget
    virtual NodeWidget * createWidget(const NodeIndex &index) const = 0;

};
}
namespace widgetMPS {

class NodeView : public QScrollArea {
    Q_OBJECT
public:
    /*! \ingroup groupeWidget
     * \brief Widget associé à un noeud.
     */
    class ArcWidget : public QWidget {
        Q_OBJECT
    protected:
        enum {NodeWidgetIndice = 0};
        bool m_expandEtat;              //!< Etat d'expansion des fils.
        NodeWidget * m_nodeWidget = nullptr;      //!< Widget de noeud.
        QPushButton * m_expandButton;   //!< Bouton d'expansion des fils du noeud.
        QVBoxLayout * m_expandLayout;   //!< Calque d'expansion des fils du noeud.
        QHBoxLayout * m_mainLayout;     //!< Calque Principale.
        QVBoxLayout * m_secondLayout;   //!< Calque secondaire.
    public:
        //! Constructeur.
        ArcWidget(QWidget * parent = nullptr);

        //! Accesseur de l'état d'expansion des fils.
        bool expandEtat() const
            {return m_expandEtat;}

        //! Accesseur du widget de noeud.
        NodeWidget * nodeWidget() const
            {return m_nodeWidget;}

        //! Mutateur du widget de noeud.
        void setNodeWidget(NodeWidget * widget);

    public slots:
        //! Mutateur de l'état d'expansion des fils.
        void setExpandEtat(bool bb);
    };
protected:
    delegateMPS::AbstractNodeDelegate * m_delegate;
    modelMPS::AbstractNodeModel * m_model;
public:
    //! Constructeur.
    NodeView(QWidget * parent = nullptr);

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
};
}
#endif // NODEVIEW_H
