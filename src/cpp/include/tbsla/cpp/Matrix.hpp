#ifndef TBSLA_CPP_Matrix
#define TBSLA_CPP_Matrix

#include <fstream>

namespace tbsla { namespace cpp {

class Matrix {
  public:
    friend std::ostream & operator<<( std::ostream &os, const Matrix &m) { return m.print(os); };
    virtual double* spmv(double* v, int vect_incr = 0) const = 0;
    virtual inline void Ax(double* r, double* v, int vect_incr = 0) const = 0;
    double* a_axpx_(double* v, int vect_incr = 0) const;
    int get_n_row() const {return n_row;}
    int get_n_col() const {return n_col;}
    int get_f_row() const {return f_row;}
    int get_f_col() const {return f_col;}
    int get_ln_row() const {return ln_row;}
    int get_ln_col() const {return ln_col;}
    int get_pr() const {return pr;}
    int get_pc() const {return pc;}
    int get_NR() const {return NR;}
    int get_NC() const {return NC;}
    long int get_nnz() const {return nnz;};

    virtual std::ostream & write(std::ostream &os) = 0;
    virtual std::istream & read(std::istream &is, std::size_t pos = 0, std::size_t n = 1) = 0;
    virtual std::ostream& print(std::ostream& os) const = 0;
    virtual std::ostream& print_as_dense(std::ostream& os) = 0;

    virtual void NUMAinit() = 0;

    virtual void fill_cdiag(int n_row, int n_col, int cdiag, int pr = 0, int pc = 0, int NR = 1, int NC = 1) = 0;
    virtual void fill_cqmat(int n_row, int n_col, int c, double q, unsigned int seed_mult = 1, int pr = 0, int pc = 0, int NR = 1, int NC = 1) = 0;

  protected:
    int n_row, n_col, f_row, f_col, ln_row, ln_col, pr, pc, NR, NC;
    long int nnz;

};

struct MatrixFormatReadException : public std::exception {
   const char * what () const throw () {
      return "This Matrix format import is not implemented!";
   }
};

}}

#endif
