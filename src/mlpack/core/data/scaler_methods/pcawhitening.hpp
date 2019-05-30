/**
 * @file pcawhitening.hpp
 * @author Jeffin Sam
 *
 * Whitening scaling to scale features, Using PCA Whitening.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_CORE_DATA_PCA_WHITENING_SCALE_HPP
#define MLPACK_CORE_DATA_PCA_WHITENING_SCALE_HPP

#include <mlpack/prereqs.hpp>
#include <mlpack/core/math/lin_alg.hpp>

namespace mlpack {
namespace data {

/**
 * A simple PcaWhitening class.
 *
 * Whitens a matrix using the eigendecomposition of the covariance matrix.
 * Whitening means the covariance matrix of the result is the identity matrix.
 * 
 * For whitening related formula and more info, check the link below.
 * http://ufldl.stanford.edu/tutorial/unsupervised/PCAWhitening/
 *
 * @code
 * arma::mat input;
 * Load("train.csv", input);
 * arma::mat output;
 *
 * // Scale the features.
 * PcaWhitening scale;
 * scale.Transform(input, output);
 *
 * // Retransform the input.
 * scale.InverseTransform(output, input);
 * @endcode
 */
class PcaWhitening
{
 public:
  /**
  * A constructor to set the regulatization parameter.
  *
  * @param eps Regularization parameter.
  */
  PcaWhitening(double eps = 0.00005)
  {
    epsilon = eps;
  }

  /**
  * Function for PCA whitening.
  *
  * @param input Dataset to scale features.
  * @param output Output matrix with whitened features.
  */
  template<typename MatType>
  void Transform(const MatType& input, MatType& output)
  {
    output.copy_size(input);
    itemMean = arma::mean(input, 1);
    output = (input.each_col() - itemMean);
    // Get eigenvectors and eigenvalues of covariance of input matrix.
    eig_sym(eigenValues, eigenVectors, mlpack::math::ColumnCovariance(output));
    for (size_t i = 0; i < eigenValues.n_elem; i++)
        eigenValues(i) = eigenValues(i) + epsilon;
    output = arma::diagmat(1.0 / (arma::sqrt(eigenValues))) * eigenVectors.t()
        * output;
  }

  /**
  * Function to retrieve original dataset.
  *
  * @param input Scaled dataset.
  * @param output Output matrix with original Dataset.
  */
  template<typename MatType>
  void InverseTransform(const MatType& input, MatType& output)
  {
    output = arma::diagmat(arma::sqrt(eigenValues)) * inv(eigenVectors.t())
        * input;
    output = (output.each_col() + itemMean);
  }

  //! Get the Mean row vector.
  const arma::vec& ItemMean() const { return itemMean; }
  //! Get the eigenvalues vector.
  const arma::vec& EigenValues() const { return eigenValues; }
  //! Get the eigenvector.
  const arma::mat& EigenVectors() const { return eigenVectors; }
  //! Get the Regularisation Parameter.
  const double& Epsilon() const { return epsilon; }

 private:
  // Vector which holds mean of each feature.
  arma::vec itemMean;
  // Mat which hold the eigenvectors.
  arma::mat eigenVectors;
  // Regularization Paramter.
  double epsilon;
  // Vector which hold the eigenvalues.
  arma::vec eigenValues;
}; // class PcaWhitening

} // namespace data
} // namespace mlpack

#endif
