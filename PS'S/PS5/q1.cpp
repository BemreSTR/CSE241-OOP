#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

class Polynomial{

    private:
    int degree; // Degree of the polynomial
    double* coeffs; // Coefficients array

public:
    Polynomial(); // creates an empty polynomial
    Polynomial(const Polynomial&);

    // The size of the coefficient array is degree of the polynomial + 1.
    Polynomial(double coefficient[], int size);
    ~Polynomial();

    //Use indexed polynomial as r-value to inspect coefficient
    //and as l-value to assign coefficient
    double& operator[](int degree);
    //This is required if we are to have const correctness
    const double& operator[](int degree) const;

    const Polynomial& operator=(const Polynomial & rhs);

    double eval(double arg);

    int mySize();

    //friend functions:
    friend double evaluate(const Polynomial& ploy, double arg);
    friend Polynomial operator+(const Polynomial& lsh, const Polynomial& rhs);
    friend Polynomial operator-(const Polynomial& lsh, const Polynomial& rhs);
    friend Polynomial operator*(const Polynomial& lsh, const Polynomial& rhs);


};

Polynomial::Polynomial(){
    degree = 0;
    coeffs = new double[1];
    coeffs[0] = 0;
}
Polynomial::Polynomial(const Polynomial& rhs){
    degree = rhs.degree;
    coeffs = new double[degree + 1];
    for (int i = 0; i <= degree; i++){
        coeffs[i] = rhs.coeffs[i];
    }
}
Polynomial::Polynomial(double coefficient[], int size){
    degree = size - 1;
    coeffs = new double[size];
    for (int i = 0; i < size; i++){
        coeffs[i] = coefficient[i];
    }
}
Polynomial::~Polynomial(){
    delete[] coeffs;
}
double& Polynomial::operator[](int degree){
    if (degree > this->degree){
        cout << "Degree out of range" << endl;
        exit(1);
    }
    return coeffs[degree];
}
const double& Polynomial::operator[](int degree) const{
    if (degree > this->degree){
        cout << "Degree out of range" << endl;
        exit(1);
    }
    return coeffs[degree];
}

const Polynomial& Polynomial::operator=(const Polynomial & rhs){
    if (this != &rhs){
        delete[] coeffs;
        degree = rhs.degree;
        coeffs = new double[degree + 1];
        for (int i = 0; i <= degree; i++){
            coeffs[i] = rhs.coeffs[i];
        }
    }
    return *this;
}

double Polynomial::eval(double arg){
    double result = 0;
    for (int i = 0; i <= degree; i++){
        result += coeffs[i] * pow(arg, i);
    }
    return result;
}

int Polynomial::mySize(){
    return degree + 1;
}

double evaluate(const Polynomial& ploy, double arg){
    double result = 0;
    for (int i = 0; i <= ploy.degree; i++){
        result += ploy.coeffs[i] * pow(arg, i);
    }
    return result;
}

Polynomial operator+(const Polynomial& lsh, const Polynomial& rhs){
    int maxDegree = max(lsh.degree, rhs.degree);
    double* resultCoeffs = new double[maxDegree + 1];
    for (int i = 0; i <= maxDegree; i++){
        resultCoeffs[i] = (i <= lsh.degree ? lsh.coeffs[i] : 0) + (i <= rhs.degree ? rhs.coeffs[i] : 0);
    }
    return Polynomial(resultCoeffs, maxDegree + 1);
}

Polynomial operator-(const Polynomial& lsh, const Polynomial& rhs){
    int maxDegree = max(lsh.degree, rhs.degree);
    double* resultCoeffs = new double[maxDegree + 1];
    for (int i = 0; i <= maxDegree; i++){
        resultCoeffs[i] = (i <= lsh.degree ? lsh.coeffs[i] : 0) - (i <= rhs.degree ? rhs.coeffs[i] : 0);
    }
    return Polynomial(resultCoeffs, maxDegree + 1);
}

Polynomial operator*(const Polynomial& lsh, const Polynomial& rhs){
    int resultDegree = lsh.degree + rhs.degree;
    double* resultCoeffs = new double[resultDegree + 1];
    for (int i = 0; i <= resultDegree; i++){
        resultCoeffs[i] = 0;
    }
    for (int i = 0; i <= lsh.degree; i++){
        for (int j = 0; j <= rhs.degree; j++){
            resultCoeffs[i + j] += lsh.coeffs[i] * rhs.coeffs[j];
        }
    }
    return Polynomial(resultCoeffs, resultDegree + 1);
}

int main() {
    Polynomial empty;
 
    double one[] = {1};
    Polynomial One(one, 1);
 
    double quad[] = {3, 2, 1};
    double cubic[] = {1, 2, 0, 3};
 
    Polynomial q(quad, 3);   // q is 3 + 2x + x^2
    Polynomial c(cubic, 4);  // c is 1 + 2x + 0x^2 + 3x^3
 
    Polynomial p = q;        // test copy constructor
    Polynomial r;
    r = q;                   // test operator=
    r = c;
 
    cout << "Polynomial q " << endl;
    for (int i = 0; i < 3; i++)
        cout << "term with degree " << i << " has coefficient " << q[i] << endl;
 
    cout << "Polynomial c " << endl;
    for (int i = 0; i < 4; i++)
        cout << "term with degree " << i << " has coefficient " << c[i] << endl;
 

    cout<< "value of q(2) is " << evaluate(q, 2) << endl
        << "value of p(2) is " << evaluate(p, 2) << endl
        << "value of r(2) is " << evaluate(r, 2) << endl
        << "value of c(2) is " << evaluate(c, 2) << endl;
 
    r = q + c;
    cout << "value of (q + c)(2) is " << evaluate(r, 2) << endl;
 
    r = q - c;
    cout << "value of (q - c)(2) is " << evaluate(r, 2) << endl;
 
    r = q * c;
    cout << "size of q*c is " << r.mySize() << endl;
    cout << "Polynomial r (= q*c) " << endl;
 
    for (int i = 0; i < r.mySize(); i++)
        cout << "term with degree " << i << " has coefficient " << r[i] << endl;
 
    cout << "value of (q * c)(2) is " << evaluate(r, 2) << endl;
 
    return 0;
}
