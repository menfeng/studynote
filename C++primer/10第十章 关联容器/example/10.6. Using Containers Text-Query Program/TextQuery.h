#include "stdafx.h"

class TextQuery {
     public:
         // typedef to make declarations easier
         typedef std::vector<std::string>::size_type line_no;
         /* interface:
          * read_file builds internal data structures for the given file
          * run_query finds the given word and returns set of lines on which it appears
          * text_line returns a requested line from the input file
          */
         void read_file(std::ifstream &is)
                    { store_file(is); build_map(); }
         std::set<line_no> run_query(const std::string&) const;
         std::string text_line(line_no) const;
     private:
         // utility functions used by read_file
         void store_file(std::ifstream&); // store input file
         void build_map(); // associated each word with a set of line numbers
         // remember the whole input file
         std::vector<std::string> lines_of_text;
         // map word to set of the lines on which it occurs
         std::map< std::string, std::set<line_no> > word_map;
     };


