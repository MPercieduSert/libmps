#include "NewModifPredef.h"

using namespace dialogMPS;
//////////////////////////////////////////////// EvenementNewModifForm ////////////////////////////////////////
EvenementNewModifForm::EvenementNewModifForm(b2d::Bdd &bdd, bool newEnt, QWidget *parent)
    : AbstractTypenc_nomNewModifForm(static_cast<b2d::bdd_predef &>(bdd),"evenement_root_tp",Evenement::ID,
                                    "Type de l'événement :","Nom abrégé de l'événement :","Nom de l'événement :",
                                    newEnt,parent){
    // Nom
    if(!m_new)
        set_noms(bdd.get_list<Evenement>(Evenement::Nom));
    // Debut
    m_debutGroupe = new QGroupBox(tr("Début :"));
    m_debutDate = new QCalendarWidget;
    m_debutHeure = new QTimeEdit;

    // Fin
    m_finGroupe = new QGroupBox(tr("Fin :"));
    m_finDate = new QCalendarWidget;
    m_finHeure = new QTimeEdit;

    // Options
    m_optionGroupe = new QGroupBox(tr("Options :"));
    m_unJourOption = new QCheckBox(tr("Un seul jour"));
    m_jourEntierOption = new QCheckBox(tr("Jour entier"));

    // Style
    m_styleLabel = new QLabel(tr("Style :"));
    m_styleICB = new widgetMPS::IdComboBox;
    m_styleICB->addText(bdd.get_list<evenement_style>(evenement_style::Nom),
                        [](const evenement_style &style){return style.nom();});

    // Titre
    m_titreLabel = new QLabel(tr("Titre :"));
    m_titreEdit = new QLineEdit;

    // Calque
    m_debutLayout = new QVBoxLayout;
    m_debutGroupe->setLayout(m_debutLayout);
    m_debutLayout->addWidget(m_debutDate);
    m_debutLayout->addWidget(m_debutHeure);

    m_finLayout = new QVBoxLayout;
    m_finGroupe->setLayout(m_finLayout);
    m_finLayout->addWidget(m_finDate);
    m_finLayout->addWidget(m_finHeure);

    m_debutFinLayout = new QHBoxLayout;
    m_debutFinLayout->addWidget(m_debutGroupe);
    m_debutFinLayout->addWidget(m_finGroupe);

    m_optionLayout = new QGridLayout;
    m_optionGroupe->setLayout(m_optionLayout);
    m_optionLayout->addWidget(m_unJourOption,0,0);
    m_optionLayout->addWidget(m_jourEntierOption,0,1);

    m_mainLayout->addWidget(m_titreLabel);
    m_mainLayout->addWidget(m_titreEdit);
    m_mainLayout->addLayout(m_debutFinLayout);
    m_mainLayout->addWidget(m_optionGroupe);
    m_mainLayout->addWidget(m_styleLabel);
    m_mainLayout->addWidget(m_styleICB);
}

void EvenementNewModifForm::connexion() {
    AbstractTypenc_nomNewModifForm::connexion();
    connect(m_unJourOption,&QCheckBox::stateChanged,this,[this](){
        if(m_unJourOption->checkState() == Qt::Checked) {
            m_finDate->setSelectedDate(m_debutDate->selectedDate());
            m_finDate->setDisabled(true);
        }
        else
            m_finDate->setEnabled(true);
    });
    connect(m_jourEntierOption,&QCheckBox::stateChanged,this,[this](){
        if(m_jourEntierOption->isChecked()) {
            m_debutHeure->setTime(QTime(0,0));
            m_debutHeure->setDisabled(true);
            m_finHeure->setTime(QTime(0,0));
            m_finHeure->setDisabled(true);
        }
        else {
            m_debutHeure->setEnabled(true);
            m_finHeure->setEnabled(true);
        }
    });
    connect(m_debutDate,&QCalendarWidget::selectionChanged,this,[this](){
        if(m_unJourOption->isChecked())
            m_finDate->setSelectedDate(m_debutDate->selectedDate());
    });
    connect(m_titreEdit,&QLineEdit::textChanged,this,[this](){emit savePermis(valide());});
    connect(m_debutDate,&QCalendarWidget::selectionChanged,this,[this](){emit savePermis(valide());});
    connect(m_debutHeure,&QTimeEdit::timeChanged,this,[this](){emit savePermis(valide());});
    connect(m_finDate,&QCalendarWidget::selectionChanged,this,[this](){emit savePermis(valide());});
    connect(m_finHeure,&QTimeEdit::timeChanged,this,[this](){emit savePermis(valide());});
    connect(m_jourEntierOption,&QCheckBox::stateChanged,this,[this](){emit savePermis(valide());});
    connect(m_unJourOption,&QCheckBox::stateChanged,this,[this](){emit savePermis(valide());});
}

void EvenementNewModifForm::save() {
    Evenement eve(id());
    if(!m_new)
        m_bdd.get(eve);
    else
        eve.set_creation(QDateTime::currentDate_Time());
    eve.set_id_style(m_styleICB->id());
    if(m_unJourOption->isChecked())
        eve.add(Evenement::UnJour);
    if(m_jourEntierOption->isChecked())
        eve.add(Evenement::JourEntier);
    QDateTime dt;
    dt.setDate(m_debutDate->selectedDate());
    dt.setTime(m_debutHeure->time());
    eve.set_debut(dt);
    dt.setDate(m_finDate->selectedDate());
    dt.setTime(m_finHeure->time());
    eve.set_fin(dt);
    eve.set_nc(nc());
    eve.set_nom(nom());
    eve.set_type(id_type());
    eve.set_titre(m_titreEdit->text());
    m_bdd.save(eve);
}
void EvenementNewModifForm::updateData() {
    if(!m_new) {
        Evenement eve;
        update_temp<Evenement>(eve);
        m_styleICB->setCurrentIndexId(eve.id_style());
        m_unJourOption->setChecked(eve.code().test(Evenement::UnJour));
        m_jourEntierOption->setChecked(eve.code().test(Evenement::JourEntier));
        m_debutDate->setSelectedDate(eve.debut().date());
        m_debutHeure->setTime(eve.debut().time());
        m_finDate->setSelectedDate(eve.fin().date());
        m_finHeure->setTime(eve.fin().time());
        m_titreEdit->set_text(eve.titre());
    }
}

//////////////////////////////////////////////// evenement_styleNewModifForm ///////////////////////////////////
evenement_styleNewModifForm::evenement_styleNewModifForm(b2d::Bdd &bdd, bool newEnt, QWidget *parent)
    : AbstractNomNewModifForm(bdd,
                              tr("Nom du style d'événements :"),
                              newEnt,parent) {
    if(!newEnt)
        set_noms(bdd.get_list<evenement_style>(evenement_style::Nom));
    // Forme
    m_formGroupe = new QGroupBox(tr("Forme et bordure:"));
    m_formCB = new QComboBox;
    for (auto forme = 0; forme != evenement_style::Nbr_Forme_Style; ++forme)
        m_formCB->addItem(evenement_style::forme_string(forme),forme);
    m_bordureCB = new QComboBox;
    m_bordureCB->addItem(tr("Continue"), static_cast<int>(Qt::SolidLine));
    m_bordureCB->addItem(tr("Tiret"), static_cast<int>(Qt::DashLine));
    m_bordureCB->addItem(tr("Point"), static_cast<int>(Qt::DotLine));
    m_bordureCB->addItem(tr("Tiret point"), static_cast<int>(Qt::DashDotLine));
    m_bordureCB->addItem(tr("Tiret point point"), static_cast<int>(Qt::DashDotDotLine));
    m_bordureLabel = new QLabel(m_fondCouleur.name());
    m_bordureLabel->setAlignment(Qt::AlignCenter);
    m_couleurBordureBouton = new QPushButton(tr("Couleur"));
    connect(m_couleurBordureBouton,&QPushButton::clicked,this,[this](){setColor(Bordure,QColorDialog::getColor(m_bordureCouleur,this));});

    // Fond
    m_fondGroupe = new QGroupBox(tr("Fond"));
    m_fondLabel = new QLabel(m_fondCouleur.name());
    m_fondLabel->setAlignment(Qt::AlignCenter);
    m_couleurFondBouton = new QPushButton(tr("Couleur"));
    m_textureCB = new QComboBox;
    m_textureCB->addItem(tr("Uni"), static_cast<int>(Qt::SolidPattern));
    m_textureCB->addItem(tr("Dense 1"), static_cast<int>(Qt::Dense1Pattern));
    m_textureCB->addItem(tr("Dense 2"), static_cast<int>(Qt::Dense2Pattern));
    m_textureCB->addItem(tr("Dense 3"), static_cast<int>(Qt::Dense3Pattern));
    m_textureCB->addItem(tr("Dense 4"), static_cast<int>(Qt::Dense4Pattern));
    m_textureCB->addItem(tr("Dense 5"), static_cast<int>(Qt::Dense5Pattern));
    m_textureCB->addItem(tr("Dense 6"), static_cast<int>(Qt::Dense6Pattern));
    m_textureCB->addItem(tr("Dense 7"), static_cast<int>(Qt::Dense7Pattern));
    m_textureCB->addItem(tr("Horizontal"), static_cast<int>(Qt::HorPattern));
    m_textureCB->addItem(tr("Vertical"), static_cast<int>(Qt::VerPattern));
    m_textureCB->addItem(tr("Croisé"), static_cast<int>(Qt::CrossPattern));
    m_textureCB->addItem(tr("Diagonal montante"), static_cast<int>(Qt::BDiagPattern));
    m_textureCB->addItem(tr("Diagonal decendante"), static_cast<int>(Qt::FDiagPattern));
    m_textureCB->addItem(tr("Diagonal Croisée"), static_cast<int>(Qt::DiagCrossPattern));
    connect(m_couleurFondBouton,&QPushButton::clicked,this,[this](){setColor(Fond,QColorDialog::getColor(m_fondCouleur,this));});

    // Texte
    m_texteGroupe = new QGroupBox(tr("Texte :"));
    m_policeTexteBouton = new QPushButton(tr("Police"));
    m_couleurTexteBouton = new QPushButton(tr("Couleur"));
    m_texteLabel = new QLabel(tr("Texte"));
    m_texteLabel->setAlignment(Qt::AlignCenter);
    connect(m_couleurTexteBouton,&QPushButton::clicked,this,[this](){setColor(Texte,QColorDialog::getColor(m_texteCouleur,this));});
    connect(m_policeTexteBouton,&QPushButton::clicked,this,[this](){
        m_texteLabel->setFont(QFontDialog::getFont(nullptr,m_texteLabel->font(),this));});

    //Titre
    m_titreGroupe = new QGroupBox(tr("Titre :"));
    m_policeTitreBouton = new QPushButton(tr("Police"));
    m_couleurTitreBouton = new QPushButton(tr("Couleur"));
    m_titreLabel = new QLabel(tr("Titre"));
    m_titreLabel->setAlignment(Qt::AlignCenter);
    connect(m_couleurTitreBouton,&QPushButton::clicked,this,[this](){setColor(Titre,QColorDialog::getColor(m_titreCouleur,this));});
    connect(m_policeTitreBouton,&QPushButton::clicked,this,[this](){
        m_titreLabel->setFont(QFontDialog::getFont(nullptr,m_titreLabel->font(),this));});

    // Calque
    m_formLayout = new QHBoxLayout;
    m_formGroupe->setLayout(m_formLayout);
    m_formLayout->addWidget(m_formCB);
    m_formLayout->addWidget(m_bordureCB);
    m_formLayout->addWidget(m_couleurBordureBouton);
    m_formLayout->addWidget(m_bordureLabel);

    m_fondLayout = new QHBoxLayout;
    m_fondGroupe->setLayout(m_fondLayout);
    m_fondLayout->addWidget(m_couleurFondBouton);
    m_fondLayout->addWidget(m_textureCB);

    m_texteLayout = new QHBoxLayout;
    m_texteGroupe->setLayout(m_texteLayout);
    m_texteLayout->addWidget(m_policeTexteBouton);
    m_texteLayout->addWidget(m_couleurTexteBouton);
    m_texteLayout->addWidget(m_texteLabel);

    m_titreLayout = new QHBoxLayout;
    m_titreGroupe->setLayout(m_titreLayout);
    m_titreLayout->addWidget(m_policeTitreBouton);
    m_titreLayout->addWidget(m_couleurTitreBouton);
    m_titreLayout->addWidget(m_titreLabel);

    m_fondLayout->addWidget(m_fondLabel);
    m_mainLayout->addWidget(m_formGroupe);
    m_mainLayout->addWidget(m_fondGroupe);
    m_mainLayout->addWidget(m_titreGroupe);
    m_mainLayout->addWidget(m_texteGroupe);
}

void evenement_styleNewModifForm::setColor(objet ob, const QColor &color) {
    switch (ob) {
    case Bordure:
        m_bordureCouleur = color;
        m_bordureLabel->setStyleSheet("QLabel {background-color:"+m_bordureCouleur.name()+" }");
        m_bordureLabel->set_text(m_fondCouleur.name());
        break;
    case Fond:
        m_fondCouleur = color;
        m_fondLabel->setStyleSheet("QLabel {background-color:"+m_fondCouleur.name()+" }");
        m_texteLabel->setStyleSheet("QLabel {background-color:"+m_fondCouleur.name()+"; color:"+m_texteCouleur.name()+" }");
        m_titreLabel->setStyleSheet("QLabel {background-color:"+m_fondCouleur.name()+"; color:"+m_titreCouleur.name()+" }");
        m_fondLabel->set_text(m_fondCouleur.name());
        break;
    case Texte:
        m_texteCouleur = color;
        m_texteLabel->setStyleSheet("QLabel {background-color:"+m_fondCouleur.name()+"; color:"+m_texteCouleur.name()+" }");
        break;
    case Titre:
        m_titreCouleur = color;
        m_titreLabel->setStyleSheet("QLabel {background-color:"+m_fondCouleur.name()+"; color:"+m_titreCouleur.name()+" }");
        break;
    }
}

void evenement_styleNewModifForm::save(){
    evenement_style style(id());
    style.set_couleur_bordure(m_bordureCouleur.name());
    style.set_couleur_fond(m_fondCouleur.name());
    style.set_couleur_texte(m_texteCouleur.name());
    style.set_couleur_titre(m_titreCouleur.name());
    style.set_bordure(m_bordureCB->currentData().toInt());
    style.set_forme(m_formCB->currentData().toInt());
    style.set_texture(m_textureCB->currentData().toUInt());
    style.set_police_texte(m_texteLabel->font().to_string());
    style.set_police_titre(m_titreLabel->font().to_string());
    style.set_nom(nom());
    m_bdd.save(style);
}

void evenement_styleNewModifForm::updateData() {
    evenement_style style;
    if(m_new) {
        style.set_id(evenement_style::Id_Style_Default);
        m_bdd.get(style);
    }
    else
        update_temp<evenement_style>(style);
    setColor(Bordure,QColor(style.couleurBordure()));
    setColor(Fond,QColor(style.couleurFond()));
    setColor(Texte,QColor(style.couleurTexte()));
    setColor(Titre,QColor(style.couleurTitre()));
    m_bordureCB->setCurrentIndex(m_bordureCB->findData(style.bordure()));
    m_formCB->setCurrentIndex(m_formCB->findData(style.forme()));
    m_textureCB->setCurrentIndex(m_textureCB->findData(style.texture()));
    QFont font;
    font.fromString(style.policeTexte());
    m_texteLabel->setFont(font);
    font.fromString(style.policeTitre());
    m_titreLabel->setFont(font);
}

////////////////////////////////////////////////// SourceNewModifForm ///////////////////////////////////////////
void SourceNewModifForm::save() {
    Source src;
    if(!m_new)
        src.set_id(id());
    src.set_nc(nc());
    src.set_nom(nom());
    src.set_type(id_type());
    m_bdd.save(src);
}
void SourceNewModifForm::updateData() {
    if(!m_new) {
        Source src;
        update_temp<Source>(src);
    }
}
