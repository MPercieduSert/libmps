#include "predef_fen_principale.h"

using namespace mps;
using namespace fenetre;

predef_fen_principale::predef_fen_principale(abstract_noyau *noyau, std::unique_ptr<b2d::bdd> &&bdd, predef_zone_centrale *central_zone,
                                         const QString &bdd_path_xml, const QString &config_path, QWidget *parent)
    :   fen_principale (noyau,std::move(bdd),central_zone,bdd_path_xml,config_path,parent) {
    add_menu_bdd();
}

void predef_fen_principale::add_menu_bdd(){
    m_bdd_menu = menuBar()->addMenu(tr("bdd"));

    // Table
    m_bdd_table_menu = m_bdd_menu->addMenu("Tables");
    auto vec = noyau()->bdd().table_entity_names(true);
    for(auto iter = vec.cbegin(); iter != vec.cend(); ++iter) {
        auto action = m_bdd_table_menu->addAction(iter->second);
        auto i =iter->first;
        connect(action,&QAction::triggered,[this,i](){centrale_zone()->open_tab({predef_tab::Gestion_Bdd_Tab_Id,i});});
    }

    // XML
    m_bdd_xml_menu = m_bdd_menu->addMenu("XML");
    auto i_xml = m_bdd_xml_menu->addAction("Importer");
    auto e_xml = m_bdd_xml_menu->addAction("Exporter");
    //auto schXml = m_bddXmlMenu->addAction("Schema Xml");
    connect(i_xml,&QAction::triggered,this,&predef_fen_principale::import_xml);
    connect(e_xml,&QAction::triggered,this,&predef_fen_principale::export_xml);
    //connect(schXml,&QAction::triggered,this,&predef_fen_principale::schema_xml);
}

void predef_fen_principale::connect_action_to_open_tab(QAction *action, const abstract_tab::tab_index &index,
                                           const std::vector<QVariant> &vec)
    {connect(action,&QAction::triggered,this,[this,index,vec](){centrale_zone()->open_tab(index,vec);});}

void predef_fen_principale::export_xml() {
    dialogue::select_table_dialog dialog(noyau()->bdd().table_entity_names(),this);
    if(dialog.exec()){
        auto name = QFileDialog::getSaveFileName(this,
                                                     tr("Exporter des données dans un fichier XML"),
                                                     m_noyau->config().default_directory(),
                                                     tr("Fichier XML (*.xml)"));
        if(!name.isEmpty()){
            vector_ptr<entities::entity> vec;
            for (auto iter = dialog.set().cbegin(); iter != dialog.set().cend(); ++iter)
                vec<<noyau()->bdd().get_list(*iter);
            m_noyau->export_xml(name,std::move(vec),dialog.option());
        }
    }
}

void predef_fen_principale::import_xml() {
    auto name = QFileDialog::getOpenFileName(this,
                                             tr("Importer des données à partir d'un fichier XML"),
                                             m_noyau->config().default_directory(),
                                             tr("Fichier XML (*.xml)"));
    if(!name.isEmpty())
        m_noyau->import_xml(name);
}

void predef_fen_principale::schema_xml() {
    auto name = QFileDialog::getSaveFileName(this,
                                                 tr("Écriture du schema xml des fichiers d'importation de données"),
                                                 m_noyau->config().default_directory(),
                                                 tr("Schema XML (*.xsd)"));
    if(!name.isEmpty())
        m_noyau->schema_xml_for_import(name);
}
