#include "stdafx.h"
#include "TextQuery.h"
/*
�û����ܲ�����ֻ��Query
Query����Query_base
Query_base��Ϊ�������ͣ��������ΪWordQuery
�κ�һ�����ϲ�ѯ����������ѯ���Ҳ�ѯ����󶼻���ʵ��WordQuery�ϵĲ�ѯ,WordQuery����TextQuery
*/
/***************************Query_base******************************/

// private, abstract class acts as a base class for concrete query types
class Query_base {
	friend class Query;
protected:
	typedef TextQuery::line_no line_no;
	virtual ~Query_base() { }
private:
	// eval returns the |set| of lines that this Query matches
	virtual std::set<line_no>
		eval(const TextQuery&) const = 0;
	// display prints the query
	virtual std::ostream&
		display(std::ostream& = std::cout) const = 0;
};


/***************************Query******************************/
// handle class to manage the Query_base inheritance hierarchy
class Query {
	// these operators need access to the Query_base* constructor
	friend Query operator~(const Query &);
	friend Query operator|(const Query&, const Query&);
	friend Query operator&(const Query&, const Query&);
public:
	Query(const std::string&); // builds a new WordQuery����������Ǵ���һ��WordQuery
	// copy control to manage pointers and use counting
	Query(const Query &c): q(c.q), use(c.use) { ++*use; }
	~Query() { decr_use(); }
	Query& operator=(const Query&);
	// interface functions: will call corresponding Query_base operations
	std::set<TextQuery::line_no>eval(const TextQuery &t) const { return q->eval(t); }
	std::ostream &display(std::ostream &os) const{ return q->display(os); }
private:
	//��Query�Ĳ������л���ô˺���
	Query(Query_base *query): q(query),use(new std::size_t(1)) { }
	Query_base *q;
	std::size_t *use;
	void decr_use()
	{ if (--*use == 0) { delete q; delete use; } }
};
inline Query& Query::operator=(const Query& rhs)
{
	++*rhs.use;
	decr_use();
	q=rhs.q;
	use=rhs.use;
	return *this;
}

inline std::ostream&
operator<<(std::ostream &os, const Query &q)
{
	return q.display(os);
}

/***************************WordQuery******************************/
class WordQuery: public Query_base {
	friend class Query; // Query uses the WordQuery constructor
	WordQuery(const std::string &s): query_word(s) { }
	// concrete class: WordQuery defines all inherited pure virtual functions
	std::set<line_no> eval(const TextQuery &t) const
	{ return t.run_query(query_word); }
	std::ostream& display (std::ostream &os) const
	{ return os << query_word; }
	std::string query_word; // word for which to search
};

//������WordQuery��  �ٶ���Query�Ĺ��캯��
inline Query::Query(const string& s):
q(new WordQuery(s)),
use(new size_t(1))
{

}

/***************************NotQuery******************************/
class NotQuery: public Query_base {
	friend Query operator~(const Query &);
	NotQuery(Query q): query(q) { }
	// concrete class: NotQuery defines all inherited pure virtual functions
	std::set<line_no> eval(const TextQuery&) const;
	std::ostream& display(std::ostream &os) const
	{ return os << "~(" << query << ")"; }
	const Query query;
};

/***************************BinaryQuery******************************/
class BinaryQuery: public Query_base {
protected:
	BinaryQuery(Query left, Query right, std::string op):
		 lhs(left), rhs(right), oper(op) { }
		 // abstract class: BinaryQuery doesn't define eval
		 std::ostream& display(std::ostream &os) const
		 { return os << "(" << lhs << " " << oper << " "
		 << rhs << ")"; }
		 const Query lhs, rhs; // right- and left-hand operands
		 const std::string oper; // name of the operator
};

/***************************AndQuery******************************/
class AndQuery: public BinaryQuery {
	friend Query operator&(const Query&, const Query&);
	AndQuery (Query left, Query right):
	BinaryQuery(left, right, "&") { }
	// concrete class: And Query inherits display and defines remaining pure virtual
	std::set<line_no> eval(const TextQuery&) const;
};

/***************************OrQuery******************************/
class OrQuery: public BinaryQuery {
	friend Query operator|(const Query&, const Query&);
	OrQuery(Query left, Query right):
	BinaryQuery(left, right, "|") { }
	// concrete class: OrQuery inherits display and defines remaining pure virtual
	std::set<line_no> eval(const TextQuery&) const;
};



//����not and or ���ٶ��������
inline Query operator&(const Query &lhs, const Query &rhs)
{
	return new AndQuery(lhs, rhs);
}
inline Query operator|(const Query &lhs, const Query &rhs)
{
	return new OrQuery(lhs, rhs);
}
inline Query operator~(const Query &oper)
{
	return new NotQuery(oper);
}




