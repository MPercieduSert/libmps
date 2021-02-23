/*Auteur: PERCIE DU SERT Maxime
 *Date: 18/09/2018
 */
#ifndef FEN_H
#define FEN_H

#include <QWidget>

namespace mps {
namespace fenetre {
/*! \ingroup groupe_fen
 * \brief Classe parente des fenêtres autre que la fenêtre principale.
 */
class fen : public QWidget {
    Q_OBJECT
public:
    //! Constructeur.
    explicit fen(QWidget *parent = nullptr)
        : QWidget(parent)
    {}

signals:

public slots:
};
}}
#endif // FEN_H
