#include "SelectInListBox.h"

SelectInListBox::SelectInListBox(const QString & titreGauche,
                                 const QString & titreDroite,
                                 bool ordre, bool repetition,
                                 const QMap<int,QString> & choicesGauche,
                                 const QMap<int, QString> &choicesDroite,
                                 QWidget *parent)
    : QWidget(parent),
      m_ordre(ordre),
      m_repetition(repetition)
{    
    // Liste Droite
    m_labelTitreDroite = new QLabel(titreDroite);
    m_listeDroite = new QListWidget();
    if(!m_ordre)
        m_listeDroite->setSelectionMode(QAbstractItemView::ExtendedSelection);

    for(QMap<int,QString>::const_iterator i = choicesDroite.cbegin(); i != choicesDroite.cend(); ++i)
        {append(i.key(),i.value(),true);}

    m_layoutListeDroite = new QVBoxLayout();
    m_layoutListeDroite->addWidget(m_labelTitreDroite);
    m_layoutListeDroite->addWidget(m_listeDroite);

    // Liste Gauche
    m_labelTitreGauche = new QLabel(titreGauche);
    m_listeGauche = new QListWidget();
    if(!m_ordre)
        m_listeGauche->setSelectionMode(QAbstractItemView::ExtendedSelection);

    for(QMap<int,QString>::const_iterator i = choicesGauche.cbegin(); i != choicesGauche.cend(); ++i)
        {append(i.key(),i.value(),false);}

    m_layoutListeGauche = new QVBoxLayout();
    m_layoutListeGauche->addWidget(m_labelTitreGauche);
    m_layoutListeGauche->addWidget(m_listeGauche);

    // Centre
    m_buttonDeplaceDroite = new QPushButton(">>>");
    connect(m_buttonDeplaceDroite,&QPushButton::clicked,this,&SelectInListBox::moveSelectedADroite);

    m_buttonDeplaceGauche = new QPushButton("<<<");
    connect(m_buttonDeplaceGauche,&QPushButton::clicked,this,&SelectInListBox::moveSelectedAGauche);

    m_layoutCentre = new QVBoxLayout();
    m_layoutCentre->addWidget(m_buttonDeplaceDroite);
    m_layoutCentre->addWidget(m_buttonDeplaceGauche);

    // Ordre
    if(ordre)
    {
        // Ordre Droite

        m_boutonPremierDroite = new QPushButton(tr("Premier"));
        connect(m_boutonPremierDroite, &QPushButton::clicked, [=](){this->premier(m_listeDroite);});

        m_boutonPrecedentDroite = new QPushButton(tr("Avant"));
        connect(m_boutonPrecedentDroite, &QPushButton::clicked, [=](){this->precedent(m_listeDroite);});

        m_boutonSuivantDroite = new QPushButton(tr("Après"));
        connect(m_boutonSuivantDroite, &QPushButton::clicked, [=](){this->suivant(m_listeDroite);});

        m_boutonDernierDroite = new QPushButton(tr("Dernier"));
        connect(m_boutonDernierDroite, &QPushButton::clicked, [=](){this->dernier(m_listeDroite);});

        m_layoutOrdreDroite = new QVBoxLayout();
        m_layoutOrdreDroite->addWidget(m_boutonPremierDroite);
        m_layoutOrdreDroite->addWidget(m_boutonPrecedentDroite);
        m_layoutOrdreDroite->addWidget(m_boutonSuivantDroite);
        m_layoutOrdreDroite->addWidget(m_boutonDernierDroite);

        // Ordre Gauche
        m_boutonPremierGauche = new QPushButton(tr("Premier"));
        connect(m_boutonPremierGauche, &QPushButton::clicked, [=](){this->premier(m_listeGauche);});

        m_boutonPrecedentGauche = new QPushButton(tr("Avant"));
        connect(m_boutonPrecedentGauche, &QPushButton::clicked, [=](){this->precedent(m_listeGauche);});


        m_boutonSuivantGauche = new QPushButton(tr("Après"));
        connect(m_boutonSuivantGauche, &QPushButton::clicked, [=](){this->suivant(m_listeGauche);});

        m_boutonDernierGauche = new QPushButton(tr("Dernier"));
        connect(m_boutonDernierGauche, &QPushButton::clicked, [=](){this->dernier(m_listeGauche);});

        m_layoutOrdreGauche = new QVBoxLayout();
        m_layoutOrdreGauche->addWidget(m_boutonPremierGauche);
        m_layoutOrdreGauche->addWidget(m_boutonPrecedentGauche);
        m_layoutOrdreGauche->addWidget(m_boutonSuivantGauche);
        m_layoutOrdreGauche->addWidget(m_boutonDernierGauche);
    }

    // Main Layout
    m_layoutMain = new QHBoxLayout(this);
    if(m_ordre)
        m_layoutMain->addLayout(m_layoutOrdreGauche);
    m_layoutMain->addLayout(m_layoutListeGauche);
    m_layoutMain->addLayout(m_layoutCentre);
    m_layoutMain->addLayout(m_layoutListeDroite);
    if(m_ordre)
        m_layoutMain->addLayout(m_layoutOrdreDroite);
}

void SelectInListBox::append(int n, const QString & txt, bool droite)
{
    if(m_repetition || (m_listeDroite->findItems(txt, Qt::MatchFixedString).isEmpty()
                        && m_listeGauche ->findItems(txt, Qt::MatchFixedString).isEmpty()))
    {
        QListWidgetItem *item = new QListWidgetItem(txt);
        item->setData(Qt::UserRole,n);

        if(droite)
            {append(m_listeDroite,item,true);}
        else
            {append(m_listeGauche,item,true);}
    }
}

void SelectInListBox::append(QListWidget * list, QListWidgetItem * item, bool end)
{
    if(m_ordre)
    {
        if(end)
            list->addItem(item);
        else
            list->insertItem(list->currentRow()+1,item);
    }
    else
    {
        int i = 0;
        while(i < list->count() && list->item(i)->data(Qt::UserRole) < item->data(Qt::UserRole))
            i += 1;
        list->insertItem(i,item);
    }
}

void SelectInListBox::dernier(QListWidget * list)
{
    QListWidgetItem * item = list->takeItem(list->currentRow());
    list->insertItem(list->count(),item);
    list->setCurrentRow(list->count()-1);
}

void SelectInListBox::moveSelectedADroite()
{
    int j = 0;
    QList<QListWidgetItem *> list = m_listeGauche->selectedItems();
    if(list.count() == 1)
        j = m_listeGauche->currentRow();
    for(QList<QListWidgetItem *>::const_iterator i = list.cbegin(); i != list.cend(); ++i)
         append(m_listeDroite,m_listeGauche->takeItem(m_listeGauche->row(*i)));
    if(list.count() == 1)
        m_listeGauche->setCurrentRow(j);
}

void SelectInListBox::moveSelectedAGauche()
{
    int j = 0;
    QList<QListWidgetItem *> list = m_listeDroite->selectedItems();
    if(list.count() == 1)
        j = m_listeDroite->currentRow();
    for(QList<QListWidgetItem *>::const_iterator i = list.cbegin(); i != list.cend(); ++i)
        append(m_listeGauche,m_listeDroite->takeItem(m_listeDroite->row(*i)));
    if(list.count() == 1)
        m_listeDroite->setCurrentRow(j);

}

void SelectInListBox::precedent(QListWidget * list)
{
    int i = list->currentRow();
    if(i > 0)
    {
        QListWidgetItem * item = list->takeItem(i);
        list->insertItem(i-1,item);
        list->setCurrentRow(i-1);
    }

}

void SelectInListBox::premier(QListWidget * list)
{
    QListWidgetItem * item = list->takeItem(list->currentRow());
    list->insertItem(0,item);
    list->setCurrentRow(0);
}

void SelectInListBox::suivant(QListWidget * list)
{
    int i = list->currentRow();
    if(i < list->count()-1)
    {
        QListWidgetItem * item = list->takeItem(i);
        list->insertItem(i+1,item);
        list->setCurrentRow(i+1);
    }
}

void SelectInListBox::remove(int n, const QString & txt)
{
    QList<QListWidgetItem *> list = m_listeDroite->findItems(txt, Qt::MatchExactly);
    for(QList<QListWidgetItem *>::const_iterator i = list.cbegin(); i != list.cend(); ++i)
    {

        if((**i).data(Qt::UserRole) == n)
        {delete m_listeDroite->takeItem(m_listeDroite->row(*i));}
    }


    list = m_listeGauche->findItems(txt, Qt::MatchExactly);
    for(QList<QListWidgetItem *>::const_iterator i = list.cbegin(); i != list.cend(); ++i)
    {
        if((**i).data(Qt::UserRole) == n)
        {delete m_listeGauche->takeItem(m_listeGauche->row(*i));}
    }
}

QPair<QVector<int>,QVector<int>> SelectInListBox::value() const
{
    QVector<int> listeDroite;
    for(int i = 0; i != m_listeDroite->count(); ++i)
        listeDroite.append(m_listeDroite->item(i)->data(Qt::UserRole).toInt());

    QVector<int> listeGauche;
    for(int i = 0; i != m_listeGauche->count(); ++i)
        listeGauche.append(m_listeGauche->item(i)->data(Qt::UserRole).toInt());

    return QPair<QVector<int>,QVector<int>>(listeGauche,listeDroite);
}
