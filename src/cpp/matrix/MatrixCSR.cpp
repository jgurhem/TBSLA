#include <tbsla/cpp/MatrixCSR.hpp>
#include <tbsla/cpp/utils/vector.hpp>
#include <tbsla/cpp/utils/cdiag.hpp>
#include <tbsla/cpp/utils/range.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

tbsla::cpp::MatrixCSR::MatrixCSR(int n_row, int n_col, std::vector<double> & values, std::vector<int> & rowptr,  std::vector<int> & colidx) {
  this->n_row = n_row;
  this->n_col = n_col;
  this->values = values;
  this->rowptr = rowptr;
  this->colidx = colidx;
}

std::ostream & tbsla::cpp::operator<<( std::ostream &os, const tbsla::cpp::MatrixCSR &m) {
  return m.print(os);
}

std::ostream& tbsla::cpp::MatrixCSR::print(std::ostream& os) const {
  os << "n_row : " << this->n_row << std::endl;
  os << "n_col : " << this->n_col << std::endl;
  os << "n_values : " << this->values.size() << std::endl;
  tbsla::utils::vector::streamvector<double>(os, "values", this->values);
  os << std::endl;
  tbsla::utils::vector::streamvector<int>(os, "rowptr", this->rowptr);
  os << std::endl;
  tbsla::utils::vector::streamvector<int>(os, "colidx", this->colidx);
  return os;
}

std::vector<double> tbsla::cpp::MatrixCSR::spmv(const std::vector<double> &v, int vect_incr) const {
  std::vector<double> r (this->n_row, 0);
  if (this->values.size() == 0)
    return r;
  for (int i = 0; i < this->rowptr.size() - 1; i++) {
    for (int j = this->rowptr[i] - this->rowptr.front(); j < this->rowptr[i + 1] - this->rowptr.front(); j++) {
       r[i + vect_incr] += this->values[j] * v[this->colidx[j]];
    }
  }
  return r;
}

std::ostream & tbsla::cpp::MatrixCSR::print_infos(std::ostream &os) {
  os << "-----------------" << std::endl;
  os << "------ CSR ------" << std::endl;
  os << "--- general   ---" << std::endl;
  os << "n_row : " << n_row << std::endl;
  os << "n_col : " << n_col << std::endl;
  os << "--- capacity  ---" << std::endl;
  os << "values : " << values.capacity() << std::endl;
  os << "rowptr : " << rowptr.capacity() << std::endl;
  os << "colidx : " << colidx.capacity() << std::endl;
  os << "--- size      ---" << std::endl;
  os << "values : " << values.size() << std::endl;
  os << "rowptr : " << rowptr.size() << std::endl;
  os << "colidx : " << colidx.size() << std::endl;
  os << "-----------------" << std::endl;
  return os;
}

std::ostream & tbsla::cpp::MatrixCSR::print_stats(std::ostream &os) {
  int s = 0, u = 0, d = 0;
  for (int i = 0; i < this->rowptr.size() - 1; i++) {
    for (int j = this->rowptr[i] - this->rowptr.front(); j < this->rowptr[i + 1] - this->rowptr.front(); j++) {
      if(i < this->colidx[j]) {
        s++;
      } else if(i > this->colidx[j]) {
        u++;
      } else {
        d++;
      }
    }
  }
  os << "upper values : " << u << std::endl;
  os << "lower values : " << s << std::endl;
  os << "diag  values : " << d << std::endl;
  return os;
}

std::ostream & tbsla::cpp::MatrixCSR::write(std::ostream &os) {
  os.write(reinterpret_cast<char*>(&this->n_row), sizeof(this->n_row));
  os.write(reinterpret_cast<char*>(&this->n_col), sizeof(this->n_col));

  size_t size_v = this->values.size();
  os.write(reinterpret_cast<char*>(&size_v), sizeof(size_v));
  os.write(reinterpret_cast<char*>(this->values.data()), this->values.size() * sizeof(double));

  size_t size_r = this->rowptr.size();
  os.write(reinterpret_cast<char*>(&size_r), sizeof(size_r));
  os.write(reinterpret_cast<char*>(this->rowptr.data()), this->rowptr.size() * sizeof(int));

  size_t size_c = this->colidx.size();
  os.write(reinterpret_cast<char*>(&size_c), sizeof(size_c));
  os.write(reinterpret_cast<char*>(this->colidx.data()), this->colidx.size() * sizeof(int));
  return os;
}

std::istream & tbsla::cpp::MatrixCSR::read(std::istream &is, std::size_t pos, std::size_t n) {
  is.read(reinterpret_cast<char*>(&this->n_row), sizeof(this->n_row));
  is.read(reinterpret_cast<char*>(&this->n_col), sizeof(this->n_col));

  size_t size;
  is.read(reinterpret_cast<char*>(&size), sizeof(size_t));
  this->values.resize(size);
  is.read(reinterpret_cast<char*>(this->values.data()), size * sizeof(double));

  is.read(reinterpret_cast<char*>(&size), sizeof(size_t));
  this->rowptr.resize(size);
  is.read(reinterpret_cast<char*>(this->rowptr.data()), size * sizeof(int));

  is.read(reinterpret_cast<char*>(&size), sizeof(size_t));
  this->colidx.resize(size);
  is.read(reinterpret_cast<char*>(this->colidx.data()), size * sizeof(int));
  return is;
}

void tbsla::cpp::MatrixCSR::fill_cdiag(int n_row, int n_col, int cdiag, int rp, int RN) {
  this->n_row = n_row;
  this->n_col = n_col;

  this->values.clear();
  this->colidx.clear();
  this->rowptr.clear();

  int nv = std::max(std::min(n_row, n_col - cdiag), 0) + std::max(std::min(n_row - cdiag, n_col), 0);
  if(cdiag == 0)
    nv /= 2;
  if(nv == 0)
    return;

  int s = tbsla::utils::range::pflv(n_row, rp, RN);
  int n = tbsla::utils::range::lnv(n_row, rp, RN);

  this->values.reserve(2 * n);
  this->colidx.reserve(2 * n);
  this->rowptr.reserve(n + 1);

  int i;
  size_t incr = 0;

  if(s < std::min(cdiag, n_row)) {
    incr += std::max(std::min(n_col - cdiag, s), 0);
  } else if(s < std::min(n_row, n_col - cdiag)) {
    incr += std::max(std::min(n_col - cdiag, cdiag), 0);
    incr += (cdiag == 0 ? 1 : 2) * (s - std::min(n_col - cdiag, cdiag));
  } else {
    incr += std::max(std::min(n_col - cdiag, cdiag), 0);
    incr += (cdiag == 0 ? 1 : 2) * (std::max(n_col - 2 * cdiag, 0));
    incr += s - (n_col - cdiag) - (cdiag - std::min(n_col - cdiag, cdiag));
  }
  this->rowptr.push_back(incr);

  for(i = s; i < std::min( {cdiag, n_row, s + n} ); i++) {
    if(i < n_col - cdiag) {
      auto curr = tbsla::utils::cdiag::cdiag_value(incr, nv, n_row, n_col, cdiag);
      this->colidx.push_back(std::get<1>(curr));
      this->values.push_back(std::get<2>(curr));
      incr++;
    }
    this->rowptr.push_back(incr);
  }
  for(; i < std::min( {n_row, n_col - cdiag, s + n} ); i++) {
    if(cdiag == 0) {
      auto curr = tbsla::utils::cdiag::cdiag_value(incr, nv, n_row, n_col, cdiag);
      this->colidx.push_back(std::get<1>(curr));
      this->values.push_back(std::get<2>(curr));
      incr++;
    } else {
      auto curr = tbsla::utils::cdiag::cdiag_value(incr, nv, n_row, n_col, cdiag);
      this->colidx.push_back(std::get<1>(curr));
      this->values.push_back(std::get<2>(curr));
      incr++;
      curr = tbsla::utils::cdiag::cdiag_value(incr, nv, n_row, n_col, cdiag);
      this->colidx.push_back(std::get<1>(curr));
      this->values.push_back(std::get<2>(curr));
      incr++;
    }
    this->rowptr.push_back(incr);
  }
  for(; i < std::min({n_row, s + n}); i++) {
    if(i < n_col + cdiag) {
      auto curr = tbsla::utils::cdiag::cdiag_value(incr, nv, n_row, n_col, cdiag);
      this->colidx.push_back(std::get<1>(curr));
      this->values.push_back(std::get<2>(curr));
      incr++;
    }
    this->rowptr.push_back(incr);
  }
}
