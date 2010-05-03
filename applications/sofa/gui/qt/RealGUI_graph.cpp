/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 4      *
*                (c) 2006-2009 MGH, INRIA, USTL, UJF, CNRS                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU General Public License as published by the Free  *
* Software Foundation; either version 2 of the License, or (at your option)   *
* any later version.                                                          *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
* more details.                                                               *
*                                                                             *
* You should have received a copy of the GNU General Public License along     *
* with this program; if not, write to the Free Software Foundation, Inc., 51  *
* Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.                   *
*******************************************************************************
*                            SOFA :: Applications                             *
*                                                                             *
* Authors: M. Adam, J. Allard, B. Andre, P-J. Bensoussan, S. Cotin, C. Duriez,*
* H. Delingette, F. Falipou, F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza,  *
* M. Nesme, P. Neumann, J-P. de la Plata Alcade, F. Poyer and F. Roy          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <sofa/gui/qt/RealGUI.h>


#ifdef SOFA_QT4
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
#include <QFileDialog>
#include <Q3FileDialog>
#include <QTabWidget>
#include <Q3PopupMenu>
#include <QToolTip>
#include <QButtonGroup>
#include <QRadioButton>
#include <QInputDialog>
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
#include <qtabwidget.h>
#include <qpopupmenu.h>
#include <qtooltip.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qinputdialog.h>
#endif


namespace sofa
{

  namespace gui
  {

    namespace qt
    {



#ifdef SOFA_QT4
      typedef Q3ListView     QListView;
      typedef Q3ListViewItem QListViewItem;
#endif

      using sofa::core::objectmodel::BaseContext;


      void RealGUI::addInitialNodes( Node* node)
      {
        //Create the list of the object present at the beginning of the scene
        for (Node::ChildIterator it = node->child.begin(); it!=node->child.end();it++)
          {
            Node *childNode = *it;
            list_object_initial.push_back( std::make_pair( node, childNode ) );
            addInitialNodes(childNode);
          }          
      }
      
      void RealGUI::clearGraph()
      {
        Node *rootNode = viewer->getScene();

	graphView->setSorting ( -1 ); visualGraphView->setSorting( -1);
	//graphView->setTreeStepSize(10);
	graphView->header()->hide();  visualGraphView->header()->hide();
	//dumpGraph(groot, new Q3ListViewItem(graphView));

        graphListener->addChild ( NULL, rootNode );
        visualGraphListener->addChild ( NULL, simulation::getSimulation()->getVisualRoot() );
	
        //Create Stats about the simulation
        graphCreateStats(rootNode);
        
        addInitialNodes(rootNode);
        
        if ( currentTab != TabGraph )
          {
            graphListener->freeze ( rootNode );
          }
        if ( currentTab != TabVisualGraph )
          {
            visualGraphListener->freeze ( simulation::getSimulation()->getVisualRoot() );
          }
        
	simulation::Simulation *s = simulation::getSimulation();

        //In case instruments are present in the scene, we create a new tab, and display the listr
        if (s->instruments.size() != 0)
	{
	  tabInstrument = new QWidget();
	  tabs->addTab(tabInstrument, QString("Instrument"));

	  QVBoxLayout *layout = new QVBoxLayout( tabInstrument, 0, 1, "tabInstrument");

	  QButtonGroup *list_instrument = new QButtonGroup(tabInstrument);
	  list_instrument->setExclusive(true);

#ifdef SOFA_QT4
	  connect ( list_instrument, SIGNAL ( buttonClicked(int) ), this, SLOT ( changeInstrument(int) ) );
#else
	  connect ( list_instrument, SIGNAL ( clicked(int) ), this, SLOT ( changeInstrument(int) ) );
#endif

	  QRadioButton *button = new QRadioButton(tabInstrument);button->setText("None");
#ifdef SOFA_QT4
	  list_instrument->addButton(button, 0);
#else
	  list_instrument->insert(button);
#endif
	  layout->addWidget(button);

	  for (unsigned int i=0;i<s->instruments.size();i++)
	  {
	    QRadioButton *button = new QRadioButton(tabInstrument);  button->setText(QString( s->instruments[i]->getName().c_str() ) );
#ifdef SOFA_QT4
	    list_instrument->addButton(button, i+1);
#else
	    list_instrument->insert(button);
#endif
	    layout->addWidget(button);
	    if (i==0)
	    {
	      button->setChecked(true); changeInstrument(1);
	    }
	    else
	      s->instruments[i]->setActive(false);

	  }
#ifdef SOFA_QT4
	  layout->addStretch(1);
#endif
#ifndef SOFA_QT4
	  layout->addWidget(list_instrument);
#endif
	}
      }


      /*****************************************************************************************************************/
      //Visibility Option in grah : expand or collapse a node : easier to get access to a node, and see its properties properly
      void RealGUI::graphCollapse()
      {
	bool isAnimated = startButton->isOn ();

	playpauseGUI ( false );
	if ( item_clicked != NULL )
	{
	  QListViewItem* child;
	  child = item_clicked->firstChild();
	  while ( child != NULL )
	  {
	    child->setOpen ( false );
	    child = child->nextSibling();
	  }
	  item_clicked->setOpen ( true );
	}

	playpauseGUI ( isAnimated );
      }

      void RealGUI::graphExpand()
      {
	bool isAnimated = startButton->isOn();
	playpauseGUI ( false );
	item_clicked->setOpen ( true );
	QListViewItem *item_clicked_back = item_clicked;
	if ( item_clicked != NULL )
	{
	  QListViewItem* child;
	  child = item_clicked->firstChild();
	  while ( child != NULL )
	  {
	    item_clicked = child;

	    child->setOpen ( true );
	    graphExpand();
	    child = child->nextSibling();
	  }
	}
	item_clicked = item_clicked_back;
	playpauseGUI ( isAnimated );
      }
      /*****************************************************************************************************************/
      void RealGUI::modifyUnlock ( void *Id )
      {
	graphCreateStats(viewer->getScene());

	map_modifyDialogOpened.erase( Id );
	map_modifyObjectWindow.erase( Id );
      }

      /*****************************************************************************************************************/
      // Fill the listview in the stats tab with information about the number of points/line/triangle/sphere of the collision models present in the scene

      //Add the current node and its child to the stats graph.
      bool RealGUI::graphCreateStats( Node *node)
      {
	sofa::helper::vector< sofa::core::CollisionModel* > list_collisionModels;
	node->get< sofa::core::CollisionModel >( &list_collisionModels, BaseContext::SearchDown);

	if (items_stats.size() != 0)
	{
	  delete items_stats[0].second;
	  items_stats.clear();
	}
	GUI::StatsCounter->clear();

	graphAddCollisionModelsStat(list_collisionModels);

	graphSummary();
	return true;
      }

      //Add a list of Collision model to the graph
      void RealGUI::graphAddCollisionModelsStat(sofa::helper::vector< sofa::core::CollisionModel* > &v)
      {
	std::map< BaseContext*, QListViewItem* > listStats;
	for (unsigned int i=0;i<v.size();i++)
	{
	  if ( !v[i]->isActive()) continue;
	  std::map< BaseContext*, QListViewItem* >::iterator it = listStats.find(v[i]->getContext());
	  QListViewItem *item;
	  if (it != listStats.end())
	  {
	   item = new QListViewItem((*it).second);
	  }
	  else
	  {
	    QListViewItem *node = new QListViewItem(GUI::StatsCounter);
	    node->setText(0,QString(v[i]->getContext()->getName().c_str()));
            QPixmap* pix = sofa::gui::qt::getPixmap(v[i]->getContext());
	    if (pix) node->setPixmap(0,*pix);
	    listStats.insert(std::make_pair(v[i]->getContext(), node));
	    item = new QListViewItem(node);
	    node->setOpen(true);
	  }
	  item->setText(0,v[i]->getName().c_str());
	  item->setText(1,QString(v[i]->getClassName().c_str()));
	  item->setText(0,v[i]->getName().c_str());
	  item->setText(2,QString::number(v[i]->getSize()));
	  items_stats.push_back(std::make_pair(v[i], item));
	}
      }


      //create global stats
      void RealGUI::graphSummary()
      {
	std::set< std::string > nameElement;
	std::map< std::string, int > mapElement;
	for (unsigned int i=0; i < items_stats.size();i++)
	    nameElement.insert(items_stats[i].first->getClassName());


	for (unsigned int i=0; i < items_stats.size();i++)
	    mapElement[items_stats[i].first->getClassName()] += atoi(items_stats[i].second->text(2));


	std::string textStats("<hr>Collision Elements present: <ul>");
	std::map< std::string, int >::const_iterator it;

	for (it=mapElement.begin();it!=mapElement.end();it++)
	  {
	    if (it->second)
	      {
		char buf[100];
		sprintf ( buf, "<li><b>%s:</b> %d</li>", it->first.c_str(), it->second );
		textStats += buf;
	      }
	  }

	textStats += "</ul><br>";
	statsLabel->setText( textStats.c_str());
	statsLabel->update();
      }


      /*****************************************************************************************************************/
      // INTERACTION WITH THE GRAPH
      /*****************************************************************************************************************/

      /*****************************************************************************************************************/
      void RealGUI::DoubleClickeItemInSceneView ( QListViewItem *item )
      {
	// This happens because the clicked() signal also calls the select callback with
	// NULL as a parameter.
	if ( item == NULL )
	  return;

	item_clicked = item;

	// cancel the visibility action caused by the double click
	item_clicked->setOpen ( !item_clicked->isOpen() );
	graphModify();
      }


      /*****************************************************************************************************************/
      void RealGUI::RightClickedItemInSceneView ( QListViewItem *item, const QPoint& point, int index )
      {
	if ( dialog == NULL )
	{
	    //Creation of the file dialog
	  dialog = new AddObject ( &list_object, this );
	  dialog->setPath ( viewer->getSceneFileName() );
	  dialog->hide();
	}


	//Creation of a popup menu at the mouse position
	item_clicked=item;

	//Search in the graph if the element clicked is a node
	node_clicked = NULL;
	if ( item_clicked == NULL ) return;


        GraphListenerQListView *listener=NULL;
        QPopupMenu *contextMenu=NULL;
        if (currentTab == TabGraph)
        {
            listener = graphListener;
             contextMenu= new QPopupMenu ( graphView, "ContextMenu" );
        }
        else if (currentTab == TabVisualGraph)
        {
            listener = visualGraphListener;
            contextMenu = new QPopupMenu ( visualGraphView, "ContextMenu" );
        }
	std::map<core::objectmodel::Base*, QListViewItem* >::iterator graph_iterator;

        for (graph_iterator = listener->items.begin(); graph_iterator != listener->items.end(); graph_iterator++)
	{
          if ( (*graph_iterator).second == item) {node_clicked = dynamic_cast< Node* >( (*graph_iterator).first); break;}
	}


	//Creation of the context Menu
	if ( node_clicked != NULL )
	{
	  contextMenu->insertItem ( "Collapse", this, SLOT ( graphCollapse() ) );
	  contextMenu->insertItem ( "Expand", this, SLOT ( graphExpand() ) );
	  contextMenu->insertSeparator ();
	  /*****************************************************************************************************************/
	  if (node_clicked->isActive())
	    contextMenu->insertItem ( "Desactivate", this, SLOT ( graphDesactivateNode() ) );
	  else
	    contextMenu->insertItem ( "Activate", this, SLOT ( graphActivateNode() ) );
	  contextMenu->insertSeparator ();
	  /*****************************************************************************************************************/

	  contextMenu->insertItem ( "Save Node", this, SLOT ( graphSaveObject() ) );
          if (currentTab == TabGraph)
          {
              contextMenu->insertItem ( "Add Node", this, SLOT ( graphAddObject() ) );
              int index_menu = contextMenu->insertItem ( "Remove Node", this, SLOT ( graphRemoveObject() ) );
              //If one of the elements or child of the current node is beeing modified, you cannot allow the user to erase the node
              if ( !isErasable ( node_clicked ) )
                contextMenu->setItemEnabled ( index_menu,false );
          }


	}
	contextMenu->insertItem ( "Modify", this, SLOT ( graphModify() ) );
	contextMenu->popup ( point, index );

      }


      /*****************************************************************************************************************/
      void RealGUI::graphSaveObject()
      {
	bool isAnimated = startButton->isOn();

	playpauseGUI ( false );
	//Just pop up the dialog window
	if ( node_clicked != NULL )
	{
	  fileSaveAs(node_clicked);
	  item_clicked = NULL;
	}
	playpauseGUI ( isAnimated );
      }
      /*****************************************************************************************************************/
      void RealGUI::graphAddObject()
      {

	bool isAnimated = startButton->isOn();

	playpauseGUI ( false );
	//Just pop up the dialog window
	if ( node_clicked != NULL )
	{
	  dialog->show();
	  dialog->raise();

	  item_clicked = NULL;
	}
	playpauseGUI ( isAnimated );
      }

      /*****************************************************************************************************************/
      void RealGUI::graphRemoveObject()
      {
	bool isAnimated = startButton->isOn();

	playpauseGUI ( false );
        if ( node_clicked != NULL )
	{
          if ( node_clicked == simulation::getSimulation()->getContext() )
	  {
		//Attempt to destroy the Root node : create an empty node to handle new graph interaction
            Node *root = simulation::getSimulation()->newNode( "Root" );

	    root->setShowVisualModels ( 1 );
	    root->setShowCollisionModels ( 0 );
	    root->setShowBoundingCollisionModels ( 0 );
	    root->setShowBehaviorModels ( 0 );
	    root->setShowMappings ( 0 );
	    root->setShowMechanicalMappings ( 0 );
	    root->setShowForceFields ( 0 );
	    root->setShowInteractionForceFields ( 0 );
	    root->setShowWireFrame ( 0 );
	    root->setShowNormals ( 0 );


	    viewer->setScene ( root, viewer->getSceneFileName().c_str() );
            graphListener->removeChild ( NULL, node_clicked );
	    graphListener->addChild ( NULL, root );
	  }
	  else
	  {
            node_clicked->detachFromGraph();
            graphListener->removeChild ( NULL, node_clicked );
            list_object_removed.push_back ( node_clicked );
	  }

	  viewer->resetView();
	  viewer->getQWidget()->update();
          node_clicked = NULL;
	  item_clicked = NULL;
	}
	playpauseGUI ( isAnimated );

	graphCreateStats(viewer->getScene());
      }

      /*****************************************************************************************************************/
      void RealGUI::graphModify()
      {

	bool isAnimated = startButton->isOn();

        GraphListenerQListView* listener=NULL;
        
        if (currentTab == TabGraph) listener = graphListener;
        else if (currentTab == TabVisualGraph) listener = visualGraphListener;

	playpauseGUI ( false );
	if ( item_clicked != NULL )
	{
	  core::objectmodel::Base* node=NULL;
	  for ( std::map<core::objectmodel::Base*, QListViewItem* >::iterator it = listener->items.begin() ; it != listener->items.end() ; ++ it )
	  {
	    if ( ( *it ).second == item_clicked )
	    {
	      node = ( *it ).first;
	      break;
	    }
	  }

	    //Opening of a dialog window automatically created
	  current_Id_modifyDialog = node;
	  std::map< void*, QDialog* >::iterator testWindow =  map_modifyObjectWindow.find( current_Id_modifyDialog);
	  if ( testWindow != map_modifyObjectWindow.end())
	  {
	     //Object already being modified: no need to open a new window
	    (*testWindow).second->raise();
	    playpauseGUI ( isAnimated );
	    return;
	  }


	  ModifyObject *dialogModify = new ModifyObject ( current_Id_modifyDialog, node, item_clicked,this,item_clicked->text(0));

	  map_modifyObjectWindow.insert( std::make_pair(current_Id_modifyDialog, dialogModify));

	    //If the item clicked is a node, we add it to the list of the element modified
	  if ( dynamic_cast<Node *> ( node ) )
	    map_modifyDialogOpened.insert ( std::make_pair ( current_Id_modifyDialog, node ) );
	  else
	  {
		//If the item clicked is just an element of the node, we add the node containing it
	    for ( std::map<core::objectmodel::Base*, QListViewItem* >::iterator it = listener->items.begin() ; it != listener->items.end() ; ++ it )
	    {
	      if ( ( *it ).second == item_clicked->parent() )
	      {
		map_modifyDialogOpened.insert ( std::make_pair ( current_Id_modifyDialog, ( *it ).first ) );
		break;
	      }
	    }
	  }

	  dialogModify->show();
	  dialogModify->raise();

	  connect ( this, SIGNAL ( newScene() ), dialogModify, SLOT ( closeNow() ) );
	  connect ( this, SIGNAL ( newStep() ),  dialogModify, SLOT ( updateTables() ) );
	  item_clicked = NULL;
	}
	playpauseGUI ( isAnimated );
      }


      /*****************************************************************************************************************/
      void RealGUI::graphDesactivateNode()
      {
        item_clicked->setText(0, QString("Deactivated ") + item_clicked->text(0));
	item_clicked->setOpen(false);

	std::string pixmap_filename("textures/media-record.png");
	if ( sofa::helper::system::DataRepository.findFile ( pixmap_filename ) )
	  pixmap_filename = sofa::helper::system::DataRepository.getFile ( pixmap_filename );

	item_clicked->setPixmap(0,QPixmap(QImage(pixmap_filename.c_str())));
	node_clicked->setActive(false);
	viewer->getQWidget()->update();
	node_clicked->reinit();

	graphCreateStats(viewer->getScene());
      }

      void RealGUI::graphActivateNode()
      {
          GraphListenerQListView *listener=NULL;
          if (currentTab == TabGraph) listener=graphListener;
          else if (currentTab == TabVisualGraph) listener=visualGraphListener;

          using core::objectmodel::BaseNode;
          std::list< BaseNode* > nodeToProcess;
          nodeToProcess.push_front((BaseNode*)node_clicked);

          //We must activate the nodes from the leaves to the root
          std::list< Node* > nodeToActivate;

          //Breadth First approach to activate all the nodes
          while (!nodeToProcess.empty())
          {
              //We take the first element of the list
              BaseNode* n=nodeToProcess.front();
              nodeToProcess.pop_front();

              nodeToActivate.push_front((Node*) n);

              //Find the corresponding node in the Qt Graph
              Q3ListViewItem *item=listener->items[n];
              //Remove the text
              QString desact_text = item->text(0);
              desact_text.remove(QString("Deactivated "), true);
              item->setText(0,desact_text);
              //Remove the icon
              QPixmap *p = getPixmap(n);
              item->setPixmap(0,*p);

              //We add to the list of node to process all its children
              core::objectmodel::BaseNode::Children children=n->getChildren();
              std::copy(children.begin(), children.end(), std::back_inserter(nodeToProcess));
          }

          for (std::list<Node*>::iterator it=nodeToActivate.begin();it!=nodeToActivate.end();++it)
              (*it)->setActive(true);


          viewer->getQWidget()->update();
          node_clicked->init();

          graphCreateStats(viewer->getScene());
      }



      void RealGUI::currentTabChanged ( QWidget* widget )
      {
	if ( widget == currentTab ) return;
        Node* root = viewer==NULL ? NULL : viewer->getScene();

	if ( widget == TabGraph )
	{
	  if ( root && graphListener )
	  {
		//graphListener->addChild(NULL, root);
	    graphListener->unfreeze ( root );
	  }
	}
	else if ( currentTab == TabGraph )
	{
	  if ( root && graphListener )
	  {
		//graphListener->removeChild(NULL, root);
	    graphListener->freeze ( root );
	  }
	}
	else if ( widget == TabVisualGraph )
	{
	  if (  simulation::getSimulation()->getVisualRoot() && visualGraphListener )
	  {
		//graphListener->addChild(NULL, root);
	    visualGraphListener->unfreeze ( simulation::getSimulation()->getVisualRoot() );
	  }
	}
	else if ( currentTab == TabVisualGraph )
	{
	  if (  simulation::getSimulation()->getVisualRoot() && visualGraphListener )
	  {
		//graphListener->removeChild(NULL, root);
	    visualGraphListener->freeze ( simulation::getSimulation()->getVisualRoot() );
	  }
	}
	else if (widget == TabStats)
	  graphCreateStats(viewer->getScene());

	currentTab = widget;
      }



      /*****************************************************************************************************************/
      // Test if a node can be erased in the graph : the condition is that none of its children has a menu modify opened
      bool RealGUI::isErasable ( core::objectmodel::Base* element )
      {
        GraphListenerQListView *listener=NULL;
        if (currentTab == TabGraph) listener = graphListener;
        else if (currentTab == TabVisualGraph) listener = visualGraphListener;

	std::map< void*, core::objectmodel::Base*>::iterator it;
	for (it = map_modifyDialogOpened.begin(); it != map_modifyDialogOpened.end();it++)
	{
	  if (it->second == element) return false;
	}

	std::map< core::objectmodel::Base*, QListViewItem*>::iterator it_item;
        it_item = listener->items.find(element);

	QListViewItem *child = it_item->second->firstChild();
	while (child != NULL)
	{
          for (it_item = listener->items.begin(); it_item != listener->items.end();it_item++)
	  {
	    if (it_item->second == child)
	    {
	     if (!isErasable(it_item->first)) return false;
	     break;
	    }
	  }
	  child = child->nextSibling();
	}
	return true;
      }



    } // namespace qt

  } // namespace gui

} // namespace sofa
