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

#include "h2sl/region.h"
#include "h2sl/constraint.h"
#include "h2sl/gui.h"

using namespace std;
using namespace h2sl;

double
diff_time( struct timeval& startTime,
            struct timeval& endTime ){
  if( endTime.tv_usec < startTime.tv_usec ){
    return ( ( double )( endTime.tv_sec - startTime.tv_sec - 1 ) + ( double )( 1000000 + endTime.tv_usec - startTime.tv_usec ) / 1000000.0 );
  } else {
    return ( ( double )( endTime.tv_sec - startTime.tv_sec ) + ( double )( endTime.tv_usec - startTime.tv_usec ) / 1000000.0 );
  }
}

QGraphicsItem_Label::
QGraphicsItem_Label( const string& label,
                      QGraphicsItem* parent ) : _label( label ) {

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
QGraphicsItem_Factor( Factor * factor,
                      QGraphicsItem* parent ) : _factor( factor ),
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
  if( _factor != NULL ){
    painter->setPen( QPen( Qt::white, 2.0 ) );
    painter->setFont( QFont( QApplication::font().defaultFamily(), 12, QFont::Bold ) );
//    painter->drawText( QRect( -18, -18, 36, 36 ), QString::fromStdString( _factor->name() ), QTextOption( Qt::AlignCenter ) );
  }
  return;
}

void
QGraphicsItem_Factor::
mousePressEvent( QGraphicsSceneMouseEvent * event ){
  if( _factor != NULL ){
    stringstream comment_string;
    comment_string << *_factor;
    emit comment( comment_string.str(), false );
  }
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
QGraphicsItem_Grounding( Grounding * grounding,
                         QGraphicsItem * parent ) : _grounding( grounding ),
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

  Phrase * phrase = dynamic_cast< Phrase* >( _grounding );
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
  if( dynamic_cast< Region* >( _grounding ) != NULL ){
    comment_string << *static_cast< Region* >( _grounding );
  } else if ( dynamic_cast< Constraint* >( _grounding ) != NULL ){
    comment_string << *static_cast< Constraint* >( _grounding );
  } else if ( dynamic_cast< Phrase* >( _grounding ) != NULL ){
    comment_string << *static_cast< Phrase* >( _grounding );
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

GUI::
GUI( Parser * parser,
      World * world,
      LLM * llm,
      DCG * dcg,
      const unsigned int& beamWidth,
      QWidget * parent ) : QWidget( parent ),
                          _line_edit_sentence( new QLineEdit( this ) ),
                          _list_widget_world( new QListWidget( this ) ),
                          _combo_box_solutions( new QComboBox( this ) ),
                          _graphics_scene_graph( new QGraphicsScene( this ) ),
                          _graphics_view_graph( new QGraphicsView( _graphics_scene_graph, this ) ),
                          _text_browser_comments (new QTextBrowser (this) ),
                          _group_box( NULL ),
                          _parser( parser ),
                          _world( world ),
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
  for( unsigned int i = 0; i < _world->objects().size(); i++ ){
    stringstream item_string;
    item_string << setw( 10 ) << setiosflags( ios::left | ios::fixed ) << _world->objects()[ i ]->name() << " ";
    item_string << setw( 10 ) << setiosflags( ios::left | ios::fixed ) << Object::type_to_std_string( _world->objects()[ i ]->type() ) << " ";
    item_string << setw( 10 ) << setiosflags( ios::left | ios::fixed ) << _world->objects()[ i ]->transform().position().to_std_string() << " ";
    item_string << setw( 10 ) << setiosflags( ios::left | ios::fixed ) << _world->objects()[ i ]->transform().orientation().to_std_string() << " ";
    _list_widget_world->addItem( QString::fromStdString( item_string.str() ) );
  }
  update();
  return;
}

void
GUI::
update_graph( void ){
  // clear the graph
  for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
    if( _graphics_items[ i ] != NULL ){
      if( dynamic_cast< QGraphicsItem_Grounding* >( _graphics_items[ i ] ) != NULL ){
        disconnect( dynamic_cast< QGraphicsItem_Grounding* >( _graphics_items[ i ] ), SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );
      } else if ( dynamic_cast< QGraphicsItem_Factor* >( _graphics_items[ i ] ) != NULL ){
        disconnect( dynamic_cast< QGraphicsItem_Factor* >( _graphics_items[ i ] ), SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );
      }
      _graphics_scene_graph->removeItem( _graphics_items[ i ] );
      delete _graphics_items[ i ];
      _graphics_items[ i ] = NULL;
    }
  }
  _graphics_scene_graph->clear();
  if( _graphics_scene_graph != NULL ){
    _graphics_view_graph->setScene( NULL );
    delete _graphics_scene_graph;
    _graphics_scene_graph = new QGraphicsScene();
    _graphics_view_graph->setScene( _graphics_scene_graph );
  }
  _graphics_items.clear();

  if( ( !_solutions.empty() ) && ( _combo_box_solutions->currentIndex() >= 0 ) ){
    _dcg->construct( _solutions[ _combo_box_solutions->currentIndex() ].second, _world, _llm, true ); 

    // add the phrases
    for( unsigned int i = 0; i < _dcg->phrases().size(); i++ ){
      // add the phrase
      QGraphicsItem_Grounding * qgraphicsitem_phrase = new QGraphicsItem_Grounding( _dcg->phrases()[ i ] );
      _graphics_items.push_back( qgraphicsitem_phrase );
      _graphics_items.back()->setPos( 128.0 * ( double )( i ), 64.0 );
      connect( qgraphicsitem_phrase, SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );
      _graphics_items.push_back( new QGraphicsItem_Label( _dcg->phrases()[ i ]->words_to_std_string() ) );
      _graphics_items.back()->setPos( 128.0 * ( double )( i ), 112.0 );

      // add the factors
      unsigned int num_phrase_factors = 0;
      for( unsigned int j = 0; j < _dcg->factors().size(); j++ ){
        if( ( _dcg->phrases()[ i ] == _dcg->factors()[ j ]->phrase() ) && ( _dcg->factors()[ j ]->cv() != CV_FALSE ) ){
          QGraphicsItem_Factor * qgraphicsitem_factor = new QGraphicsItem_Factor( _dcg->factors()[ j ] );
          _graphics_items.push_back( qgraphicsitem_factor );
          _graphics_items.back()->setPos( 128.0 * ( double )( i ), -128.0 * ( double )( num_phrase_factors ) );
          connect( qgraphicsitem_factor, SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );
          if( _dcg->factors()[ i ]->grounding() != NULL ){
            QGraphicsItem_Grounding * qgraphicsitem_grounding = new QGraphicsItem_Grounding( _dcg->factors()[ j ]->grounding() );
            _graphics_items.push_back( qgraphicsitem_grounding );
            _graphics_items.back()->setPos( 128.0 * ( double )( i ), -128.0 * ( double )( num_phrase_factors ) - 64.0 );
            connect( qgraphicsitem_grounding, SIGNAL( comment( const std::string&, const bool& ) ), this, SLOT( _receive_comment( const std::string&, const bool& ) ) );
          } 
          num_phrase_factors++;
        }
      }
    }

    // add the lines
    for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
      QGraphicsItem_Factor * qgraphicsitem_factor = dynamic_cast< QGraphicsItem_Factor* >( _graphics_items[ i ] );
      if( qgraphicsitem_factor != NULL ){
        for( unsigned int j = 0; j < _graphics_items.size(); j++ ){
          if( i != j ){
            QGraphicsItem_Grounding* qgraphicsitem_grounding = dynamic_cast< QGraphicsItem_Grounding* >( _graphics_items[ j ] );
            if( qgraphicsitem_grounding != NULL ){
              if( qgraphicsitem_factor->factor()->phrase() == qgraphicsitem_grounding->grounding() ){
                QGraphicsLineItem * qgraphicslineitem = new QGraphicsLineItem( qgraphicsitem_factor->x(), qgraphicsitem_factor->y(), qgraphicsitem_grounding->x(), qgraphicsitem_grounding->y() );
                qgraphicslineitem->setPen( QPen( Qt::black, 2.0 ) );
                _graphics_items.push_back( qgraphicslineitem );
                qgraphicsitem_factor->qgraphicslineitems().push_back( qgraphicslineitem );
                qgraphicsitem_grounding->qgraphicslineitems().push_back( qgraphicslineitem );
              }
              if( qgraphicsitem_factor->factor()->grounding() == qgraphicsitem_grounding->grounding() ){
                QGraphicsLineItem * qgraphicslineitem = new QGraphicsLineItem( qgraphicsitem_factor->x(), qgraphicsitem_factor->y(), qgraphicsitem_grounding->x(), qgraphicsitem_grounding->y() );
                qgraphicslineitem->setPen( QPen( Qt::black, 2.0 ) );
                _graphics_items.push_back( qgraphicslineitem );
                qgraphicsitem_factor->qgraphicslineitems().push_back( qgraphicslineitem );
                qgraphicsitem_grounding->qgraphicslineitems().push_back( qgraphicslineitem );
              }
              for( unsigned int k = 0; k < qgraphicsitem_factor->factor()->children().size(); k++ ){
                if( qgraphicsitem_factor->factor()->children()[ k ]->grounding() == qgraphicsitem_grounding->grounding() ){
                  QGraphicsLineItem * qgraphicslineitem = new QGraphicsLineItem( qgraphicsitem_factor->x(), qgraphicsitem_factor->y(), qgraphicsitem_grounding->x(), qgraphicsitem_grounding->y() );
                  qgraphicslineitem->setPen( QPen( Qt::black, 2.0 ) );
                  _graphics_items.push_back( qgraphicslineitem );
                  qgraphicsitem_factor->qgraphicslineitems().push_back( qgraphicslineitem ); 
                  qgraphicsitem_grounding->qgraphicslineitems().push_back( qgraphicslineitem );
                } 
              }
            }
          }
        }
      }
    } 

    for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
      if( dynamic_cast< QGraphicsLineItem* >( _graphics_items[ i ] ) != NULL ){
        _graphics_scene_graph->addItem( _graphics_items[ i ] );
      }
    }
    for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
      if( dynamic_cast< QGraphicsItem_Factor* >( _graphics_items[ i ] ) != NULL ){
        _graphics_scene_graph->addItem( _graphics_items[ i ] );
      }
    }
    for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
      if( dynamic_cast< QGraphicsItem_Label* >( _graphics_items[ i ] ) != NULL ){
        _graphics_scene_graph->addItem( _graphics_items[ i ] );
      }
    }
    for( unsigned int i = 0; i < _graphics_items.size(); i++ ){
      if( dynamic_cast< QGraphicsItem_Grounding* >( _graphics_items[ i ] ) != NULL ){
        _graphics_scene_graph->addItem( _graphics_items[ i ] );
      }
    }
  
    _graphics_view_graph->fitInView( _graphics_scene_graph->sceneRect(), Qt::KeepAspectRatio );

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
_run_inference( const string& sentence ){
  Phrase * phrase = new Phrase();
  if( _parser->parse( sentence, phrase ) ){
    struct timeval start_time;
    gettimeofday( &start_time, NULL );
    if( _dcg->search( phrase, _world, _llm, _solutions, _beam_width ) ){
      struct timeval end_time;
      gettimeofday( &end_time, NULL );
      stringstream comment_string;
      comment_string << "successfully inferred structured language for \"" << sentence << "\" in " << diff_time( start_time, end_time ) << " seconds";
      _text_browser_comments->append( _format_comment( comment_string.str(), false ) );
    } else {
      stringstream comment_string;
      comment_string << "failed to infer structured language for \"" << sentence << "\"";
      _text_browser_comments->append( _format_comment( comment_string.str(), true ) );
    }
  } else {
    stringstream comment_string;
    comment_string << "failed to parse \"" << sentence << "\"";
    _text_browser_comments->append( _format_comment( comment_string.str(), true ) );
  }
  _combo_box_solutions->clear();
  for( unsigned int i = 0; i < _solutions.size(); i++ ){
    stringstream solution_string;
    solution_string << "solution " << i + 1 << " (" << _solutions[ i ].first << ")";
    _combo_box_solutions->addItem( QString::fromStdString( solution_string.str() ) );
  } 
  cout << "finished search" << endl;
//  update_graph();
  if( phrase != NULL ){
    delete phrase;
    phrase = NULL;
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
