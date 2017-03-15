/**
 * @file    gui.h
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
 * The interface for a class used for the graphical user interface for h2sl
 */

#ifndef H2SL_GUI_H
#define H2SL_GUI_H

#include <iostream>

#include "h2sl/parser_cyk.h"
#include "h2sl/world.h"
#include "h2sl/llm.h"
#include "h2sl/factor_set.h"
#include "h2sl/dcg.h"

#include <QtGui/QWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QGraphicsScene>
#include <QtGui/QGraphicsView>
#include <QtGui/QGraphicsItem>
#include <QtGui/QComboBox>
#include <QtGui/QListWidget>
#include <QtGui/QGroupBox>
#include <QtGui/QTextBrowser>

namespace h2sl {
  class QGraphicsItem_Label : public QObject, public QGraphicsItem {
    Q_OBJECT
  public:
    QGraphicsItem_Label( const std::string& label = "na", QGraphicsItem * parent = NULL );
    virtual ~QGraphicsItem_Label();
    
    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
    QRectF boundingRect( void )const;

    inline std::string& label( void ){ return _label; };
    inline const std::string& label( void )const{ return _label; };

  protected:
    std::string _label;
  };
  
  class QGraphicsItem_Factor : public QObject, public QGraphicsItem {
    Q_OBJECT
  public:
    QGraphicsItem_Factor( QGraphicsItem * parent = NULL );
    virtual ~QGraphicsItem_Factor();

    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
    QRectF boundingRect( void )const;

    inline std::vector< QGraphicsLineItem* >& qgraphicslineitems( void ){ return _qgraphicslineitems; };
    inline const std::vector< QGraphicsLineItem* >& qgraphicslineitems( void )const{ return _qgraphicslineitems; };

  signals:
    void comment( const std::string& comment, const bool& error );

  protected:
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );

    bool _highlight;
    std::vector< QGraphicsLineItem* > _qgraphicslineitems;
  };

  class QGraphicsItem_Grounding : public QObject, public QGraphicsItem {
    Q_OBJECT
  public:
    QGraphicsItem_Grounding( const Grounding* randomVariable = NULL, QGraphicsItem * parent = NULL );
    virtual ~QGraphicsItem_Grounding();

    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
    QRectF boundingRect( void )const;

//    inline Grounding*& grounding( void ){ return _grounding; };
    inline const Grounding* grounding( void )const{ return _grounding; };
    inline std::vector< QGraphicsLineItem* >& qgraphicslineitems( void ){ return _qgraphicslineitems; };
    inline const std::vector< QGraphicsLineItem* >& qgraphicslineitems( void )const{ return _qgraphicslineitems; };

  signals:
    void comment( const std::string& comment, const bool& error );

  protected:
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );

    const Grounding * _grounding;
    bool _highlight;
    std::vector< QGraphicsLineItem* > _qgraphicslineitems;
  };

  class QGraphicsItem_Phrase : public QObject, public QGraphicsItem {
    Q_OBJECT
  public:
    QGraphicsItem_Phrase( const Phrase* randomVariable = NULL, QGraphicsItem * parent = NULL );
    virtual ~QGraphicsItem_Phrase();

    void paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget );
    QRectF boundingRect( void )const;

    inline const Phrase* phrase( void )const{ return _phrase; };
    inline std::vector< QGraphicsLineItem* >& qgraphicslineitems( void ){ return _qgraphicslineitems; };
    inline const std::vector< QGraphicsLineItem* >& qgraphicslineitems( void )const{ return _qgraphicslineitems; };

  signals:
    void comment( const std::string& comment, const bool& error );

  protected:
    virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
    virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
    virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );

    const Phrase * _phrase;
    bool _highlight;
    std::vector< QGraphicsLineItem* > _qgraphicslineitems;
  };

  class GUI : public QWidget {
    Q_OBJECT
  public:
    GUI( Grammar * grammar = NULL, Parser< Phrase > * parser = NULL, Symbol_Dictionary * symbolDictionary = NULL, Search_Space * searchSpace = NULL, World * world = NULL, Grounding * context = NULL, LLM * llm = NULL, DCG * dcg = NULL, const unsigned int& beamWidth = 4, const std::string& command = "", QWidget * parent = NULL );
    virtual ~GUI();

    virtual void update_world( void );
    virtual void update_graph( void );

    inline QGroupBox* group_box( void ){ return _group_box; };
    inline Parser< Phrase >*& parser( void ){ return _parser; };
    inline const Parser< Phrase >* parser( void )const{ return _parser; };
    inline DCG*& dcg( void ){ return _dcg; };
    inline const DCG* dcg( void )const{ return _dcg; }; 

  protected slots:
    void _receive_comment( const std::string& comment, const bool& error = false );
    void _line_edit_sentence_return_pressed( void );
    void _combo_box_solutions_index_changed( int index );

  protected:
    virtual void resizeEvent( QResizeEvent* event );

    virtual void _add_factor_set_graphics_items( const Factor_Set* factorSet, const unsigned int& solutionIndex, const std::vector< QGraphicsItem_Factor* >& parents = std::vector< QGraphicsItem_Factor* >() );
    virtual void _run_inference( const std::string& sentence );
    virtual void _send_output( void );
    QString _format_comment (const std::string& comment, const bool& error = false);

    QLineEdit* _line_edit_sentence;
    QListWidget * _list_widget_world;
    QComboBox * _combo_box_solutions;
    QGraphicsScene* _graphics_scene_graph;
    QGraphicsView* _graphics_view_graph;
    QTextBrowser * _text_browser_comments;
    QGroupBox* _group_box;

    Grammar * _grammar;
    Parser< Phrase >* _parser;
    Symbol_Dictionary * _symbol_dictionary;
    Search_Space * _search_space;
    World * _world;
    Grounding * _context;
    LLM * _llm;
    DCG* _dcg;
    unsigned int _beam_width;
    std::vector< std::pair< double, Phrase* > > _solutions;
    std::vector< std::vector< QGraphicsItem* > > _graphics_items;

  private:

  };
  std::ostream& operator<<( std::ostream& out, const GUI& other );
}

#endif /* H2SL_GUI_H */
