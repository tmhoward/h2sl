/**
 * @file    dcg.cc
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
 * The implementation of a class used to represent a Distributed 
 *   Correspondence Graph
 */

#include <fstream>
#include <utility>
#include <cassert>

#include "h2sl/grounding_set.h"
#include "h2sl/dcg.h"

using namespace std;
using namespace h2sl;

DCG::
DCG() : _solutions(),
        _root( NULL ) {

}

DCG::
~DCG() {

}

DCG::
DCG( const DCG& other ) : _solutions( other._solutions ),
                          _root( other._root ) {

}

DCG&
DCG::
operator=( const DCG& other ) {
  _solutions = other._solutions;
  _root = other._root;
  return (*this);
}

bool
DCG::
leaf_search( const Phrase* phrase,
              const Symbol_Dictionary& symbolDictionary,
              Search_Space* searchSpace,
              const World* world,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  return leaf_search( phrase, symbolDictionary, searchSpace, world, NULL, llm, beamWidth, debug );
}
  
bool
DCG::
leaf_search( const Phrase* phrase,
              const Symbol_Dictionary& symbolDictionary,
              Search_Space* searchSpace,
              const World* world,
              const Grounding* context,
              LLM * llm,
              const unsigned int beamWidth,
              const bool& debug ){
  for( unsigned int i = 0; i < _solutions.size(); i++ ){
    if( _solutions[ i ].second != NULL ){
      delete _solutions[ i ].second;
      _solutions[ i ].second = NULL;
    }
  }
  _solutions.clear();

  if( phrase != NULL ){
    if( _root != NULL ){
      delete _root;
      _root = NULL;
    }

    _root = new Factor_Set( phrase->dup() );
    _fill_factors( _root, _root->phrase() );  

    Factor_Set * leaf = NULL;
    _find_leaf( _root, leaf );
    while( leaf != NULL ){
      leaf->search( searchSpace,
                    symbolDictionary,
                    world,
                    context,
                    llm,
                    beamWidth,
                    debug );
      leaf = NULL;
      _find_leaf( _root, leaf );
    }
  
    for( unsigned int i = 0; i < _root->solutions().size(); i++ ){
      _solutions.push_back( pair< double, Phrase* >( _root->solutions()[ i ].pygx(), _root->phrase()->dup() ) );

      for( unsigned int j = 0; j < _solutions.back().second->children().size(); j++ ){
        if( _solutions.back().second->children()[ j ] != NULL ){
          delete _solutions.back().second->children()[ j ];
          _solutions.back().second->children()[ j ];
        }
      }
      _solutions.back().second->children().clear();
      if( _solutions.back().second->grounding_set() != NULL ){
        delete _solutions.back().second->grounding_set();
        _solutions.back().second->grounding_set() = NULL;
      }
   
      _fill_phrase( _root, _root->solutions()[ i ], _solutions.back().second, debug );
    }

    return true;
  } else {
    return false;
  }
}

void
DCG::
to_tikz( const Search_Space* searchSpace,
          const Phrase* phrase,
          const string& filename,
          const string& modelType,
          const string& caption,
          const string& label )const{
  ofstream outfile;
  outfile.open( filename );

  outfile << "\\begin{figure}[!htb]" << endl;
  outfile << "\\begin{center}" << endl;
  outfile << "\\begin{tikzpicture}[textnode/.style={anchor=mid,font=\\tiny},nodeknown/.style={circle,draw=black!80,fill=black!10,minimum size=6mm,font=\\tiny,top color=white,bottom color=black!20},nodeunknown/.style={circle,draw=black!80,fill=white,minimum size=6mm,font=\\tiny},factor/.style={rectangle,draw=black!80,fill=black!80,minimum size=2mm,font=\\tiny,text=white}]" << endl;

  unsigned int offset = 0;

  if( modelType == "gm" ){
    outfile << to_tikz_edges_gm( phrase, offset );
    offset = 0;
    outfile << to_tikz_nodes_gm( phrase, offset );
    outfile << "\\end{tikzpicture}" << endl;
    outfile << "\\end{center}" << endl;
    outfile << "\\caption{The DCG for the utterance \"" << phrase->all_words_to_std_string() << "\" where the number of groundings per phrase (n) is " << searchSpace->size() << ".}" << endl;
  } else if ( modelType == "egm" ){
    outfile << to_tikz_edges_egm( searchSpace, phrase, offset );
    offset = 0;
    outfile << to_tikz_nodes_egm( searchSpace, phrase, offset );
    outfile << "\\end{tikzpicture}" << endl;
    outfile << "\\end{center}" << endl;
    vector< pair< int, string > > legend;
    generate_tikz_legend_egm( searchSpace, phrase, legend );
    outfile << to_tikz_legend_egm( searchSpace, phrase, legend );
  } 

  outfile << "\\label{" << label << "}" << endl;
  outfile << "\\end{figure}" << endl;

  outfile.close();
  return;
} 

string
DCG::
to_tikz_nodes_gm( const h2sl::Phrase* phrase,
                unsigned int& offset )const{
  stringstream tmp;

  for( unsigned int i = 0; i < phrase->words().size(); i++ ){
    tmp << "\\node[textnode] (l" << offset << ") at (" << offset * 1.25 << "," << -0.25 * ( double )( i ) << ") {\\footnotesize{\\textit{" << phrase->words()[ i ].text() << "}}};" << endl;
  }
  pair< double, double > p_node_position( offset * 1.25, 0.5 );
  tmp << "\\node[nodeknown] (p" << offset << ") at (" << p_node_position.first << "," << p_node_position.second << ") {};" << endl;
  tmp << "\\node[font=\\tiny] (p" << offset << "label) at (" << p_node_position.first << "," << p_node_position.second << ") {$\\lambda_{" << offset << "}$};" << endl;
  tmp << "%" << endl;
  pair< double, double > c1_node_position( -0.5 + offset * 1.25, 1.0 );
  tmp << "\\node[nodeunknown] (c" << offset << "1) at (" << c1_node_position.first << "," << c1_node_position.second << ") {};" << endl;
  tmp << "\\node[font=\\tiny] (c" << offset << "1label) at (" << c1_node_position.first << "," << c1_node_position.second << ") {$\\phi_{" << offset << "_{1}}$};" << endl;
  pair< double, double > c2_node_position( -0.5 + offset * 1.25, 2.0 );
  tmp << "\\node[nodeunknown] (c" << offset << "2) at (" << c2_node_position.first << "," << c2_node_position.second << ") {};" << endl;
  tmp << "\\node[font=\\tiny] (c" << offset << "2label) at (" << c2_node_position.first << "," << c2_node_position.second << ") {$\\phi_{" << offset << "_{2}}$};" << endl;
  pair< double, double > cn_node_position( -0.5 + offset * 1.25, 4.0 );
  tmp << "\\node[nodeunknown] (c" << offset << "n) at (" << cn_node_position.first << "," << cn_node_position.second << ") {};" << endl;
  tmp << "\\node[font=\\tiny] (c" << offset << "nlabel) at (" << cn_node_position.first << "," << cn_node_position.second << ") {$\\phi_{" << offset << "_{n}}$};" << endl;
  pair< double, double > g1_node_position( offset * 1.25, 1.5 );
  tmp << "\\node[nodeknown] (g" << offset << "1) at (" << g1_node_position.first << "," << g1_node_position.second << ") {};" << endl;
  tmp << "\\node[font=\\tiny] (g" << offset << "1label) at (" << g1_node_position.first << "," << g1_node_position.second << ") {$\\phi_{" << offset << "_{1}}$};" << endl;
  pair< double, double > g2_node_position( offset * 1.25, 2.5 );
  tmp << "\\node[nodeknown] (g" << offset << "2) at (" << g2_node_position.first << "," << g2_node_position.second << ") {};" << endl;
  tmp << "\\node[font=\\tiny] (g" << offset << "2label) at (" << g2_node_position.first << "," << g2_node_position.second << ") {$\\phi_{" << offset << "_{2}}$};" << endl;
  tmp << "\\node[] (g" << offset << "dots) at (" << offset * 1.25 << "," << 3.375 << ") {$\\vdots$};" << endl;
  pair< double, double > gn_node_position( offset * 1.25, 4.5 );
  tmp << "\\node[nodeknown] (g" << offset << "n) at (" << gn_node_position.first << "," << gn_node_position.second << ") {};" << endl;
  tmp << "\\node[font=\\tiny] (g" << offset << "nlabel) at (" << gn_node_position.first << "," << gn_node_position.second << ") {$\\phi_{" << offset << "_{n}}$};" << endl;
  pair< double, double > f1_node_position( offset * 1.25, 1.0 );
  tmp << "\\node[factor] (f" << offset << "1) at (" << f1_node_position.first << "," << f1_node_position.second << ") {};" << endl;
  pair< double, double > f2_node_position( offset * 1.25, 2.0 );
  tmp << "\\node[factor] (f" << offset << "2) at (" << f2_node_position.first << "," << f2_node_position.second << ") {};" << endl;
  pair< double, double > fn_node_position( offset * 1.25, 4.0 );
  tmp << "\\node[factor] (f" << offset << "n) at (" << fn_node_position.first << "," << fn_node_position.second << ") {};" << endl;
  tmp << "%" << endl;

  offset++;

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    tmp << to_tikz_nodes_gm( phrase->children()[ i ], offset );
  }

  return tmp.str();
}

string
DCG::
to_tikz_edges_gm( const h2sl::Phrase* phrase,
                unsigned int& offset )const{
  stringstream tmp;

  cout << "num_phrases:" << phrase->num_phrases() << endl;

  tmp << "\\draw[-] (" << offset * 1.25 << ",0.5) to (" << offset * 1.25 << ",1);" << endl;
  tmp << "\\draw[-] (" << offset * 1.25 << ",0.5) to [bend right=30] (" << offset * 1.25 << ",2);" << endl;
  tmp << "\\draw[-] (" << offset * 1.25 << ",0.5) to [bend right=30] (" << offset * 1.25 << ",4);" << endl;
  tmp << "\\draw[-] (" << -0.5 + offset * 1.25 << ",1) to (" << offset * 1.25 << ",1);" << endl;
  tmp << "\\draw[-] (" << -0.5 + offset * 1.25 << ",2) to (" << offset * 1.25 << ",2);" << endl;
  tmp << "\\draw[-] (" << -0.5 + offset * 1.25 << ",4) to (" << offset * 1.25 << ",4);" << endl;
  tmp << "\\draw[-] (" << offset * 1.25 << ",1.5) to (" << offset * 1.25 << ",1);" << endl;
  tmp << "\\draw[-] (" << offset * 1.25 << ",2.5) to (" << offset * 1.25 << ",2);" << endl;
  tmp << "\\draw[-] (" << offset * 1.25 << ",4.5) to (" << offset * 1.25 << ",4);" << endl;

  unsigned int child_offset = offset + 1;
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    cout << "children[" << i << "] num_phrases:" << phrase->children()[ i ]->num_phrases() << endl;
    tmp << "\\draw[-] (" << child_offset * 1.25 << ",1.5) to (" << offset * 1.25 << ",1);" << endl;
    tmp << "\\draw[-] (" << child_offset * 1.25 << ",2.5) to (" << offset * 1.25 << ",1);" << endl;
    tmp << "\\draw[-] (" << child_offset * 1.25 << ",4.5) to (" << offset * 1.25 << ",1);" << endl;
    tmp << "\\draw[-] (" << child_offset * 1.25 << ",1.5) to (" << offset * 1.25 << ",2);" << endl;
    tmp << "\\draw[-] (" << child_offset * 1.25 << ",2.5) to (" << offset * 1.25 << ",2);" << endl;
    tmp << "\\draw[-] (" << child_offset * 1.25 << ",4.5) to (" << offset * 1.25 << ",2);" << endl;
    tmp << "\\draw[-] (" << child_offset * 1.25 << ",1.5) to (" << offset * 1.25 << ",4);" << endl;
    tmp << "\\draw[-] (" << child_offset * 1.25 << ",2.5) to (" << offset * 1.25 << ",4);" << endl;
    tmp << "\\draw[-] (" << child_offset * 1.25 << ",4.5) to (" << offset * 1.25 << ",4);" << endl;
    child_offset += phrase->children()[ i ]->num_phrases();
  }

  offset++;

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    tmp << to_tikz_edges_gm( phrase->children()[ i ], offset );
  }

  return tmp.str();
} 

string
DCG::
to_tikz_nodes_egm( const Search_Space* searchSpace,
                  const h2sl::Phrase* phrase,
                  unsigned int& offset )const{
  stringstream tmp;

  cout << "phrase->groundings().size():" << phrase->grounding_set()->groundings().size() << endl;

  for( unsigned int i = 0; i < phrase->words().size(); i++ ){
    tmp << "\\node[textnode] (l" << offset << ") at (" << offset * 1.25 << "," << -0.25 * ( double )( i ) << ") {\\footnotesize{\\textit{" << phrase->words()[ i ].text() << "}}};" << endl;
  }
  
  pair< double, double > p_node_position( offset * 1.25, 0.5 );
  tmp << "\\node[nodeknown] (p" << offset << ") at (" << p_node_position.first << "," << p_node_position.second << ") {};" << endl;
  tmp << "\\node[font=\\tiny] (p" << offset << "label) at (" << p_node_position.first << "," << p_node_position.second << ") {$\\lambda_{" << offset << "}$};" << endl;
  tmp << "%" << endl;

  vector< unsigned int > expressed_grounding_indices;
  for( unsigned int i = 0; i < phrase->grounding_set()->groundings().size(); i++ ){
    int index = -1;
    int counter = 0;
    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_groundings = searchSpace->grounding_pairs().begin(); it_groundings != searchSpace->grounding_pairs().end(); it_groundings++ ){
      for( unsigned int j = 0; j < it_groundings->second.second.size(); j++ ){
        if( *it_groundings->second.second[ j ] == *phrase->grounding_set()->groundings()[ i ] ){
          index = counter;
        }
        counter++;
      }
    }
    if( index == -1 ){
      cout << "did not find match for " << *phrase->grounding_set()->groundings()[ i ] << endl;
    }
    assert( index >= 0 );
    expressed_grounding_indices.push_back( index );
  }

  sort( expressed_grounding_indices.begin(), expressed_grounding_indices.end() );

  for( unsigned int i = 0; i < expressed_grounding_indices.size(); i++ ){
    pair< double, double > g_node_position( offset * 1.25, 1.5 + 1.0 * ( double )( i ) );
    tmp << "\\node[nodeknown] (g" << offset << "_" << expressed_grounding_indices[ i ] << ") at (" << g_node_position.first << "," << g_node_position.second << ") {};" << endl;
    tmp << "\\node[font=\\tiny] (g" << offset << "_" << expressed_grounding_indices[ i ] << "label) at (" << g_node_position.first << "," << g_node_position.second << ") {\\scalebox{.6}{$\\phi_{" << offset << "_{" << expressed_grounding_indices[ i ] << "}}$}};" << endl;
  
    pair< double, double > c_node_position( -0.5 + offset * 1.25, 1.0  + 1.0 * ( double )( i ) );
    tmp << "\\node[nodeknown] (c" << offset << "_" << expressed_grounding_indices[ i ] << ") at (" << c_node_position.first << "," << c_node_position.second << ") {};" << endl;
    tmp << "\\node[font=\\tiny] (c" << offset << "_" << expressed_grounding_indices[ i ] << "label) at (" << c_node_position.first << "," << c_node_position.second << ") {\\scalebox{0.6}{$\\phi_{" << offset << "_{" << expressed_grounding_indices[ i ] << "}}$}};" << endl;
    pair< double, double > f_node_position( offset * 1.25, 1.0 + 1.0 * ( double )( i ) );
    tmp << "\\node[factor] (f" << offset << "_" << expressed_grounding_indices[ i ] << ") at (" << f_node_position.first << "," << f_node_position.second << ") {};" << endl;
  }

  offset++;

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    tmp << to_tikz_nodes_egm( searchSpace, phrase->children()[ i ], offset );
  }

  return tmp.str();
}

string
DCG::
to_tikz_edges_egm( const Search_Space* searchSpace,
                    const h2sl::Phrase* phrase,
                    unsigned int& offset )const{
  stringstream tmp;

  cout << "num_phrases:" << phrase->num_phrases() << endl;

  vector< unsigned int > expressed_grounding_indices;
  for( unsigned int i = 0; i < phrase->grounding_set()->groundings().size(); i++ ){
    int index = -1;
    int counter = 0;
    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_groundings = searchSpace->grounding_pairs().begin(); it_groundings != searchSpace->grounding_pairs().end(); it_groundings++ ){
      for( unsigned int j = 0; j < it_groundings->second.second.size(); j++ ){
        if( *it_groundings->second.second[ j ] == *phrase->grounding_set()->groundings()[ i ] ){
          index = counter;
        }
        counter++;
      }
    }
    if( index == -1 ){
      cout << "did not find match for " << *phrase->grounding_set()->groundings()[ i ] << endl;
    }
    assert( index >= 0 );
    expressed_grounding_indices.push_back( index );
  }

  sort( expressed_grounding_indices.begin(), expressed_grounding_indices.end() );

  for( unsigned int i = 0; i < expressed_grounding_indices.size(); i++ ){
    pair< double, double > g_node_position( offset * 1.25, 1.5 + 1.0 * ( double )( i ) );
    if( i == 0 ){
      tmp << "\\draw[-] (" << offset * 1.25 << ",0.5) to (" << offset * 1.25 << ",1.0);" << endl;
    } else {
      tmp << "\\draw[-] (" << offset * 1.25 << ",0.5) to [bend right=30] (" << offset * 1.25 << "," << 1.0 + ( double )( i ) << ");" << endl;
    }
    tmp << "\\draw[-] (" << -0.5 + offset * 1.25 << "," << 1.0 + ( double )( i ) << ") to (" << offset * 1.25 << "," << 1.0 + ( double )( i ) << ");" << endl;
    tmp << "\\draw[-] (" << offset * 1.25 << "," << 1.25 + ( double )( i ) << ") to (" << offset * 1.25 << "," <<  1.0 + ( double )( i ) << ");" << endl;
  }

  unsigned int child_offset = offset + 1;
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    vector< unsigned int > expressed_child_grounding_indices;
    for( unsigned int j = 0; j < phrase->children()[ i ]->grounding_set()->groundings().size(); j++ ){
      int index = -1;
      int counter = 0;
      for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_groundings = searchSpace->grounding_pairs().begin(); it_groundings != searchSpace->grounding_pairs().end(); it_groundings++ ){
        for( unsigned int k = 0; k < it_groundings->second.second.size(); k++ ){
          if( *it_groundings->second.second[ k ] == *phrase->children()[ i ]->grounding_set()->groundings()[ j ] ){
            index = counter;
          } 
          counter++;
        }
      }
      if( index == -1 ){
        cout << "did not find match for " << *phrase->children()[ i ]->grounding_set()->groundings()[ j ] << endl;
      }
      assert( index >= 0 );
      expressed_child_grounding_indices.push_back( index );
    }

    sort( expressed_child_grounding_indices.begin(), expressed_child_grounding_indices.end() );

    for( unsigned int j = 0; j < expressed_child_grounding_indices.size(); j++ ){
      for( unsigned int k = 0; k < expressed_grounding_indices.size(); k++ ){
        tmp << "\\draw[-] (" << child_offset * 1.25 << "," << 1.25 + ( double )( j ) << ") to (" << offset * 1.25 << "," << 1.0 + ( double )( k ) << ");" << endl;
      }
    }
    child_offset += phrase->children()[ i ]->num_phrases();
  }

  offset++;

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    tmp << to_tikz_edges_egm( searchSpace, phrase->children()[ i ], offset );
  }

  return tmp.str();
} 

void
DCG::
generate_tikz_legend_egm( const Search_Space* searchSpace,
                          const h2sl::Phrase* phrase,
                          vector< pair< int, string > >& entries )const{
  for( unsigned int i = 0; i < phrase->grounding_set()->groundings().size(); i++ ){
    int index = -1;
    int counter = 0;
    for( map< string, pair< string, vector< Grounding* > > >::const_iterator it_groundings = searchSpace->grounding_pairs().begin(); it_groundings != searchSpace->grounding_pairs().end(); it_groundings++ ){
      for( unsigned int j = 0; j < it_groundings->second.second.size(); j++ ){
        if( *it_groundings->second.second[ j ] == *phrase->grounding_set()->groundings()[ i ] ){
          index = counter;
        }
        counter++;
      }
    }
    if( index == -1 ){
      cout << "did not find match for " << *phrase->grounding_set()->groundings()[ i ] << endl;
    }
    assert( index >= 0 );
    bool new_index = true;
    for( unsigned int j = 0; j < entries.size(); j++ ){
      if( index == entries[ j ].first ){
        new_index = false;
      }   
    }
    if( new_index ){
      entries.push_back( pair< int, string >( index, phrase->grounding_set()->groundings()[ i ]->to_latex() ) );
    }
  }

  sort( entries.begin(), entries.end() );

  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    generate_tikz_legend_egm( searchSpace, phrase->children()[ i ], entries );
  }
  return;
}

string
DCG::
to_tikz_legend_egm( const Search_Space* searchSpace,
                    const Phrase* phrase, 
                    const vector< pair< int, string > >& entries )const {
  stringstream tmp;

  tmp << "\\caption{The expressed groundings in the DCG for the utterance \"" << phrase->all_words_to_std_string() << "\" where the number of groundings per phrase is " << searchSpace->size() << " and "; 

  for( unsigned int i = 0; i < entries.size(); i++ ){
    if( i == ( entries.size() - 1 ) ){
      tmp << "and ";
    }
    tmp << "index " << entries[ i ].first << " is " << entries[ i ].second;  
    if( i < ( entries.size() - 1 ) ){
      tmp << ", ";
    }
  }   

  tmp << "}" << endl;

  return tmp.str();
}

void
DCG::
_find_leaf( Factor_Set* node, 
            Factor_Set*& leaf ){ 
  if( node->solutions().empty() ){
    bool all_child_factor_sets_known = true;
    for( unsigned int i = 0; i < node->child_factor_sets().size(); i++ ){
      if( node->child_factor_sets()[ i ]->solutions().empty() ){
        all_child_factor_sets_known = false;
      } 
    } 
    if( all_child_factor_sets_known ){
      leaf = node;
    }
  }

  for( unsigned int i = 0; i < node->child_factor_sets().size(); i++ ){
    _find_leaf( node->child_factor_sets()[ i ], leaf );
  }
  return;
}

void
DCG::
_fill_phrase( Factor_Set* node,
              Factor_Set_Solution& solution,
              Phrase* phrase,
              const bool& debug ){
  if( debug ){
    cout << "filling phrase:" << *phrase << endl;
    cout << "solution:" << solution << endl;
  }

   //cout << "transfer properties from factor set to phrase:" << endl;
   // Transfer relevant properties. 
   std::map< std::string, std::string >::const_iterator it;
   it = node->properties().find( "concrete_size" );
   if( it != node->properties().end() ){
     insert_prop< std::string >( phrase->properties(), "concrete_size", it->second );
     //cout << "found key:" << "concrete_size" << "filling: " << it->second << endl;
   } else {
     insert_prop< std::string >( phrase->properties(), "concrete_size", "0" );
     //cout << "adding the key:" << "concrete_size" << "filling: " << "0" << endl;
   }

   it = node->properties().find( "abstract_max_size" );
   if( it != node->properties().end() ){
     insert_prop< std::string >( phrase->properties(), "abstract_max_size", it->second );
   } else {
     insert_prop< std::string >( phrase->properties(), "abstract_max_size", "0" );
   }

    it = node->properties().find( "abstract_avg_size" );
   if( it != node->properties().end() ){
     insert_prop< std::string >( phrase->properties(), "abstract_avg_size", it->second );
   } else {
     insert_prop< std::string >( phrase->properties(), "abstract_avg_size", "0.0" );
   }
     
  // Copy the grounding set
  phrase->grounding_set() = solution.grounding_set()->dup();

  // Examine children.
  for( unsigned int i = 0; i < node->child_factor_sets().size(); i++ ){
    phrase->children().push_back( node->child_factor_sets()[ i ]->phrase()->dup() );
    for( unsigned int j = 0; j < phrase->children().back()->children().size(); j++ ){
      if( phrase->children().back()->children()[ j ] != NULL ){
        delete phrase->children().back()->children()[ j ];
        phrase->children().back()->children()[ j ];
      }
    }
    phrase->children().back()->children().clear();
    if( phrase->children().back()->grounding_set() != NULL ){
      delete phrase->children().back()->grounding_set();
      phrase->children().back()->grounding_set() = NULL;
    }

    assert( i < node->child_factor_sets().size() );
    assert( i < solution.child_solution_indices().size() );
    assert( solution.child_solution_indices()[ i ] < node->child_factor_sets()[ i ]->solutions().size() );

    _fill_phrase( node->child_factor_sets()[ i ],
                  node->child_factor_sets()[ i ]->solutions()[ solution.child_solution_indices()[ i ] ],
                  phrase->children().back() );

  }
  return;
}

void
DCG::
_fill_factors( Factor_Set* node,
                const Phrase* phrase, 
                const bool& fill ){
  for( unsigned int i = 0; i < phrase->children().size(); i++ ){
    node->child_factor_sets().push_back( new Factor_Set( phrase->children()[ i ] ) );
    _fill_factors( node->child_factor_sets().back(), phrase->children()[ i ] );
  } 
  return;
}

namespace h2sl {
  ostream&
  operator<<( ostream& out,
              const DCG& other ) {
    
    return out;
  }
}
