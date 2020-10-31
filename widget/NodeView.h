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

namespace widgetMPS {
/*! \ingroup groupeWidget
 * \brief Vue en arbre d'un AbstractNodeModel.
 */
class NodeView : public QScrollArea {
    Q_OBJECT
public:
    /*! \ingroup groupeWidget
     * \brief Widget associé à un noeud.
     */
    class NodeWidget : public QWidget {
        Q_OBJECT
    protected:
        bool m_expandEtat;              //!< Etat d'expansion des fils.
        QPushButton * m_expandButton;   //!< Bouton d'expansion des fils du noeud.
        QVBoxLayout * m_expandLayout;   //!< Calque d'expansion des fils du noeud.
        QHBoxLayout * m_mainLayout;     //!< Calque Principale.
        QVBoxLayout * m_secondLayout;   //!< Calque secondaire.
    public:
        //! Constructeur.
        NodeWidget(QWidget * parent = nullptr);

        //! Acceseur de l'état d'expansion des fils.
        bool expandEtat() const
            {return m_expandEtat;}
    public slots:
        //! Mutateur de l'état d'expansion des fils.
        void setExpandEtat(bool bb);
    };
protected:
    modelMPS::AbstractNodeModel * m_model;
public:
    //! Constructeur.
    NodeView(QWidget * parent = nullptr);

    //! Accesseur du model.
    modelMPS::AbstractNodeModel * model() const
        {return m_model;}

    //! Mutateur du model.
    void setModel(modelMPS::AbstractNodeModel *model);
};
}
#endif // NODEVIEW_H
