/*Auteur: PERCIE DU SERT Maxime
 *Date: 28/11/2018
 */
#ifndef ABSTRACTZONECENTRALE_H
#define ABSTRACTZONECENTRALE_H

#include <QLayout>
#include <QWidget>

/*! \ingroup groupeFen
 * \brief Zone centrale.
 */
class AbstractZoneCentrale : public QWidget
{
    Q_OBJECT
protected:


public:
    //! Constructeur.
    AbstractZoneCentrale(QWidget * parent = nullptr) : QWidget(parent) {}

    //! Initialise le widget.
    //virtual void init(QWidget * parent) = 0;
signals:
    //! Signal d'activation/désactivation de l'action effacer.
    void effacerPermis(bool);

    //! Signal d'activation/désactivation de l'action coller.
    void collerPermis(bool);

    //! Signal d'activation/désactivation de l'action copier.
    void copierPermis(bool);

    //! Signal d'activation/désactivation de l'action couper.
    void couperPermis(bool);

    //! Signal d'activation/désactivation de l'action saugarder.
    void savePermis(bool);

public slots:
    //! Demande à l'onglet actif de coller.
    virtual void coller() = 0;

    //! Demande à l'onglet actif de couper.
    virtual void couper() = 0;

    //! Demande à l'onglet actif de copier.
    virtual void copier() = 0;

    //! Demande à l'onglet actif d'effacer.
    virtual void effacer() = 0;

    //! Demande à l'onglet actif de sauvegarder.
    virtual void save() = 0;
};

#endif // ABSTRACTZONECENTRALE_H
