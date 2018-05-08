/* complex.hh */
#ifndef COMPLEX_HH
#define COMPLEX_HH

#include <cmath>

template <typename T>
class Complex
{
public:
	Complex(const T &real = 0, const T &imag = 0)
	{
		this->set(real, imag);
	}

	Complex add(const Complex &z)
	{
		this->_real += z.real();
		this->_imag += z.imag();
		return *this;
	}

	Complex subtract(const Complex &z)
	{
		return this->add(-z);
	}

	Complex multiply(const Complex &z)
	{
		Complex w{};
		w._real = (this->real() * z.real()) - (this->imag() * z.imag());
		w._imag = (this->imag() * z.real()) + (this->real() * z.imag());
		return *this = w;
	}

	Complex divide(const Complex &z)
	{
		Complex w{};
		w._real = this->real() * z.real() + this->imag() * z.imag();
		w._imag = this->imag() * z.real() - this->real() * z.imag();
		w._real /= z.norm();
		w._imag /= z.norm();
		return *this = w;
	}

	Complex square(void) const
	{
		Complex z{};
		z._real  = this->real() * this->real();
		z._real -= this->imag() * this->imag();
		z._imag  = this->real() * this->imag() * 2;
		return z;
	}

	Complex cube(void) const
	{
		return this->square() * (*this);
	}

	Complex reciprocal(void) const
	{
		Complex z{this->conjugate()};
		z._real /= this->norm();
		z._imag /= this->norm();
		return z;
	}

	Complex conjugate(void) const
	{
		return {this->real(), -this->imag()};
	}

	T argument(void) const
	{
		return std::atan(this->imag() / this->real());
	}

	/* Evaluates to the magnitude/modulus squared
	 */
	T norm(void) const
	{
		return this->real() * this->real() + this->imag() * this->imag();
	}

	T modulus(void) const
	{
		return (T)std::sqrt(this->norm());
	}

	bool equals(const Complex &z) const
	{
		return this->real() == z.real() && this->imag() == z.imag();
	}

	void set(const T &real, const T &imag)
	{
		this->_real = real;
		this->_imag = imag;
	}

	/* Casting to other types of complex numbers
	 * can be done implicitly
	 */
	template <typename S>
	operator Complex<S>(void) const
	{
		return {(S)this->real(), (S)this->imag()};
	}
	
	template <typename S>
	explicit operator S(void) const
	{
		return (S)this->real();
	}

	inline T real(void) const
	{
		return this->_real;
	}

	inline T imag(void) const
	{
		return this->_imag;
	}

	inline Complex operator +(void) const
	{
		return *this;
	}

	inline Complex operator -(void) const
	{
		return {-this->real(), -this->imag()};
	}

	inline Complex operator +(const Complex &z) const
	{
		return ((Complex)(*this)).add(z);
	}

	inline Complex operator -(const Complex &z) const
	{
		return ((Complex)(*this)).subtract(z);
	}

	inline Complex operator *(const Complex &z) const
	{
		return ((Complex)(*this)).multiply(z);
	}

	inline Complex operator /(const Complex &z) const
	{
		return ((Complex)(*this)).divide(z);
	}

	inline Complex operator +=(const Complex &z)
	{
		return this->add(z);
	}

	inline Complex operator -=(const Complex &z)
	{
		return this->subtract(z);
	}

	inline Complex operator *=(const Complex &z)
	{
		return this->multiply(z);
	}

	inline Complex operator /=(const Complex &z)
	{
		return this->divide(z);
	}

	inline bool operator ==(const Complex &z) const
	{
		return this->equals(z);
	}

	inline bool operator !=(const Complex &z) const
	{
		return !this->equals(z);
	}

	inline bool operator >=(const Complex &z) const
	{
		return *this == z || *this > z;
	}

	inline bool operator <=(const Complex &z) const
	{
		return *this == z || *this < z;
	}

	inline bool operator >(const Complex &z) const
	{
		return this->norm() > z.norm();
	}

	inline bool operator <(const Complex &z) const
	{
		return this->norm() < z.norm();
	}
private:
	T _real;
	T _imag;
};

typedef Complex<float>       Complexf;
typedef Complex<double>      Complexlf;
typedef Complex<long double> ComplexLf;

#endif /* COMPLEX_HH */
