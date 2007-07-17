/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This program is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU General Public License as published by the Free   *
* Software Foundation; either version 2 of the License, or (at your option)    *
* any later version.                                                           *
*                                                                              *
* This program is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for     *
* more details.                                                                *
*                                                                              *
* You should have received a copy of the GNU General Public License along with *
* this program; if not, write to the Free Software Foundation, Inc., 51        *
* Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.                    *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/

#ifdef SOFA_GUI_QTOGREVIEWER
#include "QtOgreViewer/QtOgreViewer.h"
#endif

#ifdef SOFA_GUI_QTVIEWER
#include "QtViewer/QtViewer.h"
#endif

#ifdef SOFA_GUI_QGLVIEWER
#include "QtGLViewer/QtGLViewer.h"
#endif
#include "RealGUI.h"


#include <sofa/simulation/tree/Simulation.h>
#include <sofa/simulation/tree/InitAction.h>
#include <sofa/simulation/tree/MutationListener.h>
#include <sofa/simulation/tree/Colors.h>

#include <sofa/helper/system/SetDirectory.h>
#include <sofa/helper/system/FileRepository.h>

#include <sofa/simulation/automatescheduler/ThreadSimulation.h>
#include <sofa/simulation/automatescheduler/ExecBus.h>
#include <sofa/simulation/automatescheduler/Node.h>

#include <sofa/core/objectmodel/BaseObject.h>
#include <sofa/component/topology/MeshTopology.h>


#include <limits.h>

namespace sofa
{
namespace simulation
{
namespace automatescheduler
{
extern simulation::tree::GNode* groot;
}
}
}


#ifdef QT_MODULE_QT3SUPPORT
#include <QWidget>
#include <QStackedWidget>
#include <QLayout>
#include <Q3ListViewItem>
#include <Q3ListView>
#include <QStatusBar>
#include <QRadioButton>
#include <QCheckBox>
#include <QSplitter>
#include <Q3TextEdit>
#include <QCursor>
#include <QAction>
#include <QMessageBox>
#include <Q3FileDialog>
#include <QTabWidget>
#include <Q3PopupMenu>

#else
#include <qwidget.h>
#include <qwidgetstack.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qstatusbar.h>
#include <qfiledialog.h>
#include <qheader.h>
#include <qimage.h>
#include <qsplitter.h>
#include <qtextedit.h>
#include <qcursor.h>
#include <qapplication.h>
#include <qaction.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qtabwidget.h>
#include <qpopupmenu.h>
#endif

#include <GenGraphForm.h>



namespace sofa
{

namespace gui
{

namespace guiviewer
{

#ifdef QT_MODULE_QT3SUPPORT
typedef Q3ListView QListView;
typedef Q3FileDialog QFileDialog;
typedef Q3DockWindow QDockWindow;
typedef QStackedWidget QWidgetStack;
typedef Q3TextEdit QTextEdit;
typedef Q3PopupMenu QPopupMenu;
#else
typedef QListViewItem Q3ListViewItem;
typedef QListView Q3ListView;
typedef QFileDialog Q3FileDialog;
//typedef QWidgetStack QStackedWidget;
typedef QTextEdit Q3TextEdit;
typedef QPopupMenu Q3PopupMenu;
#endif


using sofa::core::objectmodel::BaseObject;
using sofa::simulation::tree::GNode;
using namespace sofa::helper::system::thread;
using namespace sofa::simulation::tree;
using namespace sofa::simulation::automatescheduler;


///////////////////////////////////////////////////////////
//////////////////// SofaGUI Interface ////////////////////
///////////////////////////////////////////////////////////

#ifdef SOFA_GUI_QGLVIEWER
SOFA_DECL_CLASS(QGLViewerGUI)
int QGLViewerGUIClass = SofaGUI::RegisterGUI("qglviewer", &RealGUI::CreateGUI, &RealGUI::InitGUI, 3);
#endif
#ifdef SOFA_GUI_QTVIEWER
SOFA_DECL_CLASS(QTGUI)
int QtGUIClass = SofaGUI::RegisterGUI("qt", &RealGUI::CreateGUI, &RealGUI::InitGUI, 2);
#endif
#ifdef SOFA_GUI_QTOGREVIEWER
SOFA_DECL_CLASS(OgreGUI)
int QtOGREGUIClass = SofaGUI::RegisterGUI("ogre", &RealGUI::CreateGUI, &RealGUI::InitGUI, 1);
#endif

int RealGUI::InitGUI(const char* name, const std::vector<std::string>& /* options */)
{
#ifdef SOFA_GUI_QGLVIEWER
    if (!name[0] || !strcmp(name,"qglviewer"))
    {
        return sofa::gui::guiqglviewer::QtGLViewer::EnableViewer();
    }
    else
#endif
#ifdef SOFA_GUI_QTVIEWER
        if (!name[0] || !strcmp(name,"qt"))
        {
            return sofa::gui::qt::QtViewer::EnableViewer();
        }
        else
#endif
#ifdef SOFA_GUI_QTOGREVIEWER
            if (!name[0] || !strcmp(name,"ogre"))
            {
                return sofa::gui::qtogreviewer::QtOgreViewer::EnableViewer();
            }
            else
#endif
            {
                std::cerr << "ERROR(QtGUI): unknown or disabled gui name "<<name<<std::endl;
                return 1;
            }
}

extern QApplication* application; // = NULL;
extern RealGUI* gui;

SofaGUI* RealGUI::CreateGUI(const char* name, const std::vector<std::string>& options, sofa::simulation::tree::GNode* groot, const char* filename)
{
    {
        int argc=1;
        char* argv[1];
        argv[0] = strdup(SofaGUI::GetProgramName());
        application = new QApplication(argc,argv);
        free(argv[0]);
    }
    // create interface
    gui = new RealGUI( name, options );
    if (groot)
        gui->setScene(groot, filename);

    //gui->viewer->SwitchToPresetView();

    application->setMainWidget( gui );

    // Threads Management
    if (ThreadSimulation::initialized())
    {
        ThreadSimulation::getInstance()->computeVModelsList(groot);
        groot->setMultiThreadSimulation(true);
        sofa::simulation::automatescheduler::groot = groot;

        Automate::setDrawCB(gui->viewer);

        gui->viewer->getQWidget()->update();
        ThreadSimulation::getInstance()->start();
    }
    // show the gui
    gui->show();
    return gui;
}

int RealGUI::mainLoop()
{
    return application->exec();
}

void RealGUI::redraw()
{
    viewer->getQWidget()->update();
}

int RealGUI::closeGUI()
{
    delete this;
    return 0;
}

sofa::simulation::tree::GNode* RealGUI::currentSimulation()
{
    return viewer->getScene();
}




RealGUI::RealGUI( const char* viewername, const std::vector<std::string>& /*options*/)
    : viewerName(viewername), viewer(NULL),  modifyDialogOpened(0),  currentTab(NULL), graphListener(NULL), dialog(NULL)
{

    left_stack = new QWidgetStack(splitter2);
#ifndef QT_MODULE_QT3SUPPORT
    GUILayout->addWidget(left_stack);
#endif

    connect( startButton, SIGNAL( toggled(bool) ), this , SLOT( playpauseGUI(bool) ) );

    fpsLabel = new QLabel("9999.9 FPS", statusBar());
    fpsLabel->setAlignment(Qt::AlignRight);
    fpsLabel->setMinimumSize(fpsLabel->sizeHint());
    fpsLabel->clear();

    timeLabel = new QLabel("T: 999.9999 s", statusBar());
    timeLabel->setAlignment(Qt::AlignLeft);
    timeLabel->setMinimumSize(timeLabel->sizeHint());
    timeLabel->clear();


    statusBar()->addWidget(fpsLabel);
    statusBar()->addWidget(timeLabel);

    timerStep = new QTimer(this);
    connect( timerStep, SIGNAL(timeout()), this, SLOT(step()) );
    connect( ResetSceneButton, SIGNAL( clicked() ), this, SLOT( resetScene() ) );
    connect( dtEdit, SIGNAL( textChanged(const QString&) ), this, SLOT( setDt(const QString&) ) );
    //connect( ResetViewButton, SIGNAL( clicked() ), viewer->getQWidget(), SLOT( resetView() ) );
    //connect( SaveViewButton, SIGNAL( clicked() ), viewer->getQWidget(), SLOT( saveView() ) );
    connect( showVisual, SIGNAL( toggled(bool) ), this, SLOT( slot_showVisual(bool) ) );
    connect( showBehavior, SIGNAL( toggled(bool) ), this, SLOT( slot_showBehavior(bool) ) );
    connect( showCollision, SIGNAL( toggled(bool) ), this, SLOT( slot_showCollision(bool) ) );
    connect( showBoundingCollision, SIGNAL( toggled(bool) ), this, SLOT( slot_showBoundingCollision(bool) ) );
    connect( showMapping, SIGNAL( toggled(bool) ), this, SLOT( slot_showMapping(bool) ) );
    connect( showMechanicalMapping, SIGNAL( toggled(bool) ), this, SLOT( slot_showMechanicalMapping(bool) ) );
    connect( showForceField, SIGNAL( toggled(bool) ), this, SLOT( slot_showForceField(bool) ) );
    connect( showInteractionForceField, SIGNAL( toggled(bool) ), this, SLOT( slot_showInteractionForceField(bool) ) );
    connect( showWireFrame, SIGNAL( toggled(bool) ), this, SLOT( slot_showWireFrame(bool) ) );
    connect( showNormals, SIGNAL( toggled(bool) ), this, SLOT( slot_showNormals(bool) ) );
    connect( stepButton, SIGNAL( clicked() ), this, SLOT( step() ) );
    //connect( screenshotButton, SIGNAL( clicked() ), viewer->getQWidget(), SLOT( screenshot() ) );
    //connect( xmlSave_pushButton, SIGNAL( pressed() ), this, SLOT( saveXML() ) );
    connect( ExportGraphButton, SIGNAL( clicked() ), this, SLOT( exportGraph() ) );
    //connect( exportGraphAction, SIGNAL( activated() ), viewer, SLOT( exportGraph() ) );
    //connect( sizeW, SIGNAL( valueChanged(int) ), viewer->getQWidget(), SLOT( setSizeW(int) ) );
    //connect( sizeH, SIGNAL( valueChanged(int) ), viewer->getQWidget(), SLOT( setSizeH(int) ) );
    connect( dumpStateCheckBox, SIGNAL( toggled(bool) ), this, SLOT( dumpState(bool) ) );
    connect( exportGnuplotFilesCheckbox, SIGNAL(toggled(bool)), this, SLOT(setExportGnuplot(bool)) );
    connect( displayComputationTimeCheckBox, SIGNAL( toggled(bool) ), this, SLOT( displayComputationTime(bool) ) );

    //Dialog Add Object


    connect( tabs, SIGNAL( currentChanged(QWidget*) ), this, SLOT( currentTabChanged(QWidget*) ) );

    addViewer();

    currentTabChanged(tabs->currentPage());


#ifdef SOFA_PML
    pmlreader = NULL;
    lmlreader = NULL;
#endif
}

RealGUI::~RealGUI()
{
#ifdef SOFA_PML
    if(pmlreader)
    {
        delete pmlreader;
        pmlreader = NULL;
    }
    if(lmlreader)
    {
        delete lmlreader;
        lmlreader = NULL;
    }
#endif
}

void RealGUI::init()
{

    _animationOBJ = false;
    _animationOBJcounter = 0;
    m_dumpState = false;
    m_dumpStateStream = 0;
    m_displayComputationTime = false;
    m_exportGnuplot = false;

    //Read the object.txt that contains the information about the objects which can be added to the scenes whithin a given BoundingBox and scale range
    std::string object("object.txt");
    if (!sofa::helper::system::DataRepository.findFile(  object ) )
        return;


    object = sofa::helper::system::DataRepository.getFile(  object );
    char str [80];
    FILE * pFile;

    pFile = fopen ( object.c_str() ,"r");
    int end=0;
    end = fscanf(pFile, "BoundingBox: %f %f %f %f %f %f\n",
            &object_BoundingBox[0],&object_BoundingBox[1],&object_BoundingBox[2],&object_BoundingBox[3],&object_BoundingBox[4],&object_BoundingBox[5]);
    if (end == EOF) return;

    end = fscanf(pFile, "Scale: %f %f\n",
            &object_Scale[0], &object_Scale[1]);
    if (end == EOF) return;

    while (true)
    {
        end = fscanf(pFile, "%s", str);
        if (end == EOF) return;
        std::cout << str << " Added to objects\n";
        list_object.push_back(std::string(str));
    }

}

void RealGUI::addViewer()
{
    init();
    const char* name = viewerName;

    // set menu state
#ifdef SOFA_GUI_QTVIEWER
    viewerOpenGLAction->setEnabled(true);
#else
    viewerOpenGLAction->setEnabled(false);
    viewerOpenGLAction->setToolTip("enable SOFA_GUI_QTVIEWER in sofa-local.cfg to activate");
#endif
    viewerOpenGLAction->setOn(false);
#ifdef SOFA_GUI_QGLVIEWER
    viewerQGLViewerAction->setEnabled(true);
#else
    viewerQGLViewerAction->setEnabled(false);
    viewerQGLViewerAction->setToolTip("enable SOFA_GUI_QGLVIEWER in sofa-local.cfg to activate");
#endif
    viewerQGLViewerAction->setOn(false);
#ifdef SOFA_GUI_QTOGREVIEWER
    viewerOGREAction->setEnabled(true);
#else
    viewerOGREAction->setEnabled(false);
    viewerOGREAction->setToolTip("enable SOFA_GUI_QTOGREVIEWER in sofa-local.cfg to activate");
#endif
    viewerOGREAction->setOn(false);

#ifdef SOFA_GUI_QGLVIEWER
    if (!name[0] || !strcmp(name,"qglviewer"))
    {
        viewer = new sofa::gui::guiqglviewer::QtGLViewer( left_stack, "viewer" );
        viewerQGLViewerAction->setOn(true);
    }
    else
#endif
#ifdef SOFA_GUI_QTVIEWER
        if (!name[0] || !strcmp(name,"qt"))
        {
            viewer = new sofa::gui::qt::QtViewer( left_stack, "viewer" );
            viewerOpenGLAction->setOn(true);
        }
        else
#endif
#ifdef SOFA_GUI_QTOGREVIEWER
            if (!name[0] || !strcmp(name,"ogre"))
            {
                viewer = new sofa::gui::qtogreviewer::QtOgreViewer( left_stack , "viewer" );
                viewerOGREAction->setOn(true);
            }
            else
#endif
            {
                std::cerr << "ERROR(QtGUI): unknown or disabled viewer name "<<name<<std::endl;
                application->exit();
            }
#ifdef QT_MODULE_QT3SUPPORT
    left_stack->addWidget ( viewer->getQWidget() );
    left_stack->setCurrentWidget ( viewer->getQWidget() );
#else
    int id_viewer = left_stack->addWidget(viewer->getQWidget());
    left_stack->raiseWidget(id_viewer);
#endif
    viewer->getQWidget()->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 100, 1,
            viewer->getQWidget()->sizePolicy().hasHeightForWidth() ) );
    viewer->getQWidget()->setMinimumSize( QSize( 0, 0 ) );
#ifndef QT_MODULE_QT3SUPPORT
    viewer->getQWidget()->setCursor( QCursor( 2 ) );
#endif
    viewer->getQWidget()->setMouseTracking( TRUE );

#ifdef QT_MODULE_QT3SUPPORT
    viewer->getQWidget()->setFocusPolicy( Qt::StrongFocus );
#else
    viewer->getQWidget()->setFocusPolicy( QWidget::StrongFocus );
#endif




    viewer->setup();

    connect( ResetViewButton, SIGNAL( clicked() ), viewer->getQWidget(), SLOT( resetView() ) );
    connect( SaveViewButton, SIGNAL( clicked() ), viewer->getQWidget(), SLOT( saveView() ) );
    connect( screenshotButton, SIGNAL( clicked() ), this, SLOT( screenshot() ) );
    connect( sizeW, SIGNAL( valueChanged(int) ), viewer->getQWidget(), SLOT( setSizeW(int) ) );
    connect( sizeH, SIGNAL( valueChanged(int) ), viewer->getQWidget(), SLOT( setSizeH(int) ) );
    connect(viewer->getQWidget(), SIGNAL(resizeW(int)), sizeW, SLOT(setValue(int)));
    connect(viewer->getQWidget(), SIGNAL(resizeH(int)), sizeH, SLOT(setValue(int)));

    QSplitter *splitter_ptr = dynamic_cast<QSplitter *> ( splitter2 );
    splitter_ptr->moveToLast(left_stack);
    splitter_ptr->setOpaqueResize(false);
#ifdef QT_MODULE_QT3SUPPORT
    QList<int> list;
#else
    QValueList<int> list;
#endif
    list.push_back(75);
    list.push_back(500);
    splitter_ptr->setSizes(list);

    viewer->getQWidget()->setFocus();
    viewer->getQWidget()->show();
    viewer->getQWidget()->update();
    setGUI();
}

void RealGUI::viewerOpenGL()
{
    viewerOpenGLAction->setOn(setViewer("qt"));
}

void RealGUI::viewerQGLViewer()
{
    viewerOpenGLAction->setOn(setViewer("qglviewer"));
}

void RealGUI::viewerOGRE()
{
    viewerOpenGLAction->setOn(setViewer("ogre"));
}

bool RealGUI::setViewer(const char* name)
{
    if (!strcmp(name,viewerName))
        return true; // nothing to do
    if (!strcmp(name,"qt"))
    {
#ifndef SOFA_GUI_QTVIEWER
        std::cerr << "OpenGL viewer not activated. Enable SOFA_GUI_QGLVIEWER in sofa-local.cfg to activate.\n";
        return false;
#endif
    }
    else if (!strcmp(name,"qglviewer"))
    {
#ifndef SOFA_GUI_QGLVIEWER
        std::cerr << "QGLViewer viewer not activated. Enable SOFA_GUI_QGLVIEWER in sofa-local.cfg to activate.\n";
        return false;
#endif
    }
    else if (!strcmp(name,"ogre"))
    {
#ifndef SOFA_GUI_QTOGREVIEWER
        std::cerr << "OGRE viewer not activated. Enable SOFA_GUI_QTOGREVIEWER in sofa-local.cfg to activate.\n";
        return false;
#endif
    }
    else
    {
        std::cerr << "Unknown viewer.\n";
        return false;
    }
    if ( QMessageBox::warning ( this, "Changing Viewer", "Changing viewer requires to reload the current scene.\nAre you sure you want to do that ?", QMessageBox::Yes | QMessageBox::Default, QMessageBox::No ) != QMessageBox::Yes )
        return false;

    std::string filename = viewer->getSceneFileName();
    GNode* groot = new GNode; // empty scene to do the transition
    setScene(groot,filename.c_str());
    left_stack->removeWidget(viewer->getQWidget());
    delete viewer;
    viewer = NULL;
    // Disable Viewer-specific classes
#ifdef SOFA_GUI_QTVIEWER
    if (!strcmp(viewerName,"qt"))
    {
        sofa::gui::qt::QtViewer::DisableViewer();
    }
    else
#endif
#ifdef SOFA_GUI_QGLVIEWER
        if (!strcmp(viewerName,"qglviewer"))
        {
            sofa::gui::guiqglviewer::QtGLViewer::DisableViewer();
        }
        else
#endif
#ifdef SOFA_GUI_QTOGREVIEWER
            if (!strcmp(viewerName,"ogre"))
            {
                sofa::gui::qtogreviewer::QtOgreViewer::DisableViewer();
            }
            else
#endif
            {}
    // Enable Viewer-specific classes
#ifdef SOFA_GUI_QTVIEWER
    if (!strcmp(name,"qt"))
    {
        sofa::gui::qt::QtViewer::EnableViewer();
    }
    else
#endif
#ifdef SOFA_GUI_QGLVIEWER
        if (!strcmp(name,"qglviewer"))
        {
            sofa::gui::guiqglviewer::QtGLViewer::EnableViewer();
        }
        else
#endif
#ifdef SOFA_GUI_QTOGREVIEWER
            if (!strcmp(name,"ogre"))
            {
                sofa::gui::qtogreviewer::QtOgreViewer::EnableViewer();
            }
#endif

    viewerName = name;


    if (graphListener)
        graphListener->removeChild(NULL, groot);

    addViewer();
    fileOpen(filename.c_str());
    return true;
}

void RealGUI::fileOpen(const char* filename)
{
    //Hide the dialog to add a new object in the graph
    if (dialog != NULL) dialog->hide();
    //Hide all the dialogs to modify the graph
    emit( newScene());
    modifyDialogOpened = 0;

    //left_stack->removeWidget(viewer->getQWidget());
    //graphListener->removeChild(NULL, groot);
    //delete viewer;
    //viewer = NULL;
    //addViewer(filename);
    GNode* groot = Simulation::load(filename);
    if (groot == NULL)
    {
        qFatal("Failed to load %s",filename);
        return;
    }
    setScene(groot, filename);
}

#ifdef SOFA_PML
void RealGUI::pmlOpen(const char* filename, bool /*resetView*/)
{
    std::string scene = "PML/default.scn";
    if (!sofa::helper::system::DataRepository.findFile(scene))
    {
        std::cerr << "File " << scene << " not found " << std::endl;
        return;
    }
    groot = Simulation::load(scene.c_str());
    if (groot)
    {
        if (!pmlreader) pmlreader = new PMLReader;
        pmlreader->BuildStructure(filename, groot);
        setScene(groot, filename);
    }
}

void RealGUI::lmlOpen(const char* filename)
{
    if (pmlreader)
    {
        if (lmlreader != NULL) delete lmlreader;
        lmlreader = new LMLReader; std::cout <<"New lml reader\n";
        lmlreader->BuildStructure(filename, pmlreader);
        groot = viewer->getScene();
        Simulation::init(groot);

    }
    else
        std::cerr<<"You must load the pml file before the lml file"<<endl;
}
#endif

void RealGUI::setScene(GNode* groot, const char* filename)
{
    if (viewer->getScene()!=NULL)
    {

        Simulation::unload(viewer->getScene());

        if (graphListener!=NULL)
        {
            delete graphListener;

            graphListener = NULL;
        }
        graphView->clear();
    }

    setTitle(filename);
    //this->groot = groot;
    //sceneFileName = filename;

    viewer->setScene(groot, filename);
    eventNewTime();


    showVisual->setChecked(groot->getShowVisualModels());
    showBehavior->setChecked(groot->getShowBehaviorModels());
    showCollision->setChecked(groot->getShowCollisionModels());
    showBoundingCollision->setChecked(groot->getShowBoundingCollisionModels());
    showForceField->setChecked(groot->getShowForceFields());
    showInteractionForceField->setChecked(groot->getShowInteractionForceFields());
    showMapping->setChecked(groot->getShowMappings());
    showMechanicalMapping->setChecked(groot->getShowMechanicalMappings());
    showWireFrame->setChecked(groot->getShowWireFrame());
    showNormals->setChecked(groot->getShowNormals());
    startButton->setOn(groot->getContext()->getAnimate());
    dtEdit->setText(QString::number(groot->getDt()));

    graphView->setSorting(-1);
    //graphView->setTreeStepSize(10);
    graphView->header()->hide();
    //dumpGraph(groot, new Q3ListViewItem(graphView));
    graphListener = new GraphListenerQListView(graphView);
    graphListener->addChild(NULL, groot);
    if (currentTab != TabGraph)
    {
        std::cout << "Hide Graph"<<std::endl;
        graphListener->freeze(groot);
    }
}


void RealGUI::screenshot()
{

    QString filename;

    filename = QFileDialog::getSaveFileName(
            viewer->screenshotName().c_str(),
            "Images (*.png *.bmp *.jpg)",
            this,
            "save file dialog"
            "Choose a filename to save under" );
    viewer->getQWidget()->repaint();
    if (filename != "")
    {
        std::ostringstream ofilename;
        const char* begin = filename;
        const char* end = strrchr(begin,'_');
        if (!end) end = begin + filename.length();
        ofilename << std::string(begin, end);
        ofilename << "_";
        viewer->setPrefix(ofilename.str());
#ifdef QT_MODULE_QT3SUPPORT
        viewer->screenshot(filename.toStdString());
#else
        viewer->screenshot(filename);
#endif

    }
}

void RealGUI::fileOpen()
{
    std::string filename = viewer->getSceneFileName();

#ifdef SOFA_PML
    QString s = Q3FileDialog::getOpenFileName(filename.empty()?NULL:filename.c_str(), "Scenes (*.scn *.pml *.lml)",  this, "open file dialog",  "Choose a file to open" );

    if (s.length()>0)
    {
        if(s.endsWith(".pml"))
            pmlOpen(s);
        else if(s.endsWith(".lml"))
            lmlOpen(s);
        else
            fileOpen(s);
    }
#else
    QString s = Q3FileDialog::getOpenFileName(filename.empty()?NULL:filename.c_str(), "Scenes (*.scn)", this, "open file dialog", "Choose a file to open" );

    if (s.length()>0)
        fileOpen(s);
#endif
}

void RealGUI::fileReload()
{

    std::string filename = viewer->getSceneFileName();
    QString s = filename.c_str();

    if (filename.empty()) { std::cerr << "Reload failed: no file loaded.\n"; return;}

#ifdef SOFA_PML
    if (s.length()>0)
    {
        if(s.endsWith(".pml"))
            pmlOpen(s);
        else if(s.endsWith(".lml"))
            lmlOpen(s);
        else
            fileOpen(s);
    }
#else
    fileOpen(filename.c_str());
#endif



}

void RealGUI::fileSaveAs()
{
    QString s;
    std::string filename = viewer->getSceneFileName();
#ifdef SOFA_PML
    s = Q3FileDialog::getSaveFileName(filename.empty()?NULL:filename.c_str(), "Scenes (*.scn *.pml)",  this, "save file dialog",  "Choose where the scene will be saved" );
    if (s.length()>0)
    {
        if(pmlreader && s.endsWith(".pml"))
            pmlreader->saveAsPML(s);
        else
            fileSaveAs(s);
    }
#else
    s = Q3FileDialog::getSaveFileName( filename.empty()?NULL:filename.c_str(), "Scenes (*.scn)", this, "save file dialog", "Choose where the scene will be saved" );
    if (s.length()>0)
        fileSaveAs(s);
#endif

}

void RealGUI::fileSaveAs(const char* filename)
{
    Simulation::printXML( viewer->getScene(), filename);
}

void RealGUI::fileExit()
{
    close();
}

void RealGUI::saveXML()
{
    Simulation::printXML( viewer->getScene(), "scene.scn");
}

void RealGUI::setTitle( const char* windowTitle )
{
    std::string str = "Sofa";
    if (windowTitle && *windowTitle)
    {
        str += " - ";
        str += windowTitle;
    }
#ifdef _WIN32
    setWindowTitle(str.c_str());
#else
    setCaption(str.c_str());
#endif
}


void RealGUI::playpauseGUI(bool value)
{
    if (value) {timerStep->start(0);}
    else {timerStep->stop();}
    if (getScene())  getScene()->getContext()->setAnimate(value);
}


void RealGUI::setGUI(void)
{
    textEdit1->setText(viewer->helpString());

#ifdef SOFA_GUI_QTOGREVIEWER
    //Hide unused options
    if (!strcmp(viewerName,"ogre"))
    {
        showVisual->hide();
        showBehavior->hide();
        showCollision->hide();
        showBoundingCollision->hide();
        showMapping->hide();
        showMechanicalMapping->hide();
        showForceField->hide();
        showInteractionForceField->hide();
        showWireFrame->hide();
        showNormals->hide();
    }
    else
    {
        showVisual->show();
        showBehavior->show();
        showCollision->show();
        showBoundingCollision->show();
        showMapping->show();
        showMechanicalMapping->show();
        showForceField->show();
        showInteractionForceField->show();
        showWireFrame->show();
        showNormals->show();
    }
#endif
}
//###################################################################################################################


//*****************************************************************************************
//called at each step of the rendering

void RealGUI::step()
{
    GNode* groot = viewer->getScene();
    if (groot == NULL) return;

    if (groot->getContext()->getMultiThreadSimulation())
    {
        static Node* n = NULL;

        if(ExecBus::getInstance() != NULL)
        {
            n = ExecBus::getInstance()->getNext("displayThread", n);

            if (n)
            {
                n->execute("displayThread");
            }
        }
    }
    else
    {
        if (viewer->ready()) return;
        //groot->setLogTime(true);

        Simulation::animate(groot);

        if( m_dumpState )
            Simulation::dumpState( groot, *m_dumpStateStream );
        if( m_exportGnuplot )
            Simulation::exportGnuplot( groot, groot->getTime() );

        viewer->wait();

        eventNewStep();
        eventNewTime();

#ifdef QT_MODULE_QT3SUPPORT
        viewer->getQWidget()->setUpdatesEnabled(true);
#endif
        viewer->getQWidget()->update();
    }


    if (_animationOBJ)
    {
#ifdef CAPTURE_PERIOD
        static int counter = 0;
        if ((counter++ % CAPTURE_PERIOD)==0)
#endif
        {
            exportOBJ(false);
            ++_animationOBJcounter;
        }
    }
}

//*****************************************************************************************
// Update sofa Simulation with the time step

void RealGUI::eventNewStep()
{
    static ctime_t beginTime[10];
    static const ctime_t timeTicks = CTime::getRefTicksPerSec();
    static int frameCounter = 0;
    GNode* groot = getScene();
    if (frameCounter==0)
    {
        ctime_t t = CTime::getRefTime();
        for (int i=0; i<10; i++)
            beginTime[i] = t;
    }
    ++frameCounter;
    if ((frameCounter%10) == 0)
    {
        ctime_t curtime = CTime::getRefTime();
        int i = ((frameCounter/10)%10);
        double fps = ((double)timeTicks / (curtime - beginTime[i]))*(frameCounter<100?frameCounter:100);
        // 	    emit newFPS(fps);
        char buf[100];
        sprintf(buf, "%.1f FPS", fps);
        fpsLabel->setText(buf);
        // 	    emit newFPS(buf);
        beginTime[i] = curtime;
        //frameCounter = 0;
    }

    if (m_displayComputationTime && (frameCounter%100) == 0 && groot!=NULL)
    {

        std::cout << "========== ITERATION " << frameCounter << " ==========\n";
        const sofa::simulation::tree::GNode::NodeTimer& total = groot->getTotalTime();
        const std::map<std::string, sofa::simulation::tree::GNode::NodeTimer>& times = groot->getActionTime();
        const std::map<std::string, std::map<sofa::core::objectmodel::BaseObject*, sofa::simulation::tree::GNode::ObjectTimer> >& objtimes = groot->getObjectTime();
        const double fact = 1000000.0 / (100*groot->getTimeFreq());
        for (std::map<std::string, sofa::simulation::tree::GNode::NodeTimer>::const_iterator it = times.begin(); it != times.end(); ++it)
        {
            std::cout << "TIME "<<it->first<<": " << ((int)(fact*it->second.tTree+0.5))*0.001 << " ms (" << (1000*it->second.tTree/total.tTree)*0.1 << " %).\n";
            std::map<std::string, std::map<sofa::core::objectmodel::BaseObject*, sofa::simulation::tree::GNode::ObjectTimer> >::const_iterator it1 = objtimes.find(it->first);
            if (it1 != objtimes.end())
            {
                for (std::map<sofa::core::objectmodel::BaseObject*, sofa::simulation::tree::GNode::ObjectTimer>::const_iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
                {
                    std::cout << "  "<< sofa::helper::gettypename(typeid(*(it2->first)))<<" "<< it2->first->getName() <<": "
                            << ((int)(fact*it2->second.tObject+0.5))*0.001 << " ms (" << (1000*it2->second.tObject/it->second.tTree)*0.1 << " %).\n";
                }
            }
        }
        for (std::map<std::string, std::map<sofa::core::objectmodel::BaseObject*, sofa::simulation::tree::GNode::ObjectTimer> >::const_iterator it = objtimes.begin(); it != objtimes.end(); ++it)
        {
            if (times.count(it->first)>0) continue;
            ctime_t ttotal = 0;
            for (std::map<sofa::core::objectmodel::BaseObject*, sofa::simulation::tree::GNode::ObjectTimer>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                ttotal += it2->second.tObject;
            std::cout << "TIME "<<it->first<<": " << ((int)(fact*ttotal+0.5))*0.001 << " ms (" << (1000*ttotal/total.tTree)*0.1 << " %).\n";
            if (ttotal > 0)
                for (std::map<sofa::core::objectmodel::BaseObject*, sofa::simulation::tree::GNode::ObjectTimer>::const_iterator it2 = it->second.begin(); it2 != it->second.end(); ++it2)
                {
                    std::cout << "  "<< sofa::helper::gettypename(typeid(*(it2->first)))<<" "<< it2->first->getName() <<": "
                            << ((int)(fact*it2->second.tObject+0.5))*0.001 << " ms (" << (1000*it2->second.tObject/ttotal)*0.1 << " %).\n";
                }
        }
        std::cout << "TOTAL TIME: " << ((int)(fact*total.tTree+0.5))*0.001 << " ms (" << ((int)(100/(fact*total.tTree*0.000001)+0.5))*0.01 << " FPS).\n";
        groot->resetTime();

    }
}



void RealGUI::eventNewTime()
{
    GNode* groot = getScene();
    if (groot)
    {

        double time = groot->getTime();
        // 	    emit newTime(time);
        char buf[100];
        sprintf(buf, "T: %.3f s", time);
        timeLabel->setText(buf);
        // 	    emit newTime(buf);

    }
}




//*****************************************************************************************
// Set the time between each iteration of the Sofa Simulation

void RealGUI::setDt(double value)
{
    GNode* groot = getScene();
    if (value > 0.0)
    {

        if (groot)
            groot->getContext()->setDt(value);
    }
}

void RealGUI::setDt(const QString& value)
{
    setDt(value.toDouble());
}


//*****************************************************************************************
// Reset the simulation to t=0
void RealGUI::resetScene()
{
    GNode* groot = getScene();
    if (groot)
    {
        Simulation::reset(groot);
        eventNewTime();
        viewer->getQWidget()->update();
    }
}


//*****************************************************************************************
// Set what to display
void RealGUI::slot_showVisual(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowVisualModels(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}

void RealGUI::slot_showBehavior(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowBehaviorModels(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}

void RealGUI::slot_showCollision(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowCollisionModels(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}

void RealGUI::slot_showBoundingCollision(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowBoundingCollisionModels(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}

void RealGUI::slot_showMapping(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowMappings(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}

void RealGUI::slot_showMechanicalMapping(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowMechanicalMappings(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}

void RealGUI::slot_showForceField(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowForceFields(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}

void RealGUI::slot_showInteractionForceField(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowInteractionForceFields(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}

void RealGUI::slot_showWireFrame(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowWireFrame(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}

void RealGUI::slot_showNormals(bool value)
{
    GNode* groot = getScene();
    if (groot)
    {
        groot->getContext()->setShowNormals(value);
        Simulation::updateContext(groot);
    }
    viewer->getQWidget()->update();
}


//*****************************************************************************************
//
void RealGUI::exportGraph()
{
    exportGraph(getScene());
}


void RealGUI::exportGraph(sofa::simulation::tree::GNode* root)
{

    if (root == NULL) return;
    sofa::gui::guiviewer::GenGraphForm* form = new sofa::gui::guiviewer::GenGraphForm;
    form->setScene(root);
    form->show();
}



//*****************************************************************************************
//
void RealGUI::displayComputationTime(bool value)
{
    GNode* groot = getScene();
    m_displayComputationTime = value;
    if (groot)
    {
        groot->setLogTime(m_displayComputationTime);
    }
}



//*****************************************************************************************
//
void RealGUI::setExportGnuplot( bool exp )
{
    GNode* groot = getScene();
    m_exportGnuplot = exp;
    if( m_exportGnuplot && groot )
    {
        Simulation::initGnuplot( groot );
        Simulation::exportGnuplot( groot, groot->getTime() );
    }
}


//*****************************************************************************************
//
void RealGUI::dumpState(bool value)
{
    m_dumpState = value;
    if( m_dumpState )
    {
        m_dumpStateStream = new std::ofstream("dumpState.data");
    }
    else if( m_dumpStateStream!=NULL )
    {
        delete m_dumpStateStream;
        m_dumpStateStream = 0;
    }
}



//*****************************************************************************************
//
void RealGUI::exportOBJ(bool exportMTL)
{
    GNode* groot = getScene();
    if (!groot) return;
    std::string sceneFileName = viewer->getSceneFileName();
    std::ostringstream ofilename;
    if (!sceneFileName.empty())
    {
        const char* begin = sceneFileName.c_str();
        const char* end = strrchr(begin,'.');
        if (!end) end = begin + sceneFileName.length();
        ofilename << std::string(begin, end);
    }
    else
        ofilename << "scene";

    std::stringstream oss;
    oss.width(5);
    oss.fill('0');
    oss << _animationOBJcounter;

    ofilename << '_' << (oss.str().c_str());
    ofilename << ".obj";
    std::string filename = ofilename.str();
    std::cout << "Exporting OBJ Scene "<<filename<<std::endl;
    Simulation::exportOBJ(groot, filename.c_str(),exportMTL);
}


//*****************************************************************************************
// Called by the animate timer
void RealGUI::animate()
{
    viewer->getQWidget()->update();
}


void RealGUI::keyPressEvent ( QKeyEvent * e )
{
    switch(e->key())
    {

    case Qt::Key_O:
        // --- export to OBJ
    {
        exportOBJ();
        break;
    }
    case Qt::Key_P:
        // --- export to a succession of OBJ to make a video
    {
        _animationOBJ = !_animationOBJ;
        _animationOBJcounter = 0;
        break;
    }
    case Qt::Key_N:
        // -- new object added in the scene whithin a Bounding Box and a scale range
    {
        int index_object = (int)(( rand()/ ((float)RAND_MAX) ) * list_object.size());

        for( std::map<core::objectmodel::Base*, Q3ListViewItem* >::iterator it = graphListener->items.begin() ; it != graphListener->items.end() ; ++ it)
        {
            if(  (*it).second->itemPos() == 0 )
            {
                node_clicked = dynamic_cast< sofa::simulation::tree::GNode *>( (*it).first );
                break;
            }
        }

        if (currentTab != TabGraph)
            graphListener->unfreeze(node_clicked);

        if (node_clicked == NULL) return;

        loadObject(list_object[index_object],
                (object_BoundingBox[3]-object_BoundingBox[0]) * ( rand()/ ((float)RAND_MAX) )  + object_BoundingBox[0],
                (object_BoundingBox[4]-object_BoundingBox[1]) * ( rand()/ ((float)RAND_MAX) )  + object_BoundingBox[1],
                (object_BoundingBox[5]-object_BoundingBox[2]) * ( rand()/ ((float)RAND_MAX) )  + object_BoundingBox[2],
                (object_Scale[1]-object_Scale[0]) * ( rand()/ ((float)RAND_MAX) ) + object_Scale[0]);

        if (currentTab != TabGraph)
            graphListener->freeze(node_clicked);
    }
    default:break;
    }
}


/*****************************************************************************************************************/
// INTERACTION WITH THE GRAPH
/*****************************************************************************************************************/
void RealGUI::currentTabChanged(QWidget* widget)
{
    if (widget == currentTab) return;
    GNode* groot = viewer==NULL ? NULL : viewer->getScene();
    if (widget == TabGraph)
    {
        if (groot && graphListener)
        {
            std::cout << "Show Graph"<<std::endl;
            //graphListener->addChild(NULL, groot);
            graphListener->unfreeze(groot);
        }
    }
    else if (currentTab == TabGraph)
    {
        if (groot && graphListener)
        {
            std::cout << "Hide Graph"<<std::endl;
            //graphListener->removeChild(NULL, groot);
            graphListener->freeze(groot);
        }
    }
    currentTab = widget;
}

/*****************************************************************************************************************/
void RealGUI::DoubleClickeItemInSceneView(QListViewItem *item)
{
    // This happens because the clicked() signal also calls the select callback with
    // NULL as a parameter.
    if(item == NULL)
        return;

    item_clicked = item;

    // cancel the visibility action caused by the double click
    item_clicked->setOpen( !item_clicked->isOpen() );
    graphModify();
}


/*****************************************************************************************************************/
void RealGUI::RightClickedItemInSceneView(QListViewItem *item, const QPoint& point, int index)
{
    if (dialog == NULL)
    {
        //Creation of the file dialog
        dialog = new AddObject(this);
        dialog->setPath(viewer->getSceneFileName());
        dialog->hide();
    }

    //Creation of a popup menu at the mouse position
    item_clicked=item;
    //Search in the graph if the element clicked is a node
    node_clicked = viewer->getScene();
    if (node_clicked == NULL || item_clicked == NULL) return;

    //First initialize with the Root. Test if the node clicked on the graph has the same name as the root.
    if (node_clicked->getName() == item_clicked->text(0).ascii())
    {
        //The node clicked has the same name as the root, but we need to verify the pointer of the node clicked
        node_clicked = verifyNode(node_clicked);
        if (node_clicked == NULL) node_clicked = searchNode(viewer->getScene());

    }
    else node_clicked = searchNode(viewer->getScene());

    QPopupMenu *contextMenu = new QPopupMenu(graphView, "ContextMenu");
    //Creation of the context Menu
    if (node_clicked != NULL)
    {
        contextMenu->insertItem("Collapse", this, SLOT(graphCollapse()));
        contextMenu->insertItem("Expand", this, SLOT(graphExpand()));
        contextMenu->insertSeparator ();
    }

    int indexMenu[3];

    indexMenu[0] = contextMenu->insertItem("Add Node", this, SLOT(graphAddObject()));
    indexMenu[1] = contextMenu->insertItem("Remove Node", this, SLOT(graphRemoveObject()));
    indexMenu[2] = contextMenu->insertItem("Modify", this, SLOT(graphModify()));
    contextMenu->popup(point, index);


    //Enable the option ADD and REMOVE only for the Nodes.
    if (node_clicked == NULL)
    {
        contextMenu->setItemEnabled(indexMenu[0],false);
        contextMenu->setItemEnabled(indexMenu[1],false);
    }
    //if some modifying dialog windows are still open, we mustn't allow the user to remove nodes, it could lead to NULL pointer.
    else if ( modifyDialogOpened != 0)
    {
        contextMenu->setItemEnabled(indexMenu[1],false);
    }


}


/*****************************************************************************************************************/
void RealGUI::graphAddObject()
{
    //Just pop up the dialog window
    if (node_clicked != NULL)
    {
        dialog->show();
        dialog->raise();

        item_clicked = NULL;
    }
}

/*****************************************************************************************************************/
void RealGUI::graphRemoveObject()
{
    if (node_clicked != NULL)
    {
        if (node_clicked->getParent() == NULL)
        {
            //Attempt to destroy the Root node : create an empty node to handle new graph interaction
            GNode *groot = new GNode("Root");

            groot->setShowVisualModels           (1);
            groot->setShowCollisionModels        (0);
            groot->setShowBoundingCollisionModels(0);
            groot->setShowBehaviorModels         (0);
            groot->setShowMappings               (0);
            groot->setShowMechanicalMappings     (0);
            groot->setShowForceFields            (0);
            groot->setShowInteractionForceFields (0);
            groot->setShowWireFrame              (0);
            groot->setShowNormals                (0);

            showVisual->setChecked(groot->getShowVisualModels());
            showBehavior->setChecked(groot->getShowBehaviorModels());
            showCollision->setChecked(groot->getShowCollisionModels());
            showBoundingCollision->setChecked(groot->getShowBoundingCollisionModels());
            showForceField->setChecked(groot->getShowForceFields());
            showInteractionForceField->setChecked(groot->getShowInteractionForceFields());
            showMapping->setChecked(groot->getShowMappings());
            showMechanicalMapping->setChecked(groot->getShowMechanicalMappings());
            showWireFrame->setChecked(groot->getShowWireFrame());
            showNormals->setChecked(groot->getShowNormals());

            viewer->setScene(groot, viewer->getSceneFileName().c_str());
            graphListener->removeChild(NULL, node_clicked);
            graphListener->addChild(NULL, groot);
        }
        else
        {
            node_clicked->getParent()->removeChild(node_clicked);
            graphListener->removeChild(NULL, node_clicked);
        }

        viewer->SwitchToPresetView();
        viewer->getQWidget()->update();
        node_clicked = NULL;
        item_clicked = NULL;
    }
}

/*****************************************************************************************************************/
void RealGUI::graphModify()
{
    if (item_clicked != NULL)
    {
        core::objectmodel::Base* node=NULL;
        for( std::map<core::objectmodel::Base*, Q3ListViewItem* >::iterator it = graphListener->items.begin() ; it != graphListener->items.end() ; ++ it)
        {
            if(  (*it).second == item_clicked )
            {
                node = (*it).first;
                break;
            }
        }

        //Opening of a dialog window automatically created
        ModifyObject *dialogModify = new ModifyObject(this,node->getName().data());
        dialogModify->setNode(node, item_clicked);
        dialogModify->show();
        dialogModify->raise();
        modifyDialogOpened++;
        connect ( this, SIGNAL( newScene()), dialogModify, SLOT( closeNow()));
        item_clicked = NULL;
    }
}
/*****************************************************************************************************************/
//Nodes in the graph can have the same name. To find the right one, we have to verify the pointer itself.
//We return the Nodes clicked
GNode *RealGUI::verifyNode(GNode *node)
{
    std::map<core::objectmodel::Base*, Q3ListViewItem* >::iterator graph_iterator = graphListener->items.find(node);

    while (graph_iterator != graphListener->items.end() )
    {
        if (  item_clicked == graph_iterator->second ) {return dynamic_cast< GNode*>(graph_iterator->first);}
        graph_iterator ++;
    }
    return NULL;
}

/*****************************************************************************************************************/
//Recursive search through the GNode graph.
GNode *RealGUI::searchNode(GNode *node)
{
    if (node == NULL) return NULL;

    GNode *result=NULL;
    //For each child of the node, we are looking for one who has the same name as the one clicked
    GNode::ChildIterator it;
    for (it = node->child.begin(); it != node->child.end(); it++)
    {
        //a node with the same name has been found!!
        result = node->getChild(item_clicked->text(0).ascii());
        if ( result != NULL)
        {
            result = verifyNode(result);
            if (result != NULL)	return result;
        }


        result = searchNode((*it));
        if (result != NULL) return result;
    }
    //Nothing found
    return NULL;
}


/*****************************************************************************************************************/
//Translate an object
void RealGUI::transformObject(GNode *node, double dx, double dy, double dz, double scale)
{

    if (node == NULL) return;
    GNode::ObjectIterator obj_it = node->object.begin();
    //Verify if it exists a mesh topology. In that case, we have to recursively translate the mechanical object and the visual model
// 	bool mesh_topology = false;
// 	bool mechanical_object = false;


// 	//Using the graph corresponding to the current node, we explorate it to find a Topology Element
// 	if (graphListener->items[node] == NULL) return;

// 	Q3ListViewItem *element = graphListener->items[node]->firstChild();
// 	while (element != NULL)
// 	  {
// 	    //We search in the element of the current node, the presence of a MeshTopology: depending on its presence, it will modify the range of the translation
// 	    if (element->firstChild() == NULL)
// 	      {
// 		std::string name = element->text(0);
// 		std::string::size_type end_name = name.rfind(' ');
// 		if (end_name != std::string::npos)
// 		    name.resize(end_name-1);

// 		if (name == "MeshTopology")
// 		    mesh_topology = true;

// 	      }
// 	    element = element->nextSibling();
// 	  }

    //We translate the elements
    while (obj_it != node->object.end())
    {
        if (dynamic_cast< core::componentmodel::behavior::BaseMechanicalState *>(*obj_it))
        {
            core::componentmodel::behavior::BaseMechanicalState *mechanical = dynamic_cast< core::componentmodel::behavior::BaseMechanicalState *>(*obj_it);
            mechanical->applyTranslation(dx, dy, dz);
            mechanical->applyScale(scale);
// 		mechanical_object = true;
        }

        if (dynamic_cast< sofa::component::visualmodel::VisualModelImpl* >(*obj_it))
        {
            sofa::component::visualmodel::VisualModelImpl *visual = dynamic_cast< sofa::component::visualmodel::VisualModelImpl* >(*obj_it);
            visual->applyTranslation(dx, dy, dz);
            visual->applyScale(scale);
        }
        obj_it++;
    }

    //We don't need to go any further:
// 	if (mechanical_object && !mesh_topology)
// 	    return;

    //We search recursively with the childs of the currend node
    GNode::ChildIterator it  = node->child.begin();
    GNode::ChildIterator end = node->child.end();
    while (it != end)
    {
        transformObject( *it, dx, dy, dz, scale);
        it++;
    }
}

/*****************************************************************************************************************/
void RealGUI::loadObject(std::string path, double dx, double dy, double dz, double scale)
{

    //Loading of the xml file
    xml::BaseElement* xml = xml::load(path.c_str());
    if (xml == NULL) return;


    helper::system::SetDirectory chdir( path.c_str());

    std::cout << "Initializing objects"<<std::endl;
    if (!xml->init())
    {
        std::cerr << "Objects initialization failed."<<std::endl;
    }

    GNode* new_node = dynamic_cast<GNode*>(xml->getObject());
    if (new_node == NULL)
    {
        std::cerr << "Objects initialization failed."<<std::endl;
        delete xml;
        return ;
    }

    std::cout << "Initializing simulation "<<new_node->getName()<<std::endl;
    new_node->execute<InitAction>();

    if (node_clicked->child.begin() ==  node_clicked->child.end() &&  node_clicked->object.begin() == node_clicked->object.end())
    {
        //Temporary Root : the current graph is empty, and has only a single node "Root"
        viewer->setScene(new_node, path.c_str());
        graphListener->removeChild(NULL, node_clicked);
        graphListener->addChild(NULL, new_node);
    }
    else
    {
        node_clicked->addChild( new_node);
        graphListener->addObject(node_clicked, (core::objectmodel::BaseObject*)new_node);
    }

    //Apply the Transformation
    transformObject( new_node, dx, dy, dz, scale);

    viewer->SwitchToPresetView();
    viewer->getQWidget()->update();

    node_clicked = NULL;
    item_clicked = NULL;
}

void RealGUI::loadObject()
{
    std::string position[3];
    std::string scale;
#ifdef QT_MODULE_QT3SUPPORT
    std::string object_fileName(dialog->openFilePath->text().toStdString());
    position[0] = dialog->positionX->text().toStdString();
    position[1] = dialog->positionY->text().toStdString();
    position[2] = dialog->positionZ->text().toStdString();
    scale       = dialog->scaleValue->text().toStdString();
#else
    std::string object_fileName(dialog->openFilePath->text().latin1());
    position[0] = dialog->positionX->text().latin1();
    position[1] = dialog->positionY->text().latin1();
    position[2] = dialog->positionZ->text().latin1();
    scale       = dialog->scaleValue->text().latin1();
#endif

    loadObject(object_fileName, atof(position[0].c_str()),atof(position[1].c_str()),atof(position[2].c_str()),atof(scale.c_str()));

}

/*****************************************************************************************************************/
//Visibility Option in grah : expand or collapse a node : easier to get access to a node, and see its properties properly
void RealGUI::graphCollapse()
{
    if (item_clicked != NULL)
    {
        QListViewItem* child;
        child = item_clicked->firstChild();
        while (child != NULL)
        {
            child->setOpen(false);
            child = child->nextSibling();
        }
        item_clicked->setOpen(true);
    }
}

void RealGUI::graphExpand()
{
    item_clicked->setOpen(true);
    Q3ListViewItem *item_clicked_back = item_clicked;
    if (item_clicked != NULL)
    {
        QListViewItem* child;
        child = item_clicked->firstChild();
        while (child != NULL)
        {
            item_clicked = child;

            child->setOpen(true);
            graphExpand();
            child = child->nextSibling();
        }
    }
    item_clicked = item_clicked_back;

}
/*****************************************************************************************************************/
void RealGUI::modifyUnlock()
{
    modifyDialogOpened --;
}

} // namespace qt

} // namespace gui

} // namespace sofa
