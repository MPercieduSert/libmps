/*Auteur: PERCIE DU SERT Maxime
 *Date: 03/04/2019
 */
#ifndef MABSTRACTTREEMODEL_H
#define MABSTRACTTREEMODEL_H

#include <QObject>
#include <QAbstractItemModel>

/*! \ingroup groupeModel
 * \brief Classe mère des models de type arbre.
 */
class MAbstractTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    //! Constructeur.
    explicit MAbstractTreeModel(QObject *parent = nullptr) : QAbstractItemModel(parent) {}

    //! Destructeur par defaut.
    virtual ~MAbstractTreeModel() override = default;

    /*
    //! Effectue l'action coller.
    virtual void coller(const QModelIndexList &selection);

    //! Effectue l'action coller.
    virtual void copier(const QModelIndexList &selection);

    //! Effectue l'action coller.
    virtual void couper(const QModelIndexList &selection);

    //! Effectue l'action coller.
    virtual void effacer(const QModelIndexList &selection);

    //! Enregistre les données du model dans la base de donnée.
    virtual void save() = 0;*/
};

#endif // MABSTRACTTREEMODEL_H
