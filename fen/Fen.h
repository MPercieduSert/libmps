/*Auteur: PERCIE DU SERT Maxime
 *Date: 18/09/2018
 */

#ifndef FEN_H
#define FEN_H

#include <QWidget>

/*! \defgroup groupeFen Fenêtre
 *\brief Ensemble de classes des fenêtres.
 */

/*! \ingroup groupeFen
 *\brief Espace de noms des fenêtres.
 */

namespace fenMPS {
/*! \ingroup groupeFen
 *
 *\brief Classe parente des fenêtres autre que la fenêtre principale.
 */
class Fen : public QWidget {
    Q_OBJECT
public:
    //! Constructeur.
    explicit Fen(QWidget *parent = nullptr)
        : QWidget(parent)
    {}

signals:

public slots:
};
}
#endif // FEN_H
