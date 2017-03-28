/**
 * @file    gui.cc
 * @author  Thomas M. Howard (tmhoward@csail.mit.edu)
 *          Matthew R. Walter (mwalter@csail.mit.edu)
 * @version 1.0
 *
 * @section LICENSE
 *
 * This file is part of h2sl.
 *
 * Copyright (C) 2014 by the Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/gpl-2.0.html> or write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 * @section DESCRIPTION
 *
 * The implementation of a class used for the graphical user interface for h2sl
 */

#include <iomanip>
#include <sstream>
#include <sys/time.h>

#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtCore/QTime>

#include "h2sl/common.h"
#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/gui.h"

using namespace std;
using namespace h2sl;

QGraphicsItem_Label::
QGraphicsItem_Label( const string& label,
                      QGraphicsItem* parent ) : QGraphicsItem(),
                                                _label( label ) {

}

QGraphicsItem_Label::
~QGraphicsItem_Label() {

}

QRectF
QGraphicsItem_Label::
boundingRect( void )const{
  return QRect( -32, -16, 64, 32 );
}

void
QGraphicsItem_Label::
paint( QPainter * painter,
        const QStyleOptionGraphicsItem* option,
        QWidget * widget ){
  painter->setPen( QPen( Qt::black, 2.0 ) );
  painter->setFont( QFont( QApplication::font().defaultFamily(), 12, QFont::Bold ) );
  painter->drawText( QRect( -32, -16, 64, 32 ), QString::fromStdString( _label ), QTextOption( Qt::AlignCenter ) );
  return;
}

QGraphicsItem_Factor::
QGraphicsItem_Factor( QGraphicsItem* parent ) : QObject(),
                                                QGraphicsItem(),
                                                _highlight( false ),
                                                _qgraphicslineitems(){
  setAcceptHoverEvents( true );
}

QGraphicsItem_Factor::
~QGraphicsItem_Factor() {

}

QRectF
QGraphicsItem_Factor::
boundingRect( void )const{
  return QRect( -24, -24, 48, 48 );
}

void
QGraphicsItem_Factor::
paint( QPainter * painter,
        const QStyleOptionGraphicsItem* option,
        QWidget * widget ){
  if( _highlight ){
    painter->setBrush( Qt::yellow );
    painter->setPen( Qt::NoPen );
    painter->drawRoundedRect( QRect( -24, -24, 48, 48 ), 4, 4 );
  }
  QRadialGradient gradient( -16, -16, 8 );
  gradient.setColorAt( 0, QColor( 64, 64, 64 ) );
  gradient.setColorAt( 1, QColor( 0, 0, 0 ) );
  painter->setBrush( gradient );
  painter->setPen( Qt::NoPen );
  painter->drawRoundedRect( QRect( -18, -18, 36, 36 ), 2, 2 );
/*
  if( _factor != NULL ){
    painter->setPen( QPen( Qt::white, 2.0 ) );
    painter->setFont( QFont( QApplication::font().defaultFamily(), 12, QFont::Bold ) );
//    painter->drawText( QRect( -18, -18, 36, 36 ), QString::fromStdString( _factor->id() ), QTextOption( Qt::AlignCenter ) );
  }
*/
  return;
}

void
QGraphicsItem_Factor::
mousePressEvent( QGraphicsSceneMouseEvent * event ){
/*
  if( _factor != NULL ){
    stringstream comment_string;
    comment_string << *_factor;
    emit comment( comment_string.str(), false );
  }
*/
  return;
}

void
QGraphicsItem_Factor::
hoverEnterEvent( QGraphicsSceneHoverEvent * event ){
  _highlight = true;
  for( unsigned int i = 0; i < _qgraphicslineitems.size(); i++ ){
    _qgraphicslineitems[ i ]->setPen( QPen( Qt::yellow, 5.0 ) );
  }
  update();
  return;
}

void
QGraphicsItem_Factor::
hoverLeaveEvent( QGraphicsSceneHoverEvent * event ){
  _highlight = false;
  for( unsigned int i = 0; i < _qgraphicslineitems.size(); i++ ){
    _qgraphicslineitems[ i ]->setPen( QPen( Qt::black, 2.0 ) );
  }
  update();
  return;
} 

QGraphicsItem_Grounding::
QGraphicsItem_Grounding( const Grounding * grounding,
                         QGraphicsItem * parent ) : QObject(),
                                                    QGraphicsItem(),
                                                    _grounding( grounding ),
                                                    _highlight( false ),
                                                    _qgraphicslineitems(){
  setAcceptHoverEvents( true );
}

QGraphicsItem_Grounding::
~QGraphicsItem_Grounding() {

}

QRectF
QGraphicsItem_Grounding::
boundingRect( void )const{
  return QRect( -32, -32, 64, 64 );
}

void
QGraphicsItem_Grounding::
paint( QPainter * painter,
        const QStyleOptionGraphicsItem* option,
        QWidget * widget ){
  if( _highlight ){
    painter->setBrush( Qt::yellow );
    painter->setPen( Qt::NoPen );
    painter->drawEllipse( -32, -32, 64, 64 );
  }

  QRadialGradient gradient( -8, -8, 8 );
  gradient.setColorAt( 0, QColor( 255, 255, 255 ) );
  gradient.setColorAt( 1, QColor( 224, 224, 224 ) );
  painter->setBrush( gradient );
  painter->setPen( QPen( Qt::black, 2.0 ) );
  painter->drawEllipse( -24, -24, 48, 48 );

  const Phrase * phrase = dynamic_cast< const Phrase* >( _grounding );
  if( phrase != NULL ){
    painter->setPen( QPen( Qt::black, 2.0 ) );
    painter->setFont( QFont( QApplication::font().defaultFamily(), 12, QFont::Bold ) );
    painter->drawText( QRect( -24, -24, 48, 48 ), QString::fromStdString( Phrase::phrase_type_t_to_std_string( phrase->type() ) ), QTextOption( Qt::AlignCenter ) );
  }

  return;
}

void
QGraphicsItem_Grounding::
mousePressEvent( QGraphicsSceneMouseEvent * event ){
  stringstream comment_string;
  if( dynamic_cast< const Region* >( _grounding ) != NULL ){
    comment_string << *static_cast< const Region* >( _grounding );
  } else if ( dynamic_cast< const Constraint* >( _grounding ) != NULL ){
    comment_string << *static_cast< const Constraint* >( _grounding );
  } else if ( dynamic_cast< const Object* >( _grounding ) != NULL ){
    comment_string << *static_cast< const Object* >( _grounding );
  }
  emit comment( comment_string.str(), false );
  return;
}

void
QGraphicsItem_Grounding::
hoverEnterEvent( QGraphicsSceneHoverEvent * event ){
  _highlight = true;
  for( unsigned int i = 0; i < _qgraphicslineitems.size(); i++ ){
    _qgraphicslineitems[ i ]->setPen( QPen( Qt::yellow, 5.0 ) );
  }
  update();
  return;
}

void
QGraphicsItem_Grounding::
hoverLeaveEvent( QGraphicsSceneHoverEvent * event ){
  _highlight = false;
  for( unsigned int i = 0; i < _qgraphicslineitems.size(); i++ ){
    _qgraphicslineitems[ i ]->setPen( QPen( Qt::black, 2.0 ) );
  }
  update();
  return;
}

QGraphicsItem_Phrase::
QGraphicsItem_Phrase( const Phrase * phrase,
                         QGraphicsItem * parent ) : QObject(),
                                                    QGraphicsItem(),
                                                    _phrase( phrase ),
                                                    _highlight( false ),
                                                    _qgraphicslineitems(){
  setAcceptHoverEvents( true );
}

QGraphicsItem_Phrase::
~QGraphicsItem_Phrase() {

}

QRectF
QGraphicsItem_Phrase::
boundingRect( void )const{
  return QRect( -32, -32, 64, 64 );
}

void
QGraphicsItem_Phrase::
paint( QPainter * painter,
        const QStyleOptionGraphicsItem* option,
        QWidget * widget ){
  if( _highlight ){
    painter->setBrush( Qt::yellow );
    painter->setPen( Qt::NoPen );
    painter->drawEllipse( -32, -32, 64, 64 );
  }

  QRadialGradient gradient( -8, -8, 8 );
  gradient.setColorAt( 0, QColor( 255, 255, 255 ) );
  gradient.setColorAt( 1, QColor( 224, 224, 224 ) );
  painter->setBrush( gradient );
  painter->setPen( QPen( Qt::black, 2.0 ) );
  painter->drawEllipse( -24, -24, 48, 48 );

  if( _phrase != NULL ){
    painter->setPen( QPen( Qt::black, 2.0 ) );
    painter->setFont( QFont( QApplication::font().defaultFamily(), 12, QFont::Bold ) );
    painter->drawText( QRect( -24, -24, 48, 48 ), QString::fromStdString( Phrase::phrase_type_t_to_std_string( _phrase->type() ) ), QTextOption( Qt::AlignCenter ) );
  }

  return;
}

void
QGraphicsItem_Phrase::
mousePressEvent( QGraphicsSceneMouseEvent * event ){
  stringstream comment_string;
  if( _phrase != NULL ){
    comment_string << *_phrase;
  }
  emit comment( comment_string.str(), false );
  return;
}

void
QGraphicsItem_Phrase::
hoverEnterEvent( QGraphicsSceneHoverEvent * event ){
  _highlight = true;
  for( unsigned int i = 0; i < _qgraphicslineitems.size(); i++ ){
    _qgraphicslineitems[ i ]->setPen( QPen( Qt::yellow, 5.0 ) );
  }
  update();
  return;
}

void
QGraphicsItem_Phrase::
hoverLeaveEvent( QGraphicsSceneHoverEvent * event ){
  _highlight = false;
  for( unsigned int i = 0; i < _qgraphicslineitems.size(); i++ ){
    _qgraphicslineitems[ i ]->setPen( QPen( Qt::black, 2.0 ) );
  }
  update();
  return;
}

GUI::
GUI( Grammar * grammar,
      Parser< Phrase > * parser,
      Symbol_Dictionary * symbolDictionary,
      Search_Space * searchSpace,
      World * world,
      Grounding * context,
      LLM * llm,
      DCG * dcg,
      const unsigned int& beamWidth,
      const string& command,
      QWidget * parent ) : QWidget( parent ),
                          _line_edit_sentence( new QLineEdit( QString::fromStdString( command ), this ) ),
                          _list_widget_world( new QListWidget( this ) ),
                          _combo_box_solutions( new QComboBox( this ) ),
                          _graphics_scene_graph( new QGraphicsScene( this ) ),
                          _graphics_view_graph( new QGraphicsView( _graphics_scene_graph, this ) ),
                          _text_browser_comments (new QTextBrowser (this) ),
                          _group_box( NULL ),
                          _grammar( grammar ),
                          _parser( parser ),
                          _symbol_dictionary( symbolDictionary ),
                          _search_space( searchSpace ),
                          _world( world ),
                          _context( context ),
                          _llm( llm ),
                          _dcg( dcg ),
                          _beam_width( beamWidth ),
                          _graphics_items() {

  QLabel * sentence_label = new QLabel( "sentence" );
  sentence_label->setAlignment( Qt::AlignCenter );

  QLabel * world_label = new QLabel( "world" );
  world_label->setAlignment( Qt::AlignCenter );

  QLabel * solutions_label = new QLabel( "solutions" );
  solutions_label->setAlignment( Qt::AlignCenter );

  QLabel * graphical_model_label = new QLabel( "graphical model" );
  graphical_model_label->setAlignment( Qt::AlignCenter );

  QLabel * comments_label = new QLabel( "comments" );
  comments_label->setAlignment( Qt::AlignCenter );

  _line_edit_sentence->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed );
  
  _list_widget_world->setFixedHeight( 75 );
  _list_widget_world->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed );
  QFont list_widget_world_font( QApplication::font().defaultFamily(), 12 );
  list_widget_world_font.setStyleHint( QFont::Monospace );
  _list_widget_world->setFont( list_widget_world_font ); 

  _graphics_view_graph->setMinimumSize( 400, 300 );
  _graphics_view_graph->setStyleSheet( "background:transparent" );
  _graphics_view_graph->setRenderHints( QPainter::Antialiasing | QPainter::HighQualityAntialiasing | QPainter::SmoothPixmapTransform );

  _text_browser_comments->setFixedHeight (75);
  _text_browser_comments->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Fixed );
   
  QVBoxLayout * layout_main = new QVBoxLayout();
  layout_main->addWidget( sentence_label );
  layout_main->addWidget( _line_edit_sentence );
  layout_main->addWidget( world_label );
  layout_main->addWidget( _list_widget_world );
  layout_main->addWidget( solutions_label );
  layout_main->addWidget( _combo_box_solutions );
  layout_main->addWidget( graphical_model_label );
  layout_main->addWidget( _graphics_view_graph );
  layout_main->addWidget( comments_label );
  layout_main->addWidget( _text_browser_comments );
  
  if( parent == NULL ){
    setLayout( layout_main );
  } else {
    _group_box = new QGroupBox( "gui" );
    _group_box->setLayout( layout_main );
    QVBoxLayout * layout_widget = new QVBoxLayout();
    layout_widget->addWidget( _group_box );
    setLayout( layout_widget );
  }

  connect( _line_edit_sentence, SIGNAL( returnPressed() ), this, SLOT( _line_edit_sentence_return_pressed() ) );
  connect( _combo_box_solutions, SIGNAL( currentIndexChanged( int ) ), this, SLOT( _combo_box_solutions_index_changed( int ) ) );

  update_world();
  update_graph();
}

GUI::
~GUI() {

}

void
GUI::
update_world( void ){

  _list_widget_world->clear();
  for( map< string, Object* >::const_iterator it_world_object = _world->objects().begin(); it_world_object != _world->objects().end(); it_world_object++ ){
    stringstream item_string;
    item_string << setw( 10 ) << setiosflags( ios::left | ios::fixed ) << it_world_object->second->id() << " ";
    item_string << setw( 10 ) << setiosflags( ios::left | ios::fixed ) << it_world_object->second->type() << " ";
    item_string << setw( 10 ) << setiosflags( ios::left | ios::fixed ) << it_world_object->second->transform().position().to_std_string() << " ";
    item_string << setw( 10 ) << setiosflags( ios::left | ios::fixed ) << it_world_object->second->transform().orientation().to_std_string() << " ";
    _list_widget_world->addItem( QString::fromStdString( item_string.str() ) );
  }
  update();
  return;
}

void
GUI::
update_graph( void ){

  for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
    for( unsigned int j = 0; j < _graphics_items[ i ].size(); j++ ){
      if( _graphics_items[ i ][ j ] != NULL ){
        if( dynamic_cast< QGraphicsItem_Grounding* >( _graphics_items[ i ][ j ] ) != NULL ){
          disconnect( dynamic_cast< QGraphicsItem_Grounding* >( _graphics_items[ i ][ j ] ), SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );
        } else if ( dynamic_cast< QGraphicsItem_Factor* >( _graphics_items[ i ][ j ] ) != NULL ){
          disconnect( dynamic_cast< QGraphicsItem_Factor* >( _graphics_items[ i ][ j ] ), SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );
        } else if ( dynamic_cast< QGraphicsItem_Phrase* >( _graphics_items[ i ][ j ] ) != NULL ){
          disconnect( dynamic_cast< QGraphicsItem_Phrase* >( _graphics_items[ i ][ j ] ), SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );
        } 
        _graphics_scene_graph->removeItem( _graphics_items[ i ][ j ] );
        delete _graphics_items[ i ][ j ];
        _graphics_items[ i ][ j ] = NULL;
      }
    }
    _graphics_items[ i ].clear();
  }
  _graphics_items.clear();

  if( _graphics_scene_graph != NULL ){
    _graphics_view_graph->setScene( NULL );
    delete _graphics_scene_graph;
    _graphics_scene_graph = new QGraphicsScene();
    _graphics_view_graph->setScene( _graphics_scene_graph );
  }


  if( _dcg != NULL ){
    if( _dcg->root() != NULL ){
      if( ( !_dcg->root()->solutions().empty() ) && ( _combo_box_solutions->currentIndex() >= 0 ) ){
        _add_factor_set_graphics_items( _dcg->root(), _combo_box_solutions->currentIndex() );
      }
    
      for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
        for( unsigned int j = 0; j < _graphics_items[ i ].size(); j++ ){
          if( dynamic_cast< QGraphicsLineItem* >( _graphics_items[ i ][ j ] ) != NULL ){
            _graphics_scene_graph->addItem( _graphics_items[ i ][ j ] );
          }
        }
      }

      for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
        for( unsigned int j = 0; j < _graphics_items[ i ].size(); j++ ){
          if( dynamic_cast< QGraphicsItem_Factor* >( _graphics_items[ i ][ j ] ) != NULL ){
            _graphics_scene_graph->addItem( _graphics_items[ i ][ j ] );
          }
        }
      }

      for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
        for( unsigned int j = 0; j < _graphics_items[ i ].size(); j++ ){
          if( dynamic_cast< QGraphicsItem_Label* >( _graphics_items[ i ][ j ] ) != NULL ){
            _graphics_scene_graph->addItem( _graphics_items[ i ][ j ] );
          }
        }
      }

      for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
        for( unsigned int j = 0; j < _graphics_items[ i ].size(); j++ ){
          if( dynamic_cast< QGraphicsItem_Grounding* >( _graphics_items[ i ][ j ] ) != NULL ){
            _graphics_scene_graph->addItem( _graphics_items[ i ][ j ] );
          }
        }
      }

      for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
        for( unsigned int j = 0; j < _graphics_items[ i ].size(); j++ ){
          if( dynamic_cast< QGraphicsItem_Phrase* >( _graphics_items[ i ][ j ] ) != NULL ){
            _graphics_scene_graph->addItem( _graphics_items[ i ][ j ] );
          }
        }
      }

      _graphics_view_graph->fitInView( _graphics_scene_graph->sceneRect(), Qt::KeepAspectRatio );
    }
  }

  return;
}

void
GUI::
resizeEvent( QResizeEvent* event ){
  _graphics_view_graph->fitInView( _graphics_scene_graph->sceneRect(), Qt::KeepAspectRatio );
  return;
}

void
GUI::
_receive_comment( const string& comment, 
                  const bool& error) {
  _text_browser_comments->append( _format_comment( comment, error ) );
  return;
}

void
GUI::
_line_edit_sentence_return_pressed( void ){
  _run_inference( _line_edit_sentence->text().toStdString() );
  _send_output();
  return;
}

void
GUI::
_combo_box_solutions_index_changed( int index ){
  update_graph();
  return;
} 

void
GUI::
_add_factor_set_graphics_items( const Factor_Set* factorSet,
                                  const unsigned int& solutionIndex,
                                  const vector< QGraphicsItem_Factor* >& parents ){

  _graphics_items.push_back( vector< QGraphicsItem* >() );

  vector< QGraphicsItem_Factor* > factors;

  // add the phrase
  QGraphicsItem_Phrase * qgraphicsitem_phrase = new QGraphicsItem_Phrase( factorSet->phrase() );
  _graphics_items.back().push_back( qgraphicsitem_phrase );
  _graphics_items.back().back()->setPos( 128.0 * ( double )( _graphics_items.size() - 1 ), 0.0 );
  connect( qgraphicsitem_phrase, SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );

  // add the phrase label
  _graphics_items.back().push_back( new QGraphicsItem_Label( factorSet->phrase()->words_to_std_string() ) );
  _graphics_items.back().back()->setPos( 128.0 * ( double )( _graphics_items.size() - 1 ), 40.0 );

  // add the groundings
  for( unsigned int i = 0; i < factorSet->solutions()[ solutionIndex ].grounding_set()->groundings().size(); i++ ){
    QGraphicsItem_Factor * qgraphicsitem_factor = new QGraphicsItem_Factor();
    _graphics_items.back().push_back( qgraphicsitem_factor );
    _graphics_items.back().back()->setPos( 128.0 * ( double )( _graphics_items.size() - 1 ), -64.0 - 128.0 * ( double )( i ) );
    connect( qgraphicsitem_factor, SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );
    factors.push_back( qgraphicsitem_factor );

    QGraphicsItem_Grounding * qgraphicsitem_grounding = new QGraphicsItem_Grounding( factorSet->solutions()[ solutionIndex ].grounding_set()->groundings()[ i ] );
    _graphics_items.back().push_back( qgraphicsitem_grounding );
    _graphics_items.back().back()->setPos( 128.0 * ( double )( _graphics_items.size() - 1 ), -128.0 - 128.0 * ( double )( i ) );
    connect( qgraphicsitem_grounding, SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );

    QGraphicsLineItem * qgraphicslineitem_factor_to_phrase = new QGraphicsLineItem( qgraphicsitem_factor->x(), qgraphicsitem_factor->y(), qgraphicsitem_phrase->x(), qgraphicsitem_phrase->y() );
    qgraphicslineitem_factor_to_phrase->setPen( QPen( Qt::black, 2.0 ) );
    qgraphicsitem_factor->qgraphicslineitems().push_back( qgraphicslineitem_factor_to_phrase );
    qgraphicsitem_phrase->qgraphicslineitems().push_back( qgraphicslineitem_factor_to_phrase );
    _graphics_items.back().push_back( qgraphicslineitem_factor_to_phrase );

    QGraphicsLineItem * qgraphicslineitem_factor_to_grounding = new QGraphicsLineItem( qgraphicsitem_factor->x(), qgraphicsitem_factor->y(), qgraphicsitem_grounding->x(), qgraphicsitem_grounding->y() );
    qgraphicslineitem_factor_to_grounding->setPen( QPen( Qt::black, 2.0 ) );
    qgraphicsitem_factor->qgraphicslineitems().push_back( qgraphicslineitem_factor_to_grounding );
    qgraphicsitem_grounding->qgraphicslineitems().push_back( qgraphicslineitem_factor_to_grounding );
    _graphics_items.back().push_back( qgraphicslineitem_factor_to_grounding );

    for( unsigned int j = 0; j < parents.size(); j++ ){
      QGraphicsLineItem * qgraphicslineitem_grounding_to_parent = new QGraphicsLineItem( qgraphicsitem_grounding->x(), qgraphicsitem_grounding->y(), parents[ j ]->x(), parents[ j ]->y() );
      qgraphicslineitem_grounding_to_parent->setPen( QPen( Qt::black, 2.0 ) );
      qgraphicsitem_factor->qgraphicslineitems().push_back( qgraphicslineitem_grounding_to_parent );
      qgraphicsitem_grounding->qgraphicslineitems().push_back( qgraphicslineitem_grounding_to_parent );
      _graphics_items.back().push_back( qgraphicslineitem_grounding_to_parent );
    } 

/*
    QGraphicsLineItem * qgraphicslineitem = new QGraphicsLineItem( qgraphicsitem_factor->x(), qgraphicsitem_factor->y(), qgraphicsitem_grounding->x(), qgraphicsitem_grounding->y() );
    qgraphicslineitem->setPen( QPen( Qt::black, 2.0 ) );
    _graphics_items.push_back( qgraphicslineitem );
*/
  }

  for( unsigned int i = 0; i < factorSet->child_factor_sets().size(); i++ ){
    _add_factor_set_graphics_items( factorSet->child_factor_sets()[ i ], factorSet->solutions()[ solutionIndex ].child_solution_indices()[ i ], factors );
  }

  return;
}

void
GUI::
_run_inference( const string& sentence ){

  vector< Phrase* > phrases;
  if( _parser->parse( *_grammar, sentence, phrases ) ){
    stringstream comment_string;
    comment_string << "generated " << phrases.size() << " parse trees";
    _text_browser_comments->append( _format_comment( comment_string.str(), false ) );
    if( !phrases.empty() ){
      if( phrases.front() != NULL ){
        struct timeval start_time;
        gettimeofday( &start_time, NULL );
        if( _dcg->leaf_search( phrases.front(), *_symbol_dictionary, _search_space, _world, _context, _llm, _beam_width ) ){
          struct timeval end_time;
          gettimeofday( &end_time, NULL );
          comment_string.str( string() );
          comment_string << "successfully inferred structured language for \"" << sentence << "\" in " << diff_time( start_time, end_time ) << " seconds";
          _text_browser_comments->append( _format_comment( comment_string.str(), false ) );
        } else {
          comment_string.str( string() );
          comment_string << "failed to infer structured language for \"" << sentence << "\"";
          _text_browser_comments->append( _format_comment( comment_string.str(), true ) );
        }
      } else {
        comment_string.str( string() );
        comment_string << "failed to parse \"" << sentence << "\"";
        _text_browser_comments->append( _format_comment( comment_string.str(), true ) );
      }
      _combo_box_solutions->clear();
      if( _dcg->root() != NULL ){
        for( unsigned int i = 0; i < _dcg->root()->solutions().size(); i++ ){
          stringstream solution_string;
          solution_string << "solution " << i + 1 << " (" << _dcg->root()->solutions()[ i ].pygx() << ")";
          _combo_box_solutions->addItem( QString::fromStdString( solution_string.str() ) );
        } 
      }
    }
  }
  for( unsigned int i = 0; i < phrases.size(); i++ ){
    if( phrases[ i ] != NULL ){
      delete phrases[ i ];
      phrases[ i ] = NULL;
    }
  }
  return;
}

void
GUI::
_send_output( void ){
  return;
}

QString
GUI::
_format_comment(const string &comment, const bool &error ) {
  QString formatted_comment;
  if (error){
    formatted_comment.append( "<font color=\"red\">" );
  }
  formatted_comment.append( "(" );
  formatted_comment.append( QTime::currentTime().toString( Qt::TextDate ) );
  formatted_comment.append( ") " );
  formatted_comment.append( QString::fromStdString( comment ) );
  if( error ){
    formatted_comment.append( "</font>" );
  }
  return formatted_comment;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const GUI& other ) {
    return out;
  }

}
