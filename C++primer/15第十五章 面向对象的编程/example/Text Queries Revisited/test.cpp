#include "stdafx.h"
#include "Query.h"

ifstream& open_file(ifstream &in, const string &file)
{
	in.close();     // close in case it was already open
	in.clear();     // clear any existing errors
	// if the open fails, the stream will be in an invalid state
	in.open(file.c_str()); // open the file we were given
	return in; // condition state is good if open succeeded
}

// return plural version of word if ctr isn't 1
string make_plural(size_t ctr, const string &word,
				   const string &ending)
{
	return (ctr == 1) ? word : word + ending;
}

void print_results(const set<TextQuery::line_no>& locs, const TextQuery &file)
{
	// if the word was found, then print count and all occurrences
	typedef set<TextQuery::line_no> line_nums;
	line_nums::size_type size = locs.size();
	cout << "\n" << " occurs "<< size << " "
		<< make_plural(size, "time", "s") << endl;

	// print each line in which the word appeared
	line_nums::const_iterator it = locs.begin();
	for ( ; it != locs.end(); ++it) {
		cout << "(line "
			// don't confound user with text lines starting at 0
			<< (*it) + 1 << ") "
			<< file.text_line(*it) << endl;
	}
}

// program takes single argument specifying the file to query
int main(int argc, char **argv)
{
	// open the file from which user will query words
	ifstream infile;
	if (argc < 2 || !open_file(infile, argv[1])) {
		cerr << "No input file!" << endl;
		return EXIT_FAILURE;
	}
	TextQuery tq;
	tq.read_file(infile); 
	typedef set<TextQuery::line_no> line_nums;
	Query q=Query("a")|Query("b")&Query("c");
	const line_nums &locs=q.eval (tq);
	cout<<q;
	print_results(locs, tq);

	system("pause");
	return 0;
}



