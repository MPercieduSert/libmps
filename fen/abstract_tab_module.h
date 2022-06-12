/*Auteur: PERCIE DU SERT Maxime
 *Date: 27/09/2018
 */
#ifndef ABSTRACT_TAB_MODULE_H
#define ABSTRACT_TAB_MODULE_H

#include <QVBoxLayout>
#include <QWidget>
#include "bdd.h"
#include "fen_flags.h"

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 *\brief Classe mère des onglets.
 */
class abstract_tab_module : public QWidget {
    Q_OBJECT
public:
    using tab_index = std::pair<int,int>;
protected:
    const tab_index m_index; //!< Référence de l'onglet
public:
    enum {No_Id = -1};
    //! Constructeur.
    explicit abstract_tab_module(const tab_index &index, QWidget *parent = nullptr)
        : QWidget(parent), m_index(index) {}

    //! Destructeur.
    virtual ~abstract_tab_module() override = default;

    //! Accesseur du titre.
    virtual QString title() const = 0;

    //! Accesseur de index.
    const tab_index &index() const noexcept
        {return m_index;}

signals:
    //! Signal d'activation/désactivation des actions.
    void action_permise(flag);

    //! Demande l'ouverture d'un onglet.
    void open_tab_requested(const tab_index &);

    //! Signal de changement de titre.
    void title_changed(const QString &);

public slots:
    //! Action à effectuer lorsque l'onglet devient actif.
    virtual void become_current();

    //! A réimplémenter pour effectuer l'action coller.
    virtual void coller()   {}

    //! A réimplémenter pour effectuer l'action copier.
    virtual void copier()   {}

    //! A réimplémenter pour effectuer l'action copier.
    virtual void couper()   {}

    //! A réimplémenter pour effectuer l'action effacer.
    virtual void effacer()   {}

    //! A réimplémenter pour effectuer l'action sauvegarder.
    virtual void sauver()   {}

    //! A réimplémenter pour effectuer l'action sauvegarder comme.
    virtual void sauver_as()   {}

    //! A réimplémenter pour effectuer l'action supprimer.
    virtual void supprimer()   {}
};

/*! \ingroup groupe_fen
 *\brief Mère des onglet utilisant la base de données.
 */
class abstract_tab_module_with_bdd : public abstract_tab_module {
    Q_OBJECT
protected:
    b2d::bdd &m_bdd;    //!< Référence sur la base de donnée.
public:
    //! Constructeur.
    explicit abstract_tab_module_with_bdd(b2d::bdd &bdd, const tab_index &index, QWidget *parent = nullptr)
        : abstract_tab_module(index,parent), m_bdd(bdd) {}

    //! Destructeur.
    ~abstract_tab_module_with_bdd() override = default;

    //! Accesseur de la base de donnée.
    virtual b2d::bdd &bdd() const
        {return m_bdd;}
};
}}
#endif // ABSTRACT_TAB_MODULE_H
