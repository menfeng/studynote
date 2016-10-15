#include "stdafx.h"
#include "TextQuery.h"


// read input file: store each line as element in lines_of_text
void TextQuery::store_file(ifstream &is)
{
	string textline;
	while (getline(is, textline))
		lines_of_text.push_back(textline);
}

// finds whitespace-separated words in the input vector
// and puts the word in word_map along with the line number
void TextQuery::build_map()
{
	// process each line from the input vector
	for (line_no line_num = 0;
		line_num != lines_of_text.size();
		++line_num)
	{
		//we'll use line to read the text a word at a time
		istringstream line(lines_of_text[line_num]);
		string word;
		while (line >> word)
			// add this line number to the set;
			// subscript will add word to the map if it's not already there
			word_map[word].insert(line_num);
	}
}



set<TextQuery::line_no> TextQuery::run_query(const string &query_word) const
{
	//Note: must use find and not subscript the map directly
	//to avoid adding words to word_map!
	map<string, set<line_no> >::const_iterator
		loc = word_map.find(query_word);
	if (loc == word_map.end())
		return set<line_no>(); // not found, return empty set
	else
		// fetch and return set of line numbers for this word
		return loc->second;
}

string TextQuery::text_line(line_no line) const
{
	if (line < lines_of_text.size())
		return lines_of_text[line];
	throw std::out_of_range("line number out of range");
}






