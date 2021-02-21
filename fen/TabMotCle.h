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
#include "mot_cleDelegate.h"
#include "TreeModelmot_cle.h"
#include "mot_cleWidget.h"

namespace fenMPS {
using namespace type_mps;
/*! \ingroup groupeFen
 *\brief Classe de l'onglet de gestion des mots clés.
 */
class Tabmot_cle : public AbstractTabModuleWithBdd {
    Q_OBJECT
protected:

    QTreeView *m_treeWidget;       //!< Affichage de l'arbre des mots clés.
    modelMPS::TreeModelmot_cle *m_treeModel;  //!< Model de l'arbre des mots clés.
    widgetMPS::mot_cleWidget *m_motCleWidget;      //!< Widget d'attribution des mot clés.
    QSplitter *m_horizontalSplitter;     //!< Splitter horizontale.
    QHBoxLayout *m_mainLayout;         //!< Calque principal.

public:
    //! Constructeur.
    explicit Tabmot_cle(bmps::BddPredef &bdd, const std::pair<int, int> &pairIndex, QWidget *parent = nullptr);

    //! Destructeur.
    ~Tabmot_cle() override = default;

    //! Acceseur de la base de données.
    bmps::BddPredef &bdd() const
        {return static_cast<bmps::BddPredef &>(AbstractTabModuleWithBdd::bdd());}

    //! Titre.
    QString title() const override
        {return QString(tr("Mot-Clés"));}

public slots:
    //! Action à effectuer lorsque l'onglet devient actif.
    void become_current() override;

    //! Transmet la liste des identifiant des mots clés séléctionner dans la séléction à m_motCleWidget.
    void selectionMotcle(const QItemSelection &selected, const QItemSelection &deselected);
};
}
#endif // TABMOTCLE_H
