/*Auteur: PERCIE DU SERT Maxime
 *Date: 17/08/2017
 */
#ifndef TABMOTCLE_H
#define TABMOTCLE_H

#include <QHBoxLayout>
#include <QSet>
#include <QSplitter>
#include <QTreeView>

#include "AbstractTabModuleWithBdd.h"
#include "MotCleDelegate.h"
#include "TreeModelMotCle.h"
#include "MotCleWidget.h"

namespace fenMPS {
using namespace typeMPS;
/*! \ingroup groupeFen
 * \brief Classe de l'onglet de gestion des mots clés.
 */
class TabMotCle : public AbstractTabModuleWithBdd {
    Q_OBJECT
protected:

    QTreeView * m_treeWidget;       //!< Affichage de l'arbre des mots clés.
    modelMPS::TreeModelMotCle * m_treeModel;  //!< Model de l'arbre des mots clés.
    widgetMPS::MotCleWidget * m_motCleWidget;      //!< Widget d'attribution des mot clés.
    QSplitter * m_horizontalSplitter;     //!< Splitter horizontale.
    QHBoxLayout * m_mainLayout;         //!< Calque principal.

public:
    //! Constructeur.
    explicit TabMotCle(bmps::BddPredef & bdd, const std::pair<int, int> &pairIndex, QWidget *parent = nullptr);

    //! Destructeur.
    ~TabMotCle() override = default;

    //! Acceseur de la base de données.
    bmps::BddPredef & bdd() const
        {return static_cast<bmps::BddPredef &>(AbstractTabModuleWithBdd::bdd());}

    //! Titre.
    QString title() const override
        {return QString(tr("Mot-Clés"));}

public slots:
    //! Action à effectuer lorsque l'onglet devient actif.
    void becomeCurrent() override;

    //! Transmet la liste des identifiant des mots clés séléctionner dans la séléction à m_motCleWidget.
    void selectionMotcle(const QItemSelection &selected, const QItemSelection &deselected);
};
}
#endif // TABMOTCLE_H
