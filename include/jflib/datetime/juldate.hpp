

#ifndef __DATETIME_JULDATE_JFLIB_HPP__
#define __DATETIME_JULDATE_JFLIB_HPP__


#include <jflib/jflib.hpp>


/*! \defgroup dates Date Library

    Interfaces and implementation of rates, simple and composite.

    @{
*/

/** \file
 *  \ingroup dates
 *
 * juldate class definition
 */

namespace jflib  {



class basedate  {
public:
	virtual ~basedate() {}
	virtual bool isvalid() const {return false;}
	virtual int  year()    const {QM_FAIL("Not implemented");}
	virtual int  month()   const {QM_FAIL("Not implemented");}
	virtual int  day()     const {QM_FAIL("Not implemented");}
	virtual short day_of_week() const {QM_FAIL("Not implemented");}
	unsigned long toyyyymmdd() const {return this->day() + 100*(this->month() + 100*this->year());}
	std::string tostring() const {return obj2str(this->toyyyymmdd());}
};



/**\brief Julian date
 *
 * A juldate is defined with 30th Dec 1899 = 0. The format used by most spreadsheet
 */
class juldate : public basedate{
	public:
	typedef long    JDate;
	typedef double  JDateTime;
	//
	// Costructors
	juldate(const JDateTime& dte = 0) {this->set(dte);}
	juldate(const juldate& dte)       {this->set(dte.get());}
	juldate(int y, int m, int d);

	void  set(const JDateTime& dte);
  	const JDateTime& get() const    {return m_date;}
  	void  CalendarDate(int &year, int &month, int &day) const;

	bool       isvalid()     const {return m_date < c_dateMin;}
  	JDate      jdate()       const {return (JDate)m_date;}
  	short      day_of_week() const {return (this->jdate() + 5) % 7 + 1;}
  	int        year()        const {return 1900 + 4 * (this->jdate() - 1) / 1461;}
  	int        month()       const {int y; int m; int d; this->CalendarDate(y,m,d); return m;}
  	int        day()         const {int y; int m; int d; this->CalendarDate(y,m,d); return d;}
  	bool       isLeapYear()  const {return (this->jdate() - 1) % 1461 < 366;}

  	juldate NextWeekday(int numdays) const {return this->jdate() + (70001 + numdays - this->jdate()) % 7;}

  	juldate& operator = (const juldate& rhs) {this->set(rhs.get()); return *this;}
  	juldate& operator = (JDateTime rhs)      {this->set(rhs);       return *this;}

  	friend juldate operator + (const juldate& lhs, int rhs) {return lhs.m_date + rhs;}
  	bool operator >  (const juldate& rhs) const {return m_date >  rhs.m_date;}
  	bool operator >= (const juldate& rhs) const {return m_date >= rhs.m_date;}
  	bool operator <  (const juldate& rhs) const {return m_date <  rhs.m_date;}
  	bool operator <= (const juldate& rhs) const {return m_date <= rhs.m_date;}
  	bool operator == (const juldate& rhs) const {return m_date == rhs.m_date;}
  	bool operator != (const juldate& rhs) const {return m_date != rhs.m_date;}

	template <class date_type>
  	static date_type ToDate(const juldate& jdate)	{
  		int y,m,d;
  		jdate.CalendarDate(y,m,d);
  		return date_type(y,m,d);
  	}

	// Static methods
	//
	bool isLeapYear(int year)        {return !(year % 4);}
  	/// \brief Evaluate Roman Easter
  	/// @param year Year for which Easter day is required
  	/// @return Easter day
  	static juldate RomanEaster(int year)	{
  		int n = (204 - 11 * (year % 19)) % 30;
  		return (52 * year - 99127 + (5 * year / 4 + n + (n < 28)) / 7) * 7 + 1;
  	}
  	/// \brief Evaluate Orthodox Easter
  	/// @param year Year for which Orthodox Easter day is required
  	/// @return Orthodox Easter day
  	static juldate OrthodoxEaster(int year)	{
		return (52 * year - 99125 + ((19 * (year % 19) + 15) % 30 +
  		year * 5 / 4) / 7) * 7 + 1;
  	}
  	/// \brief Number of days between two dates
  	///
  	///@param start The start qdate
  	///@param end The end qdate
  	///@return The number of days between start nad end
  	static long daydiff(const juldate& start, const juldate& end)  {
  		return JDate(end.m_date) - JDate(start.m_date);
  	}

private:
	const static unsigned c_dateMin  = 61;       // earliest permissible date: 1 Mar 1900
	JDateTime m_date;
};



inline void juldate::set(const JDateTime& dte)  {
  	if(dte < c_dateMin) m_date = 0;
  	else m_date = dte;
}

inline juldate::juldate(int y, int m, int d)  {
	div_t f = div(12 * y + m - 22803, 12); // March 1900 is month zero
	juldate::JDate dte = (f.rem * 153 + 302) / 5 + d + f.quot * 1461L / 4;
	this->set(dte);
}

inline void juldate::CalendarDate(int &y, int &m, int &d) const  {
  		ldiv_t fy = ldiv(4 * this->jdate(), 1461);
  		short yd = (short)fy.rem, yd1 = yd - 241;
  		div_t q;
  		y = fy.quot + (yd >= 4? 1900: 1899);
  		if (yd1 >= 0) q = div(yd1 / 4 * 5 + 308, 153);
  		else q = div(yd / 4 * 5 + 1833, 153);
  		m = q.quot % 12 + 1;
  		d = q.rem / 5 + 1;
  	}


REGISTER2STRING(basedate);
REGISTER2STRING(juldate);



}


/*! @} */


#endif // __DATETIME_JULDATE_JFLIB_HPP__

