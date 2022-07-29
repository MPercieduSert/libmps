#include "select_in_list_box.h"

using namespace mps;
using namespace widget;

select_in_list_box::select_in_list_box(const QString &titre_gauche,
                                 const QString &titre_droite,
                                 bool ordre, bool repetition,
                                 const std::map<uint, QString> &choices_gauche,
                                 const std::map<uint, QString> &choices_droite,
                                 QWidget *parent)
    : QWidget(parent),
      m_ordre(ordre),
      m_repetition(repetition) {
    // Liste Droite
    m_titre_droite_label = new QLabel(titre_droite);
    m_droite_list = new QListWidget();
    if(!m_ordre)
        m_droite_list->setSelectionMode(QAbstractItemView::ExtendedSelection);

    m_droite_list_layout = new QVBoxLayout();
    m_droite_list_layout->addWidget(m_titre_droite_label);
    m_droite_list_layout->addWidget(m_droite_list);

    // Liste Gauche
    m_titre_gauche_label = new QLabel(titre_gauche);
    m_gauche_list = new QListWidget();
    if(!m_ordre)
        m_gauche_list->setSelectionMode(QAbstractItemView::ExtendedSelection);

    set_value(choices_gauche,choices_droite);

    m_gauche_list_layout = new QVBoxLayout();
    m_gauche_list_layout->addWidget(m_titre_gauche_label);
    m_gauche_list_layout->addWidget(m_gauche_list);

    // Centre
    m_deplace_droite_bouton = new QPushButton(">>>");
    connect(m_deplace_droite_bouton,&QPushButton::clicked,this,&select_in_list_box::move_selected_droite);

    m_deplace_gauche_bouton = new QPushButton("<<<");
    connect(m_deplace_gauche_bouton,&QPushButton::clicked,this,&select_in_list_box::move_selected_gauche);

    m_centre_layout = new QVBoxLayout();
    m_centre_layout->addWidget(m_deplace_droite_bouton);
    m_centre_layout->addWidget(m_deplace_gauche_bouton);

    // Ordre
    if(ordre) {
        // Ordre Droite

        m_premier_droite_bouton = new QPushButton(tr("Premier"));
        connect(m_premier_droite_bouton, &QPushButton::clicked, this, [this](){this->premier(m_droite_list);});

        m_precedent_droite_bouton = new QPushButton(tr("Avant"));
        connect(m_precedent_droite_bouton, &QPushButton::clicked, this, [this](){this->precedent(m_droite_list);});

        m_suivant_droite_bouton = new QPushButton(tr("Après"));
        connect(m_suivant_droite_bouton, &QPushButton::clicked, this, [this](){this->suivant(m_droite_list);});

        m_dernier_droite_bouton = new QPushButton(tr("Dernier"));
        connect(m_dernier_droite_bouton, &QPushButton::clicked, this, [this](){this->dernier(m_droite_list);});

        m_ordre_droite_layout = new QVBoxLayout();
        m_ordre_droite_layout->addWidget(m_premier_droite_bouton);
        m_ordre_droite_layout->addWidget(m_precedent_droite_bouton);
        m_ordre_droite_layout->addWidget(m_suivant_droite_bouton);
        m_ordre_droite_layout->addWidget(m_dernier_droite_bouton);

        // Ordre Gauche
        m_premier_gauche_bouton = new QPushButton(tr("Premier"));
        connect(m_premier_gauche_bouton, &QPushButton::clicked, this, [this](){this->premier(m_gauche_list);});

        m_precedent_gauche_bouton = new QPushButton(tr("Avant"));
        connect(m_precedent_gauche_bouton, &QPushButton::clicked, this, [this](){this->precedent(m_gauche_list);});


        m_suivant_gauche_bouton = new QPushButton(tr("Après"));
        connect(m_suivant_gauche_bouton, &QPushButton::clicked, this, [this](){this->suivant(m_gauche_list);});

        m_dernier_gauche_bouton = new QPushButton(tr("Dernier"));
        connect(m_dernier_gauche_bouton, &QPushButton::clicked, this, [this](){this->dernier(m_gauche_list);});

        m_ordre_gauche_layout = new QVBoxLayout();
        m_ordre_gauche_layout->addWidget(m_premier_gauche_bouton);
        m_ordre_gauche_layout->addWidget(m_precedent_gauche_bouton);
        m_ordre_gauche_layout->addWidget(m_suivant_gauche_bouton);
        m_ordre_gauche_layout->addWidget(m_dernier_gauche_bouton);
    }

    // Main Layout
    m_main_layout = new QHBoxLayout(this);
    if(m_ordre)
        m_main_layout->addLayout(m_ordre_gauche_layout);
    m_main_layout->addLayout(m_gauche_list_layout);
    m_main_layout->addLayout(m_centre_layout);
    m_main_layout->addLayout(m_droite_list_layout);
    if(m_ordre)
        m_main_layout->addLayout(m_ordre_droite_layout);
}

void select_in_list_box::append(QVariant n, const QString &txt, bool droite) {
    if(m_repetition || (m_droite_list->findItems(txt, Qt::MatchFixedString).isEmpty()
                        &&m_gauche_list ->findItems(txt, Qt::MatchFixedString).isEmpty())) {
        auto *item = new QListWidgetItem(txt);
        item->setData(Qt::UserRole,n);

        if(droite)
            append(m_droite_list,item,true);
        else
            append(m_gauche_list,item,true);
    }
}

void select_in_list_box::clear(){
    m_droite_list->clear();
    m_gauche_list->clear();
}

void select_in_list_box::append(QListWidget *list, QListWidgetItem *item, bool end) {
    if(m_ordre) {
        if(end)
            list->addItem(item);
        else
            list->insertItem(list->currentRow()+1,item);
    }
    else {
        auto i = 0;
        while(i < list->count() &&
              (QVariant::compare(list->item(i)->data(Qt::UserRole), item->data(Qt::UserRole))
                                 == QPartialOrdering::Less))
            i += 1;
        list->insertItem(i,item);
    }
}

void select_in_list_box::dernier(QListWidget *list) {
    QListWidgetItem *item = list->takeItem(list->currentRow());
    list->insertItem(list->count(),item);
    list->setCurrentRow(list->count()-1);
}

void select_in_list_box::move_selected(QListWidget *from, QListWidget *to) {
    auto j = 0;
    auto list = from->selectedItems();
    if(list.count() == 1)
        j = from->currentRow();
    for(auto it = list.cbegin(); it != list.cend(); ++it)
        append(to,from->takeItem(from->row(*it)));
    if(list.count() == 1)
        from->setCurrentRow(j);
}

void select_in_list_box::precedent(QListWidget *list) {
    auto i = list->currentRow();
    if(i > 0) {
        auto *item = list->takeItem(i);
        list->insertItem(i-1,item);
        list->setCurrentRow(i-1);
    }
}

void select_in_list_box::premier(QListWidget *list) {
    auto*item = list->takeItem(list->currentRow());
    list->insertItem(0,item);
    list->setCurrentRow(0);
}

void select_in_list_box::suivant(QListWidget *list) {
    auto i = list->currentRow();
    if(i < list->count()-1) {
        auto *item = list->takeItem(i);
        list->insertItem(i+1,item);
        list->setCurrentRow(i+1);
    }
}

void select_in_list_box::remove(QList<QListWidgetItem*> && list, QVariant n) {
    for(auto it = list.cbegin(); it != list.cend(); ++it) {
        if((*it)->data(Qt::UserRole) == n)
            delete m_droite_list->takeItem(m_droite_list->row(*it));
    }
}

void select_in_list_box::set_value(const std::map<uint,QString> &choices_gauche, const std::map<uint,QString> &choices_droite){
    clear();
    for(auto i = choices_droite.cbegin(); i != choices_droite.cend(); ++i)
        append(i->first, i->second, true);
    for(auto i = choices_gauche.cbegin(); i != choices_gauche.cend(); ++i)
        append(i->first, i->second, false);
}

std::pair<std::vector<QVariant>, std::vector<QVariant>> select_in_list_box::value() const {
    std::pair<std::vector<QVariant>,std::vector<QVariant>> pair{m_gauche_list->count(),m_droite_list->count()};
    for(int i = 0; i != m_gauche_list->count(); ++i)
        pair.first[static_cast<unsigned>(i)] = m_gauche_list->item(i)->data(Qt::UserRole);
    for(int i = 0; i != m_droite_list->count(); ++i)
        pair.second[static_cast<unsigned>(i)] = m_droite_list->item(i)->data(Qt::UserRole);
    return pair;
}
