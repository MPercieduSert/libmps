/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef MOTCLEWIDGET_H
#define MOTCLEWIDGET_H

#include <QLineEdit>
#include <QTextEdit>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>

#include "EntityPredef.h"
#include "MotCleModel.h"

class MotCleWidget : public QWidget
{
    Q_OBJECT
protected:
    MotCleModel * m_model;    //!< Model.

    // Widget
    QTextEdit * m_afficheListMotCle;    //!< Partie de texte faisant la liste des mots clés associés à une entité.
    QLineEdit * m_chercheMotCle;        //!< Permettant de faire la recherche d'un mot clé.
    QTreeView * m_treeView;             //!< Vue affichant l'arbre des mots clés permis.
    QVBoxLayout * m_mainLayout;         //!< Calque principal de mise en forme.

public:
    //! Constructeur.
    explicit MotCleWidget(BddPredef * bd, int cible, QWidget *parent = nullptr);

    //! Destructeur.
    ~MotCleWidget() override = default;
signals:
    void idChanged(int newId);

public slots:
    void setIdSet(QSet<int> idSet);
    //void setIndexSelected();
};

#endif // MOTCLEWIDGET_H
