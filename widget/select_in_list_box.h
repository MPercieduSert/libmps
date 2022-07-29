/*Auteur: PERCIE DU SERT Maxime
 *Date: 11/09/2018
 */
#ifndef SELECT_IN_LIST_BOX_H
#define SELECT_IN_LIST_BOX_H

#include <QPushButton>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <map>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>

namespace mps {
namespace widget {
/*! \ingroup groupe_Widget
 *\brief Widget permettant de répartir des éléments entre deux listes, avec une possible mise en ordre.
 */
class select_in_list_box : public QWidget {
    Q_OBJECT
protected:
    bool m_ordre;                       //! Permet ou non d'ordonner la liste.
    bool m_repetition;                  //! Permet ou non les répétitions.

    // Widget
    QPushButton *m_deplace_droite_bouton;
    QPushButton *m_deplace_gauche_bouton;
    QPushButton *m_dernier_droite_bouton;
    QPushButton *m_dernier_gauche_bouton;
    QPushButton *m_precedent_droite_bouton;
    QPushButton *m_precedent_gauche_bouton;
    QPushButton *m_premier_droite_bouton;
    QPushButton *m_premier_gauche_bouton;
    QPushButton *m_suivant_droite_bouton;
    QPushButton *m_suivant_gauche_bouton;
    QLabel *m_titre_droite_label;
    QLabel *m_titre_gauche_label;
    QListWidget *m_droite_list;
    QListWidget *m_gauche_list;

    // Layout
    QVBoxLayout *m_centre_layout;
    QVBoxLayout *m_droite_list_layout;
    QVBoxLayout *m_gauche_list_layout;
    QHBoxLayout *m_main_layout;
    QVBoxLayout *m_ordre_droite_layout;
    QVBoxLayout *m_ordre_gauche_layout;

public:
    //!Constructeur
    explicit select_in_list_box(const QString &titre_gauche = QString(), const QString &titre_droite = QString(),
                                bool ordre = false, bool repetition = false,
                                const std::map<uint,QString> &choices_gauche = std::map<uint,QString>(),
                                const std::map<uint,QString> &choices_droite = std::map<uint,QString>(),
                                QWidget *parent = nullptr);

    //! Destructeur par default.
    ~ select_in_list_box() override = default;

    //! Ajoute un élément dans la liste de droite
    void append(QVariant n, const QString &txt, bool droite = true);

    //! Efface les listes de droite et gauche.
    void clear();

    //! Supprime les éléments des liste de valeur n et texte txt
    void remove(QVariant n, const QString &txt) {
        remove(m_droite_list->findItems(txt, Qt::MatchExactly),n);
        remove(m_gauche_list->findItems(txt, Qt::MatchExactly),n);
    }

    //! Mutateur des deux listes.
    void set_value(const std::map<uint,QString> &choices_gauche, const std::map<uint,QString> &choices_droite);

    //! Retourne une paire contenant deux listes d'entiters, la liste des données (Qt::UserData) de la liste gauche
    //! et celles de la liste de droite.
    std::pair<std::vector<QVariant>,std::vector<QVariant>> value() const;
signals:

public slots:
    //! Déplace les éléments selectionner dans liste de gauche vers la liste de droite
    void move_selected_droite()
        {move_selected(m_gauche_list,m_droite_list);}

    //! Déplace les éléments selectionner dans liste de droite vers la liste de gauche
    void move_selected_gauche()
        {move_selected(m_droite_list,m_gauche_list);}

protected:
    /*! \brief Ajoute item a liste.
     *
     * Ajoute item a liste, si
     * - m_ordre = false, l'élément est placer suivant l'ordre croissant dans liste.
     * - m_ordre = true et end = false, l'élément est placé juste après l'élement selectionné
     *  dans la liste s'il y en a un, à la fin sinon.
     * - m_ordre = true et end = true, l'élément et placé à la fin de la liste.
     */
    void append(QListWidget *list, QListWidgetItem *item, bool end = false);

    //! Place l'élément selectionner à la fin de list.
    void dernier(QListWidget *list);

    //! Déplace les éléments selectionner d'une liste à l'autre.
    void move_selected(QListWidget* from, QListWidget* to);

    //! Place l'élément selectionner à la ligne précédente de list.
    void precedent(QListWidget *list);

    //! Place l'élément selectionner au début de list.
    void premier(QListWidget *list);

    //! Supprime les éléments d'une liste de valeur n et texte txt
    void remove(QList<QListWidgetItem*> &&list, QVariant n);

    //! Place l'élément selectionner à la ligne suivante de list.
    void suivant(QListWidget *list);
};
}}
#endif // SELECT_IN_LIST_BOX_H
