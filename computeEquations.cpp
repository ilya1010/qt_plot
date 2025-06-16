#include <iostream>
#include <string>
#include <cmath>
#include <numbers>
#include <vector>
#include <typeinfo>
#include <limits>

#include <Eigen\Eigenvalues> 
#include <Eigen\Dense>
#include <Eigen\Core>
#include <symbolicc++.h>

class MS {

private:
  double m_o;
  double m_yMe;
  double m_x;
  double m_lh;
  int m_nof;
  double m_lhor;

  #if 1
  Symbolic m_zr;
  Symbolic m_vov;
  #endif
  
  public:
  MS(double o,
              double yMe,
              double x,
              double lh,
              int nof
             ) 
  : m_o(o)
  , m_yMe(yMe)
  , m_x(x)
  , m_lh(lh)
  , m_nof(nof)
  
  , m_zr("m_zr")
  , m_vov("m_vov", m_nof)
  {
  }
  
  void DNF(int nDF) const;

  auto ACF();
  
  auto ZMPG();
  
  auto MMG();

  auto SMG();

  auto SGEP();

  auto FC();
  
};

auto MS::ACF() {
  
  Symbolic zr("zr", m_nof);
  Symbolic zc("zc");
  
  for (int i = 0; i != m_nof; ++i) {
    m_vov(i) = m_zr^(i+2);
  }
  return m_vov[m_zr];
}

auto MS::ZMPG() {
  Eigen::MatrixXd eZMP(m_nof, m_nof);
  
  for (int i = 0; i != m_nof; ++i) {
    for (int j = 0; j != m_nof; ++j) {
      eZMP(i,j) = 0;
    };
  }
  return eZMP;
}

auto MS::MMG() {
  
  Eigen::MatrixXd eMMP(m_nof, m_nof);
  Eigen::MatrixXd eMMF(2*m_nof, 2*m_nof);

  auto sMMP = (integrate(m_o * (MS::ACF())
  * (MS::ACF().transpose()), m_zr));

  for (int i=0; i != m_nof; ++i) {
    for (int j=0; j != m_nof; ++j) {
      eMMP(i, j) = sMMP[m_zr == m_lh].row(i).column(j);
    }
  }

  eMMF.block(0,0,m_nof,m_nof) =
  eMMP;
  eMMF.block(0,m_nof,m_nof,m_nof) =
  MS::ZMPG();
  eMMF.block(m_nof,0,m_nof,m_nof) =
  MS::ZMPG();
  eMMF.block(m_nof,m_nof,m_nof,m_nof) =
  eMMP;
  
  return eMMF;
}

auto MS::SMG() {
  Eigen::MatrixXd sMMP(m_nof, m_nof);
  Eigen::MatrixXd eSMF(2*m_nof, 2*m_nof);

  auto vVr_2 = (df(ACF(), m_zr, 2));
  auto symbolicStiffnessMatrixPart = (m_yMe * integrate((m_x * vVr_2 *
    (vVr_2.transpose())), m_zr));

  for (int i=0; i != m_nof; ++i) {
    for (int j=0; j != m_nof; ++j) {
      sMMP(i, j) = symbolicStiffnessMatrixPart[m_zr == m_lh].row(i).column(j);
    }
  }
   
  eSMF.block(0,0,m_nof,m_nof) =
  sMMP;
  eSMF.block(0,m_nof,m_nof,m_nof) =
  MS::ZMPG();
  eSMF.block(m_nof,0,m_nof,m_nof) =
  MS::ZMPG();
  eSMF.block(m_nof,m_nof,m_nof,m_nof) =
  sMMP;

  return eSMF;
}
  
  Eigen::GeneralizedEigenSolver<Eigen::MatrixXd> ges.compute(MS::SMG(), MS::MMG());

  auto MS::FC() {
    std::vector<double> fIHWS;
    for (size_t i = 0; i != 2*m_nof; ++i) {
      fIHWS.push_back(
        std::sqrt(ges.eigenvalues()[i].real())
      );
    }
    return fIHWS;
  }

int main() {
  MS ms(0.01, 1e5, 2.0e-5, 0.02, 18);
  ms.SGEP().eigenvectors().real().col(1)[1];
  ms.SGEP().eigenvalues()[7].real();
  ms.FC();
}
