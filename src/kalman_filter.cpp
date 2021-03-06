#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

/* 
 * Please note that the Eigen library does not initialize 
 *   VectorXd or MatrixXd objects with zeros upon creation.
 */

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  x_ = F_ * x_;
  P_ = F_ * P_ * F_.transpose() + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
   * TODO: update the state by using Kalman Filter equations
   */
  VectorXd y = z - H_ * x_ ;
  MatrixXd S = H_ * P_ * H_.transpose() + R_;
  MatrixXd K = P_ * H_.transpose() * S.inverse();
  
  x_ = x_ + (K * y);
  
  int xSize = x_.size();
  MatrixXd I = MatrixXd::Identity(xSize,xSize);
  P_ = (I - K * H_) * P_ ;
}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
   * TODO: update the state by using Extended Kalman Filter equations
   */
  double px = x_(0);
  double py = x_(1);
  double vx = x_(2);
  double vy = x_(3);
  
  double rho    = sqrt(px*px + py*py);
  double rhoDot = (px*vx + py*vy)/rho;
  double theta  = atan2(py,px);
  
//   VectorXd h = VectorXd(3);
  VectorXd zHat(3);
  zHat << rho, theta, rhoDot;
  
  VectorXd y = z-zHat;

  if( y[1] > (M_PI) )
    y[1] -= 2.f*(M_PI);
  if( y[1] < -(M_PI) )
    y[1] += 2.f*(M_PI);
  
  
  // all other steps follow the standard Kalman Filter algorithm
  MatrixXd S = H_ * P_ * H_.transpose() + R_;
  MatrixXd K = P_ * H_.transpose() * S.inverse();

  x_ = x_ + (K * y);
  
  int xSize = x_.size();
  MatrixXd I = MatrixXd::Identity(xSize,xSize);
  P_ = (I - K * H_) * P_ ;
  
}
