/*Auteur: PERCIE DU SERT Maxime
 *Date: 18/09/2018
 */

/*! \ingroup groupeFen
 *
 * \brief Classe parente des fenêtres autre que la fenêtre principale.
 */

#ifndef FEN_H
#define FEN_H

#include <QWidget>

class FenPrincipale;

class Fen : public QWidget
{
    Q_OBJECT
public:
    //! Constructeur.
    explicit Fen(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // FEN_H
