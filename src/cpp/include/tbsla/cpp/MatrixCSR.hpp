#ifndef TBSLA_CPP_MatrixCSR
#define TBSLA_CPP_MatrixCSR

#include <tbsla/cpp/Matrix.hpp>
#include <tbsla/cpp/MatrixCOO.hpp>
#include <iostream>

namespace tbsla { namespace cpp {

class MatrixCSR : public virtual Matrix {
  public:
    friend std::ostream & operator<<( std::ostream &os, const MatrixCSR &m);
    MatrixCSR(int n_row, int n_col, double* values, int* rowptr, int* colidx);
    MatrixCSR(const tbsla::cpp::MatrixCOO & m);
    MatrixCSR() : values(0), rowptr(0), colidx(0) {};
    ~MatrixCSR();
    double* spmv(double* v, int vect_incr = 0) const;
    inline void Ax(double* r, double* v, int vect_incr = 0) const;
    using tbsla::cpp::Matrix::a_axpx_;
    std::ostream & print_as_dense(std::ostream &os);
    std::ostream & write(std::ostream &os);
    std::istream & read(std::istream &is, std::size_t pos = 0, std::size_t n = 1);
    std::ostream& print(std::ostream& os) const;
    void NUMAinit();
    void fill_cdiag(int n_row, int n_col, int cdiag, int pr = 0, int pc = 0, int NR = 1, int NC = 1);
    void fill_cqmat(int n_row, int n_col, int c, double q, unsigned int seed_mult = 1, int pr = 0, int pc = 0, int NR = 1, int NC = 1);

  protected:
    double* values;
    int* rowptr;
    int* colidx;
};

}}

#endif
