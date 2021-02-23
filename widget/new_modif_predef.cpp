#include "new_modif_predef.h"

using namespace mps;
using namespace dialogue;
//////////////////////////////////////////////// evenement_new_modif_form ////////////////////////////////////////
evenement_new_modif_form::evenement_new_modif_form(b2d::bdd &bdd, bool new_ent, QWidget *parent)
    : abstract_type_nc_nom_new_modif_form(static_cast<b2d::bdd_predef &>(bdd),"evenement_root_tp",evenement::ID,
                                    "Type de l'événement :","Nom abrégé de l'événement :","Nom de l'événement :",
                                    new_ent,parent){
    // Nom
    if(!m_new)
        set_noms(bdd.get_list<evenement>(evenement::Nom));
    // Debut
    m_debut_groupe = new QGroupBox(tr("Début :"));
    m_debut_date = new QCalendarWidget;
    m_debut_heure = new QTimeEdit;

    // Fin
    m_fin_groupe = new QGroupBox(tr("Fin :"));
    m_fin_date = new QCalendarWidget;
    m_fin_heure = new QTimeEdit;

    // Options
    m_option_groupe = new QGroupBox(tr("Options :"));
    m_unJour_option = new QCheckBox(tr("Un seul jour"));
    m_jourEntier_option = new QCheckBox(tr("Jour entier"));

    // Style
    m_style_label = new QLabel(tr("Style :"));
    m_style_icb = new widget::id_combo_box;
    m_style_icb->add_text(bdd.get_list<evenement_style>(evenement_style::Nom),
                        [](const evenement_style &style){return style.nom();});

    // Titre
    m_titre_label = new QLabel(tr("Titre :"));
    m_titre_edit = new QLineEdit;

    // Calque
    m_debut_layout = new QVBoxLayout;
    m_debut_groupe->setLayout(m_debut_layout);
    m_debut_layout->addWidget(m_debut_date);
    m_debut_layout->addWidget(m_debut_heure);

    m_fin_layout = new QVBoxLayout;
    m_fin_groupe->setLayout(m_fin_layout);
    m_fin_layout->addWidget(m_fin_date);
    m_fin_layout->addWidget(m_fin_heure);

    m_debut_fin_layout = new QHBoxLayout;
    m_debut_fin_layout->addWidget(m_debut_groupe);
    m_debut_fin_layout->addWidget(m_fin_groupe);

    m_option_layout = new QGridLayout;
    m_option_groupe->setLayout(m_option_layout);
    m_option_layout->addWidget(m_unJour_option,0,0);
    m_option_layout->addWidget(m_jourEntier_option,0,1);

    m_main_layout->addWidget(m_titre_label);
    m_main_layout->addWidget(m_titre_edit);
    m_main_layout->addLayout(m_debut_fin_layout);
    m_main_layout->addWidget(m_option_groupe);
    m_main_layout->addWidget(m_style_label);
    m_main_layout->addWidget(m_style_icb);
}

void evenement_new_modif_form::connexion() {
    abstract_type_nc_nom_new_modif_form::connexion();
    connect(m_unJour_option,&QCheckBox::stateChanged,this,[this](){
        if(m_unJour_option->checkState() == Qt::Checked) {
            m_fin_date->setSelectedDate(m_debut_date->selectedDate());
            m_fin_date->setDisabled(true);
        }
        else
            m_fin_date->setEnabled(true);
    });
    connect(m_jourEntier_option,&QCheckBox::stateChanged,this,[this](){
        if(m_jourEntier_option->isChecked()) {
            m_debut_heure->setTime(QTime(0,0));
            m_debut_heure->setDisabled(true);
            m_fin_heure->setTime(QTime(0,0));
            m_fin_heure->setDisabled(true);
        }
        else {
            m_debut_heure->setEnabled(true);
            m_fin_heure->setEnabled(true);
        }
    });
    connect(m_debut_date,&QCalendarWidget::selectionChanged,this,[this](){
        if(m_unJour_option->isChecked())
            m_fin_date->setSelectedDate(m_debut_date->selectedDate());
    });
    connect(m_titre_edit,&QLineEdit::textChanged,this,[this](){emit save_permis(valide());});
    connect(m_debut_date,&QCalendarWidget::selectionChanged,this,[this](){emit save_permis(valide());});
    connect(m_debut_heure,&QTimeEdit::timeChanged,this,[this](){emit save_permis(valide());});
    connect(m_fin_date,&QCalendarWidget::selectionChanged,this,[this](){emit save_permis(valide());});
    connect(m_fin_heure,&QTimeEdit::timeChanged,this,[this](){emit save_permis(valide());});
    connect(m_jourEntier_option,&QCheckBox::stateChanged,this,[this](){emit save_permis(valide());});
    connect(m_unJour_option,&QCheckBox::stateChanged,this,[this](){emit save_permis(valide());});
}

void evenement_new_modif_form::save() {
    evenement eve(id());
    if(!m_new)
        m_bdd.get(eve);
    else
        eve.set_creation(QDateTime::currentDateTime());
    eve.set_id_style(m_style_icb->id());
    if(m_unJour_option->isChecked())
        eve.add(evenement::UnJour);
    if(m_jourEntier_option->isChecked())
        eve.add(evenement::JourEntier);
    QDateTime dt;
    dt.setDate(m_debut_date->selectedDate());
    dt.setTime(m_debut_heure->time());
    eve.set_debut(dt);
    dt.setDate(m_fin_date->selectedDate());
    dt.setTime(m_fin_heure->time());
    eve.set_fin(dt);
    eve.set_nc(nc());
    eve.set_nom(nom());
    eve.set_type(id_type());
    eve.set_titre(m_titre_edit->text());
    m_bdd.save(eve);
}
void evenement_new_modif_form::update_data() {
    if(!m_new) {
        evenement eve;
        update_temp<evenement>(eve);
        m_style_icb->set_current_index_id(eve.id_style());
        m_unJour_option->setChecked(eve.code().test(evenement::UnJour));
        m_jourEntier_option->setChecked(eve.code().test(evenement::JourEntier));
        m_debut_date->setSelectedDate(eve.debut().date());
        m_debut_heure->setTime(eve.debut().time());
        m_fin_date->setSelectedDate(eve.fin().date());
        m_fin_heure->setTime(eve.fin().time());
        m_titre_edit->setText(eve.titre());
    }
}

//////////////////////////////////////////////// evenement_style_new_modif_form ///////////////////////////////////
evenement_style_new_modif_form::evenement_style_new_modif_form(b2d::bdd &bdd, bool new_ent, QWidget *parent)
    : abstract_nom_new_modif_form(bdd,
                              tr("Nom du style d'événements :"),
                              new_ent,parent) {
    if(!new_ent)
        set_noms(bdd.get_list<evenement_style>(evenement_style::Nom));
    // Forme
    m_form_groupe = new QGroupBox(tr("Forme et bordure:"));
    m_form_cb = new QComboBox;
    for (auto forme = 0; forme != evenement_style::Nbr_Forme_Style; ++forme)
        m_form_cb->addItem(evenement_style::forme_string(forme),forme);
    m_bordure_cb = new QComboBox;
    m_bordure_cb->addItem(tr("Continue"), static_cast<int>(Qt::SolidLine));
    m_bordure_cb->addItem(tr("Tiret"), static_cast<int>(Qt::DashLine));
    m_bordure_cb->addItem(tr("Point"), static_cast<int>(Qt::DotLine));
    m_bordure_cb->addItem(tr("Tiret point"), static_cast<int>(Qt::DashDotLine));
    m_bordure_cb->addItem(tr("Tiret point point"), static_cast<int>(Qt::DashDotDotLine));
    m_bordure_label = new QLabel(m_fond_couleur.name());
    m_bordure_label->setAlignment(Qt::AlignCenter);
    m_couleur_bordure_bouton = new QPushButton(tr("Couleur"));
    connect(m_couleur_bordure_bouton,&QPushButton::clicked,this,[this]()
        {set_color(Bordure,QColorDialog::getColor(m_bordure_couleur,this));});

    // Fond
    m_fond_groupe = new QGroupBox(tr("Fond"));
    m_fond_label = new QLabel(m_fond_couleur.name());
    m_fond_label->setAlignment(Qt::AlignCenter);
    m_couleur_fond_bouton = new QPushButton(tr("Couleur"));
    m_texture_cb = new QComboBox;
    m_texture_cb->addItem(tr("Uni"), static_cast<int>(Qt::SolidPattern));
    m_texture_cb->addItem(tr("Dense 1"), static_cast<int>(Qt::Dense1Pattern));
    m_texture_cb->addItem(tr("Dense 2"), static_cast<int>(Qt::Dense2Pattern));
    m_texture_cb->addItem(tr("Dense 3"), static_cast<int>(Qt::Dense3Pattern));
    m_texture_cb->addItem(tr("Dense 4"), static_cast<int>(Qt::Dense4Pattern));
    m_texture_cb->addItem(tr("Dense 5"), static_cast<int>(Qt::Dense5Pattern));
    m_texture_cb->addItem(tr("Dense 6"), static_cast<int>(Qt::Dense6Pattern));
    m_texture_cb->addItem(tr("Dense 7"), static_cast<int>(Qt::Dense7Pattern));
    m_texture_cb->addItem(tr("Horizontal"), static_cast<int>(Qt::HorPattern));
    m_texture_cb->addItem(tr("Vertical"), static_cast<int>(Qt::VerPattern));
    m_texture_cb->addItem(tr("Croisé"), static_cast<int>(Qt::CrossPattern));
    m_texture_cb->addItem(tr("Diagonal montante"), static_cast<int>(Qt::BDiagPattern));
    m_texture_cb->addItem(tr("Diagonal decendante"), static_cast<int>(Qt::FDiagPattern));
    m_texture_cb->addItem(tr("Diagonal Croisée"), static_cast<int>(Qt::DiagCrossPattern));
    connect(m_couleur_fond_bouton,&QPushButton::clicked,this,[this]()
        {set_color(Fond,QColorDialog::getColor(m_fond_couleur,this));});

    // Texte
    m_texte_groupe = new QGroupBox(tr("Texte :"));
    m_police_texte_bouton = new QPushButton(tr("Police"));
    m_couleur_texte_bouton = new QPushButton(tr("Couleur"));
    m_texte_label = new QLabel(tr("Texte"));
    m_texte_label->setAlignment(Qt::AlignCenter);
    connect(m_couleur_texte_bouton,&QPushButton::clicked,this,[this]()
        {set_color(Texte,QColorDialog::getColor(m_texte_couleur,this));});
    connect(m_police_texte_bouton,&QPushButton::clicked,this,[this](){
        m_texte_label->setFont(QFontDialog::getFont(nullptr,m_texte_label->font(),this));});

    //Titre
    m_titre_groupe = new QGroupBox(tr("Titre :"));
    m_police_titre_bouton = new QPushButton(tr("Police"));
    m_couleur_titre_bouton = new QPushButton(tr("Couleur"));
    m_titre_label = new QLabel(tr("Titre"));
    m_titre_label->setAlignment(Qt::AlignCenter);
    connect(m_couleur_titre_bouton,&QPushButton::clicked,this,[this]()
        {set_color(Titre,QColorDialog::getColor(m_titre_couleur,this));});
    connect(m_police_titre_bouton,&QPushButton::clicked,this,[this](){
        m_titre_label->setFont(QFontDialog::getFont(nullptr,m_titre_label->font(),this));});

    // Calque
    m_form_layout = new QHBoxLayout;
    m_form_groupe->setLayout(m_form_layout);
    m_form_layout->addWidget(m_form_cb);
    m_form_layout->addWidget(m_bordure_cb);
    m_form_layout->addWidget(m_couleur_bordure_bouton);
    m_form_layout->addWidget(m_bordure_label);

    m_fond_layout = new QHBoxLayout;
    m_fond_groupe->setLayout(m_fond_layout);
    m_fond_layout->addWidget(m_couleur_fond_bouton);
    m_fond_layout->addWidget(m_texture_cb);

    m_texte_layout = new QHBoxLayout;
    m_texte_groupe->setLayout(m_texte_layout);
    m_texte_layout->addWidget(m_police_texte_bouton);
    m_texte_layout->addWidget(m_couleur_texte_bouton);
    m_texte_layout->addWidget(m_texte_label);

    m_titre_layout = new QHBoxLayout;
    m_titre_groupe->setLayout(m_titre_layout);
    m_titre_layout->addWidget(m_police_titre_bouton);
    m_titre_layout->addWidget(m_couleur_titre_bouton);
    m_titre_layout->addWidget(m_titre_label);

    m_fond_layout->addWidget(m_fond_label);
    m_main_layout->addWidget(m_form_groupe);
    m_main_layout->addWidget(m_fond_groupe);
    m_main_layout->addWidget(m_titre_groupe);
    m_main_layout->addWidget(m_texte_groupe);
}

void evenement_style_new_modif_form::set_color(objet ob, const QColor &color) {
    switch (ob) {
    case Bordure:
        m_bordure_couleur = color;
        m_bordure_label->setStyleSheet("QLabel {background-color:"+m_bordure_couleur.name()+" }");
        m_bordure_label->setText(m_fond_couleur.name());
        break;
    case Fond:
        m_fond_couleur = color;
        m_fond_label->setStyleSheet("QLabel {background-color:"+m_fond_couleur.name()+" }");
        m_texte_label->setStyleSheet("QLabel {background-color:"+m_fond_couleur.name()+"; color:"+m_texte_couleur.name()+" }");
        m_titre_label->setStyleSheet("QLabel {background-color:"+m_fond_couleur.name()+"; color:"+m_titre_couleur.name()+" }");
        m_fond_label->setText(m_fond_couleur.name());
        break;
    case Texte:
        m_texte_couleur = color;
        m_texte_label->setStyleSheet("QLabel {background-color:"+m_fond_couleur.name()+"; color:"+m_texte_couleur.name()+" }");
        break;
    case Titre:
        m_titre_couleur = color;
        m_titre_label->setStyleSheet("QLabel {background-color:"+m_fond_couleur.name()+"; color:"+m_titre_couleur.name()+" }");
        break;
    }
}

void evenement_style_new_modif_form::save(){
    evenement_style style(id());
    style.set_couleur_bordure(m_bordure_couleur.name());
    style.set_couleur_fond(m_fond_couleur.name());
    style.set_couleur_texte(m_texte_couleur.name());
    style.set_couleur_titre(m_titre_couleur.name());
    style.set_bordure(m_bordure_cb->currentData().toInt());
    style.set_forme(m_form_cb->currentData().toInt());
    style.set_texture(m_texture_cb->currentData().toUInt());
    style.set_police_texte(m_texte_label->font().toString());
    style.set_police_titre(m_titre_label->font().toString());
    style.set_nom(nom());
    m_bdd.save(style);
}

void evenement_style_new_modif_form::update_data() {
    evenement_style style;
    if(m_new) {
        style.set_id(evenement_style::Id_Style_Default);
        m_bdd.get(style);
    }
    else
        update_temp<evenement_style>(style);
    set_color(Bordure,QColor(style.couleur_bordure()));
    set_color(Fond,QColor(style.couleur_fond()));
    set_color(Texte,QColor(style.couleur_texte()));
    set_color(Titre,QColor(style.couleur_titre()));
    m_bordure_cb->setCurrentIndex(m_bordure_cb->findData(style.bordure()));
    m_form_cb->setCurrentIndex(m_form_cb->findData(style.forme()));
    m_texture_cb->setCurrentIndex(m_texture_cb->findData(style.texture()));
    QFont font;
    font.fromString(style.police_texte());
    m_texte_label->setFont(font);
    font.fromString(style.police_titre());
    m_titre_label->setFont(font);
}

////////////////////////////////////////////////// source_new_modif_form ///////////////////////////////////////////
void source_new_modif_form::save() {
    source src;
    if(!m_new)
        src.set_id(id());
    src.set_nc(nc());
    src.set_nom(nom());
    src.set_type(id_type());
    m_bdd.save(src);
}
void source_new_modif_form::update_data() {
    if(!m_new) {
        source src;
        update_temp<source>(src);
    }
}
