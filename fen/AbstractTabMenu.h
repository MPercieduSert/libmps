/*Auteur: PERCIE DU SERT Maxime
 *Date: 04/04/2019
 */
#ifndef ABSTRACTTABMENU_H
#define ABSTRACTTABMENU_H

#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QWidget>
#include <stdexcept>
#include "AbstractTabModule.h"

namespace fenMPS {
/*! \ingroup groupeFen
 * \brief Fenêtre menu d'une annee
 */
class AbstractTabMenu : public AbstractTabModule {
    Q_OBJECT
protected:
    // Widget 
    QPushButton * m_bddButton;                //!< Bouton ouvrant l'onglet de gestion de la base de donnée.
    QPushButton * m_motCleButton;               //!< Bouton ouvrant l'onglet de gestion des mots clés.
    QSignalMapper * m_ButtonMapper;                  //!< Lie les boutons à leur actions.

    // Layout
    QHBoxLayout *m_mainLayout;                    //!< Calque principal.

public:
    //! Ordre de position dans le layout principal
    enum tabIndexType{  SingleTab = -1,
                        BddTabIndex,
                        MotCleTabIndex,
                        NbrTabIndex};

    //! Constructeur
    explicit AbstractTabMenu(const std::pair<int, int> &pairIndex, QWidget * parent = nullptr);

    //! Destructeur.
    ~AbstractTabMenu() override = default;
};
}
#endif // ABSTRACTTABMENU_H
